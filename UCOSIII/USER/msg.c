#include "msg.h"


void MOTOR_MsgCreate(OS_ERR err)
{
	OSQCreate ((OS_Q*		)&MOTOR_Msg,	//��Ϣ����
                (CPU_CHAR*	)"MOTOR Msg",	//��Ϣ��������
                (OS_MSG_QTY	)MOTORMSG_Q_NUM,	//��Ϣ���г��ȣ���������Ϊ4
                (OS_ERR*	)&err);		//������
}


