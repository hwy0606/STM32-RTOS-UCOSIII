//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//USART+DMA���ͺͽ���
//���� PA9 TX PA10 RX
//Э���
//HWY 2017 10 18
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#include "usart1_dma.h"
#include "sys.h" 
#include <string.h> 	  
	

/* UART GPIO define. */            //�궨��
#define USART1_GPIO_TX       		   GPIO_Pin_9
#define USART1_GPIO_RX       		   GPIO_Pin_10
#define USART1_GPIO_TX_PinSource       GPIO_PinSource9
#define USART1_GPIO_RX_PinSource       GPIO_PinSource10
#define USART1_GPIO          		   GPIOA
#define USART1_GPIO_RCC      		   RCC_AHB1Periph_GPIOA
#define USART1_DATA_LEN  64  //���պͷ������ݵ���󳤶�



/*private*/
u8 USART1_SEND_DATA[USART1_DATA_LEN];  // ��������
u8 USART1_RECEIVE_DATA[USART1_DATA_LEN]; //��������
u8 USART1_TX_BUSY=0; //0������ 1:���ڷ���
/*public*/
struct uart1_buffer uart1_rx,uart1_tx;
	  

/**
* @Description: USART1_DMA��ʼ��
* @param baud - ���ڲ�����.    
*/ 
void USART1_DMA_Init(u32 baud) 
{  
  
    NVIC_InitTypeDef NVIC_InitStructure ;  	   		//�����жϽṹ��        
    GPIO_InitTypeDef GPIO_InitStructure;  			//����IO��ʼ���ṹ��       
    USART_InitTypeDef USART_InitStructure;    		//���崮�ڽṹ��    
    DMA_InitTypeDef DMA_InitStructure;  	   		//����DMA�ṹ��  
  
/* ��1��������IO���� */       
    RCC_AHB1PeriphClockCmd(USART1_GPIO_RCC, ENABLE);   
    GPIO_PinAFConfig(USART1_GPIO,USART1_GPIO_RX_PinSource,GPIO_AF_USART1);    
    GPIO_PinAFConfig(USART1_GPIO,USART1_GPIO_TX_PinSource,GPIO_AF_USART1);  
  
    /* USART Tx��GPIO����Ϊ���츴��ģʽ */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //�ܽ�ģʽ:�����       
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //����:����ģʽ       
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //������������    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //IO���ٶ�       
    GPIO_InitStructure.GPIO_Pin = USART1_GPIO_TX;  		   //�ܽ�ָ��     
    GPIO_Init(USART1_GPIO, &GPIO_InitStructure);  		   //��ʼ�� 
      
    /* ��USART1 Rx��GPIO����Ϊ��������ģʽ  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //�ܽ�ģʽ:�����    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //������������         
 	  GPIO_InitStructure.GPIO_Pin = USART1_GPIO_RX;  	   //�ܽ�ָ��     
    GPIO_Init(USART1_GPIO, &GPIO_InitStructure);  		   //��ʼ��
		
		
		

/* ��2�����������ڲ������� */   
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);  	  //�򿪴��ڶ�Ӧ������ʱ��  
    /* ��ʼ�����ڲ��� */   
    USART_InitStructure.USART_BaudRate = baud;                   //������
	  USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������    
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;  //�շ�ģʽ              
    USART_Init(USART1,&USART_InitStructure);    				  //��ʼ������  
      
    /* �����ж����� */  
    USART_ITConfig(USART1,USART_IT_TC,DISABLE);  
    USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);  
    USART_ITConfig(USART1,USART_IT_TXE,DISABLE);  
    USART_ITConfig(USART1,USART_IT_IDLE,ENABLE);     		  	  //�򿪿����ж�
       
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;             //ͨ������Ϊ�����ж�    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //�ж�ռ�ȵȼ�  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;         //�ж���Ӧ���ȼ�   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //���ж�    
    NVIC_Init(&NVIC_InitStructure);     
          
    /* Enable USART1 DMA Rx Tx request */            //ʹ�ܴ����շ�
    USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);  
    USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);  
    
    /* �������� */   
    USART_Cmd(USART1, ENABLE);      
	
	
/* ��3�������ڷ���DMA���� */ 	 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  		//����DMA2ʱ�� 
    /* DMA�����ж����� */ 
		
		
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;  		
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  		
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  			
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  				
    NVIC_Init(&NVIC_InitStructure);  								
     
		 
		 
    DMA_DeInit(DMA2_Stream7);  									//DMAͨ������  DMA2������7ͨ��4
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;   				   
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR); //�����ַ      
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART1_SEND_DATA; //�ڴ��ַ     
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  			//dma���䷽��  �ڴ浽����   
    DMA_InitStructure.DMA_BufferSize = USART1_DATA_LEN;  				//����DMA�ڴ���ʱ�������ĳ���     
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  	//����DMA���������ģʽ��һ������  ������    
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			//����DMA���ڴ����ģʽ    �ڴ治����
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���������ֳ�      
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //�ڴ������ֳ�      
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  						//����DMA�Ĵ���ģʽ    
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  				//����DMA�����ȼ���  �߼���          
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      	    //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ       
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;   //ָ����FIFO��ֵˮƽ              
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //ָ����Burstת�������ڴ洫��    ���δ���    
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //ָ����Burstת��������Χת��    ���δ���
    DMA_Init(DMA2_Stream7, &DMA_InitStructure);    						//����DMA2��ͨ��      
    DMA_ITConfig(DMA2_Stream7,DMA_IT_TC,ENABLE);     					//ʹ���ж� 

