#include "lib/screen_handler.h"
#include "lib/buttons_handler.h"
#include "lib/joystick_handler.h"
#include "lib/led_handler.h"
#include "lib/buzzer_handler.h"
#include "lib/memory_handler.h"
#include "lib/led_matrix_handler.h"

int main()
{
    stdio_init_all();
    initialize_memory();
    setup_display_oled();
    setup_buttons();
    setup_joystick();
    setup_leds();
    setup_buzzer();
    setup_led_matrix();

    change_screen_state(estado);

    while (true) {
        // Todo o código funciona de forma assincrona
        // baseado em interrupts e timers, logo o laço
        // while serve apenas para que o código não termine
    }

    return 0;
}
