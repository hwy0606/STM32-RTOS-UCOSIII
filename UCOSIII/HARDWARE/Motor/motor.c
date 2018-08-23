#include "gpio.h"
#include "pwm.h"
#include "motor.h"
#include "stm32f4xx.h"
#include "stm32f4xx_tim.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//�������
//A1:A2=0:0 ���ɲ��
//A1:A2=1:0 �����ת
//A1:A2=0:1 �����ת
//ռ�ձ���Ϊ�������룬���ռ�ձ�Ϊ98%
//comp Ϊ����ռ�ձ� 0-98
//���� PWM/Ve-PA2 EN-A1-PC1 DR-A2-PC3
//HWY 2017 10 17
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	



////57���������������
//u16 freq=8; //Ƶ���趨
//void Motor_Init(void)
//{    	 
// TIM5_PWM_Init(21000/freq-1,4-1);	//10KHz.  
// Gpio_Init();
//// Motor_Break();
//} 

//void Motor_Break(void)
//{
////	A1=0; //EN �ߵ�ƽΪ����

//	TIM_SetCompare1(TIM5,0);
//}

//void Motor_Positive(u16 comp)
//{
////	A1=0;//����+
////  A2=0;
//	TIM_SetCompare1(TIM5,(210/freq)*(100-comp));
//}


//void Motor_Negative(u16 comp)
//{
////	A1=1;//����-
////	A2=1;
//	TIM_SetCompare1(TIM5,(210/freq)*(100-comp));
//}


//ֱ����ˢ�������

void Motor_Init(void)
{    	 
 TIM5CH3_PWM_Init(21000-1,4-1);	//1KHz.  
 Gpio_Init();
 Motor_Break();
} 

void Motor_Break(void)
{
	A1=0; //EN �ߵ�ƽΪ����
	A2=1;
	TIM_SetCompare3(TIM5,0);
}

void Motor_Positive(u16 comp)
{
	A1=1;//EN �͵�ƽΪ����
	A2=1;//DR �ߵ�ƽ��ת
	TIM_SetCompare3(TIM5,210*(100-comp));
}


void Motor_Negative(u16 comp)
{
	A1=1;//EN �͵�ƽΪ����
	A2=1;//DR �͵�ƽ��ת
	TIM_SetCompare3(TIM5,210*(100-comp));
}
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//�������
//���ת��Ϊcomp/100*3000   
//Ve������1Vʱ ת��Ϊ0 Ve������4.5Vʱ ת��Ϊ100%
//���� DAC1-PA4 EN-A1-PC1 DR-A2-PC3
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
//	A1=0; //EN �ߵ�ƽΪ����
//	A2=1;
//	Dac1_Set_Vol(0);
//}

//void Motor_Positive(u16 comp)
//{
//	A1=1;//EN �͵�ƽΪ����
//	A2=1;//DR �ߵ�ƽ��ת
//	Dac1_Set_Vol(35*comp+1000);
//}


//void Motor_Negative(u16 comp)
//{
//	A1=1;//EN �͵�ƽΪ����
//	A2=0;//DR �͵�ƽ��ת
//	Dac1_Set_Vol(35*comp+1000);
//}
