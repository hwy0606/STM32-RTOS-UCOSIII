#include "controller.h"		
#include "motor.h"
#include "brake.h"	
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//电机和制动器控制
//增量式PID控制
//调用Motor库函数和Brake库函数
//HWY 2018 1 2
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

/*
Motor库函数说明：
void Motor_Init(void); 初始化
void Motor_Positive(u16 comp) ;电机正转 占空比comp 0-98
void Motor_Negative(u16 comp) ;电机反转 占空比comp 0-98
*/

/*
Brake库函数说明：
void Brake_Init() ; 初始化
Brake_Controller(u16 comp) ; 磁粉制动器控制 电流大小为2A*comp
*/








