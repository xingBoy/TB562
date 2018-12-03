#include "can.h"

//u32 CANERR_Flag = 0;                   // CAN����
//u8 CAN_ErrorCode;
//u32 test6;
extern CAN_HandleTypeDef hcan1;

CanTxMsgTypeDef   TxMessage;
CanRxMsgTypeDef   RxMessage;
//CAN��ʼ��
//mode:CAN_Mode_Normal,��ͨģʽ;CAN_Mode_LoopBack,�ػ�ģʽ;
//Fpclk1��ʱ���ڳ�ʼ����ʱ������Ϊ36M,�������CAN_Mode_Init(CAN_SJW_1tq,CAN_BS2_8tq,CAN_BS1_9tq,4,CAN_Mode_LoopBack);
//������Ϊ:36M/((8+9+1)*4)=500Kbps 
/*******************************************************************************
 * ����: ����CANͨ�Ų�����
 * ����: 
 * �β�: ��
 * ����: ��
 * ˵��: 
 0-----5k
 1-----10k
 2-----20k
 3-----50k
 4-----100k
 5-----125k
 6-----250k
 7-----500k
 8-----800k
 9-----1M
 ******************************************************************************/
void CAN_InitConf(uint8_t baudrate)
{ 
	hcan1.Instance = CAN1;                //CAN1ͨ��
	hcan1.pTxMsg = &TxMessage;
  hcan1.pRxMsg = &RxMessage;
	hcan1.Init.Mode = CAN_MODE_NORMAL;    //����ģʽ
  hcan1.Init.TTCM = DISABLE;            //��ʱ�䴥��ͨ��ģʽ  
  hcan1.Init.ABOM = ENABLE;							//����Զ����߹���	
  hcan1.Init.AWUM = DISABLE;						//˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
  hcan1.Init.NART = ENABLE;							//��ֹ�����Զ����� 
  hcan1.Init.RFLM = DISABLE; 						//���Ĳ�����,�µĸ��Ǿɵ�
  hcan1.Init.TXFP = ENABLE;							//���ȼ��ɱ��ı�ʶ������ 
	//���ò�����
	hcan1.Init.SJW = CAN_SJW_1TQ;	        //����ͬ����Ծ���(Tsjw)Ϊtsjw+1��ʱ�䵥λ  CAN_SJW_1tq	 CAN_SJW_2tq CAN_SJW_3tq CAN_SJW_4tq//��Ϊ�˼��ݲ�ͬ�����ʵ�����

  //tsjw:����ͬ����Ծʱ�䵥Ԫ.��Χ:CAN_SJW_1tq~ CAN_SJW_4tq
  //tbs2:ʱ���2��ʱ�䵥Ԫ.   ��Χ:CAN_BS2_1tq~CAN_BS2_8tq;
  //tbs1:ʱ���1��ʱ�䵥Ԫ.   ��Χ:CAN_BS1_1tq ~CAN_BS1_16tq
  //brp :�����ʷ�Ƶ��.��Χ:1~1024;  tq=(brp)*tpclk1
  //������=Fpclk1/((Tbs1+Tbs2+1)*brp);    
	switch(baudrate)
	{
		case BITRATE_5K: 
        hcan1.Init.Prescaler= 420;                
				hcan1.Init.BS1   = CAN_BS1_16TQ;             
				hcan1.Init.BS2   = CAN_BS2_3TQ;  
				break;
		case BITRATE_10K:
				hcan1.Init.Prescaler= 210;                
				hcan1.Init.BS1   = CAN_BS1_16TQ;             
				hcan1.Init.BS2   = CAN_BS2_3TQ;             
				break;
		case BITRATE_20K:
				hcan1.Init.Prescaler= 100;                
				hcan1.Init.BS1   = CAN_BS1_14TQ;             
				hcan1.Init.BS2   = CAN_BS2_6TQ;             
				break;
		case BITRATE_50K:
				hcan1.Init.Prescaler= 42;                
				hcan1.Init.BS1   = CAN_BS1_16TQ;             
				hcan1.Init.BS2   = CAN_BS2_3TQ;             
				break;
		case BITRATE_100K:
				hcan1.Init.Prescaler= 21;                
				hcan1.Init.BS1   = CAN_BS1_16TQ;             
				hcan1.Init.BS2   = CAN_BS2_3TQ;             
				break;
		case BITRATE_125K:
				hcan1.Init.Prescaler= 16;                
				hcan1.Init.BS1   = CAN_BS1_13TQ;             
				hcan1.Init.BS2   = CAN_BS2_7TQ;             
				break;
		case BITRATE_250K:
				hcan1.Init.Prescaler= 8;                
				hcan1.Init.BS1   = CAN_BS1_13TQ;             
				hcan1.Init.BS2   = CAN_BS2_7TQ;             
				break;
		case BITRATE_500K:
				hcan1.Init.Prescaler= 6;                
				hcan1.Init.BS1   = CAN_BS1_8TQ;             
				hcan1.Init.BS2   = CAN_BS2_5TQ;             
        break;
		/*case BITRATE_800K:
				hcan1.Init.Prescaler= 6;                
				hcan1.Init.BS1   = CAN_BS1_6TQ;             
				hcan1.Init.BS2   = CAN_BS2_1TQ;             
				break;*/
		case BITRATE_1M:
				hcan1.Init.Prescaler= 6;                
				hcan1.Init.BS1   = CAN_BS1_5TQ;             
				hcan1.Init.BS2   = CAN_BS2_1TQ;             
				break;
		default:
				hcan1.Init.Prescaler= 3;                
				hcan1.Init.BS1   = CAN_BS1_8TQ;             
				hcan1.Init.BS2   = CAN_BS2_3TQ;             
				break;
	}
	if(HAL_CAN_Init(&hcan1) != HAL_OK)
  {
    _Error_Handler(__FILE__, __LINE__);
  }
	
  CAN_RxFilerconfig(0,CANRX32IDMASK);                        //���չ���ģʽ����

	InitCircleQueue(&RxCAN_Queue);	                           //��ʼ������*/
}   
/*******************************************************************************
 * ����: 
 * ����: 
 * �β�: ��
 * ����: ��
 * ˵��: 
 ******************************************************************************/
