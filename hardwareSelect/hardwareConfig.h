/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：hardwareConfig.h
 // 文件功能描述: 硬件资源选择
 //
 // 
 // 创建标识：曾衍仁 20150721
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
#ifndef __HARDWARECONFIG_H__
#define __HARDWARECONFIG_H__

#include <stdlib.h> 
#include "toolbox.h"
#include "hardwareEntity.h"
#ifdef __cplusplus
   extern "C" {
#endif
void DisplayHardwarePanelAsTabPanel(int panel);

HARDWARE GetHardware(void);

#ifdef __cplusplus
   }
#endif

//void SetUUTList(ListType uutList);
#endif
