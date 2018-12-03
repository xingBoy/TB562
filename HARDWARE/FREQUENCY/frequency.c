#include "frequency.h"
#include "queen.h"
#include "gpio.h"

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
extern char tim_no;

/******************************************************************************************
函数名：			HAL_TIM_PeriodElapsedCallback
功能：				调频控制
传入参数：		无
传出参数：		无
返回值：			无
说明：				TIM3回调函数
*******************************************************************************************/
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(tim_no ==1)
		{
				if( time_flag1 == 1 )	//通道1调频
				{
							i1++;
							if( i1==fre_control1 )
							{
									i1=0;
									HAL_GPIO_TogglePin (GPIOC ,GPIO_PIN_0 );
							}
				}
				if( time_flag2 == 1 )
				{
							i2++;
							if( i2==fre_control2 )
							{
									i2=0;
									HAL_GPIO_TogglePin (GPIOC ,GPIO_PIN_1 );
							}
				}
				if( time_flag3 == 1 )
				{
							i3++;
							if( i3==fre_control3 )
							{
									i3=0;
									HAL_GPIO_TogglePin (GPIOC ,GPIO_PIN_2 );
							}
				}
				if( time_flag4 == 1 )
				{
							i4++;
							if( i4==fre_control4 )
							{
									i4=0;
									HAL_GPIO_TogglePin (GPIOC ,GPIO_PIN_3 );
							}
				}
				if( time_flag5 == 1 )
				{
							i5++;
							if( i5==fre_control5 )
							{
									i5=0;
									HAL_GPIO_TogglePin (GPIOC ,GPIO_PIN_4 );
							}
				}
				if( time_flag6 == 1 )
				{
							i6++;
							if( i6==fre_control6 )
							{
									i6=0;
									HAL_GPIO_TogglePin (GPIOC ,GPIO_PIN_5 );
							}
				}
		}
}


