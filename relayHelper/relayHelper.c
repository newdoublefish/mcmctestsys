/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：relayHelper.c
 // 文件功能描述:继电器操作 
 //
 // 
 // 创建标识：曾衍仁 20150930
 //
 // 
 // 修改描述：
 //-------------------------------------------------------------------------*/
#include <rs232.h>  
#include "relayHelper.h"
#include "log.h"
#include "debug.h"
#include "settingConfig.h"

#define STATE_HEADER_0XAA 0
#define STATE_HEADER_0X55 1
#define STATE_TESTTYPE 2
#define STATE_TESTVALUE 3
#define STATE_DATA 4
#define STATE_CHECKSUM 5
#define STATE_TAIL 6

static int relayLock=0; 



typedef struct{
    int state;
	int validLen;
	RELAY_REPONSE response;
}RESPONSE;


void createRelayLock()
{
     CmtNewLock ("relayLock", OPT_TL_PROCESS_EVENTS_WHILE_WAITING, &relayLock);//创建锁  
}


BOOL replayResponseHandle(RESPONSE *response,unsigned char data)
{
	 BOOL ret=FALSE;
     switch(response->state)
	 {
	    case  STATE_HEADER_0XAA:
			if(data==0xaa)
			{
				
				response->state=STATE_HEADER_0X55;
			}	
			break;
		case  STATE_HEADER_0X55:
			if(data==0x55)
			{
				response->response.synHeader|=0xaa55;
			    response->state=STATE_TESTTYPE;
			}else
			{
			    response->state=STATE_HEADER_0XAA;
			}	
			break;
		case STATE_TESTTYPE:
            response->response.testType=data;
			response->state=STATE_TESTVALUE;
			break;
			
		case STATE_TESTVALUE:
			response->response.testValue=data;
		    response->state=STATE_DATA;
			response->validLen=8;    
			break;
			
		case STATE_DATA:	
			response->response.data |=data<<((8-response->validLen) *8);
			response->validLen--;
			if(response->validLen<=0)
			{
				response->state=STATE_CHECKSUM; 
			}
			break;
		case STATE_CHECKSUM:
			response->response.checksum=data;
			response->state=STATE_TAIL;
			break;
			
		case STATE_TAIL:
			response->response.tail=data;
			ret=TRUE;
			break;
		default:
			break;
	 }
	 return ret;
}

BOOL getResult(int comPort,RelayBuf *recvBuf)
{
	BOOL ret=FALSE;
	RESPONSE response;
	memset(&response,0,sizeof(RESPONSE));
	unsigned char buff[1024]={0};
	BOOL flag=TRUE;
	int noRecvData=0;
	recvBuf->len=0;
	SetComTime(comPort,0.2);
    while(flag)
	{
		int len=0;
	    len=ComRd(comPort,buff,1024);
		if(len<=0)
		{
		    noRecvData++;
			if(noRecvData>10)
			{
			    flag=FALSE;
			}	
		}	
		/*for(int i=0;i<=len;i++)
	    {
	      PRINT("buff[%d]=0x%x",i,buff[i]);
	    }*/
		
		for(int i=0;i<len;i++) 
		{
		   recvBuf->data[recvBuf->len++]=buff[i];
		   if(TRUE==replayResponseHandle(&response,buff[i]))
		   {
			    ret=TRUE;
			    flag=FALSE;
			    break; 
		   }	 
		}	
	}
	//PRINT("response.tail:%d\n",response.response.tail);
	return ret;
}

static RelayBuf BuildRelayBuf(int eutId,RELAY_OPERATION operation,BOOL isValue)
{				   
	 
	 RelayBuf buf;
	 memset(&buf,0,sizeof(RelayBuf));
	 unsigned char checksum=0;
	 buf.len=0;
	 buf.data[buf.len++]=0xaa;
	 buf.data[buf.len++]=0x55;
	 buf.data[buf.len++]=0x01;
	 buf.data[buf.len++]=0x02; 
	 buf.data[buf.len++]=operation.testType;
	 if(isValue==TRUE)
		buf.data[buf.len++]=operation.testValue;  
	 else
	 {
	    buf.data[buf.len++]=1<<(eutId-1); 
	 }
	    
	 
	 buf.data[buf.len++]=(operation.data >>8) & 0xff; 
	 buf.data[buf.len++]=operation.data & 0xff;
	 
	 for(int i=0;i<buf.len;i++)
	 {
	     checksum+=buf.data[i];
	 }	 
	 buf.data[buf.len++]=~checksum+1;
	 buf.data[buf.len++]=0xa5;
/*	 
	 for(int i=0;i<buf.len;i++)
	 {
	   printf("%x,",buf.data[i]);
	 }
	 printf("\n");
*/	 //buf.len++;
	 return buf;
}

