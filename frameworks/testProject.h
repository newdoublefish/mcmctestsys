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

BOOL newTestProject(tTestProject *project);
BOOL loadTestProject(tTestProject *project);
BOOL saveTestProjectInfo(tTestProject info);
#ifdef __cplusplus
    }
#endif


#endif
