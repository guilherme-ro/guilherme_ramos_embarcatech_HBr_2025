#ifndef HAL_DISPLAY_H
#define HAL_DISPLAY_H

#include <stdint.h>

void hal_display_init(void);
void hal_display_clear(void);
void hal_display_draw_string(uint8_t x, uint8_t y, const char *str);
void hal_display_show(void);

#endif // HAL_DISPLAY_H

