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
#ifndef __FTPCONFIG_VIEW_H__
#define __FTPCONFIG_VIEW_H__
#include "userint.h"
#include "ftpConfigBiz.h"
#ifdef __cplusplus
   extern "C" {
#endif
	   
int DisplayFtpConfigView (int panel);
tFtpConfig GetFtpConfig();
#ifdef __cplusplus
   }
#endif

#endif
