/******************************************************************************

                  版权所有 (C), 2014-2020, 广州航新第一设计所                  

 ******************************************************************************/
/** @file    idasMaintainUARTInterface.h
 *  @version 初稿
 *  @author  tang
 *  @date    2014年11月26日 14时55分
 *  @brief   维护串口协议处理CBB外部接口描述头文件
 *  @note    修订记录:
 *  @note    1.作    者: tang 2014年11月26日
 *  @note      修订内容: 创建初稿
 *  @note    2.作    者: -------- ----年--月--日
 *  @note      修订内容: -------------------
 *  @note              : -------------------

 ******************************************************************************/


#ifndef IDAS_COMRXTX_INTERFACE_H
#define IDAS_COMRXTX_INTERFACE_H

#define CURRENT_MSG_PROTO_VER 0
#define MAX_MAINT_MSG_LEN_PER_PACKET 4096 /* 每一个维护消息包的最大长度 */
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



/********************维护消息数据的头定义****************************************
 * 名称            长度     注解
 * 同步头           2    0xAA55
 * 发送设备地址     1
 * 接收设备地址     1
 * 数据类型         1    
 * 命令字          1/2  长度由数据类型确定
 * 数据长度        1/2  根据数据类型来确定数据长度为1字节 
 *                    或者2字节单位：字节,
 *                    最长限制为4096字节长度
****************************************************************************/
typedef struct
{
    unsigned int syscWordH;
    unsigned short addrOfSender;
    unsigned short addrOfRecv;
    IDAS_MAINT_MSG_HEADER_DATA_TYPE_WORD dataType;
    unsigned short cmd; 
    unsigned int msgLen; /* 消息体中内容的长度 */
}IDAS_MAINT_MSG_HEADER;

/************数据消息包尾******************************************************
 * 名称            长度     注解
 * 校验码           2
 * 同步尾           1      0xa5
 ***************************************************************************/
typedef struct
{
    unsigned int crcCode;
    unsigned short syscWordT;
}IDAS_MAINT_MSG_TAIL;



/************数据消息格式******************************************************
 * 消息包括三个部分：
 *    - 消息数据包头
 *    - 消息数据包尾
 *    - 消息数据体
 ***************************************************************************/
typedef struct
{
    IDAS_MAINT_MSG_HEADER msgHead;
    IDAS_MAINT_MSG_TAIL msgTail;
    void * msgBodyPtr;
}IDAS_MAINT_MSG;

/* 接收缓冲区的基本状态定义 */
typedef enum 
{   
    IDAS_COM_WAIT_0XAA,                     /* 等待同步字第一字节0xaa */
    IDAS_COM_WAIT_0X55,                     /* 等待同步字第二字节0x55 */
    IDAS_COM_WAIT_SRC_ADDR,                 /* 等待源地址 */
    IDAS_COM_WAIT_DST_ADDR,                 /* 等待目标地址 */
    IDAS_COM_WAIT_DATA_TYPE,                /* 等待数据类型 */
    IDAS_COM_WAIT_CMD,                      /* 等待命令字 */
    IDAS_COM_WAIT_DATA_LEN,                 /* 等待数据长度 */
    IDAS_COM_WAIT_DATA,                     /* 等待接收数据 */
    IDAS_COM_WAIT_CHECKSUM,                 /* 等待接收数据校验码 */
    IDAS_COM_WAIT_0XA5,                     /* 等待接收数据校验码 */
    IDAS_COM_DATA_RDY                       /* 数据可以被处理 */
}IDAS_COM_RECV_STAT;



/* 串口接口类型定义 */
typedef enum 
{
    RS422,
    RS485,
    NOVALID
}COM_TYPE;
/* 接收缓冲区的结构定义 */

typedef struct
{  
	unsigned int		 handle;  
    unsigned char                txDataReadyFlag; /*发送数据准备好*/
    COM_TYPE             comType;            /* 该数据缓冲区的所属接口类型 */ 
    
     
    unsigned short       txDataBufLen;                 /* 数据发送缓冲长度 */
    unsigned char        *txDataBuf;  /* 数据发送缓冲 */ 
	void *               fatherPtr;
}IDAS_COM_RX_TX_BUF_STRUCT;



/**************************************************************************
函数名:   IdasComRxTxInit
功能描述: 接收发送端口初始化函数
输入参数: IDAS_COM_RX_TX_BUF_STRUCT缓冲区指针
返回值: OK 或者 ERROR
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
BOOL IdasComRxTxInit(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_, unsigned short maxCmd, void *pFunc);

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
BOOL IdasComRxTxDeInit(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);

/**************************************************************************
函数名:   IdasComRxEventHandler
功能描述: 接收数据消息相应函数。该函数负责字节的分析，接收状态函数的调用处理，
          接收状态的迁移等动作
输入参数: IDAS_COM_RX_TX_BUF_STRUCT缓冲区指针
返回值: OK 或者 ERROR
***************************************************************************/
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
BOOL IdasComRxEventHandler(
        IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_,
        unsigned char byteValue_);

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
BOOL Idas_ComTx_Send(IDAS_COM_RX_TX_BUF_STRUCT *pComRxTxDataBuf_);

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
BOOL ParseRXComPacket(IDAS_COM_RX_TX_BUF_STRUCT *pRxBuf_, IDAS_MAINT_MSG *pReqMsg_);


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
 BOOL BuildTxComPacket(IDAS_MAINT_MSG *pSendMsg_, IDAS_COM_RX_TX_BUF_STRUCT *pTxBuf_ );

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
        unsigned char value_);


#endif