void CAN_RxFilerconfig(uint8_t FilterNum,uint8_t FilterMode)
{
  CAN_FilterConfTypeDef  sFilterConfig; 
   
   sFilterConfig.FilterNumber=FilterNum;	//��������0~13��ѡ
   if(FilterMode==CANRX32IDMASK)
   {
     sFilterConfig.FilterMode=CAN_FILTERMODE_IDMASK; 	//��ʶ������ģʽ
     sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT; 	//32λ�� 
   }
   else if(FilterMode==CANRX32IDLIST)
   {
     sFilterConfig.FilterMode=CAN_FILTERMODE_IDLIST; 	//��ʶ���б�ģʽ
     sFilterConfig.FilterScale=CAN_FILTERSCALE_32BIT; 	//32λ�� 
   }
   else if(FilterMode==CANRX16IDMASK)
   {
     sFilterConfig.FilterMode=CAN_FILTERMODE_IDMASK; 	//��ʶ������ģʽ
     sFilterConfig.FilterScale=CAN_FILTERSCALE_16BIT; 	//16λ�� 
   }
   else if(FilterMode==CANRX16IDLIST)
   {
     sFilterConfig.FilterMode=CAN_FILTERMODE_IDLIST; 	//��ʶ���б�ģʽ
     sFilterConfig.FilterScale=CAN_FILTERSCALE_16BIT; 	//16λ��
   }
   //��ʶ���Ĵ���FxR1
   sFilterConfig.FilterIdHigh=0x0000;//0x000<<5;                  //32λID����16λ
   sFilterConfig.FilterIdLow=0x0000;                     //��16λ
   //���μĴ���FxR2
   sFilterConfig.FilterMaskIdHigh=0x0000;//0xfe1f;                //32λMASK����16λ
   sFilterConfig.FilterMaskIdLow=0x0000;//0xffff;                 //��16λ
	
   sFilterConfig.FilterFIFOAssignment=0;//������0������FIFO0
   
   sFilterConfig.FilterActivation=ENABLE;//���������0

	 sFilterConfig.BankNumber=14;      //0-13 filter BANK�����CAN1,14-27����� filter BANK�����CAN2              
	 
   if(HAL_CAN_ConfigFilter(&hcan1, &sFilterConfig) != HAL_OK)
   {
    /* Filter configuration Error */
    Error_Handler();
   }
	 
} 
/**
  * @brief  Configures the CAN, transmit and receive by polling
  * @param  None
  * @retval PASSED if the reception is well done, FAILED in other case
  */
