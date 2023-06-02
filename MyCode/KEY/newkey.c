#include "newkey.h"

/*-------------------------------------------------------------内部标志位---------------------------------------------------------------------*/
FunctionalState keys_EN = DISABLE;												 //按键使能控制标志位，初始化为失能，软起动后使能
FunctionalState Inner_Mode=DISABLE;
uint8_t Shift_Not_Shift = Not_Shift;                                             //定义复用键的标志位（默认不复用）
uint8_t ON_OFF = OFF;                                                            //定义输出关闭标志位（默认不输出,只是用来显示）
uint8_t OUTPUT_NOT_OUTPUT = NOT_OUTPUT;                                          //定义是否已经输出标志位
uint8_t CV_CC_CP = 0x00;                                                           //定义输出类型标志位 (默认恒压输出)

uint8_t LOCK_UNLOCK = UNLOCK;                                                    //定义锁定标志位（默认不锁定）

uint8_t Third_Menu_Flag=1;														 //定义三级菜单下完成设定标志
uint8_t COUNT_ESC=0;															 //ESC按键统计值
uint8_t Sleep_ON_OFF=0xff;

uint8_t Keys_Encoder_Mode = Encoder_Mode;                                        //定义用键盘输入还是用编码器输入(默认用编码器输入)
uint8_t RS232_BaudRate_State = 0;                                                //定义设定波特率状态
uint8_t Recall_Save_Number = 0;                                                  //定义读取保存的数据组（默认第0组）
uint8_t Cursor_Position;                                                   	 	 //定义光标位置的全局变量
/*-------------------------------------------------------------可设置参数----------------------------------------------------------------------*/
uint16_t Cycle_On_s = 0;                                                         //定义循环时间的秒
uint16_t Cycle_On_ms = 300;                                                        //定义循环时间的毫秒
uint16_t Cycle_Close_s = 0;                                                      //定义循环关闭时间的秒
uint16_t Cycle_Close_ms = 300;                                                     //定义循环关闭时间的毫秒

uint16_t Delay_s;                                                                  //定义延时的秒
uint16_t Delay_ms;                                                                  //定义延时的秒

uint8_t Set_V_Slope;//设定的电压变化率
uint8_t Set_I_Slope;//设定的电流变化率
uint16_t Set_Voltage=100;  //设定电压值
uint16_t Set_Current=100;  //设定电流值
uint16_t Set_Power;

float Voltage_Actual;
float Current_Actual;

float Vref[2]={0x00};
float Iref[2]={0x00};
uint16_t data_V[2]={0x00};
uint16_t data_I[2]={0x00};

/*key scan*/
uint16_t Key_Value1=0x7f;
uint16_t Key_Value2=0x7f;
uint16_t Key_Value3=0x7f;
/*key flag*/
int Row_Flag=0;
FunctionalState  Key_Enable=DISABLE;
const char Password[7]={'1','1','4','5','1','4'};

/***********************************************************************************************************************************************************************************/
/*********************************************************************************以下为函数接口*************************************************************************************/
/***********************************************************************************************************************************************************************************/

float String_to_float(char* string)
{
		float value=0.0;

		uint8_t point_index=0;
		uint8_t data_length=0;
		for(uint8_t i=0;i<strlen(string);i++)
		{
			if(string[i]=='.')
			{
				point_index=i;
				break;
			}

		}
		if(point_index==0)
		{
			for(uint8_t j=0;j<strlen(string);j++)
			{
				if(string[j]<='9' && string[j]>='0')
				{
					data_length++;
				}

			}
			for(uint8_t j=0;j<data_length;j++)
			{
				value+=(string[j]-'0')*(float)pow(10,(data_length-j-1));
			}
		}
		else
		{
			/*旧版 有bug*/
			/*
			for(uint8_t j=0;j<point_index;j++)
			{
				value+=(string[j]-'0')*(float)pow(10,(point_index-j-1));
			}
			for(uint8_t j=1;j<3;j++)
			{
				value+=(string[point_index+j]-'0')/(float)pow(10,j);
			}
			*/
			/*新版*/
			for(uint8_t i=0;i<point_index;i++)
			{
				value+=(string[i]-'0')*pow(10,(point_index+1-i));
			}
			/*小数后两位*/
			for(uint8_t j=1;j<3;j++)
			{
				if(string[j+point_index]!=' ')
				{
					value+=(string[j+point_index]-'0')*pow(10,2-j);
				}
				else value+=0;

			}
			value/=100.0;


		}


		return value;




}

/**
 * @brief  将设定电压电流功率字符数组变为浮点数
 * @param	数组名
 * @retval 对应的浮点数
 */
float String_To_Float(char* string)
{

	float sum=0.0;
	/*V set I set*/
	if((xyz.coordinates1==2) && (xyz.coordinates2!=0))
	{
		for(uint8_t i=0;i<3;i++)
		{
			sum+=(string[i]-'0')*(pow(10,(2-i)));
		}
		for(uint8_t i=4;i<=5;i++)//4 1 5 2
		{
			sum+=(string[i]-'0')/(float)(pow(10,(i-3)));
		}
		return sum;
	}
	//rata set
	if((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3!=0))
	{
		for(uint8_t i=0;i<1;i++)
		{
			sum+=(string[i]-'0')*(pow(10,(0-i)));
		}
		for(uint8_t i=2;i<=3;i++)//4 1 5 2
		{
			sum+=(string[i]-'0')/(float)(pow(10,(i-1)));
		}
		return sum;
	}
	if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3==1))
	{
		for(uint8_t i=0;i<2;i++)
		{
			sum+=(string[i]-'0')*(pow(10,(2-i)));
		}

		return sum;
	}
	if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3==2))
	{
		for(uint8_t i=0;i<2;i++)
		{
			sum+=(string[i]-'0')*(pow(10,(2-i)));
		}
		for(uint8_t i=4;i<=5;i++)
		{
			sum+=(string[i]-'0')/(float)(pow(10,(i-3)));
		}

		return sum;
	}
	/*save load*/
	if((xyz.coordinates1==3) && (xyz.coordinates2!=0) && (xyz.coordinates3!=0))
	{
		for(uint8_t i=0;i<=2;i++)
		{
			sum+=(string[i]-'0')*(pow(10,(2-i)));
		}
		for(uint8_t i=4;i<=5;i++)
		{
			sum+=(string[i]-'0')/(float)(pow(10,(i-3)));
		}

		return sum;
	}
	/*calibration*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3!=0))
	{
		for(uint8_t i=0;i<=2;i++)
		{
			sum+=(string[i]-'0')*(pow(10,(2-i)));
		}
		for(uint8_t i=4;i<=5;i++)
		{
			sum+=(string[i]-'0')/(float)(pow(10,(i-3)));
		}

		return sum;
	}

	else return 0.0;
}


/**
 * @brief  数字键盘输入设定电压的处理函数
 * @param  数字键的号码（1，2，3，4，5，6，7，8，9，0）
 * @retval None
 */
static void Keys_Input_Set_Voltage(char Number)
{
	//处于设定电压模式、无锁定和非保护状态
	if( (xyz.coordinates1==2) && (xyz.coordinates2==2) && (LOCK_UNLOCK == UNLOCK) )
	{
		Cursor_flash_off();
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		    Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
		    Cursor_flash_off();
			Cursor_Position = 0;                                    //指针设为0位

			String_Voltage[0] = ' ';
			String_Voltage[1] = ' ';
			String_Voltage[2] = ' ';
			String_Voltage[3] = ' ';
			String_Voltage[4] = ' ';
			String_Voltage[5] = ' ';

		}


		String_Voltage[Cursor_Position] = Number;                  //使光标位置置Number
		Cursor_Position ++;                                        //光标位置加1
		if(Cursor_Position==6) Cursor_Position=0;

	}

}

/**
 * @brief  数字键盘输入设定电流的处理函数
 * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
 * @retval None
 */
