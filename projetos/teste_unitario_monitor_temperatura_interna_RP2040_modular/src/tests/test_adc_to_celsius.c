#include "pico/stdlib.h"
#include "unity.h"
#include "hal_temp_sensor.h"

void test_adc_to_celsius(void) {
    uint16_t adc_val = 875; //valor entre 0 e 4095 - 12 bits - para aproximacao de 27 graus celsius
    float expected_temp = 27.0f;
    float actual = hal_adc_to_celsius(adc_val);
    printf("%.2f\n\n", actual);
    TEST_ASSERT_FLOAT_WITHIN(0.7f, expected_temp, actual); //teste com margem de erro de 0.7
}

void setUp(void) {}
void tearDown(void) {}

int main(void) {
    stdio_init_all();  //inicializar os componentes de entrada e saída padrao (stdio) e stdio USB
    sleep_ms(5000);    // Aguarda 5 segundos para o USB estabilizar
    printf("Iniciando testes...\n");  // Mensagem de depuração
    UNITY_BEGIN();
    RUN_TEST(test_adc_to_celsius);
    UNITY_END();
    stdio_flush();
    while (1) {        // Loop infinito para manter o programa ativo
        sleep_ms(1000);
    }
    return 0;
}
