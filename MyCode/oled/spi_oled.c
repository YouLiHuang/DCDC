#include "spi_oled.h"

Interface_coordinates xyz={0,0,0};//初始化为根界面
Interface_coordinates Last_Interface={0,0,0};
extern uint8_t OCP_SoftOrHard_Flag;		//OCP硬软件报警与硬件报警标志位




float Recall_Save_Voltage[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float Recall_Save_Current[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float Recall_Save_Power[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

/*****************************************************延时和循环的参数***************************************************/
char String_Cycle_On[10]={'0','0','0','0','s','0','0','0','m','s'};               //定义循环开的字符串
char String_Cycle_Close[10]={'0','0','0','0','s','0','0','0','m','s'};            //定义循环关的字符串
char String_Delay[10]={'0','0','0','0','s','0','0','0','m','s'};                  //定义延时的字符串
char String_Save_V[10]={'0','0','0','.','0','0','V'};
char String_Save_I[10]={'0','0','0','.','0','0','A'};
char String_Recall_V[10]={'0','0','0','.','0','0','V'};
char String_Recall_I[10]={'0','0','0','.','0','0','V'};
char String_RS232_Rate[10]={'1','1','5','2','0','0'};
/****************************************************新版缓升/降的参数****************************************************/
char String_I_Rate[10]={'0','.','0','0',' ','A','/','m','s'};                     //定义String_I_Rate的字符串
char String_V_Rate[10]={'0','.','0','0',' ','V','/','m','s'};                     //定义String_I_Rate的字符串
char String_Voltage[7]={'0','0','0','.','0','0','V'};                       	  //定义一个存设定电压值的字符串
char String_Current[7]={'0','0','0','.','0','0','A'};                     	  	  //定义一个存放设定电流值的字符串
char String_AD_Frq[10]={'0','0','0','H','z'};
char String_AD_Gain[10]={'0','0','0','.','0','0'};
char String_RS232_ID[5];                          							      //RS232 ID的字符串
char String_RS232_BaudRate[10];
char String_Light_Level[10]={'0','0'};
char String_Temperature_CH1[10]={'0','.','0','0','0','V'};
char String_Temperature_CH2[10]={'0','.','0','0','0','V'};
char String_FAN[10]={'0','0','0','0','0'};
char String_Calibration_Vlotage[10]={'0','0','0','.','0','0','V'};						//用户设定的电压校准值
char String_Calibration_Current[10]={'0','0','0','.','0','0','A'};						//用户设定的电流校准值
char String_Password[7]={' ',' ',' ',' ',' ',' '};                       	  		//定义一个存输入密码的字符串
char String_Password_mask[7]={' ',' ',' ',' ',' ',' '};
/****************************************************新版AD参数****************************************************/
uint8_t First_Selection = 0;   													 //定义第一菜单的选择
uint8_t Second_Selection = 0;                                                    //定义第二菜单的选择
/****************************************************亮度调节参数*****************************************************/
uint8_t Light=0x01;




/*******************************************************************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************************************************************/
/**************************************************************************************************OLED Driver API**************************************************************************************************/
/*******************************************************************************************************************************************************************************************************************/
/*******************************************************************************************************************************************************************************************************************/

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------通信接口/指令----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
//定义收发成功/失败标志


/**
	* @brief  向OLED发送数据
	* @param  要发送的数据
	* @retval None
	*/
void SPI_OLED_SendData(uint8_t byte)
{
	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE); //关闭编码器中断

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);//dc 1

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);//cs 0

	HAL_SPI_Transmit(&hspi1,&byte,1, 10);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);//cs 1

	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE); //开启编码器中断

}

void SPI_OLED_SendData_Long(uint8_t * data)
{
	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE); //关闭编码器中断

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);//dc 1

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);//cs 0

	HAL_SPI_Transmit(&hspi1,data,1, 10);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);//cs 1

	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE); //开启编码器中断
}


/**
	* @brief  向OLED发送命令
	* @param  要发送的命令
	* @retval None
	*/
void SPI_OLED_SendCommond(uint8_t byte)
{
	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE); //关闭编码器中断

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_RESET);//dc 0

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);//cs 0


	HAL_SPI_Transmit(&hspi1,&byte,1, 10);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);//cs 1

	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE); //开启编码器中断
}

void OLED_Init(void)
{

	res_OLED;							//将SPI3_RES引脚置0
	HAL_Delay(1);
	set_OLED; 						//将SPI3_RES引脚置1
	HAL_Delay(5);

	SPI_OLED_SendCommond(Set_Command_Lock);//开锁
	SPI_OLED_SendData(0x12);

	SPI_OLED_SendCommond(Set_Column_Address_Commond);//设置列地址
	SPI_OLED_SendData(0x1c);
	SPI_OLED_SendData(0x5b);

	SPI_OLED_SendCommond(Set_Row_Address_Commond);//设置行地址
	SPI_OLED_SendData(0x00);
	SPI_OLED_SendData(0x3f);

	SPI_OLED_SendCommond(Sleep_mode_ON);//休眠

	SPI_OLED_SendCommond(Set_Front_Clock_Divider);//设置前时钟分频器/振荡器频率
	SPI_OLED_SendData(0x91);

	SPI_OLED_SendCommond(Set_MUX_Ratio);//设置多路选择开关比值
	SPI_OLED_SendData(0x3f);

	SPI_OLED_SendCommond(Set_Display_Offset);//从0-127设置垂直滚动
	SPI_OLED_SendData(0x00);

	SPI_OLED_SendCommond(Set_Display_Start_Line);//设置启动行
	SPI_OLED_SendData(0x00);

	SPI_OLED_SendCommond(Normal_Display);//设置正常显示

	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);

	SPI_OLED_SendCommond(Set_GPIO);//引脚设定
	SPI_OLED_SendData(0x00);

	SPI_OLED_SendCommond(Function_Selection);//内部电压调节
	SPI_OLED_SendData(0x01);

	SPI_OLED_SendCommond(0xb4);
	SPI_OLED_SendData(0xa0);
	SPI_OLED_SendData(0xfd);

	SPI_OLED_SendCommond(Set_Contrast_Current);//设置对比度电流
	SPI_OLED_SendData(0xaf);

	SPI_OLED_SendCommond(Master_Contrast_Current_Control);//主对比度电流控制
	SPI_OLED_SendData(0x0f);

	SPI_OLED_SendCommond(Set_Gray_Scale_Table);//设置灰度表
	SPI_OLED_SendData(0x0c);
	SPI_OLED_SendData(0x18);
	SPI_OLED_SendData(0x24);
	SPI_OLED_SendData(0x30);
	SPI_OLED_SendData(0x3c);
	SPI_OLED_SendData(0x48);
	SPI_OLED_SendData(0x54);
	SPI_OLED_SendData(0x60);
	SPI_OLED_SendData(0x6c);
	SPI_OLED_SendData(0x78);
	SPI_OLED_SendData(0x84);
	SPI_OLED_SendData(0x90);
	SPI_OLED_SendData(0x9c);
	SPI_OLED_SendData(0xa8);
	SPI_OLED_SendData(0xb4);

	SPI_OLED_SendCommond(Enable_Gray_Scale_table);//开启灰度表

	SPI_OLED_SendCommond(Set_Phase_Length);//设置相位长度
	SPI_OLED_SendData(0xE2);

	SPI_OLED_SendCommond(0xD1); 								//Display Enhancement B
	SPI_OLED_SendData(0xa2);
	SPI_OLED_SendData(0x20);										//反显

	SPI_OLED_SendCommond(Set_Pre_charge_voltage);//设置预充电电压
	SPI_OLED_SendData(0x1F);

	SPI_OLED_SendCommond(Set_Second_Precharge_Period);//设置第二预充期间
	SPI_OLED_SendData(0x08);

	SPI_OLED_SendCommond(Set_VCOMH );//设置COM取消选择电压等级
	SPI_OLED_SendData(0x07);

	SPI_OLED_SendCommond(Normal_Display);//正常显示

	SPI_OLED_SendCommond(Exit_Partial_Display);//该命令被发送到退出部分显示模式

	SPI_OLED_SendCommond(Write_RAM_Command);

	Fill_RAM(0x00);   //清屏

	SPI_OLED_SendCommond(Sleep_mode_OFF);//关休眠

}
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------内部驱动函数-----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief  转换函数，将一位转为四位（SD1322四位表示一个像素点）,并写入SD1322
 * @param  要转换的数据（字模表的元素）
 * @retval None
 */
