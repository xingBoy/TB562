/*******************************************************************************
 * File name:           queen.c
 * Descriptions:        ������غ���
 * Created date:        2016-11-22
 * Last modified Date:  2016-11-22
********************************************************************************/
#include <stdio.h>
#include "queen.h"

// CircleQueue		 TxCAN_Queue;	/* CAN ���ͻ�����*/
CircleQueue		 RxCAN_Queue;		/* CAN ���ջ�����*/
Circle_buffer    buffer;
/*********************************************************************************************************
** Functoin name:       InitCircleQueue
** Descriptions:        ��ʼ�����У�ʹ����ͷָ�����β
** input paraments:     �������    
** Returned values:     TRUE��ʼ���ɹ�������ʧ��
*********************************************************************************************************/
char InitCircleQueue(CircleQueue *pCQ)  
{       
     if (pCQ == NULL)          
	 	return FALSE;      
	 else      
	 	{         
	 	  pCQ->front = 0;          
		  pCQ->rear = 0;
		  pCQ->free=QUEUESIZE;
		  pCQ->atom = FALSE;   
		}  
	 return TRUE;  
}    
/*********************************************************************************************************
** Functoin name:       IsQueueEmpty
** Descriptions:        �������Ƿ�Ϊ��
** input paraments:     ����ָ�� CircleQueue *pCQ
** Returned values:     TRUEΪ��
*********************************************************************************************************/
char IsQueueEmpty(CircleQueue *pCQ) 
{      
      if (pCQ->front == pCQ->rear)         
	  	return TRUE;     
	  else          
	  	return FALSE;  
}    
/*********************************************************************************************************
** Functoin name:       IsQueueFull
** Descriptions:        �������Ƿ�����
** input paraments:     ����ָ�� CircleQueue *pCQ
** Returned values:     TRUEΪ��
*********************************************************************************************************/
char IsQueueFull(CircleQueue *pCQ) 
{     
     if ((pCQ->rear + 1) % QUEUESIZE == pCQ->front)         
	 	return TRUE;     
	 else          
	 	return FALSE; 
}   
/*********************************************************************************************************
** Functoin name:       PushElement
** Descriptions:        ��Ԫ��ѹ�����
** input paraments:     ����ָ�� CircleQueue *pCQ�� Ҫѹ���ֵCanRxMsg dMsgData
** Returned values:     TRUEѹ����гɹ�������ʧ��
*********************************************************************************************************/
bool PushElement(CircleQueue *pCQ, CanRxMsgTypeDef *dMsgData, unsigned char dir)
{  	
    if (IsQueueFull(pCQ)) 
	{
		return FALSE;  
	}
	pCQ->atom = TRUE;

	if(dir == _REAR) 
    {
		pCQ->rear = (pCQ->rear + 1) % QUEUESIZE;  
		mymemcpy(&(pCQ->CAN_RxMsg[pCQ->rear]), dMsgData, sizeof(CanRxMsgTypeDef));
		pCQ->free--;
	} 
	else if(dir == _FRONT) 
	{
		mymemcpy(&(pCQ->CAN_RxMsg[pCQ->front]), dMsgData, sizeof(CanRxMsgTypeDef));
		if(pCQ->front == 0) 
		{
			pCQ->front = (QUEUESIZE - 1);
		} 
		else 
		{
			pCQ->front = (pCQ->front - 1);
		}
		pCQ->free--;
	} 
	else 
	{
		return FALSE;
	}
	
	pCQ->atom = FALSE;
	return TRUE;  
}  
/*********************************************************************************************************
** Functoin name:       PopElement
** Descriptions:        �����е�Ԫ��ȡ��
** input paraments:     ����ָ��CircleQueue *pCQ������ѹ�����еĽṹ��ָ��CanRxMsg *pMsgData
** Returned values:     TRUEΪ����ѹ���ɹ�������ʧ��
*********************************************************************************************************/
bool PopElement(CircleQueue *pCQ, CanRxMsgTypeDef *pMsgData)
{      
     if( (IsQueueEmpty(pCQ)) || (pCQ->atom == TRUE) )        
	 	return FALSE;       
	 pCQ->front = (pCQ->front + 1) % QUEUESIZE;  	
	 mymemcpy(pMsgData, &(pCQ->CAN_RxMsg[pCQ->front]), sizeof(CanRxMsgTypeDef)); 
	 pCQ->free++;
     
	 return TRUE;  
}  
/*********************************************************************************************************
** Functoin name:       mymemcpy
** Descriptions:        �����ݴ�Դ��ַ������Ŀ���ַ
** input paraments:     dest Ŀ�ĵ�ַ source Դ��ַ count ��������
** Returned values:     ��
*********************************************************************************************************/
void* mymemcpy(void* dest, void* source, int count)
{
	char *ret = (char *)dest;
	char *dest_t = ret;
	char *source_t = (char *)source;
	
	while (count--)
		*dest_t++ = *source_t++; 
	
	return ret;
}
/*********************************************************************************************************
** Functoin name:       
** Descriptions:       
** input paraments:     
** Returned values: 
*********************************************************************************************************/
void bufferInit(void)
{
     buffer.front=OFF;
     buffer.atom=OFF;
     buffer.rear=OFF;
}
uint8_t bufferPop(uint8_t* _buf)
{
    if(buffer.front==buffer.rear||buffer.atom==ON)                    //���ͷβ�Ӵ���ʾ������Ϊ��
          return FALSE;
    else 
    {
        if(buffer.atom==OFF)
       {
          *_buf=buffer.circle_buffer[buffer.front];    //����������ǿ���ȡͷ�ڵ�ֵ��ƫ��ͷ�ڵ�
          if(++buffer.front>=BUFFER_SIZE)
            buffer.front=0;
       }
    }
    return TRUE;
}

void bufferPush(const uint8_t _buf)
{   
    buffer.atom=ON;
    buffer.circle_buffer[buffer.rear]=_buf;          //��β��׷��
    if(++buffer.rear>=BUFFER_SIZE)                   //β�ڵ�ƫ��
        buffer.rear=0;                               //����������󳤶� ���� �γɻ��ζ���
        if(buffer.rear==buffer.front)                //���β���ڵ�׷��ͷ���ڵ� ���޸�ͷ�ڵ�ƫ��λ�ö�����������
        if(++buffer.front>=BUFFER_SIZE)
            buffer.front=0;
    buffer.atom=OFF;
}

