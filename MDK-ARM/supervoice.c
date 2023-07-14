#include "supervoice.h"
uint8_t isup=1;    //上升沿标志
uint16_t valueup;  //存储上升沿开始计数点
uint16_t valuedown;//储存下降沿开始计数点
uint8_t updatecount=0;//定时器反转计数
uint16_t w=0;      //实际脉冲宽度
uint16_t s = 500 ;//用作w与s的对比
int HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//检测脉冲宽度函数
{
	if(isup)
	{
		valueup = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);//如果是上升沿，存储计数点
		__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//设置为下降沿检测
		isup=0;//下一轮执行else部分代码
	}
	else
	{
		valuedown = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);//如果是下降沿，储存计数点，否则不改变
		__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//设置为上升沿检测
		isup=1;//下一轮执行if代码
	     w = valuedown + updatecount*65536 - valueup;//检测脉冲宽度
	}
	if(w>s)
	{
		return 1;
	}
	if(w<=s)
	{
		return 0;
	}
}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//如果定时器反转，使updatecount加一，保证valuedown永远大于valueup
{
	updatecount++;
}	