#include "Gprs.h"




/*************	���س�������	**************/
const char *string = "AT+CIPSTART=\"TCP\",\"tcp.tlink.io\",8647";	//IP��¼������
uint8_t testnum[]=
{0x43,0x4D,0x31,0x59,0x37,0x44,0x55,0x34,0x36,0x47,0x32,0x49,0x38,0x39,0x31,0x39,0x1a};

/*************  ���ر�������	**************/
//char Uart1_Buf[Buf1_Max];
char Uart_Buf[Buf_Max];
uint8_t Times;
uint8_t First_Int;
uint8_t shijian;
uint8_t Timer0_start;	//��ʱ��0��ʱ����������
uint8_t Uart2_Start;	//����2��ʼ��������
uint8_t Uart2_End;	  //����2�������ݽ���
uint8_t Heart_beat;		//��������֡��־λ
/*******************************************************************************
* ������ : CLR_Buf2
* ����   : �������2��������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void CLR_Buf2(void)
{
	uint16_t k;
	for(k=0;k<Buf_Max;k++)      //��������������
	{
		Uart_Buf[k] = 0x00;
	}
    First_Int = 0;              //�����ַ�������ʼ�洢λ��
}
/*******************************************************************************
* ������ : Wait_CREG
* ����   : �ȴ�ģ��ע��ɹ�
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
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
		printf("ע����.....\r\n");
	}
	printf("ע��ɹ�.....\r\n");
}
/*******************************************************************************
* ������ : Find
* ����   : �жϻ������Ƿ���ָ�����ַ���
* ����   : 
* ���   : 
* ����   : unsigned char:1 �ҵ�ָ���ַ���0 δ�ҵ�ָ���ַ� 
* ע��   : 
*******************************************************************************/

uint8_t Find(char *a)
{ 
  if(strstr(Uart_Buf,a)!=NULL)
	    return 1;
	else
			return 0;
}
/*******************************************************************************
* ������ : Second_AT_Command
* ����   : ����ATָ���
* ����   : �������ݵ�ָ�롢���͵ȴ�ʱ��(��λ��S)
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/

void Second_AT_Command(char *b,char *a,uint8_t wait_time)         
{
	uint8_t i;
	char *c;
	c = b;										//�����ַ�����ַ��c
	CLR_Buf2(); 
  i = 0;
	while(i == 0)                    
	{
		if(!Find(a)) 
		{
			if(Timer0_start == 0)
			{
				b = c;							//���ַ�����ַ��b
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
* ������ : Connect_Server
* ����   : GPRS���ӷ���������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Connect_Server(void)
{
	UART_SendString("AT+CIPCLOSE=1");	//�ر�����
  HAL_Delay(100);
	Second_AT_Command("AT+CIPSHUT","SHUT OK",2);		//�ر��ƶ�����
	Second_AT_Command("AT+CGCLASS=\"B\"","OK",2);//����GPRS�ƶ�̨���ΪB,֧�ְ����������ݽ��� 
	Second_AT_Command("AT+CGDCONT=1,\"IP\",\"CMNET\"","OK",2);//����PDP������,��������Э��,��������Ϣ
	Second_AT_Command("AT+CGATT=1","OK",2);//����GPRSҵ��
	Second_AT_Command("AT+CIPCSGP=1,\"CMNET\"","OK",2);//����ΪGPRS����ģʽ
	Second_AT_Command("AT+CIPHEAD=1","OK",2);//���ý���������ʾIPͷ(�����ж�������Դ,���ڵ�·������Ч)
	Second_AT_Command((char*)string,"OK",5);
	HAL_Delay(100);
	CLR_Buf2();
	printf("connect OK!!\r\n");
}
/*******************************************************************************
* ������ : Send_OK
* ����   : ��������Ӧ���������ָ��ú�������������
					1�����յ������������ݺ�Ӧ�������
					2�����������·�����ʱ��ÿ��10�뷢��һ֡���������������������
* ����   : 
* ���   : 
* ����   : 
* ע��   : 
*******************************************************************************/
void Send_OK(void)
{
	uint8_t i;
	Second_AT_Command("AT+CIPSEND",">",2);
	//Second_AT_Command("CM1Y7DU46G2I8919","SEND OK",8);			//�ظ�OK 
  for(i=0;i<17;i++)
	UART_Data(testnum[i]);
	
}

