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
#ifndef __FILEDOWNLOADHELPER_H__
#define __FILEDOWNLOADHELPER_H__
#include "userint.h"
#include "eutConfig.h"
#ifdef __cplusplus
   extern "C" {
#endif
typedef enum
{
    FD_COMMON_CANNOT_OPEN=0,//不能打开串口
	FD_SUCCESS=1,//下载文件成功
	FD_ERROR=2 //下载文件失败
}FD_RET;  //下载文件返回类型

FD_RET downloadNewestFile(char *srcDir,EUT eut,char *localFile);

#ifdef __cplusplus
   }
#endif

#endif
