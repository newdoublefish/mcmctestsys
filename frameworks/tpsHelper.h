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
    TEST_RESULT_ALLPASS=0,//测试全部通过
	TEST_RESULT_SOMEPASS=1,//测试部分通过，部分没有通过
	TEST_RESULT_ERROR=2 //测试出现故障
}METHODRET;  //测试返回类型

typedef METHODRET (*TEST_METHOD)(TestGroup,EUT,HashTableType); 
typedef METHODRET (*TEST_FUNCTION)(TestGroup,EUT,HashTableType,int); 
typedef BOOL (*PROTOCOL_INIT)(char *); 
typedef void (*TEST_PREPARE)(void);
typedef int (*ON_CREATE_TPS_PANEL)(char *name); 
	   
typedef struct{
    char tpsName[MAX_TPS_NAME_LEN];
	void *protocolInit;//协议初始化函数,系统起来的时候执行一次
	void *testPrepare;//测试准备
	void *onTestFinish;//测试准备 
	int tpsPanelHandle;
	ON_CREATE_TPS_PANEL createTpsPanel;
	TEST_METHOD autoTestFunction;//自动测试函数 
	TEST_FUNCTION testFunction;//自动测试函数 
//	TEST_METHOD manualTestFunction;//手动测试函数 	
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

