#include "bh1750.h"

u8 bh1750_Start(void)
{
	IIC_Start();
	IIC_SendByte(0x46);
	if(IIC_WaitAck())
	{
		GW_LOG(LOG_ERROR, "bh1750 ack error");
		return 1;
	}
	IIC_SendByte(0x01); // power
	IIC_WaitAck();
	IIC_Stop();
	return 0; // no error
}

float bh1750_Get(void)
{
	uint16_t buf[2] = {0,0};
	uint16_t value = 0;
	float temp = 0.0;
	
	// reset register
	IIC_Start();
	IIC_SendByte(0x46);
	IIC_WaitAck();
	IIC_SendByte(0x07);
	IIC_WaitAck();
	IIC_Stop();
	
	IIC_Start();
	IIC_SendByte(0x46);
	IIC_WaitAck();
	IIC_SendByte(0x10);
	IIC_WaitAck();
	IIC_Stop();
	delay_ms(200);
	
	IIC_Start();
	IIC_SendByte(0x47);
	IIC_WaitAck();
	buf[0] = IIC_RecvByte(1);
	buf[1] = IIC_RecvByte(0);
	IIC_Stop();
	delay_ms(5);
	value = buf[0];
	value = (value<<8) | buf[1];
	temp = (float)value/1.2;
	GW_LOG(LOG_DEBUG, "soft iic bh1750: %.3f lux",temp);
	return temp;
}