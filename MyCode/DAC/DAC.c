#include "DAC.h"

/*-------------------------------------------------------记录设定值相关变量-------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
float Set_Voltage_before=0;//记录上次的电压值，初始为0
float Set_Current_before=0;//记录上次的电流值，初始为0
/*------------------------------------------------------默认缓升模式相关变量------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------*/
/*改进后的版本*/
uint32_t V_Slope_default;//电压默认变化率
uint32_t I_Slope_default;//电流默认变化率


uint8_t V_Rise_On_Off = V_Rise_Off;
uint8_t V_Fall_On_Off = V_Fall_Off;
uint8_t I_Rise_On_Off = I_Rise_Off;
uint8_t I_Fall_On_Off = I_Fall_Off;


extern uint8_t Uart2_Send_buffer[7];

#ifdef CRC16_CHECK
const unsigned char auchCRCHi[]=
{
		0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
        0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
        0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
        0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
        0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
        0x40
};
const unsigned char auchCRCLo[] =
{
        0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
        0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
        0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
        0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
        0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
        0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
        0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
        0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
        0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
        0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
        0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
        0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
        0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
        0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
        0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
        0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
        0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
        0x40
};
uint16_t CRC16(uint8_t *puchMsg,uint8_t usDataLen )
{
		unsigned char uchCRCHi=0xFF;
		unsigned char uchCRCLo=0xFF;
		unsigned uIndex;
		while(usDataLen--)
		{
			uIndex =uchCRCLo^*puchMsg++;
			uchCRCLo=uchCRCHi^auchCRCHi[uIndex];
			uchCRCHi=auchCRCLo[uIndex];
		}
		return (uchCRCHi<<8|uchCRCLo);
}
#endif
uint16_t ADD(uint8_t *puchMsg,uint8_t usDataLen)
{

	uint16_t sum=0;
	uint8_t * pindex=puchMsg;
	for(uint8_t i=0;i<usDataLen;i++)
	{
		sum+=*(pindex++);
	}

	return sum;
}
/**
  * @brief  send command to mcu
  * @param  IDindex:machine id
  * @param  command
  * @param  Date need to be send
  * @retval
  */
void DAC_Cmd_send(uint8_t IDindex,uint8_t cmd,uint16_t Data_To_Send)
{

	__HAL_UART_ENABLE(&huart2);
	/*********************ID&Command*********************/
	Uart2_Send_buffer[0]=IDindex;//id
	Uart2_Send_buffer[1]=cmd;//cmd
	 /*********************data length*********************/
	 if((Data_To_Send&0xff00))
	 {
		 Uart2_Send_buffer[2]=0x02;
	 }
	 else if((Data_To_Send&0x00ff))
	 {
		 Uart2_Send_buffer[2]=0x01;
	 }
	 else Uart2_Send_buffer[2]=0x00;
	 /*********************data convert*********************/
	  Uart2_Send_buffer[3]=0x00;//data H
	  Uart2_Send_buffer[4]=0x00;//data L
	  switch(cmd)
	  {
		  case 0://pwm EN
		  case 1:break;//read voltage
		  case 2:break;//loop read
		  case 3:break;//read current

		  /*set max voltage current*/
		  case 4://set voltage
		  case 5://set current
		  {
			  Uart2_Send_buffer[4]|=(Data_To_Send&0x00ff);//low max_voltage
			  Uart2_Send_buffer[3]|=(Data_To_Send>>8);//Hight max_voltage
			  break;
		  }
		  /*set min voltage current*/
		  case 6://set voltage
		  case 7://set current
		  {
			  Uart2_Send_buffer[4]|=(Data_To_Send&0x00ff);//Low min_voltage
			  Uart2_Send_buffer[3]|=(Data_To_Send>>8);//high min_voltage
			  break;
		  }
		  /*set rate*/
		  case 8://set V rate
		  case 9://set I rate
		  {
			  Uart2_Send_buffer[4]=Data_To_Send;//Low :fall time
			  break;
		  }
		  /*rise/fall ctrl*/
		  case 10:
		  {
			  Uart2_Send_buffer[4]=Data_To_Send;//Low
			  break;
		  }//start Rise ON/OFF
		  case 11:break;//start Fall ON/OFF

		  default: break;
	  }
#ifdef CRC16_CHECK
	  /*********************CRC Calculate*********************/
	  Uart2_Send_buffer[5]=CRC16(Uart2_Send_buffer,5);//CRC L
	  Uart2_Send_buffer[6]=(CRC16(Uart2_Send_buffer,5)>>8);//CRC H
#endif
#ifdef ADD_CHECK
	  Uart2_Send_buffer[5]=ADD(Uart2_Send_buffer,5);//Low
	  Uart2_Send_buffer[6]=(ADD(Uart2_Send_buffer,5)>>8);//High
#endif

	  HAL_UART_Transmit_IT(&huart2,Uart2_Send_buffer,7);

}

/**
  * @brief  开始输出
  * @param  无
  * @retval 无
  */
void power_on(void)
{


	ON_OFF =ON;
	DAC_Cmd_send(1,4,Set_Voltage);
	HAL_Delay(10);
	DAC_Cmd_send(1,5,Set_Current);
}
/**
  * @brief  关闭输出
  * @param  无
  * @retval 无
  */
