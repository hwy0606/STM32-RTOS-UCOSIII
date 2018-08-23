#include "stm32f4xx.h"
#include "motor.h"
#include "motor_control.h"
#include "encoder.h" 
#include "math.h"			
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//�������
//����PID����
//����TASK�ж���
//HWY 2018 04 10
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
extern float motor_speed; //����ʵ�ʵ��ת��
u16 speed_comp=30;

struct pid
{
float SetSpeed; //����ת��
float ActualSpeed; //ʵ��ת��
float err; //����ƫ��ֵ
float err_last; //������һ��ƫ��ֵ
float Kp,Ki,Kd; //������������֡�΢��ϵ��
float calculate;  //�������ִ�����ı���
float integral; //�������ֵ
}pid;

void PID_init() //PID�ṹ���ʼ��
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
u16 Motor_control(u16 comp) //����ռ�ձ�   ��������޷�
{
	
	pid.SetSpeed=comp*3000/100;  //�趨ת��
	pid.ActualSpeed=motor_speed;
	pid.err=pid.SetSpeed-pid.ActualSpeed;
  pid.integral=pid.integral+pid.err;
	pid.calculate=pid.Kp*pid.err+pid.Ki*pid.integral+pid.Kd*(pid.err-pid.err_last);//PID����
	pid.err_last=pid.err;
	return (pid.SetSpeed+pid.calculate)*100/3000;	
}

//����ʽPID
//struct pid
//{
//float SetSpeed; //����ת��
//float ActualSpeed; //ʵ��ת��
//float err; //����ƫ��ֵ
//float err_next; //������һ��ƫ��ֵ
//float err_last; //��������ǰ��ƫ��ֵ
//float Kp,Ki,Kd; //������������֡�΢��ϵ��
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
//    pid.SetSpeed=comp*3000/100;  //�趨ת��;
//	  pid.ActualSpeed=motor_speed;
//    pid.err=pid.SetSpeed-pid.ActualSpeed;
//    float incrementSpeed=pid.Kp*(pid.err-pid.err_next)+pid.Ki*pid.err+pid.Kd*(pid.err-2*pid.err_next+pid.err_last);
//    pid.err_last=pid.err_next;
//    pid.err_next=pid.err;
//    return (pid.SetSpeed+incrementSpeed)*100/3000;
//}

