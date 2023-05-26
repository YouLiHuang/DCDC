#ifndef _OLED_H
#define _OLED_H
/*--------------------------------------------------标志位宏定义------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "spi.h"

#include "protect.h"
#include "myadc.h"
#include "PowerSupplyconfig.h"
#include "fonts.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*SPI接口定义-开头****************************/
//定义位选引脚的高低电平函数
#define  		SPI_OLED_CS_LOW()     				HAL_GPIO_WritePin(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN,GPIO_PIN_RESET)
#define  		SPI_OLED_CS_HIGH()    				HAL_GPIO_WritePin(OLED_SPI_CS_PORT, OLED_SPI_CS_PIN,GPIO_PIN_SET)

//定义命令/数据引脚的高低电平函数
#define     	SPI_OLED_DC_LOW()                   HAL_GPIO_WritePin(OLED_SPI_DC_PORT, OLED_SPI_DC_PIN,GPIO_PIN_RESET)
#define     	SPI_OLED_DC_HIGH()                  HAL_GPIO_WritePin(OLED_SPI_DC_PORT, OLED_SPI_DC_PIN,GPIO_PIN_SET)
//定义命令/数据引脚的高低电平函数
#define 		res_OLED 							HAL_GPIO_WritePin(OLED_SPI_RES_PORT, OLED_SPI_RES_PIN,GPIO_PIN_RESET)
#define 		set_OLED 							HAL_GPIO_WritePin(OLED_SPI_RES_PORT, OLED_SPI_RES_PIN,GPIO_PIN_SET)

//收发成功失败标志位
#define receive_transmit_success 1
#define receive_transmit_fail    2

//宏定义Shift复用标志位
#define Shift                   1
#define Not_Shift               0

//宏定义锁定/解锁状态
#define LOCK       1
#define UNLOCK     0

//宏定义设定/实际值状态
#define Preset     1
#define Actual     0
//宏定义实时输出状态
#define Output     2

//宏定义CV/CC/CP/DELAY/NOT DELAY状态
#define CV         0X01
#define CC         0X02
#define CP         0X03

//宏定义CYCLE/DELAY状态
#define CYCLE      0X0F
#define NOT_CYCLE  0XF0
#define DELAY      0X00
#define NOT_DELAY  0XFF



//宏定义输出显示通断状态
#define ON         1
#define OFF        0

//宏定义输出通断状态
#define OUTPUT            1
#define NOT_OUTPUT        0

//宏定义存取状态
#define Save       1
#define Recall     0

//宏定义保护菜单
#define PF              0x00        //proper functioning    正常工作
#define OVP             0x01		//过压
#define OCP             0x02		//过流
#define OTP             0x04  		//过温
#define CPP				0x08		//过功
#define Sense_Error		0x20


/*命令定义-开头*******************************/

#define Enable_Gray_Scale_table                   0x00    //这个命令被发送来启用灰度表设置。(命令B8h)
#define Set_Column_Address_Commond                0x15    //设置列开始和结束地址
#define Write_RAM_Command                         0x5c    //使单片机能够将数据写入内存中
#define Read_RAM_Command                          0x5d    //让单片机读取内存中的数据
#define Set_Row_Address_Commond                   0x75    //设置行开始和结束地址
#define Set_Re_map_and_Dual_COM_Line_mode         0xa0    //设置重映射和双COM线模式
#define Set_Display_Start_Line                    0xa1    //在0-127之间设置显示启动线寄存器
#define Set_Display_Offset                        0xa2    //从0-127设置垂直滚动
#define Entire_Display_OFF                        0xa4    //所有像素都关闭在GS级0
#define Entire_Display_ON                         0xa5    //所有像素都在GS级别15中打开
#define Normal_Display                            0xa6    //正常显示(重置)
#define Inverse_Display                           0xa7    //逆显示
#define Enable_Partial_Display                    0xa8    //这个命令打开了部分模式
#define Exit_Partial_Display                      0xa9    //该命令被发送到退出部分显示模式
#define Function_Selection                        0xab    //选择外部VDD/启用内部VDD调整器[reset]
#define Sleep_mode_ON                             0xae    //设置睡眠模式ON
#define Sleep_mode_OFF                            0xaf    //设置睡眠模式OFF
#define Set_Phase_Length                          0xb1    //设置相位长度
#define Set_Front_Clock_Divider                   0xb3    //设置前时钟分频器/振荡器频率
#define Set_GPIO                                  0xb5    //设置GPIO
#define Set_Second_Precharge_Period               0xb6    //设置第二预充期间
#define Set_Gray_Scale_Table                      0xb8    //设置灰度表
#define Select_Default_Linear_Gray_Scale_table    0xb9    //选择默认线性灰度表
#define Set_Pre_charge_voltage                    0xbb    //设置预充电电压
#define Set_VCOMH                                 0xbe    //设置COM取消选择电压等级[reset = 04h]
#define Set_Contrast_Current                      0xc1    //设置对比度电流
#define Master_Contrast_Current_Control           0xc7    //主对比度电流控制
#define Set_MUX_Ratio                             0xca    //设置多路选择开关比值
#define Set_Command_Lock                          0xfd    //设置命令锁
#define Display_Enhancement_A					  0xB4	  //显示增强A
#define Display_Enhancement_B					  0xD1    //显示增强B
/*命令定义-结尾*******************************/

