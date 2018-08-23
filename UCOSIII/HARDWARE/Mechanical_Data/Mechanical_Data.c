#include "stm32f4xx.h"

#include "Mechanical_Data.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//模型机械参数
//用于系统传动比的计算
//HWY 2017 11 6
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
//结构参数宏定义在Mechanical_Data.h文件中


//传动比计算
float deceleration_ratio=9.78; //电机减速比
float motor_brake_transmission_ratio=motor_diamete/brake_diameter_inner; //电机-制动器传动比 1
float brake_footboard_transmission_ratio=footboard_diameter/brake_diameter_outer;//制动器-踏板传动比 6


float Get_transmission_ratio_system(void)
{
	return deceleration_ratio*motor_brake_transmission_ratio*brake_footboard_transmission_ratio;
}
