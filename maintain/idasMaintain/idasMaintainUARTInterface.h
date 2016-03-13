/******************************************************************************

                  ��Ȩ���� (C), 2014-2020, ���ݺ��µ�һ�����                  

 ******************************************************************************/
/** @file    idasMaintainUARTInterface.h
 *  @version ����
 *  @author  tang
 *  @date    2014��11��26�� 14ʱ55��
 *  @brief   ά������Э�鴦��CBB�ⲿ�ӿ�����ͷ�ļ�
 *  @note    �޶���¼:
 *  @note    1.��    ��: tang 2014��11��26��
 *  @note      �޶�����: ��������
 *  @note    2.��    ��: -------- ----��--��--��
 *  @note      �޶�����: -------------------
 *  @note              : -------------------

 ******************************************************************************/


#ifndef IDAS_COMRXTX_INTERFACE_H
#define IDAS_COMRXTX_INTERFACE_H

#define CURRENT_MSG_PROTO_VER 0
#define MAX_MAINT_MSG_LEN_PER_PACKET 4096 /* ÿһ��ά����Ϣ������󳤶� */
#define MSG_REQ_DATA_TYPE_BASIC 0x00
#define MSG_RSP_SUCC_DATA_TYPE_BASIC 0x01
#define MSG_RSP_FAIL_DATA_TYPE_BASIC 0x11


#if 0
typedef struct
{
    unsigned char ver:3;
    unsigned char reslutInd:1;   
	unsigned char cmdLenInd:1; 
	unsigned char dataLenInd:1; 
	unsigned char zipFlag:1;
	unsigned char reqRspFlag:1;
}IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD;
#else
typedef struct
{
    unsigned char reqRspFlag:1;
	unsigned char zipFlag:1; 
	unsigned char dataLenInd:1;
	unsigned char cmdLenInd:1;
	unsigned char reslutInd:1;
	unsigned char ver:3;
}IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD;
#endif



/********************ά����Ϣ���ݵ�ͷ����****************************************
 * ����            ����     ע��
 * ͬ��ͷ           2    0xAA55
 * �����豸��ַ     1
 * �����豸��ַ     1
 * ��������         1    
 * ������          1/2  ��������������ȷ��
 * ���ݳ���        1/2  ��������������ȷ�����ݳ���Ϊ1�ֽ� 
 *                    ����2�ֽڵ�λ���ֽ�,
 *                    �����Ϊ4096�ֽڳ���
****************************************************************************/
typedef struct
{
    unsigned int syscWordH;
    unsigned short addrOfSender;
    unsigned short addrOfRecv;
    IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD dataType;
    unsigned short cmd; 
    unsigned int msgLen; /* ��Ϣ�������ݵĳ��� */
}IDAS_MAINT_MSG_HEADER;

/************������Ϣ��β******************************************************
 * ����            ����     ע��
 * У����           2
 * ͬ��β           1      0xa5
 ***************************************************************************/
typedef struct
{
    unsigned int crcCode;
    unsigned short syscWordT;
}IDAS_MAINT_MSG_TAIL;



/************������Ϣ��ʽ******************************************************
 * ��Ϣ�����������֣�
 *    - ��Ϣ���ݰ�ͷ
 *    - ��Ϣ���ݰ�β
 *    - ��Ϣ������
 ***************************************************************************/
typedef struct
{
    IDAS_MAINT_MSG_HEADER msgHead;
    IDAS_MAINT_MSG_TAIL msgTail;
    void * msgBodyPtr;
}IDAS_MAINT_MSG;

