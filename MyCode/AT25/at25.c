#include "at25.h"


#define AT25CS_LOW 		HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_RESET)
#define AT25CS_HIGH 	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_3,GPIO_PIN_SET)
/**
  * @brief  使用SPI完成一字节的数据的收发处理
  * @param  byte：要发送的数据
  * @retval 0xff:error/data received
  */

uint8_t SPI_AT25_SendByte(uint8_t byte)
{

	uint8_t * sned_buffer=&byte;
	uint8_t *receive_buffer=NULL;
	if(HAL_SPI_TransmitReceive(&hspi1,sned_buffer,receive_buffer,1,1000)==HAL_OK)
	{
		return *receive_buffer;
	}
	else return 0xff;


}
 /**
  * @brief  使用SPI读取一个字节的数据
  * @param  无
  * @retval 返回接收到的数据
  */
uint8_t SPI_AT25_ReadByte(void)
{

	uint8_t *buffer=NULL;
	if(HAL_SPI_Receive(&hspi1,buffer, 1, 1000)==HAL_OK) return *buffer;
	else return 0xff;

}

void AT_25_init(void)
{
	uint8_t buffer[1]={0};
	uint8_t RDY=0x01;

	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);//lcd cs 1
	AT25CS_LOW;

	buffer[0]=0x06;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd write enable
	buffer[0]=0x01;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd write status register
	buffer[0]=0xf2;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//data write to status register(1111 0010)

	AT25CS_HIGH;
	HAL_Delay(1);
	//wait for AT25 ready
	do
	{
		AT25CS_LOW;
		buffer[0]=AT25_RDSR;
		HAL_SPI_Transmit(&hspi1,buffer,1,1000);//data write to status register(1111 0010)
		HAL_SPI_Receive(&hspi1,buffer,1,1000);
		AT25CS_HIGH;
		RDY=buffer[0];
	}
	while((RDY&0x01) == 1);


}
/**
 * @brief  存储一个16bits数据到at25
 * @param  data，要存入的数据
 * @param  addr，要存入的地址
 * @retval 无
 */
void SPI_Save_Word_16bits(uint16_t data,uint8_t addr)
{
	uint8_t buffer[1]={0};
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);//lcd off
	/*AT25 config*/
	AT25CS_LOW;

	buffer[0]=0x06;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd write enable
	buffer[0]=0x01;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd write status register
	buffer[0]=0xf2;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//data write to status register(1111 0010)

	AT25CS_HIGH;

	/*16bits data send*/
	AT25CS_LOW;
	buffer[0]=AT25_Write;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd:write status register

	buffer[0]=addr;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//adress

	buffer[0]=(data>>8);
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//data high

	buffer[0]=data;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//data low
	AT25CS_HIGH;

	HAL_Delay(1);
}
/**
 * @brief  存储一个16bits数据到at25
 * @param  data，要存入的数据
 * @param  addr，要存入的地址
 * @retval 无
 */
void SPI_Save_Word_8bits(uint8_t data,uint8_t addr)
{

	uint8_t buffer[1]={0};
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);//lcd off
	/*AT25 config*/
	AT25CS_LOW;

	buffer[0]=0x06;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd write enable
	buffer[0]=0x01;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd write status register
	buffer[0]=0xf2;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//data write to status register(1111 0010)

	AT25CS_HIGH;

	/*8bits data send*/
	AT25CS_LOW;
	buffer[0]=AT25_Write;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd:write status register

	buffer[0]=addr;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//adress

	buffer[0]=data;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//data high
	AT25CS_HIGH;

	HAL_Delay(1);
}

/**
 * @brief  从at25取出一个长度16bits的数据
 * @param  addr，要取数据的地址
 * @retval 无
 */
uint16_t SPI_Load_Word_16bits(uint8_t addr)
{

	uint16_t tmp=0x00;

	uint8_t buffer[1];
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);//lcd cs 1 off

	/*load word 8bits*/
	AT25CS_LOW;
	buffer[0]=AT25_Read;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd
	buffer[0]=addr;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//address
	buffer[0]=0x00;//clear the buffer
	HAL_SPI_Receive(&hspi1,buffer,1,1000);//get data
	tmp|=buffer[0];
	tmp<<=8;
	buffer[0]=0x00;//clear the buffer
	HAL_SPI_Receive(&hspi1,buffer,1,1000);//get data
	tmp|=buffer[0];
	AT25CS_HIGH;

	return tmp;
}

