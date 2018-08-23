#include "task.h"





//������ʼ����
void StartTaskCreate(OS_ERR err)
{
OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//������ƿ�
				 (CPU_CHAR	* )"start task", 		//��������
                 (OS_TASK_PTR )start_task, 			//������
                 (void		* )0,					//���ݸ��������Ĳ���
                 (OS_PRIO	  )START_TASK_PRIO,     //�������ȼ�
                 (CPU_STK   * )&START_TASK_STK[0],	//�����ջ����ַ
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//�����ջ�����λ
                 (CPU_STK_SIZE)START_STK_SIZE,		//�����ջ��С
                 (OS_MSG_QTY  )0,					//�����ڲ���Ϣ�����ܹ����յ������Ϣ��Ŀ,Ϊ0ʱ��ֹ������Ϣ
                 (OS_TICK	  )0,					//��ʹ��ʱ��Ƭ��תʱ��ʱ��Ƭ���ȣ�Ϊ0ʱΪĬ�ϳ��ȣ�
                 (void   	* )0,					//�û�����Ĵ洢��
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //����ѡ��
                 (OS_ERR 	* )&err);				//��Ÿú�������ʱ�ķ���ֵ
}



//��ʼ������
void start_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	CPU_SR_ALLOC();
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//ʹ��ͳ������                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//���ʹ���˲����жϹر�ʱ��
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //��ʹ��ʱ��Ƭ��ת��ʱ��
	 //ʹ��ʱ��Ƭ��ת���ȹ���,ʱ��Ƭ����Ϊ1��ϵͳʱ�ӽ��ģ���1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	LED_GPIO_Config();         //LED��ʼ��
	LED3_OFF;
	OS_CRITICAL_ENTER();	//�����ٽ���
	//������ض�ʱ��

	//��������ź���
//	MOTOR_UP_SEMCreate(err);
//	MOTOR_DOWN_SEMCreate(err);
	//���������Ϣ����
	MOTOR_MsgCreate(err);
	BRAKE_MsgCreate(err);
	//��������¼���־��
	MOTOR_FLAGCreate(err);
	BRAKE_FLAGCreate(err);
	//�����������
	

	  LEDTaskCreate(err);
	//Usart2TaskCreate(err);
  	MotorTaskCreate(err);
  	BrakeTaskCreate	(err);
    MotorKeyTaskCreate(err);
		BrakeKeyTaskCreate(err);
	///////////////////////////////////// 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//����ʼ�������DEL	 
	OS_CRITICAL_EXIT();	//�˳��ٽ���
}

//����LED��������
void LEDTaskCreate(OS_ERR err)
{

	OSTaskCreate((OS_TCB 	* )&LedTaskTCB,		
				 (CPU_CHAR	* )"led task",   //��������		
                 (OS_TASK_PTR )led_task, 	 //����������		
                 (void		* )0,					
                 (OS_PRIO	  )LED_TASK_PRIO,     
                 (CPU_STK   * )&LED_TASK_STK[0],	
                 (CPU_STK_SIZE)LED_STK_SIZE/10,	
                 (CPU_STK_SIZE)LED_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,					
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR,
                 (OS_ERR 	* )&err);				
}

//led������
// ��������Ҫ����ϵͳ�Ƿ����� 
void led_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{	
		LED1_OFF;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
		LED1_ON;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ500ms
		
	}
}

	//����USART22����
void Usart2TaskCreate(OS_ERR err)
{
	OSTaskCreate((OS_TCB 	* )&USART2TaskTCB,		
				 (CPU_CHAR	* )"usart2 task", 		
                 (OS_TASK_PTR )usart2_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )USART2_TASK_PRIO,     	
                 (CPU_STK   * )&USART2_TASK_STK[0],	
                 (CPU_STK_SIZE)USART2_STK_SIZE/10,	
                 (CPU_STK_SIZE)USART2_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);
}

//USART2������
void usart2_task(void *p_arg)
{
	
	OS_ERR err;
	p_arg = p_arg;
	#define Send_Size 6
	unsigned char Send_Date[Send_Size];
	USART1_DMA_Init(115200);  //
		Send_Date[0]=0x5A;   //
		Send_Date[1]=0x5B; //
		Send_Date[2]=0x5C; //
		Send_Date[3]=0x5D; //
		Send_Date[4]=0x5E; //
	while(1)
	{
		//������ʱ�� ����ѯ�����ƶ���״̬		
//		LED2_OFF;		  
//			OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms			
//		LED2_ON;
//		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
		USART1_DMA_Send_Once_Data(Send_Date,Send_Size);		
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ200ms
	}
}

