#ifndef __GPIO_H
#define __GPIO_H

#include "stm32f0xx_hal.h"

////1---read  0---write   地址为前面7-bit
#define ad5141_1_write_address       0x5E
#define ad5141_2_write_address       0x5C
#define ad5141_3_write_address       0x58
#define ad5141_4_write_address       0x56
#define ad5141_5_write_address       0x54
#define ad5141_6_write_address       0x50



void Gpio_PowerControl(void);  //GPIO 电源控制 

#endif
