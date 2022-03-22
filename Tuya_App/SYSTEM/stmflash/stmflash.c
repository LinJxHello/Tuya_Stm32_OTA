
#include "stmflash.h"

unsigned char nbuff_5555[] = {0x55,0x55,0x55,0x55};
unsigned char nbuff_AAAA[] = {0xAA,0xAA,0xAA,0xAA};

/**
* @brief  д��Flash
* @waing  
* @param  address  д��ĵ�ַ
* @param  pBuffer  д�������
* @param  Numlengh д��ĳ���
* @example 
**/
void WriteFlash(uint32_t address,uint8_t *pBuffer,uint32_t Numlengh)
{
	uint32_t i ,temp;
  FLASH_Unlock();

	for(i = 0; i < Numlengh;i+= 4)
	{
		temp =  (uint32_t)pBuffer[i+3]<<24;
		temp |=  (uint32_t)pBuffer[i+2]<<16;
		temp |=  (uint32_t)pBuffer[i+1]<<8;
		temp |=  (uint32_t)pBuffer[i];
		FLASH_ProgramWord(address+i,temp);//�����ݰ��ֽ�д��flash
	}
  FLASH_Lock();
}
/**
* @brief  ����1K��С��Flash
* @waing  
* @param  address  �����ĵ�ַ
* @example 
**/
int EarseFlash_1K(uint32_t address)
{
  FLASH_Unlock();
	//��������ĵ�ַ�ǲ���1K�ı���
	if(address % 1024 == 0)
	{
		FLASH_ErasePage(address);
	}
	else
	{
		return 0;
	}
  FLASH_Lock();
	return 1;
}

