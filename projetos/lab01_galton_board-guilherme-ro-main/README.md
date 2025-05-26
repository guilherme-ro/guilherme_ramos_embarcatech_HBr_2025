# Projetos de Sistemas Embarcados - EmbarcaTech 2025 

Autor: **Guilherme Ramos Oliveira**

Curso: Residência Tecnológica em Sistemas Embarcados

Instituição: EmbarcaTech - HBr

Campinas, Maio de 2025

---

# Projeto 1 - Galton Board Digital com a BitDogLab

Desenvolvimento de um programa em linguagem C com pico SDK que implementa um Digital Twin do Galton Board.

### Hardware:

- Placa BitDogLab com o microcontrolador Raspberry Pi Pico W embutido.
- Display OLED compatível.
- Matriz de 25 LEDs RGB. (não utilizada)
- LED RGB isolado. (não utilizado)
- Microfone integrado. (não utilizado)
- Joystick analógico. 
- Botões A e B.
- Buzzer A e Buzzer B. (não utilizado)
- Módulo Wi-Fi. (não utilizado)

### Software:

- Código desenvolvido em linguagem C com Pico SDK.
- Implementação modular e estruturada do código.
- SDK 1.5.1

### Comportamento do Programa:

Se o código é compilado com cmake .. e make, ele será executado com 9 canaletas, se for compilado com cmake -DNUM_SLOTS=16 .. e make, ele será executado com 16 canaletas. O parâmetro pode ser alterado. A partir de 11 canaletas, a contagem de bolas por canaleta é omitida, para melhor visualização.

No display são exibidas a contagem de bolas por canaleta logo abaixo da coluna somatória de cada canaleta, a contagem total de bolas no canto superior direito e um histograma no canto superior esquerdo. 

O desvio das bolas após colisão é calculado no código e as bolas aparecem no display na posição x calculada após desvio e começam a cair alterando sua posição y até chegarem na base da canaleta.

As bolas caindo são representadas pelo caracter "o" e, ao chegarem a base da canaleta, se transformam em um retângulo de 1px de altura, acumulando na coluna junto as próximas bolas que continuam caindo.

O Botão A (GPIO5) pressionado faz o desbalanceamento para a esquerda.

O Botão B (GPIO6) pressionado faz o desbalanceamento para a direita.
 
O Botão SW do joystick (GPIO22) altera a quantidade de bolas caindo, passando de 1 bola por vez para múltiplas bolas por vez (máx de 10).

Notas:

O número de níveis de pinos que as bolas atravessam está definido em 15 níveis.

O máximo de bolas exibidas por slot está definido em 35 para limitar a altura das colunas. As bolas que caem em uma mesma coluna a partir disso são contabilizadas, mas a coluna não aumenta.

O tick - Intervalo entre atualizações do jogo - está definido em 100 ms.

A Aceleração de gravidade aplicada às bolas é de 0.1 pixels/tick².

O total de bolas que vão cair está definido em 150.

O código permite ajustes finos, como o número de estacas/pinos de desvio das bolas, número de canaletas/slots

Há um vídeo de demonstração.

### Imagens do Projeto

<p align="center">
  <img src="https://github.com/EmbarcaTech-2025/lab01_galton_board-guilherme-ro/raw/main/assets/galton_board_9_slots.png" width="361" height="378" alt="Bitdoglab" title="Galton Board com 9 Slots">
</p>
<p align="center">
  <img src="https://github.com/EmbarcaTech-2025/lab01_galton_board-guilherme-ro/raw/main/assets/galton_board_16_slots.png" width="362" height="370" alt="Bitdoglab" title="Galton Board com 16 Slots">
</p>

### Como Compilar no Linux:

Para o programa ter 9 canaletas/slots (valor padrão) com contagem de bolas em cada canaleta exibida no display:

```bash
cd ~pico/projeto

mkdir build

cd build

cmake ..

make

export PICO_SDK_PATH=/home/$USER/pico/pico-sdk 
(Adapte conforme necessário)

cp nome_arquivo_c.uf2 /media/$USER/RPI-RP2/
```

Para o programa ter 16 canaletas/slots sem contagem de bolas em cada canaleta exibida no display:

```bash
cd ~pico/projeto

mkdir build

cd build

cmake -DNUM_SLOTS=16 ..
(Adapte conforme necessário. Para mais de 10 canaletas/slots, a contagem por canaleta não será exibida)

make

export PICO_SDK_PATH=/home/$USER/pico/pico-sdk 
(Adapte conforme necessário)

cp nome_arquivo_c.uf2 /media/$USER/RPI-RP2/
```

---

## 📜 Licença
MIT License - MIT GPL-3.0.


