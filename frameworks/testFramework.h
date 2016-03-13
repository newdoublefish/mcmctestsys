/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：testFramework.h
 // 文件功能描述: 测试engine
 //
 // 
 // 创建标识：曾衍仁 20150721
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
    TEST_ALL_PASS=0x01,
	TEST_SOME_PASS=0x02,
	TEST_ERROR=0x03
}TESTresult;	
		

typedef enum
{
     TEST_IDLE=0x01,//空闲
     TEST_RUN=0x02,//测试中
     TEST_STOP=0x03,//暂停
     TEST_FINISH=0x04,//测试完成
	 TEST_NON_DEF
}TESTstate;	


typedef struct
{
	int objectIndex;
	int panelHandle;//显示面板
    EUT device; //测试设备信息
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
	int currentSubItemIndex;		   //当前显示的条例序号
	int totalFinishedTestGroupCount; //已经完成的测试组数量
}TESTobject;	

typedef struct
{
	int panelHandle;
    ListType collectList;
	ListType itemList;
	int currentCollect;
	int totalTestObject;
	TESTstate testState;   
	TESTobject objectArray[8];
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
typedef TESTresult (*ON_OBJECT_GROUP_TEST_LISTENER)(TestGroup,TESTobject *);
typedef TESTresult (*ON_OBJECT_COLLECT_TEST_LISTENER)(Collect,TESTobject *);

typedef BOOL (*ON_TEST_BEGIN_LISTENER)(TESTengine *);
typedef void (*ON_TEST_FINISH_LISTENER)(TESTengine *); 

void reSizeTestPanel(TESTengine *t);     





TESTengine *createTestEngine(int panelHandle,ListType itemList,ListType collectList); 

TESTobject *createAndaddTestObject(TESTengine *t,EUT eut) ;

void reSetEngine(TESTengine *t);

void disPlayTestPanel(TESTengine *t);

void runTestEngine(TESTengine *t);

void releaseTestEngine(TESTengine *t);

BOOL JudgeTestProcess(TESTengine *t);
#ifdef __cplusplus
    }
#endif

#endif
