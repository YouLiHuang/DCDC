#include "RS232.h"


extern DMA_HandleTypeDef hdma_usart3_rx;
uint8_t USART3_Num;//USART接收字节数
uint8_t USART3_Send_Data[20];  //发送数据数组
uint8_t USART3_Receive_Data[20]; //接收数据数组
uint8_t USART3_Receive_Data_buffer[2];
uint8_t USART3_Need_To_Send_Num;//需发送数据字节数
uint16_t CrcValue;//校验码
uint16_t AddValue;

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

/**
  * @brief  USART发送报文
  * @param  无
  * @retval 无
  */
void USART3_Send(void)
{

	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_SET);
	HAL_UART_DMAStop(&huart3);

	HAL_UART_Transmit_DMA(&huart3,USART3_Send_Data,USART3_Need_To_Send_Num);

	__HAL_UART_CLEAR_FLAG(&huart3,UART_CLEAR_TCF);
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_TC);

}
/**
  * @brief  CRC 16位转8位
  * @param  X-16位数据   *t-8位数据地址
  * @retval 无
  */
void uint16_t_to_uint8_t(uint16_t x,uint8_t *t)
{
	*(t+1)=x;
	*t=x>>8;
}

static void function20(void)
{

	USART3_Send_Data [0]=(uint8_t)RS232_ID;
	USART3_Send_Data [1]=0x20;
	USART3_Send_Data [2]=0x01;


	if(USART3_Receive_Data[3]==ON)
	{
		OUTPUT_NOT_OUTPUT = OUTPUT;
		if(Delay_Function_On_Off == Delay_Function_On)               //打开了延时功能
		{
			TIM16_DELAY_ON();
		}
		else if(Cycle_Function_On_Off == Cycle_Function_On)          //打开了循环功能
		{
			TIM17_CYCLE_ON();
		}
		else
		{
			power_on();
		}
	}
	else
	{
		OUTPUT_NOT_OUTPUT = NOT_OUTPUT;
		TIM16_DELAY_OFF();
		TIM17_CYCLE_OFF();
		power_off();
	}
	USART3_Send_Data [3]=USART3_Receive_Data[3];

	CrcValue = CRC16( USART3_Send_Data, 4) ;
	USART3_Send_Data [4]=CrcValue&0xff;
	USART3_Send_Data [5]=((CrcValue>>8)&0xff);
	USART3_Need_To_Send_Num =6;
	USART3_Send ();
}

/**
  * @brief  设定输出电压命令：0x21
  * @param  无
  * @retval 无
  */
static void function21(void)
{

	USART3_Send_Data [0]=(uint8_t)RS232_ID;//本机ID
	USART3_Send_Data [1]=0x21;//指令类型
	USART3_Send_Data [2]=0x02;


	Set_Voltage=(USART3_Receive_Data[3]<<8|USART3_Receive_Data[4])/100.0;
	CV_CC_CP = CV;
	AT25_Save_VISet();
	AT25_Save_CC_CV_CP();


	USART3_Send_Data [3]=USART3_Receive_Data[3];
	USART3_Send_Data [4]=USART3_Receive_Data[4];


	CrcValue = CRC16( USART3_Send_Data, 5) ;
	USART3_Send_Data [5]=CrcValue&0xff;
	USART3_Send_Data [6]=(CrcValue>>8)&0xff;
	USART3_Need_To_Send_Num =7;
	USART3_Send ();


}
/**
  * @brief  设定输出电流命令：0x22
  * @param  无
  * @retval 无
  */
static void function22(void)
{

	USART3_Send_Data [0]=(uint8_t)RS232_ID ;
	USART3_Send_Data [1]=0x22;
	USART3_Send_Data [2]=0x02;


	//立浩要改  这里设定电流
    Set_Current=(USART3_Receive_Data[3]<<8|USART3_Receive_Data[4])/100.0;
	CV_CC_CP = CC;
	AT25_Save_VISet();
	AT25_Save_CC_CV_CP();

	USART3_Send_Data [3]=USART3_Receive_Data[3];
	USART3_Send_Data [4]=USART3_Receive_Data[4];

	CrcValue = CRC16( USART3_Send_Data, 5) ;
	USART3_Send_Data [5]=CrcValue&0xff;
	USART3_Send_Data [6]=(CrcValue>>8)&0xff;
	USART3_Need_To_Send_Num =7;
	USART3_Send ();

}




/**
  * @brief 读取电压及电流反馈数据：0x26
  * @param  无
  * @retval 无
  */
static void function26(void)
{

	USART3_Send_Data [0]=(uint8_t)RS232_ID;
	USART3_Send_Data [1]=0x26;
	USART3_Send_Data [2]=0x04;
	USART3_Send_Data [3]=(uint16_t)(Voltage_real*100)>>8;
	USART3_Send_Data [4]=(uint16_t)(Voltage_real*100);
	USART3_Send_Data [5]=(uint16_t)(Current_real*100)>>8;
	USART3_Send_Data [6]=(uint16_t)(Current_real*100);

	CrcValue = CRC16( USART3_Send_Data,7);
	USART3_Send_Data [7]=CrcValue&0xff;
	USART3_Send_Data [8]=(CrcValue>>8)&0xff;
	USART3_Need_To_Send_Num =9;
	USART3_Send ();

}


/**
  * @brief  读取电压、电流、功率最大值数据：0x27
  * @param  无
  * @retval 无
  */
