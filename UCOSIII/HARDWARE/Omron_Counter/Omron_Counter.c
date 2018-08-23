//#include "Omron_Counter.h"
//#include "stm32f4xx.h"
//#include "Task_Circle.h"
////////////////////////////////////////////////////////////////////////////////////	 
////STM32F407IGT6
////光电开关驱动
////外部中断 低电平触发进入中断 中断处理函数Task_Circle()
////接线 PE2 
////HWY 2017 11 2 
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 	 
///**
// * @Description: PE2 EXTI_LINE2线中断配置
// * @note1： 注意触发方式
// */
// void EXTIX_Init(void)
// {
//	 //初始化GPIO时钟及引脚
//	 GPIO_InitTypeDef GPIO_InitStructure;
//	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
//	 
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOE2
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//输入模式
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
//	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //上拉  检测下降沿
//	 GPIO_Init(GPIOE,&GPIO_InitStructure); //初始化PE2
//	 
//	 //初始化外部中断
//	 EXTI_InitTypeDef EXTI_InitStructure;
//	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG ,ENABLE);
//	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);//PE2连接到EXTI_LINE2
//	 
//	 EXTI_InitStructure.EXTI_Line=EXTI_Line2;
//	 EXTI_InitStructure.EXTI_Mode =EXTI_Mode_Interrupt;
//	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //下降沿触发
//	 EXTI_InitStructure.EXTI_LineCmd =ENABLE; //中断使能
//	 EXTI_Init(&EXTI_InitStructure);
//	  
//	 //EXTI_LINE2 中断配置
//	 NVIC_InitTypeDef NVIC_InitStructure;
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn ;  
//	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//抢占优先级
//	 NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//子优先级
//	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
//	 NVIC_Init(&NVIC_InitStructure);	
//		
// }
// 

// void EXTI2_IRQHandler(void)
// {
//	 //不考虑抖动问题
//	 if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
//	 {
//		 Task_Circle();  //计圈中断
//	 }
//	 EXTI_ClearITPendingBit(EXTI_Line2);//清除中断标志位
// }
// 
// void Omron_Counter_Init(void)
// {
//	 EXTIX_Init();
// }
// 