static void Keys_Input_Set_Current(char Number)
{
	if( (xyz.coordinates1==2) && (xyz.coordinates2==1) && (LOCK_UNLOCK == UNLOCK) )
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
		    Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
		    Cursor_flash_off();
			Cursor_Position = 9;                                    //指针设为0位

			String_Current[0] = ' ';
			String_Current[1] = ' ';
			String_Current[2] = ' ';
			String_Current[3] = ' ';
			String_Current[4] = ' ';
			String_Current[5] = ' ';

		}


		String_Current[Cursor_Position-9] = Number;                  //使光标位置置Number
		Cursor_Position ++;                                        //光标位置加1


	}

}

/**
 * @brief  数字键盘输入设定V_Rate时间的处理函数（暂定0-30）
 * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
 * @retval None
 */
static void Keys_Input_Set_V_Rate(char Number)
{

	if( (xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==2) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
			Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
			Cursor_flash_off();
			Cursor_Position = 6;                                     //指针设为0位

			String_V_Rate[0] = '0';                                 //初始化
			String_V_Rate[1] = '.';
			String_V_Rate[2] = '0';
			String_V_Rate[3] = '0';
			String_V_Rate[4] = ' ';

			String_V_Rate[5] = 'V';
			String_V_Rate[6] = '/';
			String_V_Rate[7] = 'm';
			String_V_Rate[8] = 's';

		}

		if(Cursor_Position==0+6)
		{
			String_V_Rate[Cursor_Position-6] = Number;
			Cursor_Position=2+6;
			return;
		}
		if(Cursor_Position == 3+6)
		{
			String_V_Rate[Cursor_Position-6] = Number;
			Cursor_Position = 0+6;
			return;
		}
		String_V_Rate[Cursor_Position-6] = Number;                  //使光标位置置Number
		Cursor_Position ++;                                        //光标位置加1
	}


}
/**
 * @brief  数字键盘输入设定I_Rate时间的处理函数（暂定0-10）
 * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
 * @retval None
 */
static void Keys_Input_Set_I_Rate(char Number)
{
	if( (xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==1) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Keys_Encoder_Mode != Keys_Mode)                           //第一次按下按键
		{
			Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
			Cursor_Position = 6;                                     //指针设为0位
			Cursor_flash_off();
			String_I_Rate[0] = '0';                                  //初始化接收输入的字符串
			String_I_Rate[1] = '.';
			String_I_Rate[2] = '0';
			String_I_Rate[3] = '0';
			String_I_Rate[5] = ' ';
			String_I_Rate[6] = 'A';
			String_I_Rate[7] = '/';
			String_I_Rate[8] = 'm';
			String_I_Rate[9] = 's';
		}


		if(Cursor_Position==0+6)
		{
			String_I_Rate[Cursor_Position-6] = Number;
			Cursor_Position=2+6;
			return;
		}
		if(Cursor_Position == 3+6)
		{
			String_I_Rate[Cursor_Position-6] = Number;
			Cursor_Position = 0+6;
			return;
		}
		String_I_Rate[Cursor_Position-6] = Number;                  //使光标位置置Number
		Cursor_Position ++;                                       //光标位置加1

	}
}


/**
 * @brief  数字键盘输入设定串口ID的处理函数
 * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
 * @retval None
 */
static void Keys_Input_RS232(char Number)
{
	if( (xyz.coordinates1==1) && (xyz.coordinates2==3) && (xyz.coordinates3==1) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
			Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
			Cursor_flash_off();
			Cursor_Position = 0;                                     //指针设为0位
			String_RS232_ID[0] = ' ';                                 //清空字符串
			String_RS232_ID[1] = ' ';
		}

		if(Cursor_Position == 2)
		{
			Cursor_Position = 0;                                     //如果光标超过范围，回到启始位置
			return;
		}

		String_RS232_ID[Cursor_Position] = Number;                  //使光标位置置Number

		Cursor_Position ++;                                         //光标位置加1
		Write_String_8x16AsicII(48,38,String_RS232_ID);           //显示输入值
	}
}


/**
 * @brief  数字键盘输入设定Cycle时间的处理函数
 * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
 * @retval None
 */
static void Keys_Input_Cycle(char Number)
{
	//cycle on time
	if ((xyz.coordinates1==1) && (xyz.coordinates2==4) && (xyz.coordinates3==2) && (LOCK_UNLOCK == UNLOCK))
	{
		String_Cycle_On[4]='s';
		String_Cycle_On[8]='m';
		String_Cycle_On[9]='s';
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
			Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
			Cursor_flash_off();
			Cursor_Position = 0+6;                                     //指针设为0位
		}
		if( (Cursor_Position<4+6) || ((Cursor_Position>4+6) && (Cursor_Position<8+6)) )
		{
			String_Cycle_On[Cursor_Position-6] = Number;//光标位置正确，使光标位置置Number
		}

		Cursor_Position ++;                                         //光标位置加1

		if(Cursor_Position == 4+6)
		{
			Cursor_Position +=1;//位置4不设置，跳过到下一位
		}
		if(Cursor_Position==8+6)
		{
			Cursor_Position=0+6;
			return;
		}

	}
	//cycle off time
	else if ((xyz.coordinates1==1) && (xyz.coordinates2==4) && (xyz.coordinates3==3) && (LOCK_UNLOCK == UNLOCK))
	{
		String_Cycle_Close[4]='s';
		String_Cycle_Close[8]='m';
		String_Cycle_Close[9]='s';
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
			Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
			Cursor_flash_off();
			Cursor_Position = 0+6;                                     //指针设为0位

		}
		if( Cursor_Position<4+6 || ((Cursor_Position>4+6)&&(Cursor_Position<8+6)) )
		{
			String_Cycle_Close[Cursor_Position-6] = Number;//光标位置正确，使光标位置置Number
		}

		Cursor_Position ++;                                            //光标位置加1

		if(Cursor_Position == 4+6)
		{
			Cursor_Position +=1;//位置4不设置，跳过
		}
		if(Cursor_Position==8+6)//越界而没有按下enter，复位清空
		{
			Cursor_Position=0+6;
			return;

		}

	}

}

/**
 * @brief  数字键盘输入设定Delay时间的处理函数
 * @param	数字键的号码（1，2，3，4，5，6，7，8，9，0）
 * @retval None
 */
static void Keys_Input_Delay(char Number)
{
	//设置软起动延时
	if((xyz.coordinates1==1) && (xyz.coordinates2==4) && (xyz.coordinates3==1) && (LOCK_UNLOCK == UNLOCK))
	{
		String_Delay[4]='s';
		String_Delay[8]='m';
		String_Delay[9]='s';
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
			Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
			Cursor_flash_off();
			Cursor_Position = 6;                                     //指针设为0位
		}

		if( Cursor_Position<4+6 || ((Cursor_Position>4+6)&&(Cursor_Position<8+6)) )
		{
			String_Delay[Cursor_Position-6] = Number;//光标位置正确，使光标位置置Number
		}

		Cursor_Position ++;                                            //光标位置加1

		if(Cursor_Position == 4+6)
		{
			Cursor_Position +=1;//位置4不设置，跳过
		}
		if(Cursor_Position==8+6)//越界而没有按下enter，复位清空
		{
			Cursor_Position=0+6;
			return;
		}
	}

}

static void Keys_Input_Save(char Number)
{
	if((xyz.coordinates1==3) && (xyz.coordinates2==1) && (xyz.coordinates3!=0) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
			Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
			Cursor_flash_off();
			Cursor_Position = 0;                                     //指针设为0位
			String_Save_V[0]=' ';
			String_Save_V[1]=' ';
			String_Save_V[2]=' ';
			String_Save_V[3]=' ';
			String_Save_V[4]=' ';
			String_Save_V[5]=' ';

			String_Save_I[0]=' ';
			String_Save_I[1]=' ';
			String_Save_I[2]=' ';
			String_Save_I[3]=' ';
			String_Save_I[4]=' ';
			String_Save_I[5]=' ';


		}
		if(xyz.coordinates3==1)//voltage set
		{

			if(Cursor_Position==2)
			{
				String_Save_V[Cursor_Position]=Number;
				Cursor_Position=4;
				return;
			}
			if(Cursor_Position==5)
			{
				String_Save_V[Cursor_Position]=Number;
				Cursor_Position=0;
				return;
			}
			String_Save_V[Cursor_Position]=Number;
			Cursor_Position++;
		}
		else if(xyz.coordinates3==2)//current set
		{
			if(Cursor_Position==2+9)
			{
				String_Save_I[Cursor_Position-9]=Number;
				Cursor_Position=4+9;
				return;
			}
			if(Cursor_Position==5+9)
			{
				String_Save_I[Cursor_Position-9]=Number;
				Cursor_Position=0+9;
				return;
			}
			String_Save_I[Cursor_Position-9]=Number;
			Cursor_Position++;
		}


	}


}

