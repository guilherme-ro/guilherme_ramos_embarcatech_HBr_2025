#include "pico/stdlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "ssd1306.h"
#include "hardware/gpio.h"

#define BUTTON_PIN_A 5
#define BUTTON_PIN_B 6
#define SW 22
#define I2C_SDA 14
#define I2C_SCL 15

#define LEVELS 11 // numero de níveis de estacas que as bolas tem como obstaculo
#ifndef NUM_SLOTS
#define NUM_SLOTS 9  // numero de canaletas (ajustável) 9 ou 16 - valor padrão 9 caso não seja passado pelo terminal
#endif
#define SLOT_WIDTH (128 / NUM_SLOTS) // Largura de cada slot
#define HIST_HEIGHT 16
#define MAX_BALLS_PER_SLOT 35
#define TICK_INTERVAL_MS 100
#define MAX_BALLS 10
#define MAX_TOTAL_BALLS 150
#define GRAVITY 0.1f

ssd1306_t disp;

// Estrutura para uma bola
typedef struct {
    float x;
    float y;
    float vx;
    float vy;
    int level;
    int final_x;
    bool active;
} Ball;

Ball balls[MAX_BALLS];
int slots[NUM_SLOTS] = {0};
int total_balls = 0;
bool bias_left = false;
bool bias_right = false;
bool multiple_balls_mode = false;
int ball_count = 0;

// Configuração do display
void setup_display() {
    i2c_init(i2c1, 400000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);
    disp.external_vcc = false;
    ssd1306_init(&disp, 128, 64, 0x3C, i2c1);
    ssd1306_clear(&disp);
}

// Configuração dos botões
void setup_buttons() {
    gpio_init(BUTTON_PIN_A);
    gpio_init(BUTTON_PIN_B);
    gpio_init(SW);
    gpio_set_dir(BUTTON_PIN_A, GPIO_IN);
    gpio_set_dir(BUTTON_PIN_B, GPIO_IN);
    gpio_set_dir(SW, GPIO_IN);
    gpio_pull_up(BUTTON_PIN_A);
    gpio_pull_up(BUTTON_PIN_B);
    gpio_pull_up(SW);
}

// Função de escolha aleatória com desbalanceamento
int random_direction() {
    int r = rand() % 100;
    if (bias_left) {
        return (r < 90) ? -1 : 1;
    } else if (bias_right) {
        return (r < 90) ? 1 : -1;
    } else {
        return (r < 50) ? -1 : 1;
    }
}

// Simulação do caminho da bola
int simulate_drop() {
    float position = 0;
    for (int i = 0; i < LEVELS; i++) {
        position += random_direction();
    }
    int adjusted_position = round((position + LEVELS) * (NUM_SLOTS - 1.0) / (LEVELS * 2));
    if (adjusted_position < 0) adjusted_position = 0;
    if (adjusted_position >= NUM_SLOTS) adjusted_position = NUM_SLOTS - 1;
    return adjusted_position;
}

// Inicialização de uma bola
void init_ball(int index) {
    int final_slot = simulate_drop();
    balls[index].x = final_slot * SLOT_WIDTH;
    balls[index].y = 16;
    balls[index].vx = 0;
    balls[index].vy = 0;
    balls[index].level = 0;
    balls[index].final_x = final_slot * SLOT_WIDTH;
    balls[index].active = true;
}

// Atualização das bolas por tick
void update_balls() {
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            balls[i].vy += GRAVITY;
            balls[i].y += balls[i].vy;
            balls[i].level++;
            if (balls[i].level <= LEVELS) {
                balls[i].vx += random_direction() * 1.0;
                balls[i].x += balls[i].vx;
            }
            if (balls[i].y >= 51) { 
                balls[i].y = 51;  
                int slot = balls[i].final_x / SLOT_WIDTH;
                if (slot >= 0 && slot < NUM_SLOTS) {
                    slots[slot]++;
                    total_balls++;
                }
                balls[i].active = false;
                ball_count--;
            }
        }
    }
}

