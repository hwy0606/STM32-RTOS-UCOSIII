#ifndef __LED_H
#define __LED_H

#include  "stm32f4xx_gpio.h"

#define LED1_OFF  GPIO_SetBits(GPIOE,GPIO_Pin_5);    //PF9����ߵ�ƽ
#define LED1_ON  GPIO_ResetBits(GPIOE,GPIO_Pin_5);   //PF9����͵�ƽ
#define LED2_OFF  GPIO_SetBits(GPIOE,GPIO_Pin_6);    //PF10����ߵ�ƽ
#define LED2_ON  GPIO_ResetBits(GPIOE,GPIO_Pin_6);   //PF10����͵�ƽ
#define LED3_OFF  GPIO_SetBits(GPIOC,GPIO_Pin_13);    //PF10����ߵ�ƽ
#define LED3_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_13);   //PF10����͵�ƽ

void LED_GPIO_Config(void);

#endif 
