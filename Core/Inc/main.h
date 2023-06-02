/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32g0xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct
		{
			int ADC1_IT;
			int TIM7_IT;
			int TIM3_IT;
			int TIM6_IT;
			int TIM17_IT;
			int TIM17_ON_OFF;
			int TIM16_IT;
			int Encoder_BF;
			int TIM14_IT;
			int ERROR_Flag;
			int SLEEP_ON_OFF;
			int Current_error;
			float FAN_LEVEL;

		}IT_Flag;
extern IT_Flag Flag;

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SYS_LED_Pin GPIO_PIN_12
#define SYS_LED_GPIO_Port GPIOC
#define Y1_Pin GPIO_PIN_13
#define Y1_GPIO_Port GPIOC
#define Y2_Pin GPIO_PIN_14
#define Y2_GPIO_Port GPIOC
#define Y3_Pin GPIO_PIN_15
#define Y3_GPIO_Port GPIOC
#define LCD_CS_Pin GPIO_PIN_0
#define LCD_CS_GPIO_Port GPIOC
#define LCD_DC_Pin GPIO_PIN_1
#define LCD_DC_GPIO_Port GPIOC
#define LCD_RES_Pin GPIO_PIN_2
#define LCD_RES_GPIO_Port GPIOC
#define AT25_CS_Pin GPIO_PIN_3
#define AT25_CS_GPIO_Port GPIOC
#define miniboard1_Pin GPIO_PIN_2
#define miniboard1_GPIO_Port GPIOA
#define miniboard2_Pin GPIO_PIN_3
#define miniboard2_GPIO_Port GPIOA
#define softstart_Pin GPIO_PIN_5
#define softstart_GPIO_Port GPIOC
#define ADC1_CH0_temp1_Pin GPIO_PIN_0
#define ADC1_CH0_temp1_GPIO_Port GPIOB
#define ADC1_CH1_temp2_Pin GPIO_PIN_1
#define ADC1_CH1_temp2_GPIO_Port GPIOB
#define OTP1_Pin GPIO_PIN_11
#define OTP1_GPIO_Port GPIOB
#define OTP2_Pin GPIO_PIN_12
#define OTP2_GPIO_Port GPIOB
#define FAN_CRTL1_Pin GPIO_PIN_14
#define FAN_CRTL1_GPIO_Port GPIOB
#define FAN_CTRL2_Pin GPIO_PIN_15
#define FAN_CTRL2_GPIO_Port GPIOB
#define Encoder_enter_Pin GPIO_PIN_8
#define Encoder_enter_GPIO_Port GPIOA
#define X1_Pin GPIO_PIN_0
#define X1_GPIO_Port GPIOD
#define X2_Pin GPIO_PIN_1
#define X2_GPIO_Port GPIOD
#define X1D2_Pin GPIO_PIN_2
#define X1D2_GPIO_Port GPIOD
#define X4_Pin GPIO_PIN_3
#define X4_GPIO_Port GPIOD
#define X5_Pin GPIO_PIN_4
#define X5_GPIO_Port GPIOD
#define X6_Pin GPIO_PIN_5
#define X6_GPIO_Port GPIOD
#define X7_Pin GPIO_PIN_6
#define X7_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
