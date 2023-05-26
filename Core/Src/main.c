/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "PowerSupplyConfig.h"
#include "spi_oled.h"
#include "newkey.h"
#include "encoder.h"
#include "fonts.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
IT_Flag Flag;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void My_sys_Init(void);
void IT_handle(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_SPI1_Init();
  MX_TIM3_Init();
  MX_TIM6_Init();
  MX_TIM7_Init();
  MX_TIM15_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_ADC1_Init();
  MX_TIM16_Init();
  MX_TIM17_Init();
  MX_TIM14_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  My_sys_Init();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

	/*encoder&Cursor&key*/
	IT_handle();
	/*LCD update*/
	Display_Interface();

	Check_Error();

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 16;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV3;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void My_sys_Init(void)
{
	Flag.ADC1_IT=0;
	Flag.TIM7_IT=0;
	Flag.TIM3_IT=0;
	Flag.TIM6_IT=0;
	Flag.TIM7_IT=0;
	Flag.TIM17_IT=0;
	Flag.ERROR_Flag=0;
	Flag.Encoder_BF=0;
	/*main interface*/
	xyz.coordinates1=0;
	xyz.coordinates2=0;
	xyz.coordinates3=0;
	/*power off till soft start*/
	Power_AC_Off;
	/*Disable the key at first*/
	keys_EN = DISABLE;
	//PD0 PD2 key scan init
	SYSCFG_TypeDef  *SYScfg=SYSCFG;
	SYScfg->CFGR1|=(SYSCFG_CFGR1_UCPD2_STROBE);
	//PA8
	SYSCFG_TypeDef  *SYScfg1=SYSCFG;
	SYScfg1->CFGR1|=(SYSCFG_CFGR1_UCPD1_STROBE);
	/*other device init*/
	AT_25_init();
	AT25_Reset();
	AT25_load_AD_Param();
	OLED_Init();
	/*Cursor off*/
	__HAL_TIM_DISABLE(&htim6);
	/*check temperature*/
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)ADC_temp_buffer,2);
	/*start to communicate with mini board*/
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_IDLE);
	UART_Start_Receive_DMA(&huart2,Uart2_Receive_buffer,20);
	__HAL_UART_CLEAR_IT(&huart2,UART_CLEAR_IDLEF);
	/*Delay on soft start*/
	DAC_Cmd_send(1,2,0x01);
	HAL_Delay(10);
	DAC_Cmd_send(1,4,100);
	HAL_Delay(10);
	DAC_Cmd_send(1,5,100);
	HAL_Delay(10);
	/*all device initialize�?? soft start timer on，power on*/
	HAL_TIM_Base_Start_IT(&htim1);

}
/**
  * @brief
  * @retval None
  */
void IT_handle(void)
{

		if(Flag.TIM3_IT==1)
		{
			Encoder_Data_refresh();
			Flag.Encoder_BF=0;
		}
		/*key scan 100Hz*/
		if(Flag.TIM7_IT==1)
		{
			Flag.TIM7_IT=0;
			KEY_Handle();
		}
		/*Cursor flash*/
		if(Flag.TIM6_IT==1)
		{
			Flag.TIM6_IT=0;
			Cursor_flash();

		}
		if(Flag.ADC1_IT==1)
		{
			Flag.ADC1_IT=0;
			static float R1,R2,T;
			static uint16_t Voltage_Temperature[2];

			Voltage_Temperature[0]=__HAL_ADC_CALC_DATA_TO_VOLTAGE(3300,ADC_temp_buffer[0],ADC_RESOLUTION_12B);//mv
			Voltage_Temperature[1]=__HAL_ADC_CALC_DATA_TO_VOLTAGE(3300,ADC_temp_buffer[1],ADC_RESOLUTION_12B);//mv

			R1=Voltage_Temperature[0]/(Voltage_Temperature[0]-2970);
			R2=Voltage_Temperature[1]/(Voltage_Temperature[1]-2970);
			T=-6.25*(R1+R2)/2+87.5;//温度电阻曲线 Kohm
			sprintf(String_Temperature_CH1, "%6.2f", T);//update temperature
			Flag.FAN_LEVEL=99/5.0*T-485;//compare value
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_1,(uint16_t)Flag.FAN_LEVEL);//set compare value
			__HAL_TIM_SET_COMPARE(&htim15,TIM_CHANNEL_2,(uint16_t)Flag.FAN_LEVEL);
			/*过温判别*/
			if(Voltage_Temperature[0]<1500||Voltage_Temperature[1]<1500)
			{
				ON_OFF = OFF;
				OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
				Output_Zero();
				TIM16_DELAY_OFF();
				TIM17_CYCLE_OFF();
				Error |= OTP;
				xyz.coordinates1=4;
				xyz.coordinates2=1;
				xyz.coordinates3=0;
			}


		}
		if(Flag.TIM14_IT==1)
		{
			Flag.TIM14_IT=0;

		}
		if(Flag.TIM17_IT==1)
		{
			Flag.TIM17_IT=0;

		}


}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
