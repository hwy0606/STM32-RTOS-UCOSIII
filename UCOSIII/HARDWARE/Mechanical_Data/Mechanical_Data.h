#ifndef __MECHANICAL_DATA_H
#define __MECHANICAL_DATA_H
#include "sys.h"

//��е��������  �m��λmm

#define footboard_diameter  240    //̤��ֱ��
#define brake_diameter_outer 40   //�ƶ�����ֱ�� С
#define brake_diameter_inner  50  //�ƶ�����ֱ�� ��
#define motor_diamete        50  //���ֱ��

extern float deceleration_ratio; //������ٱ�
extern float motor_brake_transmission_ratio; //���-�ƶ���������
extern float brake_footboard_transmission_ratio;//�ƶ���-̤�崫����

float Get_transmission_ratio_system(void); //ϵͳ���ܴ�����

#endif  