#ifndef __BRAKE_H
#define __BRAKE_H

#include "stm32f4xx.h"

void Read_Brake_Date(void);
void Write_Brake_Date(u16 Brake_Write_Set_Vout,u16 Brake_Write_Set_Iout);
void Turn_Off_Brake(void);
void Turn_On_Brake(void);
void Set_Brake(int Vout,int Iout);
#endif     
