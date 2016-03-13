/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：tpsHelper.h
 // 文件功能描述: TPS框架 
 //
 // 
 // 创建标识：曾衍仁 20160111
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
#include "relayHelper.h"
#include "convertTool.h"
#include "resultUtil.h"

#ifdef __cplusplus
   extern "C" {
#endif
	   
#define MAX_TPS_NAME_LEN 30
	   
typedef enum
{
    TEST_RESULT_ALLPASS=0,//测试全部通过
	TEST_RESULT_SOMEPASS=1,//测试部分通过，部分没有通过
	TEST_RESULT_ERROR=2 //测试出现故障
}METHODRET;  //测试返回类型

typedef METHODRET (*TEST_METHOD)(TestGroup,EUT,HashTableType);  
typedef BOOL (*PROTOCOL_INIT)(char *); 
typedef void (*TEST_PREPARE)(void);
	   
typedef struct{
    char tpsName[MAX_TPS_NAME_LEN];
	void *protocolInit;//协议初始化函数
	void *testPrepare;//测试准备
	void *testFunction;//测试函数 
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

