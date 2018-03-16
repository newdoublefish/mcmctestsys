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
#ifndef __TEST_PROJECT_H__
#define __TEST_PROJECT_H__

#include "cvidef.h"
#ifdef __cplusplus
    extern "C" {
#endif
#define MAX_TEST_PROJECT_NAME 100
typedef struct{
	char projectName[MAX_TEST_PROJECT_NAME];
	char projectPath[MAX_PATHNAME_LEN];
}tTestProject;

BOOL newTestProject();
BOOL loadTestProject();
BOOL saveTestProjectInfo();
tTestProject *getCurrentProject(); 
#ifdef __cplusplus
    }
#endif


#endif
