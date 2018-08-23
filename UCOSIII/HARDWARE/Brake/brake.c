#include "brake.h"		
#include "usart2_dma.h"	
#include "crc_16.h"
#include "gpio.h"
////////////////////////////////////////////////////////////////////////////////////	 
////STM32F407IGT6
////磁粉制动器控制 采用数控电源 485的MODBUS协议 具体协议内容如下
////硬件封装层
////HWY 2018 1 2
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 	 
///*
//串口2设置：波特率 9600 数据位 8 校验位 N 停止位 1 
//*/

///*
//读取寄存器:
//计算机发送： 设备站号  命令   开始地址   需要读取数目    CRC校验
//      返回： 设备站号  命令   数据大小   有效数据        CRC校验
//			
//读寄存器：Reg0-4
//发出 0x01 0x03 0x00 0x00 0x00 0x05 0x85 0xC9
//接收 0x01 0x03 0x0A 0x04 0xEB 0x01 0xF1 0x00 0x00 0x00 0xFA 0x01 0xF4 0xDE 0xB2

//(0x04 0xEB) (0x01 0xF1) (0x00 0x00) (0x00 0xFA) (0x01 0xF4)
//Vin = 0x4EB/100 = 12.59V  			Vout = 0x01F1/100 = 4.97V   Iout = 0x0000/100 = 0.00A
//Set CC = 0x00FA/100 =2.50A          Set CV = 0x01F4/100 = 5.00V
//0xDE 0xB2 	CRC校验
//*/

///*
//设置多个寄存器：16
//计算机发送： 设备站号  命令   开始地址       N个寄存器     字节数  　数据   CRC校验
//      返回： 设备站号  命令   开始地址       N个寄存器    CRC校验
//读寄存器：Reg0-4
//发出 0x01 0x10 0x00 0x00 0x00 0x02 0x04 0x00 0xFA 0x01 0xF4 0xD3 0x89
//接收 0x01 0x10 0x00 0x00 0x00 0x02 0x41 0xC8
//(0x00 0xFA) (0x01 0xF4) 
//Set CC = 0x00FA/100 =2.50A          Set CV = 0x01F4/100 = 5.00V
//*/




//读寄存器
#define Read_Size 8 
#define Receive_Size 17

u8 Read_Date[Read_Size]; //发送数组全部查询
u8 Receive_Date[Receive_Size]; //接收数组全部查询
u16 Date_len=0 ; //实际返回帧数据大小 此处只作为函数返回值查询

void Read_Brake_Date(void) // 获取当前设置 即读取五个寄存器的值
{
	Read_Date[0]=0x01; //设备地址
	Read_Date[1]=0x03; //操作命令 读操作
	Read_Date[2]=0x00; //开始地址高8位
	Read_Date[3]=0x00; //开始地址低8位
	Read_Date[4]=0x00; //需要读取数目高8位
	Read_Date[5]=0x06; //需要读取数目低8位
	//启动CRC校验 高8位于低8位顺序与给的协议相反 我也不知道为什么
	usMBCRC16(Read_Date,Read_Size-2);
//	Read_Date[6]=Get_ucCRCLo(); //CRC校验高8位
//	Read_Date[7]=Get_ucCRCHi(); //CRC校验低8位
// 加快程序运行速度
		Read_Date[6]=0xC5; //CRC校验高8位
	  Read_Date[7]=0xC8; //CRC校验低8位
	
	
	//启动串口2 DMA传输

	USART2_DMA_Send_Once_Data(Read_Date,Read_Size);	
  	

	//发送完成后读取串口2的接收数据 存入Receive_Date数组中 
  //然后将Receive_Date数组中的元素赋值
	  //函数写在usart2_dma文件中
}

//设置寄存器函数
#define Write_Size 13
u8 Write_Date[Write_Size]; //发送数组

void Write_Brake_Date(u16 Brake_Write_Set_Vout,u16 Brake_Write_Set_Iout)
{
	Write_Date[0]=0x01; //设备地址
	Write_Date[1]=0x10; //操作命令 写操作
	Write_Date[2]=0x00; //开始地址高8位
	Write_Date[3]=0x00; //开始地址低8位
	Write_Date[4]=0x00; //写入寄存器数目高8位
	Write_Date[5]=0x02; //写入寄存器数目低8位
	Write_Date[6]=0x04; //写入字节数
	
	Write_Date[7]=(u8)((Brake_Write_Set_Iout>>8)&0xFF); //写入设定电流低8位
	Write_Date[8]=(u8)((Brake_Write_Set_Iout)&0xFF); //写入设定电流高8位
	Write_Date[9]=(u8)((Brake_Write_Set_Vout>>8)&0xFF); //写入设定电压低8位
	Write_Date[10]=(u8)((Brake_Write_Set_Vout)&0xFF); //写入设定电压高8位
	//启动CRC校验
	usMBCRC16(Write_Date,Write_Size-2);
	Write_Date[11]=Get_ucCRCLo(); //CRC校验高8位
	Write_Date[12]=Get_ucCRCHi(); //CRC校验低8位
	
	//启动串口2 DMA传输
//	EN=1; //拉高使能端 发送模式	
	USART2_DMA_Send_Once_Data(Write_Date,Write_Size);	
	
	//不校验输出信息了
}


#define Stauts_Size 8
u8 Status_Date[Stauts_Size]; //状态数组


void Turn_Off_Brake()
{
	Status_Date[0]=0x01; 
	Status_Date[1]=0x06; 
	Status_Date[2]=0x00; 
	Status_Date[3]=0x02; 
	Status_Date[4]=0x00; 
	Status_Date[5]=0x00; 
	Status_Date[6]=0x28; 
	Status_Date[7]=0x0A; 
	
	//启动串口2 DMA传输
//	EN=1; //拉高使能端 发送模式	
	USART2_DMA_Send_Once_Data(Status_Date,Stauts_Size);	
}

void Turn_On_Brake()
{
	Status_Date[0]=0x01; 
	Status_Date[1]=0x06; 
	Status_Date[2]=0x00; 
	Status_Date[3]=0x02; 
	Status_Date[4]=0x00; 
	Status_Date[5]=0x01; 
	Status_Date[6]=0xE9; 
	Status_Date[7]=0xCA; 
	
	USART2_DMA_Send_Once_Data(Status_Date,Stauts_Size);	
}

//方便操作 方便设定电压电流值

void Set_Brake(int Vout,int Iout)  //电流电压分别乘以100
{
	u16 Brake_Write_Set_Vout=(u16)(Vout);
	u16 Brake_Write_Set_Iout=(u16)(Iout);
	Write_Brake_Date(Brake_Write_Set_Vout,Brake_Write_Set_Iout);
}