static void Keys_Input_Recall(char Number)
{
	if((xyz.coordinates1==3) && (xyz.coordinates2==2) && (xyz.coordinates3!=0) && (LOCK_UNLOCK == UNLOCK))
	{
		//不可参数设定

	}
}




static void Keys_Input_Password(char Number)
{
	if( (xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3==1) && (LOCK_UNLOCK == UNLOCK) )
	{


		if(Cursor_Position == 5+6)
		{
			String_Password[Cursor_Position-6] = Number;
			Cursor_Position = 0+6;
			return;
		}
		String_Password[Cursor_Position-6] = Number;                  //使光标位置置Number
		Cursor_Position ++;                                        //光标位置加1

	}
}

static void Keys_Input_Calibration_Voltage(char Number)
{
	//处于设定电压模式、无锁定和非保护状态
	if( (xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3==1||xyz.coordinates3==2) && (LOCK_UNLOCK == UNLOCK) )
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
			Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
			Cursor_flash_off();

			Cursor_Position = 9;                                    //指针设为0位

			String_Calibration_Vlotage[3] = '.';

		}

		if(Cursor_Position==2+9)
		{
			String_Calibration_Vlotage[Cursor_Position-9] = Number;
			Cursor_Position=4+9;
			return;
		}
		if(Cursor_Position == 5+9)
		{
			String_Calibration_Vlotage[Cursor_Position-9] = Number;
			Cursor_Position = 0+9;
			return;
		}
		String_Calibration_Vlotage[Cursor_Position-9] = Number;                  //使光标位置置Number
		Cursor_Position ++;                                        //光标位置加1

	}

}

static void Keys_Input_Calibration_Current(char Number)
{
	//处于设定电压模式、无锁定和非保护状态
	if( (xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3==3||xyz.coordinates3==4) && (LOCK_UNLOCK == UNLOCK) )
	{
		if(Keys_Encoder_Mode != Keys_Mode)                         //第一次按下按键
		{
			Keys_Encoder_Mode = Keys_Mode;                           //切换到键盘模式
			Cursor_flash_off();

			Cursor_Position = 9;                                    //指针设为0位

			String_Calibration_Current[3] = '.';

		}

		if(Cursor_Position==2+9)
		{
			String_Calibration_Current[Cursor_Position-9] = Number;
			Cursor_Position=4+9;
			return;
		}
		if(Cursor_Position == 5+9)
		{
			String_Calibration_Current[Cursor_Position-9] = Number;
			Cursor_Position = 0+9;
			return;
		}
		String_Calibration_Current[Cursor_Position-9] = Number;                  //使光标位置置Number
		Cursor_Position ++;                                        //光标位置加1

	}

}

/*----------------------------------------------------------------按键值处理响应函数------------------------------------------------------------------------*/

/**
 * @brief  数字按键的扫描服务函数
 * @param  数字0~9&point(10)
 * @retval None
 */
void key_num_handle(int key_number)
{
	//number
	if(key_number<=9&&key_number>=0)
	{
		char number=key_number+'0';
		Keys_Input_Set_Voltage(number);
		Keys_Input_Set_Current(number);
		Keys_Input_Set_V_Rate(number);
		Keys_Input_Set_I_Rate(number);
		Keys_Input_RS232(number);
		Keys_Input_Cycle(number);
		Keys_Input_Delay(number);
		Keys_Input_Save(number);
		Keys_Input_Recall(number);
		Keys_Input_Password(number);
		Keys_Input_Calibration_Voltage(number);
		Keys_Input_Calibration_Current(number);

	}
	//point
	else if(key_number==10)
	{
		char number='.';
		Keys_Input_Set_Voltage(number);
		Keys_Input_Set_Current(number);
		Keys_Input_Set_V_Rate(number);
		Keys_Input_Set_I_Rate(number);
		Keys_Input_RS232(number);
		Keys_Input_Cycle(number);
		Keys_Input_Delay(number);
		Keys_Input_Save(number);
		Keys_Input_Recall(number);
	}

}

/**
 * @brief  按键V-set的扫描服务函数
 * @param	None
 * @retval None
 */
void Key_V_set(void)
{
	if (LOCK_UNLOCK == UNLOCK)
	{
		if(Shift_Not_Shift == Shift)   //当复用该按键时，该按键为菜单进入键
		{
			xyz.coordinates1=1;//更新界面为一级菜单界面system
			xyz.coordinates2=1;
			xyz.coordinates3=0;
			Cursor_flash_off();
			Shift_Not_Shift=Not_Shift;

		}
		else if( (xyz.coordinates1==0) && (xyz.coordinates2==0) && (xyz.coordinates3==0) && (Shift_Not_Shift == Not_Shift) )
		{

			//当不复用该按键时，该按键为电压设定功能按键
			Keys_Encoder_Mode = Encoder_Mode;   //默认用编码器输入模式
			Interface_coordinates last_xyz=xyz;//上一界面
			xyz.coordinates1=2;
			xyz.coordinates2=2;
			xyz.coordinates3=0;

			if(last_xyz.coordinates1!= xyz.coordinates1||last_xyz.coordinates2!= xyz.coordinates2||last_xyz.coordinates3!= xyz.coordinates3)  Clear_Screen();//更新前后的界面不同，清屏
			Cursor_Position = 0;   //令光标位于最低位
			Cursor_flash_on();

		}


	}
	/*特殊复用功能*/
	/*I set change to v set*/
	if((xyz.coordinates1==2) && (xyz.coordinates2==1) && (xyz.coordinates3==0) && (LOCK_UNLOCK == UNLOCK))
	{
		xyz.coordinates1=2;//更新界面v_set 界面
		xyz.coordinates2=2;
		xyz.coordinates3=0;

		Cursor_Position = 0;   //令光标位于最低位
		__HAL_TIM_ENABLE(&htim6);//开启闪烁

	}
	/*rate*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3!=0) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Cursor_Position==0)
		{
			Cursor_Position=4;
			return;
		}
		if(Cursor_Position==3)
		{
			Cursor_Position=2;
		}
		Cursor_Position--;

	}
	/*timer*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==4) && (xyz.coordinates3!=0) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Cursor_Position==0)
		{
			Cursor_Position=7;
			return;
		}
		if(Cursor_Position==5)
		{
			Cursor_Position=4;
		}

		Cursor_Position--;
	}
	if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3=1) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Cursor_Position==0)
		{
			Cursor_Position=5;
			return;
		}
		if(Cursor_Position==4)
		{
			Cursor_Position=3;
		}

		Cursor_Position--;
	}
	if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3=2) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Cursor_Position==0)
		{
			Cursor_Position=2;
			return;
		}
		Cursor_Position--;
	}
	/*save recall*/
	if((xyz.coordinates1==3) && (xyz.coordinates2!=0) && (LOCK_UNLOCK == UNLOCK))
	{
		if(xyz.coordinates2==2)//recall
		{
			Cursor_flash_off();
		}
		else if(xyz.coordinates2==1)//save
		{
			xyz.coordinates3=1;
			Cursor_Position=0;

			Cursor_flash_on();
		}


	}

	Display_Interface();


}

/**
 * @brief  按键I-set的扫描服务函数
 * @param	None
 * @retval None
 */
