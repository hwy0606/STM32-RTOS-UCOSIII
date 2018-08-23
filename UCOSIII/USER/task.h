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

	
//UCOSIII���������ȼ��û�������ʹ�ã�
//����Щ���ȼ��������UCOSIII��5��ϵͳ�ڲ�����
//���ȼ�0���жϷ������������� OS_IntQTask()
//���ȼ�1��ʱ�ӽ������� OS_TickTask()
//���ȼ�2����ʱ���� OS_TmrTask()
//���ȼ�OS_CFG_PRIO_MAX-2��ͳ������ OS_StatTask()
//���ȼ�OS_CFG_PRIO_MAX-1���������� OS_IdleTask()
////////////////////////////////////////////////////////

////////////////////////////////task//////////////////////////////////

//�������ȼ�
#define START_TASK_PRIO		3
//�����ջ��С	
#define START_STK_SIZE 		512
//������ƿ�
OS_TCB StartTaskTCB;
//�����ջ	
CPU_STK START_TASK_STK[START_STK_SIZE];
//������
void start_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//�������ȼ�
#define LED_TASK_PRIO		20
//�����ջ��С	
#define LED_STK_SIZE 		128 //��ջ��С���Ҹ�
//������ƿ�
OS_TCB LedTaskTCB;
//�����ջ	
CPU_STK LED_TASK_STK[LED_STK_SIZE];
void led_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//�������ȼ�
#define USART2_TASK_PRIO		5
//�����ջ��С	
#define USART2_STK_SIZE 		64
//������ƿ�
OS_TCB USART2TaskTCB;
//�����ջ	
CPU_STK USART2_TASK_STK[USART2_STK_SIZE];
//������
void usart2_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//�������ȼ�
#define MOTOR_TASK_PRIO		10
//�����ջ��С
#define MOTOR_STK_SIZE		128
//������ƿ�
OS_TCB	MOTORTaskTCB;
//�����ջ
__align(8) CPU_STK	MOTOR_TASK_STK[MOTOR_STK_SIZE];
//������
void motor_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//�������ȼ�
#define BRAKE_TASK_PRIO		11
//�����ջ��С
#define BRAKE_STK_SIZE		128
//������ƿ�
OS_TCB	BRAKETaskTCB;
//�����ջ
__align(8) CPU_STK	BRAKE_TASK_STK[BRAKE_STK_SIZE];
//������
void brake_task(void *p_arg);
////////////////////////////////////////////////////////
//////////////////////////////////////////////////////
//�������ȼ�
#define MOTOR_KEY_TASK_PRIO		6
//�����ջ��С
#define MOTOR_KEY_STK_SIZE		96
//������ƿ�
OS_TCB	MOTORKEYTaskTCB;
//�����ջ
__align(8) CPU_STK	MOTOR_KEY_TASK_STK[MOTOR_KEY_STK_SIZE];
//������
void motorkey_task(void *p_arg);
////////////////////////////////////////////////////////
////////////////////////////////////////////////////////
//�������ȼ�
#define BRAKE_KEY_TASK_PRIO		7
//�����ջ��С
#define BRAKE_KEY_STK_SIZE		96
//������ƿ�
OS_TCB	BRAKEKEYTaskTCB;
//�����ջ
__align(8) CPU_STK	BRAKE_KEY_TASK_STK[BRAKE_KEY_STK_SIZE];
//������
void brakekey_task(void *p_arg);
//////////////////////////////////////////////////////


//////////////////��غ���
void StartTaskCreate(OS_ERR err);
void LEDTaskCreate(OS_ERR err);
void Usart2TaskCreate(OS_ERR err);
void MotorTaskCreate(OS_ERR err);
void BrakeTaskCreate(OS_ERR err);
void MotorKeyTaskCreate(OS_ERR err);
void BrakeKeyTaskCreate(OS_ERR err);


////////////////////////////////msg//////////////////////////////////
#define MOTORMSG_Q_NUM	4	//���������Ϣ���е�����
OS_Q MOTOR_Msg;				//����һ�������Ϣ����
void MOTOR_MsgCreate(OS_ERR err);


#define BRAKEMSG_Q_NUM	4	//���������Ϣ���е�����
OS_Q BRAKE_Msg;				//����һ�������Ϣ����
void BRAKE_MsgCreate(OS_ERR err);

////////////////////////////////flag//////////////////////////////////
#define MOTOR_UP (OS_FLAGS)0x0001 //���һλ
#define MOTOR_DOWN (OS_FLAGS)0x0002 //�����ڶ�λ

#define BRAKE_UP (OS_FLAGS)0x0001
#define BRAKE_DOWN (OS_FLAGS)0x0002

OS_FLAG_GRP MOTORFLAG;
OS_FLAG_GRP BRAKEFLAG;

void MOTOR_FLAGCreate(OS_ERR err);
void BRAKE_FLAGCreate(OS_ERR err);
////////////////////////////////sem//////////////////////////////////
//�������ƶ�����������ź�
//OS_SEM	MOTOR_UP_SEM;		//����һ���ź��� �������
//OS_SEM	MOTOR_DOWN_SEM;		//����һ���ź��� �������
//OS_SEM	BRAKE_UP_SEM;		//����һ���ź��� �������
//OS_SEM	BRAKE_DOWN_SEM;		//����һ���ź��� �������

//void MOTOR_UP_SEMCreate (OS_ERR err);
//void MOTOR_DOWN_SEMCreate (OS_ERR err);
//void BRAKE_UP_SEMCreate (OS_ERR err);
//void BRAKE_DOWN_SEMCreate (OS_ERR err);

#endif 