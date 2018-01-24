#ifndef __PARAM_SETGET_H__
#define __PARAM_SETGET_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "StubNetService.h" 
#include "eutEntity.h"
BOOL ParamSet(tNET_SERVICE *servicePtr,char *paramName,char *paramValue);
BOOL ParamGet(tNET_SERVICE *servicePtr,char *paramName,char *paramValue); 
BOOL ParamGetDepend(EUT eut,char *paramName,char *paramValue);
BOOL ParamGetDependWithRetry(EUT eut,char *paramName,char *paramValue,int retryCnt);
BOOL ParamSetDependWithRetry(EUT eut,char *paramName,char *paramValue,int retryCnt);
BOOL ParamSetDepend(EUT eut,char *paramName,char *paramValue);
BOOL CommandSend(EUT eut,char *command);
#ifdef __cplusplus
    }
#endif

#endif 