void power_off(void)
{

	ON_OFF =OFF;
	DAC_Cmd_send(1,4,100);
	for(uint8_t i=0;i<100;i++)
		;
	DAC_Cmd_send(1,5,100);

}
/**
  * @brief  输出零
  * @param  无
  * @retval 无
  */
void Output_Zero(void)
{

	DAC_Cmd_send(1,4,100);
	HAL_Delay(10);
	DAC_Cmd_send(1,5,100);

}
/**
  * @brief  默认缓升功能
  * @param  无
  * @retval 无
  */
void Slope_Function_On_default(void)
{
	/*默认缓升，输出打开*/
	ON_OFF = ON;
	DAC_Cmd_send(1,0x00,0x0000);//使能输出
	/*没有开用户缓升功能，使用默认电压缓升模式*/
	DAC_Cmd_send(1,0x07,V_Slope_Max);//设定默认斜率
	DAC_Cmd_send(1,0x00,0x0000);//使能输出
	/*没有开用户缓降功能，使用默认电流缓升模式*/
	DAC_Cmd_send(1,0x07,V_Slope_Default);//设定默认斜率
	DAC_Cmd_send(1,0x07,I_Slope_Default);//设定默认斜率
	DAC_Cmd_send(1,0x08,Set_Current);//用户设定的目标电流值
	DAC_Cmd_send(1,0x08,Set_Voltage);//用户设定的目标电流值
	DAC_Cmd_send(1,0x0a,0x00);
	DAC_Cmd_send(1,0x0a,0x02);
}
/**
  * @brief  默认缓降功能
  * @param  无
  * @retval 无
  */
void Slope_Function_Off_default(void)
{
	/*默认缓降，输出关闭*/
	ON_OFF = OFF;
	DAC_Cmd_send(1,0x00,0x0000);//使能输出
	/*没有开用户缓升功能，使用默认电压缓降模式*/
	DAC_Cmd_send(1,0x07,V_Slope_Max);//设定默认斜率
	/*没有开用户缓降功能，使用默认电流缓降模式*/
	DAC_Cmd_send(1,0x07,V_Slope_Default);//设定默认斜率
	DAC_Cmd_send(1,0x07,I_Slope_Default);//设定默认斜率
	DAC_Cmd_send(1,0x08,Set_Current);//用户设定的目标电流值
	DAC_Cmd_send(1,0x08,Set_Voltage);//用户设定的目标电流值
	DAC_Cmd_send(1,0x0a,0x01);
	DAC_Cmd_send(1,0x0a,0x03);

}

/**
  * @brief  用户设定缓升功能函数
  * @param  None
  * @retval None
  */
void Slope_Function_On(void)
{
	/*用户缓升，输出关闭*/
	ON_OFF = ON;
	/*用户电压缓升功能打开*/
	DAC_Cmd_send(1,0x08,0x00);//设定电压变化最低点
	DAC_Cmd_send(1,0x04,0x00);//设定电流变化最低点
	if (V_Rise_On_Off == V_Rise_On)
	{

		DAC_Cmd_send(1,0x08,Set_Voltage);//用户设定的目标电压值
		DAC_Cmd_send(1,0x07,Set_V_Slope);
		DAC_Cmd_send(1,0x00,0x0000);//使能输出
		DAC_Cmd_send(1,0x0a,0x0000);//使能电压缓升
	}
	/*用户电压缓升功能打开*/
	if (I_Rise_On_Off == I_Rise_On)
	{
		DAC_Cmd_send(1,0x08,Set_Current);//用户设定的目标电流值
		DAC_Cmd_send(1,0x07,Set_I_Slope);
		DAC_Cmd_send(1,0x00,0x0000);//使能输出
		DAC_Cmd_send(1,0x0a,0x0002);//使能电流缓升
	}
}

/**
  * @brief  用户设定缓降功能函数
  * @param  None
  * @retval None
  */
void Slope_Function_Off(void)
{
	/*用户缓降，输出关闭*/
	ON_OFF = OFF;
	/*用户电压缓降功能打开*/
	DAC_Cmd_send(1,0x08,0x00);//设定电压变化最低点
	DAC_Cmd_send(1,0x04,0x00);//设定电流变化最低点
	if (V_Fall_On_Off == V_Fall_On)
	{
		DAC_Cmd_send(1,0x08,Set_Voltage);//用户设定的目标电压值
		DAC_Cmd_send(1,0x07,Set_V_Slope);//设定用户电压变化率
		DAC_Cmd_send(1,0x00,0x0000);//使能输出
		DAC_Cmd_send(1,0x0a,0x0000);//使能电压缓降
	}
	/*用户电流缓降功能打开*/
	if (I_Fall_On_Off == I_Fall_On)
	{
		DAC_Cmd_send(1,0x08,Set_Current);//用户设定的目标电流值
		DAC_Cmd_send(1,0x07,Set_I_Slope);//设定用户电流变化率
		DAC_Cmd_send(1,0x00,0x0000);//使能输出
		DAC_Cmd_send(1,0x0a,0x0002);//使能电流缓降
	}

}



