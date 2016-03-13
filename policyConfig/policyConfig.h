/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：policyConfig.h
 // 文件功能描述:策略增删
 //
 // 
 // 创建标识：曾衍仁 20141114
 //
 // 修改标识：曾衍仁  20150814
 // 修改描述：
 //-------------------------------------------------------------------------*/
#ifndef __POLICYCOMMON_H__
#define __POLICYCOMMON_H__

#include <stdlib.h> 
#include "toolbox.h"
#include "policyEntity.h"

#ifdef __cplusplus
    extern "C" {
#endif

ListType GetCollectList(void);

void DisplayStrategyPanelAsTabPanel(int panelHandle);

int isStrategyNeedToSave(void);

#ifdef __cplusplus
    }
#endif


#endif

