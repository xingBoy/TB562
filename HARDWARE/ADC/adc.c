 #include "adc.h"
 #include "delay.h"
 //#include "label.h"
 #include "usart.h"

//#define adcDebug   //AD�ɼ����Դ�ӡ

/*
//�¶�--AD����ֵ ���
//ADvalue=4096*Rt/(Rt+R)
//ADvalue�ǲɼ��ĵ�ѹ��Ӧ��ADֵ
//R=10K
//�¶ȴ���Χ��0~60�� 
*/
 unsigned const int tempvoltage[]={
 0xC42,0xC1C,0xBF5,0xBCE,0xBA5,0xB7D,0xB53,0xB29,0xAFE,0xAD3,
 0xAA8,0xA7C,0xA4F,0xA23,0x9F5,0x9C8,0x99B,0x96D,0x93F,0x911,
 0x8E4,0x8B6,0x888,0x85B,0x82D,0x800,0x7D3,0x7A7,0x77A,0x74E,
 0x723,0x6F8,0x6CD,0x6A3,0x67A,0x651,0x629,0x601,0x5DA,0x5B3,
 0x58D,0x568,0x544,0x520,0x4FD,0x4DB,0x4B9,0x498,0x478,0x459,
 0x43A,0x41C,0x3FF,0x3E2,0x3C6,0x3AB,0x390,0x376,0x35D,0x345,
 0x32D
 };
/*******************************************************************************
 * ����: Adc_Init
 * ����: ��ʼ��ADCͨ��1
 * �β�: ��
 * ����: ��
 * ˵��: ����ת������ͨ��������ͨ�����г�����2��ת��:ADC_Channel_10��ADC_Channel_11
         AD�ɼ��ķֱ���=3.3/4095=0.000805v
         ת��ʱ��=14/12M=1.16us
         TCONV = ����ʱ��+ 239.5��ADC����
 ******************************************************************************/
void  Adc_Init(void)
{ 	
    
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);	  //ʹ��ADC1ͨ��ʱ��
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M

	//PC0 PC1��Ϊģ��ͨ����������                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //ģ����������
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);                                       //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	    //ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ  //���һ����Ҫ�Զ��ͨ������ת������λ�ͱ�������ΪENABLE
	ADC_InitStructure.ADC_ContinuousConvMode =DISABLE;	    //ģ��ת�������ڵ���ת��ģʽ(DISABLE)������ת��ģʽ(ENABLE)   
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 2;	        //˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	            //����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	                        //ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	                    //ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	    //�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	                    //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	        //�ȴ�У׼����
 
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//ʹ��ָ����ADC1�����ת����������

	mq2adjust=MQ2DEFAULT;                           //MQ2����������ʼ����һ��Ĭ�ϵ�У׼ֵ

}				  
/*******************************************************************************
 * ����: Get_Adc
 * ����: ��ȡADC1��chͨ����ADֵ
 * �β�: ADC1ģ������ͨ�� ch:ͨ��ֵ ADC_Channel_0~17
 * ����: ���βɼ�����ADֵ
 * ˵��: ����ʱ��ΪADC_SampleTime_239Cycles5--239.5����
 ******************************************************************************/
