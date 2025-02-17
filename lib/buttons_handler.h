#include "hardware/gpio.h"

volatile bool ba_state = false;
volatile bool bb_state = false;

void buttons_callback(uint gpio, uint32_t events) {
    if (gpio == 5) {
        gpio_put(LED_RED, false);
        if (estado == 0) change_screen_state(1);
        else if (estado == 1) change_screen_state(0);
        else if (estado == 2) change_screen_state(0);
        else if (estado == 3) change_screen_state(0);
        else if (estado == 4) change_screen_state(3);
        else if (estado == 5) change_screen_state(4);
    } else if (gpio == 6) {
        gpio_put(LED_RED, true);
        if (estado == 0) change_screen_state(3);
        else if (estado == 1) {
            printf("B\n");
            if (cancelled) setup_repeating_timer();
            else cancel_timer();
            
        }
        else if (estado == 2) {
            printf("B\n");
            if (cancelled) setup_repeating_timer();
            else cancel_timer();
        }
        else if (estado == 3) change_screen_state(4);
        else if (estado == 4) change_screen_state(5);
        else if (estado == 5) change_screen_state(0);
    }
}

void setup_buttons() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    
    gpio_set_irq_enabled_with_callback(BUTTON_A, GPIO_IRQ_EDGE_FALL, true, buttons_callback);
    gpio_set_irq_enabled(BUTTON_B, GPIO_IRQ_EDGE_FALL, true);

    gpio_pull_up(BUTTON_A);
    gpio_pull_up(BUTTON_B);
}