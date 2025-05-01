# Prática - Teste unitário com a bibliotca de Testes Unity

Desenvolvimento de teste unitário com a biblioteca Unity (disponível em [Biblioteca Unity](https://www.throwtheswitch.org/unity)) para verificar se a função hal_adc_to_celsius(uint16_t adc_val) retorna valor correto (com margem de erro) para uma leitura simulada de ADC.

### Hardware:

- Placa BitDogLab com o microcontrolador Raspberry Pi Pico W embutido.
- Display OLED compatível. 
- Matriz de 25 LEDs RGB. (não utilizada)
- LED RGB isolado. (não utilizado)
- Microfone integrado. (não utilizado)
- Joystick analógico. (não utilizado)
- Botões A e B. (não utilizados)
- Buzzer A e Buzzer B. (não utilizado)
- Módulo Wi-Fi. (não utilizado)

### Software:

- Código desenvolvido em linguagem C com Pico SDK.
- Implementação modular e estruturada do código.

### Comportamento do Programa:

O programa principal exibe no display a leitura da temperatura interna do RP2040 em ºC.

O programa de teste retorna o teste aplicado na função hal_adc_to_celsius(uint16_t adc_val) do arquivo hal/hal_temp_sensor.c.

Considerações sobre o teste:

- A função recebe um inteiro sem sinal, conforme o enunciado pede, hal_adc_to_celsius(uint16_t adc_val), sendo esse inteiro um valor fixo entre 0 e 4095 (12 bits) ajustado de acordo para o teste passar. Esse valor é uma simulação de leitura do sensor de ADC interno do RP2040.
- A fórmula de conversão de ADC para graus Celsius é:
 
$$T = 27 - \left( \frac{ \left( \frac{\text{adc\-val} * 3.3}{4095} \right) - 0.706 }{0.001721} \right)$$

Em que, adc_val * 3.3/4095 converte o valor do ADC para a tensão correspondente, em número de ponto flutuante;


### Imagem do Projeto
<p align="center">
  <img src="https://github.com/guilherme-ro/guilherme_ramos_embarcatech_HBr_2025/raw/main/projetos/teste_unitario_monitor_temperatura_interna_RP2040_modular/assets/temperatura-interna-RP2040.jpg" width="500" height="600" alt="Monitor de Temperatura do RP2040">
</p>

<p align="center">
  <img src="https://github.com/guilherme-ro/guilherme_ramos_embarcatech_HBr_2025/raw/main/projetos/teste_unitario_monitor_temperatura_interna_RP2040_modular/assets/teste_unitario.png" width="579" height="264" alt="Teste Unitário">
</p>

### Como Compilar no Linux:

* Baixar a biblioteca Unity:

```
cd ~/Downloads
wget https://github.com/ThrowTheSwitch/Unity/archive/refs/heads/master.zip
unzip master.zip
mv Unity-master ~/unity
```

* Incorporar a biblioteca Unity no projeto editando CMakeLists.txt:

```bash
add_executable(test_runner
    tests/test_adc_to_celsius.c  # Inclua o arquivo de teste, se for parte do build
    hal/hal_temp_sensor.c
    ${UNITY_PATH}/src/unity.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${PICO_SDK_PATH}/include  # Diretório de cabeçalhos do SDK
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${UNITY_PATH}/src
)
```

Em que UNITY_PATH é o caminho da pasta da biblioteca Unity

* Compilar:

```bash
cd ~pico/projeto
mkdir build
cd build
cmake ..
make
```

```bash
export PICO_SDK_PATH=/home/$USER/pico/pico-sdk 
```
(adapte conforme necessário)

```bash
cp nome_arquivo_c.uf2 /media/$USER/RPI-RP2/
```
(executar esse comando para o arquivo principal ou para o arquivo de teste, ajustando nome_arquivo_c.uf2)

* Testes:

```bash
minicom -D /dev/ttyACM0
```

