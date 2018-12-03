#include "gpio.h "
#include "queen.h"


/* Private variables ---------------------------------------------------------*/
extern UART_HandleTypeDef huart1;
extern I2C_HandleTypeDef hi2c1;

//extern UART_HandleTypeDef huart2; //����
extern uint16_t fre_control1;
extern uint16_t fre_control2;
extern uint16_t fre_control3;
extern uint16_t fre_control4;
extern uint16_t fre_control5;
extern uint16_t fre_control6;
extern char time_flag1;
extern int i1;
extern char time_flag2;
extern int i2;
extern char time_flag3;
extern int i3;
extern char time_flag4;
extern int i4;
extern char time_flag5;
extern int i5;
extern char time_flag6;
extern int i6;

//uint8_t frtext[2]; //text
extern char tim_no;		

void Gpio_PowerControl(void)
{
	  RxMsgTypeDef Uart1_Rxbuf;//Uart1_Txbuf;
    uint8_t val;
	  uint16_t fre=1;
		float fr;    //���������
		
	  if (PopElement(&RxUart1_Queue,&Uart1_Rxbuf) == TRUE)  //����������������TCP�ͻ��˷�������
	  {
				//******************��ѹ���ݽ��ղ���***********************************
				 if (Uart1_Rxbuf.Data[0] == 0x01)//0x01 ͨ��1����ͷ����ʾͨ��һ�Ŀ�������ͷ
				 {			 
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);					
						val = 180 - (2*Uart1_Rxbuf.Data[1]); 									//Uart1_Rxbuf.Data[1] ��������
						HAL_I2C_Mem_Write(&hi2c1, ad5141_1_write_address, 0x10, I2C_MEMADD_SIZE_8BIT, &val, 1, 0x10);	
						
				 }
				 else if (Uart1_Rxbuf.Data[0] == 0x02)
				 {
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);	
						val = 180 - (2*Uart1_Rxbuf.Data[1]);
						HAL_I2C_Mem_Write(&hi2c1, ad5141_2_write_address, 0x10, I2C_MEMADD_SIZE_8BIT, &val, 1, 0x10);
				 }
				 else if (Uart1_Rxbuf.Data[0] == 0x03)
				 {
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);	 
						val = 180 - (2*Uart1_Rxbuf.Data[1]); 
						HAL_I2C_Mem_Write(&hi2c1, ad5141_3_write_address, 0x10, I2C_MEMADD_SIZE_8BIT, &val, 1, 0x10);  
				 }
				 else if (Uart1_Rxbuf.Data[0] == 0x04)
				 {
					 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);			 
						val = 180 - (2*Uart1_Rxbuf.Data[1]);
						HAL_I2C_Mem_Write(&hi2c1, ad5141_4_write_address, 0x10, I2C_MEMADD_SIZE_8BIT, &val, 1, 0x10);
				 }
				 else if (Uart1_Rxbuf.Data[0] == 0x05)
				 {
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);	
						val = 180 - (2*Uart1_Rxbuf.Data[1]);
						HAL_I2C_Mem_Write(&hi2c1, ad5141_5_write_address, 0x10, I2C_MEMADD_SIZE_8BIT, &val, 1, 0x10);
				 }
				 else if (Uart1_Rxbuf.Data[0] == 0x06)
				 {
						HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);	 
						val = 180 - (2*Uart1_Rxbuf.Data[1]);
						HAL_I2C_Mem_Write(&hi2c1, ad5141_6_write_address, 0x10, I2C_MEMADD_SIZE_8BIT, &val, 1, 0x10);
				 }			 
					//************************************************************
				else if (Uart1_Rxbuf.Data[0] == 0xaa)  //�ر��źŴ���
				 {
								switch (Uart1_Rxbuf.Data[1])
								{
									case 0x01:						//ͨ��1�ر��ź�
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
														time_flag1=0;	 
														break;
									case 0x02:
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
														time_flag2=0;	
														break;
									case 0x03:
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
														time_flag3=0;	
														break;
									case 0x04:
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
														time_flag4=0;	
														break;
									case 0x05:
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
														time_flag5=0;	
														break;
									case 0x06:
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);
														time_flag6=0;	
														break;
									case 0xa1:						//ͨ��1�����źŴ���
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);																									
														if(tim_no==0) time_flag1=0;
														else 
														{
															time_flag1=1;	i1=0;
														}
														break;
									case 0xa2:						
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_RESET);
														if(tim_no==0) time_flag2=0;
														else 
														{
															time_flag2=1;	i2=0;
														}
														break;
									case 0xa3:
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
														if(tim_no==0) time_flag3=0;
														else 
														{
															time_flag3=1;	i3=0;
														}
														break;
									case 0xa4:
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
														if(tim_no==0) time_flag4=0;
														else 
														{
															time_flag4=1;	i4=0;
														}
														break;
									case 0xa5:
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);
														if(tim_no==0) time_flag5=0;
														else 
														{
															time_flag5=1;	i5=0;
														}
														break;
									case 0xa6:
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_RESET);
														if(tim_no==0) time_flag6=0;
														else 
														{
															time_flag6=1;	i6=0;
														}
														break;
									case 0xaa:					//��λ���ر��ź�
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, GPIO_PIN_SET);
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
														HAL_GPIO_WritePin(GPIOC, GPIO_PIN_5, GPIO_PIN_SET);	
														time_flag1=0;	
														time_flag2=0;	
														time_flag3=0;	
														time_flag4=0;	
														time_flag5=0;	
														time_flag6=0;	
														break;
									default : break;
								}
				 }

				//********************Ƶ�ʿ��ƽ��ղ���******************************
				switch (Uart1_Rxbuf.Data[0])		//0xT0,ͨ��T����ͷ
				{
					case 0x10 :
						if(Uart1_Rxbuf.Data[1] == 0x30) 			//��0���д���
							{								
								 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);	//ltc1624ʹ��
								 time_flag1=0; tim_no=0;
							}
						else 
							{								
								if(Uart1_Rxbuf.Data[2] == 0x0a) 				fre= (uint32_t )Uart1_Rxbuf.Data[1]-0x30;					//���ո�λ�����ݴ���			 
								else if (Uart1_Rxbuf.Data[3] == 0x0a) 	fre= ((Uart1_Rxbuf.Data[1]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		//����ʮλ�����ݴ���
								else if (Uart1_Rxbuf.Data[4] == 0x0a)  	fre= ((Uart1_Rxbuf.Data[1]-0x30)*100) + ((Uart1_Rxbuf.Data[2]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		//���հ�λ�����ݴ���
								else if (Uart1_Rxbuf.Data[5] == 0x0a)		fre= 1000;    //1000Hz								
								fr=10000/fre;		//��λ�����Ʋ�������ʱ����ʱ�����Ĺ�ϵ
								fre_control1=(int)( fr+0.5) > (int) fr ? (int)fr+1:(int)fr; //�������							
								time_flag1=1;		
								i1=0;  
								tim_no=1;		//��Ƶ�ʿ��Ʊ�־
							}
							break ;
					case 0x20:
										if(Uart1_Rxbuf.Data[1] == 0x30) 
										{		 
											 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);	
											 time_flag2=0;	tim_no=0;
										}
									else 
										{
											if(Uart1_Rxbuf.Data[2] == 0x0a) 				fre= (uint32_t )Uart1_Rxbuf.Data[1]-0x30;					 
											else if (Uart1_Rxbuf.Data[3] == 0x0a) 	fre= ((Uart1_Rxbuf.Data[1]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[4] == 0x0a)  	fre= ((Uart1_Rxbuf.Data[1]-0x30)*100) + ((Uart1_Rxbuf.Data[2]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[5] == 0x0a)		fre= 1000;    											
											fr=10000/fre;
											fre_control2=(int)( fr+0.5) > (int) fr ? (int)fr+1:(int)fr; 							
											time_flag2=1;						
											i2=0;		
											tim_no=1;	
										}
										break ;
						case 0x30:
										if(Uart1_Rxbuf.Data[1] == 0x30) 
										{		 
											 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);	
											 time_flag3=0;	tim_no=0;
										}
									else 
										{
											if(Uart1_Rxbuf.Data[2] == 0x0a) 				fre= (uint32_t )Uart1_Rxbuf.Data[1]-0x30;					 
											else if (Uart1_Rxbuf.Data[3] == 0x0a) 	fre= ((Uart1_Rxbuf.Data[1]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[4] == 0x0a)  	fre= ((Uart1_Rxbuf.Data[1]-0x30)*100) + ((Uart1_Rxbuf.Data[2]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[5] == 0x0a)		fre= 1000;    											
											fr=10000/fre;
											fre_control3=(int)( fr+0.5) > (int) fr ? (int)fr+1:(int)fr; 							
											time_flag3=1;						
											i3=0;		
											tim_no=1;
										}
										break ;
						case 0x40:
										if(Uart1_Rxbuf.Data[1] == 0x30) 
										{		 
											 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);	
											 time_flag4=0;	tim_no=0;
										}
									else 
										{
											if(Uart1_Rxbuf.Data[2] == 0x0a) 				fre= (uint32_t )Uart1_Rxbuf.Data[1]-0x30;					 
											else if (Uart1_Rxbuf.Data[3] == 0x0a) 	fre= ((Uart1_Rxbuf.Data[1]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[4] == 0x0a)  	fre= ((Uart1_Rxbuf.Data[1]-0x30)*100) + ((Uart1_Rxbuf.Data[2]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[5] == 0x0a)		fre= 1000;    											
											fr=10000/fre;
											fre_control4=(int)( fr+0.5) > (int) fr ? (int)fr+1:(int)fr; 							
											time_flag4=1;						
											i4=0;		
											tim_no=1;
										}
										break ;
						case 0x50:
										if(Uart1_Rxbuf.Data[1] == 0x30) 
										{		 
											 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);	
											 time_flag5=0;	tim_no=0;
										}
									else 
										{
											if(Uart1_Rxbuf.Data[2] == 0x0a) 				fre= (uint32_t )Uart1_Rxbuf.Data[1]-0x30;					 
											else if (Uart1_Rxbuf.Data[3] == 0x0a) 	fre= ((Uart1_Rxbuf.Data[1]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[4] == 0x0a)  	fre= ((Uart1_Rxbuf.Data[1]-0x30)*100) + ((Uart1_Rxbuf.Data[2]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[5] == 0x0a)		fre= 1000;    											
											fr=10000/fre;
											fre_control5=(int)( fr+0.5) > (int) fr ? (int)fr+1:(int)fr; 							
											time_flag5=1;						
											i5=0;						
											tim_no=1;
										}
										break ;
						case 0x60:
										if(Uart1_Rxbuf.Data[1] == 0x30) 
										{		 
											 HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);	
											 time_flag6=0;	tim_no=0;
										}
									else 
										{
											if(Uart1_Rxbuf.Data[2] == 0x0a) 				fre= (uint32_t )Uart1_Rxbuf.Data[1]-0x30;					 
											else if (Uart1_Rxbuf.Data[3] == 0x0a) 	fre= ((Uart1_Rxbuf.Data[1]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[4] == 0x0a)  	fre= ((Uart1_Rxbuf.Data[1]-0x30)*100) + ((Uart1_Rxbuf.Data[2]-0x30)*10) + (Uart1_Rxbuf.Data[2]-0x30);		
											else if (Uart1_Rxbuf.Data[5] == 0x0a)		fre= 1000;    											
											fr=10000/fre;
											fre_control6=(int)( fr+0.5) > (int) fr ? (int)fr+1:(int)fr; 							
											time_flag6=1;						
											i6=0;						
											tim_no=1;
										}
										break ;
					default : break;
				}
			//=======================================================================================					 
		}                          
}




