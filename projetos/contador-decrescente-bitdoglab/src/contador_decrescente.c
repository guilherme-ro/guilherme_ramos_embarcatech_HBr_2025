/*

O tratamento de debouncing é descrito no Readme.md

*/

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "ssd1306/ssd1306.h"  

#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6

ssd1306_t disp; // Objeto para o display SSD1306

// assinatura das funcoes
void setup_button();
void setup_display();
void gpio_callback(uint, uint32_t);

int counter = 0;
int pressing_B = 0;
int counting_mode = 0;

void main(void) {
    char buffer1[32];
    char buffer2[32];

    setup_button();
    stdio_init_all();
    sleep_ms(5000);
    setup_display();

    while(1) {

        ssd1306_clear(&disp);

        if (counting_mode) {
            snprintf(buffer1, sizeof(buffer1), "Cont. Regressiva: %d", counter);
            snprintf(buffer2, sizeof(buffer2), "Botao B: %d", pressing_B);

            ssd1306_clear(&disp);
            ssd1306_draw_string(&disp, 0, 0, 1, buffer1);       
            ssd1306_draw_string(&disp, 0, 10, 1, buffer2);      
            ssd1306_show(&disp);

            sleep_ms(1000);         

            counter--;
      
            if (counter < 0) {
                counting_mode = 0;
            }
        } else {
            snprintf(buffer1, sizeof(buffer1), "Cont. Regressiva: 0");
            snprintf(buffer2, sizeof(buffer2), "Botao B: %d", pressing_B);

            ssd1306_clear(&disp);
            ssd1306_draw_string(&disp, 0, 0, 1, buffer1);       
            ssd1306_draw_string(&disp, 0, 10, 1, buffer2);      
            ssd1306_show(&disp);     
        }
        tight_loop_contents(); 
    }
}

// funcao que inicializa os botoes e a operacao de interrupcao deles
void setup_button() {
    gpio_init(BUTTON_PIN_A);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);

    gpio_init(BUTTON_PIN_B);
    gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_B);

    gpio_set_irq_enabled_with_callback(BUTTON_PIN_A, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    gpio_set_irq_enabled(BUTTON_PIN_B, GPIO_IRQ_EDGE_FALL, true);
}

// funcao para configurar o display
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

// funcao de callback para a interrupção
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN_A && (events & GPIO_IRQ_EDGE_FALL)) {
        counter = 9;     
        pressing_B = 0;    
        counting_mode = 1;   
    }

    if (counting_mode && gpio == BUTTON_PIN_B && (events & GPIO_IRQ_EDGE_FALL)) {
        pressing_B++;
    }
}