u16 Get_Adc(u8 ch)   
{
  	//����ָ��ADC�Ĺ�����ͨ����һ�����У�����ʱ��
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADCͨ��,����ʱ��Ϊ239.5����	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                        //ʹ��ָ����ADC1�����ת����������	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));                      //�ȴ�ת������

	return ADC_GetConversionValue(ADC1);	                            //�������һ��ADC1�������ת�����
}
/*******************************************************************************
 * ����: Get_Adc_Average
 * ����: ��ȡ��ADC1��chͨ���ɼ���ADֵ��ƽ��ֵ
 * �β�: ADC1ģ������ͨ�� ch:ͨ��ֵ ADC_Channel_0~17��times:�ɼ�����
 * ����: �ɼ�����ADֵ��ƽ��ֵ
 * ˵��: ��
 ******************************************************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
   u8 t;
   u32 ADtemp;
   ADtemp=0;
   //for(t=0;t<times;t++)           //���ad1����
   //	  ADbuf1[t]=0;    
	for(t=0;t<times;t++)          //���òɼ�����
   {
      ADtemp+=Get_Adc(ch);     //�ɼ�����ADֵ 
	  delay_ms(1);                //���1ms�ɼ�һ��
   }
   //for(t=0;t<times;t++)
   //{
   //    ADtemp+=ADbuf1[t];         //�õ�ad1ֵ�ܺ�
   //}
   return (ADtemp/times);         //����ֵ��AD�ɼ���ƽ��ֵ
   
}
/*******************************************************************************
* ����:Compare_tempvol
* ����:��ѹ��ӦADֵ����Ϊ�¶�
* �β�: �ɼ�����ADֵ
* ����: �¶�ֵ
* ˵��: ��
Vol=4095*(NTC/(NTC+10K))
******************************************************************************/
u8 Compare_tempvol(u16 Vol)
{ 
	 u8 cmp_cnt; 
	 cmp_cnt =0; 
	 while(Vol<tempvoltage[cmp_cnt]) 
	 { 
	  cmp_cnt++; 
	  if(cmp_cnt>59) 
	  break; 
	 } 
	 return cmp_cnt; 
} 
/*******************************************************************************
* ����:Temp_process
* ����:�ɼ��¶ȴ�������ADֵ���������ݴ��� 
* �β�:ADC1ģ������ͨ�� ch:ͨ��ֵ ADC_Channel_0~17 
* ����:�¶����ݵĲ���
* ˵��:�¶�Խ�ߣ���ѹԽ�ͣ���ֵԽС����֮�¶�Խ�ͣ���ѹԽ�ߣ���ֵԽ��
����ADֵ�������ݴ������������ݿ��Եõ�
1���¶ȴ������Ƿ���ϱ�־��    tempdata.flagerror
2���¶��Ƿ�0������             tempdata.flagBelow0
3���¶��Ƿ񳬹�60��            tempdata.flagmorethan60
4���ɼ��¶ȵ�ADֵADave1���¶�ֵtempdata.DegreeC
******************************************************************************/
Temp Temp_process(u8 ch)
{
   Temp tempdata;
   tempdata.DegreeC=CLEAR;
   tempdata.flagBelow0=CLEAR;
   tempdata.flagerror=CLEAR;
   tempdata.flagmorethan60=CLEAR;
   
   ADave1=Get_Adc_Average(ch,64);              //ad1�ɼ�ͨ��10���ɼ�64��
   if(ADave1<5 || ADave1>4090)                 //�¶ȴ�����û�нӻ��߶̽�
   {
      tempdata.flagerror=ON;                   //�¶ȴ��������ϱ�־
	  tempdata.flagBelow0=OFF;
	  tempdata.flagmorethan60=OFF;
	  #ifdef adcDebug
	  printf("Temp=F1\r\n");                   //���ڴ�ӡ���ϴ���  
      #endif
   }
   else if(ADave1<=4090 && ADave1>0xC42)        //�ɼ����¶�С��0��
   {
	 tempdata.flagerror=OFF;                   //�¶�С��0�ȱ�־
	 tempdata.flagBelow0=ON;
	 tempdata.flagmorethan60=OFF;
	  #ifdef adcDebug
	  printf("Temp=Below 0 degree\r\n");       //���ڴ�ӡ��ʾ�¶�Ϊ0������     
      #endif
   }
   else if(ADave1<0x32D && ADave1>=5)           //�ɼ����¶ȴ���60��
   {
	  tempdata.flagerror=OFF;                  //�¶ȴ���60�ȱ�־
	  tempdata.flagBelow0=OFF;
	  tempdata.flagmorethan60=ON;
	  #ifdef adcDebug
	  printf("Temp=More than 60 degree\r\n");  //���ڴ�ӡ��ʾ�¶�Ϊ60������
      #endif
   }
   else
   {
	 tempdata.flagerror=OFF;                    
	 tempdata.flagBelow0=OFF;
	 tempdata.flagmorethan60=OFF;
	 tempdata.DegreeC = Compare_tempvol(ADave1);    //�¶ȵ�ѹ�����¶�ֵ 
	 #ifdef adcDebug
	 printf("ADave1=%d\n",ADave1);                  //���ڴ�ӡ��ʾ�ɼ�ADֵ
	 printf("Temp=%d��\r\n",tempdata.DegreeC);      //���ڴ�ӡ��ʾ�¶�ֵ
     #endif
   }
   return tempdata;                                 //�����¶ȵĲ���
}
/*******************************************************************************
* ����: MQ2_process
* ����: �ɼ�����������ADֵ�����������ݴ���
* �β�: ADC1ģ������ͨ�� ch:ͨ��ֵ ADC_Channel_0~17
* ����: �������Ĳ���
* ˵��:
���崫����:Ũ��Խ�ߣ���ѹԽ�ߡ�
����������Rs=(Vcc/Vrl -1)*Rl VccΪ��·��ѹ��VrlΪ������4�š�6������ĵ�ѹ���ɼ����ѹ
RlΪ���ص��衣
�ɼ����ѹVrl=(Rl/(Rl+Rs))*Vcc
һ�㴿���Ŀ����о�̬�ĵ�ѹֵΪ0.3~1V֮�䡣
����п�ȼ������������򶯾�̬�ĵ�ѹ��Ϊ0.3v~0.6v֮�䡣
����ADֵ�������ݴ������������ݿ��Եõ�:
1����̬��ѹ��Ӧ��ADֵstamq2vol���ڴ����������ϵ紫����Ԥ�Ⱥ�ɼ�һ�Σ�����ɼ��Ķ�̬��ѹ�뾲̬��ѹ�Ա�
2����̬��ѹ��Ӧ��ADֵstrmq2vol, 
3��MQ2���������Ƿ���ϱ�־��Mq2Data.error
4��MQ2���������ɼ����Ŀ����Ƿ�������־ Mq2Data.Normal

Ũ�ȵķ�Χ:

С��0.3v ---��        

0.3v~0.4v---���

0.5v~0.6v---�ж�

����0.6v ---�ض�

��ѹ��ֵ��Ӧ��AD��ֵ(һ���ѹ�Ĳ�ֵ��0.3~0.6֮�����������߿�ȼ����ĳ���):
0.1V---125   
0.2V---250
0.3V---380
0.4V---500
0.5V---625
0.6V---750
******************************************************************************/
Mq2 MQ2_process(u8 ch) //������������
{
   Mq2 MQ2data;
      MQ2data.error=CLEAR;
	  MQ2data.mq2section=CLEAR;
	  MQ2data.Normal=CLEAR;
	  MQ2data.stamq2vol=CLEAR;
      MQ2data.strmq2vol=CLEAR;

      MQ2data.stamq2vol=mq2adjust;               //У׼MQ2�ľ�̬��ѹֵ
      MQ2data.strmq2vol=Get_Adc_Average(ch,64);  //ad2�ɼ�ͨ��11���ɼ�64�� �ɼ��Ķ�̬��ѹֵ

	  #ifdef adcDebug
	  printf("strmq2vol=%d,stamq2vol=%d \r\n",MQ2data.strmq2vol,MQ2data.stamq2vol); //���ڴ�ӡ��ʾ�ɼ�ADֵ
      #endif
	  if((MQ2data.strmq2vol>4090 || MQ2data.strmq2vol<5)||(MQ2data.stamq2vol>4090 || MQ2data.stamq2vol<5))
      {
		  MQ2data.error=ON;                           //�����������ֹ��ϱ�־
          #ifdef adcDebug
		  printf("The MQ2 is fault\r\n");             //���ڴ�ӡ��ʾ��������������Ϣ
          #endif
	  }
	  else   //���������ɼ�����	
      {
            MQ2data.error=OFF;                          

		   if(MQ2data.strmq2vol>MQ2data.stamq2vol)
		   {
			  MQ2data.question=OFF;              
			  MQ2data.mq2section=MQ2data.strmq2vol-MQ2data.stamq2vol;    //��̬��ѹ�붯̬��ѹ֮��Ĳ�ֵ
		   }
		   else
           {
              MQ2data.question=ON;                                       //��̬��ѹ���ڶ�̬��ѹ��־
			  MQ2data.mq2section=0;                                      //��̬��ѹ���ڶ�̬��ѹ�����
		   }
      }
    return MQ2data;     //����������������Ĳ���
}

