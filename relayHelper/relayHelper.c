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
#include "settingEntity.h"
#define RESPONSE_TIME_OUT 1	
//#define DEBUG

/*
FE 0F 00 00 00 02 01 03 D1 92 
FE 0F 00 00 00 02 01 05 51 90
*/
#if 0
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
	SETTING s=getSetting();
	if(s.relayEnable==FALSE)
		return TRUE;
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
	buildDoPacket(&buf,(unsigned char)doMask,1);
	return operateDo(config,&buf);
}
BOOL CloseDo(RSCONFIG config,int doMask){
	RelayBuf buf={0};
	buildDoPacket(&buf,(unsigned char)doMask,0);
	return operateDo(config,&buf);
}

BOOL OperatDoSet(RSCONFIG config,unsigned int mask)
{
	RelayBuf buf={0};
	buildALLDoPacket(&buf,mask);
	return operateDo(config,&buf);
}

static BOOL ParseResultWithMask(RelayBuf *buf,unsigned int *value)
{
	if(buf->len<=0)
		return FALSE;
	unsigned short crc = ModbusCRC(buf->data,buf->len-2);
	unsigned short recvCrc = ((buf->data[buf->len-2]) <<8) | (buf->data[buf->len-1] & 0xff); 
	if(crc == recvCrc)
	{
		if(buf->data[1]==0x0F)
		{
			return TRUE;
		}else if(buf->data[1]==0x01)
		{
			int byteNum = buf->data[2];
			for(int i=0;i<byteNum;i++)
			{
				//buf[3+i]
				*value |=(buf->data[3+i]<<(8*i));
			}
			return TRUE;
		}
		return FALSE;
		
	}else{
		return FALSE; 
	}
}

BOOL buildDoPacketWithMask(RelayBuf *buffPtr,unsigned char doIndex,BOOL openFlag)
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

BOOL buildALLDoPacketWithMask(RelayBuf *buffPtr,unsigned int doMask,unsigned int mask)
{
	if(buffPtr==NULL)
		return FALSE;
	//doMask&0xFFFF0000;
	 if((mask&0xFFFF0000)>0){
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
	 }else{
   		buffPtr->data[buffPtr->len++] = 0xFE;
     	buffPtr->data[buffPtr->len++] = 0x0F;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
	 	buffPtr->data[buffPtr->len++] = 0x00;
	 	buffPtr->data[buffPtr->len++] = 0x10;
	 	buffPtr->data[buffPtr->len++] = 0x02;
	 	buffPtr->data[buffPtr->len++] = (unsigned char)(doMask & 0xff);
	 	buffPtr->data[buffPtr->len++] = (unsigned char)(doMask>>8);
   	 	unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);
	 
	 }
	 return TRUE;
	 
}


static BOOL SendRelayDataWithMask(int portNum,RelayBuf buf)
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

BOOL getResultWithMask(int comPort,RelayBuf *recvBuf)
{
	recvBuf->len=0;
	SetComTime(comPort,2);

	recvBuf->len=ComRd(comPort,recvBuf->data,1024);
	if(recvBuf->len<0)
		return FALSE;
	return TRUE;
}

