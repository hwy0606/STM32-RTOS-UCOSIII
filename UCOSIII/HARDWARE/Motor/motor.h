#ifndef __MOTOR_H
#define __MOTOR_H
#include "stm32f4xx.h"
#include "sys.h"
//µç»ú¿ØÖÆ
void Motor_Init(void);
void Motor_Break(void);
void Motor_Positive(u16 comp);
void Motor_Negative(u16 comp);


#endif 
