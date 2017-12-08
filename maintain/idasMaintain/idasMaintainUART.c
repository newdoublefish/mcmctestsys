/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ㶫������綯����Ӫ�ɷ����޹�˾��Ȩ���У�δ����Ȩ��
 // ��ֹ���������п�����ʹ�á�
 //
 // �ļ�����
 // �ļ���������: 
 //
 // 
 // ������ʶ�������� 
 //
 // �޸ı�ʶ��
 // �޸�������
 //-------------------------------------------------------------------------*/

/*-------------------------------- ����ͷ�ļ� --------------------------------*/
#include <ansi_c.h>
#include "idasMaintainUART.h"
//#include "idasMaintMsg.h"
#include <cvirte.h>		
#include <userint.h>

/*---------------------------------- �궨�� ----------------------------------*/
#define MAINTAIN_SYNC_HEAD_WORD	0xAA55
#define MAINTAIN_SYNC_TAIL_WORD 0x55AA
/**/
#define MSG_DATA_TYPE_CMD_LEN_BITMASK 0x10 
/**/
#define MSG_DATA_TYPE_DATA_LEN_BITMASK 0x4
/**/
#define TWO_STEPS 2

/*ά���������ֽڳ�ʱʱ��*/
#define     RECV_BYTE_TIMEOUT_IN_SEC 2   
#define     MAX_HEAD_LENGTH       7
#define     MIN_HEADER_SIZE       5
/*------------------------------- �ṹ���Ͷ���--------------------------------*/

typedef struct 
{
	 unsigned char                curByteValue;       /* ��ǰ���յ��������ֽ� */
	unsigned char                rxSrcAddr;
    unsigned char                rxDstAddr;
	 unsigned char				 rxHeadSize;
	unsigned char                rxDataType;//
	unsigned char                rxStep;//
	   IDAS_COM_RECV_STAT    rxDataBufStat;     /* ���յ�״̬ */
    unsigned short               rxCmd;//
    unsigned short				 MaxCmd;//
   	//WDOG_ID              wdRecvByteTimeoutId;/* �����ֽڳ�ʱ��ʱ�� */
    
    unsigned short               rxCheckSum;//
    unsigned short                 rxDataPackLen; //
    unsigned short                 rxDataBufLen;     /* ���ݽ��ջ��峤�� */
    unsigned char   *rxDataBuf;     /* ���ݽ��ջ��壬��Դ��ַ��ʼ��¼*/
	void *              pDataRdyNotify;/* ����׼���ú�֪ͨ������ */   
}Idas_COM_Handle;





/*------------------------------- �ڲ��������� -------------------------------*/

/*------------------------------- ȫ�ֱ������� -------------------------------*/



/*------------------------------- ��̬�������� -------------------------------*/


/* ״̬ת�ַ������� */
static const char* IDAS_COMRX_STATE_TO_STRING_MAP[] = 
{
     "WAIT_0XAA",
     "WAIT_0X55",
     "WAIT_SRC_ADDR",
     "WAIT_DST_ADDR",
     "WAIT_DATA_TYPE",
     "WAIT_CMD",
     "WAIT_DATA_LEN",
     "WAIT_DATA",
     "IDAS_COM_WAIT_CHECKSUM",
     "IDAS_COM_WAIT_0XA5",
     "DATA_DATA_RDY"
}; 

