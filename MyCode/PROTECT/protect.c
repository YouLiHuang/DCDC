#include "protect.h"

uint8_t Error;
uint16_t ADC_temp_buffer[2];

uint8_t OVP_InputOrOutput_Flag = 2;   //0代表输入OVP，1代表输出OVP，2代表无报警
uint8_t OCP_SoftOrHard_Flag = 2;	  //0代表软件OCP，1代表硬件OCP，2代表无报警
uint8_t OTP_SensorX = 0;			  //1代表OTP1报警，2代表OTP2报警 0：不报警


/**
  * @brief  温度传感器检测的中断服务函数，检测IGBT温度，软件层保护
  * @param  None
  * @retval None
  */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{



	if(hadc==&hadc1)
	{
		Flag.ADC1_IT=1;
		HAL_ADC_Stop_DMA(&hadc1);


		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_temp_buffer,2);
	}

}


/**
  * @brief  OTP/OVP/OCP的硬件保护服务函数(硬件保护)
  * @param  None
  * @retval None
  */
void Check_Error()
{

	static uint8_t ERROR_Flag=1;
	static uint8_t ERROR_Flag2=1;
	static uint8_t ERROR_Flag3=1;
	static uint8_t ERROR_Flag4=1;

	if(Voltage_Actual>100)
	{


		Error|=OVP;
		xyz.coordinates1=4;
		xyz.coordinates2=1;
		xyz.coordinates3=1;
		if(ERROR_Flag==1)
		{
			ERROR_Flag=2;
			Output_Zero();
		}

	}
	else
	{
		ERROR_Flag=1;
		Error&=0xfe;//解除电压警报
	}
	if(Current_Actual>50)
	{

		Error|=OCP;
		xyz.coordinates1=4;
		xyz.coordinates2=1;
		xyz.coordinates3=1;
		if(ERROR_Flag2==1)
		{
			ERROR_Flag2=2;
			Output_Zero();
		}
	}
	else
	{
		ERROR_Flag2=1;
		Error&=0xfd;//解除电流警报
	}
	if(Current_Actual*Voltage_Actual>7500)
	{

		Error|=CPP;
		xyz.coordinates1=4;
		xyz.coordinates2=1;
		xyz.coordinates3=1;
		if(ERROR_Flag3==1)
		{
			ERROR_Flag3=2;
			Output_Zero();

		}
	}
	else
	{
		ERROR_Flag3=1;
		Error&=0xf7;//解除电流警报
	}
/*
	if(HAL_GPIO_ReadPin(OTP_PORT, OTP1_PIN)==GPIO_PIN_RESET||HAL_GPIO_ReadPin(OTP_PORT, OTP2_PIN)==GPIO_PIN_RESET)
	{
		Error |= OTP;
		if(HAL_GPIO_ReadPin(OTP_PORT, OTP1_PIN)==GPIO_PIN_RESET) OTP_SensorX=1;
		if(HAL_GPIO_ReadPin(OTP_PORT, OTP2_PIN)==GPIO_PIN_RESET) OTP_SensorX=2;
		ON_OFF = OFF;
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		TIM16_DELAY_OFF();
		TIM17_CYCLE_OFF();
		xyz.coordinates1=4;
		xyz.coordinates2=1;
		xyz.coordinates3=0;
		if(ERROR_Flag4==1)
		{
			ERROR_Flag4=2;
			Output_Zero();
		}

	}
	else
	{
		ERROR_Flag4=1;
		Error&=0xfb;//解除电流警报
	}
*/

	if(Voltage_Actual*100>0.95*Set_Voltage && Voltage_Actual*100<1.05*Set_Voltage)
	{
		CV_CC_CP=CV;
	}
	else if(Current_Actual*100>0.95*Set_Current && Current_Actual*100<1.05*Set_Current)
	{
		CV_CC_CP=CC;
	}




}

