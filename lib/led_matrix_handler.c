#include <math.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"
#include "led_matrix_handler.h"
#include "screen_handler.h"
#include "timer_handler.h"

#define LED_COUNT 25
#define LED_PIN 7

volatile uint ACTIVE_LEDS = 25;

volatile npLED_t leds[LED_COUNT];

PIO np_pio;
uint sm;

void npInit(uint pin) {
    uint offset = pio_add_program(pio0, &ws2818b_program);
    np_pio = pio0;

    sm = pio_claim_unused_sm(np_pio, false);
    if (sm < 0) {
        np_pio = pio1;
        sm = pio_claim_unused_sm(np_pio, true);
    }

    ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);

    for (uint i = 0; i < LED_COUNT; ++i) {
        leds[i].R = 0;
        leds[i].G = 0;
        leds[i].B = 0;
    }
}

void npSetLED(const uint index, npLED_t led) {
    leds[index].R = led.R;
    leds[index].G = led.G;
    leds[index].B = led.B;
}

void npClear() {
    npLED_t led = {0, 0, 0};
    for (uint i = 0; i < LED_COUNT; ++i) npSetLED(i, led);
}

void npWrite() {
    for (uint i = 0; i < LED_COUNT; ++i) {
        pio_sm_put_blocking(np_pio, sm, leds[i].G);
        pio_sm_put_blocking(np_pio, sm, leds[i].R);
        pio_sm_put_blocking(np_pio, sm, leds[i].B);
    }
}

void count_leds() {
    double total;
    double remaining;
    if (estado == 1) {
        total = foco * 60;
        remaining = tempo_foco;
    } else if (estado == 2) {
        total = descanso * 60;
        remaining = tempo_descanso;
    } else {
        total = 1;
        remaining = 0;
    }

    double percentage = remaining / total;
    ACTIVE_LEDS = ceil(LED_COUNT * percentage);
}

void setup_led_matrix() {
    npInit(LED_PIN);
    npClear();
} 

void update_led_matrix(bool pause) {
    npClear();
    count_leds();

    for (int i = 0; i < ACTIVE_LEDS; i++) {
        if (pause) {
            leds[i].R = 40;
            leds[i].G = 0;
            leds[i].B = 0;
        } else if (estado == 1) {
            leds[i].R = 0;
            leds[i].G = 0;
            leds[i].B = 40;
        } else if (estado == 2) {
            leds[i].R = 0;
            leds[i].G = 40;
            leds[i].B = 0;
        } else {
            leds[i].R = 0;
            leds[i].G = 0;
            leds[i].B = 0;
        }
    }

    npWrite();
}