/* ״̬������������ */
static IDAS_COM_RX_STATE_EVENT_HANDLER comRxStateHdr[] =
{
    Idas_ComRx_Wait0xAA_Hdr,
    Idas_ComRx_Wait0x55_Hdr,
    Idas_ComRx_WaitSrcAddr_Hdr,
    Idas_ComRx_WaitDstAddr_Hdr,
    Idas_ComRx_WaitDataType_Hdr,
    Idas_ComRx_WaitCmd_Hdr,
    Idas_ComRx_WaitDataLen_Hdr,
    Idas_ComRx_WaitData_Hdr,
    Idas_ComRx_WaitCheckSum_Hdr,
    Idas_ComRx_Wait0XA5_Hdr
};
/* ��������ת�ַ������� */
static const char* COM_TYPE_TO_STRING_MAP[] = 
{
        "RS422",
        "RS485",
        "NOVALID"
};
/* ���¾�����CRC����ʽΪ0x1021����g(x) = x16+x12+x5+x0; */
static unsigned short  crc16_ta[] = {
        0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50a5, 0x60c6, 0x70e7,
        0x8108, 0x9129, 0xa14a, 0xb16b, 0xc18c, 0xd1ad, 0xe1ce, 0xf1ef,
        0x1231, 0x0210, 0x3273, 0x2252, 0x52b5, 0x4294, 0x72f7, 0x62d6,
        0x9339, 0x8318, 0xb37b, 0xa35a, 0xd3bd, 0xc39c, 0xf3ff, 0xe3de,
        0x2462, 0x3443, 0x0420, 0x1401, 0x64e6, 0x74c7, 0x44a4, 0x5485,
        0xa56a, 0xb54b, 0x8528, 0x9509, 0xe5ee, 0xf5cf, 0xc5ac, 0xd58d,
        0x3653, 0x2672, 0x1611, 0x0630, 0x76d7, 0x66f6, 0x5695, 0x46b4,
        0xb75b, 0xa77a, 0x9719, 0x8738, 0xf7df, 0xe7fe, 0xd79d, 0xc7bc,
        0x48c4, 0x58e5, 0x6886, 0x78a7, 0x0840, 0x1861, 0x2802, 0x3823,
        0xc9cc, 0xd9ed, 0xe98e, 0xf9af, 0x8948, 0x9969, 0xa90a, 0xb92b,
        0x5af5, 0x4ad4, 0x7ab7, 0x6a96, 0x1a71, 0x0a50, 0x3a33, 0x2a12,
        0xdbfd, 0xcbdc, 0xfbbf, 0xeb9e, 0x9b79, 0x8b58, 0xbb3b, 0xab1a,
        0x6ca6, 0x7c87, 0x4ce4, 0x5cc5, 0x2c22, 0x3c03, 0x0c60, 0x1c41,
        0xedae, 0xfd8f, 0xcdec, 0xddcd, 0xad2a, 0xbd0b, 0x8d68, 0x9d49,
        0x7e97, 0x6eb6, 0x5ed5, 0x4ef4, 0x3e13, 0x2e32, 0x1e51, 0x0e70,
        0xff9f, 0xefbe, 0xdfdd, 0xcffc, 0xbf1b, 0xaf3a, 0x9f59, 0x8f78,
        0x9188, 0x81a9, 0xb1ca, 0xa1eb, 0xd10c, 0xc12d, 0xf14e, 0xe16f,
        0x1080, 0x00a1, 0x30c2, 0x20e3, 0x5004, 0x4025, 0x7046, 0x6067,
        0x83b9, 0x9398, 0xa3fb, 0xb3da, 0xc33d, 0xd31c, 0xe37f, 0xf35e,
        0x02b1, 0x1290, 0x22f3, 0x32d2, 0x4235, 0x5214, 0x6277, 0x7256,
        0xb5ea, 0xa5cb, 0x95a8, 0x8589, 0xf56e, 0xe54f, 0xd52c, 0xc50d,
        0x34e2, 0x24c3, 0x14a0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
        0xa7db, 0xb7fa, 0x8799, 0x97b8, 0xe75f, 0xf77e, 0xc71d, 0xd73c,
        0x26d3, 0x36f2, 0x0691, 0x16b0, 0x6657, 0x7676, 0x4615, 0x5634,
        0xd94c, 0xc96d, 0xf90e, 0xe92f, 0x99c8, 0x89e9, 0xb98a, 0xa9ab,
        0x5844, 0x4865, 0x7806, 0x6827, 0x18c0, 0x08e1, 0x3882, 0x28a3,
        0xcb7d, 0xdb5c, 0xeb3f, 0xfb1e, 0x8bf9, 0x9bd8, 0xabbb, 0xbb9a,
        0x4a75, 0x5a54, 0x6a37, 0x7a16, 0x0af1, 0x1ad0, 0x2ab3, 0x3a92,
        0xfd2e, 0xed0f, 0xdd6c, 0xcd4d, 0xbdaa, 0xad8b, 0x9de8, 0x8dc9,
        0x7c26, 0x6c07, 0x5c64, 0x4c45, 0x3ca2, 0x2c83, 0x1ce0, 0x0cc1,
        0xef1f, 0xff3e, 0xcf5d, 0xdf7c, 0xaf9b, 0xbfba, 0x8fd9, 0x9ff8,
        0x6e17, 0x7e36, 0x4e55, 0x5e74, 0x2e93, 0x3eb2, 0x0ed1, 0x1ef0
};

