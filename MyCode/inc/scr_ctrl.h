#ifndef _SCR_CTRL_
#define _SCR_CTRL_

#include"main.h"
#include "PowerSupplyConfig.h"

#define PWM0_HIGH	HAL_GPIO_WritePin(PWM0,PWM0PIN,GPIO_PIN_SET)
#define PWM0_LOW	HAL_GPIO_WritePin(PWM0,PWM0PIN,GPIO_PIN_RESET)
#define PWM0_R		HAL_GPIO_ReadPin(PWM0,PWM0PIN)

#define PWM1_HIGH	HAL_GPIO_WritePin(PWM1,PWM1PIN,GPIO_PIN_SET)
#define PWM1_LOW	HAL_GPIO_WritePin(PWM1,PWM1PIN,GPIO_PIN_RESET)
#define PWM1_R		HAL_GPIO_ReadPin(PWM1,PWM0PIN)

#endif
