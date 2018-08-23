#ifndef __ADC_H
#define __ADC_H	


#include "stm32f4xx.h"

void Adc_Init(void);
void Sensor_Current_Init(void);//电压测量初始化
u16 Average(); //取平均值函数


#endif 