/*--------------------------------- �������� ---------------------------------*/









/**************************************************************************
������:   IdasComRxTxInit
��������: ���շ��Ͷ˿ڳ�ʼ������
�������: IDAS_COM_RX_TX_BUF_STRUCT������ָ��
����ֵ: 1 ���� -1
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

BOOL IdasComRxTxInit(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_,unsigned short maxCmd, void* pFunc)
{
	 Idas_COM_Handle *pHandle;;
	
	  /* �����ݻ������ṹ���г�ʼ�� */
	 if(NULL == pComRxTxDataBuf_)
	 {
	    return -1;
	 }
	 //memset(pComRxTxDataBuf_, 0, sizeof(IDAS_COM_RX_TX_BUF_STRUCT));
	 pComRxTxDataBuf_->handle = (unsigned int)malloc(sizeof(Idas_COM_Handle));
	 memset((unsigned char *)pComRxTxDataBuf_->handle, 0, sizeof(Idas_COM_Handle));
	
	 
	 if(NULL == pComRxTxDataBuf_->handle)
	 {
	 	//logMsg("%s:Alloc Memory for idasMaintainUART Failed!", (int)__FUNCTION__,0,0,0,0,0);
		return -1;
	 }
	 pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
  
 
   
    pHandle->rxDataBufStat = IDAS_COM_WAIT_0XAA;
    pHandle->rxHeadSize = MIN_HEADER_SIZE;/* ͷ����(5,6,7)Ϊ��С5 */

	
    //pHandle->wdRecvByteTimeoutId = (WDOG_ID)wdCreate();
    pComRxTxDataBuf_->txDataReadyFlag = -1;
    pHandle->MaxCmd = maxCmd;
	

	pComRxTxDataBuf_->txDataBuf = (unsigned char *)malloc(MAX_MAINT_MSG_LEN_PER_PACKET + MAX_HEAD_LENGTH +5 + MAX_HEAD_LENGTH +5);
	if(NULL == pComRxTxDataBuf_->txDataBuf)
	{
	 	//logMsg("%s:Alloc Memory for idasMaintainUART Tx Buf Failed!", (int)__FUNCTION__,0,0,0,0,0);
		return -1;		
	}
	pHandle->rxDataBuf = (unsigned char *)malloc(MAX_MAINT_MSG_LEN_PER_PACKET + MAX_HEAD_LENGTH);
	if(NULL == pHandle->rxDataBuf)
	{
		//logMsg("%s:Alloc Memory for idasMaintainUART Rx Buf Failed!", (int)__FUNCTION__,0,0,0,0,0);
		return -1;	
	}
	
   /* if( (WDOG_ID)NULL == pHandle->wdRecvByteTimeoutId )
    {
        //logMsg("%s::Create WatchDog Error\n",(int)__FUNCTION__,0,0,0,0,0);
        return -1;
    }*/
    pHandle->pDataRdyNotify = (void *)pFunc;
    
    return 1;
}

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
BOOL IdasComRxTxDeInit(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
	if(NULL == pComRxTxDataBuf_)
	{
		return -1;
	}
	
	if(pComRxTxDataBuf_->txDataBuf != NULL)
	{
		free(pComRxTxDataBuf_->txDataBuf);
		pComRxTxDataBuf_->txDataBuf = NULL;
	}

	if(pComRxTxDataBuf_->handle != 0)
	{
		free((unsigned char *)pComRxTxDataBuf_->handle);
		
		//pComRxTxDataBuf_->handle = 0;
	}
	return 1;
	
}

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
BOOL IdasComRxEventHandler(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_,
        unsigned char byteValue_)
{
    BOOL result;
    IDAS_COM_RECV_STAT    curState;
    Idas_COM_Handle *pHandle ;
    if(NULL == pComRxTxDataBuf_)
    {
        return -1;
    }
    pHandle= (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    if(NULL == pHandle)
     {
         return -1;
     }
   

    pHandle->curByteValue = byteValue_ & 0xff;
    curState = pHandle->rxDataBufStat;
    result = -1;
  
    if(Idas_ComRx_Is_State_Validate(curState))
    {
    	
        result = (*(IDAS_COM_RX_STATE_EVENT_HANDLER)(comRxStateHdr[curState]))
                        (pComRxTxDataBuf_);

        if(result)
        {
            /*if( pHandle->wdRecvByteTimeoutId != (WDOG_ID)NULL )
            {
                wdCancel(pHandle->wdRecvByteTimeoutId);
            } */

            /* ��������һ��ʱ�� */
            /*if( -1 == wdStart(pHandle->wdRecvByteTimeoutId,
                                  sysClkRateGet() * RECV_BYTE_TIMEOUT_IN_SEC,
                                  (FUNCPTR)Idas_ComRx_Timeout, 
                                  (int)pComRxTxDataBuf_) )
            {
                //logMsg("Start wdTimer Error\n",0,0,0,0,0,0);
                return -1;
            }*/
        }
        else
        {
			
            
            /* �������Э�����ݷ��ʹ�������״̬���ص���ʼ����״̬ */
            Idas_ComRx_Timeout(pComRxTxDataBuf_);
        }
    }
    
    return result;
}

/** @brief      ���������ֽڳ�ʱ������
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     void
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static void Idas_ComRx_Timeout(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
	Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
	pHandle->rxDataBufStat = IDAS_COM_WAIT_0XAA;
    pHandle->rxDataBufLen = 0;
    pHandle->rxDataPackLen = 0;
}





/** @brief      ״̬�Ϸ���麯��
 *  @param[in]  IDAS_COM_RECV_STAT state_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_Is_State_Validate(IDAS_COM_RECV_STAT state_)
{
    BOOL result;
    result = -1;
    
    if( (state_ >= IDAS_COM_WAIT_0XAA) && (state_ <= IDAS_COM_DATA_RDY) )
    {
        result = 1;
    }
    return result;
}


/** @brief      ������������Ƿ���Ч�ӿں�������Ҫ����Ƿ�Ϊ��Ӧ��Ϣ���Լ�Э��汾
 *  @param[in]  IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD dataType_  
 *  @param[out] ��
 *  @return     static BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��12��10��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL IsValidDataType(IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD dataType_)
{
    BOOL result =  1;
    //logMsg("======dataTypePtr_ :%d, %d=====\r\n", dataType_.reqRspFlag,dataType_.ver,0,0,0,0);
    if( (dataType_.reqRspFlag != 0) ||
        (dataType_.ver != CURRENT_MSG_PROTO_VER))
    {
        result = -1;
    }
    return result;
}



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
        unsigned char value_)
{
	//logMsg("======dataTypePtr_ :%d=====\r\n", value_,0,0,0,0,0);
   memcpy((unsigned char *)dataTypePtr_, &value_, 1);
}
/** @brief      ����ͬ���ֵĵ�һ���ֽ�0xAA
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_Wait0xAA_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    BOOL result = 1;
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
    

    if( 0xaa == recvByte )
    {
    	pHandle->rxDataBufStat = IDAS_COM_WAIT_0X55;      
    }
    else
    {
        result = -1;
    }
    return result;
}


/** @brief      ����ͬ���ֵĵڶ����ֽ�0x55
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_Wait0x55_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    BOOL result = 1;
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
    

    if( 0x55 == recvByte )
    {
    	pHandle->rxDataBufStat = IDAS_COM_WAIT_SRC_ADDR;
        pHandle->rxDataBufLen = 0;       
    }
    else
    {
		printf("result false\r\n");
        result = -1;
    }
    return result;
}



/** @brief      ����Դ��ַ�ֽں���
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_WaitSrcAddr_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
     pHandle->rxDataBufStat = IDAS_COM_WAIT_DST_ADDR;
     pHandle->rxSrcAddr = recvByte;
     pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                 = recvByte;    
     return 1;
}

/** @brief      ����Ŀ���ַ�ֽں���
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_WaitDstAddr_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
    pHandle->rxDataBufStat = IDAS_COM_WAIT_DATA_TYPE;
    pHandle->rxDstAddr = recvByte;
    pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                = recvByte;

    return 1;
}


/** @brief      �������������ֽں���
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_WaitDataType_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    
    recvByte = pHandle->curByteValue;
    //logMsg("======Idas_ComRx_WaitDataType_Hdr:0x%x=====\r\n", recvByte,0,0,0,0,0);
    pHandle->rxDataBufStat = IDAS_COM_WAIT_CMD;
    pHandle->rxDataType = recvByte;
    pHandle->rxStep = 0;
    pHandle->rxCmd = 0;
    pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                = recvByte;
    pHandle->rxHeadSize = MIN_HEADER_SIZE;
    if((pHandle->rxDataType & MSG_DATA_TYPE_CMD_LEN_BITMASK) != 0)
    {
    	pHandle->rxHeadSize++;
    }
    if((pHandle->rxDataType & MSG_DATA_TYPE_DATA_LEN_BITMASK) != 0)
    {
    	pHandle->rxHeadSize++;
    }    
    return 1;
}


/** @brief      �������ֽڴ�����
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_WaitCmd_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    BOOL result = 1;
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
    

    if( 0 == (pHandle->rxDataType & MSG_DATA_TYPE_CMD_LEN_BITMASK) )
    {
        /* ������Ϊһ�ֽڵ���� */
    	pHandle->rxDataBufStat = IDAS_COM_WAIT_DATA_LEN;
        pHandle->rxCmd = recvByte;
        pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
        pHandle->rxDataPackLen = 0;

    }
    else
    {
        /* ������Ϊ���ֽڵ����,�����ֽ����������� */
    	pHandle->rxCmd = 
            (pHandle->rxCmd << 8) | recvByte;
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
    	pHandle->rxStep++;
        if(TWO_STEPS == pHandle->rxStep )
        {   
            /* ������Ϊ����ֽڵ���� */
        	pHandle->rxStep = 0;
        	pHandle->rxDataBufStat = IDAS_COM_WAIT_DATA_LEN;
            pHandle->rxDataPackLen = 0;
			
        }
   
    } 
    /* ����������Ƿ���Ч */   
     if( pHandle->rxCmd >= pHandle->MaxCmd )
     {
         //logMsg("The received cmd is invalidate!",0,0,0,0,0,0);
         pHandle->rxStep = 0;
         pHandle->rxDataBufStat = IDAS_COM_WAIT_0XAA;
         result = -1;
     } 
    return result;
}


