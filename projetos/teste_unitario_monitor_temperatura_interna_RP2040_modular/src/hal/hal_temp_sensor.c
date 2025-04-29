#include <stdbool.h>
#include "hal_temp_sensor.h"
#include "hardware/adc.h"

void hal_temp_sensor_init(void) {
    adc_init();
    adc_set_temp_sensor_enabled(true);
    adc_select_input(4); //sensor ADC interno da RP2040
}

float hal_adc_to_celsius(uint16_t adc_val) {
    const float conversion_factor = 3.3f / (1 << 12); // (3.3/4095)
    uint16_t raw = adc_val;
    float voltage = raw * conversion_factor;
    float temp_celsius = 27.0f - (voltage - 0.706f) / 0.001721f;

    return temp_celsius;
}

