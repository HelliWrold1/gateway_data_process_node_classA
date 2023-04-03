#ifndef _DHT11_H_
#define _DHT11_H_

#include "tremo_rcc.h"
#include "tremo_gpio.h"
#include "tremo_delay.h"
#include "gwlog.h"

#define DHT11_OUT() gpio_init(GPIOA,GPIO_PIN_9, GPIO_MODE_OUTPUT_PP_HIGH)
#define DHT11_IN() gpio_init(GPIOA,GPIO_PIN_9, GPIO_MODE_INPUT_FLOATING)
#define DHT11(x) ((x==1)?gpio_write(GPIOA,GPIO_PIN_9,GPIO_LEVEL_HIGH):gpio_write(GPIOA,GPIO_PIN_9,GPIO_LEVEL_LOW))
#define DHT11_READ() gpio_read(GPIOA,GPIO_PIN_9)

uint8_t dht11_Read(double *humi, double *temp);

#endif