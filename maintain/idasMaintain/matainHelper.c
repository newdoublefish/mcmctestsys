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
#include <formatio.h>
#include <rs232.h>
#include "toolbox.h"
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "matainHelper.h"
#include "debug.h"
   
//#include "status.h"

//#define MAINTAIN_DEBUG
unsigned char matainStart[]={0xAA,0x55,0x00,0x01,0x00 ,0x00 ,0x00 ,0x76 ,0xB4 ,0xA5};
unsigned char matainStop[]={0xAA,0x55,0x00,0x01,0x00 ,0x01 ,0x00 ,0x45 ,0x85 ,0xA5};
unsigned char statusGet[]={0xAA,0x55,0x00,0x01,0x00 ,0x05 ,0x01 ,0x0A ,0xD3 ,0xDA ,0xA5}; 



IDAS_MAINT_MSG MaintainStartProtocol()
{
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x00;
	 msg.msgHead.msgLen=0x00;
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}


IDAS_MAINT_MSG MaintainStopProtocol()
{
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x01;
	 msg.msgHead.msgLen=0x00;
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}

IDAS_MAINT_MSG StatusGetProtocol()
{
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x05;
	 msg.msgHead.msgLen=0x01;
	 msg.msgBodyPtr=(unsigned char *)malloc(1);
	 unsigned char data=0x0a; 
	 memcpy((unsigned char *)msg.msgBodyPtr,&data,1);
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}

IDAS_MAINT_MSG ReadFileProtocol(char *FileName,BOOL isDir)
{
	 unsigned char temp[256]={0};
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x08;//文件获取命令
	 msg.msgHead.msgLen = strlen(FileName)+2;//两个字节中，一个是文件类型，一个是文件名长度
	 msg.msgBodyPtr=(unsigned char *)malloc(msg.msgHead.msgLen);
     temp[0]=0x04;//文件类型
	 temp[1]=strlen(FileName);//文件名长度
	 memcpy(temp+2,FileName,strlen(FileName));
	 memcpy((unsigned char *)msg.msgBodyPtr,temp,msg.msgHead.msgLen);
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}

IDAS_MAINT_MSG getFileDirProtocol(char *str,char *dst)
{
	 unsigned char temp[256]={0};
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x08;//文件获取命令
	 msg.msgHead.msgLen = 1+1+strlen(str)+1+strlen(dst);//两个字节中，一个是文件类型，一个是文件名长度
	 //PRINT("str:%s,len:%d",str,strlen(str));
	 //PRINT("dst:%s,len:%d",dst,strlen(dst));
	 msg.msgBodyPtr=(unsigned char *)malloc(msg.msgHead.msgLen);
     //temp[0]=msg.msgHead.msgLen-1;//总长度 
	 temp[0]=0x04;//类型
	 temp[1]=strlen(str);
	 memcpy(temp+2,str,strlen(str));
	 temp[strlen(str)+2]=strlen(dst);
	 memcpy(temp+strlen(str)+3,dst,strlen(dst));
	 memcpy((unsigned char *)msg.msgBodyPtr,temp,msg.msgHead.msgLen);
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}



Scene createStatusMonitorScene(void)
{
	Scene scene;
	IDAS_MAINT_MSG msg;
	memset(&scene,0,sizeof(Scene));
	scene.protocolList=ListCreate(sizeof(IDAS_MAINT_MSG));
	msg=MaintainStartProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=StatusGetProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=MaintainStopProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	return  scene;
}


IDAS_MAINT_MSG MonitorDataProtocol(unsigned char*data,unsigned int dataLen)
{
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x0B;
	 msg.msgHead.msgLen=dataLen;
	 msg.msgBodyPtr=(unsigned char *)malloc(dataLen+1);
	 memcpy((unsigned char *)msg.msgBodyPtr,data,dataLen);
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}

IDAS_MAINT_MSG MaintainSupervisionProtocol()		 //续传
{
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x03;
	 msg.msgHead.msgLen=0x00;
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}


Scene createDataMonitorScene(unsigned char*data,unsigned int dataLen)
{
	Scene scene;
	IDAS_MAINT_MSG msg;
	memset(&scene,0,sizeof(Scene));
	scene.protocolList=ListCreate(sizeof(IDAS_MAINT_MSG));
	msg=MaintainStartProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=MonitorDataProtocol(data,dataLen);
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=MaintainStopProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	return  scene;
}