/* ���ջ������Ļ���״̬���� */
typedef enum 
{   
    IDAS_COM_WAIT_0XAA,                     /* �ȴ�ͬ���ֵ�һ�ֽ�0xaa */
    IDAS_COM_WAIT_0X55,                     /* �ȴ�ͬ���ֵڶ��ֽ�0x55 */
    IDAS_COM_WAIT_SRC_ADDR,                 /* �ȴ�Դ��ַ */
    IDAS_COM_WAIT_DST_ADDR,                 /* �ȴ�Ŀ���ַ */
    IDAS_COM_WAIT_DATA_TYPE,                /* �ȴ��������� */
    IDAS_COM_WAIT_CMD,                      /* �ȴ������� */
    IDAS_COM_WAIT_DATA_LEN,                 /* �ȴ����ݳ��� */
    IDAS_COM_WAIT_DATA,                     /* �ȴ��������� */
    IDAS_COM_WAIT_CHECKSUM,                 /* �ȴ���������У���� */
    IDAS_COM_WAIT_0XA5,                     /* �ȴ���������У���� */
    IDAS_COM_DATA_RDY                       /* ���ݿ��Ա����� */
}IDAS_COM_RECV_STAT;



/* ���ڽӿ����Ͷ��� */
typedef enum 
{
    RS422,
    RS485,
    NOVALID
}COM_TYPE;
/* ���ջ������Ľṹ���� */

typedef struct
{  
	unsigned int		 handle;  
    unsigned char                txDataReadyFlag; /*��������׼����*/
    COM_TYPE             comType;            /* �����ݻ������������ӿ����� */ 
    
     
    unsigned short       txDataBufLen;                 /* ���ݷ��ͻ��峤�� */
    unsigned char        *txDataBuf;  /* ���ݷ��ͻ��� */ 
	void *               fatherPtr;
}IDAS_COM_RX_TX_BUF_STRUCT;



/**************************************************************************
������:   IdasComRxTxInit
��������: ���շ��Ͷ˿ڳ�ʼ������
�������: IDAS_COM_RX_TX_BUF_STRUCT������ָ��
����ֵ: OK ���� ERROR
***************************************************************************/
/** @brief      ά������CBB��ʼ������
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT pComRxTxDataBuf_
 *  @param[in]  unsigned short maxCmd
 *  @param[in]  Idas_COM_Handle *pHandle
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��25��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
BOOL IdasComRxTxInit(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_, unsigned short maxCmd, void *pFunc);

/** @brief      ά��������Դ�ͷź����ӿ�
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     void
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��12��2��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
BOOL IdasComRxTxDeInit(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);

/**************************************************************************
������:   IdasComRxEventHandler
��������: ����������Ϣ��Ӧ�������ú��������ֽڵķ���������״̬�����ĵ��ô���
          ����״̬��Ǩ�Ƶȶ���
�������: IDAS_COM_RX_TX_BUF_STRUCT������ָ��
����ֵ: OK ���� ERROR
***************************************************************************/
/** @brief      ����ά����Ϣ���������������ά����Ϣ�ֽڣ�����ά��Э�����
                ����״̬�������������ݴ���
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[in]  unsigned char byteValue_                             
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��25��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
BOOL IdasComRxEventHandler(
        IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_,
        unsigned char byteValue_);

/** @brief      �������������ӿ�
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
BOOL Idas_ComTx_Send(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);

/** @brief      ���յ���ά�����ݽ�����ά����Ϣ�ṹ�崦����
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pRxBuf_  
 *  @param[in]  IDAS_MAINT_MSG *pReqMsg_            
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
BOOL ParseRXComPacket(IDAS_COM_RX_TX_BUF_STRUCT *pRxBuf_, IDAS_MAINT_MSG *pReqMsg_);


/** @brief      ������Ϣ��ά��Э����������
 *  @param[in]  IDAS_MAINT_MSG *pSendMsg_           
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pTxBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
 BOOL BuildTxComPacket(IDAS_MAINT_MSG *pSendMsg_, IDAS_COM_RX_TX_BUF_STRUCT *pTxBuf_ );

/** @brief      ����ά����Ϣ��������ֵ�����ӿ�
 *  @param[in]  IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD *dataTypePtr_  
 *  @param[in]  unsigned char value_                                        
 *  @param[out] ��
 *  @return     void
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��12��2��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
 void SetDateTypeWordValue(
        IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD *dataTypePtr_, 
        unsigned char value_);


#endif