void Font_1Bit_To_4Bit(uint8_t Data)
{
	 /*对于字模而言，用16个u8构成的数组便可以描述一个字，u8的每一bit代表一个像素点，但我们的OLED是4个bit代表一个像素点
	   因此，u8要膨胀成一个u32的数据，u8的一个bit对应u32的四个bit，u8的bit=1对应u32的4个bit=1111即可*/

	 uint8_t mask=0x01;
	 uint8_t temp[8]={0x00};
	 uint8_t data_send[4]={0x00};
	 /*新的实现*/
	 for(uint8_t i=0;i<8;i++)
	 {
		 if((Data&mask)==mask) temp[i]|=Light;//low 4 bits set
		 else 				   temp[i]&=(Light<<4);//low 4 bits reset
		 mask<<=1;

	 }
	 mask=0x0f;
	 uint8_t index=3;
	 for(uint8_t j=0;j<8;)
	 {
		 data_send[index]|=(temp[j]&mask);//0 2 4 6 low
		 data_send[index]|=((temp[j+1]&mask)<<4);//1 3 5 7 high
		 j++;
		 j++;
		 index--;

	 }
	 //写RAM

		__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE); //关闭编码器中断

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);//dc 1

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);//cs 0

		HAL_SPI_Transmit(&hspi1,data_send,4,100);
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);//cs 1

		__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE); //开启编码器中断


}

static void _1Bit_To_4Bit_16x32(uint8_t* Pdata)
{

	 uint8_t data_send_buffer[4*64]={0x00};
	 uint8_t mask=0x01;
	 uint8_t temp[8]={0x00};
	 for(uint8_t index=0;index<64;index++)
	 {
		 memset(&temp[0],0,8);
		 mask=0x01;
		 for(uint8_t i=0;i<8;i++)
		 {
			 if( ((*(Pdata+index))&mask) == mask) temp[i]|=Light;//low 4 bits set
			 else 				   				  temp[i]&=(Light<<4);//low 4 bits reset
			 mask<<=1;

		 }

		 mask=0x0f;
		 uint8_t k;
		 k=3;
		 for(uint8_t j=0;j<8;)
		 {
			 data_send_buffer[k+index*4]|=(temp[j]&mask);//0 2 4 6 low
			 data_send_buffer[k+index*4]|=((temp[j+1]&mask)<<4);//1 3 5 7 high
			 j++;
			 j++;
			 k--;

		 }


	 }

	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE); //关闭编码器中断

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);//dc 1

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);//cs 0

	HAL_SPI_Transmit(&hspi1,&data_send_buffer[0],4*64,100);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);//cs 1

	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE); //开启编码器中断


}

static void _1Bit_To_4Bit_8x16(uint8_t* Pdata)
{
	 uint8_t data_send_buffer[4*16]={0x00};
	 uint8_t mask=0x01;
	 uint8_t temp[8]={0x00};
	 for(uint8_t index=0;index<16;index++)
	 {
		 memset(&temp[0],0,8);
		 mask=0x01;
		 for(uint8_t i=0;i<8;i++)
		 {
			 if( ((*(Pdata+index))&mask) == mask) temp[i]|=Light;//low 4 bits set
			 else 				   				  temp[i]&=(Light<<4);//low 4 bits reset
			 mask<<=1;

		 }

		 mask=0x0f;
		 uint8_t k;
		 k=3;
		 for(uint8_t j=0;j<8;)
		 {
			 data_send_buffer[k+index*4]|=(temp[j]&mask);//0 2 4 6 low
			 data_send_buffer[k+index*4]|=((temp[j+1]&mask)<<4);//1 3 5 7 high
			 j++;
			 j++;
			 k--;

		 }


	 }

	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE); //关闭编码器中断

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);//dc 1

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);//cs 0

	HAL_SPI_Transmit(&hspi1,&data_send_buffer[0],4*16,100);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);//cs 1

	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE); //开启编码器中断

}


/**
 * @brief  12x24专用的转换函数，将一位转为四位（SD1322四位表示一个像素点）,并写入SD1322
 * @param  要转换的数据
 * @retval None
 */
void Font_1Bit_To_4Bit_12x24(uint8_t Data0, uint8_t Data1)
{

	 /*对于字模而言，用16个u8构成的数组便可以描述一个字，u8的每一bit代表一个像素点，但我们的OLED是4个bit代表一个像素点
	   因此，u8要膨胀成一个u32的数据，u8的一个bit对应u32的四个bit，u8的bit=1对应u32的4个bit=1111即可*/

	 /*8位data0*/
	 uint8_t mask=0x01;
	 uint8_t temp[8]={0x00};
	 uint8_t data_send[4]={0x00};
	 /*新的实现*/
	 for(uint8_t i=0;i<8;i++)
	 {
		 if((Data0&mask)==mask) temp[i]|=Light;//low 4 bits set
		 else 				   temp[i]&=(Light<<4);//low 4 bits reset
		 mask<<=1;

	 }
	 mask=0x0f;
	 uint8_t index=0;
	 for(uint8_t j=0;j<8;)
	 {
		 data_send[index]|=(temp[j]&mask);
		 data_send[index]|=((temp[j+1]&mask)<<4);
		 j++;
		 j++;
		 index++;

	 }
	 /*高四位data1*/
	 mask=0x10;
	 uint8_t temp1[4]={0x00};
	 uint8_t data_send1[2]={0x00};
	 for(uint8_t i=0;i<4;i++)
	 {
		 if((Data1&mask)==mask) temp1[i]|=Light;//low 4 bits set
		 else 				   temp1[i]&=(Light<<4);//low 4 bits reset
		 mask<<=1;

	 }
	 mask=0x0f;
	 index=0;
	 for(uint8_t j=0;j<4;)
	 {
		 data_send1[index]|=(temp1[j]&mask);
		 data_send1[index]|=((temp1[j+1]&mask)<<4);
		 j++;
		 j++;
		 index++;

	 }

	 //写RAM
	 for(int k=3;k>=0;k--)
	 {
		 SPI_OLED_SendData(data_send[k]);//一次写两个像素点，写四次
	 }
	 for(int k=1;k>=0;k--)
	 {
		 SPI_OLED_SendData(data_send1[k]);//一次写两个像素点，写两次
	 }


}
/**
	* @brief  设置列地址
	* @param  byte：起始地址
	* @param  byte：结束地址
	* @retval None
	*/
void Set_Column_Address(uint8_t a, uint8_t b)
{
	SPI_OLED_SendCommond(Set_Column_Address_Commond);			//Set Column Address
	SPI_OLED_SendData(0x1c+a);					                  //0x1c地址偏移量()
	SPI_OLED_SendData(0x1c+b);
}

/**
	* @brief  设置行地址
	* @param  byte：起始地址
	* @param  byte：结束地址
	* @retval None
	*/
void Set_Row_Address(uint8_t a, uint8_t b)
{
	SPI_OLED_SendCommond(Set_Row_Address_Commond);			// Set Row Address
	SPI_OLED_SendData(a);					//   Default => 0x00
	SPI_OLED_SendData(b);					//   Default => 0x7F
}

/**
	* @brief  写命令
	* @param  None
	* @retval None
	*/
void Set_WriteData(void)
{
	SPI_OLED_SendCommond(Write_RAM_Command);			//Enable MCU to write Data into RAM
}

/**
	* @brief  写顺序
	* @param  A[0]=0水平扫描，A[0]=1竖直扫描；A[1]设置像素四位总体的顺序；A[2]设置一个像素四位的顺序；A[4]：扫描的组；A[5]设置COM的奇偶分裂；一般设置为0x14
	* @retval None
	*/
void Set_Remap_Format(unsigned char A)
{
	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);			// 	 Set Re-Map / Dual COM Line Mode
	SPI_OLED_SendData(A);					                            //   Default => 0x14
	SPI_OLED_SendData(0x11);	   	                               		//   Default => 0x01 (Disable Dual COM Mode)
}

/**
	* @brief  整屏显示
	* @param  要整屏显示的颜色
	* @retval None
	*/
void Fill_RAM(uint8_t Data)
{
	unsigned char i,j;

	Set_Column_Address(0x00,0x3f);//0到63列
	Set_Row_Address(0x00,0x3f);   //0到63行
	SPI_OLED_SendCommond(Write_RAM_Command);//Enable MCU to write Data into RAM;//Enable MCU to write Data into RAM

	for(i=0;i<64;i++)             //一共64行
	{
		for(j=0;j<64;j++)           //一共64列
		{
			//每次发八位数据，发送两次构成16位数据，完成四个像素点的控制（列地址对应四个像素点）
			//指定行和列地址后相当于选中了四个像素点，每次发送八位数据设置两个像素点，因此要发两次
			SPI_OLED_SendData(Data);
			SPI_OLED_SendData(Data);

		}
	}
}