/*命令定义-补充Front Clock Divider*/
#define divide_1 0x50
#define divide_2 0x51
#define divide_4 0x52
#define divide_8 0x53
#define divide_16 0x54
/*命令定义-补充结束*/


typedef struct
{
	int coordinates1;
	int coordinates2;
	int coordinates3;

}Interface_coordinates;

extern Interface_coordinates xyz;
extern uint8_t Light;

/*----------------------------------------------------------用于显示的字符串-----------------------------------------------------------------*/
/*延时和循环的参数*/
extern char String_Cycle_On[10];                                                   	 //定义循环开的字符串
extern char String_Cycle_Close[10];                                                	 //定义循环关的字符串
extern char String_Delay[10];                                                        	  //定义延时的字符串
/*新版AD/DA相关参数*/
extern char String_I_Rate[10];                                                  //定义String_I_Rate的字符串
extern char String_V_Rate[10];                                                  //定义String_I_Rate的字符串
extern char String_Voltage[7];                       							//定义一个存放转变后的电压字符串的数组,也用于后面设定电压值时的显示
extern char String_Current[7];                       							//定义一个存放转变后的电流字符串的数组,也用于后面设定电流值时的显示
extern char String_AD_Frq[10];
extern char String_AD_Gain[10];
extern char String_Save_V[10];
extern char String_Save_I[10];
extern char String_Recall_V[10];
extern char String_Recall_I[10];
extern char String_RS232_Rate[10];
extern char String_Password[7];
extern char String_Password_mask[7];
extern char String_Calibration_Vlotage[10];
extern char String_Calibration_Current[10];
extern char String_FAN[10];
/*串口*/
extern char String_RS232_ID[5];                          						//RS232 ID的字符串
extern char String_RS232_BaudRate[10];

/*存取时用到的数据*/
extern float Recall_Save_Voltage[10];
extern float Recall_Save_Current[10];
extern float Recall_Save_Power[10];
/*内部参数*/
extern char String_Light_Level[10];
extern char String_Temperature_CH1[10];
extern char String_Temperature_CH2[10];

extern uint16_t Set_Voltage;//设定的电压值
extern uint16_t Set_Current;//设定的电流值
extern uint8_t Set_V_Slope;//设定的缓升/降参数
extern uint8_t Set_I_Slope;
/*标志位*/
extern Interface_coordinates Last_Interface;
extern uint8_t First_Into_ErrorInterface_Flag;

/*---------------------------------------------------------------函数声明----------------------------------------------------------------------*/
extern void SPI_FUN(void);
extern void SPI_OLED_SendCommond(uint8_t byte);
extern void SPI_OLED_SendData(uint8_t byte);
extern void OLED_Init(void);