/** @brief      ���ݳ��ȴ�����
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_WaitDataLen_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
 
    if( 0 == (pHandle->rxDataType & MSG_DATA_TYPE_DATA_LEN_BITMASK) )
    {
        /* ����Ϊһ�ֽڵ���� */
    	pHandle->rxDataPackLen = recvByte;
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
        /* ������ݳ���Ϊ0�� ��ôֱ�ӽ������CheckSum��״̬�������������*/
        if(0 == pHandle->rxDataPackLen )
        {
        	pHandle->rxDataBufStat = IDAS_COM_WAIT_CHECKSUM;
        }
        else
        {
        	pHandle->rxDataBufStat = IDAS_COM_WAIT_DATA;
        }
    }
    else
    {
        /* ����Ϊ���ֽڵ����,�����ֽ����������� */
    	pHandle->rxDataPackLen = 
            (pHandle->rxDataPackLen << 8) | recvByte;
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
    	pHandle->rxStep++;
        if(TWO_STEPS == pHandle->rxStep)
        {   
            /* ������Ϊ����ֽڵ���� */   
        	pHandle->rxStep = 0;
            if(pHandle->rxDataPackLen > MAX_MAINT_MSG_LEN_PER_PACKET)
            {
                pHandle->rxDataBufStat = IDAS_COM_WAIT_0XAA;
                return -1;
            }
            /* ������ݳ���Ϊ0�� ��ôֱ�ӽ������CheckSum��״̬�������������*/
            if(0 == pHandle->rxDataPackLen)
            {
            	pHandle->rxDataBufStat = IDAS_COM_WAIT_CHECKSUM;
            }
            else
            {
            	pHandle->rxDataBufStat = IDAS_COM_WAIT_DATA;
            }
        }
    }
    
    return 1;
}


