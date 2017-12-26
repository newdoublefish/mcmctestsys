#include <rs232.h>
#include <formatio.h>
#include "scpiHelper.h"
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
