//#include "Omron_Counter.h"
//#include "stm32f4xx.h"
//#include "Task_Circle.h"
////////////////////////////////////////////////////////////////////////////////////	 
////STM32F407IGT6
////��翪������
////�ⲿ�ж� �͵�ƽ���������ж� �жϴ�����Task_Circle()
////���� PE2 
////HWY 2017 11 2 
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 	 
///**
// * @Description: PE2 EXTI_LINE2���ж�����
// * @note1�� ע�ⴥ����ʽ
// */
// void EXTIX_Init(void)
// {
//	 //��ʼ��GPIOʱ�Ӽ�����
//	 GPIO_InitTypeDef GPIO_InitStructure;
//	 RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
//	 
//	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //GPIOE2
//	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//����ģʽ
//	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//�ٶ�100MHz
//	 GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP; //����  ����½���
//	 GPIO_Init(GPIOE,&GPIO_InitStructure); //��ʼ��PE2
//	 
//	 //��ʼ���ⲿ�ж�
//	 EXTI_InitTypeDef EXTI_InitStructure;
//	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG ,ENABLE);
//	 SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource2);//PE2���ӵ�EXTI_LINE2
//	 
//	 EXTI_InitStructure.EXTI_Line=EXTI_Line2;
//	 EXTI_InitStructure.EXTI_Mode =EXTI_Mode_Interrupt;
//	 EXTI_InitStructure.EXTI_Trigger=EXTI_Trigger_Falling; //�½��ش���
//	 EXTI_InitStructure.EXTI_LineCmd =ENABLE; //�ж�ʹ��
//	 EXTI_Init(&EXTI_InitStructure);
//	  
//	 //EXTI_LINE2 �ж�����
//	 NVIC_InitTypeDef NVIC_InitStructure;
//   NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn ;  
//	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2;//��ռ���ȼ�
//	 NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;		//�����ȼ�
//	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
//	 NVIC_Init(&NVIC_InitStructure);	
//		
// }
// 

// void EXTI2_IRQHandler(void)
// {
//	 //�����Ƕ�������
//	 if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
//	 {
//		 Task_Circle();  //��Ȧ�ж�
//	 }
//	 EXTI_ClearITPendingBit(EXTI_Line2);//����жϱ�־λ
// }
// 
// void Omron_Counter_Init(void)
// {
//	 EXTIX_Init();
// }
// 
