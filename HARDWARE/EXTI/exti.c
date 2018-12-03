#include "exti.h"

  
//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

    //GPIOA.0 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource0);//�жϹܽ�ѡ��

  	EXTI_InitStructure.EXTI_Line=EXTI_Line0;	           //��·ѡ��
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	//�¼�ѡ��
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//����ģʽ 
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;               //�����ж�
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;			        //ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;	    //��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

}

//�ⲿ�ж�0������� 
void EXTI0_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line0); //���LINE0�ϵ��жϱ�־λ  
}
 

 