void Clear_Screen(void)
{
	uint8_t clear_arry[64*128]={0x00};

	Set_Column_Address(0x00,0x3f);//0到63列
	Set_Row_Address(0x00,0x3f);   //0到63行
	SPI_OLED_SendCommond(Write_RAM_Command);//Enable MCU to write Data into RAM;//Enable MCU to write Data into RAM

	__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE); //关闭编码器中断

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);//dc 1

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);//cs 0

	HAL_SPI_Transmit(&hspi1,clear_arry,64*128,1000);


	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);//cs 1

	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE); //开启编码器中断


}


/**
 * @brief  行间隔显示
 * @param  要行间隔显示的颜色
 * @retval None
 */
void Row_Interval(uint8_t Data)
{
	uint8_t i,j;

	Set_Column_Address(0x00,0x3f);
	Set_Row_Address(0x00,0x3f);
	Set_WriteData();

	for(i=0;i<32;i++)
	{
		//第一列（4像素，颜色）
		for(j=0;j<64;j++)
		{
			SPI_OLED_SendData(Data);
			SPI_OLED_SendData(Data);
		}
		//第二列（4像素，反色）
		for(j=0;j<64;j++)
		{
			SPI_OLED_SendData(~Data);
			SPI_OLED_SendData(~Data);
		}
	}

}

/**
 * @brief  点间隔显示
 * @param  要点隔显示的方式
 * @retval None
 */
void Point_Interval(uint8_t m)
{
	uint8_t i,j;
	Set_Column_Address(0x00,0x3f);
	Set_Row_Address(0x00,0x3f);
	Set_WriteData();
	//64行
	for(i=0;i<32;i++)
	{
		//第一行（64列）
		for(j=0;j<64;j++)
		{
			//*_*_
			if(m==0xf0)
			{
				SPI_OLED_SendData(0xf0);
				SPI_OLED_SendData(0xf0);
			}
			//_*_*
			else
			{
				SPI_OLED_SendData(0x0f);
				SPI_OLED_SendData(0x0f);
			}
		}
		//第二行（64列）
		for(j=0;j<64;j++)
		{
			//_*_*（1列）
			if(m==0xf0)
			{
				SPI_OLED_SendData(0x0f);
				SPI_OLED_SendData(0x0f);
			}
			//*_*_（1列）
			else
			{
				SPI_OLED_SendData(0xf0);
				SPI_OLED_SendData(0xf0);
			}
		}

	}

}

/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------字符显示部分--------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief  将字符串的一部分给另一个字符串,用法同Strncpy，并能消除Strncpy的缺陷
 * @param  目标字符串
 * @param  原字符串数组的起始成员
 * @param  个数
 * @retval None
 */
static char *myStrncpy(char *dest, const char *src, size_t n)
{
	int size = sizeof(char)*(n+1);
	char *tmp = (char*)malloc(size);  // 开辟大小为n+1的临时内存tmp
	if(tmp)
	{
		memset(tmp, '\0', size);  // 将内存初始化为0
		memcpy(tmp, src, size-1);  // 将src的前n个字节拷贝到tmp
		memcpy(dest, tmp, size);  // 将临时空间tmp的内容拷贝到dest
		free(tmp);  // 释放内存
		return dest;
	}
	else return NULL;

}

/**
  * @brief  写一个8x16数字、字母(ASIC II)
  * @param  行，列，要写入的数字、字母
  * @retval None
  */
void Write_Single_8x16AsicII(uint8_t Row, uint8_t Column, char Data)
{

	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);

	Set_Column_Address(Column, Column+1);    //设置列地址，两列（8个像素点）
	Set_Row_Address(Row, Row+15);            //设置行地址，16行（16个像素点）
	SPI_OLED_SendCommond(Write_RAM_Command);			//Enable MCU to write Data into RAM

	uint16_t k=(Data-' ')*16;//求对空格的偏移量，确定是哪个字模（*16是因为一行有16个u8数据，构成一个字符）
							 //确定字模及其偏移量后将那行的16个u8数据取出

	_1Bit_To_4Bit_8x16(&Fonts_AsicII_8x16[k]);


}

 /**
  * @brief  写一行8x16数字、字母(ASIC II)
  * @param  行，列，要写入的数字、字母
  * @retval None
  */
void Write_String_8x16AsicII(uint8_t Row, uint8_t Column, char Data[])
{
	uint8_t i;
	uint8_t k;
	k=strlen(Data);

	for(i=0;i<k;i++)
	{
	 Write_Single_8x16AsicII(Row,Column+2*i,Data[i]);
	}
}

/**
 * @brief  写一个16x16中文
 * @param  行，列，要写入的中文(后面要加空格)
 * @retval None
 */
void Write_Single_16x16Chinese(uint8_t Row, uint8_t Column, char Data[])
{
	uint8_t i;
	uint16_t k;

	k=Chinese_Pinyin_to_16x16Fonts(Data);
	k=k*32;

	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);

	Set_Column_Address(Column, Column+3);    //设置列地址,四列，16个像素点
	Set_Row_Address(Row, Row+15);            //设置行地址

	Set_WriteData();

	for(i=0;i<32;i++)
	{
	 Font_1Bit_To_4Bit( Fonts_16x16Chinese[k+i]);
	}
}
/**
 * @brief  写一行16x16中文
 * @param  行，列，要写入的中文(后面要加空格)
 * @retval None
 */
void Write_String_16x16Chinese(uint8_t Row, uint8_t Column, char String_Data[])
{
	uint16_t i=0;
	uint16_t j;
	uint16_t k=0;
	uint16_t m=0;

	char* Single_Data=NULL;       //定义数组指针

	j=strlen(String_Data);        //求整个字符串的长度

	for(;i<j;)                    //i<j表示还没完全输出完成
	{
		for(k=0;String_Data[i]!=' ';i++,k++)   //k表示一个字的字符串长度
		{
		}
		i=i+1;
		m=m+1;                     //m表示字的个数

		Single_Data=(char*)malloc(k+1);    //开辟一个k字节的内存空间
		if(Single_Data)                    //判断是否开辟成功

		myStrncpy(Single_Data,&String_Data[i-k-1],k+1);    //将这个字从原字符串提取出来


		Write_Single_16x16Chinese(Row, Column+4*(m-1), Single_Data);    //写进SD1366

		free(Single_Data);   //释放内存
		Single_Data=NULL;    //指针定义
	}
}

/**
 * @brief  写一个16x32数字、字母(ASIC II)
 * @param  行，列，要写入的数字、字母
 * @retval None
 */
void Write_Single_16x32AsicII(uint8_t Row, uint8_t Column, char Data)
{

	uint16_t k;

	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);

	Set_Column_Address(Column, Column+3);    //设置列地址
	Set_Row_Address(Row, Row+31);            //设置行地址

	SPI_OLED_SendCommond(Write_RAM_Command);			//Enable MCU to write Data into RAM

	k=(Data-' ')*64;//求对空格的偏移量

	_1Bit_To_4Bit_16x32(&Fonts_AsicII_16x32[k]);


}

/**
 * @brief  写一行16x32数字、字母(ASIC II)   分辨率256*64，16*24==长16，宽32
 * @param  行，列，要写入的数字、字母         Row==行的左上角   Column==列的左边（总共64）
 * @retval None
 */
void Write_String_16x32AsicII(uint8_t Row, uint8_t Column, char Data[])
{
	uint8_t i;
	uint8_t k;
	k=strlen(Data);

	for(i=0;i<k;i++)
	{
	 Write_Single_16x32AsicII(Row,Column+4*i,Data[i]);
	}
}

/**
 * @brief  写一个12x24数字、字母(ASIC II)
 * @param  行，列，要写入的数字、字母
 * @retval None
 */
void Write_Single_12x24AsicII(uint8_t Row, uint8_t Column, char Data)
{
	uint8_t i;
	uint16_t k;

	SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);//设置扫描方向
	SPI_OLED_SendData(0x14);
	SPI_OLED_SendData(0x11);

	Set_Column_Address(Column, Column+2);    //设置列地址
	Set_Row_Address(Row, Row+23);            //设置行地址

	Set_WriteData();

	k=(Data-' ')*48;//求对空格的偏移量

	for(i=0;i<24;i++)
	{
		/*12*24 12bits用两个8bit组合描述，因此，每次需要访问两个u8*/
		Font_1Bit_To_4Bit_12x24(Fonts_AsicII_12x24[k],Fonts_AsicII_12x24[k+1]);
		k=k+2;
	}
}

/**
 * @brief  写一行12x24数字、字母(ASIC II)
 * @param  行，列，要写入的数字、字母
 * @retval None
 */
