#include "pico/cyw43_arch.h"
#include "pico/stdlib.h"
#include "lwip/tcp.h"
#include <string.h>
#include <stdio.h>
#include <math.h>
#include "hardware/adc.h"
#include "hardware/dma.h"
#include "hardware/pwm.h" 
#include "hardware/i2c.h"  
#include "hardware/clocks.h"
#include "ssd1306/ssd1306.h"  
#include "neopixel.c"


// Definição dos pinos usados para o joystick e LEDs
#define VRX 26          // Pino de leitura do eixo X do joystick (conectado ao ADC)
#define VRY 27          // Pino de leitura do eixo Y do joystick (conectado ao ADC)
#define ADC_CHANNEL_0 0 // Canal ADC para o eixo X do joystick
#define ADC_CHANNEL_1 1 // Canal ADC para o eixo Y do joystick
#define SW 22           // Pino de leitura do botão do joystick
#define LED_G 11                    // Pino para controle do LED verde via PWM
#define LED_B 12                    // Pino para controle do LED azul via PWM
#define LED_R 13                    // Pino para controle do LED vermelho via PWM

#define WIFI_SSID "nome_rede"  // Substitua pelo nome da sua rede Wi-Fi
#define WIFI_PASS "senha" // Substitua pela senha da sua rede Wi-Fi

// Buffer para respostas HTTP
#define HTTP_RESPONSE "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n" \
                      "<!DOCTYPE html><html><body>" \
                      "<h1>Controle do LED</h1>" \
                      "<p><a href=\"/led/on\">Controle Remoto dos Componentes da BitDogLab</a></p>" \
                      "<p><a href=\"/led/off\">Desligar LED</a></p>" \
                      "<p><a href=\"/botao-a/\">Clique para Apertar o botao A</a></p>" \
                      "<p><a href=\"/botao-b/\">Clique para Apertar o botao B</a></p>" \
                      "<p><a href=\"/som/\">Clique aqui e aproxime um som ao microfone da BitDogLab</a></p>" \
                      "<p><a href=\"/joystick/\">Clique aqui e mexa o Joystick</a></p>" \
                      "</body></html>\r\n"

#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6
#define BUZZER_PIN 21 // Configuração do pino do buzzer
#define BUZZER_FREQUENCY 100 // Configuração da frequência do buzzer (em Hz)

const float DIVIDER_PWM = 16.0;          // Divisor fracional do clock para o PWM
const uint16_t PERIOD = 4096;            // Período do PWM (valor máximo do contador)
uint16_t led_g_level, led_r_level = 100; // Inicialização dos níveis de PWM para os LEDs
uint slice_led_g, slice_led_r;           // Variáveis para armazenar os slices de PWM correspondentes aos LEDs

// Pino e canal do microfone no ADC.
#define MIC_CHANNEL 2
#define MIC_PIN (26 + MIC_CHANNEL)

// Parâmetros e macros do ADC.
#define ADC_CLOCK_DIV 96.f
#define SAMPLES 200 // Número de amostras que serão feitas do ADC.
#define ADC_ADJUST(x) (x * 3.3f / (1 << 12u) - 1.65f) // Ajuste do valor do ADC para Volts.
#define ADC_MAX 3.3f
#define ADC_STEP (3.3f/5.f) // Intervalos de volume do microfone.

// Pino e número de LEDs da matriz de LEDs.
#define LED_PIN 7
#define LED_COUNT 25

#define abs(x) ((x < 0) ? (-x) : (x))

ssd1306_t disp; // Objeto para o display SSD1306

// Canal e configurações do DMA
uint dma_channel;
dma_channel_config dma_cfg;

// Buffer de amostras do ADC.
uint16_t adc_buffer[SAMPLES];

// Variáveis para armazenar o volume e a frequência.
float frequency = 0.0;
float volume = 0.0;

