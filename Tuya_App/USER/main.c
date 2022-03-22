#include "stm32f10x.h"
#include "wifi.h"
#include "usart.h"
#include "delay.h"
#include "stmflash.h"

extern void Connect_Wifi(void);
extern uint8_t Key_Scan(GPIO_TypeDef* GPIOx,uint16_t GPIO_Pin);

 void Delay(u32 count)
 {
   u32 i=0;
   for(;i<count;i++);
 }
 int main(void)
 {	
  GPIO_InitTypeDef  GPIO_InitStructure;
	 
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO, ENABLE);	    //ʹ��PB�˿�ʱ��
	 
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE); //��PB4�ܽŽ�������ӳ��

  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //IO���ٶ�Ϊ50MHz
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_4;	            //LED1-->PE.5�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  	       //��ʼ��GPIO
  GPIO_SetBits(GPIOB,GPIO_Pin_6); 
	GPIO_SetBits(GPIOB,GPIO_Pin_4);

	 //PB5�ܽ���Ϊ ������ ����ܽ�
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_5;//KEY0-KEY1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOE4,3
	 
	 SCB->VTOR = FLASH_BASE_ADDR | 0x4000;//�����ж�ƫ��
	 
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	 uart_init(9600);	 //���ڳ�ʼ��Ϊ115200
	 wifi_protocol_init();//wifiЭ���ʼ��

  while(1)
	{
		Connect_Wifi();
		wifi_uart_service();
	  //GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	  //GPIO_SetBits(GPIOE,GPIO_Pin_5);
		//Delay(3000000);
		//GPIO_SetBits(GPIOB,GPIO_Pin_5);
		//GPIO_ResetBits(GPIOE,GPIO_Pin_5);
		//Delay(3000000);
	}
 }
