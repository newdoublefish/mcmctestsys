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
#ifndef __HARDWARECONFIG_H__
#define __HARDWARECONFIG_H__

#include <stdlib.h> 
#include "toolbox.h"
#include "hardwareEntity.h"
#ifdef __cplusplus
   extern "C" {
#endif
void DisplayHardwarePanelAsTabPanel(int panel);

HARDWARE GetHardware(void);

#ifdef __cplusplus
   }
#endif

//void SetUUTList(ListType uutList);
#endif
