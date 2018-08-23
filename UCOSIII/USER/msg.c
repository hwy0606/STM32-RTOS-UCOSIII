#include "msg.h"


void MOTOR_MsgCreate(OS_ERR err)
{
	OSQCreate ((OS_Q*		)&MOTOR_Msg,	//消息队列
                (CPU_CHAR*	)"MOTOR Msg",	//消息队列名称
                (OS_MSG_QTY	)MOTORMSG_Q_NUM,	//消息队列长度，这里设置为4
                (OS_ERR*	)&err);		//错误码
}


