#ifndef __MY_DAC_H__
#define __MY_DAC_H__




#include "usart.h"
#include "spi_oled.h"

//#define Linear
//#define RF_TIME_MODE
#define ADD_CHECK 0x01
#define RF_RATE_MODE 0x00

#define V_Rise_Time_Minimum 100                                              //最小电压缓升时间
#define I_Rise_Time_Minimum 100                                              //最小电流缓升时间
#define V_Slope_Min 1	//电压最慢的斜率
#define I_Slope_Min 1	//电流最慢的斜率
#define V_Slope_Max 30	//电压最快的斜率
#define I_Slope_Max 10	//电流最快的斜率
#define V_Slope_Default 0x000e			//默认电压斜率
#define I_Slope_Default 0x0500			//默认电流斜率

#define RISE_V	DAC_Cmd_send(1,0x0a,0x01)
#define FALL_V	DAC_Cmd_send(1,0x0a,0x02)
#define RISE_I	DAC_Cmd_send(1,0x0a,0x03)
#define FALL_I	DAC_Cmd_send(1,0x0a,0x04)
#define OUTPUT_ZERO DAC_Cmd_send(1,0x08,0x0000)
#define POWER_ON 	DAC_Cmd_send(1,0x08,0x6400)
#define POWER_OFF	DAC_Cmd_send(1,0x08,0x0000)

/*以下这些为标志位的可选项，定义了缓升缓降的开启关闭标志*/
#define V_Rise_Off          0
#define V_Rise_On           1
#define V_Fall_Off          0
#define V_Fall_On           1
#define I_Rise_Off          0
#define I_Rise_On           1
#define I_Fall_Off          0
#define I_Fall_On           1

extern uint8_t V_Rise_On_Off;
extern uint8_t V_Fall_On_Off;
extern uint8_t I_Rise_On_Off;
extern uint8_t I_Fall_On_Off;

void DAC_Cmd_send(uint8_t IDindex,uint8_t cmd,uint16_t Data_To_Send);
uint16_t ADD(uint8_t *puchMsg,uint8_t usDataLen);
uint16_t CRC16(uint8_t *puchMsg,uint8_t usDataLen );
extern void power_off(void);
extern void power_on(void);
extern void Output_Zero(void);
extern void Slope_Function_On(void);
extern void Slope_Function_Off(void);
extern void Slope_Function_On_default(void);
extern void Slope_Function_Off_default(void);

#endif
