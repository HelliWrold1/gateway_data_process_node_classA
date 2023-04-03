#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "timer.h"
#include "radio.h"
#include "tremo_uart.h"
#include "tremo_gpio.h"
#include "tremo_rcc.h"
#include "core_cm4.h"
#include "tremo_pwr.h"
#include "tremo_delay.h"
#include "rtc-board.h"
#include "tremo_adc.h"
#include "tremo_i2c.h"
#include "tremo_regs.h"
#include "tremo_system.h"
#include "relay.h"
#include "sgp30.h"
#include "bh1750.h"
#include "sha256.h"
#include "genTriplet.h"

extern int app_start(void);

void rcc_init(void)
{
	rcc_enable_oscillator(RCC_OSC_XO32K, true);
	rcc_set_adc_clk_source(RCC_ADC_CLK_SOURCE_RCO48M);
	
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOB, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOC, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOD, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_PWR, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_RTC, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_SAC, true);
    rcc_enable_peripheral_clk(RCC_PERIPHERAL_LORA, true);
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_UART0, true);
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_ADC, true);
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_I2C0, true);

	delay_ms(100);
    pwr_xo32k_lpm_cmd(true);
}
void uart_log_init(void)
{
    // uart0
    gpio_set_iomux(GPIOB, GPIO_PIN_0, 1);
    gpio_set_iomux(GPIOB, GPIO_PIN_1, 1);

    /* uart config struct init */
    uart_config_t uart_config;
    uart_config_init(&uart_config);

	//uart_config.fifo_mode = ENABLE;
    uart_config.baudrate = UART_BAUDRATE_115200;
    uart_init(UART0, &uart_config);
    uart_cmd(UART0, ENABLE);
}


int main(void)
{
	/* Add your application code here */
	// board init
	rcc_init();
	uart_log_init();
	IIC_GPIOInit();
	
	RtcInit();
	
	uart_config_interrupt(UART0,UART_INTERRUPT_RX_DONE,ENABLE);

	NVIC_SetPriority(UART0_IRQn, 2);
	NVIC_EnableIRQ(UART0_IRQn);
	
//	double nh3, h2s, co, v3;
//	int co2, tvoc;
//	double humi, temp;
	uint8_t NwkSKey[16];
	uint8_t AppSKey[16];
	uint32_t DevAddr;
	genTriplet(&DevAddr, NwkSKey, AppSKey); // gen Triplet info
	// print info
    GW_LOG(LOG_INFO, "ClassA Node For Collection");
	GW_LOG(LOG_INFO, "DevAddr: %08X",DevAddr);
	GW_LOG(LOG_INFO, "NwkSKey: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",NwkSKey[0], NwkSKey[1], NwkSKey[2], NwkSKey[3],\
																							NwkSKey[4], NwkSKey[5], NwkSKey[6], NwkSKey[7],\
																							NwkSKey[8], NwkSKey[9], NwkSKey[10], NwkSKey[11],\
																							NwkSKey[12], NwkSKey[13], NwkSKey[14], NwkSKey[15]);
	GW_LOG(LOG_INFO, "AppSKey: %02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X",AppSKey[0], AppSKey[1], AppSKey[2], AppSKey[3],\
																							AppSKey[4], AppSKey[5], AppSKey[6], AppSKey[7],\
																							AppSKey[8], AppSKey[9], AppSKey[10], AppSKey[11],\
																							AppSKey[12], AppSKey[13], AppSKey[14], AppSKey[15]);
																							
	GW_LOG(LOG_INFO, "Please wait for 15 sec, initializing SGP30 and BH1750...");
	sgp30_Init();
	bh1750_Start();
	GW_LOG(LOG_INFO, "Initialization completed!");
//	while(1)
//	{
//		getADS1115value(&nh3, &h2s, &co, &v3);
//		VoltageToPPM(&nh3, &h2s, &co);
//		sgp30_Get(&co2, &tvoc);
//		if (dht11_Read(&humi, &temp)) GW_LOG(LOG_ERROR, "dht11 error\r\n");
//			else GW_LOG(LOG_DEBUG, "humi: %.1lf %%rh, temp: %.1lf C\r\n", humi, temp);
//	}
	
	app_start();
	
}

#ifdef USE_FULL_ASSERT
void assert_failed(void* file, uint32_t line)
{
    (void)file;
    (void)line;

    while (1) { }
}
#endif
