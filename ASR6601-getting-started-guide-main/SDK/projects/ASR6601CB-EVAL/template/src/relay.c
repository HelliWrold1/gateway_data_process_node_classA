#include "relay.h"

/**
 * @brief Init relay gpio pin
 * @param gpiox Select the GPIO peripheral number(GPIOA, GPIOB, GPIOC and GPIOD)
 * @param gpio_pin Select the GPIO pin number. @ref tremo_gpio.h
 * @retval None
 */
void relay_gpio_init(gpio_t* gpiox, uint8_t gpio_pin)
{
	gpio_set_iomux(gpiox, gpio_pin, 0);
	gpio_init(gpiox, gpio_pin, GPIO_MODE_OUTPUT_PP_LOW);
}

/**
 * @brief Give the relay pin a high level
 * @param gpiox Select the GPIO peripheral number(GPIOA, GPIOB, GPIOC and GPIOD)
 * @param gpio_pin Select the GPIO pin number. @ref tremo_gpio.h 
 * @retval None
 */
void relay_on(gpio_t* gpiox, uint8_t gpio_pin)
{
	gpio_write(gpiox,gpio_pin,GPIO_LEVEL_HIGH);
}

/**
 * @brief Give the relay pin a low level
 * @param gpiox Select the GPIO peripheral number(GPIOA, GPIOB, GPIOC and GPIOD)
 * @param gpio_pin Select the GPIO pin number. @ref tremo_gpio.h
 * @retval None
 */
void relay_off(gpio_t* gpiox, uint8_t gpio_pin)
{
	gpio_write(gpiox,gpio_pin,GPIO_LEVEL_LOW);
}