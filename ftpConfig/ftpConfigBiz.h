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
#ifndef __FTPCONFIGBIZ_H__
#define __FTPCONFIGBIZ_H__
#include "userint.h"
#ifdef __cplusplus
   extern "C" {
#endif
	   
typedef struct
{
	char server[50];
	char userName[50];
	char password[50];
	char remoteDir[250];
	int mode;
}tFtpConfig;
 

tFtpConfig getFtpConfig(void);
HRESULT saveFtpConfig(tFtpConfig s);

#ifdef __cplusplus
   }
#endif

#endif