void Write_String_12x24AsicII(uint8_t Row, uint8_t Column, char Data[])
{
	uint8_t i;
	uint8_t k;
	k=strlen(Data);

	for(i=0;i<k;i++)
	{
		Write_Single_12x24AsicII(Row,Column+3*i,Data[i]);
	}
}


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------绘图部分-----------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/**
 * @brief  画一条横线
 * @param  起始点的行地址
 * @param  起始点的列地址
 * @param  结束点的列地址
 * @retval None
 */
void Display_Horizontal_Line(uint8_t Row, uint8_t Start_Column, uint8_t End_Column)
{
	uint8_t i;
	Set_Remap_Format(0x14);                      //行扫描模式（沿着一行进行扫描，也就是列地址自增） Horizontal address increment
	Set_Column_Address(Start_Column, End_Column);//设置列地址
	Set_Row_Address(Row, Row);                   //设置行地址

	Set_WriteData();//写使能

	for(i=0;i<End_Column-Start_Column+1;i++)
	{
		//写一列
		SPI_OLED_SendData(0xff);
		SPI_OLED_SendData(0xff);
	}

}

/**
 * @brief  画一条竖线竖线1(因为一列有四个像素)
 * @param  起始点的列地址
 * @param  起始点的行地址
 * @param  结束点的行地址
 * @retval None
 */
void Display_Vertical_Line_1(uint8_t Column, uint8_t Start_Row, uint8_t End_Row)
{
	uint8_t i;

	Set_Remap_Format(0x15);                //列扫描模式（沿着一列进行扫描，也就是行地址自增） Vertical address increment
	Set_Column_Address(Column, Column);    //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址

	Set_WriteData();//写使能

	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x10);
		SPI_OLED_SendData(0x00);
	}
	Set_Remap_Format(0x14);                //行扫描模式
}

/**
 * @brief  画一条竖线竖线2
 * @param  起始点的列地址
 * @param  起始点的行地址
 * @param  结束点的行地址
 * @retval None
 */
void Display_Vertical_Line_2(uint8_t Column, uint8_t Start_Row, uint8_t End_Row)
{
	uint8_t i;

	Set_Remap_Format(0x15);                //列扫描模式
	Set_Column_Address(Column, Column);    //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址

	Set_WriteData();//写使能

	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x0f);
		SPI_OLED_SendData(0x00);
	}
	Set_Remap_Format(0x14);                //行扫描模式
}

/**
 * @brief  画一条竖线竖线3
 * @param  起始点的列地址
 * @param  起始点的行地址
 * @param  结束点的行地址
 * @retval None
 */
void Display_Vertical_Line_3(uint8_t Column, uint8_t Start_Row, uint8_t End_Row)
{
	uint8_t i;

	Set_Remap_Format(0x15);                //列扫描模式
	Set_Column_Address(Column, Column);    //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址

	Set_WriteData();//写使能

	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x00);
		SPI_OLED_SendData(0xf0);
	}
	Set_Remap_Format(0x14);                //行扫描模式
}

/**
 * @brief  画一条竖线竖线4
 * @param  起始点的列地址
 * @param  起始点的行地址
 * @param  结束点的行地址
 * @retval None
 */
void Display_Vertical_Line_4(uint8_t Column, uint8_t Start_Row, uint8_t End_Row)
{
	uint8_t i;

	Set_Remap_Format(0x15);                //列扫描模式
	Set_Column_Address(Column, Column);    //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址

	Set_WriteData();//写使能

	for(i=0; i<End_Row-Start_Row+1; i++)
	{
		SPI_OLED_SendData(0x00);
		SPI_OLED_SendData(0x0f);
	}
	Set_Remap_Format(0x14);                //行扫描模式
}

/**
 * @brief  画一个矩形边框
 * @param  起始点的行地址
 * @param  起始点的列地址
 * @param  结束点的行地址
 * @param  结束点的列地址
 * @retval None
 */
void Display_Rectangular(uint8_t Start_Row, uint8_t Start_Column, uint8_t End_Row, uint8_t End_Column)
{
	Display_Horizontal_Line(Start_Row, Start_Column, End_Column);
	Display_Horizontal_Line(End_Row, Start_Column, End_Column);
	Display_Vertical_Line_1(Start_Column, Start_Row, End_Row);
	Display_Vertical_Line_1(End_Column, Start_Row, End_Row);
}

/**
 * @brief  画一个任意大小，颜色的实心矩形
 * @param  起始行、列，结束行、列，颜色
 * @retval None
 */
void Dsiplay_Solid_Rectangular(uint8_t Start_Row, uint8_t Start_Column, uint8_t End_Row, uint8_t End_Column, uint8_t Colour)
{
	uint16_t i;  //循环写入次数

	Set_Remap_Format(0x14);                //行扫描模式
	Set_Column_Address(Start_Column, End_Column);   //设置列地址
	Set_Row_Address(Start_Row, End_Row);   //设置行地址

	Set_WriteData();//写使能

	for(i=0; i<(End_Row-Start_Row+1)*(End_Column-Start_Column+1); i++)
	{
		SPI_OLED_SendData(Colour);
		SPI_OLED_SendData(Colour);
	}
}


/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*-------------------------------------------------------------------------------------------------界面部分-----------------------------------------------------------------------------------------------------*/
/*--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------主界面部分-----------------------------------------------------------------------------------------------------*/
/**
  * @brief  将电压值显示出来
  * @param  电压值
  * @retval None
  */
void Display_Main_Interface_Voltage(float Voltage)
{
	char String_voltage_real[7]={'0','0','0','.','0','0'};
	if(Voltage>=100)   		sprintf(String_voltage_real, "%6.2f", Voltage);//将float转为string，保留一位小数
	else if(Voltage>=9.99)
	{
		String_voltage_real[0]='0';
		sprintf(&String_voltage_real[1], "%5.2f", Voltage);//保留两位小数
	}
	else if(Voltage>=0)
	{
		String_voltage_real[0]='0';
		String_voltage_real[1]='0';
		sprintf(&String_voltage_real[2], "%4.2f", Voltage);

	}


	Write_String_16x32AsicII(0, 0, String_voltage_real);
	Write_String_16x32AsicII(0, 24, "V");


}

 /**
  * @brief  将电流值显示出来
  * @param  电流值
  * @retval None
  */
void Display_Main_Interface_Current(float Current)
{
	char String_Current_real[7]={'0','0','0','.','0','0'};
	if(Current>=100)     	sprintf(String_Current_real, "%6.2f", Current);//将float转为string，数据长度为6，保留两位小数
	else if(Current>=9.99)
	{
		String_Current_real[0]='0';
		sprintf(&String_Current_real[1], "%5.2f", Current);
	}
	else if(Current>=0)
	{
		String_Current_real[0]='0';
		String_Current_real[1]='0';
		sprintf(&String_Current_real[2],"%4.2f", Current);
	}

	Write_String_16x32AsicII(0,36, String_Current_real);
	Write_String_16x32AsicII(0,60, "A");

}

/**
 * @brief  显示功率
 * @param  Shift/not Shift
 * @retval None
 */
void Display_Main_Interface_Power(float Voltage,float Current)
{
	char String_Power_real[10]={'0','0','0','.','0','0'};
	float Power=Voltage*Current;


	if(Power>1000)
	{
		Power/=1000.0;
		String_Power_real[0]='0';
		String_Power_real[1]='0';
		sprintf(&String_Power_real[2], "%4.2f", Power);

		Write_String_16x32AsicII(32,0,String_Power_real);
		Write_String_16x32AsicII(32,24,"KW");
	}
	else
	{
		if(Power>=100)     	sprintf(String_Power_real, "%6.2f", Power);//将float转为string，数据长度为6，保留两位小数
		else if(Power>=9.99)
		{
			String_Power_real[0]=' ';
			sprintf(&String_Power_real[1], "%5.2f", Power);
		}
		else if(Power>=0)
		{
			String_Power_real[0]=' ';
			String_Power_real[1]=' ';
			sprintf(&String_Power_real[2],"%4.2f", Power);
		}

		Write_String_12x24AsicII(40,0,String_Power_real);
		Write_Single_12x24AsicII(40,24,'W');
	}

}


/**
 * @brief  显示键盘锁定状态
 * @param  显示ON，不显示OFF
 * @retval None
 */
static void Display_Main_Interface_Lock(uint8_t State)
{
	if(State==UNLOCK) Write_String_8x16AsicII(32,36, "      ");
	else 			  Write_String_8x16AsicII(32,36, "LOCK  ");

}

/**
 * @brief  显示处于设定/实际值/实时输出状态
 * @param  设定值状态Preset/实际值状态Actual
 * @retval None
 */

