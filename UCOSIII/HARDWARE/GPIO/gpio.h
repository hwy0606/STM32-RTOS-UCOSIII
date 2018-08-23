#ifndef __GPIO_H
#define __GPIO_H


#include "led.h"
#include "includes.h"
#include "os_app_hooks.h"


//GPIO端口定义 位带操作
#define A1 PCout(1)	// PC1-A1  
#define A2 PCout(3)	// PC3-A2	 
#define A3 PCout(5)	// PC5-A3	 
#define A4 PCout(7)	// PC7-A4	 


void Gpio_Init(void);//初始化		
void KEY_Init(void);
#endif 
