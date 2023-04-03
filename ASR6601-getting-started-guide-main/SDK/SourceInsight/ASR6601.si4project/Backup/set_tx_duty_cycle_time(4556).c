#include "set_tx_duty_cycle_time.h"

/**
 * @brief Change TxDutyCycleTime,the TxDutyCycleTime must be less than 24 hours[such as:0x1E 0x17 0x3B 0x3B]
 * @Note the TxDutyCycleTime will be changed in DEVICE_STATE_CYCLE
 * @param TxDutyCycleTime Point to TxDutyCycleTime
 * @param cmd The instrution to change TxDutyCycleTime,it must be 0x1E
 * @param h Hours(<24)
 * @param m Minutes(<60)
 * @param s Seconds(<60)
 * @param AppData If the TxDutyCycleTime is queried, then the TxDutyCycleTime will be filled in the AppData
 * @param AppDataSize Point to AppDataSize
 * @param TxDutyCycleTimeQuery Point to TxDutyCycleTimeQuery Indicates if the TxDutyCycleTime has been queried
 * @retval Indicates whether the command has been executed[true: executed,false:error]
 * @author ZhaoZeqiang
 */
bool TxDutyCycleTime_instruction_analysis(uint32_t* TxDutyCycleTime, uint8_t cmd, uint8_t h, uint8_t m, uint8_t s, uint8_t* AppData, uint8_t* AppDataSize,bool* TxDutyCycleTimeQuery)
{
	if(cmd == 0x1E)
	{
		if(h == 0xFF && m == 0xFF && s == 0xFF)
		{// If query TxDutyCycleTime
//			printf("read time:%d\r\n",*TxDutyCycleTime); // Debug Code
			AppData[0] |= 0x08;
			AppData[16] = (*TxDutyCycleTime / 1000) % 60;
			AppData[15] = ( ( (*TxDutyCycleTime / 1000) - AppData[3] ) % 3600 ) / 60;
			AppData[14] = ( (*TxDutyCycleTime / 1000) - AppData[3] - AppData[2] * 60 ) / 3600;
			*AppDataSize = 4;
			*TxDutyCycleTimeQuery = true;
			return true;
		}
		else if(h == 0 && m == 0 && s == 0)
		{
			return false; // set default TxDutyCycleTime: 30s
		}
		else if(h < 24 && m < 60 && s < 60)
		{
			*TxDutyCycleTime = 0;
			*TxDutyCycleTime += h * 60 * 60 * 1000;
			*TxDutyCycleTime += m * 60 * 1000;
			*TxDutyCycleTime += s * 1000;
//			printf("set time:%d\r\n",*TxDutyCycleTime); // Debug Code
			return true;
		}
	}
	return true;
}