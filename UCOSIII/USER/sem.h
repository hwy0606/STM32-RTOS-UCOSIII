#ifndef __SEM_H
#define __SEM_H	


#include "includes.h"
#include "os_app_hooks.h"

//�������ƶ�����������ź�
OS_SEM	MOTOR_UP_SEM;		//����һ���ź��� �������
OS_SEM	MOTOR_DOWN_SEM;		//����һ���ź��� �������
OS_SEM	BRAKE_UP_SEM;		//����һ���ź��� �������
OS_SEM	BRAKE_DOWN_SEM;		//����һ���ź��� �������

void MOTOR_UP_SEMCreate ();
void MOTOR_DOWN_SEMCreate ();
void BRAKE_UP_SEMCreate ();
void BRAKE_DOWN_SEMCreate ();





#endif 