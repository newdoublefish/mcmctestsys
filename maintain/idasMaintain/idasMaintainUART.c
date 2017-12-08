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

/*-------------------------------- 包含头文件 --------------------------------*/
#include <ansi_c.h>
#include "idasMaintainUART.h"
//#include "idasMaintMsg.h"
#include <cvirte.h>		
#include <userint.h>

/*---------------------------------- 宏定义 ----------------------------------*/
#define MAINTAIN_SYNC_HEAD_WORD	0xAA55
#define MAINTAIN_SYNC_TAIL_WORD 0x55AA
/**/
#define MSG_DATA_TYPE_CMD_LEN_BITMASK 0x10 
/**/
#define MSG_DATA_TYPE_DATA_LEN_BITMASK 0x4
/**/
#define TWO_STEPS 2

/*维护包接收字节超时时间*/
#define     RECV_BYTE_TIMEOUT_IN_SEC 2   
#define     MAX_HEAD_LENGTH       7
#define     MIN_HEADER_SIZE       5
/*------------------------------- 结构类型定义--------------------------------*/

typedef struct 
{
	 unsigned char                curByteValue;       /* 当前接收到的数据字节 */
	unsigned char                rxSrcAddr;
    unsigned char                rxDstAddr;
	 unsigned char				 rxHeadSize;
	unsigned char                rxDataType;//
	unsigned char                rxStep;//
	   IDAS_COM_RECV_STAT    rxDataBufStat;     /* 接收的状态 */
    unsigned short               rxCmd;//
    unsigned short				 MaxCmd;//
   	//WDOG_ID              wdRecvByteTimeoutId;/* 接收字节超时计时器 */
    
    unsigned short               rxCheckSum;//
    unsigned short                 rxDataPackLen; //
    unsigned short                 rxDataBufLen;     /* 数据接收缓冲长度 */
    unsigned char   *rxDataBuf;     /* 数据接收缓冲，从源地址开始记录*/
	void *              pDataRdyNotify;/* 数据准备好后通知处理函数 */   
}Idas_COM_Handle;





/*------------------------------- 内部函数声明 -------------------------------*/

/*------------------------------- 全局变量定义 -------------------------------*/



/*------------------------------- 静态变量定义 -------------------------------*/


/* 状态转字符串数组 */
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

/* 状态函数处理数组 */
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
/* 串口类型转字符串数组 */
static const char* COM_TYPE_TO_STRING_MAP[] = 
{
        "RS422",
        "RS485",
        "NOVALID"
};
/* 以下均采用CRC多项式为0x1021即：g(x) = x16+x12+x5+x0; */
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

/*--------------------------------- 常量定义 ---------------------------------*/









/**************************************************************************
函数名:   IdasComRxTxInit
功能描述: 接收发送端口初始化函数
输入参数: IDAS_COM_RX_TX_BUF_STRUCT缓冲区指针
返回值: 1 或者 -1
***************************************************************************/
/** @brief      维护串口CBB初始化函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT pComRxTxDataBuf_
 *  @param[in]  unsigned short maxCmd
 *  @param[in]  Idas_COM_Handle *pHandle
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月25日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

*/

BOOL IdasComRxTxInit(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_,unsigned short maxCmd, void* pFunc)
{
	 Idas_COM_Handle *pHandle;;
	
	  /* 对数据缓存区结构进行初始化 */
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
    pHandle->rxHeadSize = MIN_HEADER_SIZE;/* 头长度(5,6,7)为最小5 */

	
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

/** @brief      维护串口资源释放函数接口
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     void
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年12月2日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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

/** @brief      接收维护消息处理函数，负责分析维护消息字节，根据维护协议调用
                接收状态处理函数进行数据处理
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[in]  unsigned char byteValue_                             
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月25日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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

            /* 重新启动一个时钟 */
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
			
            
            /* 如果发送协议数据发送错误，重置状态，回到初始接收状态 */
            Idas_ComRx_Timeout(pComRxTxDataBuf_);
        }
    }
    
    return result;
}

