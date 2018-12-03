#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f0xx_hal.h"



//串口1发送回车换行
//#define UART_SendLR() UART_SendString("\r\n")
extern uint8_t Rx_len,flagUsartRx;
extern uint8_t ReceiveBuff[255];

/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;

void UART_SendString(uint8_t* s);     //发送当前字符
void UART_Data(uint8_t byte);         //发送当前数据
void UartData_process(void);          //接收到的数据处理
#endif


