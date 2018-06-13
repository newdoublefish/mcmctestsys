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
#include"UartUtil.h"
#include <rs232.h>     
#include "log.h"
//#define DEBUG
tUartRx *createUartRx(RSCONFIG config,int maxRxLen,int timeOut)
{
	tUartRx *tur =(tUartRx *)malloc(sizeof(tUartRx));
	if(tur!=NULL)
	{
		tur->rsconfig = config;
		tur->rxLen = 0;
		tur->rxBuffer = (unsigned char *)malloc(maxRxLen);
		tur->maxRxLen = maxRxLen;
		if(tur->rxBuffer!=NULL)
		{
			tur->timeOut = timeOut;
			return tur;
		}else{
			free(tur);
		}
	}
	return NULL;
}

BOOL releaseUartRx(tUartRx *tur)
{
	free(tur->rxBuffer);
	free(tur);
	return TRUE;
}

static BOOL SendUartData(tUartRx *ptr)
{
     int hasSendDataC=0;
	 while(hasSendDataC<ptr->rxLen)
	 {
		   int currentSendData=0;
		   currentSendData=ComWrt(ptr->rsconfig.portNum,ptr->rxBuffer+hasSendDataC,ptr->rxLen-hasSendDataC);
		   if(currentSendData<0)
			   return FALSE;
		   hasSendDataC+=currentSendData;
	 }
	 return TRUE;
	 //PRINT("portNum:%d,hasSendDataC:%d",portNum,hasSendDataC);
}

BOOL getUartResponse(tUartRx *ptr)
{
	ptr->rxLen = 0;
	SetComTime(ptr->rsconfig.portNum,ptr->timeOut);

	ptr->rxLen = ComRd(ptr->rsconfig.portNum,ptr->rxBuffer,ptr->maxRxLen);
	if(ptr->rxLen<0)
		return FALSE;
	return TRUE;
}

void uartRxLog(int portNum,char *type,tUartRx *ptr)
{
	if(ptr->rxLen <=0)
		return;
	unsigned char *tt = (unsigned char *)malloc(ptr->rxLen*3);
	memset(tt,0,ptr->rxLen);
	for(int i=0;i<ptr->rxLen;i++)
	{
		char hexStr[5] = {0};
		sprintf(hexStr,"%x ",ptr->rxBuffer[i]);
		strcat(tt,hexStr);						
	}
	LOG_EVENT_FORMAT(LOG_INFO,"com:%d,%s:%s",portNum,type,tt);
	free(tt);
}

BOOL doUartRxProcess(tUartRx *ptr)
{
	if(OpenComConfig (ptr->rsconfig.portNum, NULL, ptr->rsconfig.baudRate,ptr->rsconfig.parity,ptr->rsconfig.dataBit,ptr->rsconfig.stopBit, 1024, 1024)<0)
	{
		 LOG_EVENT_FORMAT(LOG_INFO,"uart open error :com:%d,rate:%d,parity:%d,dataBit:%d,stopBit:%d"
						  ,ptr->rsconfig.portNum,ptr->rsconfig.baudRate,ptr->rsconfig.parity,ptr->rsconfig.dataBit,ptr->rsconfig.stopBit);
	     return FALSE;
	}

	if(SendUartData(ptr)<0)
	{
		LOG_EVENT_FORMAT(LOG_INFO,"uart send error :com:%d,rate:%d,parity:%d,dataBit:%d,stopBit:%d"
						  ,ptr->rsconfig.portNum,ptr->rsconfig.baudRate,ptr->rsconfig.parity,ptr->rsconfig.dataBit,ptr->rsconfig.stopBit);		
		goto ERROR;
	}
	
	//LOG_EVENT_FORMAT(LOG_INFO,"uart:com:%d,rate:%d,parity:%d,dataBit:%d,stopBit:%d",ptr->rsconfig.portNum,ptr->rsconfig.baudRate,ptr->rsconfig.parity,ptr->rsconfig.dataBit,ptr->rsconfig.stopBit);
#ifdef DEBUG
	printf("send:");
	for(int i=0;i<ptr->rxLen;i++)
	{
		printf("%x ",ptr->rxBuffer[i]);
	}
	printf("\n");
#endif	
	uartRxLog(ptr->rsconfig.portNum,"send",ptr);	
	memset(ptr->rxBuffer,0,ptr->maxRxLen);
	if(FALSE==getUartResponse(ptr))
	{
		LOG_EVENT_FORMAT(LOG_INFO,"uart recv error :com:%d,rate:%d,parity:%d,dataBit:%d,stopBit:%d"
						  ,ptr->rsconfig.portNum,ptr->rsconfig.baudRate,ptr->rsconfig.parity,ptr->rsconfig.dataBit,ptr->rsconfig.stopBit);		
		
		goto ERROR;
	}
	
#ifdef DEBUG
	printf("recv:");
	for(int i=0;i<ptr->rxLen;i++)
	{
		printf("%x ",ptr->rxBuffer[i]);
	}
	printf("\n");	
#endif	
	uartRxLog(ptr->rsconfig.portNum,"recv",ptr);	
	CloseCom(ptr->rsconfig.portNum);
	return TRUE;
ERROR:
	return FALSE;		
}
