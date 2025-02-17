#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/timer.h"
#include "hardware/adc.h"
#include "../inc/ssd1306.h"

void cancel_timer();
void setup_repeating_timer();
void setup_repeating_timer_joystick();
void change_screen_state(uint state);

struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

uint8_t* ssd;
volatile uint8_t estado = 0;
volatile uint8_t foco = 25;
volatile uint8_t descanso = 5;
volatile uint8_t ciclos = 3;

char texto[8][17];

volatile uint16_t tempo_foco;
volatile uint16_t tempo_descanso;
volatile uint16_t ciclos_restante;

struct repeating_timer timer;
volatile bool cancelled = true;

void setup_display_oled() {
    calculate_render_area_buffer_length(&frame_area);
    ssd = malloc(sizeof(uint8_t) * ssd1306_buffer_length);

    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();

    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

void draw_screen(char text[8][17]) {
    memset(ssd, 0, ssd1306_buffer_length);
    int y = 0;

    for (uint i = 0; i < 8; i++) {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }

    render_on_display(ssd, &frame_area);
}

void get_screen_text() {
    if (estado == 0) {
        if (!cancelled) {
            cancel_timer();
        } 

        ciclos_restante = ciclos;
        char text[8][17] = {
            "A Comecar       ",
            "B Configurar    ",
            "                ",
            "Configuracoes   ",
            "                ",
        };

        snprintf(text[5], 17, "  Foco     % 2um  ", foco);
        snprintf(text[6], 17, "  Descanso % 2um  ", descanso);
        snprintf(text[7], 17, "  Ciclos    % 2u  ", ciclos);

        for (int i = 0; i < 8; i++) {
            strcpy(texto[i], text[i]);
        }

        ciclos_restante = ciclos;
    } else if (estado == 1) {
        if (!cancelled) {
            cancel_timer();
        } 

        if (ciclos_restante > 0) ciclos_restante--;
        else {
            //printf("Reset\n");
            change_screen_state(0);
            return;
        }
        tempo_foco = foco;
        char text[8][17] = {
            "Foco            ",
            "                ",
            "Tempo Restante  ",
            "XX minutos      ",
            "                ",
            "                ",
            "A Inicio        ",
            "B Parar/Iniciar ",
        };

        snprintf(text[3], 17, "  % 2u minutos    ", foco);

        for (int i = 0; i < 8; i++) {
            strcpy(texto[i], text[i]);
        }

        setup_repeating_timer();
    } else if (estado == 2) {
        if (!cancelled) {
            cancel_timer();
        } 
        tempo_descanso = descanso;
        char text[8][17] = {
            "Descanso        ",
            "                ",
            "Tempo Restante  ",
            "XX minutos      ",
            "                ",
            "                ",
            "A Inicio        ",
            "B Parar/Iniciar ",
        };

        snprintf(text[3], 17, "  % 2u minutos    ", descanso);

        for (int i = 0; i < 8; i++) {
            strcpy(texto[i], text[i]);
        }
        setup_repeating_timer();
    } else if (estado == 3) {
        if (!cancelled) {
            //printf("Joystick\n");
            cancel_timer();
            setup_repeating_timer_joystick();
        } else {
            //printf("Joystick\n");
            setup_repeating_timer_joystick();
        }
        
        char text[8][17] = {
            "Configuracao    ",
            "                ",
            "Foco            ",
            "XX minutos      ",
            "                ",
            "A Inicio        ",
            "B Descanso      ",
            "                ",
        };

        snprintf(text[3], 17, "  % 2u minutos    ", foco);

        for (int i = 0; i < 8; i++) {
            strcpy(texto[i], text[i]);
        }
    } else if (estado == 4) {
        if (!cancelled) {
            //printf("Joystick\n");
            cancel_timer();
            setup_repeating_timer_joystick();
        } else {
            //printf("Joystick\n");
            setup_repeating_timer_joystick();
        }

        char text[8][17] = {
            "Configuracao    ",
            "                ",
            "Descanso        ",
            "XX minutos      ",
            "                ",
            "A Foco          ",
            "B Ciclos        ",
            "                ",
        };

        snprintf(text[3], 17, "  % 2u minutos    ", descanso);

        for (int i = 0; i < 8; i++) {
            strcpy(texto[i], text[i]);
        }
    } else if (estado == 5) {
        if (!cancelled) {
            //printf("Joystick\n");
            cancel_timer();
            setup_repeating_timer_joystick();
        } else {
            //printf("Joystick\n");
            setup_repeating_timer_joystick();
        }

        char text[8][17] = {
            "Configuracao    ",
            "                ",
            "Ciclos          ",
            "XX ciclos       ",
            "                ",
            "A Descanso      ",
            "B Inicio        ",
            "                ",
        };

        snprintf(text[3], 17, "  % 2u ciclos     ", ciclos);

        for (int i = 0; i < 8; i++) {
            strcpy(texto[i], text[i]);
        }
    }
}

void change_screen_state(uint state) {
    estado = state;
    
    get_screen_text();

    if (texto != NULL) {
        draw_screen(texto);
    }
}

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