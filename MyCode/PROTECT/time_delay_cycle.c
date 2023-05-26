#include "time_delay_cycle.h"


uint8_t Cycle_Function_On_Off = Cycle_Function_Off;
uint8_t  Delay_Function_On_Off = Delay_Function_On;            //DELAY功能是否开启标志

uint32_t Cycle_On_Time;
uint32_t Cycle_Close_Time;
uint32_t TIM_Delay_time;


/**
  * @brief  开启DELAY功能，设置定时器时间和使能定时器
  * @param  None
  * @retval None
  */
void TIM16_DELAY_ON(void)
{
	TIM_Delay_time = Delay_h * 3600 + Delay_m * 60 + Delay_s;
	DELAY_NOT_DELAY = DELAY;
	ON_OFF = OFF;
	__HAL_TIM_SET_COUNTER(&htim16,0x00);
	__HAL_TIM_CLEAR_FLAG(&htim16,TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&htim16);

}

/**
  * @brief  关闭DELAY功能
  * @param  None
  * @retval None
  */
void TIM16_DELAY_OFF(void)
{
	DELAY_NOT_DELAY = NOT_DELAY;
	__HAL_TIM_DISABLE(&htim16);
}
/**
  * @brief  打开CYCLE，打开TIM17定时器
  * @param  None
  * @retval None
  */
void TIM17_CYCLE_ON(void)
{
	Cycle_On_Time = Cycle_On_s * 1000 + Cycle_On_ms;
	Cycle_Close_Time = (Cycle_Close_s * 1000 + Cycle_Close_ms);

	CYCLE_NOT_CYCLE = CYCLE;
	ON_OFF = OFF;
	__HAL_TIM_SET_COUNTER(&htim17,0x00);
	__HAL_TIM_CLEAR_FLAG(&htim17,TIM_FLAG_UPDATE);
	HAL_TIM_Base_Start_IT(&htim17);
	DAC_Cmd_send(1,0xff,0x00);

}
/**
  * @brief  关闭CYCLE，关闭TIM17定时器
  * @param  None
  * @retval None
  */
void TIM17_CYCLE_OFF(void)
{
	CYCLE_NOT_CYCLE = NOT_CYCLE;
	ON_OFF = OFF;
	__HAL_TIM_DISABLE(&htim17);
}

void Delay_Cycle_IT_Habdle(void)
{
	/*TIM6的中断服务函数，用于用于软启动，电压和电流，中断时间为0.1s，软开关时间为3s*/
	/*
	if(__HAL_TIM_DISABLE_IT(&htim6, TIM_IT_UPDATE)!=RESET)
	{
		TIM1_Protect_IT_Handle();
	}
	*/


}
