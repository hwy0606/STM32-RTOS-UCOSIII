#include "stm32f4xx.h"
#include "motor.h"
#include "motor_control.h"
#include "encoder.h" 
#include "math.h"			
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//电机控制
//数字PID调速
//放入TASK中断中
//HWY 2018 04 10
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
extern float motor_speed; //导入实际电机转速
u16 speed_comp=30;

struct pid
{
float SetSpeed; //定义转速
float ActualSpeed; //实际转速
float err; //定义偏差值
float err_last; //定义上一个偏差值
float Kp,Ki,Kd; //定义比例、积分、微分系数
float calculate;  //定义控制执行器的变量
float integral; //定义积分值
}pid;

void PID_init() //PID结构体初始化
{
	pid.SetSpeed=0.0;
	pid.ActualSpeed=0.0;
	pid.err=0.0;
	pid.err_last=0.0;
	pid.calculate=0.0;
	pid.integral=0.0;
	pid.Kp=0.3;
	pid.Ki=0.015;
	pid.Kd=0.2;
}
u16 Motor_control(u16 comp) //返回占空比   加入积分限幅
{
	
	pid.SetSpeed=comp*3000/100;  //设定转速
	pid.ActualSpeed=motor_speed;
	pid.err=pid.SetSpeed-pid.ActualSpeed;
  pid.integral=pid.integral+pid.err;
	pid.calculate=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);//PID计算
	pid.err_last=pid.err;
	return (pid.SetSpeed+pid.calculate)*100/3000;	
}

//增量式PID
//struct pid
//{
//float SetSpeed; //定义转速
//float ActualSpeed; //实际转速
//float err; //定义偏差值
//float err_next; //定义上一个偏差值
//float err_last; //定义最上前的偏差值
//float Kp,Ki,Kd; //定义比例、积分、微分系数
//}pid;

//void PID_init(){
//    pid.SetSpeed=0.0;
//    pid.ActualSpeed=0.0;
//    pid.err=0.0;
//    pid.err_last=0.0;
//    pid.err_next=0.0;
//    pid.Kp=0.2;
//    pid.Ki=0.015;
//    pid.Kd=0.2;
//}


//u16 Motor_control(u16 comp){
//    pid.SetSpeed=comp*3000/100;  //设定转速;
//	  pid.ActualSpeed=motor_speed;
//    pid.err=pid.SetSpeed-pid.ActualSpeed;
//    float incrementSpeed=pid.Kp*(pid.err-pid.err_next)+pid.Ki*pid.err+pid.Kd*(pid.err-2*pid.err_next+pid.err_last);
//    pid.err_last=pid.err_next;
//    pid.err_next=pid.err;
//    return (pid.SetSpeed+incrementSpeed)*100/3000;
//}

