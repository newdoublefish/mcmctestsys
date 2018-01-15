#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include <tcpsupp.h>
#include <userint.h>
#include "StubNetService.h"
#include "common.h"
#define tcpChk(f) if ((tcpErr=(f))<0) {ReportTCPError(tcpErr); goto Done;} else    
//#define DEBUG	
tNET_SERVICE *gServicePtr=NULL;	
	
char *getCompletePacket(char *str)
{
	char *begin=NULL;
	char *end=NULL;
	begin =strstr(str,"begin");
	end=strstr(str,"end");
	if(begin!=NULL && end!=NULL && (end-begin > 0))
	{
		//是否保存完整包？？？？？？？？？？？？？？？？？？？？？？？？？
		*(end)='\0';
		begin = begin + 6;
		return begin;
	}else if(strstr(str,"ok")!=NULL){
		return str;	
	}else if(strstr(str,"当前序号")!=NULL)
	{
		return str;
	}else if(strstr(str,"设置当前")!=NULL)
	{
		return str;
	}else if(strstr(str,"finish")!=NULL)
	{
		return str; 
	}
	return NULL;
}
	
	
void processRecvData(tNET_SERVICE *servicePtr,char *buffer,int len)
{
	if(servicePtr->dataCnt+len<MAX_NET_RECV_BUFFER)
	{
		memcpy(servicePtr->buffer+servicePtr->dataCnt,buffer,len);
		servicePtr->dataCnt+=len;
		//printf("recv:%s\n",servicePtr->buffer);
		
		if((servicePtr->packetPtr=getCompletePacket(servicePtr->buffer))!=NULL)
		{
			servicePtr->setFlag = 0;
			servicePtr->dataCnt=0;
			/*if(servicePtr->onOnePacket!=NULL)
			{
				(*(ON_RECEIVE_ONE_PACKET)servicePtr->onOnePacket)(servicePtr);
			}
			servicePtr->onOnePacket=NULL;*/
			memset(servicePtr->packet,0,MAX_NET_RECV_BUFFER);
			sprintf(servicePtr->packet,"%s",servicePtr->packetPtr);
			memset(servicePtr->buffer,0,MAX_NET_RECV_BUFFER);
		}
	}else{
		servicePtr->dataCnt=0;   //抛弃前面的数据
		processRecvData(servicePtr,buffer,len);
	}
}


static int CVICALLBACK TCPCallback (unsigned int handle, int xType, 
	int errCode, void *callbackData)
{
	tNET_SERVICE *servicePtr = (tNET_SERVICE*)callbackData;
	switch (xType)
		{
		case TCP_DISCONNECT:
			
			//servicePtr->connected = 0;
			//DisconnectFromTCPServer(handle);
			if(servicePtr->onDisConnect!=NULL)
				(*(ON_NET_CONNECTED)(servicePtr->onDisConnect))(servicePtr);
			break;
		case TCP_DATAREADY:
			//DisableBreakOnLibraryErrors ();
			char buffer[4096]={0};
			int bytesRead=0;
			//do{
			bytesRead = ClientTCPRead (handle, buffer, 4096, 5);
#ifdef DEBUG
			printf("recv :%s",buffer);
#endif			
			if(servicePtr->setFlag==1)
			{
				processRecvData(servicePtr,buffer,bytesRead);
				
			}
			//}while(bytesRead>0);
			//EnableBreakOnLibraryErrors ();
			break;
		}
		
	return 0;
}