Scene createFileReadScene(char *fileOrDirName,BOOL isDir)
{
	Scene scene;
	IDAS_MAINT_MSG msg;
	memset(&scene,0,sizeof(Scene));
	scene.protocolList=ListCreate(sizeof(IDAS_MAINT_MSG));
	msg=MaintainStartProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	//msg=MonitorDataProtocol(data,dataLen);
	msg=ReadFileProtocol(fileOrDirName,isDir);
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=MaintainStopProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	
	//sprintf(scene.saveFileName,"%s",saveFileName);
	return  scene;
}

IDAS_MAINT_MSG qauFileDownLoadProtocol(char *src,char *dst)
{
	 //unsigned char temp[256]={0};
	 //unsigned char srcString[128]={0};
	 //unsigned char dstString[128]={0};
	 //sprintf(srcString,"%s",src);
	 //sprintf(dstString,"%s",dst);
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x4;  //2个字节数据
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x13;//文件获取命令
	 msg.msgHead.msgLen = 256;//两个字节中，一个是文件类型，一个是文件名长度  //每
	 msg.msgBodyPtr=(unsigned char *)malloc(msg.msgHead.msgLen);
     /*temp[0]=0x04;//文件类型
	 temp[1]=strlen(FileName);//文件名长度
	 memcpy(temp+2,FileName,strlen(FileName));
	 memcpy((unsigned char *)msg.msgBodyPtr,temp,msg.msgHead.msgLen);*/
	 unsigned char *temp=(unsigned char *)msg.msgBodyPtr;
	 memset(temp,0,256);
	 memcpy(temp,src,strlen(src));
	 memcpy(temp+128,dst,strlen(dst));
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}


Scene createDirInfoGet(char *fileSrc,char *dstSrc)
{
	Scene scene;
	IDAS_MAINT_MSG msg;
	memset(&scene,0,sizeof(Scene));
	scene.protocolList=ListCreate(sizeof(IDAS_MAINT_MSG));
	msg=MaintainStartProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	//msg=MonitorDataProtocol(data,dataLen);
	msg=getFileDirProtocol(fileSrc,dstSrc);
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=MaintainStopProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	
	//sprintf(scene.saveFileName,"%s",saveFileName);
	return  scene;
}


Scene createQauFileDownLoad(char *fileSrc,char *dstSrc)
{
	Scene scene;
	IDAS_MAINT_MSG msg;
	memset(&scene,0,sizeof(Scene));
	scene.protocolList=ListCreate(sizeof(IDAS_MAINT_MSG));
	msg=MaintainStartProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	//msg=MonitorDataProtocol(data,dataLen);
	msg=qauFileDownLoadProtocol(fileSrc,dstSrc);
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=MaintainStopProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	
	//sprintf(scene.saveFileName,"%s",saveFileName);
	return  scene;
}

IDAS_MAINT_MSG checkDownLoadProtocol()
{
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x12;
	 msg.msgHead.msgLen=0x00;
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}

Scene createCheckDownLoad(void)
{
	Scene scene;
	IDAS_MAINT_MSG msg;
	memset(&scene,0,sizeof(Scene));
	scene.protocolList=ListCreate(sizeof(IDAS_MAINT_MSG));
	msg=MaintainStartProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	//msg=MonitorDataProtocol(data,dataLen);
	msg=checkDownLoadProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=MaintainStopProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	
	//sprintf(scene.saveFileName,"%s",saveFileName);
	return  scene;

}

IDAS_MAINT_MSG systemCheckProtocol(unsigned char cmd)    
{
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x05;
	 msg.msgHead.msgLen=0x01;
	 msg.msgBodyPtr=(unsigned char *)malloc(1);
	 unsigned char data=cmd; 
	 memcpy((unsigned char *)msg.msgBodyPtr,&data,1);
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}

