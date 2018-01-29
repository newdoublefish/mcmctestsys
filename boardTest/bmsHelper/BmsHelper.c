#include "toolbox.h"
#include <rs232.h>
#include <ansi_c.h>
#include "BmsHelper.h"
#include "CrcCalc.h"
#include "common.h"

#define DEBUG

typedef struct
{
    int len;
	unsigned char data[1024];
}BmsBuf;

static BOOL ParseResponse(BmsBuf buf,tBmsItem *item)
{
	if(buf.len<=0)
		return FALSE;
	unsigned short crc = CalculateCrc(buf.data,buf.len-2);
	unsigned short recvCrc = ((buf.data[buf.len-1]) <<8) | (buf.data[buf.len-2] & 0xff); 
	if(crc == recvCrc)
	{
		if(buf.data[1]==0x03)
		{
			item->value=((buf.data[3]) <<8) | (buf.data[4] & 0xff);
			return TRUE;
		}else if(buf.data[1]==0x06)
		{
			return TRUE;
		}
	}else{
		return FALSE; 
	}
}

static BOOL getBmsResponse(int comPort,BmsBuf *recvBuf)
{
	recvBuf->len=0;
	SetComTime(comPort,3);

	recvBuf->len=ComRd(comPort,recvBuf->data,1024);
	if(recvBuf->len<0)
		return FALSE;
	return TRUE;
}

static BOOL SendBmsRequest(int portNum,BmsBuf buf)
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

BOOL operateBms(RSCONFIG config,BmsBuf *bufPtr)
{
	if(OpenComConfig (config.portNum, NULL, config.baudRate,config.parity,config.dataBit,config.stopBit, 1024, 1024)<0)
	{
		WarnShow1(0,"´®¿Ú´ò¿ªÊ§°Ü");
	     return FALSE;
	}

	if(FALSE==SendBmsRequest(config.portNum,*bufPtr))
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
	
	memset(bufPtr,0,sizeof(BmsBuf));
	if(FALSE==getBmsResponse(config.portNum,bufPtr))
	{
		goto ERROR;
	}
	
	
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

BOOL buildGetPacket(BmsBuf *buffPtr,tBmsItem item)
{
	if(buffPtr==NULL)
		return FALSE;
#if 1  	
	 buffPtr->data[buffPtr->len++] = 0x01;
	 buffPtr->data[buffPtr->len++] = 0x03;
	 buffPtr->data[buffPtr->len++] = (unsigned char)((item.address>>8) & 0xff); 
	 buffPtr->data[buffPtr->len++] = (unsigned char)(item.address&0xff); 
	 buffPtr->data[buffPtr->len++] = 0x00; 
	 buffPtr->data[buffPtr->len++] = 0x01; 	 
   	 unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);

     buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8); 	 
	 return TRUE;
#else
	 buffPtr->data[buffPtr->len++] = 0x01;
	 buffPtr->data[buffPtr->len++] = 0x04;
	 buffPtr->data[buffPtr->len++] = 0x12; 
	 buffPtr->data[buffPtr->len++] = 0x03; 
	 buffPtr->data[buffPtr->len++] = 0x00; 
	 buffPtr->data[buffPtr->len++] = 0x01; 	 
   	 unsigned short crc = ModbusCRC(buffPtr->data,buffPtr->len);

     buffPtr->data[buffPtr->len++] =0xC4;
     buffPtr->data[buffPtr->len++] =0xB2; 	 
	 return TRUE;	 
#endif	 
	 
}


BOOL BmsGetItem(RSCONFIG config,tBmsItem *itemPtr)
{
	BmsBuf buf={0};
	buildGetPacket(&buf,*itemPtr);
	if(FALSE==operateBms(config,&buf))
		return FALSE;
	if(FALSE==ParseResponse(buf,itemPtr))  	
		return FALSE;
	return TRUE;
}

BOOL buildSetPacket(BmsBuf *buffPtr,tBmsItem item)
{
	if(buffPtr==NULL)
		return FALSE;
   	 buffPtr->data[buffPtr->len++] = 0x01;
	 buffPtr->data[buffPtr->len++] = 0x06;
	 buffPtr->data[buffPtr->len++] = (unsigned char)((item.address>>8) & 0xff); 
	 buffPtr->data[buffPtr->len++] = (unsigned char)(item.address&0xff); 
	 buffPtr->data[buffPtr->len++] = (unsigned char)((item.value>>8) & 0xff);
	 buffPtr->data[buffPtr->len++] = (unsigned char)(item.value&0xff); 	 
   	 unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);

     buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8); 	 
	 return TRUE;
}


BOOL BmsSetItem(RSCONFIG config,tBmsItem item)
{
	BmsBuf buf={0};
	buildSetPacket(&buf,item);
	if(FALSE==operateBms(config,&buf))
		return FALSE;
	if(FALSE==ParseResponse(buf,&item))  	
		return FALSE;	
	return TRUE;
}
