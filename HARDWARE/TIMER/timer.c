#include "timer.h"
#include "label.h"
#include "can.h"
  	 
/*******************************************************************************
 * ����: TIM3_Time_Init
 * ����: TIM3��ʱ�жϳ�ʼ��
 * �β�: arr:�Զ���װֵ��psc:ʱ��Ԥ��Ƶ��
 * ����: ��
 * ˵��: ��Ҫ���ڶ�ʱ�ж�
 * ����ʱ��ѡ��ΪAPB1��2����ȴAPB1��ʱ��Ϊ36M
 * ��ʱʱ��T=(arr+1)*(psc+1)/72M 
 ******************************************************************************/
void TIM3_Time_Init(u16 arr,u16 psc)
{
    //GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
    //TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);       //TIM3ʱ��ʹ��
	
	//��ʱ��TIM3��ʼ��
	TIM_TimeBaseStructure.TIM_Period = arr;                    //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	
	TIM_TimeBaseStructure.TIM_Prescaler =psc;                  //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);            //����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;            //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                            //��ʼ��NVIC�Ĵ���
    
	TIM_ClearFlag(TIM3,TIM_IT_Update);                         //������жϱ�־λ
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);                   //ʹ��ָ����TIM3�ж�,��������ж�
	TIM_Cmd(TIM3, ENABLE);                                     //ʹ��TIM3					 
}
/*******************************************************************************
 * ����: TIM5_PWMIn_Init
 * ����: TIM5PWM���벶���ʼ��
 * �β�: arr:�趨�������Զ���װֵ psc:Ԥ��Ƶ��ֵ
 * ����: ��
 * ˵��: ��Ҫ���ڲ����ⲿ�����Ƶ�ʼ�ռ�ձ�
 * ���벶�������ַ�ʽ:��ͨ���벶���PWM���벶��
 * PWM���벶����Լ�������Ƶ�ʼ�ռ�ձ�
 ******************************************************************************/
void TIM5_PWMIn_Init(u16 arr,u16 psc)
{	 
	TIM_ICInitTypeDef  TIM5_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);	             //ʹ��TIM5ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);             //ʹ��GPIOAʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;            //PAO ����   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		
	//��ʼ����ʱ��TIM5	 
	TIM_TimeBaseStructure.TIM_Period = arr;                          //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	                     //Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;          //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;      //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);              
  
	//��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel =TIM_Channel_1;                 //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
  	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 //�����ز���
  	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 
  	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //���������Ƶ,����Ƶ 
  	TIM5_ICInitStructure.TIM_ICFilter = 0x00;                        //IC1F=0000 ���������˲��� ���˲�
  	//TIM_ICInit(TIM5, &TIM5_ICInitStructure);                       //���벶��ģʽ
    TIM_PWMIConfig(TIM5, &TIM5_ICInitStructure);                     //PWM���벶��ģʽ

	TIM_SelectInputTrigger(TIM5,TIM_TS_TI1FP1);                      //ѡ��IC1Ϊʼ�մ���Դ

    TIM_SelectSlaveMode(TIM5,TIM_SlaveMode_Reset);                   //TIM��ģʽ�������źŵ����������³�ʼ���������ʹ����Ĵ����ĸ����¼�
 
    TIM_SelectMasterSlaveMode(TIM5,TIM_MasterSlaveMode_Enable);      //������ʱ���ı�������
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;                  //TIM5�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;         //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =2;                //�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                         
     
	TIM_ClearFlag(TIM5,TIM_IT_CC1);                                  //����жϱ�־λ
    TIM_Cmd(TIM5,ENABLE); 	                                         //ʹ�ܶ�ʱ��5
	TIM_ITConfig(TIM5,TIM_IT_CC1,ENABLE);                            //����CC1IE�����ж�	
}
/**************************************************************** 
 * ��������TIM8_PWMOut_Init  
 * ����:  TIM8 PWM�����ʼ�� 
 *      CH1:��� T=2.5ms(f=1/2.5ms=400Hz)  D=0.6��PWM��(�ߵ�ƽ��ǰ���͵�ƽ�ں�) 
 *       
 *      ����һ��ͨ��T��TIMxCLK��ʱ��Դȷ��TIM_Period��TIM_Prescaler  
 *          T=(TIM_Period+1)*(TIM_Prescaler+1)/TIMxCLK=2.5ms  
 *          ��Ϊ TIM_Period<65535������ TIM_Prescaler>1,�� TIM_Prescaler=2 
 *          ���� TIM_Period=59999=0xEA5F 
 *      �����������TIM_Period��ֵ���ߵ͵�ƽ���Ⱥ�D��ȷ��CCR��TIM_OCPolarity 
 *          CH1����ΪD=0.6���ȸߺ�ͣ� 
 *               ����CCR1=(TIM_Period+1)* D=36000;TIM_OCPolarity=TIM_OCPolarity_High 
 * �β�:  arr:�Զ���װֵ(0--65535)��psc:ʱ��Ԥ��Ƶ��(0--65535), duty:�趨ռ�ձ�ֵ
 * ����ֵ:��
 * ˵������Ҫ����PWM���
 * PWM��Ƶ��=72M/((1+arr)*(1+psc))
 *    ռ�ձ�=duty/(1+arr)
 ***************************************************************/  