Scene createSystemCheckScene(int cmd)
{
	Scene scene;
	IDAS_MAINT_MSG msg;
	memset(&scene,0,sizeof(Scene));
	scene.protocolList=ListCreate(sizeof(IDAS_MAINT_MSG));
	msg=MaintainStartProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	//msg=MonitorDataProtocol(data,dataLen);
	msg=systemCheckProtocol(cmd);
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=MaintainStopProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	
	//sprintf(scene.saveFileName,"%s",saveFileName);
	return  scene;    
}

IDAS_MAINT_MSG systemSetProtocol(unsigned char*data,unsigned int dataLen)    
{
     IDAS_MAINT_MSG msg;
     memset(&msg,0,sizeof(IDAS_MAINT_MSG));
     msg.msgHead.syscWordH=0xAA55;
	 msg.msgHead.addrOfSender=0x00;
	 msg.msgHead.addrOfRecv=0x01;
	 unsigned char type=0x00;
	 memcpy(&msg.msgHead.dataType,&type, 1);
	 msg.msgHead.cmd=0x04;
	 msg.msgHead.msgLen=dataLen;
	 //msg.msgBodyPtr=(unsigned char *)malloc(1);
	 //unsigned char data=cmd; 
	 //memcpy((unsigned char *)msg.msgBodyPtr,&data,1);
	 msg.msgBodyPtr=(unsigned char *)malloc(dataLen+1);
	 memcpy((unsigned char *)msg.msgBodyPtr,data,dataLen);
	 msg.msgTail.syscWordT=0xa5;
	 return msg;
}

Scene createSystemSetScene(unsigned char*data,unsigned int dataLen)
{
	Scene scene;
	IDAS_MAINT_MSG msg;
	memset(&scene,0,sizeof(Scene));
	scene.protocolList=ListCreate(sizeof(IDAS_MAINT_MSG));
	msg=MaintainStartProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	//msg=MonitorDataProtocol(data,dataLen);
	msg=systemSetProtocol(data,dataLen);
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	msg=MaintainStopProtocol();
	ListInsertItem(scene.protocolList,&msg,END_OF_LIST);
	
	//sprintf(scene.saveFileName,"%s",saveFileName);
	return  scene;    
}


void releaseScene(Scene *scenePtr)
{
	IDAS_MAINT_MSG msg; 
	for(int i=1;i<=ListNumItems(scenePtr->protocolList);i++)
	{
		ListGetItem(scenePtr->protocolList,&msg,i); 
		if(msg.msgHead.msgLen>0)
		{
		   free(msg.msgBodyPtr);
		}	
	}
	ListDispose(scenePtr->protocolList);
}

static void sendexTraProtocol(Scene *scenePtr,IDAS_COM_RX_TX_BUF_STRUCT *pRxBuf_,IDAS_MAINT_MSG msg)
{
     //ProtocolData pData;
	 //IDAS_MAINT_MSG msg;
	 //printf("sendProtocol scenePtr->index=%d\n",scenePtr->index);  
	 //ListGetItem(scenePtr->protocolList,&msg,scenePtr->index);
	 
	 BuildTxComPacket(&msg,pRxBuf_);
	 ComWrt(scenePtr->comPort,pRxBuf_->txDataBuf,pRxBuf_->txDataBufLen);
#ifdef MAINTAIN_DEBUG	 
	 printf("\n send protocolIndex:%d:",scenePtr->index);
	 for(int i=0;i<pRxBuf_->txDataBufLen;i++)
     {
	      printf("%x,",pRxBuf_->txDataBuf[i]);
	 }
#endif	 
	 //scenePtr->index++;
}

static void sendProtocol(Scene *scenePtr,IDAS_COM_RX_TX_BUF_STRUCT *pRxBuf_)
{
     //ProtocolData pData;
	 IDAS_MAINT_MSG msg;
	 //printf("sendProtocol scenePtr->index=%d\n",scenePtr->index);  
	 ListGetItem(scenePtr->protocolList,&msg,scenePtr->index);
	 
	 BuildTxComPacket(&msg,pRxBuf_);
	 ComWrt(scenePtr->comPort,pRxBuf_->txDataBuf,pRxBuf_->txDataBufLen);


#ifdef MAINTAIN_DEBUG	 
	 printf("\n send protocolIndex:%d:",scenePtr->index);
	 for(int i=0;i<pRxBuf_->txDataBufLen;i++)
     {
	      printf("%x,",pRxBuf_->txDataBuf[i]);
	 }
#endif	 
	 //scenePtr->index++;
}


