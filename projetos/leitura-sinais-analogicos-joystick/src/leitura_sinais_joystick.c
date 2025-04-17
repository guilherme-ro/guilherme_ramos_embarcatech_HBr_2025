#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/adc.h" 
#include "ssd1306/ssd1306.h"  

// Definição dos pinos usados para o joystick e LEDs
#define VRY 26          // Pino de leitura do eixo Y do joystick (conectado ao ADC)
#define VRX 27          // Pino de leitura do eixo X do joystick (conectado ao ADC)
#define ADC_CHANNEL_0 0 // Canal ADC para o eixo X do joystick
#define ADC_CHANNEL_1 1 // Canal ADC para o eixo Y do joystick
#define SW 22           // Pino de leitura do botão do joystick

ssd1306_t disp; // Objeto para o display SSD1306

// assinatura das funcoes
void setup_joystick();
void joystick_play();
void joystick_read_axis(uint16_t *, uint16_t *);
void joystick_exec();
void setup_display();
void message_on_display(char *, uint32_t, uint32_t, bool);

char buffer1[32];
char buffer2[32];

int main() {

    stdio_init_all();

    // Delay para o usuário abrir o monitor serial...
    sleep_ms(5000);

    setup_display();

    // Joystick
    joystick_exec(); 
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

// Função de configuração geral
void joystick_play()
{
    stdio_init_all();                                // Inicializa a porta serial para saída de dados
    setup_joystick();                                // Chama a função de configuração do joystick
}

// Função para ler os valores dos eixos do joystick (X e Y)
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value)
{
    // Leitura do valor do eixo X do joystick
    adc_select_input(ADC_CHANNEL_1); // Seleciona o canal ADC para o eixo X
    sleep_us(2);                     // Pequeno delay para estabilidade
    *vrx_value = adc_read();         // Lê o valor do eixo X (0-4095)

    // Leitura do valor do eixo Y do joystick 
    adc_select_input(ADC_CHANNEL_0); // Seleciona o canal ADC para o eixo Y
    sleep_us(2);                     // Pequeno delay para estabilidade
    *vry_value = adc_read();         // Lê o valor do eixo Y (0-4095)


}

void joystick_exec() {
    uint16_t vrx_value, vry_value, sw_value; // Variáveis para armazenar os valores do joystick (eixos X e Y) e botão

    joystick_play();
    while (true) {
        
        joystick_read_axis(&vrx_value, &vry_value); // Lê os valores dos eixos do joystick

        snprintf(buffer1, sizeof(buffer1), "x: %d", vrx_value);
        snprintf(buffer2, sizeof(buffer2), "y: %d", vry_value);

        ssd1306_clear(&disp);
        ssd1306_draw_string(&disp, 0, 0, 1, buffer1);       
        ssd1306_draw_string(&disp, 0, 10, 1, buffer2);      
        ssd1306_show(&disp);
            
        sleep_ms(100); // Pequeno delay para evitar leituras muito frequentes
    }
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

// Função para mostrar uma mensagem
void message_on_display(char *str, uint32_t x, uint32_t y, bool should_clear) {
    if (should_clear) {
        ssd1306_clear(&disp);
    }
    sleep_ms(50);
    ssd1306_draw_string(&disp, x, y, 1, str);
    ssd1306_show(&disp);
}


