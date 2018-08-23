#ifndef __USART1_DMA_H
#define __USART1_DMA_H
#include "sys.h"

struct uart1_buffer
{
	unsigned char  len;   //数据长度
	unsigned char  buf[128];  //接收数据缓存区
};
extern struct uart1_buffer uart1_rx,uart1_tx;

extern u8 USART1_TX_BUSY; //0：空闲 1:正在发送
void USART1_DMA_Init(u32 baud) ;
void USART1_DMA_Send_Once_Data(uint8_t *data,uint16_t size)  ;
uint8_t USART1_RX_Finish_IRQ(uint8_t *buf)  ;
void USART1_Receive_Check(void);//接收检查并处理	 				    
#endif
