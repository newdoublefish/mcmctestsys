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
#ifndef __SETTINGCONFIG_H__
#define __SETTINGCONFIG_H__
#include "settingEntity.h"
#ifdef __cplusplus
   extern "C" {
#endif

HRESULT DisplaySettingPanelAsTabPanel(int panelHandle);     

SETTING GetSetting(void);

#ifdef __cplusplus
   }
#endif

#endif
