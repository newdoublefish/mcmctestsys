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

#ifndef __TPSHELPER_H__
#define __TPSHELPER_H__

#include <stdlib.h>  
#include "eutConfig.h"
#include "testGroupInit.h" 
#include "debug.h"
#include "log.h"
#include "convertTool.h"
#include "resultUtil.h"

#ifdef __cplusplus
   extern "C" {
#endif
	   
#define MAX_TPS_NAME_LEN 30
	   
typedef enum
{
    TEST_RESULT_ALLPASS=0,//����ȫ��ͨ��
	TEST_RESULT_SOMEPASS=1,//���Բ���ͨ��������û��ͨ��
	TEST_RESULT_ERROR=2 //���Գ��ֹ���
}METHODRET;  //���Է�������

typedef METHODRET (*TEST_METHOD)(TestGroup,EUT,HashTableType);  
typedef BOOL (*PROTOCOL_INIT)(char *); 
typedef void (*TEST_PREPARE)(void);
	   
typedef struct{
    char tpsName[MAX_TPS_NAME_LEN];
	void *protocolInit;//Э���ʼ������
	void *testPrepare;//����׼��
	void *testFunction;//���Ժ��� 
}TPS;

typedef TPS (*REGISTER_TPS_FUNCTION)(void);

int initTps(void);
TPS newTps(char *name);
int getTps(char *tpsName,TPS *tpsPtr);

void TpsPrepareTest(void);
#ifdef __cplusplus
    {
#endif
		
#endif

