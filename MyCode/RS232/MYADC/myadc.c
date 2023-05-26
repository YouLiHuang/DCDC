#include "myadc.h"

uint16_t ADC_frequency;
float ADC_Gain_V=0.00253;
float ADC_Gain_I=0.00253;
float Eror_ADC_V=-0.2185;
float Eror_ADC_I=-0.2185;
/*改进版本*/
float Voltage_real=0.0;      //实际电压值，用于显示
float Current_real=0.0;      //实际电流值，用于显示

float Voltage_Data[100]={0};


void get_voltage_current(uint8_t mode)
{


	memset(Uart2_Receive_buffer,0,7);
	if(mode==READ_VOLTAGE)
	{
		DAC_Cmd_send(1,0x01,0x00);//发送读电压指令
	}
	else if(mode==READ_CURRENT)
	{
		DAC_Cmd_send(1,0x03,0x00);//发送读电压指令
	}



}



/*
	if(Voltage_real!=0)
	{
		Eror_ADC=(Uart2_Receive_buffer[3]*256+Uart2_Receive_buffer[4])-Voltage_real*ADC_Gain;//自校准
	}
*/

