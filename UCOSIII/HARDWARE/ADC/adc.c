#include "adc.h"
#include "delay.h"		 
//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//PA5 DMA-AD�������� VREF+���ð��ص�3.3V
//���� PA5
//�������adc2_value������
//HWY 2017 10 20
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 

#define ADC2_BUFx_LEN		      7
#define ADC2_BUF_LEN         1*ADC2_BUFx_LEN 		

unsigned short int adc2_value[ADC2_BUF_LEN];  //�������


void Adc_Init(void)
{
	    
    //������IO������     
    GPIO_InitTypeDef GPIO_InitStructure;  
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);   		//��ʼ��GPIOAʱ�� 	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; 			//�ܽ�ָ��
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;        			//�ܽ�ģʽ:�����  ģ������  
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP  ;   				//������������      
   	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  

	
	
    //DMA����  		
    DMA_InitTypeDef DMA_InitStructure;	
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);   					//����DMAʱ��       															   	
    DMA_DeInit(DMA2_Stream3);  											   //DMAͨ������   DMA2������3ͨ��1
    DMA_InitStructure.DMA_Channel = DMA_Channel_1;   
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&ADC2->DR); 	   //�����ַ 
    DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)adc2_value; //�ڴ��ַ   
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;  				//dma���䷽��  ���赽�ڴ�
    DMA_InitStructure.DMA_BufferSize = ADC2_BUF_LEN;  						//����DMA�ڴ���ʱ�������ĳ���  
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 		//����DMA���������ģʽ��һ������   
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;    				//����DMA���ڴ����ģʽ �������� 
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���������ֳ�   
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;     //�ڴ������ֳ� 
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;  						//����DMA�Ĵ���ģʽ  ѭ������  
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;    	    			//����DMA�����ȼ���   
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;      			//ָ�����FIFOģʽ��ֱ��ģʽ������ָ������:��ʹ��FIFOģʽ     
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;  		//ָ����FIFO��ֵˮƽ           
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single; 			//ָ����Burstת�������ڴ洫��          
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; 	//ָ����Burstת��������Χת��              
    DMA_Init(DMA2_Stream3, &DMA_InitStructure); 							//����DMA��ͨ��       
    DMA_Cmd(DMA2_Stream3, ENABLE);   										//ʹ��ͨ��       
  
    //ADC����      
		  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);   					//����ADCʱ��  
    ADC_DeInit();  //��ʼ��ADC  ��λʱ�� 
    //ADCͨ������  

    ADC_CommonInitTypeDef ADC_CommonInitStructure; 
    ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_1;         //DMAģʽ     
    ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;  				 //��������ģʽ  
    ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;  			 //4��Ƶ ȷ��ADC1ʱ��Ϊ21MHz
    ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //���β������   	5�ӳ�������
    ADC_CommonInit(&ADC_CommonInitStructure);     
  
	
	  ADC_InitTypeDef ADC_InitStructure;    
    ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;  				  //����Ϊ12λ        
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;     						  //ɨ�跽ʽ     
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;    					  //����ת��    
   	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;	//�ⲿ������ֹ���������
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;    				  //�����Ҷ���   
    ADC_InitStructure.ADC_NbrOfConversion = 1;    				  //1��ת���ڹ���������
    ADC_Init(ADC2, &ADC_InitStructure);    
    ADC_Cmd(ADC2, ENABLE);    												 //ʹ��ADC2
    //����ģʽͨ������ 
		ADC_RegularChannelConfig(ADC2, ADC_Channel_5, 1, ADC_SampleTime_480Cycles);    //������������ 112��480��
    //ʹ��ADC��DMA    
		ADC_DMARequestAfterLastTransferCmd(ADC2, ENABLE);
		ADC_DMACmd(ADC2, ENABLE);              
    //����ADC2�����ת��  
		ADC_SoftwareStartConv(ADC2);      
}
  

void Sensor_Current_Init(void)//������������ʼ��
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
	sum/=(ADC2_BUFx_LEN);//    ȡƽ��ֵ������DEBUG���Թ�����֤     
	return sum;	
}

//������ֵ�˲������ڿɸ���ʵ����Ҫ�Ż��˲��㷨





