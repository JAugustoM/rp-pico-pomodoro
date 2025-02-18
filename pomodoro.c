#include "lib/screen_handler.h"
#include "lib/buttons_handler.h"
#include "lib/joystick_handler.h"
#include "lib/led_handler.h"

int main()
{
    stdio_init_all();
    setup_display_oled();
    setup_buttons();
    setup_joystick();
    setup_leds();

    change_screen_state(estado);

    while (true) {}

    return 0;
}
