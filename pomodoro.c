#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"

const uint LED_RED = 13;
const uint I2C_SDA = 14;
const uint I2C_SCL = 15;

struct render_area frame_area = {
    start_column : 0,
    end_column : ssd1306_width - 1,
    start_page : 0,
    end_page : ssd1306_n_pages - 1
};

uint estado = 0;
uint foco = 25;
uint descanso = 5;

void setup_display_oled(uint8_t* ssd) {
    i2c_init(i2c1, ssd1306_i2c_clock * 1000);
    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA);
    gpio_pull_up(I2C_SCL);

    ssd1306_init();

    memset(ssd, 0, ssd1306_buffer_length);
    render_on_display(ssd, &frame_area);
}

void draw_screen(uint8_t *ssd, char text[8][17]) {
    memset(ssd, 0, ssd1306_buffer_length);
    int y = 0;

    for (uint i = 0; i < 8; i++) {
        ssd1306_draw_string(ssd, 5, y, text[i]);
        y += 8;
    }

    render_on_display(ssd, &frame_area);
}

void get_screen_text(uint screen, char texto[8][17]) {
    if (screen == 0) {
        char text[8][17] = {
            "  A Comecar     ",
            "  B Configurar  ",
            "                ",
            "  Configuracoes ",
            "                ",
            "  Foco     25m  ",
            "  Descanso  5m  ",
            "  Ciclos     3  "
        };

        for (int i = 0; i < 8; i++) {
            strcpy(texto[i], text[i]);
        }
    }
}

int main()
{
    calculate_render_area_buffer_length(&frame_area);
    uint8_t* ssd = malloc(sizeof(uint8_t) * ssd1306_buffer_length);

    stdio_init_all();
    setup_display_oled(ssd);

    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
    gpio_put(LED_RED, true);

    char text[8][17];
    
    get_screen_text(estado, text);

    if (text != NULL) {
        draw_screen(ssd, text);
    }

    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }

    return 0;
}
