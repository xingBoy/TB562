#include "Gprs.h"




/*************	本地常量声明	**************/
const char *string = "AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647";	//IP登录服务器
uint8_t testnum[]=
{0x43,0x4D,0x31,0x59,0x37,0x44,0x55,0x34,0x36,0x47,0x32,0x49,0x38,0x39,0x31,0x39,0x1a};

/*************  本地变量声明	**************/
//char Uart1_Buf[Buf1_Max];
char Uart_Buf[Buf_Max];
uint8_t Times;
uint8_t First_Int;
uint8_t shijian;
uint8_t Timer0_start;	//定时器0延时启动计数器
uint8_t Uart2_Start;	//串口2开始接收数据
uint8_t Uart2_End;	  //串口2接收数据结束
uint8_t Heart_beat;		//发送心跳帧标志位
/*******************************************************************************
* 函数名 : CLR_Buf2
* 描述   : 清除串口2缓存数据
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	uint16_t k;
	for(k=0;k<Buf_Max;k++)      //将缓存内容清零
	{
		Uart_Buf[k] = 0x00;
	}
    First_Int = 0;              //接收字符串的起始存储位置
}
/*******************************************************************************
* 函数名 : Wait_CREG
* 描述   : 等待模块注册成功
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Wait_CREG(void)
{
	uint8_t i;
	uint8_t k;
	i = 0;
	CLR_Buf2();
  while(i == 0)        			
	{
		CLR_Buf2();        
		UART_SendString("AT+CREG?");
		UART_SendString("\r\n");
    HAL_Delay(2000);		
	  for(k=0;k<Buf_Max;k++)      			
    {
			if(Uart_Buf[k] == ':')
			{
				if((Uart_Buf[k+4] == '1')||(Uart_Buf[k+4] == '5'))
				{
					i = 1;
					UART_SendString("\r\n");
				  break;
				}
			}
		}
		printf("注册中.....\r\n");
	}
	printf("注册成功.....\r\n");
}
/*******************************************************************************
* 函数名 : Find
* 描述   : 判断缓存中是否含有指定的字符串
* 输入   : 
* 输出   : 
* 返回   : unsigned char:1 找到指定字符，0 未找到指定字符 
* 注意   : 
*******************************************************************************/

uint8_t Find(char *a)
{ 
  if(strstr(Uart_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}
/*******************************************************************************
* 函数名 : Second_AT_Command
* 描述   : 发送AT指令函数
* 输入   : 发送数据的指针、发送等待时间(单位：S)
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,uint8_t wait_time)         
{
	uint8_t i;
	char *c;
	c = b;										//保存字符串地址到c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//将字符串地址给b
				for (; *b!='\0';b++)
				{
					while(__HAL_UART_GET_FLAG(&huart4, UART_FLAG_TC)==RESET);
					HAL_UART_Transmit(&huart4,(uint8_t *)&b,1,0xffff);//UART2_SendData(*b);
				}
				UART_SendString("\r\n");	
				Times = 0;
				shijian = wait_time;
				Timer0_start = 1;
		   }
    }
 	  else
		{
			i = 1;
			Timer0_start = 0;
		}
	}
	CLR_Buf2(); 
}
/*******************************************************************************
* 函数名 : Connect_Server
* 描述   : GPRS连接服务器函数
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Connect_Server(void)
{
	UART_SendString("AT+CIPCLOSE=1");	//关闭连接
  HAL_Delay(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//关闭移动场景
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//设置GPRS移动台类别为B,支持包交换和数据交换 
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//设置PDP上下文,互联网接协议,接入点等信息
	Second_AT_Command("AT+CGATT=1","OK",2);//附着GPRS业务
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//设置为GPRS连接模式
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//设置接收数据显示IP头(方便判断数据来源,仅在单路连接有效)
	Second_AT_Command((char*)string,"OK",5);
	HAL_Delay(100);
	CLR_Buf2();
	printf("connect OK!!\r\n");
}
/*******************************************************************************
* 函数名 : Send_OK
* 描述   : 发送数据应答服务器的指令，该函数在有两功能
					1：接收到服务器的数据后，应答服务器
					2：服务器无下发数据时，每隔10秒发送一帧心跳，保持与服务器连接
* 输入   : 
* 输出   : 
* 返回   : 
* 注意   : 
*******************************************************************************/
void Send_OK(void)
{
	uint8_t i;
	Second_AT_Command("AT+CIPSEND",">",2);
	//Second_AT_Command("CM1Y7DU46G2I8919","SEND OK",8);			//回复OK 
  for(i=0;i<17;i++)
	UART_Data(testnum[i]);
	
}

