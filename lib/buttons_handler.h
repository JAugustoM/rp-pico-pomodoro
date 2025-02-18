#ifndef BUTTONS_HANDLER_H
#define BUTTONS_HANDLER_H

#include "hardware/gpio.h"

extern void buttons_callback(uint gpio, uint32_t events);
extern void setup_buttons();

#endif