/* ��3�������ڽ���DMA���� */     
     																    //����DMAʱ��
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);  
     
    DMA_DeInit(DMA2_Stream5);  
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;  					 //DMAͨ������     
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);  //�����ַ      
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART1_RECEIVE_DATA;//�ڴ��ַ      
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  			 //dma���䷽��    ���赽�ڴ�
    DMA_InitStructure.DMA_BufferSize = USART1_DATA_LEN;  				 //����DMA�ڴ���ʱ�������ĳ���   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  	 //����DMA���������ģʽ��һ������    
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			 //����DMA���ڴ����ģʽ    
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //���������ֳ�       
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  	 //�ڴ������ֳ�     
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  						 //����DMA�Ĵ���ģʽ  
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  			 //����DMA�����ȼ���         
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      		 //ָ�����FIFOģʽ��ֱ��ģʽ������ָ������ �� ��ʹ��FIFOģʽ       
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;    //ָ����FIFO��ֵˮƽ              
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;          //ָ����Burstת�������ڴ洫��  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  //ָ����Burstת��������Χת��                   
    DMA_Init(DMA2_Stream5, &DMA_InitStructure);    						 //����DMA2��ͨ��  
    
    DMA_Cmd(DMA2_Stream5,ENABLE);  										 //ʹ��ͨ��  
} 

/**
 * @Description:UART1_DMA���ⷢ������
 * @param data - �������ݴ�ŵ�ַ
 * @param size - ���������ֽ���    
 */  
void USART1_DMA_Send_Once_Data(uint8_t *data,uint16_t size)  //�������ݺ���
{  
    /* �ȴ����� */ 
    while (USART1_TX_BUSY);  
    USART1_TX_BUSY = 1;  //������
    /* �������� */ 
    memcpy(USART1_SEND_DATA,data,size);
	/* �ı�datasizeǰ��Ҫ��ֹͨ������ */ 
	DMA_Cmd(DMA2_Stream7,DISABLE);  
    /* ���ô������ݳ���  */
    DMA_SetCurrDataCounter(DMA2_Stream7,size);  
    /* ��DMA,��ʼ���� */ 
    DMA_Cmd(DMA2_Stream7,ENABLE);  
}  

  
/**
* @Description:���ڴ����������ж� 
* @param  buf - ���յ����� 
* @return len - ���յ����ݳ���   
*/   
uint8_t USART1_RX_Finish_IRQ(uint8_t *buf)
{
    uint16_t len = 0;

    USART1->SR;
    USART1->DR; 									//��USART_IT_IDLE��־   
    DMA_Cmd(DMA2_Stream5, DISABLE);				    //�ر�DMA   
    DMA_ClearFlag(DMA2_Stream5, DMA_FLAG_TCIF5);	//�����־λ

    /* ��ý���֡֡�� */
    len = USART1_DATA_LEN - DMA_GetCurrDataCounter(DMA2_Stream5);
    memcpy(buf, USART1_RECEIVE_DATA, len);
    /* ���ô������ݳ��� */
    DMA_SetCurrDataCounter(DMA2_Stream5, USART1_DATA_LEN);
    /* ��DMA */
    DMA_Cmd(DMA2_Stream5, ENABLE);
    return len;

}

/**
 * @Description: USARTʹ��DMA�������жϷ������ 
 */
void DMA2_Stream7_IRQHandler(void) 	  
{  
    if(DMA_GetITStatus(DMA2_Stream7,DMA_IT_TCIF7) != RESET)   
    {  
        /* �����־λ */ 
        DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);  
        /* �ر�DMA */ 
        DMA_Cmd(DMA2_Stream7,DISABLE);  
        /* �򿪷�������ж�,������������ֽ� */ 
        USART_ITConfig(USART1,USART_IT_TC,ENABLE);  
    }  
}  
     
/**
 * @Description: �����жϴ�����   ����ʹ�ñ��ļ������������ڰ����ŵ�stm32f4xx_it.c��
 */ 
void USART1_IRQHandler(void) 		   
{  
      
    /* ��������жϴ��� */ 
    if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)  
    {  
        /* �رշ�������ж� */ 
        USART_ITConfig(USART1,USART_IT_TC,DISABLE);  
        /* ������� */ 
        USART1_TX_BUSY = 0;  
    }   
      
    /* ��������жϴ��� */ 
	if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
	{ 
    	uart1_rx.len = USART1_RX_Finish_IRQ(uart1_rx.buf);
	}  
}  
 






/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/






