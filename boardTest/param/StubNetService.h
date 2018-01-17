//==============================================================================
//
// Title:		NetService.h
// Purpose:		A short description of the interface.
//
// Created on:	2018/1/2 at 16:30:23 by .
// Copyright:	. All Rights Reserved.
//
//==============================================================================

#ifndef __NetService_H__
#define __NetService_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
		
		
// Global functions
		
#define MAX_NET_RECV_BUFFER 4096
#define TIME_OUT 15
#define TCP_CLIENT_SYNC
typedef struct{
	char ip[20];
	int port;
	unsigned int connectHandler;
	int connected;
	int reConnectCnt;
	char *packetPtr;
	char buffer[MAX_NET_RECV_BUFFER];
	char packet[MAX_NET_RECV_BUFFER];
	int setFlag;
	int dataCnt;
	int quitFlag;
	void *onConnect;
	void *onDisConnect;
	void *onOnePacket;
	void *onTimeOut;
}tNET_SERVICE;

typedef void (*ON_NET_CONNECTED)(tNET_SERVICE *servicePtr);
typedef void (*ON_NET_DISCONNECTED)(tNET_SERVICE *servicePtr);
typedef void (*ON_NET_TIMEOUT)(tNET_SERVICE *servicePtr); 
typedef void (*ON_RECEIVE_ONE_PACKET)(tNET_SERVICE *servicePtr); 

void connectToStub(tNET_SERVICE *servicePtr);


int startCommand(tNET_SERVICE *servicePtr,char *cmd);  

void onStubDisConnected(tNET_SERVICE *servicePtr);

void ReleaseStubNetService();


void connectToStubSync(tNET_SERVICE *servicePtr);
int startCommandSync(tNET_SERVICE *servicePtr,char *cmd);   //这个去掉，再内部用宏来定义用同步还是异步，加入了processSystemEvent来处理系统事物，可能卡顿会有所好转

tNET_SERVICE *getStubNetService(char *ip,int port);




#ifdef __cplusplus
    }
#endif

#endif  /* ndef __NetService_H__ */
