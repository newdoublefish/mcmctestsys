#include <formatio.h>
#include "toolbox.h"
#include "ParamSetGet.h"
#include "common.h"
#include "ParamExcelParse.h"
#include "EutEntity.h"
#include "Log.h"

BOOL ParamSetDepend(EUT eut,char *paramName,char *paramValue)
{
	PARAMETER param={0};
	char temp[30]={0};
	BOOL ret=TRUE;
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	FALSE;
	}	
	if(FALSE==getParameter(paramName,&param))
	{
		
		sprintf(temp,"%s:%s",paramName,"�޸ò�������");
		LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
		WarnShow1(0,temp);
		ret = FALSE;
	}
	sprintf(param.value,"%s",paramValue); 
	if(ConfigParameter(servicePtr,param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"����ʧ��");
		LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
		WarnShow1(0,temp);
		onStubDisConnected(servicePtr);
		ret = FALSE;
	}
	
	
	return TRUE;
}
BOOL ParamGetDepend(EUT eut,char *paramName,char *paramValue)
{
	BOOL ret=TRUE; 
	PARAMETER param={0};
	char temp[30]={0};
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	FALSE;
	}		
	if(FALSE==getParameter(paramName,&param))
	{
		sprintf(temp,"%s:%s",paramName,"�޸ò�������");
		LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
		WarnShow1(0,temp);
		ret=FALSE;
	}
	
	//printfParam(param);
	if(GetParameter(servicePtr,&param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"��ȡʧ��");
		LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
		WarnShow1(0,temp);
		onStubDisConnected(servicePtr);
		ReleaseStubNetService();		
		ret=FALSE;
	}
	sprintf(paramValue,"%s",param.value);

	return ret;
}


BOOL ParamSet(tNET_SERVICE *servicePtr,char *paramName,char *paramValue)
{
	PARAMETER param={0};
	char temp[30]={0};
	if(FALSE==getParameter(paramName,&param))
	{
		
		sprintf(temp,"%s:%s",paramName,"�޸ò�������");
		WarnShow1(0,temp);
		return FALSE;
	}
	sprintf(param.value,"%s",paramValue); 
	if(ConfigParameter(servicePtr,param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"����ʧ��");
		WarnShow1(0,temp);
		return FALSE;
	}
	return TRUE;
}
BOOL ParamGet(tNET_SERVICE *servicePtr,char *paramName,char *paramValue)
{
	PARAMETER param={0};
	char temp[30]={0};
	if(FALSE==getParameter(paramName,&param))
	{
		sprintf(temp,"%s:%s",paramName,"�޸ò�������");
		WarnShow1(0,temp);
		return FALSE;
	}
	
	//printfParam(param);
	if(GetParameter(servicePtr,&param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"��ȡʧ��");
		WarnShow1(0,temp);
		return FALSE;
	}
	sprintf(paramValue,"%s",param.value);
	return TRUE;
}
