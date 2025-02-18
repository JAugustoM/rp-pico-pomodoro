#include "hardware/gpio.h"
#include "led_handler.h"

const uint LED_GREEN = 11;
const uint LED_BLUE = 12;
const uint LED_RED = 13;

void setup_leds() {
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);

    gpio_init(LED_BLUE);
    gpio_set_dir(LED_BLUE, GPIO_OUT);

    gpio_init(LED_GREEN);
    gpio_set_dir(LED_GREEN, GPIO_OUT);
}

void turn_red() {
    gpio_put(LED_RED, true);
    gpio_put(LED_BLUE, false);
    gpio_put(LED_GREEN, false);
}

void turn_blue() {
    gpio_put(LED_RED, false);
    gpio_put(LED_BLUE, true);
    gpio_put(LED_GREEN, false);
}

void turn_green() {
    gpio_put(LED_RED, false);
    gpio_put(LED_BLUE, false);
    gpio_put(LED_GREEN, true);
}

void turn_off() {
    gpio_put(LED_RED, false);
    gpio_put(LED_BLUE, false);
    gpio_put(LED_GREEN, false);
}