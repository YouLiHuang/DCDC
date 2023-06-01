#ifndef _NEWKEY_H_
#define _NEWKEY_H_

#include "main.h"
#include "spi_oled.h"
#include "timeoled.h"
#include "time_delay_cycle.h"
#include "math.h"
#include "at25.h"


//宏定义键盘/编码器输入
#define Keys_Mode               0
#define Encoder_Mode            1


/*-------------------------------------------------------------内部标志位---------------------------------------------------------------------*/
extern FunctionalState keys_EN;												 //按键使能控制标志位，初始化为失能，软起动后使能
extern FunctionalState Inner_Mode;
extern uint8_t Shift_Not_Shift;                                             //定义复用键的标志位（默认不复用）
extern uint8_t ON_OFF;                                                            //定义输出关闭标志位（默认不输出,只是用来显示）
extern uint8_t OUTPUT_NOT_OUTPUT;                                          //定义是否已经输出标志位
extern uint8_t CV_CC_CP;                                                           //定义输出类型标志位 (默认恒压输出)

extern uint8_t LOCK_UNLOCK ;                                                    //定义锁定标志位（默认不锁定）
extern uint8_t Recall_Save ;                                                    //定义读取保存标志位（默认为读取）

extern uint8_t Keys_Encoder_Mode ;                                        //定义用键盘输入还是用编码器输入(默认用编码器输入)
extern uint8_t RS232_BaudRate_State;                                                //定义设定波特率状态
extern uint8_t Recall_Save_Number;                                                  //定义读取保存的数据组（默认第0组）
extern uint8_t Cursor_Position;                                                   	 	 //定义光标位置的全局变量
extern uint8_t Third_Menu_Flag;
extern uint8_t COUNT_ESC;
extern uint8_t Sleep_ON_OFF;
/*-------------------------------------------------------------可设置参数----------------------------------------------------------------------*/
extern uint16_t Cycle_On_s;                                                         //定义循环时间的秒
extern uint16_t Cycle_On_ms;                                                        //定义循环时间的毫秒
extern uint16_t Cycle_Close_s;                                                      //定义循环关闭时间的秒
extern uint16_t Cycle_Close_ms;                                                     //定义循环关闭时间的毫秒



extern uint16_t Delay_s;                                                                  //定义延时的秒
extern uint16_t Delay_ms;                                                                  //定义延时的秒

extern uint8_t Set_V_Slope;//设定的电压变化率
extern uint8_t Set_I_Slope;//设定的电流变化率
extern uint16_t Set_Voltage;  //设定电压值
extern uint16_t Set_Current;  //设定电流值
extern uint16_t Set_Power;

extern float Voltage_Actual;
extern float Current_Actual;


extern uint16_t Key_Value1;
extern uint16_t Key_Value2;
extern uint16_t Key_Value3;
extern int Row_Flag;
extern FunctionalState  Key_Enable;

/*------------------------------------------------------------------函数声明-------------------------------------------------------------------------*/
extern float String_To_Float(char* string);
extern float String_to_float(char* string);
extern void key_num_handle(int key_number);
extern void key_scan(void);
extern void KEY_Handle(void);




#endif
