# Estruturação de código para arquitetura modular

Organização de código blink que faz piscar o LED embutido da Raspberry Pi Pico W para se adequar a arquitetura modular.

### Hardware:

- Placa BitDogLab com o microcontrolador Raspberry Pi Pico W embutido.
- Display OLED compatível. (não utilizado)
- Matriz de 25 LEDs RGB. (não utilizada)
- LED RGB isolado. 
- Microfone integrado. (não utilizado)
- Joystick analógico. (não utilizado)
- Botões A e B. (não utilizados)
- Buzzer A e Buzzer B. (não utilizado)
- Módulo Wi-Fi. (não utilizado)

### Software:

- Código desenvolvido em linguagem C com Pico SDK.
- Implementação modular e estruturada do código.

### Comportamento do Programa:

LED embutido da Raspberry Pi Pico W pisca indefinidamente.

### Imagem do Projeto
<p align="center">
  <img src="https://github.com/guilherme-ro/guilherme_ramos_embarcatech_HBr_2025/raw/main/projetos/led_embutido_blink_modular/assets/led_RP2040_blink.jpg" width="500" height="600" alt="LED RP2040">
</p>

### Como Compilar no Linux:

cd ~pico/projeto

mkdir build

cd build

cmake ..

make

export PICO_SDK_PATH=/home/$USER/pico/pico-sdk 
(adapte conforme necessário)

cp nome_arquivo_c.uf2 /media/$USER/RPI-RP2/


