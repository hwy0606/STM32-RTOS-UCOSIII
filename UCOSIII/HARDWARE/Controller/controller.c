#include "controller.h"		
#include "motor.h"
#include "brake.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//������ƶ�������
//����ʽPID����
//����Motor�⺯����Brake�⺯��
//HWY 2018 1 2
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

/*
Motor�⺯��˵����
void Motor_Init(void); ��ʼ��
void Motor_Positive(u16 comp) ;�����ת ռ�ձ�comp 0-98
void Motor_Negative(u16 comp) ;�����ת ռ�ձ�comp 0-98
*/

/*
Brake�⺯��˵����
void Brake_Init() ; ��ʼ��
Brake_Controller(u16 comp) ; �ŷ��ƶ������� ������СΪ2A*comp
*/








