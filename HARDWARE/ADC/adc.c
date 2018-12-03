 #include "adc.h"
 #include "delay.h"
 //#include "label.h"
 #include "usart.h"

//#define adcDebug   //AD采集调试打印

/*
//温度--AD采样值 查表
//ADvalue=4096*Rt/(Rt+R)
//ADvalue是采集的电压对应的AD值
//R=10K
//温度处理范围是0~60℃ 
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
 * 名称: Adc_Init
 * 功能: 初始化ADC通道1
 * 形参: 无
 * 返回: 无
 * 说明: 单次转换规则通道，规则通道序列长度有2个转换:ADC_Channel_10、ADC_Channel_11
         AD采集的分辨率=3.3/4095=0.000805v
         转换时间=14/12M=1.16us
         TCONV = 采样时间+ 239.5个ADC周期
 ******************************************************************************/
void  Adc_Init(void)
{ 	
    
	ADC_InitTypeDef ADC_InitStructure; 
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_ADC1,ENABLE);	  //使能ADC1通道时钟
 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M

	//PC0 PC1作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //模拟输入引脚
	GPIO_Init(GPIOC, &GPIO_InitStructure);	

	ADC_DeInit(ADC1);                                       //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	    //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式  //如果一次需要对多个通道进行转换，这位就必须设置为ENABLE
	ADC_InitStructure.ADC_ContinuousConvMode =DISABLE;	    //模数转换工作在单次转换模式(DISABLE)、连续转换模式(ENABLE)   
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 2;	        //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	            //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	                        //使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	                    //使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	    //等待复位校准结束
	
	ADC_StartCalibration(ADC1);	                    //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	        //等待校准结束
 
	//ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能

	mq2adjust=MQ2DEFAULT;                           //MQ2烟雾传感器初始化给一个默认的校准值

}				  
/*******************************************************************************
 * 名称: Get_Adc
 * 功能: 获取ADC1的ch通道的AD值
 * 形参: ADC1模拟输入通道 ch:通道值 ADC_Channel_0~17
 * 返回: 单次采集到的AD值
 * 说明: 采样时间为ADC_SampleTime_239Cycles5--239.5周期
 ******************************************************************************/
u16 Get_Adc(u8 ch)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_239Cycles5);	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		                        //使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC));                      //等待转换结束

	return ADC_GetConversionValue(ADC1);	                            //返回最近一次ADC1规则组的转换结果
}
/*******************************************************************************
 * 名称: Get_Adc_Average
 * 功能: 获取到ADC1的ch通道采集到AD值的平均值
 * 形参: ADC1模拟输入通道 ch:通道值 ADC_Channel_0~17，times:采集次数
 * 返回: 采集到的AD值的平均值
 * 说明: 无
 ******************************************************************************/