BOOL operateDoWithMask(RSCONFIG config,RelayBuf *bufPtr)
{
	if(OpenComConfig (config.portNum, NULL, config.baudRate,config.parity,config.dataBit,config.stopBit, 1024, 1024)<0)
	{
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
	if(FALSE==getResult(config.portNum,bufPtr))
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

BOOL OpenDoWithMask(RSCONFIG config,int doMask){
	RelayBuf buf={0};
	buildDoPacket(&buf,doMask,1);
	return operateDo(config,&buf);
}
BOOL CloseDoWithMask(RSCONFIG config,int doMask){
	RelayBuf buf={0};
	buildDoPacket(&buf,doMask,0);
	return operateDo(config,&buf);
}

BOOL OperatDoSetWithMask(RSCONFIG config,unsigned int doMask,unsigned int mask)
{
	
	RelayBuf buf={0};
	buildALLDoPacketWithMask(&buf,doMask,mask);
	if(FALSE==operateDo(config,&buf))
		return FALSE;
	if(FALSE== ParseResultWithMask(&buf,NULL))
	{
		return FALSE;	
	}
	return TRUE;
}

BOOL buildCheckOptoCouplerPacket(RelayBuf *buffPtr,unsigned int mask)
{
	if(buffPtr==NULL)
		return FALSE;
	if((mask&0x0000FFFF)>0)
	{
    	buffPtr->data[buffPtr->len++] = 0xFE;
     	buffPtr->data[buffPtr->len++] = 0x02;
        buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x10;
    	unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);
	}else{
		return FALSE;
	}
	return TRUE;		
}

BOOL buildCheckPacketWithMask(RelayBuf *buffPtr,unsigned int mask)
{
	if(buffPtr==NULL)
		return FALSE;
	if((mask&0xFFFF0000)>0)
	{
    	buffPtr->data[buffPtr->len++] = 0xFE;
     	buffPtr->data[buffPtr->len++] = 0x01;
        buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x20;
    	unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);
	}else{
    	buffPtr->data[buffPtr->len++] = 0xFE;
     	buffPtr->data[buffPtr->len++] = 0x01;
        buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x10;
    	unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);	
	}
	 return TRUE;
}

BOOL CheckDoSetWithMask(RSCONFIG config,unsigned int *value,unsigned int mask)
{
	RelayBuf buf={0};
	buildCheckPacketWithMask(&buf,mask);
	if(FALSE==operateDo(config,&buf))
		return FALSE;
	if(FALSE== ParseResultWithMask(&buf,value))
	{
		return FALSE;	
	}
	return TRUE;	
}

#else
static BOOL ParseResult(RelayBuf *buf,unsigned int *value)
{
	if(buf->len<=0)
		return FALSE;
	unsigned short crc = CalculateCrc(buf->data,buf->len-2);
	unsigned short recvCrc = ((buf->data[buf->len-1]) <<8) | (buf->data[buf->len-2] & 0xff); 
	if(crc == recvCrc)
	{
		if(buf->data[1]==0x0F)
		{
			return TRUE;
		}else if(buf->data[1]==0x01 || buf->data[1]==0x02)
		{
			int byteNum = buf->data[2];
			for(int i=0;i<byteNum;i++)
			{
				//buf[3+i]
				*value |=(buf->data[3+i]<<(8*i));
			}
			return TRUE;
		}
		return FALSE;
		
	}else{
		return FALSE; 
	}
}

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

BOOL buildALLDoPacket(RelayBuf *buffPtr,unsigned int doMask,unsigned int mask)
{
	if(buffPtr==NULL)
		return FALSE;
	//doMask&0xFFFF0000;
	 if((mask&0xFFFF0000)>0){
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
	 }else if((mask&0x0000FF00)>0){
   		buffPtr->data[buffPtr->len++] = 0xFE;
     	buffPtr->data[buffPtr->len++] = 0x0F;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
	 	buffPtr->data[buffPtr->len++] = 0x00;
	 	buffPtr->data[buffPtr->len++] = 0x10;
	 	buffPtr->data[buffPtr->len++] = 0x02;
	 	buffPtr->data[buffPtr->len++] = (unsigned char)(doMask & 0xff);
	 	buffPtr->data[buffPtr->len++] = (unsigned char)(doMask>>8);
   	 	unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);
	 }else if((mask&0x000000FF)>0){
   		buffPtr->data[buffPtr->len++] = 0xFE;
     	buffPtr->data[buffPtr->len++] = 0x0F;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
	 	buffPtr->data[buffPtr->len++] = 0x00;
	 	buffPtr->data[buffPtr->len++] = 0x08;
	 	buffPtr->data[buffPtr->len++] = 0x01;
	 	buffPtr->data[buffPtr->len++] = (unsigned char)(doMask & 0xff);
   	 	unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);			 	
	 }
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
	SetComTime(comPort,RESPONSE_TIME_OUT);

	recvBuf->len=ComRd(comPort,recvBuf->data,1024);
	if(recvBuf->len<0)
		return FALSE;
	return TRUE;
}

