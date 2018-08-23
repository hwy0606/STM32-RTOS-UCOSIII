#ifndef __USART2_DMA_H
#define __USART2_DMA_H
#include "sys.h"

struct uart2_buffer
{
	unsigned char  len;   //数据长度
	unsigned char  buf[128];  //接收数据缓存区
};
extern struct uart2_buffer uart2_rx,uart2_tx;

extern u8 USART2_TX_BUSY; //0：空闲 1:正在发送
void USART2_DMA_Init(u32 baud) ;
void USART2_DMA_Send_Once_Data(uint8_t *data,uint16_t size)  ;
uint8_t USART2_RX_Finish_IRQ(uint8_t *buf)  ;
void USART2_Receive_Check(void);//接收检查并处理	 				    
#endif
