#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f0xx_hal.h"



//����1���ͻس�����
//#define UART_SendLR() UART_SendString("\r\n")
extern uint8_t Rx_len,flagUsartRx;
extern uint8_t ReceiveBuff[255];

/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;

void UART_SendString(uint8_t* s);     //���͵�ǰ�ַ�
void UART_Data(uint8_t byte);         //���͵�ǰ����
void UartData_process(void);          //���յ������ݴ���
#endif


