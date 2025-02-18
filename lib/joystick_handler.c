#include "hardware/adc.h"
#include "pico/stdlib.h"
#include "joystick_handler.h"

const int ADC_CHANNEL_0 = 0;
const uint JOY_X = 26;

void setup_joystick() {
  adc_init(); 
  adc_gpio_init(JOY_X); 
  adc_select_input(ADC_CHANNEL_0);
}