#include "supervoice.h"
uint8_t isup=1;    //�����ر�־
uint16_t valueup;  //�洢�����ؿ�ʼ������
uint16_t valuedown;//�����½��ؿ�ʼ������
uint8_t updatecount=0;//��ʱ����ת����
uint16_t w=0;      //ʵ���������
uint16_t s = 500 ;//����w��s�ĶԱ�
int HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)//���������Ⱥ���
{
	if(isup)
	{
		valueup = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);//����������أ��洢������
		__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_FALLING);//����Ϊ�½��ؼ��
		isup=0;//��һ��ִ��else���ִ���
	}
	else
	{
		valuedown = HAL_TIM_ReadCapturedValue(htim,TIM_CHANNEL_1);//������½��أ���������㣬���򲻸ı�
		__HAL_TIM_SET_CAPTUREPOLARITY(htim,TIM_CHANNEL_1,TIM_ICPOLARITY_RISING);//����Ϊ�����ؼ��
		isup=1;//��һ��ִ��if����
	     w = valuedown + updatecount*65536 - valueup;//����������
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)//�����ʱ����ת��ʹupdatecount��һ����֤valuedown��Զ����valueup
{
	updatecount++;
}	