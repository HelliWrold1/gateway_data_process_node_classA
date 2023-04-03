#ifndef _ADS1115_H_
#define _ADS1115_H_

#include "iic.h"
#include "gwlog.h"
#include <math.h>

#define ADS1115_ADDRESS_0 (0x48) //ADDR PIN ->GND
#define ADS1115_ADDRESS_1 (0x49) //ADDR PIN ->VDD
#define ADS1115_ADDRESS_2 (0x50) //ADDR PIN ->SDA
#define ADS1115_ADDRESS_3 (0x51) //ADDR PIN ->SCL

#define OS_MASK                                                  (0x80)
#define OS_START_OR_CONVERTING                                   (0x80)
#define OS_CONVERSTION                                           (0x00)

#define MUX_MASK                                                 (0x70)
#define MUX_AIN0_GND                                             (0x40)
#define MUX_AIN1_GND                                             (0x50)
#define MUX_AIN2_GND                                             (0x60)
#define MUX_AIN3_GND                                             (0x70)

#define PGA_MASK                                                 (0x0E)
#define PGA_6V                                                   (0x00) // 6V

#define MODE_MASK                                                (0x01)
#define MODE_CONTINUE                                            (0x00)
#define MODE_SINGLE                                              (0x01)

#define DR_MASK                                                  (0xE0)
#define DR_8SPS                                                  (0x00)
#define DR_16SPS                                                 (0x20)
#define DR_32SPS                                                 (0x40)
#define DR_64SPS                                                 (0x60)
#define DR_128SPS                                                (0x80)
#define DR_250SPS                                                (0xA0)
#define DR_475SPS                                                (0xC0)
#define DR_860SPS                                                (0xE0)

#define COMP_MODE_MASK                                           (0x10)
#define COMP_MODE_TRAD                                           (0x00)
#define COMP_MODE_WINDOW                                         (0x10)

#define COMP_POL_ACTIVE_LO                                       (0x00)
#define COMP_LAT_NON_LATCH                                       (0x00)
#define COMP_QUE_DISABLE                                         (0x03)

#define ADS1115_ADDRESS ADS1115_ADDRESS_0 //ADDR PIN ->GND
#define ADS1115_ADDRESS_W (ADS1115_ADDRESS<<1|0x00) //write address
#define ADS1115_ADDRESS_R (ADS1115_ADDRESS<<1|0x01) //read address

#define ADS1115_POINT_CONVERTION (0x00) // point to conversion register
#define ADS1115_POINT_CONFIG (0x01) // point to config register

uint8_t ads1115Config(uint8_t highCmd, uint8_t lowCmd);
uint16_t recvConvertionVal();
uint16_t getAD(uint8_t highCmd,uint8_t lowCmd);
void getADS1115value(double *v0, double *v1, double *v2, double *v3);
void VoltageToPPM(double *nh3, double *h2s, double *co);

#endif