#include "pico/stdlib.h"
#include "hal_led.h"

int main() {
    stdio_init_all(); //inicializar os componentes de entrada e saída padrao (stdio)
    hal_led_init(); //abstracao da funcao que interage diretamente com a biblioteca cyw43_arch.h

    while (true) {
        hal_led_toggle(); //abstracao da funcao que faz o LED da RP2040 piscar
        sleep_ms(500);
    }
}
