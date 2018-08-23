#ifndef __SEM_H
#define __SEM_H	


#include "includes.h"
#include "os_app_hooks.h"

//定义电机制动器控制相关信号
OS_SEM	MOTOR_UP_SEM;		//定义一个信号量 电机上升
OS_SEM	MOTOR_DOWN_SEM;		//定义一个信号量 电机上升
OS_SEM	BRAKE_UP_SEM;		//定义一个信号量 电机上升
OS_SEM	BRAKE_DOWN_SEM;		//定义一个信号量 电机上升

void MOTOR_UP_SEMCreate ();
void MOTOR_DOWN_SEMCreate ();
void BRAKE_UP_SEMCreate ();
void BRAKE_DOWN_SEMCreate ();





#endif 