extern void Set_Column_Address(uint8_t a, uint8_t b);
extern void Set_Row_Address(uint8_t a, uint8_t b);
extern void Set_WriteData(void);
extern void Set_Remap_Format(unsigned char A);
extern void Fill_RAM(uint8_t Data);
extern void Clear_Screen(void);
extern void Row_Interval(uint8_t Data);
extern void Point_Interval(uint8_t m);
extern void Display_Horizontal_Line(uint8_t Row, uint8_t Start_Column, uint8_t End_Column);
extern void Display_Vertical_Line_1(uint8_t Column, uint8_t Start_Row, uint8_t End_Row);
extern void Display_Vertical_Line_2(uint8_t Column, uint8_t Start_Row, uint8_t End_Row);
extern void Display_Vertical_Line_3(uint8_t Column, uint8_t Start_Row, uint8_t End_Row);
extern void Display_Vertical_Line_4(uint8_t Column, uint8_t Start_Row, uint8_t End_Row);
extern void Display_Rectangular(uint8_t Start_Row, uint8_t Start_Column, uint8_t End_Row, uint8_t End_Column);
extern void Dsiplay_Solid_Rectangular(uint8_t Start_Row, uint8_t Start_Column, uint8_t End_Row, uint8_t End_Column, uint8_t Colour);
extern void Font_1Bit_To_4Bit(uint8_t Data);

extern void Write_Single_8x16AsicII(uint8_t Row, uint8_t Column, char Data);
extern void Write_String_8x16AsicII(uint8_t Row, uint8_t Column, char Data[]);
extern void Write_Single_12x24AsicII(uint8_t Row, uint8_t Column, char Data);
extern void Write_String_12x24AsicII(uint8_t Row, uint8_t Column, char Data[]);
extern void Write_Single_16x16Chinese(uint8_t Row, uint8_t Column, char Data[]);
extern void Write_String_16x16Chinese(uint8_t Row, uint8_t Column, char String_Data[]);
extern void Write_Single_16x32AsicII(uint8_t Row, uint8_t Column, char Data);
extern void Write_String_16x32AsicII(uint8_t Row, uint8_t Column, char Data[]);
/*---------------------------------------------------------显示主界面(真实值界面和设定值查看界面)-----------------------------------------------------*/
extern void Display_Main_Interface_Voltage(float Voltage);
extern void Display_Main_Interface_Current(float Current);
extern void Display_Main_Interface_Power(float Voltage,float Current);
extern void Display_Main_Interface_CV_CC_CP(uint8_t CV_CC_CP);
extern void Display_Main_Interface_CYCLE_DELAY(uint8_t CYCLE_NOT_CYCLE, uint8_t DELAY_NOT_DELAY);
extern void Display_Main_Interface_ON_OFF(uint8_t State);
extern void Display_Main_Interface_V_set(void);
extern void Display_Main_Interface_I_set(void);

extern void Display_Main_Interface( const float Voltage,
									const float Current,
									const uint8_t ON_OFF,
									const uint8_t CV_CC_CP,
									const uint8_t CYCLE_NOT_CYCLE,
									const uint8_t DELAY_NOT_DELAY,
									const uint8_t Preset_Actual,
									const uint8_t LOCK_UNLOCK,
									const uint8_t Shift_NotShift);

/*显示AD隐藏界面*/
extern void  Display_Advanced_Interface_AD_Frq_set(uint16_t);
extern void Display_Advanced_Interface_AD_Gain_set(uint16_t);
extern void Display_Advanced_Interface_AD_Param_set(uint16_t ADC_frequency,float ADC_Gain);
/*显示存取界面*/
extern void Display_Access_Interface(uint8_t Recall_Save, uint8_t Recall_Save_Number);
/*显示一级菜单*/
extern void Display_Menu_Function_Interface_First_Menu(int Selection);
/*显示二级菜单*/
extern void Display_Menu_Function_Interface_Second_Menu(int , int );
/*显示保护界面*/
extern void Display_Protect_Interface(uint8_t Error);
/*save recall*/
extern void Display_Access_Interface_Save(void);
extern void Display_Access_Interface_Recall(void);
/*其他部分*/
extern void Display_RS232_ID(uint16_t RS232_ID);
extern void Display_RS232_BaudRate(int RS232_BaudRate);
extern void Display_Oled_Delay(void);
extern void Display_Oled_CycleOn(void);
extern void Display_Oled_CycleClose(void);
extern void Display_Oled_V_Rate(void);
extern void Display_Oled_I_Rate(void);
extern void Cursor_flash(void);
extern void Cursor_flash_off(void);
extern void Cursor_flash_on(void);
/*综合显示*/
extern void Display_Interface(void);
/*参数界面*/
extern void Display_Param_Interface(const uint16_t Set_Voltage,const uint16_t Set_Current);



#endif