BOOL operateDo(RSCONFIG config,RelayBuf *bufPtr)
{
	if(OpenComConfig (config.portNum, NULL, config.baudRate,config.parity,config.dataBit,config.stopBit, 1024, 1024)<0)
	{
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
	if(FALSE==getResult(config.portNum,bufPtr))
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

BOOL OpenDo(RSCONFIG config,int doMask){
	SETTING s=getSetting();
	if(s.relayEnable==FALSE)
		return TRUE;	
	RelayBuf buf={0};
	buildDoPacket(&buf,doMask,1);
	return operateDo(config,&buf);
}
BOOL CloseDo(RSCONFIG config,int doMask){
	SETTING s=getSetting();
	if(s.relayEnable==FALSE)
		return TRUE;	
	RelayBuf buf={0};
	buildDoPacket(&buf,doMask,0);
	return operateDo(config,&buf);
}

BOOL OperatDoSet(RSCONFIG config,unsigned int doMask,unsigned int mask)
{
	SETTING s=getSetting();
	if(s.relayEnable==FALSE)
		return TRUE;	
	RelayBuf buf={0};
	buildALLDoPacket(&buf,doMask,mask);
	if(FALSE==operateDo(config,&buf))
		return FALSE;
	if(FALSE== ParseResult(&buf,NULL))
	{
		return FALSE;	
	}
	return TRUE;
}

BOOL buildCheckPacket(RelayBuf *buffPtr,unsigned int mask)
{
	if(buffPtr==NULL)
		return FALSE;
	if((mask&0xFFFF0000)>0)
	{
    	buffPtr->data[buffPtr->len++] = 0xFE;
     	buffPtr->data[buffPtr->len++] = 0x01;
        buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x20;
    	unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);
	}else{
    	buffPtr->data[buffPtr->len++] = 0xFE;
     	buffPtr->data[buffPtr->len++] = 0x01;
        buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x10;
    	unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);	
	}
	 return TRUE;
}

BOOL CheckDoSet(RSCONFIG config,unsigned int *value,unsigned int mask)
{
	RelayBuf buf={0};
	buildCheckPacket(&buf,mask);
	if(FALSE==operateDo(config,&buf))
		return FALSE;
	if(FALSE== ParseResult(&buf,value))
	{
		return FALSE;	
	}
	return TRUE;	
}

BOOL buildCheckOptoCouplerPacket(RelayBuf *buffPtr,unsigned int mask)
{
	if(buffPtr==NULL)
		return FALSE;
	if((mask&0x0000FFFF)>0)
	{
    	buffPtr->data[buffPtr->len++] = 0xFE;
     	buffPtr->data[buffPtr->len++] = 0x02;
        buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x00;
     	buffPtr->data[buffPtr->len++] = 0x10;
    	unsigned short crc = CalculateCrc(buffPtr->data,buffPtr->len);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc & 0xff);
     	buffPtr->data[buffPtr->len++] = (unsigned char)(crc>>8);
	}else{
		return FALSE;
	}
	return TRUE;		
}

BOOL CheckDoCheckOptoCoupler(RSCONFIG config,unsigned int *value,unsigned int mask)
{
	RelayBuf buf={0};
	buildCheckOptoCouplerPacket(&buf,mask);
	if(FALSE==operateDo(config,&buf))
		return FALSE;
	if(FALSE== ParseResult(&buf,value))
	{
		return FALSE;	
	}
	return TRUE;	
}
#endif