uint8_t SPI_Load_Word_8bits(uint8_t addr)
{
	uint8_t buffer[1];
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0,GPIO_PIN_SET);//lcd cs 1 off

	/*load word 8bits*/
	AT25CS_LOW;
	buffer[0]=AT25_Read;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//cmd
	buffer[0]=addr;
	HAL_SPI_Transmit(&hspi1,buffer,1,1000);//address
	buffer[0]=0x00;//clear the buffer
	HAL_SPI_Receive(&hspi1,buffer,1,1000);//get data
	AT25CS_HIGH;

	return buffer[0];
}

/**
 * @brief  对AT25写入数据
 * @param	pBuffer，要写入数据的指针
 * @param  WriteAddr，写入地址
 * @param  NumByteToWrite，写入数据长度，必须小于等于SPI_FLASH_PerWritePageSize
 * @retval 无
 */
void SPI_AT25_Write(uint16_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite)
{
	NumByteToWrite = NumByteToWrite/2;

	 while (NumByteToWrite > 0)
	 {
			SPI_Save_Word_16bits(*pBuffer++, WriteAddr);
			NumByteToWrite --;
			WriteAddr+=2;
	 }
}

/**
 * @brief   读取AT25数据
 * @param   pBuffer，存储读出数据的指针
 * @param   ReadAddr，读取地址
 * @param   NumByteToRead，读取数据长度
 * @retval  无
 */
void SPI_AT25_Read(uint16_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead)
{
	NumByteToRead = NumByteToRead/2;

	/* 读取数据 */
 while (NumByteToRead > 0) /* while there is data to be read */
 {
		*pBuffer++ = SPI_Load_Word_16bits(ReadAddr);
		NumByteToRead--;
		ReadAddr = ReadAddr + 2;
 }


}
/*----------------------------------------------------------------------------------------------------------------------------*/
/*---------------------------------------------------向AT25写入/存储数据部分--------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/

/**
 * @brief  对AT25写入电流电压设定值
 * @param	无
 * @retval 无
 */
void AT25_Save_VISet(void)
{
	uint16_t temp[3];
	float Power=(Set_Voltage/100.0)*(Set_Current/100.0);
	//将设定值强制类型转换成uint16_t
	temp[0] = (uint16_t) Set_Voltage;
	temp[1] = (uint16_t) Set_Current;
	temp[2] = (uint16_t) Power;

	SPI_AT25_Write(temp, 0, sizeof(temp));
}



/**
 * @brief  对AT25写入设定的电流电压值
 * @param	Recall_Save的编号
 * @retval 无
 */
void AT25_Save_Recall_Save_VI(uint8_t Number)
{
	uint16_t temp;
	//将设定值强制类型转换成uint16_t(保留两位小数)
	temp = (uint16_t) (Recall_Save_Voltage[Number] * 100.0);//保留两位小数
	SPI_AT25_Write(&temp, 6+2*Number,sizeof(temp));//6 7、8 9...24、25

	temp = (uint16_t) (Recall_Save_Current[Number] * 100.0);
	SPI_AT25_Write(&temp, 26+2*Number,sizeof(temp));//26 27、28 29...44、45

	temp=Recall_Save_Power[Number];//W
	SPI_AT25_Write(&temp, 46+2*Number,sizeof(temp));//46、47...64、65

}



uint16_t AT25_Save_Recall_Recall_VI(uint8_t Number)
{

	uint16_t temp;
	if(xyz.coordinates3==1)//V
	{
		SPI_AT25_Read(&temp,6+2*Number,2);

		return temp;
	}
	else if(xyz.coordinates3==2)//I
	{
		SPI_AT25_Read(&temp,26+2*Number,2);

		return temp;
	}

	SPI_AT25_Read(&temp,46+2*Number,2);//P
	return temp;


}


/**
 * @brief  对AT25写入RS232设定值
 * @param	无
 * @retval 无
 */
void AT25_Save_RS232(void)
{
	uint16_t temp_ID[1];
	uint32_t temp_BaudRate[1];

	//保存id号
	temp_ID[0] = RS232_ID;
	SPI_AT25_Write(temp_ID, 66, sizeof(temp_ID));

	//保存波特率
	temp_BaudRate[0] = RS232_BaudRate;
	SPI_AT25_Write((uint16_t*)temp_BaudRate, 68, sizeof(temp_BaudRate));
}

