#include "crc_16.h"
#include "delay.h"		 
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//CRC-16 查表法校验 速度快，不占资源 
//每次查询完 到原始状态
//HWY 2017 12 29
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
u8 ucCRCHi;
u8 ucCRCLo;
//*pucFrame 待校验数据首地址,usLen 待校验数据长度 返回校验结果 u16
void usMBCRC16( u8 * pucFrame, u8 usLen )

{
    int iIndex;
    ucCRCHi=0xFF ;
    ucCRCLo=0XFF ;
    while( usLen-- )

    {

        iIndex = ucCRCLo ^ *( pucFrame++ );

        ucCRCLo = ( u8 )( ucCRCHi ^ aucCRCHi[iIndex] );

        ucCRCHi = aucCRCLo[iIndex];
			
			

    }
}
u8 Get_ucCRCHi()//获取高8位
{
	return ucCRCHi;
}

u8 Get_ucCRCLo()//获取低8位
{
	return ucCRCLo;
}










