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

#ifndef __UART_UTIL_H__
#define __UART_UTIL_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "eutEntity.h"   
		
typedef struct
{
    RSCONFIG rsconfig;
	unsigned char *rxBuffer;
	int rxLen;
	int maxRxLen;
	int timeOut;
}tUartRx;

tUartRx *createUartRx(RSCONFIG config,int maxRxLen,int timeOut);
BOOL releaseUartRx(tUartRx *tur);
BOOL doUartRxProcess(tUartRx *ptr);
		
#ifdef __cplusplus
    }
#endif

#endif  /* ndef __log_H__ */
