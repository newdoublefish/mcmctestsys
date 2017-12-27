#include <utility.h>
#include <ansi_c.h>
#include <rs232.h>
#include <formatio.h>
#include "scpiHelper.h"
#include "regexpr.h"  
int scpiDispPage(int port,enumPageType pageType){
	char buffer[100]={0};
	char *page="mset";
	if(MEASUREMENT==pageType)
	{
		page="meas";	
	}else if(MSETUP==pageType)
	{
		page="mset";
	}else if(SYSTEM==pageType)
	{
		page="syst";
	}else if(SYSTEMINFO==pageType)
	{
		page="sinf";
	}else if(CATALOG==pageType)
	{
		page="cata";
	}
	Fmt(buffer,"disp:page %s\n",page);
	return ComWrt (port, buffer, StringLength(buffer)); 
}


int scpiSetType(int port,enumTType tType){
	char buffer[100]={0};
	char *type="acw";
	if(MEASUREMENT==tType)
	{
		type="acw";	
	}else if(MSETUP==tType)
	{
		type="dcw";
	}else if(SYSTEM==tType)
	{
		type="ir";
	}
	Fmt(buffer,"func:sour:step1:type %s\n",type);
	return ComWrt (port, buffer, StringLength(buffer)); 
}

int scpiSetVolt(int port,float val)
{
	char buffer[100]={0};
	Fmt(buffer,"func:sour:step1:volt %f\n",val); 
	return ComWrt (port, buffer, StringLength(buffer)); 
}

int scpiSetTestTime(int port,float val)
{
	char buffer[100]={0};
	Fmt(buffer,"func:sour:step1:ttim %f\n",val);
	return ComWrt (port, buffer, StringLength(buffer)); 
}

int scpiGetResult(int port)
{
	char buffer[100]={0};
	Fmt(buffer,"fetch:auto on\n");
	return ComWrt (port, buffer, StringLength(buffer)); 
}


float scpiGetVolt(int port){
    char buffer[100]={0};
	Fmt(buffer,"func:sour:step1:volt?\n");
	return ComWrt (port, buffer, StringLength(buffer)); 
}

int scpiStartTest(int port){
    char buffer[100]={0};
	Fmt(buffer,"func:start\n");
	return ComWrt (port, buffer, StringLength(buffer)); 
}

int scpiStopTest(int port){
    char buffer[100]={0};
	Fmt(buffer,"func:start\n");
	return ComWrt (port, buffer, StringLength(buffer)); 
}

void printfScpiCmd(tSCPICMD cmd)
{
	printf("%s,%s,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",cmd.name,cmd.type,cmd.volt,cmd.upper,cmd.lower,cmd.rtim,cmd.ttim,cmd.ftim,cmd.arc,cmd.freq,cmd.wtim,cmd.ramp,cmd.range);
}

float readResistent(char *resultStr)
{
	 int matched,position,matchedLen;
	 RegExpr_FindPatternInText("[0-9]*[.][0-9]*{M¦¸}|{G¦¸}",0,resultStr,strlen(resultStr),1,1,&matched,&position,&matchedLen);
	 if(matched)
	 {	
		char temp[20]={0};
	 	memcpy(temp,resultStr+position,matchedLen-2);
	 	return atof(temp);
	 }
	return 0;
}

float readElectricCurrent(char *resultStr)
{
	 int matched,position,matchedLen;
	 RegExpr_FindPatternInText("[0-9]*[.][0-9]*{mA}",0,resultStr,strlen(resultStr),1,1,&matched,&position,&matchedLen);
	 if(matched)
	 {	
		char temp[20]={0};
	 	memcpy(temp,resultStr+position,matchedLen-2);
	 	return atof(temp);
	 }
	return 0;
}

int scpiSendCmd(int port,tSCPICMD cmd)
{
#if 0	
	char buffer[1024]={0};
	char temp[128]={0};
	//TYPE
	Fmt(temp,"func:sour:step1:type %s;",cmd.type);
	strcat(buffer,temp);
	//VOLT
	memset(temp,0,128);
	Fmt(temp,"func:sour:step1:volt %f;",cmd.volt);
	strcat(buffer,temp);
	//UPPER
	
	memset(temp,0,128);
	Fmt(temp,"func:sour:step1:upper %f;",cmd.upper);
	strcat(buffer,temp);
	//UPPER
	memset(temp,0,128);
	Fmt(temp,"func:sour:step1:lower %f;",cmd.lower);
	strcat(buffer,temp);

	//ADD NL
	strcat(buffer,"\n");
	
	printf("%s",buffer);
	 	
	return ComWrt (port, buffer, StringLength(buffer));
#else
	if(strcmp("IR",cmd.type)==0)
		scpiSetType(port,IR);
	else if(strcmp("ACW",cmd.type)==0)
		scpiSetType(port,ACW);
	Delay(0.2);
	scpiSetVolt(port,cmd.volt);
	Delay(0.2);
	scpiSetTestTime(port,cmd.ttim);
	Delay(0.2);
	return 0;
#endif	
}
