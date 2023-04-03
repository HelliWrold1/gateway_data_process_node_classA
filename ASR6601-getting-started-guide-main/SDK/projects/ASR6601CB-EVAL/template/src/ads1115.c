#include "ads1115.h"

uint8_t ads1115Config(uint8_t highCmd, uint8_t lowCmd)
{
    uint8_t i;
    uint8_t configData[4] = 
    {
        ADS1115_ADDRESS_W,
        ADS1115_POINT_CONFIG,
        highCmd,
        lowCmd,
    };

		IIC_Start();
    for ( i = 0; i < 4; i++)
    {
        IIC_SendByte(configData[i]);
				if(IIC_WaitAck())
				{
						GW_LOG(LOG_ERROR,"config ack error");
				}
    }
    IIC_Stop();
    return 0; // no error
}


uint16_t recvConvertionVal()
{
    uint16_t data;
    // Point to Conversion Register

    IIC_Start();
    IIC_SendByte(ADS1115_ADDRESS_W);
		if(IIC_WaitAck())
		{
				GW_LOG(LOG_ERROR,"conver ack error");
		}
    IIC_SendByte(ADS1115_POINT_CONVERTION);
		if(IIC_WaitAck())
		{
				GW_LOG(LOG_ERROR,"conver ack error");
		}
    IIC_Stop();

    // Read Convertion Register
    IIC_Start();
    IIC_SendByte(ADS1115_ADDRESS_R);
		if(IIC_WaitAck())
		{
				GW_LOG(LOG_ERROR,"conver ack error");
		}
    data = IIC_RecvByte(IIC_ACK);
    data = data << 8;
    data |= IIC_RecvByte(IIC_ACK);
    IIC_Stop();
    
    return data;
}


uint16_t getAD(uint8_t highCmd,uint8_t lowCmd)
{
    uint16_t value=0;
    ads1115Config(highCmd,lowCmd); // config channel
    delay_ms(500); // delay        
    value=recvConvertionVal(); // read data
		if(value == 0x7fff|| value== 0x8000)
		{
			GW_LOG(LOG_DEBUG,"out of the range");
		}
    return value;
}

void getADS1115value(double *v0, double *v1, double *v2, double *v3)
{
	uint8_t highCmd = OS_START_OR_CONVERTING|MUX_AIN0_GND|PGA_6V|MODE_SINGLE;
	uint8_t lowCmd = DR_128SPS|COMP_MODE_TRAD|COMP_POL_ACTIVE_LO|COMP_LAT_NON_LATCH|COMP_QUE_DISABLE;
	uint16_t value[4];
	highCmd &= ~MUX_MASK; // set mux bits 000
	highCmd |= MUX_AIN0_GND;
	value[0] = getAD(highCmd, lowCmd);
					GW_LOG(LOG_DEBUG, "%X\t%X", highCmd, lowCmd);
	highCmd &= ~MUX_MASK; // set mux bits 000
	highCmd |= MUX_AIN1_GND;
	value[1] = getAD(highCmd, lowCmd);
	highCmd &= ~MUX_MASK; // set mux bits 000
	highCmd |= MUX_AIN2_GND;
	value[2] = getAD(highCmd, lowCmd);
	highCmd &= ~MUX_MASK; // set mux bits 000
	highCmd |= MUX_AIN3_GND;
	value[3] = getAD(highCmd, lowCmd);
	*v0 = (double)value[0]*187.5/1000000.0;
	*v1 = (double)value[1]*187.5/1000000.0;
	*v2 = (double)value[2]*187.5/1000000.0;
	*v3 = (double)value[3]*187.5/1000000.0;
	GW_LOG(LOG_DEBUG, "ADS1115:| %lf | %lf | %lf | %lf |", *v0, *v1, *v2, *v3);
}
/**
 *voltage in,ppm out
 */
void VoltageToPPM(double *nh3, double *h2s, double *co)
{
	double v0, v1, v2;
	double RS,R0;
	v0 = *nh3;
	v1 = *h2s;
	v2 = *co;
	*nh3 = 410.74*pow(v0,5)-2718.5*pow(v0,4)+7015.6*pow(v0,3)-8747.9*pow(v0,2)+5271.7*v0-1228.7;
	*h2s = 450.25*pow(v1,5)-3049.4*pow(v1,4)+8088.3*pow(v1,3)-10422*pow(v1,2)+6524.1*v1-1586.1;

	v2 = ( (double)( (int)( (v2+0.005)*100 ) ) )/100;
	RS = (5.0f - v2) / v2 * 4.7;
	R0 = RS / pow(20 / 613.9f, 1 / -2.074f);
	*co = 613.9f * pow(RS/R0, -2.074f);
	GW_LOG(LOG_DEBUG, "AIR:| NH3: %lf ppm | H2S: %lf ppm | CO: %lf ppm |", *nh3, *h2s, *co);
}