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

### Cálculo da Média de 10 amostras

Além da leitura e conversão para ºC da temperatura interna do RP2040, optou-se também em ler mais 10 amostras de temperatura e também exibir no display OLED a média dessas amostras para fazer um comparativo com uma amostra só captada e comparar se há um desvio muito grande em relação à média.

O código para ler as 10 amostras segue abaixo. O código escrito para ler apenas um valor é raw = adc_read();

```
// leitura de 10 amostras e calcular a média
uint16_t sum = 0;
for (int i = 0; i < 10; i++) {
    sum += adc_read();
    sleep_ms(10);
}
uint16_t raw_media = sum / 10;
```

### Imagem do Projeto
<p align="center">
  <img src="https://github.com/guilherme-ro/guilherme_ramos_embarcatech_HBr_2025/raw/main/projetos/monitor-temperatura-interna-MCU/assets/temperatura-interna-RP2040.jpg" width="500" height="600" alt="Bitdoglab">
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