void Key_I_set(void)
{
	if (LOCK_UNLOCK == UNLOCK)
	{
		if(Shift_Not_Shift == Shift)
		{

			/*进入设定值展示界面*/
			xyz.coordinates1=9;
			xyz.coordinates2=9;
			xyz.coordinates3=9;
			Shift_Not_Shift=Not_Shift;
			Cursor_flash_off();
		}
		else if( (xyz.coordinates1==0) && (xyz.coordinates2==0) && (xyz.coordinates3==0) && (Shift_Not_Shift == Not_Shift) )//补充一个设定界面坐标
		{
			//当不复用该按键时，该按键为电压设定功能按键
			Keys_Encoder_Mode = Encoder_Mode;   //默认用编码器输入模式
			Interface_coordinates last_xyz=xyz;//上一界面
			xyz.coordinates1=2;
			xyz.coordinates2=1;
			xyz.coordinates3=0;

			if(last_xyz.coordinates1!= xyz.coordinates1||last_xyz.coordinates2!= xyz.coordinates2||last_xyz.coordinates3!= xyz.coordinates3)   Clear_Screen();;//更新前后的界面不同，清屏
			Cursor_Position = 9;   //Cursor reset


			Cursor_flash_on();

		}


	}

	/*特殊复用功能*/
	/*V set change to I set*/
	if((xyz.coordinates1==2) && (xyz.coordinates2==2) && (xyz.coordinates3==0) && (LOCK_UNLOCK == UNLOCK))
	{
		xyz.coordinates1=2;//更新界面v_set 界面
		xyz.coordinates2=1;
		xyz.coordinates3=0;

		Cursor_Position = 9;   //令光标位于最低位
		__HAL_TIM_ENABLE(&htim6);//开启闪烁

	}
	/*rate*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3!=0) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Cursor_Position==4)
		{
			Cursor_Position=0;
			return;
		}
		if(Cursor_Position==1)
		{
			Cursor_Position=2;
		}

		Cursor_Position++;

	}
	/*timer*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==4) && (xyz.coordinates3!=0) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Cursor_Position==12)
		{
			Cursor_Position=5;
			return;
		}
		if(Cursor_Position==8)
		{
			Cursor_Position=10;
			return;

		}

		Cursor_Position++;
	}
	/*AD*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3=1) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Cursor_Position==5)
		{
			Cursor_Position=0;
			return;
		}
		if(Cursor_Position==2)
		{
			Cursor_Position=3;
		}

		Cursor_Position++;
	}
	if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3=2) && (LOCK_UNLOCK == UNLOCK))
	{
		if(Cursor_Position==2)
		{
			Cursor_Position=0;
			return;
		}
		Cursor_Position++;
	}
	/*save recall*/
	if((xyz.coordinates1==3) && (xyz.coordinates2!=0) && (LOCK_UNLOCK == UNLOCK))
	{
		if(xyz.coordinates2==2)//recall
		{
			Cursor_flash_off();
		}
		else if(xyz.coordinates2==1)
		{
			xyz.coordinates3=2;
			Cursor_Position=9;

			Cursor_flash_on();
		}

	}

}


static void enter_second_menu(void)
{
	switch(xyz.coordinates2)
	{
		case 1://system to reset
		case 3://RS232 to braudrate
		case 7://protect
		{
			Cursor_Position=0;
			Cursor_flash_off();
			xyz.coordinates3=1;
			break;
		}

		case 5://ADparam show
		case 2://slope to rate
		case 4://timer to delay cycle
		case 6://hard param
		{
			Cursor_Position=6;
			Cursor_flash_on();
			xyz.coordinates3=1;
			break;
		}
		case 8://calibration
		{
			Cursor_flash_on();
			Cursor_Position=9;
			xyz.coordinates3=1;
			break;
		}

	}


}

static void ISET_enter(void)
{
	if(Keys_Encoder_Mode==Encoder_Mode)
	{
		//Set_Current = String_To_Float(String_Current)*100;
		Set_Current=String_to_float(String_Current)*100;
		if(Set_Current > 5000) Set_Current = 5000;//50A
		if(Set_Current<=10)   Set_Current=10;
		AT25_Save_VISet();



		if((OUTPUT_NOT_OUTPUT == OUTPUT) && (Delay_Function_On_Off==Delay_Function_Off))//处于非延时输出
		{

			DAC_Cmd_send(1,5,Set_Current);
		}

		Cursor_Position=9;
		Keys_Encoder_Mode=Encoder_Mode;
		Cursor_flash_on();

	}
	else if(Keys_Encoder_Mode==Keys_Mode)
	{
		Keys_Encoder_Mode = Encoder_Mode;//恢复为编码器输入模式

		//Set_Current = String_To_Float(String_Current)*100;
		Set_Current=String_to_float(String_Current)*100;
		if(Set_Current > 5000) Set_Current = 5000;//50A
		if(Set_Current<=100)   Set_Current=100;
		AT25_Save_VISet();

		if((OUTPUT_NOT_OUTPUT == OUTPUT) && (Delay_Function_On_Off==Delay_Function_Off))//处于非延时输出模式
		{
			DAC_Cmd_send(1,4,Set_Voltage);
			HAL_Delay(5);
			DAC_Cmd_send(1,5,Set_Current);
		}

		Cursor_Position=9;
		Keys_Encoder_Mode=Encoder_Mode;
		Cursor_flash_on();
	}

}

static void VSET_enter(void)
{
	if(Keys_Encoder_Mode==Encoder_Mode)
	{

		Set_Voltage = String_to_float(&String_Voltage[0])*100;
		if(Set_Voltage > 15000)
		{
			Set_Voltage = 15000;  //限制电压最大值为150V
		}
		if(Set_Voltage<=10)
		{
			Set_Voltage=10;
		}


		AT25_Save_VISet();
		if((OUTPUT_NOT_OUTPUT == OUTPUT) && (Delay_Function_On_Off==Delay_Function_Off) )//非延时输出
		{
			DAC_Cmd_send(1,4,Set_Voltage);
		}

		xyz.coordinates2=1;//change to I set
		Cursor_Position=9;//光标更新

	}
	else if(Keys_Encoder_Mode==Keys_Mode)
	{
		Keys_Encoder_Mode = Encoder_Mode;   //恢复为编码器输入模式


		Set_Voltage = String_to_float(&String_Voltage[0])*100;
		if(Set_Voltage > 15000) Set_Voltage = 15000;  //限制电压最大值为150V

		AT25_Save_VISet();
		if((OUTPUT_NOT_OUTPUT == OUTPUT) && (Delay_Function_On_Off==Delay_Function_Off) )
		{
			DAC_Cmd_send(1,4,Set_Voltage);
		}

		xyz.coordinates2=1;//change to I set
		Cursor_Position=9;//光标更新
		Keys_Encoder_Mode= Encoder_Mode;
		Cursor_flash_on();

	}
}

static void Password_enter(void)
{
	if(strcmp(String_Password,Password)==0)//密码正确
	{
		Third_Menu_Flag=1;
		xyz.coordinates3=2;//更新为显示隐藏界面
		Cursor_Position=10;//光标复位
		Cursor_flash_off();

		for(uint8_t i=0;i<6;i++)
		{
			String_Password_mask[i]=' ';
			String_Password[i]=' ';
		}
	}
	else
	{
		Third_Menu_Flag=1;
		xyz.coordinates3=1;
		Cursor_Position=10;//光标复位

		for(uint8_t i=0;i<6;i++)
		{
			String_Password_mask[i]=' ';
			String_Password[i]=' ';
		}

	}

}

static void IRateSET_enter(void)
{
	/*此处补充指令修改控制板的rate*/
	Set_I_Slope=String_To_Float(&String_I_Rate[0])*10;
	DAC_Cmd_send(1,8,Set_I_Slope);
	AT25_Save_VI_Rate();
	Third_Menu_Flag=1;

	xyz.coordinates3=2;//完成后设定，进入下一设定
	Cursor_Position=6;//光标复位


}