/**
 * @brief  对AT25写入延时时间设定值
 * @param	无
 * @retval 无
 */
void AT25_Save_Delay(void)
{
	uint8_t temp[4];
	uint8_t temp_1[2];

	temp[0] = Delay_h;
	temp[1] = Delay_m;
	temp[2] = Delay_s;
	temp[3] = 0x00;

	temp_1[0] = Delay_Function_On_Off;
	temp_1[1] = 0x00;

	SPI_AT25_Write((uint16_t*)temp, 78, sizeof(temp));
	SPI_AT25_Write((uint16_t*)temp_1, 108, sizeof(temp_1));
}

/**
 * @brief  对AT25写入循环时间设定值
 * @param	无
 * @retval 无
 */
void AT25_Save_Cycle(void)
{
	uint16_t temp[4];

	uint8_t temp_1[2];

	temp[0] = Cycle_On_s;
	temp[1] = Cycle_On_ms;
	temp[2] = Cycle_Close_s;
	temp[3] = Cycle_Close_ms;

	temp_1[0] = Cycle_Function_On_Off;
	temp_1[1] = 0x00;

	SPI_AT25_Write(temp, 82, sizeof(temp));
	SPI_AT25_Write((uint16_t*)temp_1, 110, sizeof(temp_1));
}

/**
 * @brief  对AT25写入CC_CV_CP设定值
 * @param	无
 * @retval 无
 */
void AT25_Save_CC_CV_CP(void)
{
	uint8_t temp[2];

	temp[0] = CV_CC_CP;
	temp[1] = 0x00;

	SPI_AT25_Write((uint16_t*)temp, 80, sizeof(temp));
}
/**
 * @brief  对AT25写入电压电流变化速率
 * @param	无
 * @retval 无
 */
void AT25_Save_VI_Rate(void)
{
	uint16_t temp[2];

	temp[0] = Set_V_Slope;
	temp[1] = Set_I_Slope;

	SPI_AT25_Write(temp, 82, sizeof(temp));

}
/**
 * @brief  对AT25写入电压电流变化速率
 * @param	无
 * @retval 无
 */
void AT25_Save_AD_Param(void)
{
	uint16_t temp[4];

	temp[0] = ADC_Gain_V*1000000;
	temp[1] = ADC_Gain_I*1000000;
	/*test*/
	temp[2] = (-1*Eror_ADC_V)*10000;
	temp[3] = (-1*Eror_ADC_I)*10000;

	SPI_AT25_Write(temp, 86, sizeof(temp));

}

void AT25_load_AD_Param(void)
{

	uint16_t temp[4];

	SPI_AT25_Read((uint16_t*)temp, 86, sizeof(temp));

	ADC_Gain_V=temp[0]/1000000.0;
	ADC_Gain_I=temp[1]/1000000.0;
	Eror_ADC_V=-(temp[2]/10000.0);
	Eror_ADC_I=-(temp[3]/10000.0);
}


/**
 * @brief  对AT25重置所有设定值
 * @param	无
 * @retval 无
 */
void AT25_Reset(void)
{
	AT_25_init();

	CV_CC_CP = CV;
	AT25_Save_CC_CV_CP();
	Set_V_Slope=15;
	Set_I_Slope=5;
	AT25_Save_VI_Rate();
	Set_Current=0;
	Set_Voltage=0;
	AT25_Save_VISet();
	/*save recall reset*/
	uint16_t temp[30] = {0};
	for(uint8_t i=0;i<10;i++)//1000-10000
	{
		temp[i]=1000*(i+1);
	}
	for(uint8_t j=10;j<20;j++)//100-1000
	{
		temp[j]=(j-9)*100;
	}
	for(uint8_t k=20;k<30;k++)
	{
		temp[k]=temp[k-20]*temp[k-10];
	}
	SPI_AT25_Write(temp,0x06, sizeof(temp));



}

/*----------------------------------------------------------------------------------------------------------------------------*/
/*-----------------------------------------------------从AT25读出数据部分---------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------*/


 /**
  * @brief  对AT25读出电流电压功率设定值
  * @param	无
  * @retval 无
  */
