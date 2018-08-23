#include "brake.h"		
#include "usart2_dma.h"	
#include "crc_16.h"
#include "gpio.h"
////////////////////////////////////////////////////////////////////////////////////	 
////STM32F407IGT6
////�ŷ��ƶ������� �������ص�Դ 485��MODBUSЭ�� ����Э����������
////Ӳ����װ��
////HWY 2018 1 2
////All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////// 	 
///*
//����2���ã������� 9600 ����λ 8 У��λ N ֹͣλ 1 
//*/

///*
//��ȡ�Ĵ���:
//��������ͣ� �豸վ��  ����   ��ʼ��ַ   ��Ҫ��ȡ��Ŀ    CRCУ��
//      ���أ� �豸վ��  ����   ���ݴ�С   ��Ч����        CRCУ��
//			
//���Ĵ�����Reg0-4
//���� 0x01 0x03 0x00 0x00 0x00 0x05 0x85 0xC9
//���� 0x01 0x03 0x0A 0x04 0xEB 0x01 0xF1 0x00 0x00 0x00 0xFA 0x01 0xF4 0xDE 0xB2

//(0x04 0xEB) (0x01 0xF1) (0x00 0x00) (0x00 0xFA) (0x01 0xF4)
//Vin = 0x4EB/100 = 12.59V  			Vout = 0x01F1/100 = 4.97V   Iout = 0x0000/100 = 0.00A
//Set CC = 0x00FA/100 =2.50A          Set CV = 0x01F4/100 = 5.00V
//0xDE 0xB2 	CRCУ��
//*/

///*
//���ö���Ĵ�����16
//��������ͣ� �豸վ��  ����   ��ʼ��ַ       N���Ĵ���     �ֽ���  ������   CRCУ��
//      ���أ� �豸վ��  ����   ��ʼ��ַ       N���Ĵ���    CRCУ��
//���Ĵ�����Reg0-4
//���� 0x01 0x10 0x00 0x00 0x00 0x02 0x04 0x00 0xFA 0x01 0xF4 0xD3 0x89
//���� 0x01 0x10 0x00 0x00 0x00 0x02 0x41 0xC8
//(0x00 0xFA) (0x01 0xF4) 
//Set CC = 0x00FA/100 =2.50A          Set CV = 0x01F4/100 = 5.00V
//*/




//���Ĵ���
#define Read_Size 8 
#define Receive_Size 17

u8 Read_Date[Read_Size]; //��������ȫ����ѯ
u8 Receive_Date[Receive_Size]; //��������ȫ����ѯ
u16 Date_len=0 ; //ʵ�ʷ���֡���ݴ�С �˴�ֻ��Ϊ��������ֵ��ѯ

void Read_Brake_Date(void) // ��ȡ��ǰ���� ����ȡ����Ĵ�����ֵ
{
	Read_Date[0]=0x01; //�豸��ַ
	Read_Date[1]=0x03; //�������� ������
	Read_Date[2]=0x00; //��ʼ��ַ��8λ
	Read_Date[3]=0x00; //��ʼ��ַ��8λ
	Read_Date[4]=0x00; //��Ҫ��ȡ��Ŀ��8λ
	Read_Date[5]=0x06; //��Ҫ��ȡ��Ŀ��8λ
	//����CRCУ�� ��8λ�ڵ�8λ˳�������Э���෴ ��Ҳ��֪��Ϊʲô
	usMBCRC16(Read_Date,Read_Size-2);
//	Read_Date[6]=Get_ucCRCLo(); //CRCУ���8λ
//	Read_Date[7]=Get_ucCRCHi(); //CRCУ���8λ
// �ӿ���������ٶ�
		Read_Date[6]=0xC5; //CRCУ���8λ
	  Read_Date[7]=0xC8; //CRCУ���8λ
	
	
	//��������2 DMA����

	USART2_DMA_Send_Once_Data(Read_Date,Read_Size);	
  	

	//������ɺ��ȡ����2�Ľ������� ����Receive_Date������ 
  //Ȼ��Receive_Date�����е�Ԫ�ظ�ֵ
	  //����д��usart2_dma�ļ���
}

//���üĴ�������
#define Write_Size 13
u8 Write_Date[Write_Size]; //��������

void Write_Brake_Date(u16 Brake_Write_Set_Vout,u16 Brake_Write_Set_Iout)
{
	Write_Date[0]=0x01; //�豸��ַ
	Write_Date[1]=0x10; //�������� д����
	Write_Date[2]=0x00; //��ʼ��ַ��8λ
	Write_Date[3]=0x00; //��ʼ��ַ��8λ
	Write_Date[4]=0x00; //д��Ĵ�����Ŀ��8λ
	Write_Date[5]=0x02; //д��Ĵ�����Ŀ��8λ
	Write_Date[6]=0x04; //д���ֽ���
	
	Write_Date[7]=(u8)((Brake_Write_Set_Iout>>8)&0xFF); //д���趨������8λ
	Write_Date[8]=(u8)((Brake_Write_Set_Iout)&0xFF); //д���趨������8λ
	Write_Date[9]=(u8)((Brake_Write_Set_Vout>>8)&0xFF); //д���趨��ѹ��8λ
	Write_Date[10]=(u8)((Brake_Write_Set_Vout)&0xFF); //д���趨��ѹ��8λ
	//����CRCУ��
	usMBCRC16(Write_Date,Write_Size-2);
	Write_Date[11]=Get_ucCRCLo(); //CRCУ���8λ
	Write_Date[12]=Get_ucCRCHi(); //CRCУ���8λ
	
	//��������2 DMA����
//	EN=1; //����ʹ�ܶ� ����ģʽ	
	USART2_DMA_Send_Once_Data(Write_Date,Write_Size);	
	
	//��У�������Ϣ��
}


#define Stauts_Size 8
u8 Status_Date[Stauts_Size]; //״̬����


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
	
	//��������2 DMA����
//	EN=1; //����ʹ�ܶ� ����ģʽ	
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

//������� �����趨��ѹ����ֵ

void Set_Brake(int Vout,int Iout)  //������ѹ�ֱ����100
{
	u16 Brake_Write_Set_Vout=(u16)(Vout);
	u16 Brake_Write_Set_Iout=(u16)(Iout);
	Write_Brake_Date(Brake_Write_Set_Vout,Brake_Write_Set_Iout);
}