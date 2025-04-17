# Contador Decrescente com interrupção atuando nos botões
Desenvolvimento de um programa em linguagem C com pico SDK que implementa um contador decrescente controlado por interrupção.

### Hardware:

- Placa BitDogLab com o microcontrolador Raspberry Pi Pico W embutido.
- Display OLED compatível.
- Matriz de 25 LEDs RGB. (não utilizada)
- LED RGB isolado. (não utilizado)
- Microfone integrado. (não utilizado)
- Joystick analógico. (não utilizado)
- Botões A e B.
- Buzzer A e Buzzer B. (não utilizado)
- Módulo Wi-Fi. (não utilizado)

### Software:

- Código desenvolvido em linguagem C com Pico SDK.
- Implementação modular e estruturada do código.

### Comportamento do Programa:

Toda vez que o Botão A (GPIO5) é pressionado:

O contador decrescente reinicia em 9 e o valor da contagem é mostrado no display OLED.
O sistema entra em modo de contagem regressiva ativa, decrementando o contador de 1 em 1 a cada segundo até chegar em zero.
Durante essa contagem (ou seja, de 9 até 0), o programa deve registrar quantas vezes o Botão B (GPIO6) foi pressionado. O valor deste registro de eventos de botão pressionado também deve ser mostrado no display OLED.
Quando o contador atingir zero, o sistema congela e ignora temporariamente os cliques no Botão B (eles não devem ser acumulados fora do intervalo ativo).

O sistema permanece parado após a contagem, exibindo:

O valor 0 no contador
A quantidade final de cliques no Botão B registrados durante o período de 9 segundo (contagem regressiva)

Somente ao pressionar novamente o Botão A, o processo todo se reinicia:

O contador volta para 9
O número de cliques do Botão B é zerado
A contagem recomeça do início

### Tratamento de Debouncing:

Há mais cliques do que realmente realizados quando o botão B é clicado. Por que isso acontece?

Esse problema é comum em botões físicos e chamado de efeito bouncing (ou "rebote mecânico"). Quando o botão é pressionado, os contatos internos não fecham ou não abrem de forma imediata e limpa, havendo oscilação por alguns milissegundos e gerando vários pulsos rápidos. O microncontrolador interpreta isso como vários cliques, mesmo tendo pressionado o botão somente uma vez.

Uma possível solução por Software é adicionar um atraso (delay) da interrupção do botão B, desabilitando ela por alguns milisegundos.

Pode-se usar o tipo absolute_time_t da biblioteca time.h do Pico SDK para criar variáveis de controle de tempo para desabilitar a interrupção por tempo determinado, sendo essas variáveis analisadas por absolute_time_diff_us(last_pressingB, pressingB_now) > 200000.

```
absolute_time_t ultimo_clique_b = {0}; //variavel global

// funcao de callback para a interrupção
void gpio_callback(uint gpio, uint32_t events) {
    absolute_time_t agora = get_absolute_time();

    if (gpio == BUTTON_PIN_A && (events & GPIO_IRQ_EDGE_FALL)) {
        counter = 9;     
        pressing_B = 0;    
        counting_mode = 1;   
    }

    if (gpio == BUTTON_PIN_B && (events & GPIO_IRQ_EDGE_FALL)) {
        // Debounce - aceita apenas se passou pelo menos 200ms
        if (counting_mode && absolute_time_diff_us(ultimo_clique_b, agora) > 200000) {
            pressing_B++;
            ultimo_clique_b = agora;
        }
    }
}
```

### Imagem do Projeto
<p align="center">
  <img src="https://github.com/guilherme-ro/guilherme_ramos_embarcatech_HBr_2025/raw/main/projetos/contador-decrescente-bitdoglab/assets/contador-decrescente.jpg" width="400" height="500" alt="Imagem centralizada">
</p>




### Como Compilar no Linux:

cd ~pico/projeto

mkdir build

cd build

cmake ..

make

export PICO_SDK_PATH=/home/$USER/pico/pico-sdk 
(adapte conforme necessário)

cp nome_arquivo_c.uf2 /media/$USER/RPI-RP2/

