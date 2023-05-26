#ifndef _FILTER_H_
#define _FILTER_H_

#include "myadc.h"

#define PI 3.1415
#define FRQ	100
#define FH	5

extern float First_Order_Filter(float input);
extern float* Filter(void);

#endif