/*
static void Display_Main_Interface_Preset(uint8_t State)
{
	if(State==Preset)		  Write_String_8x16AsicII(48, 8, "Preset");
	else if(State==Actual)	  Write_String_8x16AsicII(48, 8, "Actual");
	else if (State == Output) Write_String_8x16AsicII(48, 8, "Output");
	else 					  Write_String_8x16AsicII(48, 8, "      ");
}
*/

/**
 * @brief  显示处于CV/CC/CP状态
 * @param  CV/CC/CP
 * @retval None
 */
void Display_Main_Interface_CV_CC_CP(uint8_t CV_CC_CP)
{
	uint8_t State;

	State = CV_CC_CP;
	switch(State)
	{
		case CV:
			Write_String_8x16AsicII(48, 36, "CV");break;
		case CC:
			Write_String_8x16AsicII(48, 36, "CC");break;
		default:
			{
				Write_String_8x16AsicII(48, 36, "  ");
				break;
			}
	}
}

/**
 * @brief  显示处于CYCLE和DELAY状态
 * @param  CYCLE_NOT_CYCLE
 * @param  DELAY_NOT_DELAY
 * @retval None
 */
/**
 * @brief  显示处于ON/OFF状态
 * @param  ON/OFF
 * @retval None
 */
void Display_Main_Interface_ON_OFF(uint8_t State)
{
	if(State==OUTPUT)
	{
		Write_String_8x16AsicII(48, 58, "ON ");
	}
	else
	{
		Write_String_8x16AsicII(48, 58, "OFF");
	}
}

/**
 * @brief  显示处于Shift/not Shift状态
 * @param  Shift/not Shift
 * @retval None
 */
static void Display_Main_Interface_Shift(uint8_t State)
{
	switch(State)
	{
		case Shift: Write_String_8x16AsicII(48,44, "Shift"); break;
		case Not_Shift: Write_String_8x16AsicII(48, 44, "     "); break;
		default: break;
	}
}


/**
 * @brief  显示处于CYCLE/DELAY状态
 * @param  Shift/not Shift
 * @retval None
 */
void Display_Main_Interface_CYCLE_DELAY(uint8_t CYCLE_NOT_CYCLE, uint8_t DELAY_NOT_DELAY)
{

	if (DELAY_NOT_DELAY == DELAY)
	{
		Write_String_8x16AsicII(48,46, "delay:ON ");
	}
	else
	{
		Write_String_8x16AsicII(48,46, "delay:OFF");
	}

	if (CYCLE_NOT_CYCLE == CYCLE)
	{
		Write_String_8x16AsicII(48, 24, "cycle:ON ");
	}
	else
	{
		Write_String_8x16AsicII(48, 24, "cycle:OFF");
	}


}




/**
* @brief  显示没有电流值的主界面（用于电压输入界面）
* @param  电压值
* @param  电流值
* @param  功率值
* @param  ON/OFF
* @param  CV/CC/CP
* @param  Preset/Actual
* @param  LUCK/UNLUCK
* @param  Shift/NotShift
* @retval None
*/
void Display_Main_Interface_V_set  (void)

{
	if(Keys_Encoder_Mode==Encoder_Mode)
	{
		Write_String_8x16AsicII(48,56,"VSET");


		/*显示上一电流设定值*/
		float Current=Set_Current/100.0;
		static char String_Current_real[7]={'0','0','0','.','0','0'};
		if(Current>=100)   		sprintf(String_Current_real, "%6.2f", Current);//将float转为string，保留一位小数
		else if(Current>=9.99)
		{
			String_Current_real[0]='0';
			sprintf(&String_Current_real[1], "%5.2f", Current);//保留两位小数
		}
		else if(Current>=0)
		{
			String_Current_real[0]='0';
			String_Current_real[1]='0';
			sprintf(&String_Current_real[2], "%4.2f", Current);

		}
		Write_String_16x32AsicII(16,36,String_Current_real);
		Write_Single_16x32AsicII(16,60,'A');
		/*显示上电压设定值*/
		float Voltage=String_to_float(String_Voltage);//实时设定值
		if(Voltage>=100)   		sprintf(String_Voltage, "%6.2f", Voltage);//将float转为string，保留一位小数
		else if(Voltage>=9.99)
		{
			String_Voltage[0]='0';
			sprintf(&String_Voltage[1], "%5.2f", Voltage);//保留两位小数
		}
		else if(Voltage>=0)
		{
			String_Voltage[0]='0';
			String_Voltage[1]='0';
			sprintf(&String_Voltage[2], "%4.2f", Voltage);

		}

		Write_String_16x32AsicII(16, 0, String_Voltage);
		Write_Single_16x32AsicII(16,24, 'V');


	}
	else if(Keys_Encoder_Mode==Keys_Mode)
	{
		Write_String_8x16AsicII(48,56,"VSET");
		Write_String_16x32AsicII(16,0,String_Voltage);

		/*显示上一电流设定值*/
		float Current=Set_Current/100.0;
		static char String_Current_real[7]={'0','0','0','.','0','0'};
		if(Current>=100)   		sprintf(String_Current_real, "%6.2f", Current);//将float转为string，保留一位小数
		else if(Current>=9.99)
		{
			String_Current_real[0]='0';
			sprintf(&String_Current_real[1], "%5.2f", Current);//保留两位小数
		}
		else if(Current>=0)
		{
			String_Current_real[0]='0';
			String_Current_real[1]='0';
			sprintf(&String_Current_real[2], "%4.2f", Current);

		}
		Write_String_16x32AsicII(16,36,String_Current_real);
		Write_Single_16x32AsicII(16,60,'A');
		/*显示当前电压设定值*/
		Write_String_16x32AsicII(16, 0, String_Voltage);
	}



}

/**
* @brief  显示没有电压值的主界面（用于电流输入界面）
* @param  电压值
* @param  电流值
* @param  功率值
* @param  ON/OFF
* @param  CV/CC/CP
* @param  Preset/Actual
* @param  LUCK/UNLUCK
* @param  Shift/NotShift
* @retval None
*/
void Display_Main_Interface_I_set  (void)

{
	if(Keys_Encoder_Mode==Encoder_Mode)
	{
		Write_String_8x16AsicII(48,56,"ISET");

		/*编码器模式下显示电压设定值*/
		float Voltage=Set_Voltage/100.0;
		static char String_Voltage_real[7]={'0','0','0','.','0','0'};
		if(Voltage>=100)   		sprintf(String_Voltage_real, "%6.2f", Voltage);//将float转为string，保留一位小数
		else if(Voltage>=9.99)
		{
			String_Voltage_real[0]='0';
			sprintf(&String_Voltage_real[1], "%5.2f", Voltage);//保留两位小数
		}
		else if(Voltage>=0)
		{
			String_Voltage_real[0]='0';
			String_Voltage_real[1]='0';
			sprintf(&String_Voltage_real[2], "%4.2f", Voltage);

		}

		Write_String_16x32AsicII(16, 0, String_Voltage_real);
		Write_Single_16x32AsicII(16,24, 'V');

		/*编码器模式下显示电流设定值*/
		float Current=String_to_float(String_Current);
		if(Current>=100)   		sprintf(String_Current, "%6.2f", Current);//将float转为string，保留一位小数
		else if(Current>=9.99)
		{
			String_Current[0]='0';
			sprintf(&String_Current[1], "%5.2f", Current);//保留两位小数
		}
		else if(Current>=0)
		{
			String_Current[0]='0';
			String_Current[1]='0';
			sprintf(&String_Current[2], "%4.2f", Current);

		}
		Write_String_16x32AsicII(16,36,String_Current);
		Write_Single_16x32AsicII(16,60,'A');


	}
	else if(Keys_Encoder_Mode==Keys_Mode)
	{
		Write_String_8x16AsicII(48,56,"ISET");

		/*按键模式下显示上一电压设定值*/
		float Voltage=Set_Voltage/100.0;
		static char String_Voltage_real[7]={'0','0','0','.','0','0'};
		if(Voltage>=100)   		sprintf(String_Voltage_real, "%6.2f", Voltage);//将float转为string，保留一位小数
		else if(Voltage>=9.99)
		{
			String_Voltage_real[0]='0';
			sprintf(&String_Voltage_real[1], "%5.2f", Voltage);//保留两位小数
		}
		else if(Voltage>=0)
		{
			String_Voltage_real[0]='0';
			String_Voltage_real[1]='0';
			sprintf(&String_Voltage_real[2], "%4.2f", Voltage);

		}

		Write_String_16x32AsicII(16, 0, String_Voltage_real);
		Write_Single_16x32AsicII(16,24, 'V');
		/*按键模式下显示当前电流设定值*/
		Write_String_16x32AsicII(16,36,String_Current);
	}


}




