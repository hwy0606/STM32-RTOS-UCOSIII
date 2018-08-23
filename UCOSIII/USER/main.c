#include "delay.h"
#include "includes.h"
#include "os_app_hooks.h"



//#include "task.h"
//调用uc/probe 示波器相关文件

//#define PROBE_SCOPE

#ifdef PROBE_SCOPE
#include "probe_scope.h"
#include "probe_scope_cfg.h"
#endif


int main(void)
{
	
	#ifdef PROBE_SCOPE
	ProbeScope_Init(10000); //传参采样频率 HZ
	Task_Timer_Init(10000-1,84-1);  //任务循环 10ms
	#endif
	
	
	CPU_SR_ALLOC(); //调用临界区保护时先声明CPU_SR_ALLOC()
	delay_init(168);  	//时钟初始化
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//中断分组配置
	

	

//Motor_Init();  //电机初始化
//USART1_DMA_Init(115200);  //串口1DMA初始化 上位机 监测数据
//USART2_DMA_Init(9600);   //串口2DMA初始化 磁粉制动器电源控制
//Encoder_Init();   //正交编码器初始化
//Sensor_Current_Init(); //电机电流传感器初始化
//Omron_Counter_Init(); //光电开关初始化
//Task_Timer_Init(10000-1,84-1);  //任务循环 10ms
//PID_init(); //PID结构体初始化
//Brake_mode(3);
//Motor_Positive(30);
	
	
	
	OS_ERR err;
	OSInit(&err);		//初始化UCOSIII
	OS_CRITICAL_ENTER();//进入临界区
	StartTaskCreate(err);//创建开始任务
	OS_CRITICAL_EXIT();	//退出临界区	 
	OSStart(&err);  //开启UCOSIII
	
	while(1);
}



	



