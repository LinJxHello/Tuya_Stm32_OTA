#define IAP_C_
#include "iap.h"

iapfun jump2app; 


//addr:ջ����ַ
__asm void MSR_MSP(u32 addr) 
{
    MSR MSP, r0 			//set Main Stack value
    BX r14
}

//��ת��Ӧ�ó����
//appxaddr:�û�������ʼ��ַ.
void iap_load_app(u32 appxaddr)
{
	if(((*(vu32*)appxaddr)&0x2FFE0000)==0x20000000)	//���ջ����ַ�Ƿ�Ϸ�.
	{ 
		jump2app=(iapfun)*(vu32*)(appxaddr+4);		//����APP����ĸ�λ�жϺ�����ַ
		MSR_MSP(*(vu32*)appxaddr);					//��ʼ��APP��ջָ��,��APP����Ķ�ջ�����ع�,����˵���·���RAM
		jump2app();									//ִ��APP�ĸ�λ�жϺ���,���ձ����ת��APP��main����
	}
}		 