/**
 * @brief  主界面
 * @param  电压值
 * @param  电流值
 * @param  功率值
 * @param  ON/OFF
 * @param  CV/CC/CP
 * @param  Preset/Actual
 * @param  LUCK/UNLUCK
 * @param  Shift/NotShift
 * @retval None
 */
void Display_Main_Interface(const float Voltage,
							const float Current,
							const uint8_t ON_OFF,
	                        const uint8_t CV_CC_CP,
							const uint8_t Preset_Actual,
							const uint8_t LOCK_UNLOCK,
							const uint8_t Shift_NotShift)

{


		//Current_Actual=Current_real;
		Display_Main_Interface_Voltage(Voltage_Actual);
		Display_Main_Interface_Current(Current_Actual);
		Display_Main_Interface_Power(Voltage_Actual,Current_Actual);
		Display_Main_Interface_Lock(LOCK_UNLOCK);

		Display_Main_Interface_CV_CC_CP(CV_CC_CP);
		//Display_Main_Interface_CYCLE_DELAY(CYCLE_NOT_CYCLE, DELAY_NOT_DELAY);
		Display_Main_Interface_ON_OFF(OUTPUT_NOT_OUTPUT);
		Display_Main_Interface_Shift(Shift_NotShift);
		//Display_Vertical_Line_1(44,0,63);

//		Voltage_index++;

}

void Display_Param_Interface(const uint16_t Set_Voltage,const uint16_t Set_Current)
{


	float Voltage,Current;
	Voltage=Set_Voltage/100.0;
	Current=Set_Current/100.0;
	Display_Main_Interface_Voltage(Voltage);
	Display_Main_Interface_Current(Current);
	Display_Main_Interface_Power(Voltage,Current);
}

/*----------------------------------------------------------------------------------------------存取界面部分-----------------------------------------------------------------------------------------------------*/
/**
 * @brief  显示存取界面电压值
 * @param  设定的电压值，范围0到99.99V
 * @retval None
 */
static void Display_Access_Interface_Voltage(float Voltage)
{

	char string_voltage[7]={0};
	if(Voltage>=100)      sprintf(string_voltage, "%6.2f", Voltage);
	else if(Voltage>=10)
	{
		string_voltage[0]='0';
		sprintf(&string_voltage[1], "%5.2f", Voltage);

	}
	else if(Voltage>=0)
	{
		string_voltage[0]='0';
		string_voltage[1]='0';
		sprintf(&string_voltage[2], "%4.2f", Voltage);
	}
	if((xyz.coordinates1==3) && (xyz.coordinates2==2))//recall
	{
		Write_String_16x32AsicII(16,0,string_voltage);
		Write_Single_16x32AsicII(16,24,'V');
	}
	else
	{
		Write_String_16x32AsicII(16,0,string_voltage);
		Write_Single_16x32AsicII(16,24,'V');
	}


}
/**
 * @brief  显示存取界面电流值
 * @param  设定的电流值，范围0到999.99A
 * @retval None
 */
static void Display_Access_Interface_Current(float Current)
{
	char string_current[7]={0};
	if(Current>=100)      sprintf(string_current, "%6.2f", Current);
	else if(Current>=10)
	{
		string_current[0]='0';
		sprintf(&string_current[1], "%5.2f", Current);

	}
	else if(Current>=0)
	{
		string_current[0]='0';
		string_current[1]='0';
		sprintf(&string_current[2], "%4.2f", Current);
	}
	if((xyz.coordinates1==3) && (xyz.coordinates2==2))//recall
	{
		Write_String_16x32AsicII(16,36,string_current);
		Write_Single_16x32AsicII(16,60,'A');
	}
	else
	{
		Write_String_16x32AsicII(16,36,string_current);
		Write_Single_16x32AsicII(16,60,'A');
	}

}
/**
 * @brief  综合显示存取界面
 * @param  Recall/Save
 * @param  Recall/Save_Number
 * @param  频率值
 * @param  电压值
 * @param  电流值
 * @retval None
 */
void Display_Access_Interface_default(uint8_t Recall_Save, uint8_t Recall_Save_Number)
{
	//Display_Access_Interface_Recall_Save(Recall_Save, Recall_Save_Number);
	if(xyz.coordinates2==2)
	{
		/*load the data we save last time and display*/
		Write_String_16x32AsicII(0,14,"RECALL");
		Write_String_8x16AsicII(0,38,"RANK:");
		Write_Single_8x16AsicII(0,48,Recall_Save_Number+'0');


	}
	else if(xyz.coordinates2==1)
	{
		/*display the real value and press enter to save*/
		Write_String_16x32AsicII(0,14,"SAVE  ");
		Write_String_8x16AsicII(0,38,"RANK:");
		Write_Single_8x16AsicII(0,48,Recall_Save_Number+'0');


	}

}

void Display_Access_Interface_Save(void)
{

	Write_String_8x16AsicII(48,36,"SAVE  ");
	if(Recall_Save_Number<10)
	{
		Write_Single_8x16AsicII(48,60,'0');
		Write_Single_8x16AsicII(48,62,Recall_Save_Number+'0');
	}
	else Write_Single_8x16AsicII(48,60,Recall_Save_Number+'0');



	float voltage=String_To_Float(String_Save_V);
	Display_Access_Interface_Voltage(voltage);


	float current=String_To_Float(String_Save_I);
	Display_Access_Interface_Current(current);


}
void Display_Access_Interface_Recall(void)
{

	Write_String_8x16AsicII(48,36,"RECALL");
	if(Recall_Save_Number<10)
	{
		Write_Single_8x16AsicII(48,60,'0');
		Write_Single_8x16AsicII(48,62,Recall_Save_Number+'0');
	}
	else Write_Single_8x16AsicII(48,60,Recall_Save_Number+'0');


	//float voltage=String_To_Float(String_Recall_V);
	xyz.coordinates3=1;
	float voltage=AT25_Save_Recall_Recall_VI(Recall_Save_Number)/100.0;
	Display_Access_Interface_Voltage(voltage);

	xyz.coordinates3=2;
	//float current=String_To_Float(String_Recall_I);
	float current=AT25_Save_Recall_Recall_VI(Recall_Save_Number)/100.0;
	Display_Access_Interface_Current(current);

}


/*----------------------------------------------------------------------------------------------一级界面部分-----------------------------------------------------------------------------------------------------*/
 /**
  * @brief  显示一级菜单界面
  * @param  一级菜单的功能
  * @retval None
  */
void Display_Menu_Function_Interface_First_Menu(int Selection)
{

	Write_String_8x16AsicII(0,0,"FUNCTION");
	switch(Selection)
	{
		case 1:
			{
				Write_String_16x32AsicII(16,24,"SYSTEM     ");
				break;
			}
		case 2:
			{
				Write_String_16x32AsicII(16,24,"SLOPE      ");
				break;
			}
		case 3:
			{
				Write_String_16x32AsicII(16,24,"RS232       ");
				break;
			}
		case 4:
			{
				Write_String_16x32AsicII(16,24,"TIMER       ");
				break;
			}
		case 5:
			{
				Write_String_16x32AsicII(16,24,"AD SET      ");
				break;
			}
		case 6:
			{
				Write_String_16x32AsicII(16,18,"LIGHT TEMP  ");
				break;
			}
		case 7:
			{
				Write_String_16x32AsicII(16,18,"PROTECT     ");
				break;
			}
		case 8:
			{
				Write_String_16x32AsicII(16,18,"CALIRRATION ");
				break;
			}
		default :break;
	}
}
/*----------------------------------------------------------------------------------------------二级界面部分-----------------------------------------------------------------------------------------------------*/

static void second_munu_system(int z)
{

	if(z==1)
	{
		Write_String_16x32AsicII(16,16,"MINI_RESET");
		Cursor_flash_off();
		return;
	}
	else if(z==2)
	{
		Write_String_16x32AsicII(16,16,"MCU_RESET ");
		Cursor_flash_off();
		return;
	}
	else if(z==3)
	{
		Write_String_16x32AsicII(16,16,"ALL_RESET ");
		Cursor_flash_off();
		return;
	}

}

static void second_munu_slope(int z)
{
	Cursor_flash_on();
	if(z==1)
	{
		Write_String_16x32AsicII(16,0,"IRate");
		Write_String_16x32AsicII(16,24, String_I_Rate);//显示清空后的输入值

	}
	else if(z==2)
	{
		Write_String_16x32AsicII(16,0,"VRate");
		Write_String_16x32AsicII(16,24, String_V_Rate);//显示清空后的输入值

	}

}

static void second_munu_RS232(int z)
{
	Cursor_flash_off();
	Write_String_16x32AsicII(16,0,"BaudRate");

	if(z==1) 	  Write_String_16x32AsicII(16,36,String_RS232_Rate);
	else if(z==2);
}

