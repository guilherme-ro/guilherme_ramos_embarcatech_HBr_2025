#include <stdio.h>
#include "pico/stdlib.h"
#include "include/wifi_conn.h"
#include "include/mqtt_comm_subscribe.h"
#include "include/xor_cipher.h"

#define WIFI_SSID "Rede"
#define WIFI_PASSWORD "senha"

int main() {
    stdio_init_all();

    // Conecta ao Wi-Fi
    connect_to_wifi(WIFI_SSID, WIFI_PASSWORD);

    // Configura o cliente MQTT com as credenciais fornecidas
    mqtt_setup("bitdog1", "ip_rede", "aluno", "senha123");

    // Inscreve no tópico desejado (exemplo: "escola/sala1/temperatura")
    mqtt_comm_subscribe("escola/sala1/temperatura");

    while (true) {
        // Mantém o programa em execução para processar mensagens recebidas
        sleep_ms(1000);
    }

    return 0;
}
