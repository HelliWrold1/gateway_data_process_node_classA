#ifndef _IIC_H_
#define _IIC_H_
#include "tremo_cm4.h"
#include "tremo_rcc.h"
#include "tremo_gpio.h"
#include "tremo_delay.h"
#include "tremo_i2c.h"
#include "gwlog.h"

#define SCL_PIN GPIO_PIN_8
#define SDA_PIN GPIO_PIN_11

#define SDA_OUT() gpio_init(GPIOA,SDA_PIN, GPIO_MODE_OUTPUT_PP_HIGH)
#define SDA_IN() gpio_init(GPIOA,SDA_PIN, GPIO_MODE_INPUT_PULL_UP)
#define IIC_SDA(x) ((x==1)?gpio_write(GPIOA,SDA_PIN,GPIO_LEVEL_HIGH):gpio_write(GPIOA,SDA_PIN,GPIO_LEVEL_LOW))
#define IIC_CLK(x) ((x==1)?gpio_write(GPIOA,SCL_PIN,GPIO_LEVEL_HIGH):gpio_write(GPIOA,SCL_PIN,GPIO_LEVEL_LOW))
#define IIC_READ_BIT() gpio_read(GPIOA,SDA_PIN)
#define IIC_ACK (1)
#define IIC_NACK (0)

void IIC_GPIOInit(void);
void IIC_Start(void);
void IIC_Stop(void);
u8 IIC_WaitAck(void);
void IIC_SendByte(u8 dat);
u8 IIC_RecvByte(u8 ack);
void IIC_HardInit(void);

#endif