#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "pico/stdlib.h"
#include "screen_handler.h"
#include "led_handler.h"
#include "timer_handler.h"

const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

volatile uint8_t estado = 0;
volatile uint8_t foco = 25;
volatile uint8_t descanso = 5;
volatile uint8_t ciclos = 3;

struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

char texto[8][17];
uint8_t* ssd;

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

        turn_off();

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

        turn_blue();

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

        turn_green();

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