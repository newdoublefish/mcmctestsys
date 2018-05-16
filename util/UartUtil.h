/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ㶫������綯����Ӫ�ɷ����޹�˾��Ȩ���У�δ����Ȩ��
 // ��ֹ���������п�����ʹ�á�
 //
 // �ļ�����
 // �ļ���������: 
 //
 // 
 // ������ʶ�������� 
 //
 // �޸ı�ʶ��
 // �޸�������
 //-------------------------------------------------------------------------*/

#ifndef __UART_UTIL_H__
#define __UART_UTIL_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "eutEntity.h"   
		
typedef struct
{
    RSCONFIG rsconfig;
	unsigned char *rxBuffer;
	int rxLen;
	int maxRxLen;
	int timeOut;
}tUartRx;

tUartRx *createUartRx(RSCONFIG config,int maxRxLen,int timeOut);
BOOL releaseUartRx(tUartRx *tur);
BOOL doUartRxProcess(tUartRx *ptr);
		
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __log_H__ */
