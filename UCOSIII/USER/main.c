#include "delay.h"
#include "includes.h"
#include "os_app_hooks.h"



//#include "task.h"
//����uc/probe ʾ��������ļ�

//#define PROBE_SCOPE

#ifdef PROBE_SCOPE
#include "probe_scope.h"
#include "probe_scope_cfg.h"
#endif


int main(void)
{
	
	#ifdef PROBE_SCOPE
	ProbeScope_Init(10000); //���β���Ƶ�� HZ
	Task_Timer_Init(10000-1,84-1);  //����ѭ�� 10ms
	#endif
	
	
	CPU_SR_ALLOC(); //�����ٽ�������ʱ������CPU_SR_ALLOC()
	delay_init(168);  	//ʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�жϷ�������
	

	

//Motor_Init();  //�����ʼ��
//USART1_DMA_Init(115200);  //����1DMA��ʼ�� ��λ�� �������
//USART2_DMA_Init(9600);   //����2DMA��ʼ�� �ŷ��ƶ�����Դ����
//Encoder_Init();   //������������ʼ��
//Sensor_Current_Init(); //���������������ʼ��
//Omron_Counter_Init(); //��翪�س�ʼ��
//Task_Timer_Init(10000-1,84-1);  //����ѭ�� 10ms
//PID_init(); //PID�ṹ���ʼ��
//Brake_mode(3);
//Motor_Positive(30);
	
	
	
	OS_ERR err;
	OSInit(&err);		//��ʼ��UCOSIII
	OS_CRITICAL_ENTER();//�����ٽ���
	StartTaskCreate(err);//������ʼ����
	OS_CRITICAL_EXIT();	//�˳��ٽ���	 
	OSStart(&err);  //����UCOSIII
	
	while(1);
}



	



