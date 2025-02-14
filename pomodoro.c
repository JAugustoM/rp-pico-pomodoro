#include <stdio.h>
#include "pico/stdlib.h"

#define LED_RED 13


int main()
{
    stdio_init_all();

    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, true);

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