void TIM8_PWMOut_Init(u16 arr,u16 psc,u16 duty)  
{  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;  
    TIM_OCInitTypeDef  TIM_OCInitStructure;  
	GPIO_InitTypeDef GPIO_InitStructure; 
   
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_TIM8, ENABLE); //ʹ��GPIOCʱ�ӡ���ʱ��TIM8ʱ��
 
	   
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_9; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;                           // �����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	  
    /* Time base configuration */                                            
    TIM_TimeBaseStructure.TIM_Period = arr;  
    TIM_TimeBaseStructure.TIM_Prescaler = psc;                                //����Ԥ��Ƶ 
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;                              //����ʱ�ӷ�Ƶϵ��������Ƶ  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;               //���ϼ������ģʽ  
    TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);  
    /* PWM1 Mode configuration: Channel4 */  
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;                         //����ΪPWMģʽ1  
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;                
    TIM_OCInitStructure.TIM_Pulse = duty;                                     //��������ֵ�������������������ֵʱ����ƽ��������  
    TIM_OCInitStructure.TIM_OCPolarity =TIM_OCPolarity_High;                  //����ʱ������ֵС��CCR1ʱΪ�ߵ�ƽ  
    TIM_OC4Init(TIM8, &TIM_OCInitStructure);                                       
    TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  
   
    TIM_CtrlPWMOutputs(TIM8,ENABLE);                                          //MOE �����ʹ��//ֻ�и߼���ʱ��TIM1��TIM8��Ҫ���������ʹ��
	TIM_ARRPreloadConfig(TIM8, ENABLE);                                       //ʹ��TIM8���ؼĴ���ARR  
    /* TIM8 enable counter */  
    TIM_Cmd(TIM8, ENABLE);                                                    //ʹ��TIM8
} 
/*******************************************************************************
 * ����: ControlPWMOut1()
 * ����: ����PWM�����Ƶ�ʼ�ռ�ձ�
 * �β�: TIMx:ͨ��ѡ�� arr:�趨�������Զ���װֵ psc:Ԥ��Ƶ��ֵ duty:ռ�ձ�
 * ����: ��
 * ˵��: ��Ҫ����PWM���������������PWM��Ƶ�ʼ�ռ�ձ�
 * Ƶ��f=72M/((arr+1)*(psc+1)), arr:0~65535��psc:0~65535
 * ռ�ձ�dutyvalue=duty/(arr+1)   ����:ռ�ձ�Ϊ0.6����duty=0.6*(arr+1)
 * ��ʱ��8ͨ��4���PWM
 * ���ڵ�Ƶ�ʷ�Χ:1HZ~24MHZ
 * ���ڵ�ռ�ձȵķ�Χ:0~100%
 * Ƶ�����:
 ******************************************************************************/
