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

#ifndef __AUTOTEST_H__
#define __AUTOTEST_H__

#include <stdlib.h>  

#ifdef __cplusplus
    extern "C" {
#endif
typedef enum{
	ENUM_TEST_PANEL_AUTO=0,
	ENUM_TEST_PANEL_MANUAL=1,
	ENUM_TEST_PANEL_NONE
}ENUMETestPanel;
void DisplayAutoTestPanel(ListType itemList,ListType deviceList,ListType collectList,ENUMETestPanel type);
#ifdef __cplusplus
    }
#endif

#endif

