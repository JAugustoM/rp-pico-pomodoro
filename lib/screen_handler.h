#ifndef SCREEN_HANDLER_H
#define SCREEN_HANDLER_H

#include "../inc/ssd1306.h"
#include "pico/stdlib.h"

extern volatile uint8_t estado;
extern volatile uint8_t foco;
extern volatile uint8_t descanso;
extern volatile uint8_t ciclos;

extern char texto[8][17];

extern volatile uint16_t tempo_foco;
extern volatile uint16_t tempo_descanso;
extern volatile uint16_t ciclos_restante;

extern void setup_display_oled();
extern void draw_screen(char text[8][17]);
extern void update_line(char text[17], int line);
extern void get_screen_text();
extern void change_screen_state(uint state);

#endif