HAL_StatusTypeDef CAN_Polling(void)
{
  /*##-3- Start the Transmission process #####################################*/
  hcan1.pTxMsg->StdId = 0x11;
  hcan1.pTxMsg->RTR = CAN_RTR_DATA;
  hcan1.pTxMsg->IDE = CAN_ID_STD;
  hcan1.pTxMsg->DLC = 2;
  hcan1.pTxMsg->Data[0] = 0xCA;
  hcan1.pTxMsg->Data[1] = 0xFE;
  
  if(HAL_CAN_Transmit(&hcan1, 10) != HAL_OK)
  {
    /* Transmission Error */
    Error_Handler();
  }
  
  if(HAL_CAN_GetState(&hcan1) != HAL_CAN_STATE_READY)
  {
    return HAL_ERROR;
  }
  
  /*##-4- Start the Reception process ########################################*/
  if(HAL_CAN_Receive(&hcan1, CAN_FIFO0,10) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }
  
  if(HAL_CAN_GetState(&hcan1) != HAL_CAN_STATE_READY)
  {
    return HAL_ERROR;
  }
  
  if (hcan1.pRxMsg->StdId != 0x11)
  {
    return HAL_ERROR;  
  }

  if (hcan1.pRxMsg->IDE != CAN_ID_STD)
  {
    return HAL_ERROR;
  }

  if (hcan1.pRxMsg->DLC != 2)
  {
    return HAL_ERROR;  
  }

  if ((hcan1.pRxMsg->Data[0]<<8|RxMessage.Data[1]) != 0xCAFE)
  {
    return HAL_ERROR;
  }
  
  return HAL_OK; /* Test Passed */
}

/*******************************************************************************
 * ����: CAN_Dataprocess
 * ����: ��������״̬��������������GPIO�Ͷ�ȡGPIO��״̬
 * �β�: ��
 * ����: ��
 * ˵��: 
CANͨѶ����Э��=��11λ����׼֡+���ݣ�1�� 8λ��
��׼֡��0~6λ����CANID ����7~10Ϊ����״̬
Ŀǰֻ�ƶ�����״̬��
0x01 ---��ȡGPIO��״̬  ---0x80+CANID
0x02 ---д��GPIO        ---0x100+CANID

 ******************************************************************************/
