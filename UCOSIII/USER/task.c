#include "task.h"





//创建开始任务
void StartTaskCreate(OS_ERR err)
{
OSTaskCreate((OS_TCB 	* )&StartTaskTCB,		//任务控制块
				 (CPU_CHAR	* )"start task", 		//任务名字
                 (OS_TASK_PTR )start_task, 			//任务函数
                 (void		* )0,					//传递给任务函数的参数
                 (OS_PRIO	  )START_TASK_PRIO,     //任务优先级
                 (CPU_STK   * )&START_TASK_STK[0],	//任务堆栈基地址
                 (CPU_STK_SIZE)START_STK_SIZE/10,	//任务堆栈深度限位
                 (CPU_STK_SIZE)START_STK_SIZE,		//任务堆栈大小
                 (OS_MSG_QTY  )0,					//任务内部消息队列能够接收的最大消息数目,为0时禁止接收消息
                 (OS_TICK	  )0,					//当使能时间片轮转时的时间片长度，为0时为默认长度，
                 (void   	* )0,					//用户补充的存储区
                 (OS_OPT      )OS_OPT_TASK_STK_CHK|OS_OPT_TASK_STK_CLR, //任务选项
                 (OS_ERR 	* )&err);				//存放该函数错误时的返回值
}



//开始任务函数
void start_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	CPU_SR_ALLOC();
	CPU_Init();
#if OS_CFG_STAT_TASK_EN > 0u
   OSStatTaskCPUUsageInit(&err);  	//使能统计任务                
#endif
	
#ifdef CPU_CFG_INT_DIS_MEAS_EN		//如果使能了测量中断关闭时间
    CPU_IntDisMeasMaxCurReset();	
#endif

#if	OS_CFG_SCHED_ROUND_ROBIN_EN  //当使用时间片轮转的时候
	 //使能时间片轮转调度功能,时间片长度为1个系统时钟节拍，既1*5=5ms
	OSSchedRoundRobinCfg(DEF_ENABLED,1,&err);  
#endif		
	LED_GPIO_Config();         //LED初始化
	LED3_OFF;
	OS_CRITICAL_ENTER();	//进入临界区
	//开启相关定时器

	//开启相关信号量
//	MOTOR_UP_SEMCreate(err);
//	MOTOR_DOWN_SEMCreate(err);
	//开启相关消息队列
	MOTOR_MsgCreate(err);
	BRAKE_MsgCreate(err);
	//开启相关事件标志组
	MOTOR_FLAGCreate(err);
	BRAKE_FLAGCreate(err);
	//开启相关任务
	

	  LEDTaskCreate(err);
	//Usart2TaskCreate(err);
  	MotorTaskCreate(err);
  	BrakeTaskCreate	(err);
    MotorKeyTaskCreate(err);
		BrakeKeyTaskCreate(err);
	///////////////////////////////////// 
	OS_TaskSuspend((OS_TCB*)&StartTaskTCB,&err);		//挂起开始任务或者DEL	 
	OS_CRITICAL_EXIT();	//退出临界区
}

//创建LED测试任务
void LEDTaskCreate(OS_ERR err)
{

	OSTaskCreate((OS_TCB 	* )&LedTaskTCB,		
				 (CPU_CHAR	* )"led task",   //任务名字		
                 (OS_TASK_PTR )led_task, 	 //任务函数名字		
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

//led任务函数
// 本任务主要测试系统是否运行 
void led_task(void *p_arg)
{
	OS_ERR err;
	p_arg = p_arg;
	while(1)
	{	
		LED1_OFF;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
		LED1_ON;
		OSTimeDlyHMSM(0,0,0,500,OS_OPT_TIME_HMSM_STRICT,&err); //延时500ms
		
	}
}

	//创建USART22任务
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

//USART2任务函数
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
		//创建定时器 并轮询接收制动器状态		
//		LED2_OFF;		  
//			OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms			
//		LED2_ON;
//		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
		USART1_DMA_Send_Once_Data(Send_Date,Send_Size);		
		OSTimeDlyHMSM(0,0,0,200,OS_OPT_TIME_HMSM_STRICT,&err); //延时200ms
	}
}

void MotorTaskCreate(OS_ERR err)
	//创建电机控制任务
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
//电机控制任务
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
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时100ms		
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
                 (OS_MSG_QTY  )0,			 //是否启用任务消息队列		
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
		OSTimeDlyHMSM(0,0,0,100,OS_OPT_TIME_HMSM_STRICT,&err); //延时100ms	

	}
}
//电机控制按钮任务
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
                 (OS_MSG_QTY  )0,			 //是否启用任务消息队列		
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
	CPU_INT32U motor_comp=20;  //初始值
	OS_FLAGS motor_value;
	while(1)
	{
		motor_value=OSFlagPend(&MOTORFLAG,
							MOTOR_UP+MOTOR_DOWN,
							0,
							OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							0,
							&err);
		//修改comp占空比
		
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
		
		//发送相关消息队列
		OSQPost((OS_Q*		)&MOTOR_Msg,		
		(void*		)motor_comp,
		(OS_MSG_SIZE)sizeof (void *),
		(OS_OPT		)OS_OPT_POST_FIFO,
		(OS_ERR*	)&err);
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时50ms		
	}
	
}


