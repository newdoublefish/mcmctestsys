/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ��ݺ��º��տƼ��ɷ����޹�˾��Ȩ���У�δ����Ȩ����ֹ����
 // �����п�����ʹ�á�
 //
 // �ļ�����hardwareConfig.h
 // �ļ���������: Ӳ����Դѡ��
 //
 // 
 // ������ʶ�������� 20150721
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
