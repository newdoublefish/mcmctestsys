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
#ifndef __FTPCONFIGBIZ_H__
#define __FTPCONFIGBIZ_H__
#include "userint.h"
#ifdef __cplusplus
   extern "C" {
#endif
	   
typedef struct
{
	char server[50];
	char userName[50];
	char password[50];
	int mode;
}tFtpConfig;
 

tFtpConfig getFtpConfig(void);
HRESULT saveFtpConfig(tFtpConfig s);

#ifdef __cplusplus
   }
#endif

#endif

