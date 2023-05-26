#ifndef _TIME_D_C_H_
#define _TIME_D_C_H_


#include "spi_oled.h"
#include "encoder.h"
#include "timeoled.h"
#include "myadc.h"
#include "PowerSupplyConfig.h"
#include "DAC.h"
#include "tim.h"
#include "gpio.h"
#include "adc.h"

#define Cycle_Function_Off       0
#define Cycle_Function_On        1

#define Delay_Function_Off           0
#define Delay_Function_On            1


extern uint8_t Cycle_Function_On_Off;
extern uint8_t Delay_Function_On_Off;
extern uint8_t Cycle_Function_On_Off;
extern uint32_t TIM_Delay_time;
extern uint32_t Cycle_On_Time;
extern uint32_t Cycle_Close_Time;

extern void TIM6_Protect_IT_Handle(void);
extern void TIM17_CYCLE_ON(void);
extern void TIM17_CYCLE_OFF(void);

extern void TIM16_DELAY_ON(void);
extern void TIM16_DELAY_OFF(void);

void TIM6_Protect_IT_Handle(void);


#endif