/** @brief      接收数据字节超时处理函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     void
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

*/
static void Idas_ComRx_Timeout(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
	Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
	pHandle->rxDataBufStat = IDAS_COM_WAIT_0XAA;
    pHandle->rxDataBufLen = 0;
    pHandle->rxDataPackLen = 0;
}





/** @brief      状态合法检查函数
 *  @param[in]  IDAS_COM_RECV_STAT state_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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


/** @brief      检查数据类型是否有效接口函数，主要检查是否为响应消息，以及协议版本
 *  @param[in]  IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD dataType_  
 *  @param[out] 无
 *  @return     static BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年12月10日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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



/** @brief      设置维护消息数据类型值函数接口
 *  @param[in]  IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD *dataTypePtr_  
 *  @param[in]  unsigned char value_                                        
 *  @param[out] 无
 *  @return     void
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年12月2日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

*/
void SetDateTypeWordValue(
        IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD *dataTypePtr_, 
        unsigned char value_)
{
	//logMsg("======dataTypePtr_ :%d=====\r\n", value_,0,0,0,0,0);
   memcpy((unsigned char *)dataTypePtr_, &value_, 1);
}
/** @brief      处理同步字的第一个字节0xAA
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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


/** @brief      处理同步字的第二个字节0x55
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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



/** @brief      处理源地址字节函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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

/** @brief      处理目标地址字节函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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


/** @brief      处理数据类型字节函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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


/** @brief      命令字字节处理函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

*/
static BOOL Idas_ComRx_WaitCmd_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    BOOL result = 1;
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
    

    if( 0 == (pHandle->rxDataType & MSG_DATA_TYPE_CMD_LEN_BITMASK) )
    {
        /* 命令字为一字节的情况 */
    	pHandle->rxDataBufStat = IDAS_COM_WAIT_DATA_LEN;
        pHandle->rxCmd = recvByte;
        pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
        pHandle->rxDataPackLen = 0;

    }
    else
    {
        /* 命令字为二字节的情况,按高字节优先来传送 */
    	pHandle->rxCmd = 
            (pHandle->rxCmd << 8) | recvByte;
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
    	pHandle->rxStep++;
        if(TWO_STEPS == pHandle->rxStep )
        {   
            /* 命令字为最后字节的情况 */
        	pHandle->rxStep = 0;
        	pHandle->rxDataBufStat = IDAS_COM_WAIT_DATA_LEN;
            pHandle->rxDataPackLen = 0;
			
        }
   
    } 
    /* 检查命令字是否有效 */   
     if( pHandle->rxCmd >= pHandle->MaxCmd )
     {
         //logMsg("The received cmd is invalidate!",0,0,0,0,0,0);
         pHandle->rxStep = 0;
         pHandle->rxDataBufStat = IDAS_COM_WAIT_0XAA;
         result = -1;
     } 
    return result;
}


