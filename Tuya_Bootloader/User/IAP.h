#ifndef __IAP_H__
#define __IAP_H__
#include <stm32f10x.h>

#ifndef IAP_C_//���û�ж���
#define IAP_C_ extern
#else
#define IAP_C_
#endif

typedef  void (*iapfun)(void);				//����һ���������͵Ĳ���.  			   
void iap_load_app(u32 appxaddr);			//��ת��APP����ִ��
#endif

