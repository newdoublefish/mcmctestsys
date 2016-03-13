/******************************************************************************

                  ��Ȩ���� (C), 2014-2020, ���ݺ��µ�һ�����                  

 ******************************************************************************/
/** @file    idasMaintainUART.h
 *  @version ����
 *  @author  tang
 *  @date    2014��11��26�� 14ʱ37��
 *  @brief   ά������Э�鴦��CBB�ڲ������ӿ�ͷ�ļ�
 *  @note    �޶���¼:
 *  @note    1.��    ��: tang 2014��11��26��
 *  @note      �޶�����: ��������
 *  @note    2.��    ��: -------- ----��--��--��
 *  @note      �޶�����: -------------------
 *  @note              : -------------------

 ******************************************************************************/

#ifndef IDAS_COMRXTX_H
#define IDAS_COMRXTX_H
#include <cvirte.h>		
#include <userint.h>


#include "idasMaintainUARTInterface.h"







/* ����׼���ú�֪ͨ������������Ͷ��� */
typedef void (*IDAS_COM_DATA_RDY_NOTIFICATION)(IDAS_COM_RX_TX_BUF_STRUCT *);

/* ����״̬�£��ֽڽ��մ������������ */
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
