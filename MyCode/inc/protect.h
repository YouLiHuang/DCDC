#ifndef _PROTECT_H_
#define _PRORECT_H_



#include "spi_oled.h"
#include "myadc.h"
#include "PowerSupplyConfig.h"
#include "DAC.h"
#include "tim.h"
#include "adc.h"
#include "usart.h"


extern uint8_t Error;
extern uint8_t OVP_InputOrOutput_Flag;   //0代表输入OVP，1代表输出OVP，2代表无报警
extern uint8_t OCP_SoftOrHard_Flag;		 //0代表软件OCP，1代表硬件OCP，2代表无报警
extern uint8_t OTP_SensorX;					 //1代表OTP1报警，2代表OTP2报警 0：不报警
extern uint16_t ADC_temp_buffer[2];


extern void TIM1_Protect_IT_Handle(void);	 //软起动回调

extern void Check_Error(void);

#endif
