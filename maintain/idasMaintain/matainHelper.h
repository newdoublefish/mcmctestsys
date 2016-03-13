#ifndef __PROTOCOLHELPER_H__
#define __PROTOCOLHELPER_H__
#include "idasMaintainUARTInterface.h" 
#include "toolbox.h"
#include <ansi_c.h>
#include <cvirte.h>		

#ifdef __cplusplus
   extern "C" {
#endif

/*typedef struct
{
   int len;
   unsigned char *data;
}ProtocolData;*/
#define RESULT_BUFFER_LEN 4096	  
	   
#define TMP_DATA_PATH "\\tmpData\\"  
typedef struct
{
   int index; //�ڼ���protocol
   int comPort;//����
   int quitFlag;
   IDAS_COM_RX_TX_BUF_STRUCT gRxTxBufTest; //�����շ�����
   unsigned char resultBuffer[RESULT_BUFFER_LEN];
   unsigned int resultLen;
   int retryCount;
   ListType protocolList;
   int noDateTimeOut;
   int saveFileDsctiption;
   int recvFileLen;
   int recvDataCount;//���յ�����Ч�����ܳ���
}Scene;

Scene createDataMonitorScene(unsigned char*data,unsigned int dataLen);

Scene createStatusMonitorScene(void);

Scene createFileReadScene(char *fileOrDirName,BOOL isDir);

Scene createQauFileDownLoad(char *fileSrc,char *dstSrc) ;

Scene createCheckDownLoad(void);

Scene createDirInfoGet(char *fileSrc,char *dstSrc);

Scene createSystemCheckScene(int cmd);

Scene createSystemSetScene(unsigned char*data,unsigned int dataLen);

void releaseScene(Scene *scenePtr);

BOOL runSceneMatainProcess(Scene *scinePtr);

void TestProtocol(void);
#ifdef __cplusplus																																															  
   }
#endif

#endif

