//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//USART+DMA发送和接收
//引脚 PA2 TX PA3 RX
//协议层
//HWY 2017 10 18
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#include "usart2_dma.h"
#include "sys.h" 
#include <string.h> 	  
#include "gpio.h"	
#include "delay.h"

/* UART GPIO define. */
#define USART2_GPIO_TX       		   GPIO_Pin_2
#define USART2_GPIO_RX       		   GPIO_Pin_3
#define USART2_GPIO_TX_PinSource   GPIO_PinSource2
#define USART2_GPIO_RX_PinSource   GPIO_PinSource3
#define USART2_GPIO          		   GPIOA
#define USART2_GPIO_RCC      		   RCC_AHB1Periph_GPIOA


#define USART2_DATA_LEN  64  //接收和发送数据的最大长度
/*private*/
u8 USART2_SEND_DATA[USART2_DATA_LEN];  
u8 USART2_RECEIVE_DATA[USART2_DATA_LEN]; 
u8 USART2_TX_BUSY=0; //0：空闲 1:正在发送
/*public*/
struct uart2_buffer uart2_rx,uart2_tx;
	  

/**
* @Description: UART2_DMA初始化
* @param baud - 串口波特率.    
*/ 
void USART2_DMA_Init(u32 baud) 
{  
  
    NVIC_InitTypeDef NVIC_InitStructure ;  	   		//定义中断结构体        
    GPIO_InitTypeDef GPIO_InitStructure;  			//定义IO初始化结构体       
    USART_InitTypeDef USART_InitStructure;    		//定义串口结构体    
    DMA_InitTypeDef DMA_InitStructure;  	   		//定义DMA结构体  
  
/* 第1步：串口IO设置 */       
    RCC_AHB1PeriphClockCmd(USART2_GPIO_RCC, ENABLE);   
    GPIO_PinAFConfig(USART2_GPIO,USART2_GPIO_RX_PinSource,GPIO_AF_USART2);    
    GPIO_PinAFConfig(USART2_GPIO,USART2_GPIO_TX_PinSource,GPIO_AF_USART2);  
  
    /* USART Tx的GPIO配置为推挽复用模式 */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //管脚模式:输出口       
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;     //类型:推挽模式       
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;       //上拉下拉设置    
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; //IO口速度       
    GPIO_InitStructure.GPIO_Pin = USART2_GPIO_TX;  		   //管脚指定     
    GPIO_Init(USART2_GPIO, &GPIO_InitStructure);  		   //初始化 
      
    /* 将USART2 Rx的GPIO配置为浮空输入模式  */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;       //管脚模式:输入口    
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   //上拉下拉设置         
 	GPIO_InitStructure.GPIO_Pin = USART2_GPIO_RX;  	   //管脚指定     
    GPIO_Init(USART2_GPIO, &GPIO_InitStructure);  		   //初始化

/* 第2步：基本串口参数设置 */   

      
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);  	  //打开串口对应的外设时钟  
    /* 初始化串口参数 */   
    USART_InitStructure.USART_BaudRate = baud;  
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;    
    USART_InitStructure.USART_StopBits = USART_StopBits_1;    
    USART_InitStructure.USART_Parity = USART_Parity_No;    
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;    
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;               
    USART_Init(USART2,&USART_InitStructure);    				  //初始化串口  
      
    /* 串口中断配置 */  
    USART_ITConfig(USART2,USART_IT_TC,DISABLE);  
    USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);  
    USART_ITConfig(USART2,USART_IT_TXE,DISABLE);  
    USART_ITConfig(USART2,USART_IT_IDLE,ENABLE);     		  	  //打开空闲中断
       
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;             //通道设置为串口中断    
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;  //中断占先等级  
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;         //中断响应优先级   
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;               //打开中断    
    NVIC_Init(&NVIC_InitStructure);     
          
    /* Enable USART2 DMA Rx Tx request */
    USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);  
    USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);  
    
    /* 启动串口 */   
    USART_Cmd(USART2, ENABLE);      
//	  USART_HalfDuplexCmd(USART2, ENABLE);//使能或者失能USART半双工模式
	
    /* 第3步：串口发送DMA配置 */ 	 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  		//启动DMA时钟 
    /* DMA发送中断设置 */ 
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;  		
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  		
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  			
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  				
    NVIC_Init(&NVIC_InitStructure);  								
     
    DMA_DeInit(DMA1_Stream6);  									//DMA通道配置 
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;   				   
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR); //外设地址      
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2_SEND_DATA; //内存地址     
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;  			//dma传输方向     
    DMA_InitStructure.DMA_BufferSize = USART2_DATA_LEN;  				//设置DMA在传输时缓冲区的长度     
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  	//设置DMA的外设递增模式，一个外设      
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			//设置DMA的内存递增模式     
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据字长      
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte; //内存数据字长      
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  						//设置DMA的传输模式    
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;  				//设置DMA的优先级别          
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      	    //指定如果FIFO模式或直接模式将用于指定的流 ： 不使能FIFO模式       
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;   //指定了FIFO阈值水平              
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //指定的Burst转移配置内存传输      
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //指定的Burst转移配置外围转移               
    DMA_Init(DMA1_Stream6, &DMA_InitStructure);    						//配置DMA2的通道      
    DMA_ITConfig(DMA1_Stream6,DMA_IT_TC,ENABLE);     					//使能中断 