BOOL ParseResponseMsg(Scene *scenePtr,IDAS_MAINT_MSG RspMsg)
{
	BOOL ret=TRUE;
	if(RspMsg.msgHead.cmd==0x5 || RspMsg.msgHead.cmd==0x0b || RspMsg.msgHead.cmd==0x12)
	{
	    scenePtr->resultLen=RspMsg.msgHead.msgLen;
		memcpy(scenePtr->resultBuffer,(unsigned char*)RspMsg.msgBodyPtr,RspMsg.msgHead.msgLen);
	}
	if(RspMsg.msgHead.cmd==0x08)
	{
	     if(scenePtr->saveFileDsctiption!=0)
		 {
		      //char temp[MAX_PATHNAME_LEN]={0};
			 unsigned char *data=(unsigned char *)RspMsg.msgBodyPtr;
			 int fileLen= *(data+1) & 0xff | (*(data+2)<<8) & 0xff00  | (*(data+3)<<16) & 0xff0000 | (*(data+4)<<24) & 0xff000000;
             int dataLen= RspMsg.msgHead.msgLen-5; 
			 scenePtr->recvFileLen=fileLen;
			 scenePtr->recvDataCount+=dataLen;
#ifdef MAINTAIN_DEBUG			 
             printf("fileLen=%d,dataLen=%d\n",fileLen,dataLen);
#endif			 
			 WriteFile (scenePtr->saveFileDsctiption, data+5,dataLen);
			 if(scenePtr->recvDataCount<scenePtr->recvFileLen)
			 {
			     return FALSE;
			 }	 
           
		 }
	}
	if(RspMsg.msgHead.cmd==0x03)
	{
	      //scenePtr->recvDataCount+=dataLen; 
	     if(scenePtr->saveFileDsctiption!=0)
		 {
			 //int fileLen= *(data+1) & 0xff | (*(data+2)<<8) & 0xff00  | (*(data+3)<<16) & 0xff0000 | (*(data+4)<<24) & 0xff000000; 
			 scenePtr->recvDataCount+= RspMsg.msgHead.msgLen;
			 
		     WriteFile (scenePtr->saveFileDsctiption, (unsigned char*)RspMsg.msgBodyPtr,RspMsg.msgHead.msgLen);
			 if(scenePtr->recvDataCount<scenePtr->recvFileLen)
			 {
			     return FALSE;
			 } 
		 }
	}	
	return ret;

}


static BOOL ProcessScene(IDAS_COM_RX_TX_BUF_STRUCT *pRxBuf_,Scene *scenePtr,IDAS_MAINT_MSG RspMsg)
{
	//Scene *scenePtr=(Scene *)(pRxBuf_->fatherPtr); 
	IDAS_MAINT_MSG reqMsg;
	ListGetItem(scenePtr->protocolList,&reqMsg,scenePtr->index);

	if(RspMsg.msgHead.cmd==reqMsg.msgHead.cmd || RspMsg.msgHead.cmd==0x03)   //如果询问和响应的命令一直，则发送下一个协议，否则重置协议
	{	
	   if(ParseResponseMsg(scenePtr,RspMsg)==TRUE) //处理下位机回复  
	   {	   
	      scenePtr->index++;  //正常处理
	   }else{//续传
		   IDAS_MAINT_MSG msg= MaintainSupervisionProtocol();
		   
		   sendexTraProtocol(scenePtr,pRxBuf_,msg);//发送续传指令
	   	   return TRUE;
	   }
	   
	   if(RspMsg.msgHead.cmd==0x01)
	   {
	      scenePtr->quitFlag=1; 
	   }	   
		   
	}else 
	{
	    scenePtr->index=1;
		scenePtr->retryCount++;
	}	
	
	
	
    if(scenePtr->index >=ListNumItems(scenePtr->protocolList))   //这个地方需要注意，如果不用模拟器，而是现实环境，应该把大于等于号换成大于号
	{
	    scenePtr->quitFlag=1;
	}
	pRxBuf_->txDataReadyFlag=1;
	if(scenePtr->retryCount>=3)
	{
	    scenePtr->quitFlag=1; 
	}	
	return TRUE;
}