static void second_menu_timer(int z)
{
	Cursor_flash_on();
	if(z==1)
	{
		Write_String_16x32AsicII(16,0,"delay ");

		Write_String_16x32AsicII(16,24, String_Delay);//显示输入值
	}
	else if(z==2)
	{
		Write_String_16x32AsicII(16,0,"Ton   ");

		Write_String_16x32AsicII(16,24, String_Cycle_On);//显示输入值
	}
	else if(z==3)
	{
		Write_String_16x32AsicII(16,0,"Toff  ");
		Write_String_16x32AsicII(16,24, String_Cycle_Close);//显示输入值
	}
}

static void second_menu_AD(int z)
{
	if(z==1)
	{
		Cursor_flash_on();
		/*输入密码界面*/
		Write_String_16x32AsicII(16,0, "INPUT:");
		if(Shift_Not_Shift == Shift)//显示输入的密码
		{
			Write_String_16x32AsicII(16,24, String_Password);
		}
		else if(Shift_Not_Shift == Not_Shift)
		{
			for(uint8_t i=0;i<6;i++)
			{
				if(String_Password[i]!=' ')	String_Password_mask[i]='*';
			}
			Write_String_16x32AsicII(16,24, String_Password_mask);

		}


	}
	if(z==2)
	{

		Write_String_8x16AsicII(0,0,"GainV:");
		Write_String_8x16AsicII(16,0,"GainI:");
		Write_String_8x16AsicII(32,0,"interceptV:");
		Write_String_8x16AsicII(48,0,"interceptI:");
		char String[9]={'0','0','.','0','0','0','0','0'};

		sprintf(String, "%6.5f", ADC_Gain_V);//将float转为string，数据长度为6，保留两位小数
		Write_String_8x16AsicII(0,26, String);
		sprintf(String, "%6.5f", ADC_Gain_I);//将float转为string，数据长度为6，保留两位小数
		Write_String_8x16AsicII(16,26, String);
		sprintf(String, "%6.5f", Eror_ADC_V);//将float转为string，数据长度为6，保留两位小数
		Write_String_8x16AsicII(32,26, String);
		sprintf(String, "%6.5f", Eror_ADC_I);//将float转为string，数据长度为6，保留两位小数
		Write_String_8x16AsicII(48,26, String);
	}
	else if(z==3)
	{
		char String[9]={'0','0','.','0','0','0','0','0'};
		Write_String_8x16AsicII(0,0,"ERROR_I:");
		sprintf(String, "%6.5f", Current_Error);//将float转为string，数据长度为6，保留两位小数
		Write_String_8x16AsicII(0,26, String);

	}
}

static void second_menu_Hard(int z)
{
	if(z==1)
	{
		Write_String_16x32AsicII(16,0,"Light ");

		String_Light_Level[0]=Light/10+'0';
		String_Light_Level[1]=Light%10+'0';
		Write_String_16x32AsicII(16,30, String_Light_Level);//显示输入值

		Cursor_flash_off();
		return;
	}
	else if(z==2)
	{
		Write_String_16x32AsicII(16,0,"Temp: ");
		Write_String_16x32AsicII(16,24, String_Temperature_CH1);//显示温度

		Cursor_flash_off();
		return;
	}
	else if(z==3)
	{
		Write_String_16x32AsicII(16,0,"FANS: ");
		Write_String_16x32AsicII(16,24, String_FAN);//转速

		Cursor_flash_off();
		return;
	}

}


static void second_menu_Calibration(int z)
{
	if(z==1)
	{
		Cursor_flash_on();
		Write_String_16x32AsicII(16,0,"POINT1");
		/*用户设定值*/
		Write_String_16x32AsicII(16,36,String_Calibration_Vlotage);
		Write_Single_8x16AsicII(48,44, 'V');

	}
	if(z==2)
	{
		Cursor_flash_on();
		Write_String_16x32AsicII(16,0,"POINT2");
		/*用户设定值*/
		Write_String_16x32AsicII(16,36,String_Calibration_Vlotage);
		Write_Single_8x16AsicII(48,44, 'V');

	}
	if(z==3)
	{
		Cursor_flash_off();
		Write_String_16x32AsicII(16,8,"Current_Zero");
		if(Flag.Current_error==0)
		{
			AT25_Save_AD_Param();
			Clear_Screen();
			xyz.coordinates1=0;
			xyz.coordinates2=0;
			xyz.coordinates3=0;
		    __HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);    //关了空闲中断
		    __HAL_UART_CLEAR_IDLEFLAG(&huart2);				//清除IDLE标志
		    __HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);	//清除IDLE标志
		    __HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);    	//使能空闲中断
		    UART_Start_Receive_DMA(&huart2,Uart2_Receive_buffer,9);
		}

	}
	if(z==4)
	{
		Cursor_flash_on();
		Write_String_16x32AsicII(16,0,"POINT1");
		/*用户设定值*/
		Write_String_16x32AsicII(16,36,String_Calibration_Current);
		Write_Single_8x16AsicII(48,44, 'A');

	}
	if(z==5)
	{
		Cursor_flash_on();
		Write_String_16x32AsicII(16,0,"POINT2");
		/*用户设定值*/
		Write_String_16x32AsicII(16,36,String_Calibration_Current);
		Write_Single_8x16AsicII(48,44, 'A');
	}


}

/**
 * @brief  显示二级菜单界面
 * @param  xyz.y
 * @param  xyz.z
 * @retval None
 */
void Display_Menu_Function_Interface_Second_Menu(int y,int z)
{

	Write_String_8x16AsicII(48,48, "2nd Menu");
	//一级菜单功能，显示在中间
	switch(y)
	{
		case 1://system
		{
			/*此处补充复位对象界面*/
			second_munu_system(z);
			break;
		}
		case 2://slope
		{
			second_munu_slope(z);
			break;
		}
		case 3://Communication
		{
			second_munu_RS232(z);
			break;
		}
		case 4://timer
		{
			second_menu_timer(z);
			break;
		}
		case 5://AD
		{
			second_menu_AD(z);
			break;

		}
		case 6:
		{
			second_menu_Hard(z);
			break;
		}
		/*
		case 7:
		{
			 second_menu_Portect(z);
		}
		*/
		case 8:
		{
			second_menu_Calibration(z);
			break;
		}
		default :break;
	}


}
/*----------------------------------------------------------------------------------------------保护界面部分-----------------------------------------------------------------------------------------------------*/
 /**
  * @brief  显示保护界面
  * @param  出现的错误类型
  * @retval None
  */
void Display_Protect_Interface(uint8_t Error)
{

	Cursor_flash_off();
	uint8_t mask=0x01;
	static uint8_t count=0;

	count=0;
	if((Error&mask)==mask)//0x01
	{
		count++;
	}
	mask<<=1;
	if((Error&mask)==mask)//0x01
	{
		count++;
	}
	mask<<=1;
	if((Error&mask)==mask)//0x01
	{
		count++;
	}
	mask=0x01;

	switch(count)
	{
		/*无警报，返回主界面*/
		case 0:
		{
			Clear_Screen();
			xyz.coordinates1=0;
			xyz.coordinates2=0;
			xyz.coordinates3=0;
		}
		case 1:
		{
			if((Error&0x01)==0x01)
			{
				Write_String_16x32AsicII(16,4,"   ");
				Write_String_16x32AsicII(16,18,"  ");
				Write_String_16x32AsicII(16,26,"OVP");
				Write_String_16x32AsicII(16,38,"  ");
				Write_String_16x32AsicII(16,44,"    ");

			}
			else if((Error&0x02)==0x02)
			{
				Write_String_16x32AsicII(16,4,"   ");
				Write_String_16x32AsicII(16,18,"  ");
				Write_String_16x32AsicII(16,26,"OCP");
				Write_String_16x32AsicII(16,38,"  ");
				Write_String_16x32AsicII(16,44,"    ");

			}
			else if((Error&0x04)==0x04)
			{
				Write_String_16x32AsicII(16,4,"   ");
				Write_String_16x32AsicII(16,18,"  ");
				Write_String_16x32AsicII(16,26,"OTP");
				Write_String_16x32AsicII(16,38,"  ");
				Write_String_16x32AsicII(16,44,"    ");
			}
			break;

		}
		case 2:
		{
			if((Error&0x02)==0x02 && (Error&0x04)==0x04)
			{

				Write_String_16x32AsicII(16,18,"OCP");
				Write_String_16x32AsicII(16,30," ");
				Write_String_16x32AsicII(16,34,"OTP");

			}
			else if((Error&0x01)==0x01 && (Error&0x04)==0x04)
			{

				Write_String_16x32AsicII(16,18,"OVP");
				Write_String_16x32AsicII(16,30," ");
				Write_String_16x32AsicII(16,34,"OTP");

			}
			else if((Error&0x02)==0x02 && (Error&0x01)==0x01)
			{
				Write_String_16x32AsicII(16,18,"OVP");
				Write_String_16x32AsicII(16,30," ");
				Write_String_16x32AsicII(16,34,"OCP");
			}
			break;
		}
		case 3:
		{
			Write_String_16x32AsicII(16,18," ");
			Write_String_16x32AsicII(16,4,"OVP");
			Write_String_16x32AsicII(16,24,"OCP");
			Write_String_16x32AsicII(16,44,"OTP");
			Write_String_16x32AsicII(16,36," ");
			break;
		}

	}



}