// assinatura das funcoes
void setup_joystick();
void setup_pwm_led(uint, uint *, uint16_t);
void joystick_play();
void joystick_read_axis(uint16_t *, uint16_t *);
void setup_buzzer_and_button();
void pwm_init_buzzer(uint);
void beep(uint, uint);
void setup_adc();
void setup_dma();
void setup_display();
void leds_matrix_changed_by_mic(uint);
void message_on_display(char *, uint32_t, uint32_t, bool);
void sample_mic();
float mic_power();
uint8_t get_intensity(float);
float calculate_frequency();
void get_volume();
static err_t http_callback(void *, struct tcp_pcb *, struct pbuf *, err_t err);
static err_t connection_callback(void *, struct tcp_pcb *, err_t err);
static void start_http_server(void);
void botoes_exec(char *buffer);
void microfone_exec();
void joystick_exec();
void wifi_exec(char *buffer);

int main() {
  char buffer[64];

  setup_buzzer_and_button();

  stdio_init_all();

  // Delay para o usuário abrir o monitor serial...
  sleep_ms(5000);

  // Preparação/setup da matriz de LEDs.
  printf("Preparando NeoPixel...");
  npInit(LED_PIN, LED_COUNT);

  setup_display();

  setup_adc();
  setup_dma();

  //botao A e botao B
  botoes_exec(buffer);

  // Microfone
  snprintf(buffer, sizeof(buffer), "Aproxime um som");
  message_on_display(buffer, 0, 0, true);
  sleep_ms(2000);
  microfone_exec();

  // Joystick
  snprintf(buffer, sizeof(buffer), "Mexa o joystick");
  message_on_display(buffer, 0, 0, true);
  joystick_exec(); 

  //Wifi
  snprintf(buffer, sizeof(buffer), "Configurando Wifi...");
  message_on_display(buffer, 0, 0, true);
  wifi_exec(buffer);
}

// Função para configurar o joystick (pinos de leitura e ADC)
void setup_joystick()
{
  // Inicializa o ADC e os pinos de entrada analógica
  adc_init();         // Inicializa o módulo ADC
  adc_gpio_init(VRX); // Configura o pino VRX (eixo X) para entrada ADC
  adc_gpio_init(VRY); // Configura o pino VRY (eixo Y) para entrada ADC

  // Inicializa o pino do botão do joystick
  gpio_init(SW);             // Inicializa o pino do botão
  gpio_set_dir(SW, GPIO_IN); // Configura o pino do botão como entrada
  gpio_pull_up(SW);          // Ativa o pull-up no pino do botão para evitar flutuações
}

// Função para configurar o PWM de um LED (genérica para verde e vermelho)
void setup_pwm_led(uint led, uint *slice, uint16_t level)
{
  gpio_set_function(led, GPIO_FUNC_PWM); // Configura o pino do LED como saída PWM
  *slice = pwm_gpio_to_slice_num(led);   // Obtém o slice do PWM associado ao pino do LED
  pwm_set_clkdiv(*slice, DIVIDER_PWM);   // Define o divisor de clock do PWM
  pwm_set_wrap(*slice, PERIOD);          // Configura o valor máximo do contador (período do PWM)
  pwm_set_gpio_level(led, level);        // Define o nível inicial do PWM para o LED
  pwm_set_enabled(*slice, true);         // Habilita o PWM no slice correspondente ao LED
}

// Função de configuração geral
void joystick_play()
{
  stdio_init_all();                                // Inicializa a porta serial para saída de dados
  setup_joystick();                                // Chama a função de configuração do joystick
  setup_pwm_led(LED_G, &slice_led_g, led_g_level); // Configura o PWM para o LED verde
  setup_pwm_led(LED_R, &slice_led_r, led_r_level); // Configura o PWM para o LED vermelho
}

// Função para ler os valores dos eixos do joystick (X e Y)
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
  // Leitura do valor do eixo X do joystick
  adc_select_input(ADC_CHANNEL_0); // Seleciona o canal ADC para o eixo X
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vrx_value = adc_read();         // Lê o valor do eixo X (0-4095)

  // Leitura do valor do eixo Y do joystick
  adc_select_input(ADC_CHANNEL_1); // Seleciona o canal ADC para o eixo Y
  sleep_us(2);                     // Pequeno delay para estabilidade
  *vry_value = adc_read();         // Lê o valor do eixo Y (0-4095)
}

