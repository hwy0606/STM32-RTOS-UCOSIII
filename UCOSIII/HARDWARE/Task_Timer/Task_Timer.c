//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//��������ѭ����ʱ��TIM4
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
 * @Description: TIM4����ģʽ��ʼ��
 * @note1�� TIM4��16λ��ʱ��
 */
 
 //����uc/probe ʾ��������ļ�

#include "probe_scope.h"
#include "probe_scope_cfg.h"
extern u16 speed_comp;
void Task_Timer_Init(u16 arr,u16 psc)  //�Ƽ�arr=10000-1 psc=84-1  10ms   arr<16777214
{

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  			//ʹ��TIM4ʱ��
	  TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    TIM_TimeBaseInitStructure.TIM_Period =arr; 				//�Զ���װ��ֵ  
    TIM_TimeBaseInitStructure.TIM_Prescaler=psc;  					//��ʱ����Ƶ            
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//����Ƶ
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);				//��ʼ��TIM4
	
	
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE ); 						//����ʱ�������ж�
	
	
	
	  NVIC_InitTypeDef         NVIC_InitStructure;
	  NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn; 		//��ʱ���ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1; 	//��ռ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=2; 		//��Ӧ���ȼ�1
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);              //��ʼ��NVIC  
	
	
    TIM_Cmd(TIM4,ENABLE); 											//ʹ�ܶ�ʱ��
}



void TIM4_IRQHandler(void)
{
   if(TIM_GetITStatus (TIM4,TIM_IT_Update)==SET)//���ж�
	 {
		//���жϽ��еĲ���
		 
//		 Angle_Calculate();
//		 Angle_Transmission();
//		 Task_Loop();		
//     ��ѯ�ŷ��ƶ���״̬
//     Read_Brake_Date();	
//     �������		 ���µ��ռ�ձ�
//     u16 comp=Motor_control(speed_comp);
		 
		 ProbeScope_Sampling();
		 
	 }	 
	  TIM_ClearITPendingBit (TIM4,TIM_IT_Update );//����жϱ�־λ
}



		