static void VRateSET_enter(void)
{
	/*此处补充指令修改控制板的rate*/
	Set_V_Slope=String_To_Float(&String_V_Rate[0])*10;
	DAC_Cmd_send(1,7,Set_V_Slope);
	AT25_Save_VI_Rate();
	Third_Menu_Flag=1;

	xyz.coordinates3=0;//完成后设定，返回上一级
	Cursor_Position=6;//光标复位
	Cursor_flash_off();
}

static void Save_enter(void)
{
	/*
	Recall_Save_Voltage[Recall_Save_Number]=String_To_Float(String_Save_V);//将交互界面的数值进行转换
	Recall_Save_Current[Recall_Save_Number]=String_To_Float(String_Save_I);
	*/


	/*新的实现*/
	Recall_Save_Voltage[Recall_Save_Number]=String_to_float(String_Save_V);//将交互界面的数值进行转换
	Recall_Save_Current[Recall_Save_Number]=String_to_float(String_Save_I);


	Recall_Save_Power[Recall_Save_Number]=Recall_Save_Voltage[Recall_Save_Number]*Recall_Save_Current[Recall_Save_Number];
	AT25_Save_Recall_Save_VI(Recall_Save_Number);

	if((OUTPUT_NOT_OUTPUT == OUTPUT) && (Delay_Function_On_Off==Delay_Function_Off) )     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
	{
		power_on();
	}
	Cursor_Position=0;
}

static void Load_enter(void)
{

	xyz.coordinates3=1;
	Recall_Save_Voltage[Recall_Save_Number]=AT25_Save_Recall_Recall_VI(Recall_Save_Number)/100.0;
	Set_Voltage=AT25_Save_Recall_Recall_VI(Recall_Save_Number);


	xyz.coordinates3=2;
	Recall_Save_Current[Recall_Save_Number]=AT25_Save_Recall_Recall_VI(Recall_Save_Number)/100.0;
	Set_Current=AT25_Save_Recall_Recall_VI(Recall_Save_Number);


	if((OUTPUT_NOT_OUTPUT == OUTPUT) && (Delay_Function_On_Off==Delay_Function_Off) )     //处于输出模式并且延时时间结束,当改变设定值以后要改变输出电压
	{
		power_on();
	}

	Cursor_Position=0;//光标复位
	Cursor_flash_off();

}

static void Delay_enter(void)
{

	Delay_s=(String_Delay[0] - '0') * 1000 + (String_Delay[1] - '0')*100+ (String_Delay[2] - '0')*10+ (String_Delay[3] - '0');
	Delay_ms=(String_Delay[5] - '0') * 100 + (String_Delay[6] - '0')*10+ (String_Delay[7] - '0');

	Keys_Encoder_Mode = Encoder_Mode;
	if((Delay_s == 0) && (Delay_ms == 0)) Delay_Function_On_Off = Delay_Function_Off;
	else    							  Delay_Function_On_Off = Delay_Function_On;

	AT25_Save_Delay();

	Third_Menu_Flag=1;

	if(xyz.coordinates3==3)	xyz.coordinates3=1;
	else xyz.coordinates3++;//完成后设定，进入下一级

	Cursor_Position=6;//光标复位
	Cursor_flash_on();

}

static void Cycle_enter(void)
{
	//计算Cycle功能的导通和关断时间，加以限制并存储
	Cycle_On_s = (String_Cycle_On[0] - '0') * 1000 + (String_Cycle_On[1] - '0') * 100 + (String_Cycle_On[2] - '0') * 10 + (String_Cycle_On[3] - '0');
	Cycle_On_ms = (String_Cycle_On[5] - '0') * 100 + (String_Cycle_On[6] - '0') * 10 + (String_Cycle_On[7] - '0');

	Cycle_Close_s = (String_Cycle_Close[0] - '0') * 1000 + (String_Cycle_Close[1] - '0') * 100 + (String_Cycle_Close[2] - '0') * 10 + (String_Cycle_Close[3] - '0');
	Cycle_Close_ms = (String_Cycle_Close[5] - '0') * 100 + (String_Cycle_Close[6] - '0') * 10 + (String_Cycle_Close[7] - '0');
	//最短也是5Hz的波
	if ((Cycle_On_s == 0) && (Cycle_On_ms > 0))
	{
		Cycle_On_s = 1;
		Cycle_On_ms = 0;
	}
	if (Cycle_On_s > 9999) Cycle_On_s = 9999;
	if (Cycle_On_ms > 1000) Cycle_On_ms = 1000;
	if ((Cycle_Close_s == 0) && (Cycle_Close_ms > 0))
	{
		Cycle_Close_s = 1;
		Cycle_Close_ms = 0;
	}
	if (Cycle_Close_s > 9999) Cycle_Close_s = 9999;
	if (Cycle_Close_ms > 1000) Cycle_On_ms = 1000;
	//判断是否为Cycle功能输出模式
	if(((Cycle_On_s == 0) && (Cycle_On_ms == 0)) || ((Cycle_Close_s == 0) && (Cycle_Close_ms == 0)))
	{
		Cycle_Function_On_Off = Cycle_Function_Off;
	}
	else if( (Cycle_On_s != 0 || Cycle_On_ms != 0) && (Cycle_Close_s != 0 || Cycle_Close_ms != 0) )
	{
		Cycle_Function_On_Off = Cycle_Function_On;

	}
	AT25_Save_Cycle();


	if(OUTPUT_NOT_OUTPUT == OUTPUT)
	{
		/*先输出零，延时一段时间后开始循环输出*/
		Output_Zero();

	}

	if(xyz.coordinates3==3)	xyz.coordinates3=1;
	else xyz.coordinates3++;//完成后设定，进入下一级

	Cursor_Position=6;//光标复位
	Cursor_flash_on();
}

static void Reset_enter(void)
{

	AT25_Reset();
	Cursor_flash_off();

	switch(xyz.coordinates3)
	{
		case 1:
		{
			DAC_Cmd_send(1,12,0);
			break;
		}
		case 2:
		{
			NVIC_SystemReset();
			break;
		}
		case 3:
		{
			DAC_Cmd_send(1,12,0);
			HAL_Delay(10);
			NVIC_SystemReset();
			break;
		}

	}


}

static void Frq_enter(void)
{
	;
}

static void Sample_show_enter(void)
{

	static uint8_t count=1;
	if(count==1)
	{
		count=2;
		DAC_Cmd_send(1,2,0x00);
	}
	else if(count==2)
	{
		count=1;
		DAC_Cmd_send(1,2,0x01);
	}
	Shift_Not_Shift=Not_Shift;
}

/**
 * @brief  按键Enter的扫描服务函数
 * @param	None
 * @retval None
 */