u16 Get_Adc_Average(u8 ch,u8 times)
{
   u8 t;
   u32 ADtemp;
   ADtemp=0;
   //for(t=0;t<times;t++)           //清除ad1缓存
   //	  ADbuf1[t]=0;    
	for(t=0;t<times;t++)          //设置采集次数
   {
      ADtemp+=Get_Adc(ch);     //采集到的AD值 
	  delay_ms(1);                //间隔1ms采集一次
   }
   //for(t=0;t<times;t++)
   //{
   //    ADtemp+=ADbuf1[t];         //得到ad1值总和
   //}
   return (ADtemp/times);         //返回值是AD采集的平均值
   
}
/*******************************************************************************
* 名称:Compare_tempvol
* 功能:电压对应AD值查表成为温度
* 形参: 采集到的AD值
* 返回: 温度值
* 说明: 无
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
* 名称:Temp_process
* 功能:采集温度传感器的AD值并进行数据处理 
* 形参:ADC1模拟输入通道 ch:通道值 ADC_Channel_0~17 
* 返回:温度数据的参数
* 说明:温度越高，电压越低，阻值越小。反之温度越低，电压越高，阻值越大。
根据AD值进行数据处理，有以下数据可以得到
1、温度传感器是否故障标志。    tempdata.flagerror
2、温度是否0度以下             tempdata.flagBelow0
3、温度是否超过60度            tempdata.flagmorethan60
4、采集温度的AD值ADave1，温度值tempdata.DegreeC
******************************************************************************/
Temp Temp_process(u8 ch)
{
   Temp tempdata;
   tempdata.DegreeC=CLEAR;
   tempdata.flagBelow0=CLEAR;
   tempdata.flagerror=CLEAR;
   tempdata.flagmorethan60=CLEAR;
   
   ADave1=Get_Adc_Average(ch,64);              //ad1采集通道10，采集64次
   if(ADave1<5 || ADave1>4090)                 //温度传感器没有接或者短接
   {
      tempdata.flagerror=ON;                   //温度传感器故障标志
	  tempdata.flagBelow0=OFF;
	  tempdata.flagmorethan60=OFF;
	  #ifdef adcDebug
	  printf("Temp=F1\r\n");                   //串口打印故障代码  
      #endif
   }
   else if(ADave1<=4090 && ADave1>0xC42)        //采集的温度小于0度
   {
	 tempdata.flagerror=OFF;                   //温度小于0度标志
	 tempdata.flagBelow0=ON;
	 tempdata.flagmorethan60=OFF;
	  #ifdef adcDebug
	  printf("Temp=Below 0 degree\r\n");       //串口打印提示温度为0度以下     
      #endif
   }
   else if(ADave1<0x32D && ADave1>=5)           //采集的温度大于60度
   {
	  tempdata.flagerror=OFF;                  //温度大于60度标志
	  tempdata.flagBelow0=OFF;
	  tempdata.flagmorethan60=ON;
	  #ifdef adcDebug
	  printf("Temp=More than 60 degree\r\n");  //串口打印提示温度为60度以上
      #endif
   }
   else
   {
	 tempdata.flagerror=OFF;                    
	 tempdata.flagBelow0=OFF;
	 tempdata.flagmorethan60=OFF;
	 tempdata.DegreeC = Compare_tempvol(ADave1);    //温度电压查表成温度值 
	 #ifdef adcDebug
	 printf("ADave1=%d\n",ADave1);                  //串口打印显示采集AD值
	 printf("Temp=%d℃\r\n",tempdata.DegreeC);      //串口打印显示温度值
     #endif
   }
   return tempdata;                                 //返回温度的参数
}
/*******************************************************************************
* 名称: MQ2_process
* 功能: 采集烟雾传感器的AD值，并进行数据处理
* 形参: ADC1模拟输入通道 ch:通道值 ADC_Channel_0~17
* 返回: 检测气体的参数
* 说明:
气体传感器:浓度越高，电压越高。
传感器电阻Rs=(Vcc/Vrl -1)*Rl Vcc为回路电压，Vrl为传感器4脚、6脚输出的电压即采集点电压
Rl为负载电阻。
采集点电压Vrl=(Rl/(Rl+Rs))*Vcc
一般纯净的空气中静态的电压值为0.3~1V之间。
如果有可燃气体或者烟雾则动静态的电压差为0.3v~0.6v之间。
根据AD值进行数据处理，有以下数据可以得到:
1、静态电压对应的AD值stamq2vol，在纯净空气中上电传感器预热后采集一次，后面采集的动态电压与静态电压对比
2、动态电压对应的AD值strmq2vol, 
3、MQ2烟雾传感器是否故障标志，Mq2Data.error
4、MQ2烟雾传感器采集到的空气是否正常标志 Mq2Data.Normal

浓度的范围:

小于0.3v ---无        

0.3v~0.4v---轻度

0.5v~0.6v---中度

大于0.6v ---重度

电压差值对应的AD差值(一般电压的差值在0.3~0.6之间就有烟雾或者可燃气体的出现):
0.1V---125   
0.2V---250
0.3V---380
0.4V---500
0.5V---625
0.6V---750
******************************************************************************/
Mq2 MQ2_process(u8 ch) //烟雾传感器处理
{
   Mq2 MQ2data;
      MQ2data.error=CLEAR;
	  MQ2data.mq2section=CLEAR;
	  MQ2data.Normal=CLEAR;
	  MQ2data.stamq2vol=CLEAR;
      MQ2data.strmq2vol=CLEAR;

      MQ2data.stamq2vol=mq2adjust;               //校准MQ2的静态电压值
      MQ2data.strmq2vol=Get_Adc_Average(ch,64);  //ad2采集通道11，采集64次 采集的动态电压值

	  #ifdef adcDebug
	  printf("strmq2vol=%d,stamq2vol=%d \r\n",MQ2data.strmq2vol,MQ2data.stamq2vol); //串口打印显示采集AD值
      #endif
	  if((MQ2data.strmq2vol>4090 || MQ2data.strmq2vol<5)||(MQ2data.stamq2vol>4090 || MQ2data.stamq2vol<5))
      {
		  MQ2data.error=ON;                           //烟雾传感器出现故障标志
          #ifdef adcDebug
		  printf("The MQ2 is fault\r\n");             //串口打印显示烟雾传感器故障信息
          #endif
	  }
	  else   //烟雾传感器采集正常	
      {
            MQ2data.error=OFF;                          

		   if(MQ2data.strmq2vol>MQ2data.stamq2vol)
		   {
			  MQ2data.question=OFF;              
			  MQ2data.mq2section=MQ2data.strmq2vol-MQ2data.stamq2vol;    //静态电压与动态电压之间的差值
		   }
		   else
           {
              MQ2data.question=ON;                                       //静态电压大于动态电压标志
			  MQ2data.mq2section=0;                                      //静态电压大于动态电压的情况
		   }
      }
    return MQ2data;     //返回烟雾传感器处理的参数
}

/*******************************************************************************
* 名称:MQ2_Calibration
* 功能:烟雾传感器校准
* 形参:采集的通道
* 返回:校准的状态
* 说明:无
一般纯净的空气中静态的电压值为0.3~1V之间，即对应的AD值在 370~1250之间
使用MQ2_Calibration函数需要预热3分钟才能采集
******************************************************************************/
u8 MQ2_Calibration(u8 ch)
{
   static u8 adjstate=0;                         //校准状态
   u8 i=0;                                       //校准计数
   static u16 mq2backups=0;                      //校准值备份
   
    for(i=0;i<3;i++)
    {
   	     mq2backups=Get_Adc_Average(ch,64);      //校准一次作为静态值
		  if(mq2backups<370||mq2backups>1250)    //校准值判断，0.3v~1v之间
		  {
			 adjstate=ADJFAIL;                   //校准失败   
		  }
		  else
		  {
			  mq2adjust=mq2backups;             //备份校准值
	          adjstate=ADJSUCCESS;              //校准成功
	          #ifdef adcDebug
               printf("mq2adjust=%d \r\n",mq2adjust);     //串口打印校准的状态
              #endif

			  break;                            //跳出当前的循环 
		  }
     }
	 return adjstate;                          //返回校准的状态
}


















