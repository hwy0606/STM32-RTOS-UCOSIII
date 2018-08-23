#include "sem.h"

void MOTOR_UP_MEMCreate (OS_ERR err)
{
	OSSemCreate ((OS_SEM*	)&MOTOR_UP_SEM,
                 (CPU_CHAR*	)"MOTOR_UP_SEM",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);	
}

void MOTOR_DOWN_MEMCreate (OS_ERR err)
{
	OSSemCreate ((OS_SEM*	)&MOTOR_DOWN_SEM,
                 (CPU_CHAR*	)"MOTOR_DOWN_SEM",
                 (OS_SEM_CTR)0,		
                 (OS_ERR*	)&err);	
}