void Key_Enter(void)
{
	if(LOCK_UNLOCK == UNLOCK)
	{
		if((xyz.coordinates1==0) && (xyz.coordinates2==0) && (xyz.coordinates3==0))
		{
			DAC_Cmd_send(1,2,0x01);
			HAL_Delay(10);
		    __HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);    //关了空闲中断
		    __HAL_UART_CLEAR_IDLEFLAG(&huart2);				//清除IDLE标志
		    __HAL_UART_DISABLE_IT(&huart2,UART_IT_IDLE);	//清除IDLE标志
		    __HAL_UART_ENABLE_IT(&huart2,UART_IT_IDLE);    	//使能空闲中断
		    UART_Start_Receive_DMA(&huart2,Uart2_Receive_buffer,9);
		}
		/*first menu enter second menu*/
		if( xyz.coordinates1==1 && (xyz.coordinates3==0) )//当处于第一级菜单,按下enter进入二级菜单
		{
			enter_second_menu();
			return;
		}
		/*I——SET enter*/
		if( (xyz.coordinates1==2) && (xyz.coordinates2==1) && (xyz.coordinates3==0) )//I_set 按下enter
		{
			ISET_enter();
		}
		/*V——SET enter*/
		if( (xyz.coordinates1==2) && (xyz.coordinates2==2) && (xyz.coordinates3==0) )//V_set 按下enter
		{
			VSET_enter();
		}
		/*AD GAIN FRQ enter*/
		if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3==1))//GAIN
		{
			Password_enter();
		}
		/*rate set*/
		if((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==1) )//i rate
		{
			IRateSET_enter();
		}
		if((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==2) )//v rate
		{
			VRateSET_enter();
		}
		/*CYCLE DELAY enter*/
		if((xyz.coordinates1==1) && (xyz.coordinates2==4) && (xyz.coordinates3==1) )//delay third menu enter
		{
			Delay_enter();
		}
		if((xyz.coordinates1==1) && (xyz.coordinates2==4) && ((xyz.coordinates3==2) || (xyz.coordinates3==3)))//cycle third menu enter
		{
			Cycle_enter();
		}
		/*Save  enter*/
		if((xyz.coordinates1==3) && (xyz.coordinates2==1) && (xyz.coordinates3!=0))
		{
			Save_enter();
		}
		/*load enter*/
		if((xyz.coordinates1==3) && (xyz.coordinates2==2) && (xyz.coordinates3!=0))
		{
			Load_enter();
		}
		/*reset enter*/
		if((xyz.coordinates1==1) && (xyz.coordinates2==1) && (xyz.coordinates3>=1))
		{
			Reset_enter();
		}
		/*light level ennter*/
		if((xyz.coordinates1==1) && (xyz.coordinates2==6) && (xyz.coordinates3==1))
		{
			Cursor_flash_off();
		}

		if(Shift_Not_Shift==Shift && (xyz.coordinates1==0 && xyz.coordinates2==0 && xyz.coordinates3==0))
		{
			Sample_show_enter();
		}
		/*gain frq*/
		if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3==2))
		{
			//Frq_enter();

		}
		if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3==3))
		{
			/*
			ADC_Gain_V = String_To_Float(String_AD_Gain);//完成参数修改
			AT25_Save_AD_Param();
			Third_Menu_Flag=1;
			xyz.coordinates1=0;
			xyz.coordinates2=0;
			xyz.coordinates3=0;
			Cursor_Position=6;//光标复位
			Cursor_flash_off();
			*/
		}
		/*V calibration*/
		//此处采用两点法校准
		if( (xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3==1))
		{
			data_V[0]=Uart2_Receive_buffer[3]*256+Uart2_Receive_buffer[4];
			Vref[0]=String_To_Float(String_Calibration_Vlotage);
			xyz.coordinates3++;
			Cursor_Position=9;
			return;
		}
		if( (xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3==2))
		{
			data_V[1]=Uart2_Receive_buffer[3]*256+Uart2_Receive_buffer[4];
			Vref[1]=String_To_Float(String_Calibration_Vlotage);
			ADC_Gain_V=(Vref[1]-Vref[0])/(data_V[1]-data_V[0]);
			Eror_ADC_V=Vref[0]-ADC_Gain_V*data_V[0];

			/*电流校准要进行后续修改*/
			ADC_Gain_I=ADC_Gain_V;
			Eror_ADC_I=Eror_ADC_V;

			AT25_Save_AD_Param();
			xyz.coordinates3++;
			Cursor_Position=9;
			return;
		}
		/*I calibration*/

		if( (xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3==3))
		{
			Flag.Current_error=1;

		}
		if( (xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3==4))
		{
			data_I[0]=Uart2_Receive_buffer[5]*256+Uart2_Receive_buffer[6];
			Iref[0]=String_To_Float(String_Calibration_Current);
			xyz.coordinates3++;
			return;
		}
		if( (xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3==5))
		{
			data_I[1]=Uart2_Receive_buffer[5]*256+Uart2_Receive_buffer[6];
			Iref[1]=String_To_Float(String_Calibration_Current);
			ADC_Gain_I=(Iref[1]-Iref[0])/(data_I[1]-data_I[0]);
			AT25_Save_AD_Param();
			Eror_ADC_I=Iref[0]-ADC_Gain_I*data_I[0];
			return;
		}

	}


}

/**
 * @brief  按键Shift的扫描服务函数
 * @param	None
 * @retval None
 */
void Key_Shift(void)
{
	if( ((xyz.coordinates1==0) && (xyz.coordinates2==0) && (xyz.coordinates3==0)) || ((xyz.coordinates1==9) && (xyz.coordinates2==9) && (xyz.coordinates3==9)) )
	{
		if(Shift_Not_Shift == Not_Shift)          Shift_Not_Shift = Shift;//如果没复用，就复用
		else if(Shift_Not_Shift == Shift)         Shift_Not_Shift = Not_Shift; //如果复用了，就取消复用
	}

	if( (xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3==1) )
	{
		if(Shift_Not_Shift == Not_Shift)          Shift_Not_Shift = Shift;//如果没复用，就复用
		else if(Shift_Not_Shift == Shift)         Shift_Not_Shift = Not_Shift; //如果复用了，就取消复用
	}

	if(xyz.coordinates1==2 && xyz.coordinates2!=0 && xyz.coordinates3==0)
	{
		/*vset*/
		if(xyz.coordinates2==2)
		{
			if(Keys_Encoder_Mode==Encoder_Mode)
			{
				Cursor_Position=0;
				Cursor_flash_on();

				String_Voltage[0]=' ';
				String_Voltage[1]=' ';
				String_Voltage[2]=' ';
				String_Voltage[3]=' ';
				String_Voltage[4]=' ';
				String_Voltage[5]=' ';
				String_Voltage[6]='V';

			}
			else if(Keys_Encoder_Mode==Keys_Mode)
			{
				Cursor_flash_off();
				Cursor_Position=0;
				String_Voltage[0]=' ';
				String_Voltage[1]=' ';
				String_Voltage[2]=' ';
				String_Voltage[3]=' ';
				String_Voltage[4]=' ';
				String_Voltage[5]=' ';
				String_Voltage[6]='V';

			}

		}
		/*Iset*/
		else if(xyz.coordinates2==1)
		{
			if(Keys_Encoder_Mode==Encoder_Mode)
			{
				Cursor_Position=9;
				Cursor_flash_on();
				String_Current[0]=' ';
				String_Current[1]=' ';
				String_Current[2]=' ';
				String_Current[3]=' ';
				String_Current[4]=' ';
				String_Current[5]=' ';
				String_Current[6]='A';

			}
			else if(Keys_Encoder_Mode==Keys_Mode)
			{
				Cursor_flash_off();
				Cursor_Position=9;
				String_Current[0]=' ';
				String_Current[1]=' ';
				String_Current[2]=' ';
				String_Current[3]=' ';
				String_Current[4]=' ';
				String_Current[5]=' ';
				String_Current[6]='A';
			}
		}

	}


}

/**
 * @brief  按键ON/OFF的扫描服务函数
 * @param  None
 * @retval None
 */
void Key_ON_OFF(void)
{

	/* actual interface */
	if((xyz.coordinates1==0)&&(xyz.coordinates2==0)&&(xyz.coordinates3==0))
	{
		if(OUTPUT_NOT_OUTPUT ==NOT_OUTPUT)
		{
			OUTPUT_NOT_OUTPUT =OUTPUT;
			if(Delay_Function_On_Off==Delay_Function_On)
			{
				TIM16_DELAY_ON();
				return;
			}

			power_on();

		}
		else if(OUTPUT_NOT_OUTPUT ==OUTPUT)
		{
			OUTPUT_NOT_OUTPUT =NOT_OUTPUT;
			power_off();
		}

	}


}

/**
 * @brief  按键Preset/Lock的扫描服务函数
 * @param	None
 * @retval None
 */
void Key_Preset_Lock(void)
{
	if(Shift_Not_Shift == Not_Shift)//当不复用该按键时，该按键为切换功能，切换实时值和设定值界面
	{
		Cursor_flash_off();
		xyz.coordinates1=9;
		xyz.coordinates2=9;
		xyz.coordinates3=9;

	}
	else if(Shift_Not_Shift == Shift)
	{
		if(LOCK_UNLOCK == UNLOCK)
		{
			LOCK_UNLOCK = LOCK;
			Shift_Not_Shift =Not_Shift;
			xyz.coordinates1=0;
			xyz.coordinates2=0;
			xyz.coordinates3=0;

		}
		else if(LOCK_UNLOCK == LOCK)
		{
			LOCK_UNLOCK = UNLOCK;
		}

	}
}


