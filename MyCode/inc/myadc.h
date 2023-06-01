#ifndef _MYADC_H_
#define _MYADC_H_

#include "dac.h"
#include "spi_oled.h"
#include "string.h"

#define READ_VOLTAGE 0x01
#define READ_CURRENT 0x02
#define Current_Error 2.21

extern uint16_t ADC_frequency;
extern float ADC_Gain_V;
extern float ADC_Gain_I;

extern float Voltage_real;      //实际电压值，用于显示
extern float Current_real;      //实际电流值，用于显示

extern float ADC_Gain;
extern float Eror_ADC_V;
extern float Eror_ADC_I;
extern float Voltage_Data[100];

extern void get_voltage_current(uint8_t mode);


#endif
