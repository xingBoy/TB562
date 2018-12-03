#ifndef __QUEEN_h
#define __QUEEN_h

#include <stdio.h>
#include "CAN.h"


#define TRUE		1
#define FALSE		0
#define _FRONT		0
#define _REAR		1

#define ON  1		/*�ߵ�ƽ*/
#define OFF 0		/*�͵�ƽ*/

#define QUEUESIZE 30    //������еĴ�С
#define BUFFER_SIZE 20  //���建������Ĵ�С 

typedef int DataType;
typedef char bool;
typedef struct  
{      
	CanRxMsgTypeDef CAN_RxMsg[QUEUESIZE];
	unsigned char atom;	// ��    
	int front; //ָ���ͷ�������������ָ�Ŀռ䲻���Ԫ��      
	int rear; //ָ���β��������������һ��Ԫ��
	uint8_t free;   //ͳ�ƻ������������  
}CircleQueue;  
typedef struct
{
   uint8_t circle_buffer[BUFFER_SIZE]; //
   uint8_t atom;                // ��
   uint8_t front;               //ָ���ͷ�������������ָ�Ŀռ䲻���Ԫ�� 
   uint8_t rear;                //ָ���β��������������һ��Ԫ�� 
}Circle_buffer;

// extern CircleQueue		 TxCAN_Queue;
extern CircleQueue RxCAN_Queue;
extern Circle_buffer buffer;

bool InitCircleQueue(CircleQueue *pCQ);
bool IsQueueEmpty(CircleQueue *pCQ);
bool IsQueueFull(CircleQueue *pCQ);
bool PushElement(CircleQueue *pCQ, CanRxMsgTypeDef *dMsgData, unsigned char dir);
bool PopElement(CircleQueue *pCQ, CanRxMsgTypeDef *pMsgData);
bool GetHeadElement(CircleQueue *pCQ, CanRxMsgTypeDef *pMsgData);
void* mymemcpy(void* dest, void* source, int count);
uint8_t bufferPop(uint8_t* _buf);
void bufferPush(const uint8_t _buf);
void bufferInit(void);


#endif
