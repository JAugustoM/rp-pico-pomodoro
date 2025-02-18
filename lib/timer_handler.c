#include <stdio.h>
#include <string.h>
#include "hardware/adc.h"
#include "timer_handler.h"
#include "screen_handler.h"

struct repeating_timer timer;
volatile bool cancelled = true;

volatile uint16_t tempo_foco;
volatile uint16_t tempo_descanso;
volatile uint16_t ciclos_restante;

bool repeating_timer_callback(__unused struct repeating_timer *t) {
    if (estado == 1) {
        if (tempo_foco > 0) {
            tempo_foco--;
            char text[8][17];

            for (int i = 0; i < 8; i++) {
                strcpy(text[i], texto[i]);
            }

            snprintf(text[3], 17, "  % 2u minutos    ", tempo_foco);

            draw_screen(text);
            
            //printf("Tempo restante %u\n", tempo_foco);
            return true;
        } else {
            change_screen_state(2);
            return false;
        }
    } else if (estado == 2) {
        if (tempo_descanso > 0) {
            tempo_descanso--;

            char text[8][17];

            for (int i = 0; i < 8; i++) {
                strcpy(text[i], texto[i]);
            }

            snprintf(text[3], 17, "  % 2u minutos    ", tempo_descanso);

            draw_screen(text);

            //printf("Tempo restante %u\n", tempo_descanso);
            return true;
        } else {
            change_screen_state(1);
            return false;
        }
    }
}

bool repeating_timer_callback_joystick(__unused struct repeating_timer *t) {
    uint16_t joystick = adc_read();
    printf("Joystick %u\n", joystick);

    if (estado == 3) {
        printf("Foco %u\n", foco);
        if (joystick > 4000 && foco < 60) {
            foco++;
            char text[8][17];

            for (int i = 0; i < 8; i++) {
                strcpy(text[i], texto[i]);
            }

            snprintf(text[3], 17, "% 2u minutos      ", foco);

            draw_screen(text);
        }
        else if (joystick < 25 && foco > 1) {
            foco--;
            char text[8][17];

            for (int i = 0; i < 8; i++) {
                strcpy(text[i], texto[i]);
            }

            snprintf(text[3], 17, "% 2u minutos      ", foco);

            draw_screen(text);
        }
    }
    
    if (estado == 4) {
        printf("Descanso %u\n", descanso);
        if (joystick > 4000 && descanso < 60) {
            descanso++;
            char text[8][17];

            for (int i = 0; i < 8; i++) {
                strcpy(text[i], texto[i]);
            }

            snprintf(text[3], 17, "% 2u minutos      ", descanso);

            draw_screen(text);
        }
        else if (joystick < 25 && descanso > 1) {
            descanso--;
            char text[8][17];

            for (int i = 0; i < 8; i++) {
                strcpy(text[i], texto[i]);
            }

            snprintf(text[3], 17, "% 2u minutos      ", descanso);

            draw_screen(text);
        }
    }

    if (estado == 5) {
        printf("Ciclos %u\n", ciclos);
        if (joystick > 4000) {
            ciclos++;
            char text[8][17];

            for (int i = 0; i < 8; i++) {
                strcpy(text[i], texto[i]);
            }

            snprintf(text[3], 17, "% 2u ciclos       ", ciclos);

            draw_screen(text);
        }
        else if (joystick < 25 && ciclos > 1) {
            ciclos--;
            char text[8][17];

            for (int i = 0; i < 8; i++) {
                strcpy(text[i], texto[i]);
            }

            snprintf(text[3], 17, "% 2u ciclos       ", ciclos);

            draw_screen(text);
        }
    }

    return true;
}

void setup_repeating_timer_joystick() {
    cancelled = false; 
    add_repeating_timer_ms(200, repeating_timer_callback_joystick, NULL, &timer);
}

void setup_repeating_timer() {
    cancelled = false; 
    add_repeating_timer_ms(1000, repeating_timer_callback, NULL, &timer);
}

void cancel_timer() {
    cancelled = true;
    cancel_repeating_timer(&timer);
}