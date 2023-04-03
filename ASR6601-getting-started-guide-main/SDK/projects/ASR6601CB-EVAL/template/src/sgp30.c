#include "sgp30.h"

void sgp30_Init(void)
{
		IIC_Start();
		IIC_SendByte(0xB0); // send address 0x58<<1 | 0x00
		if(IIC_WaitAck())
		{
			GW_LOG(LOG_ERROR, "sgp30 iic error");
		}
		IIC_SendByte(0x20);
		IIC_WaitAck();
		IIC_SendByte(0x03);
		IIC_WaitAck();
		IIC_Stop();
		delay_ms(15000); // init for 15s
}

void sgp30_SetHumi(void)
{
	// TODO feed humi
}

u8 sgp30_Get(int *co2, int *tvoc)
{
		IIC_Start();
		IIC_SendByte(0xB0); // send address 0x58<<1 | 0x00
		if(IIC_WaitAck())
		{
			GW_LOG(LOG_ERROR, "sgp30 iic error");
		}
		// send measure command
		IIC_SendByte(0x20);
		IIC_WaitAck();
		IIC_SendByte(0x08);
		IIC_WaitAck();
		IIC_Stop();
		delay_ms(1000);
	
		// read
		uint32_t data;
		uint8_t crc;
		IIC_Start();
		IIC_SendByte(0xB1); // send address 0x58<<1 | 0x01
		if(IIC_WaitAck())
		{
			GW_LOG(LOG_ERROR, "sgp30 iic error");
		}
		data = IIC_RecvByte(1);
		data <<= 8;
		data |= IIC_RecvByte(1);
		crc = IIC_RecvByte(1);
		crc = crc;
		data <<= 8;
		data |= IIC_RecvByte(1);
		data <<= 8;
		data |= IIC_RecvByte(0);
		IIC_Stop();
		*co2 = (data & 0xffff0000) >> 16;
		*tvoc = data & 0x0000ffff;
		GW_LOG(LOG_DEBUG, "CO2: %d ppm, TVOC: %d ppb", *co2, *tvoc);
		return 0;
}