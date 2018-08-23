#ifndef __MECHANICAL_DATA_H
#define __MECHANICAL_DATA_H
#include "sys.h"

//机械参数定义  单位mm

#define footboard_diameter  240    //踏板直径
#define brake_diameter_outer 40   //制动器外直径 小
#define brake_diameter_inner  50  //制动器内直径 大
#define motor_diamete        50  //电机直径

extern float deceleration_ratio; //电机减速比
extern float motor_brake_transmission_ratio; //电机-制动器传动比
extern float brake_footboard_transmission_ratio;//制动器-踏板传动比

float Get_transmission_ratio_system(void); //系统的总传动比

#endif  