#ifndef MQTT_COMM_H
#define MQTT_COMM_H

#include <stdint.h>

// Declarações das funções
void mqtt_setup(const char *client_id, const char *broker_ip, const char *user, const char *pass);
void mqtt_comm_subscribe(const char *topic);
void xor_encrypt(const uint8_t *input, uint8_t *output, size_t len, uint8_t key);

#endif
