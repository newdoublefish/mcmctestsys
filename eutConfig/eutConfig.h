/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ��ݺ��º��տƼ��ɷ����޹�˾��Ȩ���У�δ����Ȩ����ֹ����
 // �����п�����ʹ�á�
 //
 // �ļ�����eutConfig.h
 // �ļ���������: eut�豸����
 //
 // 
 // ������ʶ�������� 20150721
 //
 // �޸ı�ʶ��
 // �޸�������
 //-------------------------------------------------------------------------*/
#ifndef __EUTCOMMON_H__
#define __EUTCOMMON_H__
#include <stdlib.h> 
#include "toolbox.h"
#include "eutEntity.h"
#ifdef __cplusplus
   extern "C" {
#endif


ListType getEutList(void);

void DisplayEutPanelAsTabPanel(int panel);

#ifdef __cplusplus
   }
#endif

#endif
