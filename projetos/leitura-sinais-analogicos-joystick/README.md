# Contador Decrescente com interrupção atuando nos botões
Desenvolvimento de um programa em linguagem C com pico SDK que lê os valores convertidos digitalmente do joystick da BitDogLab.

### Hardware:

- Placa BitDogLab com o microcontrolador Raspberry Pi Pico W embutido.
- Display OLED compatível.
- Matriz de 25 LEDs RGB. (não utilizada)
- LED RGB isolado. (não utilizado)
- Microfone integrado. (não utilizado)
- Joystick analógico. 
- Botões A e B. (não utilizados)
- Buzzer A e Buzzer B. (não utilizados)
- Módulo Wi-Fi. (não utilizado)

### Software:

- Código desenvolvido em linguagem C com Pico SDK.
- Implementação modular e estruturada do código.

### Comportamento do Programa:

Mostra a leitura das coordenadas x e y do Joystick.

### Imagem do Projeto
<p align="center">
  <img src="https://github.com/guilherme-ro/guilherme_ramos_embarcatech_HBr_2025/raw/main/projetos/leitura-sinais-analogicos-joystick/assets/leitura-joystick.jpg" width="500" height="600" alt="Bitdoglab">
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

