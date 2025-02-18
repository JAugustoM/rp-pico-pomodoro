#include "buttons_handler.h"
#include "screen_handler.h"
#include "led_handler.h"
#include "timer_handler.h"
#include "led_matrix_handler.h"
#include "memory_handler.h"

const uint BUTTON_A = 5;
const uint BUTTON_B = 6;

volatile bool ba_state = false;
volatile bool bb_state = false;

volatile absolute_time_t last_press_time_a = 0;
volatile absolute_time_t last_press_time_b = 0;

void buttons_callback(uint gpio, uint32_t events) {
    if (gpio == 5) {
        if (absolute_time_diff_us(last_press_time_a, get_absolute_time()) > 200000) {
            last_press_time_a = get_absolute_time();
            if (estado == 0) change_screen_state(1);
            else if (estado == 1) change_screen_state(0);
            else if (estado == 2) change_screen_state(0);
            else if (estado == 3) {
                store_values(foco, descanso, ciclos);
                change_screen_state(0);
            }
            else if (estado == 4) change_screen_state(3);
            else if (estado == 5) change_screen_state(4);
        }
    } else if (gpio == 6) {
        if (absolute_time_diff_us(last_press_time_b, get_absolute_time()) > 200000) {
            last_press_time_b = get_absolute_time();
            if (estado == 0) change_screen_state(3);
            else if (estado == 1) {
                if (cancelled) {
                    update_led_matrix(false);
                    setup_repeating_timer();
                }
                else {
                    update_led_matrix(true);
                    cancel_timer();
                }
                
            }
            else if (estado == 2) {
                if (cancelled) {
                    update_led_matrix(false);
                    setup_repeating_timer();
                }
                else {
                    update_led_matrix(true);
                    cancel_timer();
                }
            }
            else if (estado == 3) change_screen_state(4);
            else if (estado == 4) change_screen_state(5);
            else if (estado == 5) {
                store_values(foco, descanso, ciclos);
                change_screen_state(0);
            }
        }
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