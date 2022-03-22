#include "main.h"
#include "System.h"
#include "delay.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include "stmflash.h"

typedef void (*APP_FUNC)();//����һ������ָ��
APP_FUNC jumpapp; //����ָ������һ������ָ��
/*****************************************************************************
�������� : JumpToApp
�������� : ������ת
������� : ��
���ز��� : ��
ʹ��˵�� : ��
*****************************************************************************/
void JumpToApp(uint32_t app_address)                  
{
  if(((*(vu32*)app_address)&0x2FFE0000)==0x20000000)
	//app_address����û�����Ķ�ջ��ַ
	//����û�����ջ����ַ�Ƿ�Ϸ�.
	//�ж�APP���еĳ���ջ��ָ������SRAM����
	//https://my.oschina.net/u/4581400/blog/4821070
	{ 
		volatile uint32_t JumpAPPaddr;//ȡ��Ҫ��ת����ĸ�λ��ַ
		if (((*(uint32_t*)app_address) &0x2FFE0000 ) == 0x20000000)//�ٴ��ж�
		{
			JumpAPPaddr = *(volatile uint32_t*)(app_address + 4);//ջ����ַ+4��ŵ��Ǹ�λ��ַ
			jumpapp = (APP_FUNC) JumpAPPaddr;
			__set_MSP(*(__IO uint32_t*) app_address);//�û������ջ����ַ��Ϊջ��ָ��
			jumpapp();//����PCָ��Ϊ��λ��ַ
		}
	}
}
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	/* stm32ϵͳ���� */
	NVIC_Configuration();

	while(1)
	{
		if(ReadFlashTest(UPFLAGADDR) == 0x55555555)  //������־
		{
				update_firmware(APP1ADDR,APP2ADDR);//�̼���������
				EarseFlash_1K(UPFLAGADDR);//����������־λ
				WriteFlash(UPFLAGADDR,nbuff_AAAA,4);//д��AAAAAAAA
				NVIC_SystemReset();//ϵͳ��λ
		}
		else 
		{
				JumpToApp(APP1ADDR);
		}
	}
}
