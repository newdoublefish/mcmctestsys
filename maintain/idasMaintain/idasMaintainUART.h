/******************************************************************************

                  版权所有 (C), 2014-2020, 广州航新第一设计所                  

 ******************************************************************************/
/** @file    idasMaintainUART.h
 *  @version 初稿
 *  @author  tang
 *  @date    2014年11月26日 14时37分
 *  @brief   维护串口协议处理CBB内部函数接口头文件
 *  @note    修订记录:
 *  @note    1.作    者: tang 2014年11月26日
 *  @note      修订内容: 创建初稿
 *  @note    2.作    者: -------- ----年--月--日
 *  @note      修订内容: -------------------
 *  @note              : -------------------

 ******************************************************************************/

#ifndef IDAS_COMRXTX_H
#define IDAS_COMRXTX_H
#include <cvirte.h>		
#include <userint.h>


#include "idasMaintainUARTInterface.h"







/* 数据准备好后通知处理函数句柄类型定义 */
typedef void (*IDAS_COM_DATA_RDY_NOTIFICATION)(IDAS_COM_RX_TX_BUF_STRUCT *);

/* 各个状态下，字节接收处理函数句柄定义 */
typedef BOOL (*IDAS_COM_RX_STATE_EVENT_HANDLER)(IDAS_COM_RX_TX_BUF_STRUCT *);




static BOOL Idas_ComRx_Wait0xAA_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);


static BOOL Idas_ComRx_Wait0x55_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);



static BOOL Idas_ComRx_WaitSrcAddr_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);


static BOOL Idas_ComRx_WaitDstAddr_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);


static BOOL Idas_ComRx_WaitDataType_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);


static BOOL Idas_ComRx_WaitCmd_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);


static BOOL Idas_ComRx_WaitDataLen_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);


static BOOL Idas_ComRx_WaitData_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);


static BOOL Idas_ComRx_WaitCheckSum_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);


static BOOL Idas_ComRx_Wait0XA5_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);




static void Idas_ComRx_Timeout(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);


static BOOL Idas_ComRx_Is_State_Validate(IDAS_COM_RECV_STAT state_);

static BOOL IsValidDataType(IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD dataType_);

static unsigned short CalcCRC16 ( unsigned char *ptr_, unsigned short len_ );
#endif
