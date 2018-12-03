#include "usart.h"	  
#include "stm32f0xx_hal.h"
#include <stdio.h>  
#include "queen.h"


uint8_t Rx_len = 0,flagUsartRx = 0;
uint8_t ReceiveBuff[255];

RxMsgTypeDef Uart1_buf;

#ifdef __GNUC__  
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf 
     set to 'Yes') calls __io_putchar() */  
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)  
#else  
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)  
#endif /* __GNUC__ */  
  
/** 
  * @brief  Retargets the C library printf function to the USART. 
  * @param  None 
  * @retval None 
  */  
PUTCHAR_PROTOTYPE  
{  
  /* Place your implementation of fputc here */
  /* Loop until the end of transmission */  
   while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC) == RESET){}   
  /* e.g. write a character to the USART */  
    huart1.Instance->TDR = (uint8_t) ch;  

  return ch;  
}  
/*******************************************************************************
* ������  : UART_SendString
* ����    : USART�����ַ���
* ����    : *s�ַ���ָ��
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void UART_SendString(uint8_t* s)
{
	while(*s)//����ַ���������
	{
		while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC)==RESET); 
		huart1.Instance->TDR = (uint8_t) *s++;                    //���͵�ǰ�ַ�
	}
}
/*******************************************************************************
* ������  : UART1_Data
* ����    : USART1����
* ����    : byte 
* ���    : ��
* ����    : �� 
* ˵��    : ��
*******************************************************************************/
void UART_Data(uint8_t byte)
{
		while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC)==RESET); //�ж��Ƿ�������
		HAL_UART_Transmit(&huart1,&byte,1,0xffff);
}

void UartData_process(void)
{
     if (flagUsartRx == 1)
		 {
		     flagUsartRx = 0;
			   for(int i=0; i<Rx_len; i++)
				 {
						Uart1_buf.Data[i]= ReceiveBuff[i]; //�����ջ��������ݷŵ��ṹ���Data������
				 }
				 
				 Uart1_buf.Len = Rx_len;
				 PushElement(&RxUart1_Queue,Uart1_buf,1);//�����յ�������ѹ������� ???
		     
				 for(int i = 0; i < Rx_len; i++)//��ջ�����
				 {			
						 ReceiveBuff[i] = 0;
				 }
				 Rx_len=0;
				 HAL_UART_Receive_DMA(&huart1,(uint8_t*)ReceiveBuff,255);
				 
		 }
}