void CAN_Dataprocess(void)
{
	CanRxMsgTypeDef RxData;
	uint8_t i;
  uint8_t Canstatus,canbuf[8],CanDeviceID,CANID,TxStdId;
	CanDeviceID=Get_CanID();
	//printf("CANID=%d \r\n",CANID);
	//    CanTxMsg TxMessage;
	  /*CANID=0x01;
    for(i=0;i<8;i++)
    {
	     canbuf[i]=i;//��䷢�ͻ�����
    }
    if(CANID>0)
	  {
      status=CAN_Send_Msg(canbuf,8,CANID);
	    printf("Tx status=%d \r\n",status);
	    HAL_Delay(100);
		}*/
		if(PopElement(&RxCAN_Queue,&RxData)==TRUE)      //�ж϶����������ݲ�ȥ������ѹ������
		{	
			 printf("RxData.StdId=%d \r\n",RxData.StdId);
			 CAN_Send_Msg(RxData.Data,RxData.DLC,RxData.StdId);
			    /*CANID=0x07f&RxData.StdId;       //��ȡCANID
			    Canstatus=RxData.StdId>>7;      //
			    printf("CANID=%d Canstatus=%d \r\n",CANID,Canstatus);
			    if(CANID==CanDeviceID&&Canstatus==0x01)//��ȡ״̬
					{
			       TxStdId=CanDeviceID+0x80;
						 canbuf[0]=Get_SwitchValue();
						 CAN_Send_Msg(canbuf,1,TxStdId);
      		   //printf("RxData.StdId=%d \r\n",RxData.StdId);
          }
					else if(CANID==CanDeviceID&&Canstatus==0x02)//д��״̬
					{
					   Write_OutputGpio(RxData.Data[0]);         //����IO
					}*/
		}
}
//can����һ������(�̶���ʽ:IDΪ0X12,��׼֡,����֡)	
//len:���ݳ���(���Ϊ8)				     
//msg:����ָ��,���Ϊ8���ֽ�.
//����ֵ:0,�ɹ�;
//		 ����,ʧ��;
HAL_StatusTypeDef CAN_Send_Msg(uint8_t* msg,uint8_t len,uint8_t id)
{	
  uint8_t i;
  hcan1.pTxMsg->StdId=id;         //|0x10;
	hcan1.pTxMsg->ExtId=0x00;
	hcan1.pTxMsg->IDE=CAN_ID_STD; 	// ��׼֡
	hcan1.pTxMsg->RTR=CAN_RTR_DATA;		// ����֡
	hcan1.pTxMsg->DLC=len;				// Ҫ���͵����ݳ���
	
	for(i=0;i<len;i++)
	hcan1.pTxMsg->Data[i]=msg[i];	

  HAL_CAN_Transmit(&hcan1, 10);	
  //if(HAL_CAN_Transmit(&hcan1, 100) != HAL_OK)
  //{
  //  printf("Send data fail !! \r\n");
		/* Transmission Error */
  //  Error_Handler();
 // }
	//if(HAL_CAN_GetState(&hcan1) != HAL_CAN_STATE_READY)
 // {
	//	printf("Send data fail1 !! \r\n");
  //  return HAL_ERROR;
  //}		

	return HAL_OK; /* Test Passed */
}
/**
  * @brief  Transmission  complete callback in non blocking mode 
  * @param  CanHandle: pointer to a CAN_HandleTypeDef structure that contains
  *         the configuration information for the specified CAN.
  * @retval None
  */
void HAL_CAN_RxCpltCallback(CAN_HandleTypeDef* hcan1)
{
  //if ((CanHandle->pRxMsg->StdId == 0x321)&&(CanHandle->pRxMsg->IDE == CAN_ID_STD) && (CanHandle->pRxMsg->DLC == 2))
  //{
  //  LED_Display(CanHandle->pRxMsg->Data[0]);
  //  ubKeyNumber = CanHandle->pRxMsg->Data[0];
  //}
  /* Receive */
  if(HAL_CAN_Receive_IT(hcan1, CAN_FIFO0) != HAL_OK)
  {
    /* Reception Error */
    Error_Handler();
  }
	 PushElement(&RxCAN_Queue,hcan1->pRxMsg,1);//�����յ�������ѹ�������
	 //printf("hcan1->pRxMsg->StdId=%d\r\n",RxCAN_Queue.CAN_RxMsg->StdId);
	 //printf("received success !! \r\n");
	 
}
/**
  * @brief  CAN1������
  * @param  None
  * @retval None
  */
