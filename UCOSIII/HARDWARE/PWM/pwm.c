#include "pwm.h"
#include "stm32f4xx.h"  //引入u32等头文件
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//PA0 TIM5_CH1 PWM驱动 
//PA1 TIM5_CH2 PWM驱动
//HWY 2017 10 17
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 


//TIM5_CH1 CH2 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数

void TIM5CH3_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2,GPIO_AF_TIM5); //GPIOA0 A2复用为定时器5
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_2;           //GPIOA0 A2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA2
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //不分频
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器5
	
	//初始化TIM5 CHl和CH2  PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC3Init(TIM5, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIM5 4OC1

	TIM_OC3PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1上的预装载寄存器

  TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM5, ENABLE);  //使能TIM5
 
										  
}  



void TIM5CH2_PWM_Init(u32 arr,u32 psc)
{		 					 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);  	//TIM5时钟使能    
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	//使能PORTA时钟	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource1,GPIO_AF_TIM5); //GPIOA0 A1复用为定时器5
	
	GPIO_InitStructure.GPIO_Pin =GPIO_Pin_1;           //GPIOA0 A1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        //复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      //推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;        //上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure);              //初始化PA0
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=arr;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; //不分频
	
	TIM_TimeBaseInit(TIM5,&TIM_TimeBaseStructure);//初始化定时器5
	
	//初始化TIM5 CHl和CH2  PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; //输出极性:TIM输出比较极性低
	TIM_OC2Init(TIM5, &TIM_OCInitStructure);  //根据指定的参数初始化外设TIM5 4OC1

	TIM_OC2PreloadConfig(TIM5, TIM_OCPreload_Enable);  //使能TIM5在CCR1上的预装载寄存器

  TIM_ARRPreloadConfig(TIM5,ENABLE);//ARPE使能 
	
	TIM_Cmd(TIM5, ENABLE);  //使能TIM5
 
										  
} 

