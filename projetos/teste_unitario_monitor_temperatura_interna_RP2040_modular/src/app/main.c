#include "pico/stdlib.h"
#include "hal_display.h"
#include "hal_temp_sensor.h"
#include "hardware/adc.h"
#include <stdio.h>

void main(void) {
    char buffer[32];

    hal_display_init(); //abstracao para a inicializacao do display OLED ssd1306
    hal_temp_sensor_init(); //abstracao para a inicializacao do sensor interno ADC da RP2040

    while (1) {
        uint16_t raw = adc_read(); //leitura do sensor interno ADC da RP2040

        float temp_celsius = hal_adc_to_celsius(raw);

        snprintf(buffer, sizeof(buffer), "Temperatura: %.2f C", temp_celsius);
        hal_display_clear(); //abstracao para a limpeza do display OLED
        hal_display_draw_string(0, 0, buffer); //abstracao para a escrita de string no display OLED
        hal_display_show(); //abstracao para mostrar conteúdo no display OLED

        sleep_ms(1000);
    }
}