/*void CAN_ERR_Handle(void)
{	
	static uint8_t bus_off_cnt = 0;
	
    if( (CANERR_Flag & CAN_IT_LEC) == CAN_IT_LEC)// �ϴδ�����ж�
    {    
        CANERR_Flag &= ~CAN_IT_LEC;
		switch(CAN_ErrorCode)
        {
			case CAN_ErrorCode_NoErr:
				printf("No Error\n\r");
				break;
			case CAN_ErrorCode_StuffErr:
				printf("Stuff Error\n\r");
				break;
			case CAN_ErrorCode_FormErr:
				printf("Form Error\n\r");
				break;
			case CAN_ErrorCode_ACKErr:
				printf("Accknowlege Error\n\r");
				break;
			case CAN_ErrorCode_BitRecessiveErr:
				printf("Bit Recessive Error\n\r");
				break;
			case CAN_ErrorCode_BitDominantErr:
				printf("Bit Dominant Error\n\r");
				break;
			case CAN_ErrorCode_CRCErr:
				printf("CRC Error\n\r");
				break;
			case CAN_ErrorCode_SoftwareSetErr:
				printf("Software Set Error\n\r");
				break;
			default:
				break;
		}
    }
    if( (CANERR_Flag & CAN_IT_EPV) == CAN_IT_EPV ) // ���󱻶�
    {    
        CANERR_Flag &= ~CAN_IT_EPV;
		printf("error Passive\n\r");
    }	
    if( (CANERR_Flag & CAN_IT_EWG) == CAN_IT_EWG) // ���󾯸�
    {   
        CANERR_Flag &= ~CAN_IT_EWG;
		printf("error active\n\r");	
    }
    if( (CANERR_Flag & CAN_IT_ERR) == CAN_IT_ERR)// �д���
    {    
        CANERR_Flag &= ~CAN_IT_ERR;
		printf("error occured\n\r");		
    }
    if( (CANERR_Flag & CAN_IT_BOF) == CAN_IT_BOF)  // ��������
    {   
        CANERR_Flag &= ~CAN_IT_BOF;
		bus_off_cnt++;
		printf("BUS Off times %d \n\r", bus_off_cnt);		
    }
}

//�жϷ�����			    
void USB_LP_CAN1_RX0_IRQHandler(void)
{
  	CanRxMsg RxMessage;
    //u8  i=0;
    
    if(CAN_GetITStatus(CAN1,CAN_IT_FOV0)!=RESET)     //������������ж�
    {
       CAN_ClearITPendingBit(CAN1,CAN_IT_FOV0);     //���жϱ�־λ
       //�������������ô�죬�ǲ���Ҫ���ǵ��µ�
	}
	else if(CAN_GetITStatus(CAN1,CAN_IT_FF0)!=RESET) //�����������ж�
	{
       CAN_ClearITPendingBit(CAN1,CAN_IT_FF0);
	}
	else                                            //�������������ж�
	{	
       CAN_Receive(CAN1,0,&RxMessage);
       PushElement(&RxCAN_Queue,RxMessage,1);
	   printf("ID:%d \r\n",RxMessage.StdId);
    }
}
///////////////CAN �жϷ���//////////////////////////////
void USB_HP_CAN1_TX_IRQHandler(void)
{
	if(CAN_msg_num[0])
    {
       if(CAN_GetITStatus(CAN1,CAN_IT_RQCP0)!=RESET)
	   {
          CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP0);
		  CAN_ITConfig(CAN1,CAN_IT_TME,DISABLE);				//�����ж�����
          CAN_msg_num[0]=0;
	   }
	}
    if(CAN_msg_num[1])
    {
       if(CAN_GetITStatus(CAN1,CAN_IT_RQCP1)!=RESET)
	   {
          CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP1);
		  CAN_ITConfig(CAN1,CAN_IT_TME,DISABLE);				//�����ж�����
          CAN_msg_num[1]=0;
	   }
	}
	if(CAN_msg_num[2])
	{
      if(CAN_GetITStatus(CAN1,CAN_IT_RQCP2)!=RESET)
	   {
          CAN_ClearITPendingBit(CAN1,CAN_IT_RQCP2);
		  CAN_ITConfig(CAN1,CAN_IT_TME,DISABLE);				//�����ж�����
          CAN_msg_num[2]=0;
	   }
	} 
}
*/

