int sendTcpDataSync(const char *buffer,tNET_SERVICE *servicePtr,float timeOutSec){
	int ret=0;
	double baseTime;
	int messageSize = strlen(buffer);
	int bytesToWrite = messageSize;
	int bytesWritten=0;
	while (bytesToWrite > 0)
	{
		bytesWritten = ClientTCPWrite (servicePtr->connectHandler, &buffer[messageSize - bytesToWrite], bytesToWrite,5);
		bytesToWrite -= bytesWritten;
	}
	servicePtr->setFlag = 1;
	
	baseTime = Timer();
#if 1
	DisableBreakOnLibraryErrors ();
	while(1==servicePtr->setFlag){
		char buffer[512]={0};
		int bytesRead=0;
		bytesRead = ClientTCPRead (servicePtr->connectHandler, buffer, 512, 5);
		if(bytesRead>0)
			processRecvData(servicePtr,buffer,bytesRead);
		if(Timer()-baseTime > timeOutSec)
		{
			ret=-1;
			servicePtr->setFlag=0; 
			(*(ON_NET_TIMEOUT)servicePtr->onTimeOut)(servicePtr);
			continue;
		}		
	}
	EnableBreakOnLibraryErrors ();
#endif	
	return ret;
}


//准备使用第二种方法
int sendTcpData(const char *buffer,tNET_SERVICE *servicePtr,float timeOutSec)
{
	int ret=0;
	double baseTime;
	int messageSize = strlen(buffer);
	int bytesToWrite = messageSize;
	int bytesWritten=0;
	servicePtr->setFlag=1;
	//ClientTCPWrite(servicePtr->connectHandler,buffer,strlen(buffer),0);
	baseTime = Timer(); 
	while (bytesToWrite > 0)
	{
		ProcessTCPEvents();
		ProcessSystemEvents();		
		bytesWritten = ClientTCPWrite (servicePtr->connectHandler, &buffer[messageSize - bytesToWrite], bytesToWrite, 5);
		if(bytesWritten<0)
		{
			(*(ON_NET_TIMEOUT)servicePtr->onTimeOut)(servicePtr);
			return -1;
		}
		bytesToWrite -= bytesWritten;
		if(Timer()-baseTime > timeOutSec)
		{
			//if()
			servicePtr->setFlag=0; 
			(*(ON_NET_TIMEOUT)servicePtr->onTimeOut)(servicePtr);
			return -1;
		}		
	}
#ifdef DEBUG	
	printf("send:%s",buffer);
#endif	
	baseTime = Timer();
	while(servicePtr!=NULL && servicePtr->setFlag==1)
	{
		ProcessTCPEvents();
		ProcessSystemEvents();
		if(Timer()-baseTime > timeOutSec)
		{
			ret=-1;
			servicePtr->setFlag=0; 
			(*(ON_NET_TIMEOUT)servicePtr->onTimeOut)(servicePtr);
			break;
		}
	}
	
	return ret;
}

int startCommand(tNET_SERVICE *servicePtr,char *cmd)
{
	if(servicePtr==NULL)
	{
		return -1;
	}
	if(servicePtr->connected>0)
		return sendTcpData(cmd,servicePtr,TIME_OUT);  //60S超时
	else 
		return -1;
}

int startCommandCallback(tNET_SERVICE *servicePtr,char *cmd,ON_RECEIVE_ONE_PACKET onReceivePacket)
{
	if(servicePtr->connected>0){
		servicePtr->onOnePacket = onReceivePacket;
		return sendTcpData(cmd,servicePtr,TIME_OUT);  //60S超时
	}else 
		return -1;
}

int startCommandSync(tNET_SERVICE *servicePtr,char *cmd)
{
	return sendTcpDataSync(cmd,servicePtr,TIME_OUT);  //60S超时
}

static int CVICALLBACK TCPCallbackSync (unsigned int handle, int xType, 
	int errCode, void *callbackData)
{
	tNET_SERVICE *servicePtr = (tNET_SERVICE*)callbackData;
	switch (xType)
		{
		case TCP_DISCONNECT:
			
			//servicePtr->connected = 0;
			//DisconnectFromTCPServer(handle);
			if(servicePtr->onDisConnect!=NULL)
				(*(ON_NET_CONNECTED)(servicePtr->onDisConnect))(servicePtr);
			break;
		case TCP_DATAREADY:
			ProcessSystemEvents();
			break;
		}
		
	return 0;
}