/*******************************************************************************
* ����:MQ2_Calibration
* ����:��������У׼
* �β�:�ɼ���ͨ��
* ����:У׼��״̬
* ˵��:��
һ�㴿���Ŀ����о�̬�ĵ�ѹֵΪ0.3~1V֮�䣬����Ӧ��ADֵ�� 370~1250֮��
ʹ��MQ2_Calibration������ҪԤ��3���Ӳ��ܲɼ�
******************************************************************************/
u8 MQ2_Calibration(u8 ch)
{
   static u8 adjstate=0;                         //У׼״̬
   u8 i=0;                                       //У׼����
   static u16 mq2backups=0;                      //У׼ֵ����
   
    for(i=0;i<3;i++)
    {
   	     mq2backups=Get_Adc_Average(ch,64);      //У׼һ����Ϊ��ֵ̬
		  if(mq2backups<370||mq2backups>1250)    //У׼ֵ�жϣ�0.3v~1v֮��
		  {
			 adjstate=ADJFAIL;                   //У׼ʧ��   
		  }
		  else
		  {
			  mq2adjust=mq2backups;             //����У׼ֵ
	          adjstate=ADJSUCCESS;              //У׼�ɹ�
	          #ifdef adcDebug
               printf("mq2adjust=%d \r\n",mq2adjust);     //���ڴ�ӡУ׼��״̬
              #endif

			  break;                            //������ǰ��ѭ�� 
		  }
     }
	 return adjstate;                          //����У׼��״̬
}


















