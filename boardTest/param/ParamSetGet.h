#ifndef __PARAM_SETGET_H__
#define __PARAM_SETGET_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "StubNetService.h" 
BOOL ParamSet(tNET_SERVICE *servicePtr,char *paramName,char *paramValue);
BOOL ParamGet(tNET_SERVICE *servicePtr,char *paramName,char *paramValue); 
#ifdef __cplusplus
    }
#endif

#endif 