/* 第3步：串口接收DMA配置 */     
     																    //启动DMA时钟
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);  
     
    DMA_DeInit(DMA1_Stream5);  
    DMA_InitStructure.DMA_Channel = DMA_Channel_4;  					 //DMA通道配置     
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART2->DR);  //外设地址      
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)USART2_RECEIVE_DATA;//内存地址      
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  			 //dma传输方向    
    DMA_InitStructure.DMA_BufferSize = USART2_DATA_LEN;  				 //设置DMA在传输时缓冲区的长度   
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  	 //设置DMA的外设递增模式，一个外设    
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;  			 //设置DMA的内存递增模式    
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据字长       
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  	 //内存数据字长     
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  						 //设置DMA的传输模式  
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;  			 //设置DMA的优先级别         
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      		 //指定如果FIFO模式或直接模式将用于指定的流 ： 不使能FIFO模式       
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;    //指定了FIFO阈值水平              
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;          //指定的Burst转移配置内存传输  
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;  //指定的Burst转移配置外围转移                   
    DMA_Init(DMA1_Stream5, &DMA_InitStructure);    						 //配置DMA2的通道  
    
    DMA_Cmd(DMA1_Stream5,ENABLE);  										 //使能通道  
} 

/**
 * @Description:UART1_DMA向外发送数据
 * @param data - 发送数据存放地址
 * @param size - 发送数据字节数    
 */  
void USART2_DMA_Send_Once_Data(uint8_t *data,uint16_t size)  
{  
    /* 等待空闲 */ 
    while (USART2_TX_BUSY);  
    USART2_TX_BUSY = 1;  
    /* 复制数据 */ 
    memcpy(USART2_SEND_DATA,data,size);
	  /* 改变datasize前先要禁止通道工作 */ 
	  DMA_Cmd(DMA1_Stream6,DISABLE);  
    /* 设置传输数据长度  */
    DMA_SetCurrDataCounter(DMA1_Stream6,size);  
    /* 打开DMA,开始发送 */ 
    DMA_Cmd(DMA1_Stream6,ENABLE);  
}  

  
/**
* @Description:串口处理接收完成中断 
* @param  buf - 接收的数据 
* @return len - 接收的数据长度   
*/   
uint8_t USART2_RX_Finish_IRQ(uint8_t *buf)
{
    uint16_t len = 0;

    USART2->SR;
    USART2->DR; 									//清USART_IT_IDLE标志   
    DMA_Cmd(DMA1_Stream5, DISABLE);				    //关闭DMA   
    DMA_ClearFlag(DMA1_Stream5, DMA_FLAG_TCIF5);	//清除标志位

    /* 获得接收帧帧长 */
    len = USART2_DATA_LEN - DMA_GetCurrDataCounter(DMA1_Stream5);
    memcpy(buf, USART2_RECEIVE_DATA, len);
    /* 设置传输数据长度 */
    DMA_SetCurrDataCounter(DMA1_Stream5, USART2_DATA_LEN);
    /* 打开DMA */
    DMA_Cmd(DMA1_Stream5, ENABLE);
    return len;

}

/**
 * @Description: USART使用DMA发数据中断服务程序 
 */
void DMA1_Stream6_IRQHandler(void) 	  
{  
    if(DMA_GetITStatus(DMA1_Stream6,DMA_IT_TCIF6) != RESET)   
    {  
        /* 清除标志位 */ 
        DMA_ClearFlag(DMA1_Stream6,DMA_FLAG_TCIF6);  
        /* 关闭DMA */ 
        DMA_Cmd(DMA1_Stream6,DISABLE);  
        /* 打开发送完成中断,发送最后两个字节 */ 
        USART_ITConfig(USART2,USART_IT_TC,ENABLE);  
    }  
}  
     
/**
 * @Description: 串口中断处理函数   单独使用本文件是用他，现在把他放到stm32f4xx_it.c中
 */ 

#define Receive_Size 17
extern u16 Date_len;
extern u8 Receive_Date[Receive_Size]; //接收数组全部查询

 u16 Brake_Current_Vin; // 当前输入电压 寄存器地址0x00 0x00 
 u16 Brake_Current_Vout; //当前输出电压 寄存器地址0x00 0x01
 u16 Brake_Current_Iout;//当前输出电流  寄存器地址0x00 0x02
 u16 Brake_Set_Iout; //当前设置输出电流 寄存器地址0x00 0x03
 u16 Brake_Set_Vout; //当前设置输出电压 寄存器地址0x00 0x04
 u16 Brake_Status; //电源状态 寄存器地址0x00 0x05  0x01 开启 0x00 关闭


void USART2_IRQHandler(void) 		   
{  
      
    /* 发送完成中断处理 */ 
    if(USART_GetITStatus(USART2, USART_IT_TC)!= RESET)  
    {  
        /* 关闭发送完成中断 */ 
        USART_ITConfig(USART2,USART_IT_TC,DISABLE);  
        /* 发送完成 */ 
        USART2_TX_BUSY = 0; 
			
	
			   
			
			
    }   
      
    /* 接收完成中断处理 */ 
	if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET)
	{ 
//    	uart2_rx.len = USART2_RX_Finish_IRQ(uart2_rx.buf);
		
		
		 Date_len=USART2_RX_Finish_IRQ(Receive_Date);
		//取对应数据赋值
		 Brake_Current_Vin=(unsigned short)(Receive_Date[3]<<8 | Receive_Date[4]);
	   Brake_Current_Vout=(unsigned short)(Receive_Date[5]<<8 | Receive_Date[6]);
	   Brake_Current_Iout=(unsigned short)(Receive_Date[7]<<8 | Receive_Date[8]);
	   Brake_Set_Iout=(unsigned short)(Receive_Date[9]<<8 | Receive_Date[10]);
	   Brake_Set_Vout=(unsigned short)(Receive_Date[11]<<8 | Receive_Date[12]);
		 Brake_Status=(unsigned short)(Receive_Date[13]<<8 | Receive_Date[14]);		
		 
	
	}  
}  
 

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/






