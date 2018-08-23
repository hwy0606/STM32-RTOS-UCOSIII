#include  "stm32f4xx_gpio.h"
#include "led.h"

void LED_GPIO_Config(void)
{
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
  GPIO_InitTypeDef  GPIO_InitStructure;
  	/*����GPIOF������ʱ��*/
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOC, ENABLE);
/*ѡ��Ҫ���Ƶ�GPIOF����*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
  /*��������ģʽΪͨ���������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  /*������������Ϊ100MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	/*���ÿ⺯������ʼ��GPIOF*/
  GPIO_Init(GPIOE, &GPIO_InitStructure);


	/*ѡ��Ҫ���Ƶ�GPIOB����*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  /*��������ģʽΪͨ���������*/
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  /*������������Ϊ100MHz */
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  	/*���ÿ⺯������ʼ��GPIOF*/
  GPIO_Init(GPIOC, &GPIO_InitStructure);

}
