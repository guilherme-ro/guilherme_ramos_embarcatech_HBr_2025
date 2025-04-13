# Monitor da temperatura interna da MCU - RP2040
Desenvolvimento de um programa em linguagem C com pico SDK que lê a temperatura interna do RP2040 e converte a leitura do ADC em um valor em ºC.

### Hardware:

- Placa BitDogLab com o microcontrolador Raspberry Pi Pico W embutido.
- Display OLED compatível.
- Matriz de 25 LEDs RGB. (não utilizada)
- LED RGB isolado. (não utilizado)
- Microfone integrado. (não utilizado)
- Joystick analógico. (não utilizado)
- Botões A e B. (não utilizados)
- Buzzer A e Buzzer B. (não utilizados)
- Módulo Wi-Fi. (não utilizado)

### Software:

- Código desenvolvido em linguagem C com Pico SDK.
- Implementação modular e estruturada do código.

### Comportamento do Programa:

Mostra a leitura da temperatura interna do RP2040 em ºC

### Como Compilar no Linux:

cd ~pico/projeto

mkdir build

cd build

cmake ..

make

export PICO_SDK_PATH=/home/$USER/pico/pico-sdk 
(adapte conforme necessário)

cp nome_arquivo_c.uf2 /media/$USER/RPI-RP2/
