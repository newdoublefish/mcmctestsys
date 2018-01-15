#ifndef __RULE_PARSE_H__
#define __RULE_PARSE_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "StubNetService.h"
typedef struct{
	char paramName[50];
	int gunIndex;
	int pool;
	int group;
	int item;
	int array;
	int element;
	char key[128];
	char value[128];
}PARAMETER;
int ConfigParameter(tNET_SERVICE *servicePtr,PARAMETER para);
int GetParameter(tNET_SERVICE *servicePtr,PARAMETER *para);
int GetParameterWithRetry(tNET_SERVICE *servicePtr,PARAMETER *para);
int ConfigParameterWithRetry(tNET_SERVICE *servicePtr,PARAMETER para);
void printfParam(PARAMETER para);


#ifdef __cplusplus
    }
#endif

#endif 