/**
 * @brief  按键Recall/Save的扫描服务函数
 * @param	None
 * @retval None
 */
void Key_Recall_Save(void)
{

	if( ((xyz.coordinates1==0) && (xyz.coordinates2==0) && (xyz.coordinates3==0) && (LOCK_UNLOCK==UNLOCK))
	 || ((xyz.coordinates1==9) && (xyz.coordinates2==9) && (xyz.coordinates3==9) && (LOCK_UNLOCK==UNLOCK)) )
	{
		if(Shift_Not_Shift == Not_Shift)          //无复用，处于取界面
		{
			Recall_Save_Number = 0;//组别
			xyz.coordinates1=3;
			xyz.coordinates2=2;
			xyz.coordinates3=1;//无用
			__HAL_TIM_DISABLE(&htim6);//cursor flash off

		}

		else if(Shift_Not_Shift == Shift)//save
		{
			Recall_Save_Number = 0;//组别
			xyz.coordinates1=3;
			xyz.coordinates2=1;
			xyz.coordinates3=1;
			Shift_Not_Shift = Not_Shift;
			__HAL_TIM_ENABLE(&htim6);//cursor flash off
		}

	}


}


/**
 * @brief  按键Up的扫描服务函数(Cursor_Position move)
 * @param	None
 * @retval None
 */
