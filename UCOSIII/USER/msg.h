#ifndef __MSG_H
#define __MSG_H	

#include "includes.h"
#include "os_app_hooks.h"

#define MOTORMSG_Q_NUM	4	//���������Ϣ���е�����
OS_Q MOTOR_Msg;				//����һ�������Ϣ����
void MOTOR_MsgCreate(OS_ERR err);




#endif 