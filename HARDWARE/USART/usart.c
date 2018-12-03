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
* 函数名  : UART_SendString
* 描述    : USART发送字符串
* 输入    : *s字符串指针
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void UART_SendString(uint8_t* s)
{
	while(*s)//检测字符串结束符
	{
		while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC)==RESET); 
		huart1.Instance->TDR = (uint8_t) *s++;                    //发送当前字符
	}
}
/*******************************************************************************
* 函数名  : UART1_Data
* 描述    : USART1接收
* 输入    : byte 
* 输出    : 无
* 返回    : 无 
* 说明    : 无
*******************************************************************************/
void UART_Data(uint8_t byte)
{
		while(__HAL_UART_GET_FLAG(&huart1, UART_FLAG_TC)==RESET); //判断是否接收完成
		HAL_UART_Transmit(&huart1,&byte,1,0xffff);
}

void UartData_process(void)
{
     if (flagUsartRx == 1)
		 {
		     flagUsartRx = 0;
			   for(int i=0; i<Rx_len; i++)
				 {
						Uart1_buf.Data[i]= ReceiveBuff[i]; //将接收缓存区数据放到结构体的Data数组中
				 }
				 
				 Uart1_buf.Len = Rx_len;
				 PushElement(&RxUart1_Queue,Uart1_buf,1);//将接收到的数据压入队列中 ???
		     
				 for(int i = 0; i < Rx_len; i++)//清空缓存区
				 {			
						 ReceiveBuff[i] = 0;
				 }
				 Rx_len=0;
				 HAL_UART_Receive_DMA(&huart1,(uint8_t*)ReceiveBuff,255);
				 
		 }
}





