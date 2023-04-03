#include "tremo_gpio.h"

void relay_gpio_init(gpio_t* gpiox, uint8_t gpio_pin);
void relay_on(gpio_t* gpiox, uint8_t gpio_pin);
void relay_off(gpio_t* gpiox, uint8_t gpio_pin);