BOOL COMRxDataReadyNotification(IDAS_COM_RX_TX_BUF_STRUCT *pRxBuf_)
{
	unsigned char	contOffset = 0;
	IDAS_MAINT_MSG   RspMsg; 
	Scene *scenePtr=(Scene *)(pRxBuf_->fatherPtr);

    if( pRxBuf_->comType != RS422 )
    {
        printf("comType is not valid!\n",0,0,0,0,0,0);
        return FALSE;
    }
    
    if( !ParseRXComPacket(pRxBuf_, &RspMsg) )
    {
        printf("Parse RX Com Packet Error!\n",0,0,0,0,0,0);
        return FALSE;
    }else
	{
		
		//ParseResponseMsg(scenePtr,RspMsg);
		ProcessScene(pRxBuf_,scenePtr,RspMsg);
	}
	
	if(pRxBuf_->txDataReadyFlag)
	{
		sendProtocol(scenePtr,pRxBuf_); 
		pRxBuf_->txDataReadyFlag=0;
	}	
    
  //printf("Received Msg: ");
     
    return TRUE;
}



BOOL runSceneMatainProcess(Scene *scenePtr)
{
	HRESULT ret=TRUE;
	unsigned char buf[1024]={0}; 
	int j;
	int len=0;
	memset(&(scenePtr->gRxTxBufTest),0,sizeof(IDAS_COM_RX_TX_BUF_STRUCT));
	scenePtr->gRxTxBufTest.fatherPtr=scenePtr;
	scenePtr->index=1;//链表是从1开始的
	scenePtr->retryCount=0;
	IdasComRxTxInit(&(scenePtr->gRxTxBufTest),0xff, (void *)COMRxDataReadyNotification); 
	
	sendProtocol(scenePtr,&(scenePtr->gRxTxBufTest));
	
	SetComTime(scenePtr->comPort,0.2);
	
	while(scenePtr->quitFlag!=1)
	{
	    len=ComRd(scenePtr->comPort,buf,1024);
	    if(len>0)
		{
#ifdef MAINTAIN_DEBUG			
		   printf("\n recv protocolIndex:%d:",scenePtr->index);	
           for(j = 0; j < len; j++)
           {
			  printf("%x,",buf[j]);    
              IdasComRxEventHandler(&(scenePtr->gRxTxBufTest), buf[j]);
			  scenePtr->noDateTimeOut=0;
           }
#else
           for(j = 0; j < len; j++)
           {
              IdasComRxEventHandler(&(scenePtr->gRxTxBufTest), buf[j]);
			  scenePtr->noDateTimeOut=0;
           }		   
#endif		   
		}else
		{
		   scenePtr->noDateTimeOut+=1;
		}	
	    
		if(scenePtr->noDateTimeOut>=10)
		{
		   if(scenePtr->retryCount>3)
		   { 	   
		      break;	//这个地方是直接跳出，还是要进行重发?
		   }else{
			  scenePtr->noDateTimeOut=0;
			  scenePtr->retryCount++;
			  sendProtocol(scenePtr,&(scenePtr->gRxTxBufTest));
		   }	  
		}	
	   
	}
	
	IdasComRxTxDeInit(&(scenePtr->gRxTxBufTest));
	
	if(scenePtr->retryCount>=3)
	{
	   ret=FALSE;
	}
	
	if(scenePtr->index<ListNumItems(scenePtr->protocolList))
	{
	   ret=FALSE;
	}
	return ret;
}


void TestProtocol(void)
{
	IDAS_MAINT_MSG msg=getFileDirProtocol("/s0p0:1/vd/","host:D:\\TData\\downTemp\\FileListInfo.txt");
	IDAS_COM_RX_TX_BUF_STRUCT pRxBuf;
		memset(&pRxBuf,0,sizeof(IDAS_COM_RX_TX_BUF_STRUCT));    
	IdasComRxTxInit(&pRxBuf,0xff, (void *)COMRxDataReadyNotification); 

    BuildTxComPacket(&msg,&pRxBuf);
	for(int i=0;i<pRxBuf.txDataBufLen;i++)
    {
	   if(i%16==0)
		   printf("\n");
	   printf("%x,",pRxBuf.txDataBuf[i]);
	}	
	
}




