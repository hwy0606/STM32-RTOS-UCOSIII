//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//踏板转动一圈进入中断  
//中断任务1 EXTI_LINE2_CAPTURE_VAL的自增
//中断任务2 清空角度值
//HWY 2017 11 3
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#include "stm32f4xx.h"
#include "delay.h"
#include "Omron_Counter.h"
#include "Task_Circle.h"
#include "encoder.h"
#include "Task_Loop.h"

u8  EXTI_LINE2_CAPTURE_VAL=0x00;	//计数值

void Task_Circle(void)   
{
    EXTI_LINE2_CAPTURE_VAL++;		 //圈数自增
//	  angle_footboard=0;   //清零角度值	
//	  angle_motor=0 ;
}

 u8 Get_EXTI_LINE2_CAPTURE_VAL()
 {
	 return EXTI_LINE2_CAPTURE_VAL;
 }





		




