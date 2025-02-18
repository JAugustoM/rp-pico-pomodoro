#include "lib/screen_handler.h"
#include "lib/buttons_handler.h"
#include "lib/joystick_handler.h"
#include "lib/led_handler.h"
#include "lib/buzzer_handler.h"
#include "lib/memory_handler.h"

int main()
{
    stdio_init_all();
    initialize_memory();
    setup_display_oled();
    setup_buttons();
    setup_joystick();
    setup_leds();
    setup_buzzer();

    change_screen_state(estado);

    while (true) {}

    return 0;
}
