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
#ifndef __LOGIN_CONFIGBIZ_H__
#define __LOGIN_CONFIGBIZ_H__
#include "userint.h"
#ifdef __cplusplus
   extern "C" {
#endif
	   
typedef struct
{
	char userName[50];
	char password[50];
	char url[50];
	char method[50];	
	int remember;
}tLoginConfig;
 

tLoginConfig getLoginConfig(void);
HRESULT saveLoginConfig(tLoginConfig s);

#ifdef __cplusplus
   }
#endif

#endif

