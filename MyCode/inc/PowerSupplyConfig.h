/****************************************************************************/
/*                            软启动引脚配置                                  */
/****************************************************************************/
/*软启动引脚为PC5*/
#define 			Softstart_GPIO_Port					GPIOC
#define 			Softstart_GPIO_PIN					GPIO_PIN_5
#define				Power_AC_On							HAL_GPIO_WritePin(Softstart_GPIO_Port,Softstart_GPIO_PIN,GPIO_PIN_SET)
#define				Power_AC_Off						HAL_GPIO_WritePin(Softstart_GPIO_Port,Softstart_GPIO_PIN,GPIO_PIN_RESET)
/****************************************************************************/
/*                         温控开关引脚配置                                  */
/****************************************************************************/
#define 			OTP_PORT							GPIOB
#define 			OTP1_PIN							GPIO_PIN_11
#define 			OTP2_PIN							GPIO_PIN_12
/****************************************************************************/
/*                     SPI1通讯引脚配置（AT25和OLED共用）                                      */
/****************************************************************************/
#define      	OLED_SPIx                       hspi1
#define      	AT25_SPIx                       hspi1
/*SCK引脚------------------------------------------------>PD8*/
#define      	OLED_SPI_SCK_PORT               GPIOD
#define      	OLED_SPI_SCK_PIN                GPIO_PIN_8
/*MOSI引脚----------------------------------------------->PA12*/
#define      	OLED_SPI_MOSI_PORT              GPIOA
#define      	OLED_SPI_MOSI_PIN               GPIO_PIN_12
/*OLED片选引脚（普通IO）---------------------------------->PC0*/
#define      	OLED_SPI_CS_PORT                GPIOC
#define      	OLED_SPI_CS_PIN                 GPIO_PIN_0
/*DC引脚------------------------------------------------->PC1*/
#define      	OLED_SPI_DC_PORT                GPIOC
#define      	OLED_SPI_DC_PIN                 GPIO_PIN_1
/*RES引脚------------------------------------------------>PC2*/
#define			OLED_SPI_RES_PORT				GPIOC
#define			OLED_SPI_RES_PIN				GPIO_PIN_2

/*AT25片选（CS）引脚（普通IO）---------------------------------->PC3*/
#define 		SPI_AT25_CS_PIN					GPIO_PIN_3
#define 		SPI_AT25_CS_PORT				GPIOC
/*AT25MOSI引脚----------------------------------------------->PA12*/
#define      	OLED_SPI_MOSI_PORT              GPIOA
#define      	OLED_SPI_MOSI_PIN               GPIO_PIN_12
/*SCK引脚------------------------------------------------>PD8*/
#define      	OLED_SPI_SCK_PORT               GPIOD
#define      	OLED_SPI_SCK_PIN                GPIO_PIN_8

/****************************************************************************/
/*                         按键引脚配置                                      */
/****************************************************************************/
//按键宏定义
#define 			KeyRow_Port        							GPIOC
#define 			KeyRow_1_Pin       							GPIO_PIN_13
#define 			KeyRow_2_Pin       							GPIO_PIN_14
#define 			KeyRow_3_Pin       							GPIO_PIN_15

#define 			KeyCol_Port        							GPIOD
#define 			KeyCol_1_Pin       							GPIO_PIN_0
#define 			KeyCol_2_Pin       							GPIO_PIN_1
#define 			KeyCol_3_Pin       							GPIO_PIN_2
#define 			KeyCol_4_Pin       							GPIO_PIN_3
#define 			KeyCol_5_Pin       							GPIO_PIN_4
#define 			KeyCol_6_Pin       							GPIO_PIN_5
#define 			KeyCol_7_Pin       							GPIO_PIN_6

/****************************************************************************/
/*                         指示灯引脚配置                                    */
/****************************************************************************/
#define 			Sys_Led_Port        							GPIOC
#define 			Sys_Led_Pin       								GPIO_PIN_12







