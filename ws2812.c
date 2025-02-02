//Inclusão das bibliotecas necessárias
#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

// Definição dos pinos utilizados
#define LED_RED 13
#define LED_GREEN 11
#define LED_BLUE 12
#define BUTTON_A 5
#define BUTTON_B 6
#define LED_MATRIX 7

// Variáveis globais para armazenar o número atual e estados dos botões
volatile int number = 0;
volatile bool button_a_pressed = false;
volatile bool button_b_pressed = false;

// Função de debounce separada para cada botão
void debounce(uint gpio, uint32_t events) {
    static absolute_time_t last_press_time_A = 0;
    static absolute_time_t last_press_time_B = 0;
    absolute_time_t now = get_absolute_time();
    
    if (gpio == BUTTON_A && absolute_time_diff_us(last_press_time_A, now) > 200000) {
        last_press_time_A = now;
        button_a_pressed = true;
    }
    else if (gpio == BUTTON_B && absolute_time_diff_us(last_press_time_B, now) > 200000) {
        last_press_time_B = now;
        button_b_pressed = true;
    }
}

// Função para piscar o LED vermelho a cada 200 ms (5 Hz)
bool blink_red_led(struct repeating_timer *t) {
    static bool state = false;
    gpio_put(LED_RED, state);
    state = !state;
    return true;
}

// Inicialização do protocolo WS2812 para controlar a matriz de LEDs
void ws2812_init(uint gpio) {
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);
    ws2812_program_init(pio, sm, offset, gpio, 800000, false);
}

// Função para enviar cor a um LED WS2812 específico
void put_pixel(uint32_t pixel_grb) {
    sleep_us(50);  // Pequeno atraso para garantir atualização
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Exibição dos números de 0 a 9 na matriz de LEDs corretamente orientados
void ws2812_display_number(int num) {
    static const uint32_t digits[10][5] = {
        {0x1F, 0x11, 0x11, 0x11, 0x1F},
        {0x04, 0x0C, 0x04, 0x04, 0x0E},
        {0x1F, 0x01, 0x1F, 0x10, 0x1F},
        {0x1F, 0x01, 0x1F, 0x01, 0x1F},
        {0x11, 0x11, 0x1F, 0x01, 0x01},
        {0x1F, 0x10, 0x1F, 0x01, 0x1F},
        {0x1F, 0x10, 0x1F, 0x11, 0x1F},
        {0x1F, 0x01, 0x02, 0x04, 0x04},
        {0x1F, 0x11, 0x1F, 0x11, 0x1F},
        {0x1F, 0x11, 0x1F, 0x01, 0x1F}
    };
    
    for (int row = 4; row >= 0; row--) { // Inverte a ordem das linhas
        for (int col = 4; col >= 0; col--) { // Inverte a ordem das colunas para espelhar horizontalmente
            bool pixel_on = (digits[num][row] >> col) & 1;
            uint32_t color = pixel_on ? 0x00FF00 : 0x000000;
            put_pixel(color);
        }
    }
}

// Atualiza a matriz de LEDs com o número atual
void update_led_matrix() {
    ws2812_display_number(number);
}

int main() {
    stdio_init_all();

    // Configuração dos LEDs
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, 0);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
    gpio_put(LED_GREEN, 0);

    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);
    gpio_put(LED_BLUE, 0);

    // Configuração dos botões
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    // Configuração de interrupções para os botões
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, &debounce);
    gpio_set_irq_enabled_with_callback(BUTTON_B, GPIO_IRQ_EDGE_FALL, true, &debounce);

    // Configuração do timer para piscar o LED vermelho
    struct repeating_timer timer;
    add_repeating_timer_ms(-200, blink_red_led, NULL, &timer);

    // Inicializa a matriz de LEDs WS2812
    ws2812_init(LED_MATRIX);
    update_led_matrix();

    // Loop principal para monitorar os botões
    while (1) {
        if (button_a_pressed) {
            button_a_pressed = false;
            number = (number + 1) % 10;
            update_led_matrix();
        }

        if (button_b_pressed) {
            button_b_pressed = false;
            number = (number - 1 + 10) % 10;
            update_led_matrix();
        }
    }
}
