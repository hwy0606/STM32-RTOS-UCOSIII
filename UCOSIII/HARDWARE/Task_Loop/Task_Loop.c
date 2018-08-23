//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//上电后任务机循环函数，周期由Task_Timer函数控制
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
float angle_motor;	//角度测量值电机端
float angle_footboard; //角度换算到踏板侧

extern u16 Brake_Current_Vout; //当前输出电压 寄存器地址0x00 0x01
extern u16 Brake_Current_Iout;//当前输出电流  寄存器地址0x00 0x02



void Angle_Calculate(void)  //计算电机角度  motor_speed单位n/min
{
	angle_motor=angle_motor+motor_speed*360/60*0.01;   //10ms=0.01s一个计数周期 
  	
}

void Angle_Transmission(void)
{
	angle_footboard=angle_motor/(Get_transmission_ratio_system());  //转换到踏板处的角度值 实际输出在0-360度 每次通过原点清0
}

void Task_Loop(void)   //任务循环
{
  Send_Date[0]=0x5A;   //0x5A 开头
	temp16=(u16)(motor_speed);
	//发送电机采集速度 十六进制格式 单位n/min 0xFFFF=65535 
//	Send_Date[1]=(unsigned char)((temp16 & 0xFF000000)>>24);
//	Send_Date[2]=(unsigned char)((temp16 & 0x00FF0000)>>16);
	Send_Date[1]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[2]=temp16&0x000000FF;
	
	//发送电机电流  十六进制发送采样结果 不超过0x0FFF 电压=采样结果*3.3/4096	
	temp16=(u16)(Average());
	Send_Date[3]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[4]=temp16&0x000000FF;
	
	//发送光电开关计数值  十六进制发送采样结果 不超过0xFF 
	temp16=(u16)(Get_EXTI_LINE2_CAPTURE_VAL());
	Send_Date[5]=temp16&0x000000FF; 
	
	//发送角度值,电机的角度 十六进制发送结果 单位°
	temp16=(u16)(angle_motor);
//	Send_Date[6]=(unsigned char)((temp16 & 0xFF000000)>>24);
//	Send_Date[7]=(unsigned char)((temp16 & 0x00FF0000)>>16);
	Send_Date[6]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[7]=temp16&0x000000FF;
	//发送角度值,踏板的角度 十六进制发送结果 单位°
	temp16=(u16)(angle_footboard);
	Send_Date[8]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[9]=temp16&0x000000FF;
	
	//发送制动器相关信息
	temp16=Brake_Current_Vout;
	Send_Date[10]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[11]=temp16&0x000000FF;
	temp16=Brake_Current_Iout;
	Send_Date[12]=(unsigned char)((temp16 & 0x0000FF00)>>8);
	Send_Date[13]=temp16&0x000000FF;
	
	Send_Date[14]=0x0a; //0x0a结尾

  
	USART1_DMA_Send_Once_Data(Send_Date,Send_Size);		
}


