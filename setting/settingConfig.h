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
#ifndef __SETTINGCONFIG_H__
#define __SETTINGCONFIG_H__
#include "settingEntity.h"
#ifdef __cplusplus
   extern "C" {
#endif

HRESULT DisplaySettingPanelAsTabPanel(int panelHandle);     

SETTING GetSetting(void);

#ifdef __cplusplus
   }
#endif

#endif