void AT25_Load_VISet(void)
{
	uint16_t temp[2];
	SPI_AT25_Read(temp, 0, sizeof(temp));
	//将设定值强制类型转换成uint16_t
	Set_Voltage = temp[0];
	Set_Current = temp[1];

}

/**
 * @brief  对AT25读出上次存储的实际电压电流值
 * @param	无
 * @retval 无
 */
float AT25_Load_V(void)
{
	uint16_t temp[2];
	SPI_AT25_Read(temp, 122, sizeof(temp));

	return ((float)temp[0]+temp[1]/100.0);

}

float AT25_Load_I(void)
{
	uint16_t temp[2];
	SPI_AT25_Read(temp, 124, sizeof(temp));

	return ((float)temp[0]+temp[1]/100.0);

}

 /**
  * @brief  对AT25读出Recall_Save的电流电压功率设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_RecallSaveSet(void)
{
	uint16_t temp[30];

	int i = 0;

	uint8_t Number = 0;

	SPI_AT25_Read(temp, 6, sizeof(temp));

	for(; i < 30; i++)
	{
	  //将设定值强制类型转换成uint16_t
    Recall_Save_Voltage[Number] = ((float)temp[i]) / 100;
		i ++;
    Recall_Save_Current[Number] = ((float)temp[i]) / 100;
		i ++;

	}
}

 /**
  * @brief  对AT25写入RS232设定值
  * @param	Recall_Save的编号
  * @retval 无
  */
static void AT25_Load_RS232(void)
{
	uint16_t temp_ID[1]={0};
	uint32_t temp_BaudRate[1]={0};

	//读取id号
	SPI_AT25_Read(temp_ID, 66, sizeof(temp_ID));
	RS232_ID = temp_ID[0];

	//读取波特率
	SPI_AT25_Read((uint16_t*)temp_BaudRate, 68, sizeof(temp_BaudRate));
	RS232_BaudRate = temp_BaudRate[0];
}

 /**
  * @brief  对AT25读出延时时间设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_Delay(void)
{
	uint8_t temp[4];
	uint8_t temp_1[2];

	SPI_AT25_Read((uint16_t*)temp, 78, sizeof(temp));
	SPI_AT25_Read((uint16_t*)temp_1, 108, sizeof(temp_1));

	Delay_h = temp[0];
	Delay_m = temp[1];
	Delay_s = temp[2];

	Delay_Function_On_Off = temp_1[0];
}

 /**
  * @brief  对AT25读出循环时间设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_Cycle(void)
{
	uint16_t temp[4];
	uint8_t temp_1[2];

	SPI_AT25_Read(temp, 82, sizeof(temp));
	SPI_AT25_Read((uint16_t*)temp_1, 110, sizeof(temp_1));

	Cycle_On_s = temp[0];
	Cycle_On_ms = temp[1];
	Cycle_Close_s = temp[2];
	Cycle_Close_ms = temp[3];

	Cycle_Function_On_Off = temp_1[0];
}

 /**
  * @brief  对AT25读出CC_CV_CP设定值
  * @param	无
  * @retval 无
  */
static void AT25_Load_CC_CV_CP(void)
{
	uint8_t temp[2];

	SPI_AT25_Read((uint16_t*)temp, 90, sizeof(temp));

	CV_CC_CP = temp[0];
}
/**
 * @brief  对AT25读出电压电流的变化斜率设定值
 * @param  无
 * @retval 无
 */
static void AT25_Load_VI_Rate(void)
{
	uint16_t temp[2];

	SPI_AT25_Read((uint16_t*)temp, 225, sizeof(temp));

	Set_V_Slope = temp[0];
	Set_I_Slope = temp[1];

}



 /**
  * @brief  对AT25读出所有设定值
  * @param	无
  * @retval 无
  */
void AT25_Load_Data(void)
{
/*
	AT25_Load_Fist_Use_Device();
	if(Fist_Use_Device == 0xff)   //如果第一次写入设备，就重置at25
	{
		AT25_Reset();
	}
*/
	AT25_Load_VISet();
	AT25_Load_RecallSaveSet();
	AT25_Load_RS232();
	AT25_Load_Delay();
	AT25_Load_Cycle();
	AT25_Load_CC_CV_CP();

#ifdef RF_RATE_MODE
	AT25_Load_VI_Rate();
#endif

}
