#ifndef __ADC_H
#define __ADC_H	

#include "stm32f10x.h"

#include "label.h"

void Adc_Init(void);//AD初始化
u16  Get_Adc(u8 ch);  //获取AD值
//u16 Get_Adc_Average(u8 ch,u8 times); 
//u16 Temp_process(u8 ch);
Temp Temp_process(u8 ch);
Mq2 MQ2_process(u8 ch); //烟雾传感器处理

u16 Get_Adc_Average(u8 ch,u8 times);
u8 Compare_tempvol(u16 Vol);//电压查表成为温度值
u8 MQ2_Calibration(u8 ch);       //烟雾传感器的校准

#endif 
