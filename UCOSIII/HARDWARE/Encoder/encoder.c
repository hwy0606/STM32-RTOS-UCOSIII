//////////////////////////////////////////////////////////////////////////////////	 
//STM32F407IGT6
//编码器驱动程序 欧姆龙E6B2-CWZ6C 光电编码器 1000线
//引脚 绿色A-PA6 白色B-PA7 黄色Z-PC4
//电机速度存储在float型变量motor_speed中
//HWY 2017 10 19
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	 
#include "stm32f4xx.h"
#include "encoder.h" 
#include "math.h"			
#define ENCODER_GPIO_PINA       GPIO_Pin_6
#define ENCODER_GPIO_PINB       GPIO_Pin_7
#define ENCODER_GPIO_PinSourceA GPIO_PinSource6
#define ENCODER_GPIO_PinSourceB GPIO_PinSource7
#define ENCODER_GPIO_RCC      	RCC_AHB1Periph_GPIOA
#define ENCODER_GPIO          	GPIOA
#define ENCODER_GPIO_AF_TIM		  GPIO_AF_TIM3

#define ENCODER_TIM  			  TIM3
#define ENCODER_TIM_RCC  		RCC_APB1Periph_TIM3
#define	ENCODER_TIM_IRQ			TIM3_IRQn

#define	ENCODER_PLUSES			1000

 u32 count_now;//计数器当前值
 u32 count_last;//计数器上一次进中断计数值
 u32 count_diff;//计数器差值
float motor_speed;//电机转速
void Timer3_Init(void)
{	
    GPIO_InitTypeDef         GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef        TIM_ICInitStructure;
    NVIC_InitTypeDef         NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(ENCODER_TIM_RCC,ENABLE);								//开启TIMx时钟
    RCC_AHB1PeriphClockCmd(ENCODER_GPIO_RCC ,ENABLE);							//开启GPIO时钟

    GPIO_PinAFConfig(ENCODER_GPIO,ENCODER_GPIO_PinSourceA,ENCODER_GPIO_AF_TIM);	//PA6引脚复用
    GPIO_PinAFConfig(ENCODER_GPIO,ENCODER_GPIO_PinSourceB,ENCODER_GPIO_AF_TIM);	//PA7引脚服用

	  //GPIO引脚配置
    GPIO_InitStructure.GPIO_Pin = ENCODER_GPIO_PINA|ENCODER_GPIO_PINB;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP ;
    GPIO_Init(ENCODER_GPIO,&GPIO_InitStructure);
 
    //NVIC引脚配置
    NVIC_InitStructure.NVIC_IRQChannel=ENCODER_TIM_IRQ; 		//定时器中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3; 	//抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority=3; 		//子优先级2
    NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
    NVIC_Init(&NVIC_InitStructure);


    //TIMBaseStructure配置
    TIM_TimeBaseStructure.TIM_Period = 4*ENCODER_PLUSES-1;    	//设置下一个更新事件装入活动的自动重装载寄存器周期的值   4倍的线数
    TIM_TimeBaseStructure.TIM_Prescaler = 0; 					//设置用来作为TIMx时钟频率除数的预分频值  不分频
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//设置时钟分割:TDTS = Tck_tim
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM向上计数模式
    TIM_TimeBaseInit(ENCODER_TIM, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(ENCODER_TIM, TIM_EncoderMode_TI12,TIM_ICPolarity_BothEdge,TIM_ICPolarity_BothEdge);   //bothedge上升下降都计数，一般编码器都使用这个
    
	  TIM_ICStructInit(&TIM_ICInitStructure);				        //设置TIM_Channel = TIM_Channel_1;
    TIM_ICInitStructure.TIM_ICFilter = (u8) 6;  				//配置TIM_Channel_1输入滤波器	   这里的滤波是6/42M= 142ns
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);
    
	  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;			//设置TIM_Channel = TIM_Channel_2;
    TIM_ICInit(ENCODER_TIM, &TIM_ICInitStructure);		        //配置TIM_Channel_2输入滤波器
	
	  TIM_ClearFlag(ENCODER_TIM, TIM_FLAG_Update);  				//清楚所有标志位
    TIM_ITConfig(ENCODER_TIM, TIM_IT_Update, ENABLE); 			//允许溢出中断更新

	  TIM_ClearFlag(ENCODER_TIM,TIM_IT_CC1);					    //清除输入捕获1中断标志位
    TIM_ITConfig(ENCODER_TIM, TIM_IT_CC1, ENABLE); 			    //允许通道1更新中断 ,
	  TIM_ClearFlag(ENCODER_TIM,TIM_IT_CC2);					    //清除输入捕获2中断标志位
    TIM_ITConfig(ENCODER_TIM, TIM_IT_CC2, ENABLE); 			    //允许通道2更新中断 
		
		//Reset counter
    ENCODER_TIM->CNT = 0;

    TIM_Cmd(ENCODER_TIM, ENABLE);  								//使能TIM
}




/**
 * @Description: TIM2计数模式初始化
 * @note1： TIM2是32位定时器最大可计数到4294967295
 * @note2： 4294967295@21MHz，最大计时204.5s
 */
void Timer2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  			//使能TIM时钟
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFFFFFE; 				//自动重装载值  4294967294+1
    TIM_TimeBaseInitStructure.TIM_Prescaler=4-1;  					//定时器分频            
    TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up; 	//向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision=TIM_CKD_DIV1; 		//
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);				//初始化TIM
    TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE); 						//禁止定时器更新中断
    TIM_Cmd(TIM2,ENABLE); 											//使能定时器
}




void Get_Motor_Speed(void)   //获取电机速度函数,将速度写入float型motor_speed变量
{
	if(count_now>count_last) //考虑上溢
	{count_diff=count_now-count_last;}

   else{count_diff=count_now+4294967294.0-count_last;}  //0xFFFFFFFE

	 motor_speed=1260000000.0/(float)(count_diff);            //单位 n/min
	 count_last=count_now;
}
/**
 * @Description: 编码器中断函数           更新中断
 */


void TIM3_IRQHandler(void)
{
    /* enter interrupt */
	  count_now= TIM2->CNT;	 //取当前时间
   if(TIM_GetITStatus (ENCODER_TIM,TIM_IT_Update)!=RESET)//进中断
	 {
		 Get_Motor_Speed();
		 TIM_ClearITPendingBit (ENCODER_TIM,TIM_IT_Update );//清除中断标志位	 
	 }	 
}

void Encoder_Init()
{
	Timer2_Init();
	Timer3_Init();
}




		



