/*---------------------------------------------------------------------------
 // 版权声明：本文件由广东万城万充电动车运营股份有限公司版权所有，未经授权，
 // 禁止第三方进行拷贝和使用。
 //
 // 文件名：
 // 文件功能描述: 
 //
 // 
 // 创建标识：曾衍仁 
 //
 // 修改标识：
 // 修改描述：
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
     TEST_IDLE=0x01,//空闲
     TEST_RUN=0x02,//测试中
     TEST_STOP=0x03,//暂停
     TEST_FINISH=0x04,//测试完成
	 TEST_NON_DEF
}TESTstate;	

typedef struct{
	int beginCollect;
	int beginGroup;
}TESTObjectSeq;


typedef struct
{
	int objectIndex;
	int panelHandle;//显示面板
    EUT device; //测试设备信息
	TESTObjectSeq seq;
	HashTableType resultHashTable;//测试结果
	void *enginePtr;
	void *onObjectPanelCreateListener;   //对象面板创建
	void *onObjectPanelInitListener;   //对象面板初始化
	void *onObjectGroupTestListener;     //对象测试
	void *onObjectCollectTestListener;     //对象测试 
	void *onResultShowListener;	   //对象结果显示
	void *onObjectTestErrorListener;       //对象测试出错
	void *onObjectTestFinish;//该对象测试完成
	BOOL errorFlag;			              //如果串口或者网络错误标志，如果错误就不会继续往下测试了
	int totalFinishedTestGroupCount; //已经完成的测试组数量
	char errorInfo[128];
	int completeRate;//完成率
	int passRate;//合格率
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
	void *onTestBeginListener;// 测试开始前回调
	void *onTestFinishListener;// 测试完成回调
	int totalTestGroupCount;//测试组数量 用来显示完成数量 
	int reTestCnt;//重测次数
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

void runSingleObject(TESTengine *t,TESTobject *testObject);

void releaseTestEngine(TESTengine *t);

TESTobject *getTestObjectByPanelHandle(int objPanelHandle,TESTengine *engine);

BOOL JudgeTestProcess(TESTengine *t);
#ifdef __cplusplus
    }
#endif

#endif
