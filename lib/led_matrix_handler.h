#ifndef LED_MATRIX_HANDLER_H
#define LED_MATRIX_HANDLER_H

struct pixel_t {
    uint8_t G, R, B;
};

typedef struct pixel_t pixel_t;
typedef pixel_t npLED_t; // Mudança de nome de "struct pixel_t" para "npLED_t", por clareza.

extern void setup_led_matrix();
extern void update_led_matrix(bool pause);

#endif