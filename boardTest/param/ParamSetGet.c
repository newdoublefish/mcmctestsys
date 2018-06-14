/*---------------------------------------------------------------------------
 // 版权声明：本文件由广东万城万充电动车运营股份有限公司版权所有，未经授权，
 // 禁止第三方进行拷贝和使用。
 //
 // 文件名：
 // 文件功能描述: 
 //
 // 
 // 创建标识：曾衍仁 
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
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
		
		sprintf(temp,"%s:%s",paramName,"无该参数配置");
		LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
		WarnShow1(0,temp);
		ret = FALSE;
	}
	sprintf(param.value,"%s",paramValue); 
	if(ConfigParameter(servicePtr,param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"设置失败");
		LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
		WarnShow1(0,temp);
		onStubDisConnected(servicePtr);
		ret = FALSE;
	}
	
	
	return TRUE;
}

BOOL ParamSetDependWithRetry(EUT eut,char *paramName,char *paramValue,int retryCnt)
{

	BOOL ret=TRUE;
	do{
		PARAMETER param={0};
		char temp[30]={0};
		tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
		if(servicePtr==NULL)
		{
			return 	FALSE;
		}	
		if(FALSE==getParameter(paramName,&param))
		{
		
			sprintf(temp,"%s:%s",paramName,"无该参数配置");
			LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
			WarnShow1(0,temp);
			ret = FALSE;
			break;
		}
		sprintf(param.value,"%s",paramValue); 
		if(ConfigParameter(servicePtr,param)<0)
		{	
			sprintf(temp,"%s:%s",paramName,"设置失败");
			LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
			WarnShow1(0,temp);
			onStubDisConnected(servicePtr);
			ret = FALSE;
		}else{
			ret=TRUE;
			break;
		}
	}while(retryCnt--);	
	
	
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
		sprintf(temp,"%s:%s",paramName,"无该参数配置");
		LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
		WarnShow1(0,temp);
		ret=FALSE;
	}
	
	//printfParam(param);
	if(GetParameter(servicePtr,&param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"获取失败");
		LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
		WarnShow1(0,temp);
		onStubDisConnected(servicePtr);
		//ReleaseStubNetService();		
		ret=FALSE;
	}
	sprintf(paramValue,"%s",param.value);

	return ret;
}

BOOL ParamGetDependWithRetry(EUT eut,char *paramName,char *paramValue,int retryCnt)
{
	BOOL ret=TRUE; 

	do{
		PARAMETER param={0};
		char temp[30]={0};
		tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
		if(servicePtr==NULL)
		{
			return 	FALSE;
		}		
		if(FALSE==getParameter(paramName,&param))
		{
			sprintf(temp,"%s:%s",paramName,"无该参数配置");
			LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
			WarnShow1(0,temp);
			
			ret=FALSE;
			break; 
		}
	
		//printfParam(param);
		if(GetParameter(servicePtr,&param)<0)
		{	
			sprintf(temp,"%s:%s",paramName,"获取失败");
			LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
			//WarnShow1(0,temp);
			onStubDisConnected(servicePtr);
			//ReleaseStubNetService();		
			ret=FALSE;
		}else{
			sprintf(paramValue,"%s",param.value);
			ret=TRUE;
			break;
		}
		
	}while(retryCnt--);

	return ret;
}


BOOL ParamSet(tNET_SERVICE *servicePtr,char *paramName,char *paramValue)
{
	PARAMETER param={0};
	char temp[30]={0};
	if(FALSE==getParameter(paramName,&param))
	{
		
		sprintf(temp,"%s:%s",paramName,"无该参数配置");
		WarnShow1(0,temp);
		return FALSE;
	}
	sprintf(param.value,"%s",paramValue); 
	if(ConfigParameter(servicePtr,param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"设置失败");
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
		sprintf(temp,"%s:%s",paramName,"无该参数配置");
		WarnShow1(0,temp);
		return FALSE;
	}
	
	//printfParam(param);
	if(GetParameter(servicePtr,&param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"获取失败");
		WarnShow1(0,temp);
		return FALSE;
	}
	sprintf(paramValue,"%s",param.value);
	return TRUE;
}


BOOL CommandSend(EUT eut,char *command)
{
	BOOL ret=TRUE; 
	PARAMETER param={0};
	char temp[30]={0};
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	FALSE;
	}		
								  
	if(startCommand(servicePtr,"product self_test\r\n")<0)
	{
		onStubDisConnected(servicePtr);
		WarnShow1(0,"命令发送失败!");
		return FALSE;
	}
				
	
	return ret;	
}


BOOL CommandSendWithRetry(EUT eut,char *command,int retryCnt)
{
	BOOL ret=TRUE; 
	do
	{
		tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	    if(servicePtr==NULL)
	    {
		     return FALSE;
	    }	
		if(startCommand(servicePtr,command)<0)
		{
			onStubDisConnected(servicePtr);
			//WarnShow1(0,"命令发送失败!");
			ret=FALSE;
		}else{
			ret=TRUE;
			break;
		}
	}while(retryCnt--);
	return ret;	
}


