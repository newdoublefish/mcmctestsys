/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：hardwareEntity.h
 // 文件功能描述: 硬件资源数据存储操作
 //
 // 
 // 创建标识：曾衍仁 20150721
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
#ifndef __HARDWAREENTITY_H__
#define __HARDWAREENTITY_H__

#include <stdlib.h>
#include "toolbox.h"
#include <utility.h>

#ifdef __cplusplus
   extern "C" {
#endif


typedef struct
{
    int arinc429card;
}HARDWARE;

HRESULT saveHardware(HARDWARE hardware);

HARDWARE getHardware(void);

#ifdef __cplusplus
   }
#endif

#endif
