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
#ifndef __FTPCONFIG_VIEW_H__
#define __FTPCONFIG_VIEW_H__
#include "userint.h"
#include "ftpConfigBiz.h"
#ifdef __cplusplus
   extern "C" {
#endif
	   
int DisplayFtpConfigView (int panel);
tFtpConfig GetFtpConfig();
#ifdef __cplusplus
   }
#endif

#endif
