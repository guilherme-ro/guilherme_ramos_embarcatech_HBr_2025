#include "lwip/apps/mqtt.h"
#include "include/mqtt_comm_subscribe.h"
#include "lwipopts.h"
#include <stdio.h>
#include <string.h>

// Variável global estática para armazenar a instância do cliente MQTT
static mqtt_client_t *client;

// Callback para mensagens recebidas no tópico inscrito
static void mqtt_incoming_data_cb(void *arg, const u8_t *data, u16_t len, u8_t flags) {
    printf("Mensagem recebida no tópico: ");
    for (u16_t i = 0; i < len; i++) {
        printf("%c", data[i]);
    }
    printf("\n");

    // Exemplo de decifragem XOR, se a mensagem for criptografada (chave 42)
    uint8_t decifrada[16];
    xor_encrypt(data, decifrada, len, 42);
    printf("Mensagem decifrada: %s\n", decifrada);
}

// Callback para confirmação de inscrição
static void mqtt_sub_request_cb(void *arg, err_t result) {
    if (result == ERR_OK) {
        printf("Inscrição no tópico realizada com sucesso!\n");
    } else {
        printf("Erro ao inscrever no tópico: %d\n", result);
    }
}

// Callback de conexão MQTT
static void mqtt_connection_cb(mqtt_client_t *client, void *arg, mqtt_connection_status_t status) {
    if (status == MQTT_CONNECT_ACCEPTED) {
        printf("Conectado ao broker MQTT com sucesso!\n");
    } else {
        printf("Falha ao conectar ao broker, código: %d\n", status);
    }
}

// Função para configurar e iniciar a conexão MQTT
void mqtt_setup(const char *client_id, const char *broker_ip, const char *user, const char *pass) {
    ip_addr_t broker_addr;

    // Converte o IP de string para formato numérico
    if (!ip4addr_aton(broker_ip, &broker_addr)) {
        printf("Erro no IP\n");
        return;
    }

    // Cria uma nova instância do cliente MQTT
    client = mqtt_client_new();
    if (client == NULL) {
        printf("Falha ao criar o cliente MQTT\n");
        return;
    }

    // Configura as informações de conexão do cliente
    struct mqtt_connect_client_info_t ci = {
        .client_id = client_id,
        .client_user = user,
        .client_pass = pass
    };

    // Inicia a conexão com o broker
    mqtt_client_connect(client, &broker_addr, 1883, mqtt_connection_cb, NULL, &ci);
}

// Função para se inscrever em um tópico MQTT
void mqtt_comm_subscribe(const char *topic) {
    // Configura o callback para dados recebidos
    mqtt_set_inpub_callback(client, NULL, mqtt_incoming_data_cb, NULL);

    // Inscreve no tópico
    err_t status = mqtt_subscribe(
        client,              
        topic,               
        0,                   
        mqtt_sub_request_cb, 
        NULL                 
    );

    if (status != ERR_OK) {
        printf("Falha ao inscrever no tópico: %d\n", status);
    }
}
