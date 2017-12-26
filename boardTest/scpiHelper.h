#ifndef __SCPIHELPER_H__
#define __SCPIHELPER_H__
#include "userint.h"
#ifdef __cplusplus
   extern "C" {
#endif
typedef enum
{
	MEASUREMENT,
	MSETUP,
	SYSTEM,
	SYSTEMINFO,
	CATALOG
}enumPageType;


typedef enum
{
	ACW,
	DCW,
	IR
}enumTType;
int scpiDispPage(int port,enumPageType pageType);
int scpiSetType(int port,enumTType tType); 
int scpiSetVolt(int port,float val);
float scpiGetVolt(int port);
int scpiSetTestTime(int port,float val);
int scpiGetResult(int port);
int scpiStartTest(int port);
int scpiStopTest(int port);
#ifdef __cplusplus
   }
#endif

#endif