// Desenhar histograma e gaussiana
void draw_histogram_and_gauss(int slots[NUM_SLOTS]) {
    int bar_width = 40 / NUM_SLOTS;
    int total_bar_width = bar_width * NUM_SLOTS;
    int x_offset = (40 - total_bar_width) / 2;

    int max_height = 0;
    for (int i = 0; i < NUM_SLOTS; i++) {
        if (slots[i] > max_height) max_height = slots[i];
    }

    float scale_factor = max_height > 0 ? (float)HIST_HEIGHT / max_height : 1.0;

    for (int i = 0; i < NUM_SLOTS; i++) {
        int bar_height = (int)(slots[i] * scale_factor);
        if (bar_height > HIST_HEIGHT) bar_height = HIST_HEIGHT;
        int x = x_offset + i * bar_width;
        ssd1306_draw_square(&disp, x, HIST_HEIGHT - bar_height, bar_width - 1, bar_height);
    }

    float mu = 20.0;
    float sigma = 40.0 / 6.0;
    float scale = (float)HIST_HEIGHT;

    for (int x = 0; x < 40; x++) {
        float z = (x - mu) / sigma;
        float gauss = scale * exp(-z * z / 2);
        int y = HIST_HEIGHT - (int)(gauss + 0.5);
        if (y >= 0 && y < HIST_HEIGHT) {
            ssd1306_draw_pixel(&disp, x, y);
        }
    }
}

// Desenhar contador de bolas
void draw_ball_count(int num_balls) {
    char buffer[16];
    snprintf(buffer, sizeof(buffer), "Qtde: %d", num_balls);
    ssd1306_draw_string(&disp, 68, 0, 1, buffer);
}

// Desenhar número de bolas por slot
void draw_slot_counts(int slots[NUM_SLOTS]) {
    if (NUM_SLOTS <= 10) { 
        for (int i = 0; i < NUM_SLOTS; i++) {
            char buffer[4];
            snprintf(buffer, sizeof(buffer), "%d", slots[i]);
            int x = i * SLOT_WIDTH;
            ssd1306_draw_string(&disp, x, 57, 1, buffer); 
        }
    }
}

// Desenhar o frame
void draw_frame() {
    ssd1306_clear(&disp);
    draw_histogram_and_gauss(slots);
    draw_ball_count(total_balls);
    draw_slot_counts(slots);
    for (int i = 0; i < MAX_BALLS; i++) {
        if (balls[i].active) {
            ssd1306_draw_char(&disp, balls[i].final_x, (int)balls[i].y, 1, 'o');
        }
    }
    for (int i = 0; i < NUM_SLOTS; i++) {
        int pixel_x = i * SLOT_WIDTH;
        int balls_to_draw = slots[i] > MAX_BALLS_PER_SLOT ? MAX_BALLS_PER_SLOT : slots[i];
        for (int j = 0; j < balls_to_draw; j++) {
            int stack_y = 55 - j; 
            if (stack_y >= 24) {
                ssd1306_draw_line(&disp, pixel_x, stack_y, pixel_x + SLOT_WIDTH - 4, stack_y);
            }
        }
    }
    for (int i = 0; i < NUM_SLOTS; i++) {
        int x = i * SLOT_WIDTH;
        ssd1306_draw_square(&disp, x, 55, SLOT_WIDTH - 3, 1); 
    }
    ssd1306_show(&disp);
}

int main() {
    stdio_init_all();
    sleep_ms(5000);
    setup_display();
    setup_buttons();
    srand(time(NULL));
    init_ball(0);
    ball_count = 1;
    int tick_counter = 0;

    while (1) {
        bias_left = !gpio_get(BUTTON_PIN_A);
        bias_right = !gpio_get(BUTTON_PIN_B);
        if (!gpio_get(SW) && !multiple_balls_mode) {
            multiple_balls_mode = true;
        }
        if (total_balls < MAX_TOTAL_BALLS) {
            if (multiple_balls_mode && tick_counter % 10 == 0 && ball_count < MAX_BALLS) {
                for (int i = 0; i < MAX_BALLS; i++) {
                    if (!balls[i].active) {
                        init_ball(i);
                        ball_count++;
                        break;
                    }
                }
            }
            if (!multiple_balls_mode && ball_count == 0) {
                init_ball(0);
                ball_count = 1;
            }
        }
        update_balls();
        draw_frame();
        tick_counter++;
        sleep_ms(TICK_INTERVAL_MS);
    }
    return 0;
}