void setup_buzzer_and_button() {
  // buzzer e botao
  gpio_init(BUTTON_PIN_A);
  gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
  gpio_pull_up(BUTTON_PIN_A);

  gpio_init(BUTTON_PIN_B);
  gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
  gpio_pull_up(BUTTON_PIN_B);

  // Configuração do GPIO para o buzzer como saída
  gpio_init(BUZZER_PIN);
  gpio_set_dir(BUZZER_PIN, GPIO_OUT);
  // Inicializar o PWM no pino do buzzer
  pwm_init_buzzer(BUZZER_PIN);
}

// Definição de uma função para inicializar o PWM no pino do buzzer
void pwm_init_buzzer(uint pin) {
    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096)); // Divisor de clock
    pwm_init(slice_num, &config, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pin, 0);
}

// Definição de uma função para emitir um beep com duração especificada
void beep(uint pin, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin, 2048);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

void setup_adc() {
  // Preparação do ADC.
  printf("Preparando ADC...\n");

  adc_gpio_init(MIC_PIN);
  adc_init();
  adc_select_input(MIC_CHANNEL);

  adc_fifo_setup(
    true, // Habilitar FIFO
    true, // Habilitar request de dados do DMA
    1, // Threshold para ativar request DMA é 1 leitura do ADC
    false, // Não usar bit de erro
    false // Não fazer downscale das amostras para 8-bits, manter 12-bits.
  );

  adc_set_clkdiv(ADC_CLOCK_DIV);

  printf("ADC Configurado!\n\n");
}

void setup_dma() {
  printf("Preparando DMA...");

  // Tomando posse de canal do DMA.
  dma_channel = dma_claim_unused_channel(true);

  // Configurações do DMA.
  dma_cfg = dma_channel_get_default_config(dma_channel);

  channel_config_set_transfer_data_size(&dma_cfg, DMA_SIZE_16); // Tamanho da transferência é 16-bits (usamos uint16_t para armazenar valores do ADC)
  channel_config_set_read_increment(&dma_cfg, false); // Desabilita incremento do ponteiro de leitura (lemos de um único registrador)
  channel_config_set_write_increment(&dma_cfg, true); // Habilita incremento do ponteiro de escrita (escrevemos em um array/buffer)
  
  channel_config_set_dreq(&dma_cfg, DREQ_ADC); // Usamos a requisição de dados do ADC

  // Amostragem de teste.
  printf("Amostragem de teste...\n");
  sample_mic();

  printf("Configuracoes completas!\n");
}

// Função para configurar o display
void setup_display() {
    i2c_init(i2c1, 400000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, i2c1);
    ssd1306_clear(&disp);
}