/** @brief      �����ֽڴ�����
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_WaitData_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{

    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
    if( (pHandle->rxDataBufLen) < 
            (pHandle->rxDataPackLen + 
            		pHandle->rxHeadSize - 1))
    {
        /* �������ݵ��������ݻ����� */
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
    }
    else
    {
        /* ���һ���ֽ����� */
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                            = recvByte;
    	pHandle->rxDataBufStat = IDAS_COM_WAIT_CHECKSUM;
        pHandle->rxDataBufLen %= (MAX_MAINT_MSG_LEN_PER_PACKET + MAX_HEAD_LENGTH);
        pHandle->rxCheckSum = 0;
    }
    return 1;
}

/** @brief      У����ֽڴ�����
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_WaitCheckSum_Hdr
(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
   
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
  



    /* ������Ϊ���ֽڵ����,�����ֽ����������� */
            pHandle->rxCheckSum = 
        (pHandle->rxCheckSum << 8) | recvByte;
    
    pHandle->rxStep++;
    if(TWO_STEPS == pHandle->rxStep )
    {   
        /* ������Ϊ����ֽڵ���� */   
    	pHandle->rxDataBufStat = IDAS_COM_WAIT_0XA5;
        pHandle->rxStep = 0;
    }

    return 1;
}


/** @brief      ͬ�����ֽڴ�����
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] ��
 *  @return     BOOL
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��11��26��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static BOOL Idas_ComRx_Wait0XA5_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    BOOL result = 1;
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;


    if( 0xa5 == recvByte )
    {
    	pHandle->rxDataBufStat = IDAS_COM_DATA_RDY;
        if(pHandle->pDataRdyNotify != NULL)
        {

            (*(IDAS_COM_DATA_RDY_NOTIFICATION)(pHandle->pDataRdyNotify))
                     (pComRxTxDataBuf_);
        }

        pHandle->rxDataBufStat = IDAS_COM_WAIT_0XAA;

        result = 1;
    }
    else
    {
        result = -1;
    }
    return result;
}



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
BOOL Idas_ComTx_Send(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
 

    /*������ر�־��ʹ����ѭ�����Է���Tx�е�����*/
   if(NULL == pComRxTxDataBuf_)
   {
	   return -1;
   }
    pComRxTxDataBuf_->txDataReadyFlag = 1;
    return 1;
}


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
BOOL ParseRXComPacket(IDAS_COM_RX_TX_BUF_STRUCT *pRxBuf_, 
                      IDAS_MAINT_MSG *pReqMsg_)
{   
	Idas_COM_Handle *pHandle ;
	if((NULL == pRxBuf_)||(NULL == pReqMsg_) )
	{
		return -1;
	}
	pHandle= (Idas_COM_Handle *)pRxBuf_->handle;
    /* Message Header Assignment */
    pReqMsg_->msgHead.syscWordH = MAINTAIN_SYNC_HEAD_WORD;
    pReqMsg_->msgHead.addrOfSender = pHandle->rxSrcAddr;
    pReqMsg_->msgHead.addrOfRecv = pHandle->rxDstAddr;    
    SetDateTypeWordValue(&(pReqMsg_->msgHead.dataType),pHandle->rxDataType);	

    /* ����������͵���Ч�� */
    if( !IsValidDataType(pReqMsg_->msgHead.dataType) )
    {
        //logMsg("Error Message DataType\n",0,0,0,0,0,0);
        return -1;
    }
    pReqMsg_->msgHead.cmd = pHandle->rxCmd; 
   
    pReqMsg_->msgHead.msgLen = pHandle->rxDataPackLen;
    /* Message Tail Assignemnt */
    pReqMsg_->msgTail.crcCode = pHandle->rxCheckSum;
    pReqMsg_->msgTail.syscWordT = 0x55AA;
    
    /* Message Body Pointer Assignment */
    pReqMsg_->msgBodyPtr = pHandle->rxDataBuf + pHandle->rxHeadSize;
    
    return 1;
}

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
BOOL BuildTxComPacket(IDAS_MAINT_MSG *pSendMsg_,
           IDAS_COM_RX_TX_BUF_STRUCT *pTxBuf_ )
{
    unsigned short crc16;
    unsigned short dataOffset = 5;
	if((NULL == pSendMsg_)||(NULL == pTxBuf_) )
	{
		return -1;
	}
    /* �������Ͱ�ͷ,����ͬ���� */
    pTxBuf_->txDataBufLen = 0;
    pTxBuf_->txDataBuf[pTxBuf_->txDataBufLen] = 0xaa;
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 0x55;
    
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
                                          pSendMsg_->msgHead.addrOfSender;
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
                                          pSendMsg_->msgHead.addrOfRecv;
    memcpy(&(pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)]),
           (unsigned char *)&(pSendMsg_->msgHead.dataType),1);
    
    if(1 == pSendMsg_->msgHead.dataType.cmdLenInd )
    {
        pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
            (unsigned char)((pSendMsg_->msgHead.cmd >> 8) & 0xff);
        dataOffset++;
    }
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
            (unsigned char)( pSendMsg_->msgHead.cmd & 0xff); 
    
    if(1 == pSendMsg_->msgHead.dataType.dataLenInd)
    {
    	pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
            (unsigned char)(( pSendMsg_->msgHead.msgLen >> 8 ) & 0xff );
        dataOffset++;
    }
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
            (unsigned char)( pSendMsg_->msgHead.msgLen & 0xff); 
    /* �������Ͱ��� */
    if( pSendMsg_->msgHead.msgLen > 0)
    {
        memcpy((unsigned char *)&(pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)]),
            pSendMsg_->msgBodyPtr, pSendMsg_->msgHead.msgLen);
        pTxBuf_->txDataBufLen += pSendMsg_->msgHead.msgLen-1;
    }

    /* ���㷢��checkSumУ���� */
    crc16 = CalcCRC16(&(pTxBuf_->txDataBuf[2]), pSendMsg_->msgHead.msgLen + dataOffset); 
    /* �������Ͱ�β */
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
            (unsigned char)( (crc16 >> 8) & 0xff); 
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
            (unsigned char)( crc16 & 0xff); 
    
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 0xa5;
    /* pTxBuf_->txDataBufLenָ�����һ����Ч�ֽڣ����Գ�����Ҫ��1 */
    ++pTxBuf_->txDataBufLen;

    return 1;
}


/** @brief      ά������CRCУ������㺯��
 *  @param[in]  unsigned char *ptr_  
 *  @param[in]  unsigned short len_  
 *  @param[out] ��
 *  @return     static unsigned short
 *  @note       �޶���¼  :
 *  @note       1.��    ��: tang 2014��12��2��
 *  @note         �޶�����: ��������
 *  @note       2.��    ��: --------- ----��--��--��
 *  @note         �޶�����: ------------------------

*/
static unsigned short CalcCRC16(unsigned char *ptr_ ,unsigned short len_)
{
    unsigned short crc;
    unsigned char da;
    crc = 0;
    while( len_-- != 0 )
    {
        da =(unsigned char)(crc >> 8);    /* ��8λ������������ʽ�ݴ�CRC�ĸ�8λ */
        crc <<= 8;              /* ����8λ */
        crc ^= crc16_ta[da^(*ptr_)];  /* ��8λ�͵�ǰ�ֽ���Ӻ��ٲ����CRC ���ټ�����ǰ��CRC */
        ptr_++;
    }
    return crc ;
}