void MotorTaskCreate(OS_ERR err)
	//���������������
{
	OSTaskCreate((OS_TCB 	* )&MOTORTaskTCB,		
				 (CPU_CHAR	* )"motor_task", 		
                 (OS_TASK_PTR )motor_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )MOTOR_TASK_PRIO,     	
                 (CPU_STK   * )&MOTOR_TASK_STK[0],	
                 (CPU_STK_SIZE)MOTOR_STK_SIZE/10,	
                 (CPU_STK_SIZE)MOTOR_STK_SIZE,		
                 (OS_MSG_QTY  )0,					
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);			
}
//�����������
void motor_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	Motor_Init();
	CPU_INT32U motor_comp;
  OS_MSG_SIZE size;
	while(1)
	{
	  motor_comp=	(CPU_INT32U)OSQPend((OS_Q*			)&MOTOR_Msg,   
					         (OS_TICK		)0,
                    (OS_OPT			)OS_OPT_PEND_BLOCKING,
                    (OS_MSG_SIZE*	)&size,		
                    (CPU_TS*		)0,
                    (OS_ERR*		)&err);
		Motor_Negative(motor_comp);		
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ100ms		
	}
};


void BrakeTaskCreate(OS_ERR err)
{
		OSTaskCreate((OS_TCB 	* )&BRAKETaskTCB,		
				 (CPU_CHAR	* )"brake_task", 		
                 (OS_TASK_PTR )brake_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )BRAKE_TASK_PRIO,     	
                 (CPU_STK   * )&BRAKE_TASK_STK[0],	
                 (CPU_STK_SIZE)BRAKE_STK_SIZE/10,	
                 (CPU_STK_SIZE)BRAKE_STK_SIZE,		
                 (OS_MSG_QTY  )0,			 //�Ƿ�����������Ϣ����		
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);			
	
}

void brake_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	Brake_Init();
	CPU_INT32U brake_comp;
  OS_MSG_SIZE size;
	while(1)
	{
		 brake_comp=	(CPU_INT32U)OSQPend((OS_Q*			)&BRAKE_Msg,   
					         (OS_TICK		)0,
                    (OS_OPT			)OS_OPT_PEND_BLOCKING,
                    (OS_MSG_SIZE*	)&size,		
                    (CPU_TS*		)0,
                    (OS_ERR*		)&err);
		Brake_Positive(brake_comp);		
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ100ms	

	}
}
//������ư�ť����
void MotorKeyTaskCreate(OS_ERR err)
{
	OSTaskCreate((OS_TCB 	* )&MOTORKEYTaskTCB,		
				 (CPU_CHAR	* )"motorkey_task", 		
                 (OS_TASK_PTR )motorkey_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )MOTOR_KEY_TASK_PRIO,     	
                 (CPU_STK   * )&MOTOR_KEY_TASK_STK[0],	
                 (CPU_STK_SIZE)MOTOR_KEY_STK_SIZE/10,	
                 (CPU_STK_SIZE)MOTOR_KEY_STK_SIZE,		
                 (OS_MSG_QTY  )0,			 //�Ƿ�����������Ϣ����		
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				
}
void motorkey_task(void *p_arg)
{
	KEY_Init();
  OS_ERR err;
	p_arg = p_arg;
	CPU_INT32U motor_comp=20;  //��ʼֵ
	OS_FLAGS motor_value;
	while(1)
	{
		motor_value=OSFlagPend(&MOTORFLAG,
							MOTOR_UP+MOTOR_DOWN,
							0,
							OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							0,
							&err);
		//�޸�compռ�ձ�
		
		if(motor_value==MOTOR_UP)
		{
			if(motor_comp<90)
			{
			motor_comp=motor_comp+5;
			}
		}
		if(motor_value==MOTOR_DOWN)
		{
			if(motor_comp>10)
			{
			motor_comp=motor_comp-5;
			}
		}
		
		//���������Ϣ����
		OSQPost((OS_Q*		)&MOTOR_Msg,		
		(void*		)motor_comp,
		(OS_MSG_SIZE)sizeof (void *),
		(OS_OPT		)OS_OPT_POST_FIFO,
		(OS_ERR*	)&err);
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ50ms		
	}
	
}


//�ƶ������ư�ť����
void BrakeKeyTaskCreate(OS_ERR err)
{
	OSTaskCreate((OS_TCB 	* )&BRAKEKEYTaskTCB,		
				 (CPU_CHAR	* )"brakekey_task", 		
                 (OS_TASK_PTR )brakekey_task, 			
                 (void		* )0,					
                 (OS_PRIO	  )BRAKE_KEY_TASK_PRIO,     	
                 (CPU_STK   * )&BRAKE_KEY_TASK_STK[0],	
                 (CPU_STK_SIZE)BRAKE_KEY_STK_SIZE/10,	
                 (CPU_STK_SIZE)BRAKE_KEY_STK_SIZE,		
                 (OS_MSG_QTY  )0,			 //�Ƿ�����������Ϣ����		
                 (OS_TICK	  )0,					
                 (void   	* )0,				
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, 
                 (OS_ERR 	* )&err);				
}