void leds_matrix_changed_by_mic(uint intensity) {
    // A depender da intensidade do som, acende LEDs específicos.
    switch (intensity) {
      case 0: break; // Se o som for muito baixo, não acende nada.
      case 1:
        npSetLED(12, 0, 0, 0); // Acende apenas o centro.
        break;
      case 2:
        npSetLED(12, 0, 0, 0); // Acende o centro.

        // Primeiro anel.
        npSetLED(7, 0, 0, 80);
        npSetLED(11, 0, 0, 80);
        npSetLED(13, 0, 0, 80);
        npSetLED(17, 0, 0, 80);
        break;
      case 3:
        // Centro.
        npSetLED(12, 0, 0, 0);
        
        // Primeiro anel.
        npSetLED(7, 120, 0, 0);
        npSetLED(11, 120, 0, 0);
        npSetLED(13, 120, 0, 0);
        npSetLED(17, 120, 0, 0);

        // Segundo anel.
        npSetLED(2, 0, 0, 120);
        npSetLED(6, 0, 0, 120);
        npSetLED(8, 0, 0, 120);
        npSetLED(10, 0, 0, 120);
        npSetLED(14, 0, 0, 120);
        npSetLED(16, 0, 0, 120);
        npSetLED(18, 0, 0, 120);
        npSetLED(22, 0, 0, 120);
        break;
      case 4:
        // Centro.
        npSetLED(12, 255, 210, 0);

        // Primeiro anel.
        npSetLED(7, 255, 0, 0);
        npSetLED(11, 255, 0, 0);
        npSetLED(13, 255, 0, 0);
        npSetLED(17, 255, 0, 0);

        // Segundo anel.
        npSetLED(2, 255, 0, 0);
        npSetLED(6, 255, 0, 0);
        npSetLED(8, 255, 0, 0);
        npSetLED(10, 255, 0, 0);
        npSetLED(14, 255, 0, 0);
        npSetLED(16, 255, 0, 0);
        npSetLED(18, 255, 0, 0);
        npSetLED(22, 255, 0, 0);

        // Terceiro anel.
        npSetLED(1, 255, 0, 0);
        npSetLED(3, 255, 0, 0);
        npSetLED(5, 255, 0, 0);
        npSetLED(9, 255, 0, 0);
        npSetLED(15, 255, 0, 0);
        npSetLED(19, 255, 0, 0);
        npSetLED(21, 255, 0, 0);
        npSetLED(23, 255, 0, 0); 
        break;
    }
}

// Função para mostrar uma mensagem
void message_on_display(char *str, uint32_t x, uint32_t y, bool should_clear) {
    if (should_clear) {
        ssd1306_clear(&disp);
    }
    sleep_ms(50);
    ssd1306_draw_string(&disp, x, y, 1, str);
    ssd1306_show(&disp);
}

// Funções para sample e cálculo de potência e frequência.
/**
 * Realiza as leituras do ADC e armazena os valores no buffer.
 */
void sample_mic() {
  adc_fifo_drain(); // Limpa o FIFO do ADC.
  adc_run(false); // Desliga o ADC (se estiver ligado) para configurar o DMA.

  dma_channel_configure(dma_channel, &dma_cfg,
    adc_buffer, // Escreve no buffer.
    &(adc_hw->fifo), // Lê do ADC.
    SAMPLES, // Faz SAMPLES amostras.
    true // Liga o DMA.
  );

  // Liga o ADC e espera acabar a leitura.
  adc_run(true);
  dma_channel_wait_for_finish_blocking(dma_channel);
  
  // Acabou a leitura, desliga o ADC de novo.
  adc_run(false);
}


// Calcula a potência média das leituras do ADC - valor RMS
float mic_power() {
  float avg = 0.f;

  for (uint i = 0; i < SAMPLES; ++i)
    avg += adc_buffer[i] * adc_buffer[i];
  
  avg /= SAMPLES;
  return sqrt(avg);
}


// Calcula a intensidade do volume registrado no microfone, de 0 a 4, usando a tensão.
uint8_t get_intensity(float v) {
  uint count = 0;

  while ((v -= ADC_STEP/20) > 0.f)
    ++count;
  
  return count;
}

void get_volume() {
    // Pega a potência média da amostragem do microfone.
    volume = mic_power();
    volume = 2.f * abs(ADC_ADJUST(volume)); // Ajusta para intervalo de 0 a 3.3V. (apenas magnitude, sem sinal)

    // Exibe o volume no display.
    char buffer[64];
    static int volume_anterior = -1;

    int volume_atual = (int)(volume * 100);
    if (volume_atual > 100) {
        volume_atual = 100;
    }
    if (volume_atual <= 2) {
        volume_atual = 0;
    }
    if (volume_atual != volume_anterior) {
        snprintf(buffer, 64, "Volume: %d %%", volume_atual);
        message_on_display(buffer, 0, 0, true);
        volume_anterior = volume_atual;
        sleep_ms(10);
    }
}

