#include "delay.h"
				   
volatile unsigned int timenum;      //????volatile!!!  
volatile unsigned long time_delay; // ????,?????????  
//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void delay_us(uint32_t nus)
{		
	//SYSTICK��Ƶ--1us��ϵͳʱ���ж�  
    if (SysTick_Config(HAL_RCC_GetHCLKFreq()/1000000))  
    {  
     
        while (1);  
    }  
    time_delay=nus;//��ȡ��ʱʱ�� 
    while(time_delay);  
    SysTick->CTRL=0x00; //�رռ�����
    SysTick->VAL =0X00; //��ռ�����
}

void delay_ms(uint16_t nms)
{	 		  	  
//SYSTICK��Ƶ--1ms��ϵͳʱ���ж�   
    if (SysTick_Config(HAL_RCC_GetHCLKFreq()/1000))  
    {  
     
        while (1);  
    }  
    time_delay=nms;//��ȡ��ʱʱ��
    while(time_delay);  
    SysTick->CTRL=0x00; //�رռ�����
    SysTick->VAL =0X00; //��ռ�����
}  








































