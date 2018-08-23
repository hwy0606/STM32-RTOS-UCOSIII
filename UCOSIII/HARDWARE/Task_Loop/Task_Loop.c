//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//�ϵ�������ѭ��������������Task_Timer��������
//HWY 2017 10 23
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#include "stm32f4xx.h"
#include "Task_Loop.h" 	
#include "usart1_dma.h"
#include "usart2_dma.h"
#include "encoder.h"
#include "adc.h"
#include "sys.h"
#include "delay.h"
#include "Omron_Counter.h"
#include "Task_Circle.h"
#include "Mechanical_Data.h"
#include "brake.h"
#define Send_Size 15



#include "delay.h"
unsigned char Send_Date[Send_Size];

u16 temp16;
extern float motor_speed;
float angle_motor;	//�ǶȲ���ֵ�����
float angle_footboard; //�ǶȻ��㵽̤���

extern u16 Brake_Current_Vout; //��ǰ�����ѹ �Ĵ�����ַ0x00 0x01
extern u16 Brake_Current_Iout;//��ǰ�������  �Ĵ�����ַ0x00 0x02



void Angle_Calculate(void)  //�������Ƕ�  motor_speed��λn/min
{
	angle_motor=angle_motor+motor_speed*360/60*0.01;   //10ms=0.01sһ���������� 
  	
}

void Angle_Transmission(void)
{
	angle_footboard=angle_motor/(Get_transmission_ratio_system());  //ת����̤�崦�ĽǶ�ֵ ʵ�������0-360�� ÿ��ͨ��ԭ����0
}

void Task_Loop(void)   //����ѭ��
{
  Send_Date[0]=0x5A;   //0x5A ��ͷ
	temp16=(u16)(motor_speed);
	//���͵���ɼ��ٶ� ʮ�����Ƹ�ʽ ��λn/min 0xFFFF=65535 
//	Send_Date[1]=(unsigned char)((temp16 & 0xFF000000)>>24);
//	Send_Date[2]=(unsigned char)((temp16 & 0x00FF0000)>>16);
	Send_Date[1]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[2]=temp16&0x000000FF;
	
	//���͵������  ʮ�����Ʒ��Ͳ������ ������0x0FFF ��ѹ=�������*3.3/4096	
	temp16=(u16)(Average());
	Send_Date[3]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[4]=temp16&0x000000FF;
	
	//���͹�翪�ؼ���ֵ  ʮ�����Ʒ��Ͳ������ ������0xFF 
	temp16=(u16)(Get_EXTI_LINE2_CAPTURE_VAL());
	Send_Date[5]=temp16&0x000000FF; 
	
	//���ͽǶ�ֵ,����ĽǶ� ʮ�����Ʒ��ͽ�� ��λ��
	temp16=(u16)(angle_motor);
//	Send_Date[6]=(unsigned char)((temp16 & 0xFF000000)>>24);
//	Send_Date[7]=(unsigned char)((temp16 & 0x00FF0000)>>16);
	Send_Date[6]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[7]=temp16&0x000000FF;
	//���ͽǶ�ֵ,̤��ĽǶ� ʮ�����Ʒ��ͽ�� ��λ��
	temp16=(u16)(angle_footboard);
	Send_Date[8]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[9]=temp16&0x000000FF;
	
	//�����ƶ��������Ϣ
	temp16=Brake_Current_Vout;
	Send_Date[10]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[11]=temp16&0x000000FF;
	temp16=Brake_Current_Iout;
	Send_Date[12]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[13]=temp16&0x000000FF;
	
	Send_Date[14]=0x0a; //0x0a��β

  
	USART1_DMA_Send_Once_Data(Send_Date,Send_Size);		
}


