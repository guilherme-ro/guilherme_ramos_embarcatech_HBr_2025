// Bibliotecas necessárias
#include <string.h>                 // Para funções de string como strlen()
#include "pico/stdlib.h"            // Biblioteca padrão do Pico (GPIO, tempo, etc.)
#include "pico/cyw43_arch.h"        // Driver WiFi para Pico W
#include "include/wifi_conn.h"      // Funções personalizadas de conexão WiFi
#include "include/mqtt_comm.h"      // Funções personalizadas para MQTT
#include "include/xor_cipher.h"     // Funções de cifra XOR
#include <stdio.h>
#include <time.h>

#define wifi_ssid "Rede" 
#define wifi_password "senha"

int main()
{
    stdio_init_all();
    connect_to_wifi(wifi_ssid, wifi_password);
    mqtt_setup("bitdog1", "ip_rede", "aluno", "senha123"); 

    // Buffer para a mensagem JSON
    char mensagem[64];
    // Buffer para mensagem criptografada
    uint8_t criptografada[64];

    while (true) {
        // Formata a mensagem JSON com valor e timestamp
        snprintf(mensagem, sizeof(mensagem), "{\"valor\":26.5,\"ts\":%lu}", (unsigned long)time(NULL));
        
        // Criptografa a mensagem usando XOR com chave 42
        xor_encrypt((const uint8_t *)mensagem, criptografada, strlen(mensagem), 42);
        
        // Publica a mensagem criptografada
        mqtt_comm_publish("escola/sala1/temperatura", criptografada, strlen(mensagem));
        
        sleep_ms(1000); 
    }
}