static void function27(void)
{
	USART3_Send_Data [0]=(uint8_t)RS232_ID;
	USART3_Send_Data [1]=0x27;
	USART3_Send_Data [2]=0x06;

	USART3_Send_Data [3]=0x1f;
	USART3_Send_Data [4]=0x40;//8000
	USART3_Send_Data [5]=2;
	USART3_Send_Data [6]=0x27;
	USART3_Send_Data [7]=0x10;//10000
	USART3_Send_Data [8]=2;
	USART3_Send_Data [9]=0x17;
	USART3_Send_Data [10]=0x70;//6000
	USART3_Send_Data [11]=0;


	CrcValue = CRC16( USART3_Send_Data,12);
	USART3_Send_Data [12]=CrcValue&0xff;
	USART3_Send_Data [13]=(CrcValue>>8)&0xff;
	USART3_Need_To_Send_Num =14;
	USART3_Send ();
}


/**
  * @brief  读取电压及电流设定输出数据：0x28
  * @param  无
  * @retval 无
  */
static void function28(void)
{

	USART3_Send_Data [0]=(uint8_t)RS232_ID;
	USART3_Send_Data [1]=0x28;
	USART3_Send_Data [2]=0x05;

	USART3_Send_Data [3]=ON_OFF;
	USART3_Send_Data [4]=(uint16_t)(Set_Voltage*100)>>8;
	USART3_Send_Data [5]=(uint16_t)(Set_Voltage*100);
	USART3_Send_Data [6]=(uint16_t)(Set_Current*100)>>8;
	USART3_Send_Data [7]=(uint16_t)(Set_Current*100);


	CrcValue =CRC16(USART3_Send_Data ,8);
	USART3_Send_Data [8]=CrcValue&0xff;
	USART3_Send_Data [9]=(CrcValue>>8)&0xff;
	USART3_Need_To_Send_Num =10;
	USART3_Send ();
}

/**
  * @brief  设定机器编号：0x29
  * @param  无
  * @retval 无
  */
static void function29(void)
{
	USART3_Send_Data[0]=(uint8_t)RS232_ID ;
	USART3_Send_Data[1]=0x29;
	USART3_Send_Data[2]=0x01;

	RS232_ID = (uint16_t)USART3_Receive_Data[3];
	USART3_Send_Data[3]=USART3_Receive_Data[3];

	CrcValue =CRC16 (USART3_Send_Data,4);
	USART3_Send_Data [4]=CrcValue&0xff;
	USART3_Send_Data [5]=(CrcValue>>8)&0xff;
	USART3_Need_To_Send_Num =6;
	USART3_Send ();
}

/**
  * @brief  读取电压及电流反馈数据：0x2a
  * @param  无
  * @retval 无
  */
static void function2a(void)
{
	USART3_Send_Data[0]=(uint8_t)RS232_ID ;
	USART3_Send_Data[1]=0x2a;
	USART3_Send_Data[2]=0x05;

	USART3_Send_Data [3]=ON_OFF ;
	USART3_Send_Data [4]=(uint16_t)(Voltage_real*100)>>8;
	USART3_Send_Data [5]=(uint16_t)(Voltage_real*100);
	USART3_Send_Data [6]=(uint16_t)(Current_real*100)>>8;
	USART3_Send_Data [7]=(uint16_t)(Current_real*100);

	CrcValue =CRC16 (USART3_Send_Data,8);
	USART3_Send_Data [8]=CrcValue&0xff;
	USART3_Send_Data [9]=(CrcValue>>8)&0xff;
	USART3_Need_To_Send_Num =10;
	USART3_Send ();
}

void USART3_Handler(void)
{

	if((USART3_Receive_Data [0] == (uint8_t)RS232_ID ) || (USART3_Receive_Data [0] ==0))
	{
		CrcValue=((USART3_Receive_Data [USART3_Num-2])|((uint16_t)(USART3_Receive_Data[USART3_Num -1])<<8));
		if(CrcValue==CRC16(USART3_Receive_Data,USART3_Num-2 ))
		{
			switch(USART3_Receive_Data [1])
			{
				case 0x20:function20();break;
				case 0x21:function21();break;
				case 0x22:function22();break;

				case 0x26:function26();break;
				case 0x27:function27();break;
				case 0x28:function28();break;
				case 0x29:function29();break;
				case 0x2a:function2a();break;

				default: break;
			}
		}
		else return;
	}
	else return;


}
/**
  * @brief  RS232接收中断,来自电脑端指令
  * @param  无
  * @retval 无
  */
void RS232_IT(void)
{
	if(__HAL_UART_GET_IT(&huart3,UART_IT_ERR)==SET)
	{
		HAL_UART_Receive_IT(&huart3, USART3_Receive_Data,2);
		__HAL_UART_CLEAR_FLAG(&huart3,UART_CLEAR_OREF);

	}
	/*USART1_RDE PIN Control*/
	if(__HAL_UART_GET_IT(&huart3,UART_IT_TC)==SET)
	{
		__HAL_UART_DISABLE_IT(&huart3, UART_IT_TC);
		__HAL_UART_CLEAR_FLAG(&huart3,UART_CLEAR_TCF);
		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_15,GPIO_PIN_RESET);
	}
	if(__HAL_UART_GET_IT(&huart3,UART_IT_IDLE)==SET)
	{

		HAL_UART_DMAStop(&huart3);
		USART3_Num=50-__HAL_DMA_GET_COUNTER(&hdma_usart3_rx);
		hdma_usart3_rx.Instance->CNDTR=50;
		HAL_UART_Receive_DMA(&huart3,USART3_Receive_Data, USART3_Num);
		USART3_Handler();
		__HAL_UART_CLEAR_FLAG(&huart3,UART_CLEAR_IDLEF);

	}

}
