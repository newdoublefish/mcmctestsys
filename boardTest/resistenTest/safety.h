#ifndef __SAFETY_H__
#define __SAFETY_H__
#ifdef __cplusplus
   extern "C" {
#endif
#include "scpiHelper.h"	   
typedef struct{
	int len;
	unsigned char buffer[100];
}tSafetyBuffer;
BOOL JinkoReset(int comPort);
BOOL SetJinkoChannel(int comPort,int channel);
BOOL SetJinkoParam(int comPort,tSCPICMD cmd);
BOOL StartJinkoTest(int comPort);
BOOL parseJinkoResult(char *resultBuffer,char *result);

#ifdef __cplusplus
   }
#endif


#endif
