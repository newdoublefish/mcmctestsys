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

typedef struct{
	char name[50];
	char type[10];
	double volt;
	double upper;
	double lower;//小于1E4视为0
	double rtim;
	double ttim;
	double ftim;
	double arc;
	double freq;
	double wtim;
	double ramp;
	double range;
}tSCPICMD;

int scpiDispPage(int port,enumPageType pageType);
int scpiSetType(int port,enumTType tType); 
int scpiSetVolt(int port,float val);
float scpiGetVolt(int port);
int scpiSetTestTime(int port,float val);
int scpiGetResult(int port);
int scpiStartTest(int port);
int scpiStopTest(int port);
int scpiSendCmd(int port,tSCPICMD cmd);
void printfScpiCmd(tSCPICMD cmd);
float readResistent(char *resultStr);
float readElectricCurrent(char *resultStr);
#ifdef __cplusplus
   }
#endif

#endif
