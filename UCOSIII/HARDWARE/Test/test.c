//#include "brake.h"		
//#include "usart2_dma.h"	
//#include "crc_16.h"
#include "gpio.h"
#include "test.h"
#include "pwm.h"
//#include "delay.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//测试专用
//HWY 2018 1 2
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 
//新版制动器控制程序
void Brake_Init(void)
{
	 TIM5CH2_PWM_Init(21000-1,4-1);	//1KHz.  
   Gpio_Init();
	 Brake_Break();
}

void Brake_Break(void)
{
	A3=0; 
	A4=1;
	TIM_SetCompare2(TIM5,0);
}

void Brake_Positive(u16 comp)
{
	A3=1;
	A4=0;
	TIM_SetCompare2(TIM5,210*(100-comp));
}