#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "lib/pins_definition.h"
#include "lib/screen_handler.h"
#include "lib/buttons_handler.h"
#include "lib/joystick_handler.h"

int main()
{
    stdio_init_all();
    setup_display_oled();
    setup_buttons();
    setup_joystick();
    
    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, true);

    change_screen_state(estado);

    while (true) {}

    return 0;
}
