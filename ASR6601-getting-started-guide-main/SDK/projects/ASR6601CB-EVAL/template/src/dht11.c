#include "dht11.h"

void DHT11_GPIO_OUT()
{
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
	DHT11_OUT();
}

void DHT11_GPIO_IN()
{
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
	DHT11_IN();
}

uint8_t DHT11_Read_Byte(void)
{
	uint8_t temp;
	uint8_t ReadData=0;
	
	uint8_t retry = 0;	
	uint8_t i;
	
	for(i=0;i<8;i++)
	{
		// low level 50us
		while(DHT11_READ()==0&&retry<100)
		{		
			delay_us(1);
			retry++;		
		}
		retry=0;
		delay_us(30);
		temp=0;// digital signal 0,temp==0
		// digital signal 0:high level 28us, digital signal 1: high level 70us, delay 30us confirm the 0 or 1
		if(DHT11_READ()==1) temp=1;		
		while(DHT11_READ()==1&&retry<100) // digital signal 1:high level 40us
		{		
			delay_us(1);
			retry++;
		}
		retry=0;
		ReadData<<=1;
		ReadData|=temp;
	}	
	return ReadData;
}

uint8_t dht11_Read(double *humi, double *temp)
{
	uint8_t data[5];
	uint8_t sum = 0;
	uint8_t i;
	uint8_t retry = 0;

	// set dht11 gpio pp out, send start signal (pull down the level for 18ms, pull up the level for 40us)
	DHT11_GPIO_OUT();
	DHT11(0);
	delay_ms(18);
	DHT11(1);
	delay_us(40);
	// set dht11 gpio floating in
	DHT11_GPIO_IN();
	delay_us(20);
	// delay for 20us, pull down the level for 80us, surplus 60 us,check the low level
	if(DHT11_READ()==0)
	{
		while(DHT11_READ()==0&&retry<100) // receive the response signal,low level surplus 60us
		{
			delay_us(1);
			retry++;			
		}
		retry=0; // if over 100us auto run
		while(DHT11_READ()==1&&retry<100) // receive the response signal ,high level for 80us
		{
			delay_us(1);
			retry++;			
		}
		retry=0;
		// receive 8 bytes data
		for(i=0;i<5;i++)
		{
			data[i]=DHT11_Read_Byte();
		}
		delay_us(50); // pull down the bus for 50us to stop 
		
	}
	sum = data[0] + data[1] + data[2] + data[3];
	if(data[4] == sum)
	{
		GW_LOG(LOG_DEBUG, "dht11 raw data: %d.%d %%rh, %d.%d C", data[0], data[1], data[2], data[3]);
		*humi = data[0] + (double)data[1] / 10;
		*temp = data[2] + (double)data[3] / 10;
		return 0; // success
	}
	else
	{
		GW_LOG(LOG_ERROR, "get dht11 error");
		return 1; // error

	}
}