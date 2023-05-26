#include "filter.h"

float output[100]={0};

float First_Order_Filter(float input)
{
	static float alpha=1/( 1+FRQ/(2*PI*FH) );
	static float last_output=0.0;
	float output=0.0;

	output=last_output+alpha*(input-last_output);

	last_output=output;

	return output;

}

float* Filter(void)
{
	float * pindex=Voltage_Data;

	for(int i=0;i<sizeof(Voltage_Data)/4-1;i++)
	{
		output[i]=First_Order_Filter(*pindex++);
	}
	return output;
}
