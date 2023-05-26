#ifndef _RS232_H_
#define _RS232_H_
#include "usart.h"
#include "at25.h"
#include "time_delay_cycle.h"
#include "timeoled.h"
#include "DAC.h"

extern uint8_t USART3_Num;//USART接收字节数
extern uint8_t USART3_Send_Data[20];  //发送数据数组
extern uint8_t USART3_Receive_Data[20]; //接收数据数组
extern uint8_t USART3_Receive_Data_buffer[2];
extern uint8_t USART3_Need_To_Send_Num;//需发送数据字节数
extern uint16_t CrcValue;//校验码
extern uint16_t AddValue;

uint16_t CRC16(uint8_t *puchMsg,uint8_t usDataLen );
void USART3_Send(void);
void uint16_t_to_uint8_t(uint16_t x,uint8_t *t);
void USART3_Handler(void);
void RS232_IT(void);

#endif