void ControlPWMOut1(TIM_TypeDef* TIMx,u16 arr,u16 psc,u16 duty)
{
   TIM_SetAutoreload(TIMx,arr);                                   //arr,psc���������Ƶ��
   TIM_PrescalerConfig(TIMx,psc,TIM_PSCReloadMode_Immediate);
   TIM_SetCompare4(TIMx,duty);                                    //ռ�ձ�
}
/*******************************************************************************
 * ����: TIM8_In_Init
 * ����: TIM8 ��ͨ���벶���ʼ��
 * �β�: arr:�趨�������Զ���װֵ psc:Ԥ��Ƶ��ֵ
 * ����: ��
 * ˵��: ��Ҫ���ڲ��������Ƶ��
 * ���벶�������ַ�ʽ:��ͨ���벶���PWM���벶��
 * ���벶����Լ�������Ƶ��
 ******************************************************************************/
void TIM8_In_Init(u16 arr,u16 psc)
{	 
	TIM_ICInitTypeDef  TIM8_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
   	NVIC_InitTypeDef NVIC_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_TIM8, ENABLE); //ʹ��GPIOCʱ�ӡ���ʱ��TIM8ʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_9;                   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;                    //PC9 ����   
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);			                     //PC9 ����
	
	//��ʼ����ʱ��8 TIM8	 
	TIM_TimeBaseStructure.TIM_Period = arr;                          //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	                     //Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;          //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;      //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); 
  
	//��ʼ��TIM8���벶�����
	TIM8_ICInitStructure.TIM_Channel =TIM_Channel_4;                 //CC4S=01 	ѡ������� IC4ӳ�䵽TI4��
  	TIM8_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 //�����ز���
  	TIM8_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI4��
  	TIM8_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	         //���������Ƶ,����Ƶ 
  	TIM8_ICInitStructure.TIM_ICFilter = 0x00;                        //IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM8, &TIM8_ICInitStructure);                         //���벶��ģʽ
    
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM8_CC_IRQn;               //TIM8�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =0;         //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;                //�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;                  //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                          
  
    TIM_Cmd(TIM8,ENABLE); 	                                         //ʹ�ܶ�ʱ��8
	TIM_ITConfig(TIM8,TIM_IT_CC4,ENABLE);                            //����CC4IE�����ж�	
}
/*******************************************************************************
 * ����: TIM2_Extcount_Init
 * ����: ��ʱ��2�ⲿ����ģʽ��ʼ��
 * �β�: ��
 * ����: ��
 * ˵��: 
   ��ʱ���ⲿ����������TIM_ETR���Ž����ⲿ����,���ڼ�������
 ******************************************************************************/
void TIM2_ExtCount_Init(void)
{	 
   
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	   //ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);      //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0;                  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;              //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);						   //PA0 ����
   
	//��ʼ����ʱ��2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;                 //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =0; 	               //Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;    //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);            //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	 
		
	//TIM_ITRxExternalClockConfig(TIM2,TIM_TS_ETRF);             //����TIMx�ڲ�����Ϊ�ⲿʱ��ģʽ
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0);//����TIMxΪ�ⲿʱ��ģʽ2
    TIM_SetCounter(TIM2,0); 
	//�ж����ȼ�NVIC����
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;            //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =0;          //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;            //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);                            //��ʼ��NVIC�Ĵ���
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);                       //��������жϱ�־,��ΪֻҪ������ʱ������������һ���ж�
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                   //ʹ��ָ����TIM2�ж�,��������ж�

  	TIM_Cmd(TIM2,ENABLE); 	                                   //ʹ�ܶ�ʱ��2
}
/*******************************************************************************
 * ����: TIM3_IRQHandler
 * ����: TIM3��ʱ�жϷ�����
 * �β�: ��
 * ����: ��
 * ˵��: 1S��ʱ�ж�һ��
 ******************************************************************************/
