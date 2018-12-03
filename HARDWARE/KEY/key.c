#include "key.h"
#define KeyDebug  //����������Դ�ӡ
/*******************************************************************************
* ����:KEY_Init
* ����:���밴����ʼ������
* �β�:��
* ����:��
* ˵��:��PD8,9,10��PE9,14,15���г�ʼ���������ó���������
******************************************************************************/
void Key_Init(void) //IO��ʼ��
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOE|RCC_APB2Periph_GPIOD,ENABLE);//ʹ��PORTD,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;   //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOD, &GPIO_InitStructure);//��ʼ��GPIOD.8,9,10
 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9|GPIO_Pin_14|GPIO_Pin_15;  //
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOE, &GPIO_InitStructure);//��ʼ��GPIOE.9,14,15
 	
}
/*******************************************************************************
* ����:scan_key
* ����:ɨ����뿪��ֵ
* �β�:��
* ����:���˿���ֵ
* ˵��: ��
******************************************************************************/
u8 scan_key(void)
{
     u8 keyvalue;
	 
	 if(KEY1==0 && KEY2==1 && KEY3==1 && KEY4==1 && KEY5==1 && KEY6==1)
     {
        keyvalue=1;
	 }
	 else if(KEY1==1 && KEY2==0 && KEY3==1 && KEY4==1 && KEY5==1 && KEY6==1) 
	 {
        keyvalue=2;
	 }
     else if(KEY1==1 && KEY2==1 && KEY3==0 && KEY4==1 && KEY5==1 && KEY6==1) 
	 {
        keyvalue=3;
	 }
	 else if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==0 && KEY5==1 && KEY6==1) 
	 {
        keyvalue=4;
	 }
	 else if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==1 && KEY5==0 && KEY6==1) 
	 {
        keyvalue=5;
	 }
	 else if(KEY1==1 && KEY2==1 && KEY3==1 && KEY4==1 && KEY5==1 && KEY6==0) 
	 {
        keyvalue=6;
	 }
	 else 
	 {
        keyvalue=7;
	 }
	 
	 return keyvalue;     
}
/*******************************************************************************
* ����:scan_key
* ����:ɨ����뿪��ֵ
* �β�:��
* ����:���˿���ֵ
* ˵��: ��
******************************************************************************/
void Key_Process(void)
{
    u8 KEYS;
    KEYS=scan_key();
	switch(KEYS)
	{
       case 1: 
              status=1;
	          break;
       case 2: 
              status=2;
			  break;
	   case 3: 
              status=3;
		      break;
	   case 4: 
              status=4;
	          break;
	   case 5: 
              status=5;
		      break;
	   case 6: 
              status=6;
		      break;
	   default:
	   	      status=0;
		      break;
	}
	   #ifdef KeyDebug
	   printf("status=%d\r\n",status);
	   #endif
}

/*u8 scan_key(void)    //����ɨ��
{
	//u8 key_status;
	
	if((longkey == OFF) && (keybuf != _KEYNULL))
	{
		// ��keybuf�����Ϊ������̰������ǰ����Ѿ��ɿ���
		// ������ѭ������һ�κ󣬼�ʱ������״̬���Ϊ�ް������¡�
		keybuf = _KEYNULL;
	}
	if(key_handle ==ON) 
	{ //5ms����һ��
		key_handle = OFF;	  // ��λ5ms��־λ 
		//key_status = WK_UP;
		//if(key_status == 0) 
		if(WK_UP==OFF)      //WK_UP��������
		{	
			longkey++;      //���°����ۼ�
		}
		else if(USER1==OFF) //USER1��������
		{
      longkey1++;     //���°����ۼ�
    }	
		else if(USER2==OFF) //USER2��������
		{
      longkey2++;     //���°����ۼ�
    }	
		else if(TAMPER==OFF)//TAMPER��������
		{
      longkey3++;     //���°����ۼ�
    }	
		else              //�ɿ�����
		{	
			if((longkey >= 3)&&(longkey <= 100))  // ����15ms - 1s
			{	
				//keybuf = _SHORTKEY;
				longkey = 0;                   
				longkey1 = 0;
				longkey2 = 0;
				longkey3 = 0;
				return WKUP_PRES;              //WKUP��������
			} 
			else if((longkey1 >= 3)&&(longkey1 <= 100))  // ����15ms - 1s
			{
				longkey = 0;                   //�������¼���1��0
				longkey1 = 0;                  //�������¼���2��0
				longkey2 = 0;                  //�������¼���3��0
				longkey3 = 0;                  //�������¼���4��0
				return KEY2_PRES;              //KEY2��������
      }
			else if((longkey2 >= 3)&&(longkey2 <= 100))  // ����15ms - 1s
			{
        longkey = 0;                   //�������¼���1��0
				longkey1 = 0;                  //�������¼���2��0
				longkey2 = 0;                  //�������¼���3��0
				longkey3 = 0;                  //�������¼���4��0
				return KEY1_PRES;              //KEY1��������
      }
			else if((longkey3 >= 3)&&(longkey3 <= 100))  // ����15ms - 1s
			{
        longkey = 0;                   //�������¼���1��0
				longkey1 = 0;                  //�������¼���2��0
				longkey2 = 0;                  //�������¼���3��0
				longkey3 = 0;                  //�������¼���4��0
				return KEY0_PRES;              //KEY0��������
      }
			//else if(longkey >= 200) 
			//{ // 2s
			//	//keybuf = _LONGKEY;
			//	longkey = 0;
			//	return _LONGKEY;
			//}
			else 
			{
				//keybuf = _KEYNULL;// ��Ϊ�Ŷ�������״̬Ҳ��Ϊ�ް�������
				longkey = 0;                  //�������¼���1��0
				longkey1 = 0;                 //�������¼���2��0
				longkey2 = 0;                 //�������¼���3��0
				longkey3 = 0;                 //�������¼���4��0
			  return _KEYNULL;              //�޼�������
      }
		}
	}
	return _KEYNULL;                    //�޼�������
}
void key_process(void) //��������
{
    keynum=scan_key();               //����ֵ��ȡ
		if(keynum==KEY0_PRES)
		{
			LED2=!LED2;                   //D2��ȡ��������֮��ԭ�������䰵��ԭ���ǰ�����
		  //printf("\r\nAD:");       //���ڴ�ӡ����
			adcx=1234;
			USART_SendData(USART1,adcx/1000);        
			USART_SendData(USART1,adcx%1000/100);
			USART_SendData(USART1,adcx%1000/100/10);
			USART_SendData(USART1,adcx%1000/100%10);
		}
		else if(keynum==KEY1_PRES)
		{
      LED0=!LED0;                   //D0��ȡ��������֮��ԭ�������䰵��ԭ���ǰ�����
		  printf("\r\nKEY3\r\n");       //���ڴ�ӡ����
    }
    else if(keynum==KEY2_PRES)	
    {
      LED1=!LED1;                   //D1��ȡ��������֮��ԭ�������䰵��ԭ���ǰ�����
		  printf("\r\nKEY4\r\n");       //���ڴ�ӡ����
    }
    else if(keynum==WKUP_PRES)
    {
      LED3=!LED3;                   //D3��ȡ��������֮��ԭ�������䰵��ԭ���ǰ�����
		  printf("\r\nKEY2\r\n");       //���ڴ�ӡ����
    }		
}*/	