void connectToStubSync(tNET_SERVICE *servicePtr)
{
	if(ConnectToTCPServer (&(servicePtr->connectHandler), servicePtr->port, servicePtr->ip, 
		NULL, servicePtr, 0)>=0){	
		servicePtr->connected=1;
		if(servicePtr->onConnect!=NULL)
			(*(ON_NET_CONNECTED)(servicePtr->onConnect))(servicePtr);
	}else{
		servicePtr->connected=-1;
	}
	return;	
}

void connectToStub(tNET_SERVICE *servicePtr)
{
	if(ConnectToTCPServer (&(servicePtr->connectHandler), servicePtr->port, servicePtr->ip, 
		TCPCallback, servicePtr, 500)>=0){	
		servicePtr->connected=1;
		if(servicePtr->onConnect!=NULL)
			(*(ON_NET_CONNECTED)(servicePtr->onConnect))(servicePtr);
	}else{
		servicePtr->connected=-1;
	}
	return;	
}


void disConnectFromStub(tNET_SERVICE *servicePtr)
{
	if(servicePtr->connected != -1)
	{
		DisconnectFromTCPServer(servicePtr->connectHandler);
		servicePtr->connected = -1;
	}
}

void onStubConnected(tNET_SERVICE *servicePtr)
{
	//printf("on----Connected----\n");
	if(startCommand(servicePtr,"pwd mcmc@2017\r\n")>=0)
	{
		//printf("%s\n",service.packetPtr);
	}else{
		return;
	}
	
	if(startCommand(servicePtr,"product\r\n")>=0)
	{
		//printf("%s\n",service.packetPtr);
	}else{
		return;
	}
}

void ReleaseStubNetService()
{
	if(gServicePtr!=NULL){
		free(gServicePtr);
		gServicePtr=NULL;
	}
}


void onStubDisConnected(tNET_SERVICE *servicePtr)
{
	//MessagePopup("warning","diconnect from server");
	if(servicePtr!=NULL)
		disConnectFromStub(servicePtr);
	ReleaseStubNetService();
}

void onStubTimeOut(tNET_SERVICE *servicePtr)
{
	//MessagePopup("warning","time out");
	onStubDisConnected(servicePtr);
}


tNET_SERVICE *getStubNetService(char *ip,int port)
{
	ProcessTCPEvents();
	if(gServicePtr==NULL)
	{
		 gServicePtr =(tNET_SERVICE*)malloc(sizeof(tNET_SERVICE));
		 memset(gServicePtr,0,sizeof(tNET_SERVICE));
		 sprintf(gServicePtr->ip,"%s",ip);
		 gServicePtr->port = port;
		 gServicePtr->onConnect = onStubConnected;
		 gServicePtr->onDisConnect = onStubDisConnected; 
		 gServicePtr->onTimeOut = onStubTimeOut; 
		 connectToStub(gServicePtr);
		 if(gServicePtr!=NULL){
		 	if(gServicePtr->connected==-1)
		 	{
		 		free(gServicePtr);
		 		gServicePtr=NULL;
		 	}
		 }
		 
	}else if(gServicePtr->connected==-1)
	{
		 memset(gServicePtr,0,sizeof(tNET_SERVICE));
		 sprintf(gServicePtr->ip,"%s",ip);
		 gServicePtr->port = port;
		 gServicePtr->onConnect = onStubConnected;
		 gServicePtr->onDisConnect = onStubDisConnected; 
		 gServicePtr->onTimeOut = onStubTimeOut; 
		 connectToStub(gServicePtr);
		 if(gServicePtr->connected==-1)
		 {
		 	free(gServicePtr);
		 	gServicePtr=NULL;
		 }		
	}
	
	if(gServicePtr==NULL)
	{
		if(TRUE==AlertDialogWithRet(0,"网络故障","重连网络？","不重连","重连"))
		{
			getStubNetService(ip,port);
		}
	}
	return gServicePtr;
}


