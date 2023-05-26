#ifndef __AT25_H
#define __AT25_H

#include "spi_oled.h"
#include "timeoled.h"
#include "PowerSupplyConfig.h"
#include "main.h"
#include "spi.h"
#include "newkey.h"
#include "time_delay_cycle.h"



#define SPI_FLASH_PageSize              256
#define SPI_FLASH_PerWritePageSize      256


//AT25芯片指令
/*命令定义-开头*******************************/
#define AT25_WREN		          0x06
#define AT25_WRDI		          0x04
#define AT25_RDSR		          0x05
#define AT25_WRSR		          0x01
#define AT25_Read			      0x03
#define AT25_Write		          0x02
#define AT25_SectorErase          0x52
#define AT25_ChipErase            0x62





extern void SPI_Save_Word_8bits(uint8_t data,uint8_t addr);
extern void SPI_Save_Word_16bits(uint16_t data,uint8_t addr);
extern uint8_t SPI_Load_Word_8bits(uint8_t addr);
extern uint16_t SPI_Load_Word_16bits(uint8_t addr);

extern void SPI_AT25_Write(uint16_t* pBuffer, uint8_t WriteAddr, uint16_t NumByteToWrite);
extern void SPI_AT25_Read(uint16_t* pBuffer, uint8_t ReadAddr, uint16_t NumByteToRead);

/*User Function*/
extern void AT_25_init(void);
extern void AT25_Reset(void);

extern void AT25_Save_VISet(void);
extern void AT25_Save_VI_Rate(void);
extern void AT25_Save_Recall_Save_VI(uint8_t Number);
extern uint16_t AT25_Save_Recall_Recall_VI(uint8_t Number);

extern void AT25_Save_AD_Param(void);

extern void AT25_Save_RS232(void);
extern void AT25_Save_Delay(void);
extern void AT25_Save_Cycle(void);
extern void AT25_Save_CC_CV_CP(void);

extern void AT25_Load_VISet(void);
extern float AT25_Load_V(void);
extern float AT25_Load_I(void);
extern void AT25_load_AD_Param(void);
extern void AT25_Load_Data(void);

#endif /*__AT25_H*/
