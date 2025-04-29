#ifndef HAL_TEMP_SENSOR_H
#define HAL_TEMP_SENSOR_H

#include <stdint.h>

float hal_adc_to_celsius(uint16_t adc_val);
void hal_temp_sensor_init(void);

#endif // HAL_TEMP_SENSOR_H

