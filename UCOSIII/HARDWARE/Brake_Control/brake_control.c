#include "stm32f4xx.h"

#include "brake_control.h"

//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407VGT6
//�ƶ�������
//HWY 2018 04 10
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

void Brake_mode (u8 mode) //0-12 ���������� ��������ɴŷ��ƶ������� �����ŷ��ƶ���ʱ�������ֵ����Ҫʵ����������ֵ
{
	switch (mode){
		case 0: Set_Brake(0,200);
		case 1: Set_Brake(100,200);
		case 2: Set_Brake(200,200);
    case 3: Set_Brake(300,200);
    case 4: Set_Brake(400,200);
    case 5:	Set_Brake(500,200);
		case 6:	Set_Brake(600,200);
		case 7:	Set_Brake(700,200);
		case 8:	Set_Brake(800,200);
		case 9:	Set_Brake(900,200);
		case 10:	Set_Brake(1000,200);
		case 11:	Set_Brake(1100,200);
		case 12:	Set_Brake(1200,200);			
		}			
}
