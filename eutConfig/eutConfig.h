/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：eutConfig.h
 // 文件功能描述: eut设备配置
 //
 // 
 // 创建标识：曾衍仁 20150721
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
#ifndef __EUTCOMMON_H__
#define __EUTCOMMON_H__
#include <stdlib.h> 
#include "toolbox.h"
#include "eutEntity.h"
#ifdef __cplusplus
   extern "C" {
#endif


ListType getEutList(void);

void DisplayEutPanelAsTabPanel(int panel);

#ifdef __cplusplus
   }
#endif

#endif