// Função de callback para processar requisições HTTP
static err_t http_callback(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err) {
    char buffer[64];

    if (p == NULL) {
        // Cliente fechou a conexão
        tcp_close(tpcb);
        return ERR_OK;
    }

    // Processa a requisição HTTP
    char *request = (char *)p->payload;

    if (strstr(request, "GET /led/on")) {
        pwm_set_gpio_level(LED_G, 0); 
        pwm_set_gpio_level(LED_R, 0); 
        ssd1306_clear(&disp);
        npClear();
        gpio_put(LED_B, 1);  // Liga o LED
    } 
    if (strstr(request, "GET /led/off")) {
        pwm_set_gpio_level(LED_G, 0); 
        pwm_set_gpio_level(LED_R, 0); 
        ssd1306_clear(&disp);
        npClear();
        gpio_put(LED_B, 0);  // Desliga o LED
    }
    if (strstr(request, "GET /botao-a/")) {
      ssd1306_clear(&disp);
      pwm_set_gpio_level(LED_G, 0); 
      pwm_set_gpio_level(LED_R, 0); 
      gpio_put(LED_B, 0); 
      npClear();
      snprintf(buffer, sizeof(buffer), "Aperte o botao A");
      message_on_display(buffer, 0, 0, true);
      while (gpio_get(BUTTON_PIN_A)); // Espera o botão ser pressionado
      beep(BUZZER_PIN, 500); // Emite som por 500ms
      ssd1306_clear(&disp);
    }
    if (strstr(request, "GET /botao-b/")) {
      ssd1306_clear(&disp);
      pwm_set_gpio_level(LED_G, 0); 
      pwm_set_gpio_level(LED_R, 0); 
      gpio_put(LED_B, 0); 
      npClear();
      snprintf(buffer, 64, "Aperte o botao B");
      message_on_display(buffer, 0, 0, true);
      while (gpio_get(BUTTON_PIN_B)); // Espera o botão ser pressionado
      beep(BUZZER_PIN, 500); // Emite som por 500ms
      ssd1306_clear(&disp);
    }
    if (strstr(request, "GET /som/")) {
      ssd1306_clear(&disp);
      pwm_set_gpio_level(LED_G, 0); 
      pwm_set_gpio_level(LED_R, 0); 
      gpio_put(LED_B, 0); 
      npClear();
      microfone_exec();
      ssd1306_clear(&disp);
    }
    if (strstr(request, "GET /joystick/")) {
      ssd1306_clear(&disp);
      gpio_put(LED_B, 0); 
      npClear();
      joystick_exec();
      ssd1306_clear(&disp);
    }


    // Envia a resposta HTTP
    tcp_write(tpcb, HTTP_RESPONSE, strlen(HTTP_RESPONSE), TCP_WRITE_FLAG_COPY);

    // Libera o buffer recebido
    pbuf_free(p);

    return ERR_OK;
}

// Callback de conexão: associa o http_callback à conexão
static err_t connection_callback(void *arg, struct tcp_pcb *newpcb, err_t err) {
    tcp_recv(newpcb, http_callback);  // Associa o callback HTTP
    return ERR_OK;
}

// Função de setup do servidor TCP
static void start_http_server(void) {
    struct tcp_pcb *pcb = tcp_new();
    if (!pcb) {
        printf("Erro ao criar PCB\n");
        return;
    }

    // Liga o servidor na porta 80
    if (tcp_bind(pcb, IP_ADDR_ANY, 80) != ERR_OK) {
        printf("Erro ao ligar o servidor na porta 80\n");
        return;
    }

    pcb = tcp_listen(pcb);  // Coloca o PCB em modo de escuta
    tcp_accept(pcb, connection_callback);  // Associa o callback de conexão

    printf("Servidor HTTP rodando na porta 80...\n");
}

void botoes_exec(char *buffer) {
    // Botão A
    snprintf(buffer, 64, "Aperte o botao A");
    message_on_display(buffer, 0, 0, true);
    while (gpio_get(BUTTON_PIN_A)); // Espera o botão ser pressionado
    beep(BUZZER_PIN, 500); // Emite som por 500ms
  
    // Botão B
    snprintf(buffer, 64, "Aperte o botao B");
    message_on_display(buffer, 0, 0, true);
    while (gpio_get(BUTTON_PIN_B)); // Espera o botão ser pressionado
    beep(BUZZER_PIN, 500); // Emite som por 500ms
}

