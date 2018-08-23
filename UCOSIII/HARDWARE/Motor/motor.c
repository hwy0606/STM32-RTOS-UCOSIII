#include "gpio.h"
#include "pwm.h"
#include "motor.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//电机驱动
//A1:A2=0:0 电机刹车
//A1:A2=1:0 电机正转
//A1:A2=0:1 电机反转
//占空比作为参数输入，最高占空比为98%
//comp 为输入占空比 0-98
//接线 PWM/Ve-PA2 EN-A1-PC1 DR-A2-PC3
//HWY 2017 10 17
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	



////57步进电机驱动程序
//u16 freq=8; //频率设定
//void Motor_Init(void)
//{    	 
// TIM5_PWM_Init(21000/freq-1,4-1);	//10KHz.  
// Gpio_Init();
//// Motor_Break();
//} 

//void Motor_Break(void)
//{
////	A1=0; //EN 高电平为待机

//	TIM_SetCompare1(TIM5,0);
//}

//void Motor_Positive(u16 comp)
//{
////	A1=0;//方向+
////  A2=0;
//	TIM_SetCompare1(TIM5,(210/freq)*(100-comp));
//}


//void Motor_Negative(u16 comp)
//{
////	A1=1;//方向-
////	A2=1;
//	TIM_SetCompare1(TIM5,(210/freq)*(100-comp));
//}


//直流无刷电机驱动

void Motor_Init(void)
{    	 
 TIM5CH3_PWM_Init(21000-1,4-1);	//1KHz.  
 Gpio_Init();
 Motor_Break();
} 

void Motor_Break(void)
{
	A1=0; //EN 高电平为待机
	A2=1;
	TIM_SetCompare3(TIM5,0);
}

void Motor_Positive(u16 comp)
{
	A1=1;//EN 低电平为正常
	A2=1;//DR 高电平正转
	TIM_SetCompare3(TIM5,210*(100-comp));
}


void Motor_Negative(u16 comp)
{
	A1=1;//EN 低电平为正常
	A2=1;//DR 低电平反转
	TIM_SetCompare3(TIM5,210*(100-comp));
}
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//电机驱动
//电机转速为comp/100*3000   
//Ve端输入1V时 转速为0 Ve端输入4.5V时 转速为100%
//接线 DAC1-PA4 EN-A1-PC1 DR-A2-PC3
//HWY 2018 04 16
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//void Motor_Init(void)
//{    	 
// Dac1_Init(); 
// Gpio_Init();
//} 

//void Motor_Break(void)
//{
//	A1=0; //EN 高电平为待机
//	A2=1;
//	Dac1_Set_Vol(0);
//}

//void Motor_Positive(u16 comp)
//{
//	A1=1;//EN 低电平为正常
//	A2=1;//DR 高电平正转
//	Dac1_Set_Vol(35*comp+1000);
//}


//void Motor_Negative(u16 comp)
//{
//	A1=1;//EN 低电平为正常
//	A2=0;//DR 低电平反转
//	Dac1_Set_Vol(35*comp+1000);
//}
