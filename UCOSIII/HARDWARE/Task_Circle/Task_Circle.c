//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//̤��ת��һȦ�����ж�  
//�ж�����1 EXTI_LINE2_CAPTURE_VAL������
//�ж�����2 ��սǶ�ֵ
//HWY 2017 11 3
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#include "stm32f4xx.h"
#include "delay.h"
#include "Omron_Counter.h"
#include "Task_Circle.h"
#include "encoder.h"
#include "Task_Loop.h"

u8  EXTI_LINE2_CAPTURE_VAL=0x00;	//����ֵ

void Task_Circle(void)   
{
    EXTI_LINE2_CAPTURE_VAL++;		 //Ȧ������
//	  angle_footboard=0;   //����Ƕ�ֵ	
//	  angle_motor=0 ;
}

 u8 Get_EXTI_LINE2_CAPTURE_VAL()
 {
	 return EXTI_LINE2_CAPTURE_VAL;
 }





		




