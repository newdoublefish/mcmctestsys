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
	   
#define APPEND_INFO(panelHandle,msg)   \
		do{ 		 \
		    char temp[1024]={0};          \
			sprintf((char*)temp,"%s\n",(char*)msg);	   \
			AppendInfo(panelHandle,(char *)temp);							\
	     }while(0)

#define APPEND_INFO_FORMAT(panelHandle,format,...)   \
		do{ 		 \
		    char temp[1024]={0};          \
			sprintf((char*)temp,""format"\n",##__VA_ARGS__);	   \
			AppendInfo(panelHandle,(char *)temp);							\
	     }while(0)

	   
typedef enum
{
    TEST_RESULT_ALLPASS=0,//����ȫ��ͨ��
	TEST_RESULT_SOMEPASS=1,//���Բ���ͨ��������û��ͨ��
	TEST_RESULT_ERROR=2 //���Գ��ֹ���
}METHODRET;  //���Է�������

typedef METHODRET (*TEST_METHOD)(TestGroup,EUT,HashTableType); 
typedef METHODRET (*TEST_FUNCTION)(TestGroup,EUT,HashTableType,int); 
typedef BOOL (*PROTOCOL_INIT)(char *); 
typedef void (*TEST_PREPARE)(void);
typedef int (*ON_CREATE_TPS_PANEL)(char *name); 
	   
typedef struct{
    char tpsName[MAX_TPS_NAME_LEN];
	void *protocolInit;//Э���ʼ������,ϵͳ������ʱ��ִ��һ��
	void *testPrepare;//����׼��
	void *onTestFinish;//����׼�� 
	int tpsPanelHandle;
	ON_CREATE_TPS_PANEL createTpsPanel;
	TEST_METHOD autoTestFunction;//�Զ����Ժ��� 
	TEST_FUNCTION testFunction;//�Զ����Ժ��� 
//	TEST_METHOD manualTestFunction;//�ֶ����Ժ��� 	
}TPS;

typedef TPS (*REGISTER_TPS_FUNCTION)(void);

int initTps(void);
TPS newTps(char *name);
int getTps(char *tpsName,TPS *tpsPtr);
void TpsInitProtocol(void);
void AppendInfo(int tpsPanelHandle,char *msg);

void TpsPrepareTest(void);
#ifdef __cplusplus
    {
#endif
		
#endif

