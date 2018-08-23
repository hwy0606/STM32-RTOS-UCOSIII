#include "adc.h"
#include "delay.h"		 
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//PA5 DMA-AD采样驱动 VREF+采用板载的3.3V
//引脚 PA5
//结果存在adc2_value数组中
//HWY 2017 10 20
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#define ADC2_BUFx_LEN		      7
#define ADC2_BUF_LEN         1*ADC2_BUFx_LEN 		

unsigned short int adc2_value[ADC2_BUF_LEN];  //结果数组


void Adc_Init(void)
{
	    
    //采样脚IO口设置     
    GPIO_InitTypeDef GPIO_InitStructure;  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   		//初始化GPIOA时钟 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 			//管脚指定
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;        			//管脚模式:输入口  模拟输入  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP  ;   				//上拉下拉设置      
   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

	
	
    //DMA设置  		
    DMA_InitTypeDef DMA_InitStructure;	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);   					//启动DMA时钟       															   	
    DMA_DeInit(DMA2_Stream3);  											   //DMA通道配置   DMA2数据流3通道1
    DMA_InitStructure.DMA_Channel = DMA_Channel_1;   
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC2->DR); 	   //外设地址 
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)adc2_value; //内存地址   
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  				//dma传输方向  外设到内存
    DMA_InitStructure.DMA_BufferSize = ADC2_BUF_LEN;  						//设置DMA在传输时缓冲区的长度  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 		//设置DMA的外设递增模式，一个外设   
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    				//设置DMA的内存递增模式 数组自增 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //外设数据字长   
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;     //内存数据字长 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  						//设置DMA的传输模式  循环传输  
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;    	    			//设置DMA的优先级别   
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      			//指定如果FIFO模式或直接模式将用于指定的流:不使能FIFO模式     
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  		//指定了FIFO阈值水平           
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 			//指定的Burst转移配置内存传输          
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 	//指定的Burst转移配置外围转移              
    DMA_Init(DMA2_Stream3, &DMA_InitStructure); 							//配置DMA的通道       
    DMA_Cmd(DMA2_Stream3, ENABLE);   										//使能通道       
  
    //ADC配置      
		  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);   					//启动ADC时钟  
    ADC_DeInit();  //初始化ADC  复位时钟 
    //ADC通用配置  

    ADC_CommonInitTypeDef ADC_CommonInitStructure; 
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;         //DMA模式     
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  				 //独立工作模式  
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;  			 //4分频 确保ADC1时钟为21MHz
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //两次采样间隔   	5延迟周期数
    ADC_CommonInit(&ADC_CommonInitStructure);     
  
	
	  ADC_InitTypeDef ADC_InitStructure;    
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  				  //精度为12位        
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;     						  //扫描方式     
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    					  //连续转换    
   	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//外部触发禁止，软件触发
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;    				  //数据右对齐   
    ADC_InitStructure.ADC_NbrOfConversion = 1;    				  //1个转换在规则序列中
    ADC_Init(ADC2, &ADC_InitStructure);    
    ADC_Cmd(ADC2, ENABLE);    												 //使能ADC2
    //规则模式通道配置 
		ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 1, ADC_SampleTime_480Cycles);    //采样周期设置 112？480？
    //使能ADC的DMA    
		ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
		ADC_DMACmd(ADC2, ENABLE);              
    //开启ADC2的软件转换  
		ADC_SoftwareStartConv(ADC2);      
}
  

void Sensor_Current_Init(void)//电流传感器初始化
{	 
		Adc_Init();
}
		  
u16 Average()
{
	u16 sum=0;
	u16 i;
	for(i=0;i<ADC2_BUF_LEN;i++)	
	{
		sum+=adc2_value[i];
	}
	sum/=(ADC2_BUFx_LEN);//    取平均值，具体DEBUG调试过程验证     
	return sum;	
}

//采样均值滤波，后期可根据实际需要优化滤波算法