/*----------------------------------------------------------------------------------------------隐藏界面部分-----------------------------------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------------------------其他显示部分-----------------------------------------------------------------------------------------------------*/
/**
  * @brief  将RS232 ID号转为字符串
  * @param  ID号
  * @retval None
  */
void Display_RS232_ID(uint16_t RS232_ID)
{
	if(RS232_ID >= 10)
	{
		sprintf(String_RS232_ID, "%2d", RS232_ID);
	}
	else
	{
		sprintf(String_RS232_ID, "%2d", RS232_ID);
		String_RS232_ID[0] = '0';
	}

	Write_String_16x32AsicII(0, 12, String_RS232_ID);
}
/**
  * @brief  将RS232 波特率转为字符串
  * @param  波特率
  * @retval None
  */
void Display_RS232_BaudRate(int RS232_BaudRate)
{
	if (RS232_BaudRate >= 100000)
	{
		sprintf(String_RS232_BaudRate, "%6d", RS232_BaudRate);
	}
	else if (RS232_BaudRate >= 10000)
	{
		sprintf(String_RS232_BaudRate, "%6d", RS232_BaudRate);
		String_RS232_BaudRate[0] = '0';
	}
	else if (RS232_BaudRate >= 1000)
	{
		sprintf(String_RS232_BaudRate, "%6d", RS232_BaudRate);
		String_RS232_BaudRate[0] = '0';
		String_RS232_BaudRate[1] = '0';
	}
	else if (RS232_BaudRate >= 100)
	{
		sprintf(String_RS232_BaudRate, "%6d", RS232_BaudRate);
		String_RS232_BaudRate[0] = '0';
		String_RS232_BaudRate[1] = '0';
		String_RS232_BaudRate[2] = '0';
	}

  Write_String_16x32AsicII(32, 40, String_RS232_BaudRate);
}







/**
 * @brief  显示V-Rate时间的设置界面
 * @param	None
 * @retval None
 */
void Display_Oled_V_Rate(void)
{

	String_V_Rate[0] = ' ';
	String_V_Rate[1] = ' ';
	String_V_Rate[2] = (Set_V_Slope / 10) % 10 + '0';
	String_V_Rate[3] = '.';
	String_V_Rate[4] = Set_V_Slope % 10 + '0';

	String_V_Rate[5] = 'v';
	String_V_Rate[6] = '/';
	String_V_Rate[7] = 'm';
	String_V_Rate[8] = 's';
	String_V_Rate[9] = ' ';
	Write_String_16x32AsicII(0, 0, "V-Rate:");          //在OLED屏显示
	Write_String_16x32AsicII(32, 12, String_V_Rate);

}
/**
 * @brief  显示V-Rate时间的设置界面
 * @param  None
 * @retval None
 */
void Display_Oled_I_Rate(void)
{


	String_I_Rate[0] = ' ';
	String_I_Rate[1] = ' ';
	String_I_Rate[2] = (Set_I_Slope / 10) % 10 + '0';
	String_I_Rate[3] = '.';
	String_I_Rate[4] = Set_I_Slope % 10 + '0';

	String_I_Rate[5] = 'A';
	String_I_Rate[6] = '/';
	String_I_Rate[7] = 'm';
	String_I_Rate[8] = 's';
	String_I_Rate[9] = ' ';
	Write_String_16x32AsicII(0, 0, "I-Rate:");          //在OLED屏显示
	Write_String_16x32AsicII(32, 12, String_I_Rate);

}


void Cursor_flash(void)
{
	static int count;
	uint8_t clear_arry[16*64*2]={0x00};
	count++;
	if(count==2)
	{
		count=0;

		SPI_OLED_SendCommond(Set_Re_map_and_Dual_COM_Line_mode);
		SPI_OLED_SendData(0x14);
		SPI_OLED_SendData(0x11);

		Set_Column_Address(Cursor_Position*4, Cursor_Position*4+1);
		Set_Row_Address(0, 0+15);
		SPI_OLED_SendCommond(Write_RAM_Command);//Enable MCU to write Data into RAM
		for(uint8_t i=0;i<16;i++)
		{
			Font_1Bit_To_4Bit(Pointleft[i]);
		}
		Set_Column_Address(Cursor_Position*4+2, Cursor_Position*4+3);
		Set_Row_Address(0, 0+15);
		SPI_OLED_SendCommond(Write_RAM_Command);//Enable MCU to write Data into RAM
		for(uint8_t i=0;i<16;i++)
		{
			Font_1Bit_To_4Bit(Pointright[i]);
		}

	}
	else
	{

		Set_Column_Address(0x00,0x3f);//0到63列
		Set_Row_Address(0x00,15);   //0到15行
		SPI_OLED_SendCommond(Write_RAM_Command);//Enable MCU to write Data into RAM;//Enable MCU to write Data into RAM

		__HAL_TIM_DISABLE_IT(&htim3, TIM_IT_UPDATE); //关闭编码器中断

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1,GPIO_PIN_SET);//dc 1

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_RESET);//cs 0

		HAL_SPI_Transmit(&hspi1,clear_arry,32*64,100);

		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0,GPIO_PIN_SET);//cs 1

		__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE); //开启编码器中断

	}


}

void Cursor_flash_off(void)
{

	for(uint8_t i=0;i<64;i++)
	{

		Write_Single_8x16AsicII(0,i,' ');

	}
	__HAL_TIM_DISABLE(&htim6);//cusor flash off

}

void Cursor_flash_on(void)
{
	__HAL_TIM_ENABLE(&htim6);//开启闪烁
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------------综合界面显示-----------------------------------------------------------------------------------------*/
/*----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

 /**
  * @brief  综合各个界面的显示函数
  * @param  需要显示的界面
  * @retval None
  */
void Display_Interface(void)
{
	if(Sleep_ON_OFF!=0xff)
	{
		Clear_Screen();
		return;
	}
	if(Last_Interface.coordinates1!=xyz.coordinates1
	   ||Last_Interface.coordinates2!=xyz.coordinates2
	   ||Last_Interface.coordinates3!=xyz.coordinates3)
	{
		Clear_Screen();

	}
	/*main interface*/
	if((xyz.coordinates1==0) && (xyz.coordinates2==0) && (xyz.coordinates3==0))
	{
		Display_Main_Interface(Voltage_real,Current_real,ON_OFF,CV_CC_CP,Actual,LOCK_UNLOCK,Shift_Not_Shift);
	}
	/*first menu*/
	if((xyz.coordinates1==1) && (xyz.coordinates3==0))
	{
		Display_Menu_Function_Interface_First_Menu(xyz.coordinates2);
	}
	/*second menu*/
	if((xyz.coordinates1==1) && (xyz.coordinates2!=0) && (xyz.coordinates3!=0))
	{
		Display_Menu_Function_Interface_Second_Menu(xyz.coordinates2,xyz.coordinates3);
	}
	/*V_set I_set*/
	if(xyz.coordinates1==2)
	{
		if(xyz.coordinates2==1 ) 	 Display_Main_Interface_I_set();
		else if(xyz.coordinates2==2) Display_Main_Interface_V_set();
	}
	/*Reacll Save*/
	if(xyz.coordinates1==3)
	{
		if(xyz.coordinates3==0)
		{
			//Display_Access_Interface_default(Recall_Save,Recall_Save_Number);
		}
		//save
		if( (xyz.coordinates2==1) && (xyz.coordinates3!=0))
		{
			Display_Access_Interface_Save();
		}
		//recall
		else if( (xyz.coordinates2==2) && (xyz.coordinates3!=0))
		{
			Display_Access_Interface_Recall();
		}

	}
	/*Protect*/
	if(xyz.coordinates1==4)
	{
		Display_Protect_Interface(Error);
	}
	/*advance interface*/
	if((xyz.coordinates1==9) && (xyz.coordinates2==9) && (xyz.coordinates3==9))
	{
		Display_Param_Interface(Set_Voltage,Set_Current);
	}

	Last_Interface=xyz;//记录退出前界面

}