void Key_Up(void)
{
	/*test*/
	if((xyz.coordinates1==0) && (xyz.coordinates2==0) && (xyz.coordinates3==0))
	{

		;

	}
	/*first menu*/
	if( xyz.coordinates1==1 && (xyz.coordinates3==0) )
	{
		if(xyz.coordinates2==8) xyz.coordinates2=1;
		else 					xyz.coordinates2++;

	}
	/*V——SET*/
	if((xyz.coordinates1==2) && (xyz.coordinates2==2) && (xyz.coordinates3==0))
	{
		Cursor_Position++;
		if(Cursor_Position==3)	Cursor_Position=4;
		if(Cursor_Position==6)	Cursor_Position=0;
	}
	/*I SET*/
	if((xyz.coordinates1==2) && (xyz.coordinates2==1) && (xyz.coordinates3==0))
	{
		Cursor_Position++;
		if(Cursor_Position==3+9)	Cursor_Position=4+9;
		if(Cursor_Position==6+9)	Cursor_Position=0+9;
	}
	/*reset*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==1) && (xyz.coordinates3!=0))
	{
		if(xyz.coordinates3==3)	xyz.coordinates3=1;
		else 					xyz.coordinates3++;
	}
	/*AD set*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3>1))
	{
		if(xyz.coordinates3==3)	xyz.coordinates3=2;
		else 					xyz.coordinates3++;
	}

	/*I rate V rate */
	if( ((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==1)) || ((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==2)) )
	{
		Cursor_Position=6;
		if(xyz.coordinates3==2)	xyz.coordinates3=1;
		else 					xyz.coordinates3++;
	}
	/*Save load*/
	if(((xyz.coordinates1==3) && (xyz.coordinates2==1) && (xyz.coordinates3!=0)) || ((xyz.coordinates1==3) && (xyz.coordinates2==2) && (xyz.coordinates3!=0)))
	{
		Cursor_Position=0;
		String_Save_V[0]='0';
		String_Save_V[1]='0';
		String_Save_V[2]='0';
		String_Save_V[3]='.';
		String_Save_V[4]='0';
		String_Save_V[5]='0';

		String_Save_I[0]='0';
		String_Save_I[1]='0';
		String_Save_I[2]='0';
		String_Save_I[3]='.';
		String_Save_I[4]='0';
		String_Save_I[5]='0';

		if(Recall_Save_Number==9) Recall_Save_Number=0;
		else 					  Recall_Save_Number++;
	}
	/*cycle_on cycle_off  delay*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==4) && (xyz.coordinates3!=0))//cycle third menu enter
	{
		if(xyz.coordinates3==3) xyz.coordinates3=1;
		else                    xyz.coordinates3++;

	}
	/*RS232*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==3) && (xyz.coordinates3==1))
	{
		;//之后完善
	}
	/*light temperature*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==6) && (xyz.coordinates3!=0))//cycle third menu enter
	{
		if(xyz.coordinates3==3) xyz.coordinates3=1;
		else                    xyz.coordinates3++;

	}
	/*calibration*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3!=0))//cycle third menu enter
	{
		Cursor_Position=9;
		if(xyz.coordinates3==5) xyz.coordinates3=1;
		else                    xyz.coordinates3++;

	}

}


/**
 * @brief  按键Up的扫描服务函数
 * @param	None
 * @retval None
 */
void Key_Down(void)
{


	/*test*/
	if((xyz.coordinates1==0) && (xyz.coordinates2==0) && (xyz.coordinates3==0))
	{
		//get_voltage_current(READ_CURRENT);
		DAC_Cmd_send(1,0x0a,0x01);//fall

	}
	/*first menu*/
	if( xyz.coordinates1==1 && (xyz.coordinates3==0) )
	{
		if(xyz.coordinates2==1) xyz.coordinates2=8;
		else 					xyz.coordinates2--;

	}
	/*V——SET*/
	if((xyz.coordinates1==2) && (xyz.coordinates2==2) && (xyz.coordinates3==0))
	{
		if(Cursor_Position==0)
		{
			Cursor_Position=5;
			return;
		}
		if(Cursor_Position==4)
		{
			Cursor_Position=3;
		}
		Cursor_Position--;

	}
	/*I SET*/
	if((xyz.coordinates1==2) && (xyz.coordinates2==1) && (xyz.coordinates3==0))
	{
		if(Cursor_Position==0+9)
		{
			Cursor_Position=5+9;
			return;
		}
		if(Cursor_Position==4+9)
		{
			Cursor_Position=3+9;
		}
		Cursor_Position--;
	}

	/*AD set*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3>1))
	{
		if(xyz.coordinates3==2)	xyz.coordinates3=3;
		else 					xyz.coordinates3--;
	}

	/*I rate V rate */
	if( ((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==1)) || ((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==2)) )
	{
		Cursor_Position=6;
		if(xyz.coordinates3==1)	xyz.coordinates3=2;
		else 					xyz.coordinates3--;
	}
	/*Save load*/
	if(((xyz.coordinates1==3) && (xyz.coordinates2==1)) || ((xyz.coordinates1==3) && (xyz.coordinates2==2)))
	{
		Cursor_Position=0;
		String_Save_V[0]='0';
		String_Save_V[1]='0';
		String_Save_V[2]='0';
		String_Save_V[3]='.';
		String_Save_V[4]='0';
		String_Save_V[5]='0';

		String_Save_I[0]='0';
		String_Save_I[1]='0';
		String_Save_I[2]='0';
		String_Save_I[3]='.';
		String_Save_I[4]='0';
		String_Save_I[5]='0';

		if(Recall_Save_Number==0) Recall_Save_Number=9;
		else 					   Recall_Save_Number--;
	}
	/*cycle_on cycle_off  delay*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==4) && (xyz.coordinates3!=0))//cycle third menu enter
	{
		if(xyz.coordinates3==1) xyz.coordinates3=3;
		else                    xyz.coordinates3--;

	}
	/*RS232*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==3) && (xyz.coordinates3==1))
	{
		;//之后完善
	}
	/*light temperature*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==6) && (xyz.coordinates3!=0))//cycle third menu enter
	{
		if(xyz.coordinates3==1) xyz.coordinates3=3;
		else                    xyz.coordinates3--;

	}
	/*calibration*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==8) && (xyz.coordinates3!=0))//cycle third menu enter
	{
		Cursor_Position=9;
		if(xyz.coordinates3==1) xyz.coordinates3=5;
		else                    xyz.coordinates3--;

	}


}



/**
 * @brief  按键ESC的扫描服务函数
 * @param	None
 * @retval None
 */
void Key_ESC(void)
{
	if((xyz.coordinates1==0) && (xyz.coordinates2==0) && (xyz.coordinates3==0))
	{
		COUNT_ESC++;
		return;
	}
	/*first menu*/
	if( xyz.coordinates1==1 && (xyz.coordinates3==0) )
	{
		xyz.coordinates1=0;
		xyz.coordinates2=0;
		xyz.coordinates3=0;
	}
	/*second menu enter*/
	if( (xyz.coordinates1==1) && (xyz.coordinates2!=0) && (xyz.coordinates3!=0))
	{
		Cursor_Position=0;
		xyz.coordinates3=0;
	}
	/*I——SET V——SET*/
	if( ((xyz.coordinates1==2) && (xyz.coordinates2==1) && (xyz.coordinates3==0)) || ((xyz.coordinates1==2) && (xyz.coordinates2==2) && (xyz.coordinates3==0)) )
	{
		xyz.coordinates1=0;
		xyz.coordinates2=0;
		xyz.coordinates3=0;
	}
	/*AD set*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==5) && (xyz.coordinates3!=0))
	{
		xyz.coordinates3=0;
	}

	/*I rate*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==1))
	{

	}
	/*V rate*/
	if((xyz.coordinates1==1) && (xyz.coordinates2==2) && (xyz.coordinates3==2))
	{
		xyz.coordinates3=0;
	}
	/*Save load*/
	if(((xyz.coordinates1==3) && (xyz.coordinates2==1)) || ((xyz.coordinates1==3) && (xyz.coordinates2==2)))
	{
		xyz.coordinates1=0;
		xyz.coordinates2=0;
		xyz.coordinates3=0;
		Cursor_flash_off();
	}
	/*protect*/
	if( (xyz.coordinates1==4) && (xyz.coordinates2==1) )
	{

		NVIC_SystemReset();

	}
	if( (xyz.coordinates1==9) && (xyz.coordinates2==9) && (xyz.coordinates3==9))
	{
		xyz.coordinates1=0;
		xyz.coordinates2=0;
		xyz.coordinates3=0;
		Cursor_flash_off();
	}

	Cursor_flash_off();


}



/**
 * @brief  按键扫描函数
 * @param	None
 * @retval None
 */
void key_scan(void)
{
	if(keys_EN==DISABLE) return;

	static uint8_t Key_Count1=0;
	static uint8_t Key_Count2=0;
	static uint8_t Key_Count3=0;

	static uint16_t IO_Status=0x00;

	static int Press_Status1;
	static int Press_Status2;
	static int Press_Status3;


	/*IO RESET*/
	HAL_GPIO_WritePin(KeyRow_Port, KeyRow_1_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(KeyRow_Port, KeyRow_2_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(KeyRow_Port, KeyRow_3_Pin,GPIO_PIN_SET);

	/*Scan the firest row*/
	HAL_GPIO_WritePin(KeyRow_Port, KeyRow_1_Pin,GPIO_PIN_RESET);
	for(uint8_t i=0;i<100;i++)
		;
	IO_Status=(GPIOD->IDR&0x007f);
	if(IO_Status != 0x7f)
	{
		Press_Status1=1;//第一行按键按下
		if(++Key_Count1>5)
		{
			Row_Flag=1;
			Key_Count1=5;
			Key_Value1 = IO_Status;	//记下键值
		}

	}
	else Press_Status1=0;
	/*scan the second row*/
	HAL_GPIO_WritePin(KeyRow_Port, KeyRow_1_Pin,GPIO_PIN_SET);//第一行恢复
	HAL_GPIO_WritePin(KeyRow_Port, KeyRow_2_Pin,GPIO_PIN_RESET);//拉低第二行
	for(uint8_t i=0;i<100;i++)
		;
	IO_Status=(GPIOD->IDR&0x007f);
	if(IO_Status != 0x7f)
	{
		Press_Status2=1;//第二行按键按下
		if(++Key_Count2>5)
		{
			Row_Flag=2;
			Key_Count2=5;
			Key_Value2 = IO_Status;	//记下键值
		}
	}
	else Press_Status2=0;
	/*scan the third row */
	HAL_GPIO_WritePin(KeyRow_Port, KeyRow_2_Pin,GPIO_PIN_SET);//第二行恢复
	HAL_GPIO_WritePin(KeyRow_Port, KeyRow_3_Pin,GPIO_PIN_RESET);//第三行拉低
	for(uint8_t i=0;i<100;i++)
		;
	IO_Status=(GPIOD->IDR&0x007f);
	if(IO_Status != 0x7f)
	{
		Press_Status3=1;//第三行按键按下
		if(++Key_Count3>5)
		{
			Row_Flag=3;
			Key_Count3=5;
			Key_Value3 = IO_Status;	//记下键值
		}
	}
	else Press_Status3=0;


	if(Press_Status1==0 && Press_Status2==0 && Press_Status3==0)//无按键按下
	{
		switch(Row_Flag)
		{
			case 1:
			{
					Key_Count1--;
					if(Key_Count1<=0)
					{
						Key_Count1=0;
						Key_Enable=ENABLE;
					}
					break;
			}
			case 2:
			{
					Key_Count2--;
					if(Key_Count2<=0)
					{
						Key_Count2=0;
						Key_Enable=ENABLE;
					}
					break;

			}
			case 3:
			{
					Key_Count3--;
					if(Key_Count3<=0)
					{
						Key_Count3=0;
						Key_Enable=ENABLE;
					}
					break;

			}

			default :break;

		}


	}



}

/**
 * @brief  按键处理函数（先判断行列，再调用按键函数）
 * @param	None
 * @retval None
 */
void KEY_Handle(void)
{
	if(Key_Enable==ENABLE)
	{

		switch(Row_Flag)
			{
				case 1:
				{
					Row_Flag=0;
					switch(Key_Value1)
					{
						case 0X7e:
						{
							key_num_handle(1);
							break;
						}
						case 0x7d:
						{
							key_num_handle(2);
							break;
						}
						case 0x7b:
						{
							key_num_handle(3);
							break;
						}
						case 0x77:
						{
							Key_ESC();
							break;
						}
						case 0x6f:
						{
							Key_V_set();
							break;
						}
						case 0x5f:
						{
							Key_I_set();
							break;
						}
						case 0x3f:
						{
							Key_Up();
							break;
						}
					}
					break;

				}
				case 2:
				{
					Row_Flag=0;
					switch(Key_Value2)
					{
						case 0X7e:
						{
							key_num_handle(4);
							break;
						}
						case 0x7d:
						{
							key_num_handle(5);
							break;
						}
						case 0x7b:
						{
							key_num_handle(6);
							break;
						}
						case 0x77:
						{
							key_num_handle(0);
							break;
						}
						case 0x6f:
						{
							Key_Recall_Save();
							break;
						}
						case 0x5f:
						{
							Key_Preset_Lock();
							break;
						}
						case 0x3f:
						{
							Key_Enter();
							break;
						}
					}
					break;

				}
				case 3:
				{
					Row_Flag=0;
					switch(Key_Value3)
					{
						case 0X7e:
						{
							key_num_handle(7);
							break;
						}
						case 0x7d:
						{
							key_num_handle(8);
							break;
						}
						case 0x7b:
						{
							key_num_handle(9);
							break;
						}
						case 0x77:
						{
							key_num_handle(10);
							break;
						}
						case 0x6f:
						{
							Key_Shift();
							break;
						}
						case 0x5f:
						{
							Key_ON_OFF();
							break;
						}
						case 0x3f:
						{
							Key_Down();
							break;
						}
					}
					break;

				}


			}
		Key_Enable=DISABLE;
		Row_Flag=0;
		Key_Value1=0x7f;
		Key_Value2=0x7f;
		Key_Value3=0x7f;

	}

}



