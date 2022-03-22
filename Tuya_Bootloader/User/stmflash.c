
#include "stmflash.h"
#include "stdio.h"
#include <stdint.h>
#include <string.h>

uint8_t nbuff_AAAA[4] = {0xAA,0xAA,0xAA,0xAA};
uint8_t cBuffer[1024];//�����ݴ�����

/**
* @brief  ��ȡFlash
* @waing  
* @param  address  ��ȡ�ĵ�ַ
* @param  pBuffer  ��ȡ������
* @param  Numlengh ��ȡ�ĳ���
* @example 1kb = 8λ������
**/
void ReadFlash(uint32_t address,uint8_t *Nbuffer ,uint32_t  length)
{
	uint32_t temp = 0;
	uint32_t count = 0;
	while(count < length)
	{
		temp = *((volatile uint32_t *)address);//ȡ��ַ�е�ֵ���Ĵ������32λ
		*Nbuffer++ = (temp & 0xff);//ȡ��8λ�ŵ�����[0]
		count++;
    if(count >= length)
      break;
	  *Nbuffer++ = (temp >> 8) & 0xff;//����8λ ȡ��8λ�ŵ�����[1]
		count++;
		if(count >= length)
      break;
		*Nbuffer++ = (temp >> 16) & 0xff;//����8λ ȡ��8λ�ŵ�����[2]
		count++;
		if(count >= length)
      break;
		*Nbuffer++ = (temp >> 24) & 0xff;//����8λ ȡ��8λ�ŵ�����[3] 32λ����ȡ��
		count++;
		if(count >= length)
      break;
		address += 4;//��ַ + 4 ������һ��32λ
	}
}
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
  FLASH_Unlock();//flashд��������
	for(i = 0; i < Numlengh;i+= 4)
	{
		temp =  (uint32_t)pBuffer[i+3]<<24;//���λ 0xff000000
		temp |=  (uint32_t)pBuffer[i+2]<<16;//0x00ff0000
		temp |=  (uint32_t)pBuffer[i+1]<<8;//0x0000ff00
		temp |=  (uint32_t)pBuffer[i];//���λ0x000000ff
		FLASH_ProgramWord(address+i,temp);//�����ݰ��ֽ�д��flash
	}
  FLASH_Lock();//flash����
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
/**
* @brief  ��ȡFlash���Գ���
* @waing  
* @param  address  ��ȡ�ĵ�ַ
* @example 
**/
uint32_t ReadFlashTest(uint32_t addr)
{
	uint32_t temp = *(__IO uint32_t*)(addr);
	return temp;
}
/**
* @brief  �̼�����
* @waing  
* @param  SourceAddress  Ŀ���ַ
* @param  TargetAddress  Դ��ַ
* @example 
**/
void update_firmware(uint32_t SourceAddress,uint32_t TargetAddress)
{
//�ȶ�  �����  ��д  
  uint16_t i;
	volatile uint32_t nK ;
	nK	= (TargetAddress - SourceAddress)/1024;//����Ҫȡ���ݵĴ�С ��λKb
	for(i = 0;i < nK;i++)
	{
		memset(cBuffer,0xff,sizeof(cBuffer));
		ReadFlash(TargetAddress+i*0x0400,cBuffer,1024);//ÿ��ȡԴ��ַ1K����
		EarseFlash_1K(SourceAddress+i*0x0400);//����Ŀ���ַ1K
		WriteFlash(SourceAddress+i*0x0400,cBuffer,1024);//��Ŀ�ĵ�ַд��1K
	}

}