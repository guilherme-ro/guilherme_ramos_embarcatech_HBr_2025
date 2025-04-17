#include "pico/stdlib.h"
#include <stdio.h>
#include "hardware/adc.h"  
#include "ssd1306/ssd1306.h"  

ssd1306_t disp; // Objeto para o display SSD1306

void setup_display();

void main(void) {
    char buffer1[32];
    char buffer2[32];

    // Inicializa a biblioteca padrão e o ADC
    stdio_init_all();
    adc_init();
    setup_display();

    // Habilita o sensor de temperatura interno
    adc_set_temp_sensor_enabled(true);

    // Seleciona o canal ADC4 (sensor de temperatura)
    adc_select_input(4);

    while(1) {
        // Lê o valor bruto do ADC (12 bits, 0-4095)
        uint16_t raw = adc_read();

        // leitura de 10 amostras e calcular a média
        uint16_t sum = 0;
        for (int i = 0; i < 10; i++) {
            sum += adc_read();
            sleep_ms(10);
        }
        uint16_t raw_media = sum / 10;

        // Converte o valor bruto para tensão (Vref = 3.3V, 12 bits)
        const float conversion_factor = 3.3f / (1 << 12);
        float voltage = raw * conversion_factor;
        float voltage_media = raw_media * conversion_factor;

        // Converte a tensão para temperatura em Celsius
        // Formula - T = 27 - (V - 0.706) / 0.001721
        float temp_celsius = 27.0f - (voltage - 0.706f) / 0.001721f;
        float temp_celsius_media = 27.0f - (voltage_media - 0.706f) / 0.001721f;

        snprintf(buffer1, sizeof(buffer1), "Temperatura: %.2f C", temp_celsius);
        snprintf(buffer2, sizeof(buffer2), "Media: %.2f C", temp_celsius_media);

        ssd1306_clear(&disp);
        ssd1306_draw_string(&disp, 0, 0, 1, buffer1);       
        ssd1306_draw_string(&disp, 0, 10, 1, buffer2);      
        ssd1306_show(&disp);

        sleep_ms(1000);
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