void TIM3_IRQHandler(void)
{
    u16 TIM2CH1_CAPTURE_VAL;
	if(TIM_GetITStatus(TIM3,TIM_IT_Update)!= RESET)            //���TIM3�����жϷ������
	{
		  TIM_ClearITPendingBit(TIM3,TIM_IT_Update);           //���TIM3�����жϱ�־ 
		  if(msecnum<180)
		  {
		     msecnum++;
             //msecnum=CLEAR;
			 if(msecnum==180)                                  //�ϵ�3���Ӻ�:5ms*35000=175s �Ϳ��Խ���MQ2���崫������У׼
			 flagfirstmq2=ON;                                  //����У׼��־
		  }
        //res=Can_Send_Msg(canbuf,8);//����8���ֽ� 
        //printf("res=%d \r\n",res);
		TIM2CH1_CAPTURE_VAL=TIM_GetCounter(TIM2);
        if(countover>0)                                        //����������������                            
	    {
		   frq=TIM2CH1_CAPTURE_VAL+countover*65536;
		   countover=0;
        }
		else
	    {
           frq=TIM2CH1_CAPTURE_VAL;
	    }
		TIM_SetCounter(TIM2,0);
	}
}
/*******************************************************************************
 * ����: TIM2_IRQHandler
 * ����: TIM2 ��������жϷ�����
 * �β�: ��
 * ����: ��
 * ˵��: ����������ж�
 ******************************************************************************/
void TIM2_IRQHandler(void)     
{
     if(TIM_GetITStatus(TIM2,TIM_IT_Update)!= RESET)           //���TIM2�����жϷ������
	{
		  TIM_ClearITPendingBit(TIM2,TIM_IT_Update);           //���TIM2�����жϱ�־ 
		  if(countover<65535)                         
		  countover++;                                         //�������������
    }
}
/*******************************************************************************
 * ����: TIM5_IRQHandler
 * ����: ͨ�ö�ʱ��5�жϷ�����
 * �β�: ��
 * ����: ��
 * ˵��: PWM���벶��Ƶ�ʼ�ռ�ձȼ���
 ******************************************************************************/
void TIM5_IRQHandler(void)
{
   u16 IC2Value;
   if(TIM_GetITStatus(TIM5,TIM_IT_CC1)!= RESET)                //���TIM5�����жϷ������
  {
	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1);                   //����жϱ�־λ
	IC2Value = TIM_GetCapture1(TIM5);                          //��ȡIC1����Ĵ�����ֵ����ΪPWM���ڵļ���ֵ
	  if(IC2Value != 0)
	  {
	   
	     DutyCycle =(TIM_GetCapture2(TIM5) * 100) / IC2Value;  //��ȡIC2����Ĵ�����ֵ��������ռ�ձ�
	     frq=72000000/IC2Value;                                //����PWMƵ�ʡ�
	  }
	  else
	  {
	     DutyCycle = 0;
	     frq= 0;
	  }
  }
}
/*******************************************************************************
 * ����: TIM8_CC_IRQHandler
 * ����: TIM8 ���ش����жϷ�����
 * �β�: ��
 * ����: ��
 * ˵��: 
 ******************************************************************************/
void TIM8_CC_IRQHandler(void)
{
   static u8 risecount=0;
   static u16 timecount=0,timecount1=0;
   u32 countsection;    //��������
   if(TIM_GetITStatus(TIM8,TIM_IT_CC4)!=RESET)
   {
	   TIM_ClearITPendingBit(TIM8,TIM_IT_CC4); //����жϱ�־λ
	   if(risecount==0)
       {
           risecount=1;
		   timecount=TIM_GetCapture4(TIM8);    //��ȡ��һ�������ص�CNT��
	   }
	   else if(risecount==1)
	   {
           risecount=0;
		   timecount1=TIM_GetCapture4(TIM8);   //��ȡ�ڶ��������ص�CNT��
           if(timecount<timecount1)
           {
               countsection=timecount1-timecount;
		   }
		   else if(timecount>timecount1)
		   {
               countsection=(0xffff-timecount)+timecount1;
		   }
		   else
		   	   countsection=0;

		   frq=1000000/(countsection);        //����Ƶ��
	   }
   }
}















