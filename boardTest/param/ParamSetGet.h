#ifndef __PARAM_SETGET_H__
#define __PARAM_SETGET_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "StubNetService.h" 
#include "eutEntity.h"
typedef struct{
	int gunIndex; 
	char errorTime[20];
	int errorid;
	char errorString[50];
}tErrorMesg;
		
BOOL ParamSet(tNET_SERVICE *servicePtr,char *paramName,char *paramValue);
BOOL ParamGet(tNET_SERVICE *servicePtr,char *paramName,char *paramValue); 
BOOL ParamGetDepend(EUT eut,char *paramName,char *paramValue);
BOOL ParamGetDependWithRetry(EUT eut,char *paramName,char *paramValue,int retryCnt);
BOOL ParamSetDependWithRetry(EUT eut,char *paramName,char *paramValue,int retryCnt);
BOOL ParamSetDepend(EUT eut,char *paramName,char *paramValue);
BOOL CommandSend(EUT eut,char *command);
BOOL CommandSendWithRetry(EUT eut,char *command,int retryCnt);
BOOL CommandGetResultWithRetry(EUT eut,char *command,int retryCnt,char **resultBuffer);
BOOL GetPileErrorList(EUT eut,char *command,int retryCnt,ListType list);
#ifdef __cplusplus
    }
#endif

#endif 
