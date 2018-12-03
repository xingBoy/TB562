#ifndef __USERDEF_H__
#define __USERDEF_H__
//#ifndef typedef_h
//#define typedef_h
#include "typedef.h"
/*date type defination*/
typedef signed 	 char  		int8;
typedef signed 	 short 	  int16;
typedef signed 	 long  		int32;
typedef unsigned char  		uint8;
typedef unsigned short 		uint16;
typedef unsigned long  	  uint32;
typedef unsigned int      UINT16;

struct FLAG
{
   /*uint8 bit7:1;
   uint8 bit6:1;
   uint8 bit5:1;
   uint8 bit4:1;
   uint8 bit3:1;
   uint8 bit2:1;
   uint8 bit1:1;
   uint8 bit0:1;*/
    uint8 bit0:1;
    uint8 bit1:1;
    uint8 bit2:1;
    uint8 bit3:1;
    uint8 bit4:1;
    uint8 bit5:1;
    uint8 bit6:1;
    uint8 bit7:1;      
};
union FLAGS
{
    uint8 byte;
    struct FLAG bit;
};
/*
typedef struct
{
 // UINT16 fover:1;
 // UINT16 frequest:1;
 // UINT16 count:14;
  UINT16 count:14;         //��14λ
  UINT16 frequest:1;       //��15λ
  UINT16 fover:1;          //��16λ
}TIME16;*/
typedef struct
{
   UINT16 DegreeC;
   uint8  flagerror;
   uint8  flagBelow0;
   uint8  flagmorethan60;
}Temp;        //�����¶Ȳ���
typedef struct
{
   UINT16 strmq2vol;   //��̬��ѹֵADֵ
   UINT16 stamq2vol;   //��̬��ѹֵADֵ 
   UINT16 mq2section;  //��̬��ѹֵ�붯̬��ѹֵ֮��Ĳ�ֵ
   uint8  error;       //���ϱ�־
   uint8  Normal;      //���б�־
   uint8  question;    //��̬��ѹ���ڶ�̬��ѹ��־
}Mq2;
/*typedef union
{
   UINT16 word;
   TIME16 time;
}UTIME16;*/

/*struct MQDATA
{
   //uint32 strvol:13;       //��̬��ѹֵ
   //uint32 stavol:13;       //��̬��ѹֵ
   uint8 fault:1;         //���ϱ�־
   uint8 Normal:1;        //����״̬��־
};
union MQ2
{
    uint8 byte8;
	struct MQDATA flag;
};*/

#endif
