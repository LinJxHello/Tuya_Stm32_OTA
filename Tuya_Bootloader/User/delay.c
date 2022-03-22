#define __DELAY_C_

#include "usart.h"
#include "main.h"
#include "delay.h"

u32 SysTickCntMs = 0;//��ʱ����ʹ��
u32 SysTickIdleTime = 500;//���ô��ڿ���ʱ��(MS)


/****************
ϵͳ��ʱ����ʼ��
****************/
void DelayInit(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
	SysTick_Config(SystemCoreClock / 1000);//1Ms
}


void delay_ms(u16 ms)
{
  SysTickCntMs = 0;
	while(SysTickCntMs<ms);
}

/*ϵͳ��ʱ���ж�*/
void SysTick_Handler(void)
{	
	
	if(Usart1ReadCnt!=0)//���ڽ��յ�����
	{
	  Usart1IdleCnt++;//�����ۼ�(�ñ����ڴ��ڽ���ʱ����)
		if(Usart1IdleCnt>=SysTickIdleTime)//�ۼӵ�50,���յ�һ������������
		{
		  Usart1IdleCnt = 0;
			Usart1ReadCntCopy = Usart1ReadCnt;
			Usart1ReadCnt = 0;
			Usart1ReadFlage = 1;
			
			if(UpdataStartFlage)
			{
				UpdataReadEndFlage = 1;
			}
		}
	}
}


