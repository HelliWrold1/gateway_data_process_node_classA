#include "iic.h"

void delay(void)
{
	//delay_ms(200);
	delay_us(4);
}
/*
模拟IIC引脚初始化
IIC_CLK GPIO_PIN_8
IIC_SDA GPIO_PIN_11
*/
void IIC_GPIOInit(void)
{
	rcc_enable_peripheral_clk(RCC_PERIPHERAL_GPIOA, true);
	
	gpio_init(GPIOA,SDA_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
	gpio_init(GPIOA,SCL_PIN, GPIO_MODE_OUTPUT_PP_HIGH);
}

//发送起始信号
void IIC_Start(void)
{
	SDA_OUT();
	IIC_CLK(1);
	IIC_SDA(1);
	delay();
	IIC_SDA(0);
	delay();
	IIC_CLK(0);
}

//发送停止信号
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_CLK(0);
	IIC_SDA(0);
	delay();
	IIC_CLK(1);
	delay();
	IIC_SDA(1);
	delay();
}

//等待应答信号 0获取到ack  1没有获取到ack
u8 IIC_WaitAck(void)
{
	u8 ucErrTime=0;
	SDA_IN();
	IIC_SDA(1);
	delay();
	IIC_CLK(1);
	delay();
	while(IIC_READ_BIT())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			GW_LOG(LOG_ERROR, "WaitAck Error!");
			return 1;
		}
	}
	IIC_CLK(0);
	return 0;
}

//发送ack
void IIC_Ack(void)
{
	IIC_CLK(0);
	SDA_OUT();
	IIC_SDA(0);
	delay();
	IIC_CLK(1);
	delay();
	IIC_CLK(0);
}

//发送nAck
void IIC_NoAck(void)
{
	IIC_CLK(0);
	SDA_OUT();
	IIC_SDA(1);
	delay();
	IIC_CLK(1);
	delay();
	IIC_CLK(0);
}


//IIC发送一个字节
void IIC_SendByte(u8 dat)
{
	u8 t;
	SDA_OUT();
	for(t=0;t<8;t++)
	{
		IIC_SDA((dat&0x80)>>7);
		dat<<=1;//高位先发
		IIC_CLK(1);
		delay();
		IIC_CLK(0);
		delay();
	}
}

//IIC接收一个字节 ack=1,发送ack. ack=0,发送nack
u8 IIC_RecvByte(u8 ack)
{
	u8 i,dat=0;
	SDA_IN();
	for(i=0;i<8;i++)
	{
		IIC_CLK(0);
		delay();
		IIC_CLK(1);
		dat<<=1;
		if(IIC_READ_BIT())dat++;
		delay();
	}
	if(!ack)IIC_NoAck();//发送nACK
	else IIC_Ack();//发送ACK

	return dat;
}