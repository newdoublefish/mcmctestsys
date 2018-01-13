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
#include "toolBox.h" 
#include <rs232.h>
#include "relayHelper.h"
#include "crcCalc.h"
#include "common.h"
//#define DEBUG

/*
FE 0F 00 00 00 02 01 03 D1 92 
FE 0F 00 00 00 02 01 05 51 90
*/

BOOL buildDoPacket(RelayBuf *buffPtr,unsigned char doIndex,BOOL openFlag)
{
	if(buffPtr==NULL)
		return FALSE;
    buffPtr->data[buffPtr->len++] = 0xFE;
     buffPtr->data[buffPtr->len++] = 0x05;
     buffPtr->data[buffPtr->len++] = 0x00;
     buffPtr->data[buffPtr->len++] = doIndex;
     buffPtr->data[buffPtr->len++] = (unsigned char)((openFlag) ? 0xff : 0x00);
     buffPtr->data[buffPtr->len++] = 0x00;
    unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);
	 return TRUE;
}

BOOL buildALLDoPacket(RelayBuf *buffPtr,unsigned int doMask)
{
	if(buffPtr==NULL)
		return FALSE;
   	 buffPtr->data[buffPtr->len++] = 0xFE;
     buffPtr->data[buffPtr->len++] = 0x0F;
     buffPtr->data[buffPtr->len++] = 0x00;
     buffPtr->data[buffPtr->len++] = 0x00;
	 buffPtr->data[buffPtr->len++] = 0x00;
	 buffPtr->data[buffPtr->len++] = 0x20;
	 buffPtr->data[buffPtr->len++] = 0x04;
	 buffPtr->data[buffPtr->len++] = (unsigned char)(doMask & 0xff);
	 buffPtr->data[buffPtr->len++] = (unsigned char)(doMask>>8);
	 buffPtr->data[buffPtr->len++] = (unsigned char)(doMask>>16);
	 buffPtr->data[buffPtr->len++] = (unsigned char)(doMask>>24);
   	 unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);
	 return TRUE;
}


static BOOL SendRelayData(int portNum,RelayBuf buf)
{
     int hasSendDataC=0;
	 while(hasSendDataC<buf.len)
	 {
		   int currentSendData=0;
		   currentSendData=ComWrt(portNum,buf.data+hasSendDataC,buf.len-hasSendDataC);
		   if(currentSendData<0)
			   return FALSE;
		   hasSendDataC+=currentSendData;
	 }
	 return TRUE;
	 //PRINT("portNum:%d,hasSendDataC:%d",portNum,hasSendDataC);
}

BOOL getResult(int comPort,RelayBuf *recvBuf)
{
	recvBuf->len=0;
	SetComTime(comPort,0.2);

	recvBuf->len=ComRd(comPort,recvBuf->data,1024);
	if(recvBuf->len<0)
		return FALSE;
	return TRUE;
}

BOOL operateDo(RSCONFIG config,RelayBuf *bufPtr)
{
	if(OpenComConfig (config.portNum, NULL, config.baudRate,config.parity,config.dataBit,config.stopBit, 1024, 1024)<0)
	{
		WarnShow1(0,"打开继电器串口失败");
	     return FALSE;
	}

	if(SendRelayData(config.portNum,*bufPtr)<0)
	{
		goto ERROR;
	}
#ifdef DEBUG
	printf("send:");
	for(int i=0;i<bufPtr->len;i++)
	{
		printf("%x ",bufPtr->data[i]);
	}
	printf("\n");
#endif	
	
	memset(bufPtr,0,sizeof(RelayBuf));
	getResult(config.portNum,bufPtr);
	
#ifdef DEBUG
	printf("recv:");
	for(int i=0;i<bufPtr->len;i++)
	{
		printf("%x ",bufPtr->data[i]);
	}
	printf("\n");	
#endif	
	
	CloseCom(config.portNum);
	return TRUE;
ERROR:
	return FALSE;	
}

BOOL OpenDo(RSCONFIG config,int doMask){
	RelayBuf buf={0};
	buildDoPacket(&buf,doMask,1);
	return operateDo(config,&buf);
}
BOOL CloseDo(RSCONFIG config,int doMask){
	RelayBuf buf={0};
	buildDoPacket(&buf,doMask,0);
	return operateDo(config,&buf);
}

BOOL OperatDoSet(RSCONFIG config,unsigned int mask)
{
	RelayBuf buf={0};
	buildALLDoPacket(&buf,mask);
	return operateDo(config,&buf);
}


