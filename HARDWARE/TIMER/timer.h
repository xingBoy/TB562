#ifndef __TIMER_H
#define __TIMER_H

#include "stm32f10x.h"


void TIM3_Time_Init(u16 arr,u16 psc);                             //TIM3��ʱ�жϳ�ʼ��
void TIM3_PWM_Init(u16 arr,u16 psc);  
void TIM5_PWMIn_Init(u16 arr,u16 psc);                            //TIM5 PWM���벶���ʼ��
void TIM8_In_Init(u16 arr,u16 psc);                               //TIM8 ���벶���ʼ��

void TIM8_PWMOut_Init(u16 arr,u16 psc,u16 duty);                  //TIM8 PWM�����ʼ��
void ControlPWMOut1(TIM_TypeDef* TIMx,u16 arr,u16 psc,u16 duty);  //TIMX ����PWM���

void TIM2_ExtCount_Init(void);                                    //TIM2 �ⲿ����ģʽ��ʼ��

#endif