//制动器控制按钮任务
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
                 (OS_MSG_QTY  )0,			 //是否启用任务消息队列		
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
	CPU_INT32U brake_comp=10;  //初始值
	OS_FLAGS brake_value;
	while(1)
	{
		brake_value=OSFlagPend(&BRAKEFLAG,
							BRAKE_UP+BRAKE_DOWN,
							0,
							OS_OPT_PEND_FLAG_SET_ANY+OS_OPT_PEND_FLAG_CONSUME,
							0,
							&err);
		//修改comp占空比
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
		
		//发送相关消息队列
		OSQPost((OS_Q*		)&BRAKE_Msg,		
		(void*		)brake_comp,
		(OS_MSG_SIZE)sizeof (void *),
		(OS_OPT		)OS_OPT_POST_FIFO,
		(OS_ERR*	)&err);
		OSTimeDlyHMSM(0,0,0,50,OS_OPT_TIME_HMSM_STRICT,&err); //延时50ms		
	}
	
}
////////////////////////////////msg//////////////////////////////////
void MOTOR_MsgCreate(OS_ERR err)
{
	OSQCreate ((OS_Q*		)&MOTOR_Msg,	//消息队列
                (CPU_CHAR*	)"MOTOR Msg",	//消息队列名称
                (OS_MSG_QTY	)MOTORMSG_Q_NUM,	//消息队列长度，这里设置为4
                (OS_ERR*	)&err);		//错误码
}
void BRAKE_MsgCreate(OS_ERR err)
{
	OSQCreate ((OS_Q*		)&BRAKE_Msg,	//消息队列
                (CPU_CHAR*	)"BRAKE Msg",	//消息队列名称
                (OS_MSG_QTY	)BRAKEMSG_Q_NUM,	//消息队列长度，这里设置为4
                (OS_ERR*	)&err);		//错误码
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

//////////////////////////////中断///////////////////////////////////
//外部中断1服务程序
void EXTI1_IRQHandler(void)
{
	OSIntEnter();
	delay_ms(5);	//消抖
	OS_ERR err;
	OS_FLAGS flags;
	 if(EXTI_GetITStatus(EXTI_Line1)!=RESET)
	 {
		 flags=OSFlagPost(&BRAKEFLAG,BRAKE_UP,OS_OPT_POST_FLAG_SET,&err);
		  LED2_ON;
	 }	
   OSIntExit(); 	 
	 EXTI_ClearITPendingBit(EXTI_Line1); //清除LINE1上的中断标志位 
}	
//外部中断2服务程序
void EXTI2_IRQHandler(void)
{
	OSIntEnter();
	delay_ms(5);	//消抖
	OS_ERR err;
	OS_FLAGS flags;
	 if(EXTI_GetITStatus(EXTI_Line2)!=RESET)
	 {
		 flags=OSFlagPost(&BRAKEFLAG,BRAKE_DOWN,OS_OPT_POST_FLAG_SET,&err);	
			LED2_OFF;		 
	 }	
   OSIntExit(); 	 
	 EXTI_ClearITPendingBit(EXTI_Line2);//清除LINE2上的中断标志位 
}
//外部中断3服务程序
void EXTI3_IRQHandler(void)
{
	OSIntEnter();
	delay_ms(2);	//消抖
	OS_ERR err;
	OS_FLAGS flags;
	 if(EXTI_GetITStatus(EXTI_Line3)!=RESET)
	 {
		  LED3_ON;
		flags=OSFlagPost(&MOTORFLAG,MOTOR_UP,OS_OPT_POST_FLAG_SET,&err);
	 }		 
	 OSIntExit(); 
	 EXTI_ClearITPendingBit(EXTI_Line3);  //清除LINE3上的中断标志位
}
//外部中断4服务程序
void EXTI4_IRQHandler(void)
{
	OSIntEnter();
	OS_ERR err;
	OS_FLAGS flags;
	delay_ms(2);	//消抖
	 if(EXTI_GetITStatus(EXTI_Line4)!=RESET)
	 {
		 LED3_OFF;
		 flags=OSFlagPost(&MOTORFLAG,MOTOR_DOWN,OS_OPT_POST_FLAG_SET,&err);		  
	 } 
	 OSIntExit(); 
	 EXTI_ClearITPendingBit(EXTI_Line4);//清除LINE4上的中断标志位  
}