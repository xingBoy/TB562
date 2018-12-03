#ifndef  __GPRS_H
#define  __GPRS_H

#include "stm32f1xx_hal.h"
#include "usart.h"
#include "string.h"

#define Buf_Max 200 					  //´®¿Ú2»º´æ³¤¶È
extern uint8_t First_Int;
extern char Uart_Buf[Buf_Max];

void Wait_CREG(void);
void Connect_Server(void);
void Send_OK(void);

#endif