/** @brief      数据长度处理函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

*/
static BOOL Idas_ComRx_WaitDataLen_Hdr(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
 
    if( 0 == (pHandle->rxDataType & MSG_DATA_TYPE_DATA_LEN_BITMASK) )
    {
        /* 长度为一字节的情况 */
    	pHandle->rxDataPackLen = recvByte;
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
        /* 如果数据长度为0， 那么直接进入接收CheckSum的状态，否则接收数据*/
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
        /* 长度为二字节的情况,按高字节优先来传送 */
    	pHandle->rxDataPackLen = 
            (pHandle->rxDataPackLen << 8) | recvByte;
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
    	pHandle->rxStep++;
        if(TWO_STEPS == pHandle->rxStep)
        {   
            /* 命令字为最后字节的情况 */   
        	pHandle->rxStep = 0;
            if(pHandle->rxDataPackLen > MAX_MAINT_MSG_LEN_PER_PACKET)
            {
                pHandle->rxDataBufStat = IDAS_COM_WAIT_0XAA;
                return -1;
            }
            /* 如果数据长度为0， 那么直接进入接收CheckSum的状态，否则接收数据*/
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


/** @brief      数据字节处理函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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
        /* 填入数据到接收数据缓冲区 */
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                    = recvByte;
    }
    else
    {
        /* 最后一个字节数据 */
    	pHandle->rxDataBuf[pHandle->rxDataBufLen++] 
                                            = recvByte;
    	pHandle->rxDataBufStat = IDAS_COM_WAIT_CHECKSUM;
        pHandle->rxDataBufLen %= (MAX_MAINT_MSG_LEN_PER_PACKET + MAX_HEAD_LENGTH);
        pHandle->rxCheckSum = 0;
    }
    return 1;
}

/** @brief      校验和字节处理函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

*/
static BOOL Idas_ComRx_WaitCheckSum_Hdr
(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
   
    unsigned char recvByte;
    Idas_COM_Handle *pHandle = (Idas_COM_Handle *)pComRxTxDataBuf_->handle;
    recvByte = pHandle->curByteValue;
  



    /* 命令字为二字节的情况,按高字节优先来传送 */
            pHandle->rxCheckSum = 
        (pHandle->rxCheckSum << 8) | recvByte;
    
    pHandle->rxStep++;
    if(TWO_STEPS == pHandle->rxStep )
    {   
        /* 命令字为最后字节的情况 */   
    	pHandle->rxDataBufStat = IDAS_COM_WAIT_0XA5;
        pHandle->rxStep = 0;
    }

    return 1;
}


/** @brief      同步码字节处理函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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



/** @brief      发送启动函数接口
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

*/
BOOL Idas_ComTx_Send(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_)
{
 

    /*设置相关标志，使得主循环可以发送Tx中的数据*/
   if(NULL == pComRxTxDataBuf_)
   {
	   return -1;
   }
    pComRxTxDataBuf_->txDataReadyFlag = 1;
    return 1;
}


/** @brief      接收到的维护数据解析成维护消息结构体处理函数
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pRxBuf_  
 *  @param[in]  IDAS_MAINT_MSG *pReqMsg_            
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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

    /* 检查数据类型的有效性 */
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

/** @brief      发送消息按维护协议打包处理函数
 *  @param[in]  IDAS_MAINT_MSG *pSendMsg_           
 *  @param[in]  IDAS_COM_RX_TX_BUF_STRUCT *pTxBuf_  
 *  @param[out] 无
 *  @return     BOOL
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年11月26日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

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
    /* 构建发送包头,加上同步字 */
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
    /* 构建发送包体 */
    if( pSendMsg_->msgHead.msgLen > 0)
    {
        memcpy((unsigned char *)&(pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)]),
            pSendMsg_->msgBodyPtr, pSendMsg_->msgHead.msgLen);
        pTxBuf_->txDataBufLen += pSendMsg_->msgHead.msgLen-1;
    }

    /* 计算发送checkSum校验码 */
    crc16 = CalcCRC16(&(pTxBuf_->txDataBuf[2]), pSendMsg_->msgHead.msgLen + dataOffset); 
    /* 构建发送包尾 */
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
            (unsigned char)( (crc16 >> 8) & 0xff); 
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 
            (unsigned char)( crc16 & 0xff); 
    
    pTxBuf_->txDataBuf[(++pTxBuf_->txDataBufLen)] = 0xa5;
    /* pTxBuf_->txDataBufLen指向最后一个有效字节，所以长度需要加1 */
    ++pTxBuf_->txDataBufLen;

    return 1;
}


/** @brief      维护串口CRC校验码计算函数
 *  @param[in]  unsigned char *ptr_  
 *  @param[in]  unsigned short len_  
 *  @param[out] 无
 *  @return     static unsigned short
 *  @note       修订记录  :
 *  @note       1.作    者: tang 2014年12月2日
 *  @note         修订内容: 创建初稿
 *  @note       2.作    者: --------- ----年--月--日
 *  @note         修订内容: ------------------------

*/
static unsigned short CalcCRC16(unsigned char *ptr_ ,unsigned short len_)
{
    unsigned short crc;
    unsigned char da;
    crc = 0;
    while( len_-- != 0 )
    {
        da =(unsigned char)(crc >> 8);    /* 以8位二进制数的形式暂存CRC的高8位 */
        crc <<= 8;              /* 左移8位 */
        crc ^= crc16_ta[da^(*ptr_)];  /* 高8位和当前字节相加后再查表求CRC ，再加上以前的CRC */
        ptr_++;
    }
    return crc ;
}




