[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/G8V_0Zaq)

# Tarefa: IoT Security Lab - EmbarcaTech 2025

Autor: **Guilherme Ramos Oliveira e Inaelson Franklin Alves Ferreira**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, Maio de 2025

---

Desenvolvimento de um programa em linguagem C com pico SDK que implementa Segurança em IoT com a BitDogLab. Os experimentos desse projeto tiveram um broker MQTT criado com o software open source mosquitto. Fez-se publish com mosquitto e com a BitdogLab e fez-se subscribe com o mosquitto e com a BitdogLab, de acordo com as etapas propostas. Os testes foram feitos com conexão sem segurança e em seguida, conexão em que a mensagem é cifrada na fonte e decifrada no destino.

### Hardware:

- Placa BitDogLab com o microcontrolador Raspberry Pi Pico W embutido.
- Display OLED compatível. (não utilizado)
- Matriz de 25 LEDs RGB. (não utilizada)
- LED RGB isolado. (não utilizado)
- Microfone integrado. (não utilizado)
- Joystick analógico. (não utilizado)
- Botões A e B. (não utilizados)
- Buzzer A e Buzzer B. (não utilizado)
- Módulo Wi-Fi. 

### Software:

- Código desenvolvido em linguagem C com Pico SDK.
- Implementação modular e estruturada do código.

### Desenvolvimento do Projeto:

**Etapa 1: Conectando a BitDogLab ao Wi-Fi**

 - Configuração do arquivo src/wifi_conn.c

**Etapa 2: Setup MQTT básico para IoT**

 - Configuração do arquivo src/mqtt_comm.c

**Etapa 3: Publicação MQTT sem segurança**

 - Em /etc/mosquitto/mosquitto.conf foram adicionadas as linhas

```
listener 1883
allow_anonymous true
```

Em CMakeLists.txt foram adicionados os termos ```pico_cyw43_arch_lwip_threadsafe_background``` e ```pico_lwip_mqtt```

```
target_link_libraries(comunicacao_mqtt
        pico_stdlib
        pico_cyw43_arch_lwip_threadsafe_background
        pico_lwip_mqtt
)
```

Iniciar o broker Mosquitto:

```
mosquitto -v
```

Subscribe no broker pela rede wi-fi:

```
mosquitto_sub -h 192.168.86.10 -t escola/sala1/temperatura -v
```

Publish no broker pela rede wi-fi:

```
mosquitto_pub -h 192.168.86.10 -t escola/sala1/temperatura -m "26.5"
```

ou

```
cp iot_security_lab.uf2 /media/$USER/RPI-RP2/
```
(executa o publish através do programa na BitDogLab)

Os trechos de código em comunicacao_mqtt.c para o publish são:

```
mqtt_setup("client_id", "192.168.86.10", NULL, NULL);
```

e

```
mqtt_comm_publish("escola/sala1/temperatura", (const uint8_t *)"26.5", strlen("26.5"));
```
Subscribe no broker pela rede wi-fi:
<p align="center">
  <img src="https://github.com/EmbarcaTech-2025/tarefa-iot-security-lab-sociedade-esportiva-embarcatech/raw/main/assets/figura1.png" width="764" height="297" alt="Figura 1" title="Subscribe no broker pela rede wi-fi">
</p>

Wireshark mostra conteúdo legível:
<p align="center">
  <img src="https://github.com/EmbarcaTech-2025/tarefa-iot-security-lab-sociedade-esportiva-embarcatech/raw/main/assets/figura2.png" width="955" height="664" alt="Figura 2" title="Wireshark mostra conteúdo legível">
</p>

**Etapa 4: Autenticação básica no Mosquitto**

O arquivo /etc/mosquitto/mosquitto.conf foi alterado:

```
allow_anonymous false
```

Agora é necessário autenticação para publish e subscribe:

```
mosquitto_sub -h 192.168.86.10 -p 1883 -t "escola/sala1/temperatura" -u "aluno" -P "senha123"
```

```
mosquitto_pub -h 192.168.86.10 -p 1883 -t "escola/sala1/temperatura" -u "aluno" -P "senha123" -m "26.6"
```

Publish não autorizado por falta de acesso:
<p align="center">
  <img src="https://github.com/EmbarcaTech-2025/tarefa-iot-security-lab-sociedade-esportiva-embarcatech/raw/main/assets/figura3.png" width="956" height="267" alt="Figura 3" title="Publish não autorizado por falta de acesso">
</p>

**Etapa 5: Simulando criptografia leve (XOR)**

Subscribe no broker pela rede wi-fi: 

```
mosquitto_sub -h 192.168.86.10 -p 1883 -t "escola/sala1/temperatura" -u "aluno" -P "senha123"
```

Programa executado na Bitdoglab:

```
cp iot_security_lab.uf2 /media/$USER/RPI-RP2/
```

Wireshark mostra conteúdo ofuscado:
<p align="center">
  <img src="https://github.com/EmbarcaTech-2025/tarefa-iot-security-lab-sociedade-esportiva-embarcatech/raw/main/assets/figura4.png" width="954" height="667" alt="Figura 4" title="Wireshark mostra conteúdo ofuscado">
</p>

**Etapa 6: Proteção contra replay**

A função ```mqtt_incoming_data_cb``` verifica se nova_timestamp é maior que ```ultima_timestamp_recebida```. Para reforçar a proteção contra replays e mensagens antigas, foi adicionada uma janela de tempo para validar se o timestamp recebido está dentro de um intervalo aceitável em relação ao tempo atual do sistema. Mensagens são descartadas mesmo sendo "novas" em relação ao último timestamp, podendo estar desatualizadas ou fora de sincronia. Portanto, somente mensagens dentro de uma janela de tempo são aceitas.

**MQTT Subscribe**

O código que simula MQTT subscribe na BitdogLab pode ser monitorado pelo USB ou porta serial através de

```
minicom -D /dev/ttyACM0
```

Para identificar a porta serial:

```
ls /dev/tty*
```
**Etapa final: relato e análise**

Quais técnicas são escaláveis?

As técnicas escaláveis incluem conexão Wi-Fi com configuração centralizada, autenticação básica no Mosquitto com gerenciamento automatizado de credenciais, e proteção contra replays com timestamps sincronizados via NTP. A criptografia XOR é limitada e deve ser substituída por AES para redes escolares maiores. Estruturas de tópicos hierárquicas e brokers em cluster suportam múltiplas BitdogLab eficientemente.

Como aplicá-las com várias BitDogLab em rede escolar?

Para aplicar essas técnicas em uma rede escolar, é necessário considerar gerenciamento centralizado, segurança robusta (AES), sincronização de tempo com protocolo NTP, estrutura de tópicos hierárquica, redundância e balanceamento, uma infraestrutura de rede adequada, monitoramento e depuração e automação de configuração. Isso garante que o sistema seja gerenciável e seguro, mesmo com muitas BitDogLab. É importante utilizar DHCP para atribuir endereços IP automaticamente às BitdogLab, reduzindo a configuração manual.


### Como Compilar no Linux:

Para compilar iot_security_lab:

```
mkdir build
cd build
cmake -DPROJECT=iot_security_lab ..
make
```
Para compilar mqtt_subscribe:

```
mkdir build
cd build
cmake -DPROJECT=mqtt_subscribe ..
make
```

---

## 📜 Licença
GNU GPL-3.0.
