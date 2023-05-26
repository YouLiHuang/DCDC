#ifndef _ENCODER_H_
#define _ENCODER_H_

#include "main.h"
#include "tim.h"
#include "PowerSupplyconfig.h"
#include "newkey.h"


#define Stop	2
#define Forward 1
#define Reverse 0



/*data refresh*/
extern void Encoder_Data_refresh(void);
extern void Encoder_Enter_Handle(void);


#endif
