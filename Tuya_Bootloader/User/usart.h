#ifndef __USART_H
#define __USART_H
#include <stm32f10x.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include "stdio.h"	
#ifndef USART_C_//���û�ж���
#define USART_C_ extern
#else
#define USART_C_
#endif

#define USART_n		USART2  //����ʹ��printf����
#define USART2_REC_LEN  			200  	//����USART2�������ֽ���
extern u8  USART2_RX_BUF[USART2_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з�
extern u16 USART2_RX_STA;         		//����״̬���	
//��������
void USART2_Init(u32 bound);//����2��ʼ��������
void USART2_printf(char* fmt,...); //����2��ר��printf����

#endif


