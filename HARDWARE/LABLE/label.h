
//������������

#ifndef label_h
#define label_h
#include "typedef.h"
#include "userdef.h"
#ifdef LABEL
#define extern
#endif

extern uint16 test;
extern uint16 test1;
extern uint8 test2;
extern uint8 test5;
//extern uint8 dir;           //
extern uint16 led0pwmval;
extern uint16 adcx;       //AD�ɼ��������
extern float temp;        //AD�ɼ��������

extern uint16 ADbuf1[64];        //ad1��������a
extern uint16 ADave1;    //�ɼ�ad1��ƽ��ֵ
extern uint16 ADave2;    //�ɼ�ad2��ƽ��ֵ 
extern uint16 msecnum;     //�����

extern uint16 mq2adjust;  //MQ2��������У׼ֵ
extern uint16 mq2adjust1;

extern uint16 countover;   //�����������
extern uint16 speed;
     
extern uint32 frq;           //����Ƶ��ֵ            
extern uint16 DutyCycle;

extern uint16 res;
extern uint8 canbuf[8];
extern uint8 canbuf1[8];
extern uint8 status;
extern uint8 sendcount;
extern uint8 CAN_msg_num[3];    //����������
//extern uint8 KeyStatus;
/*typedef struct
{
  UTIME16 testone;
}_tm_4ms16;
extern _tm_4ms16 tm_4ms16;*/
extern Temp TempData;      //�����¶ȵĲ���
extern Mq2  Mq2Datas;       //��������Ĳ���
 
extern union FLAGS flag0; 

#define flagtest            flag0.byte
#define flagfirstmq2        flag0.bit.bit0     //MQ2���������ϵ�Ԥ�ȱ�־
//#define flagmq2             flag0.bit.bit1     //MQ2����������̬�ɼ���־
//#define flagcalibration     flag0.bit.bit2     //У׼�Ƿ�������־
//#define flagmq2Normal       flag0.bit.bit2   //MQ2������������Ƿ�������־
//#define flagmq2fault        flag0.bit.bit3   //MQ2�����������ϱ�־
//#define flagtempfault       flag0.bit.bit4   //�¶ȴ���������״̬��־
//#define flagtempBelow0      flag0.bit.bit5   //0�����±�־
//#define flagtempmorethan60  flag0.bit.bit6   //����60�����ϱ�־

//extern union TEMP TempData;  //�����¶ȵĲ���
//extern union MQ2  Mq2Datas;   //��������Ĳ���




#endif

