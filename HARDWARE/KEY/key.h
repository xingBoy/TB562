#ifndef __KEY_H
#define __KEY_H	 
#include "stm32f10x.h"
#include "usart.h"



//#define USER2    GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)  //��ȡ����0
//#define TAMPER   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13) //��ȡ����1
//#define USER1    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)  //��ȡ����2 
//#define WK_UP    GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)  //��ȡ����3(WK_UP) 

#define KEY3       GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_8)  //��ȡ����3
#define KEY4       GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_9)  //��ȡ����4
#define KEY5       GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_10) //��ȡ����5
#define KEY6       GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_9)  //��ȡ����6
#define KEY1       GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_14) //��ȡ����1
#define KEY2       GPIO_ReadInputDataBit(GPIOE,GPIO_Pin_15) //��ȡ����2



void Key_Init(void);    //����IO��ʼ��	
u8 scan_key(void);      //����ɨ��
void Key_Process(void);  

#endif
