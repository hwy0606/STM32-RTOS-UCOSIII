#ifndef __MSG_H
#define __MSG_H	

#include "includes.h"
#include "os_app_hooks.h"

#define MOTORMSG_Q_NUM	4	//电机参数消息队列的数量
OS_Q MOTOR_Msg;				//定义一个电机消息队列
void MOTOR_MsgCreate(OS_ERR err);




#endif 