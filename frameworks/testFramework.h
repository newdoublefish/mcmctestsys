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

#ifndef __AUTOCONTEXT_H__
#define __AUTOCONTEXT_H__

#include <stdlib.h>  
#include "eutConfig.h"
#include "testGroupInit.h"
#include "policyEntity.h"


#ifdef __cplusplus
    extern "C" {
#endif
		
typedef enum
{
	TYPE_AUTO=0x00,
	TYPEMANUAL=0x01
}TESTType;

typedef enum
{
    TEST_RESULT_ALL_PASS=0x01,
	TEST_RESULT_SOME_PASS=0x02,
	TEST_ERROR=0x03,
	TEST_RESULT_QUIT=0x04, 
	TEST_RESULT_NON_DEF 
}ENUMTestResult;	
		

typedef enum
{
     TEST_IDLE=0x01,//����
     TEST_RUN=0x02,//������
     TEST_STOP=0x03,//��ͣ
     TEST_FINISH=0x04,//�������
	 TEST_NON_DEF
}TESTstate;	


typedef struct
{
	int objectIndex;
	int panelHandle;//��ʾ���
    EUT device; //�����豸��Ϣ
	HashTableType resultHashTable;//���Խ��
	void *enginePtr;
	void *onObjectPanelCreateListener;   //������崴��
	void *onObjectPanelInitListener;   //��������ʼ��
	void *onObjectGroupTestListener;     //�������
	void *onObjectCollectTestListener;     //������� 
	void *onResultShowListener;	   //��������ʾ
	void *onObjectTestErrorListener;       //������Գ���
	void *onObjectTestFinish;//�ö���������
	BOOL errorFlag;			              //������ڻ�����������־���������Ͳ���������²�����
	int totalFinishedTestGroupCount; //�Ѿ���ɵĲ���������
	char errorInfo[128];
}TESTobject;	

typedef struct{
	ListType collectList;
	ListType itemList;
}TESTresource;

typedef enum{
	ENUM_TEST_TYPE_AUTO=0,
	ENUM_TEST_TYPE_MANUAL,
	ENUM_TEST_TYPE_NONE
}TESTtype;

typedef struct
{
	int panelHandle;
    ListType collectList;
	ListType itemList;
	int currentCollect;
	int totalTestObject;
	TESTstate testState;   
	TESTobject objectArray[8];
	//TESTtype type;
	int maxObjectPanelCountPerRow;
	void *onTestBeginListener;// ���Կ�ʼǰ�ص�
	void *onTestFinishListener;// ������ɻص�
	int totalTestGroupCount;//���������� ������ʾ������� 
	int reTestCnt;//�ز����
}TESTengine;

typedef void (*ON_OBJECT_PANEL_CREATE_LISTENER)(int *); 
typedef void (*ON_OBJECT_PANEL_INIT_LISTENER)(TESTobject *); 
typedef BOOL (*ON_RESULT_SHOW_LISTENER)(TESTobject *object,TestGroup group,int groupIndex,int*itemIndex); 
typedef void (*ON_OBJECT_TEST_ERROR_LISTENER)(TESTobject *); 
typedef void (*ON_OBJECT_TEST_FINISH_LISTENER)(TESTobject *);  
typedef ENUMTestResult (*ON_OBJECT_GROUP_TEST_LISTENER)(TestGroup,TESTobject *,TESTType);
typedef ENUMTestResult (*ON_OBJECT_COLLECT_TEST_LISTENER)(Collect,TESTobject *);

typedef BOOL (*ON_TEST_BEGIN_LISTENER)(TESTengine *);
typedef void (*ON_TEST_FINISH_LISTENER)(TESTengine *); 

void reSizeTestPanel(TESTengine *t);     





TESTengine *createTestEngine(int panelHandle,ListType itemList,ListType collectList); 

TESTobject *createAndaddTestObject(TESTengine *t,EUT eut) ;

void reSetEngine(TESTengine *t);

void disPlayTestPanel(TESTengine *t);

void runTestEngine(TESTengine *t);

void releaseTestEngine(TESTengine *t);

TESTobject *getTestObjectByPanelHandle(int objPanelHandle,TESTengine *engine);

BOOL JudgeTestProcess(TESTengine *t);
#ifdef __cplusplus
    }
#endif

#endif
