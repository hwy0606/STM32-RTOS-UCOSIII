#include "stm32f4xx.h"

#include "Mechanical_Data.h"
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//ģ�ͻ�е����
//����ϵͳ�����ȵļ���
//HWY 2017 11 6
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
//�ṹ�����궨����Mechanical_Data.h�ļ���


//�����ȼ���
float deceleration_ratio=9.78; //������ٱ�
float motor_brake_transmission_ratio=motor_diamete/brake_diameter_inner; //���-�ƶ��������� 1
float brake_footboard_transmission_ratio=footboard_diameter/brake_diameter_outer;//�ƶ���-̤�崫���� 6


float Get_transmission_ratio_system(void)
{
	return deceleration_ratio*motor_brake_transmission_ratio*brake_footboard_transmission_ratio;
}
