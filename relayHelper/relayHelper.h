/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：relayHelper.h
 // 文件功能描述:继电器操作 
 //
 // 
 // 创建标识：曾衍仁 20150930
 //
 // 
 // 修改描述：
 //-------------------------------------------------------------------------*/
#ifndef __RELAYHELPER_H__
#define __RELAYHELPER_H__
#include "eutConfig.h"   
typedef struct
{
   unsigned char testType;	   //测试类型
   unsigned char testValue;	   //测试值
   unsigned short data;		  //测试数据
}RELAY_OPERATION;

typedef struct
{
   unsigned short synHeader;  //同步头   0xAA55
   unsigned char sndDevice;   //发送设备
   unsigned char recvDevice;  //接收设备
   unsigned char testType;	   //测试类型
   unsigned char testValue;	   //测试值
   unsigned short data;		  //测试数据
   unsigned char checksum;	  //校验码   前面所有字节之和取256的补码
   unsigned char tail;		 //同步尾 0XA5
}RELAY_REQUEST;

typedef struct
{
   unsigned short synHeader;  //同步头   0xAA55
   unsigned char testType;	   //测试类型
   unsigned char testValue;	   //测试值
   unsigned long data;		  //测试数据
   unsigned char checksum;	  //校验码   异或校验和 
   unsigned char tail;		 //同步尾 0XA5
}RELAY_REPONSE;

typedef struct
{
    int len;
	unsigned char data[1024];
}RelayBuf;


BOOL SendRelayOperation(EUT eut,RELAY_OPERATION operation,BOOL isValue);

BOOL SendRelayOperationWithRelayBuf(EUT eut,RELAY_OPERATION operation,BOOL isValue,RelayBuf *sndBuf,RelayBuf *rcvBuf);  
#endif  


