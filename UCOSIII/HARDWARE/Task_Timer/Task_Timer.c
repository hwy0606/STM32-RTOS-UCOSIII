//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//控制任务循环定时器TIM4
//HWY 2017 10 23
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#include "stm32f4xx.h"
#include "Task_Timer.h"
#include "Task_Loop.h" 
#include "brake.h"
#include "motor_control.h"
#include "motor.h"
/**
 * @Description: TIM4计数模式初始化
 * @note1： TIM4是16位定时器
 */
 
 //调用uc/probe 示波器相关文件

#include "probe_scope.h"
#include "probe_scope_cfg.h"
extern u16 speed_comp;
void Task_Timer_Init(u16 arr,u16 psc)  //推荐arr=10000-1 psc=84-1  10ms   arr<16777214
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  			//使能TIM4时钟
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period =arr; 				//自动重装载值  
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  					//定时器分频            
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//不分频
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);				//初始化TIM4
	
	
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 						//允许定时器更新中断
	
	
	
	  NVIC_InitTypeDef         NVIC_InitStructure;
	  NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; 		//定时器中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; 	//抢占优先级1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; 		//响应优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);              //初始化NVIC  
	
	
    TIM_Cmd(TIM4,ENABLE); 											//使能定时器
}



void TIM4_IRQHandler(void)
{
   if(TIM_GetITStatus (TIM4,TIM_IT_Update)==SET)//进中断
	 {
		//进中断进行的操作
		 
//		 Angle_Calculate();
//		 Angle_Transmission();
//		 Task_Loop();		
//     查询磁粉制动器状态
//     Read_Brake_Date();	
//     电机控制		 更新电机占空比
//     u16 comp=Motor_control(speed_comp);
		 
		 ProbeScope_Sampling();
		 
	 }	 
	  TIM_ClearITPendingBit (TIM4,TIM_IT_Update );//清除中断标志位
}



		



