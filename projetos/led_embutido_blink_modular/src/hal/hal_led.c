#include "hal_led.h"
#include "led_embutido.h"

void hal_led_init(void) {
    led_embutido_init(); //abstracao da funcao cyw43_arch_init()
}

//static garante que a variavel estado seja persistente entre as chamadas da funcao hal_led_toggle
void hal_led_toggle(void) {
    static int estado = 0; 
    estado = !estado;
    led_embutido_set(estado);
}