void brakekey_task(void *p_arg)
{
	KEY_Init();
  OS_ERR err;
	p_arg = p_arg;
	CPU_INT32U brake_comp=10;  //��ʼֵ
	OS_FLAGS brake_value;
	while(1)
	{
		brake_value=OSFlagPend(&BRAKEFLAG,
							BRAKE_UP+BRAKE_DOWN,
							0,
							OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							0,
							&err);
		//�޸�compռ�ձ�
		if(brake_value==BRAKE_UP)
		{
			if(brake_comp<90)
			{
			brake_comp=brake_comp+5;
			}
		}
		if(brake_value==BRAKE_DOWN)
		{
			if(brake_comp>10)
			{
			brake_comp=brake_comp-5;
			}
		}
		
		//���������Ϣ����
		OSQPost((OS_Q*		)&BRAKE_Msg,		
		(void*		)brake_comp,
		(OS_MSG_SIZE)sizeof (void *),
		(OS_OPT		)OS_OPT_POST_FIFO,
		(OS_ERR*	)&err);
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //��ʱ50ms		
	}
	
}
////////////////////////////////msg//////////////////////////////////
void MOTOR_MsgCreate(OS_ERR err)
{
	OSQCreate ((OS_Q*		)&MOTOR_Msg,	//��Ϣ����
                (CPU_CHAR*	)"MOTOR Msg",	//��Ϣ��������
                (OS_MSG_QTY	)MOTORMSG_Q_NUM,	//��Ϣ���г��ȣ���������Ϊ4
                (OS_ERR*	)&err);		//������
}
void BRAKE_MsgCreate(OS_ERR err)
{
	OSQCreate ((OS_Q*		)&BRAKE_Msg,	//��Ϣ����
                (CPU_CHAR*	)"BRAKE Msg",	//��Ϣ��������
                (OS_MSG_QTY	)BRAKEMSG_Q_NUM,	//��Ϣ���г��ȣ���������Ϊ4
                (OS_ERR*	)&err);		//������
}
////////////////////////////////sem//////////////////////////////////
//void MOTOR_UP_SEMCreate (OS_ERR err)
//{
//	OSSemCreate ((OS_SEM*	)&MOTOR_UP_SEM,
//                 (CPU_CHAR*	)"MOTOR_UP_SEM",
//                 (OS_SEM_CTR)0,		
//                 (OS_ERR*	)&err);	
//}

//void MOTOR_DOWN_SEMCreate (OS_ERR err)
//{
//	OSSemCreate ((OS_SEM*	)&MOTOR_DOWN_SEM,
//                 (CPU_CHAR*	)"MOTOR_DOWN_SEM",
//                 (OS_SEM_CTR)0,		
//                 (OS_ERR*	)&err);	
//}

//////////////////////////////flag///////////////////////////////////
void MOTOR_FLAGCreate(OS_ERR err)
{
	OSFlagCreate(&MOTORFLAG,
								"MOTORFLAG",
								(OS_FLAGS)0,
								&err);
	
}
void BRAKE_FLAGCreate(OS_ERR err)
{
	OSFlagCreate(&BRAKEFLAG,
								"BRAKEFLAG",
								(OS_FLAGS)0,
								&err);	
}

//////////////////////////////�ж�///////////////////////////////////
//�ⲿ�ж�1�������
void EXTI1_IRQHandler(void)
{
	OSIntEnter();
	delay_ms(5);	//����
	OS_ERR err;
	OS_FLAGS flags;
	 if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	 {
		 flags=OSFlagPost(&BRAKEFLAG,BRAKE_UP,OS_OPT_POST_FLAG_SET,&err);
		  LED2_ON;
	 }	
   OSIntExit(); 	 
	 EXTI_ClearITPendingBit(EXTI_Line1); //���LINE1�ϵ��жϱ�־λ 
}	
//�ⲿ�ж�2�������
void EXTI2_IRQHandler(void)
{
	OSIntEnter();
	delay_ms(5);	//����
	OS_ERR err;
	OS_FLAGS flags;
	 if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
	 {
		 flags=OSFlagPost(&BRAKEFLAG,BRAKE_DOWN,OS_OPT_POST_FLAG_SET,&err);	
			LED2_OFF;		 
	 }	
   OSIntExit(); 	 
	 EXTI_ClearITPendingBit(EXTI_Line2);//���LINE2�ϵ��жϱ�־λ 
}
//�ⲿ�ж�3�������
void EXTI3_IRQHandler(void)
{
	OSIntEnter();
	delay_ms(2);	//����
	OS_ERR err;
	OS_FLAGS flags;
	 if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	 {
		  LED3_ON;
		flags=OSFlagPost(&MOTORFLAG,MOTOR_UP,OS_OPT_POST_FLAG_SET,&err);
	 }		 
	 OSIntExit(); 
	 EXTI_ClearITPendingBit(EXTI_Line3);  //���LINE3�ϵ��жϱ�־λ
}
//�ⲿ�ж�4�������
void EXTI4_IRQHandler(void)
{
	OSIntEnter();
	OS_ERR err;
	OS_FLAGS flags;
	delay_ms(2);	//����
	 if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	 {
		 LED3_OFF;
		 flags=OSFlagPost(&MOTORFLAG,MOTOR_DOWN,OS_OPT_POST_FLAG_SET,&err);		  
	 } 
	 OSIntExit(); 
	 EXTI_ClearITPendingBit(EXTI_Line4);//���LINE4�ϵ��жϱ�־λ  
}