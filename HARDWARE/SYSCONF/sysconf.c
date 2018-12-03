#include "sysconf.h"



void NVIC_INIT(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // �ж����ȼ�����
	// 4�������ȼ��� 4�������ȼ�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�

    //Usart2 �ж����ȼ�����
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;        //����2�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;  //��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;		 //�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
    //CAN �����ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN1_TX_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;   // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;          // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    //CAN FIFO0�����ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    //CAN �����ж����ȼ�����
	NVIC_InitStructure.NVIC_IRQChannel = CAN1_SCE_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;     // �����ȼ�Ϊ1
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;            // �����ȼ�Ϊ0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
}
void Hardware_Init(void)
{
	 LED_Init();			                         //LED�˿ڳ�ʼ��
	 Key_Init();                                     //����IO��ʼ��
     Relay_Init();                                   //�̵�����ʼ��
     uart_init(115200);	                             //���ڳ�ʼ��Ϊ115200
     Can_Init(BITRATE_500K);                         //CAN��ʼ��
}
void Variable_Init(void)                             //ȫ�ֱ�����ʼ��
{  
     ;
}

 
