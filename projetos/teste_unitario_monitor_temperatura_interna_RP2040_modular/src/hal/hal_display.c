#include "hal_display.h"
#include "ssd1306.h"
#include "hardware/i2c.h"
#include "hardware/gpio.h"

static ssd1306_t disp;

void hal_display_init(void) {
    i2c_init(i2c1, 400000);
    gpio_set_function(14, GPIO_FUNC_I2C);
    gpio_set_function(15, GPIO_FUNC_I2C);
    gpio_pull_up(14);
    gpio_pull_up(15);

    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, i2c1);
    ssd1306_clear(&disp);
}

void hal_display_clear(void) {
    ssd1306_clear(&disp);
}

void hal_display_draw_string(uint8_t x, uint8_t y, const char *str) {
    ssd1306_draw_string(&disp, x, y, 1, str);
}

void hal_display_show(void) {
    ssd1306_show(&disp);
}