static void SendRelayData(int portNum,RelayBuf buf)
{
     int hasSendDataC=0;
	 while(hasSendDataC<buf.len)
	 {
		   int currentSendData=0;
		   currentSendData=ComWrt(portNum,buf.data+hasSendDataC,buf.len-hasSendDataC);
		   hasSendDataC+=currentSendData;
	 }
	 //PRINT("portNum:%d,hasSendDataC:%d",portNum,hasSendDataC);
}


BOOL SendRelayOperation(EUT eut,RELAY_OPERATION operation,BOOL isValue)
{
	
	BOOL ret=TRUE;
	SETTING setting=GetSetting();
	if(!setting.relayEnable)
		return FALSE;
	//if(setting.)
	
	if(relayLock==0)
	{
	    createRelayLock();
	}
	CmtGetLock (relayLock);  
	if(OpenComConfig (eut.relayConfig.portNum, NULL, eut.relayConfig.baudRate,eut.relayConfig.parity,eut.relayConfig.dataBit,eut.relayConfig.stopBit, 1024, 1024)<0)
	{
		 LOG_EVENT_FORMAT(LOG_ERROR,"relay port open error!! euName:%s,port:%d,rate:%d,parity:%d,dataBit:%d,stopBit:%d\n",
		       eut.eutName,eut.relayConfig.portNum, eut.relayConfig.baudRate,eut.relayConfig.parity,eut.relayConfig.dataBit,eut.relayConfig.stopBit);
	     return FALSE;
	}
	//PRINT("relay port open error!! euName:%s,port:%d,rate:%d,parity:%d,dataBit:%d,stopBit:%d\n",
	//	       eut.eutName,eut.relayConfig.portNum, eut.relayConfig.baudRate,eut.relayConfig.parity,eut.relayConfig.dataBit,eut.relayConfig.stopBit);
	//组包
	RelayBuf buf=BuildRelayBuf(eut.index,operation,isValue);
	//发送值
	
	SendRelayData(eut.relayConfig.portNum,buf); //发送继电器操作
	Delay(0.2);
	RelayBuf recvBuf;
	ret=getResult(eut.relayConfig.portNum,&recvBuf);  //等待继电器操作响应，最多尝试5此，否则出错
	CloseCom(eut.relayConfig.portNum);
	CmtReleaseLock (relayLock);  
	return ret;
}

BOOL SendRelayOperationWithRelayBuf(EUT eut,RELAY_OPERATION operation,BOOL isValue,RelayBuf *sndBuf,RelayBuf *rcvBuf)
{
	
	BOOL ret=TRUE;
	if(OpenComConfig (eut.relayConfig.portNum, NULL, eut.relayConfig.baudRate,eut.relayConfig.parity,eut.relayConfig.dataBit,eut.relayConfig.stopBit, 1024, 1024)<0)
	{
		 LOG_EVENT_FORMAT(LOG_ERROR,"relay port open error!! euName:%s,port:%d,rate:%d,parity:%d,dataBit:%d,stopBit:%d\n",
		       eut.eutName,eut.relayConfig.portNum, eut.relayConfig.baudRate,eut.relayConfig.parity,eut.relayConfig.dataBit,eut.relayConfig.stopBit);
	     return FALSE;
	}
	//PRINT("relay port open error!! euName:%s,port:%d,rate:%d,parity:%d,dataBit:%d,stopBit:%d\n",
	//	       eut.eutName,eut.relayConfig.portNum, eut.relayConfig.baudRate,eut.relayConfig.parity,eut.relayConfig.dataBit,eut.relayConfig.stopBit);
	//组包
	RelayBuf buf=BuildRelayBuf(eut.index,operation,isValue);
	//发送值
	
	SendRelayData(eut.relayConfig.portNum,buf); //发送继电器操作
	Delay(0.2);
	ret=getResult(eut.relayConfig.portNum,rcvBuf);  //等待继电器操作响应，最多尝试5此，否则出错
	CloseCom(eut.relayConfig.portNum);
	
	sndBuf->len=buf.len;
	memcpy(sndBuf->data,buf.data,buf.len);
	
	return ret;
}


