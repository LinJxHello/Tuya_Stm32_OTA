#include "usart.h"
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE {
	int handle; 
}; 
FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x){ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f){      
	while((USART_n->SR&0X40)==0);//ѭ������,ֱ���������   
    USART_n->DR = (u8) ch;      
	return ch;
}
#endif 

u8 USART2_RX_BUF[USART2_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART2_RX_STA=0;       //����״̬���	  

/*
USART2ר�õ�printf����
��ͬʱ����2�����ϴ���ʱ��printf����ֻ����������֮һ����������Ҫ�Դ�������printf����
���÷�����USART2_printf("123"); //��USART2�����ַ�123
*/
void USART2_printf (char *fmt, ...){ 
	char buffer[USART2_REC_LEN+1];  // ���ݳ���
	u8 i = 0;	
	va_list arg_ptr;
	va_start(arg_ptr, fmt);  
	vsnprintf(buffer, USART2_REC_LEN+1, fmt, arg_ptr);
	while ((i < USART2_REC_LEN) && (i < strlen(buffer))){
        USART_SendData(USART2, (u8) buffer[i++]);
        while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET); 
	}
	va_end(arg_ptr);
}


void USART2_Init(u32 bound){ //����2��ʼ��������
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
		 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); //ʹ��UART2����GPIOA��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE); //ʹ�ܴ��ڵ�RCCʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3; //����USART2��RX�ӿ���PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //����USART2��TX�ӿ���PA2
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

   //USART2 ��ʼ������
	USART_InitStructure.USART_BaudRate = bound;//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	USART_Init(USART2, &USART_InitStructure); //��ʼ������
	USART_ITConfig(USART2, USART_IT_RXNE,DISABLE);//����ENABLE/�ر�DISABLE�ж�
	USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 

}