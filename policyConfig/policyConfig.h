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
#ifndef __POLICYCOMMON_H__
#define __POLICYCOMMON_H__

#include <stdlib.h> 
#include "toolbox.h"
#include "policyEntity.h"

#ifdef __cplusplus
    extern "C" {
#endif

ListType GetCollectList(void);

void DisplayStrategyPanelAsTabPanel(int panelHandle);

int isStrategyNeedToSave(void);

#ifdef __cplusplus
    }
#endif


#endif