void microfone_exec() {
  while (true) {
    // Realiza uma amostragem do microfone.
    sample_mic();

    get_volume();

    uint intensity = get_intensity(volume); // Calcula intensidade a ser mostrada na matriz de LEDs.

    // Limpa a matriz de LEDs.
    npClear();

    leds_matrix_changed_by_mic(intensity);

    // Atualiza a matriz.
    npWrite();

    // Envia a intensidade e a média das leituras do ADC por serial.
    printf("%2d %8.4f %8.4f\r", intensity, volume, frequency);

    if (gpio_get(BUTTON_PIN_A) == 0) { // Botão A pressionado, sair do loop
      npClear();
      break;
    }
  } 
}

void joystick_exec() {
  uint16_t vrx_value, vry_value, sw_value; // Variáveis para armazenar os valores do joystick (eixos X e Y) e botão
  printf("Joystick-PWM\n");                // Exibe uma mensagem inicial via porta serial

  joystick_play();
  while (true) {
      
    joystick_read_axis(&vrx_value, &vry_value); // Lê os valores dos eixos do joystick
    // Ajusta os níveis PWM dos LEDs de acordo com os valores do joystick
    pwm_set_gpio_level(LED_G, vrx_value); // Ajusta o brilho do LED verde com o valor do eixo X
    pwm_set_gpio_level(LED_R, vry_value); // Ajusta o brilho do LED vermelho com o valor do eixo Y


      
    if (gpio_get(SW) == 0) { // Botão do joystick pressionado, sair do loop
      //snprintf(buffer, 64, "Fim");
      //message_on_display(buffer, 0, 0, true);
      pwm_set_gpio_level(LED_G, 0); 
      pwm_set_gpio_level(LED_R, 0); 
      break;
    }
      
    sleep_ms(100); // Pequeno delay para evitar leituras muito frequentes
  }
}

void wifi_exec(char *buffer) {
  printf("Iniciando servidor HTTP\n");

  // Inicializa o Wi-Fi
  if (cyw43_arch_init()) {
      printf("Erro ao inicializar o Wi-Fi\n");
      snprintf(buffer, 64, "Erro inic WiFi");
      message_on_display(buffer, 0, 0, true);
      sleep_ms(2000);
      return;
  }

  cyw43_arch_enable_sta_mode();
  printf("Conectando ao Wi-Fi...\n");

  if (cyw43_arch_wifi_connect_timeout_ms(WIFI_SSID, WIFI_PASS, CYW43_AUTH_WPA2_AES_PSK, 10000)) {
      printf("Falha ao conectar ao Wi-Fi\n");
      snprintf(buffer, 64, "Falha inic WiFi");
      message_on_display(buffer, 0, 0, true);
      sleep_ms(2000);
      return;
  }else {
      printf("Connected.\n");
      // Read the ip address in a human readable way
      uint8_t *ip_address = (uint8_t*)&(cyw43_state.netif[0].ip_addr.addr);
      snprintf(buffer, 64, "IP %d.%d.%d.%d\n", 
        ip_address[0], ip_address[1], ip_address[2], ip_address[3]);
      message_on_display(buffer, 0, 0, true);
      sleep_ms(10000);
  }

  printf("Wi-Fi conectado!\n");
  printf("Para ligar ou desligar o LED acesse o Endereço IP seguido de /led/on ou /led/off\n");

  // Configura o LED como saída
  gpio_init(LED_B);
  gpio_set_dir(LED_B, GPIO_OUT);

  // Inicia o servidor HTTP
  start_http_server();
  
  // Loop principal
  while (true) {
      cyw43_arch_poll();  // Necessário para manter o Wi-Fi ativo
      cyw43_arch_enable_sta_mode();
      cyw43_wifi_pm(&cyw43_state, 0);

      //sleep_ms(100);
  }

  cyw43_arch_deinit();  // Desliga o Wi-Fi (não será chamado, pois o loop é infinito)
}
