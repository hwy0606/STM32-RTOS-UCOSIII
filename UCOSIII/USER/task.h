#ifndef __TASK_H
#define __TASK_H	



#include "includes.h"
#include "os_app_hooks.h"


#include "delay.h"
#include "led.h"
#include "motor.h"
#include "test.h"
#include "usart1_dma.h"
#include "gpio.h"

	
//UCOSIII中以下优先级用户程序不能使用，
//将这些优先级分配给了UCOSIII的5个系统内部任务
//优先级0：中断服务服务管理任务 OS_IntQTask()
//优先级1：时钟节拍任务 OS_TickTask()
//优先级2：定时任务 OS_TmrTask()
//优先级OS_CFG_PRIO_MAX-2：统计任务 OS_StatTask()
//优先级OS_CFG_PRIO_MAX-1：空闲任务 OS_IdleTask()
////////////////////////////////////////////////////////

////////////////////////////////task//////////////////////////////////

//任务优先级
#define START_TASK_PRIO		3
//任务堆栈大小	
#define START_STK_SIZE 		512
//任务控制块
OS_TCB StartTaskTCB;
//任务堆栈	
CPU_STK START_TASK_STK[START_STK_SIZE];
//任务函数
void start_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//任务优先级
#define LED_TASK_PRIO		20
//任务堆栈大小	
#define LED_STK_SIZE 		128 //堆栈大小别乱改
//任务控制块
OS_TCB LedTaskTCB;
//任务堆栈	
CPU_STK LED_TASK_STK[LED_STK_SIZE];
void led_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//任务优先级
#define USART2_TASK_PRIO		5
//任务堆栈大小	
#define USART2_STK_SIZE 		64
//任务控制块
OS_TCB USART2TaskTCB;
//任务堆栈	
CPU_STK USART2_TASK_STK[USART2_STK_SIZE];
//任务函数
void usart2_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//任务优先级
#define MOTOR_TASK_PRIO		10
//任务堆栈大小
#define MOTOR_STK_SIZE		128
//任务控制块
OS_TCB	MOTORTaskTCB;
//任务堆栈
__align(8) CPU_STK	MOTOR_TASK_STK[MOTOR_STK_SIZE];
//任务函数
void motor_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//任务优先级
#define BRAKE_TASK_PRIO		11
//任务堆栈大小
#define BRAKE_STK_SIZE		128
//任务控制块
OS_TCB	BRAKETaskTCB;
//任务堆栈
__align(8) CPU_STK	BRAKE_TASK_STK[BRAKE_STK_SIZE];
//任务函数
void brake_task(void *p_arg);
////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//任务优先级
#define MOTOR_KEY_TASK_PRIO		6
//任务堆栈大小
#define MOTOR_KEY_STK_SIZE		96
//任务控制块
OS_TCB	MOTORKEYTaskTCB;
//任务堆栈
__align(8) CPU_STK	MOTOR_KEY_TASK_STK[MOTOR_KEY_STK_SIZE];
//任务函数
void motorkey_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//任务优先级
#define BRAKE_KEY_TASK_PRIO		7
//任务堆栈大小
#define BRAKE_KEY_STK_SIZE		96
//任务控制块
OS_TCB	BRAKEKEYTaskTCB;
//任务堆栈
__align(8) CPU_STK	BRAKE_KEY_TASK_STK[BRAKE_KEY_STK_SIZE];
//任务函数
void brakekey_task(void *p_arg);
//////////////////////////////////////////////////////


//////////////////相关函数
void StartTaskCreate(OS_ERR err);
void LEDTaskCreate(OS_ERR err);
void Usart2TaskCreate(OS_ERR err);
void MotorTaskCreate(OS_ERR err);
void BrakeTaskCreate(OS_ERR err);
void MotorKeyTaskCreate(OS_ERR err);
void BrakeKeyTaskCreate(OS_ERR err);


////////////////////////////////msg//////////////////////////////////
#define MOTORMSG_Q_NUM	4	//电机参数消息队列的数量
OS_Q MOTOR_Msg;				//定义一个电机消息队列
void MOTOR_MsgCreate(OS_ERR err);


#define BRAKEMSG_Q_NUM	4	//电机参数消息队列的数量
OS_Q BRAKE_Msg;				//定义一个电机消息队列
void BRAKE_MsgCreate(OS_ERR err);

////////////////////////////////flag//////////////////////////////////
#define MOTOR_UP (OS_FLAGS)0x0001 //最后一位
#define MOTOR_DOWN (OS_FLAGS)0x0002 //倒数第二位

#define BRAKE_UP (OS_FLAGS)0x0001
#define BRAKE_DOWN (OS_FLAGS)0x0002

OS_FLAG_GRP MOTORFLAG;
OS_FLAG_GRP BRAKEFLAG;

void MOTOR_FLAGCreate(OS_ERR err);
void BRAKE_FLAGCreate(OS_ERR err);
////////////////////////////////sem//////////////////////////////////
//定义电机制动器控制相关信号
//OS_SEM	MOTOR_UP_SEM;		//定义一个信号量 电机上升
//OS_SEM	MOTOR_DOWN_SEM;		//定义一个信号量 电机上升
//OS_SEM	BRAKE_UP_SEM;		//定义一个信号量 电机上升
//OS_SEM	BRAKE_DOWN_SEM;		//定义一个信号量 电机上升

//void MOTOR_UP_SEMCreate (OS_ERR err);
//void MOTOR_DOWN_SEMCreate (OS_ERR err);
//void BRAKE_UP_SEMCreate (OS_ERR err);
//void BRAKE_DOWN_SEMCreate (OS_ERR err);

#endif 