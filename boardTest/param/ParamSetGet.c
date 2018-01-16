#include <formatio.h>
#include "toolbox.h"
#include "ParamSetGet.h"
#include "common.h"
#include "ParamExcelParse.h"


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
	
	printfParam(param);
	if(GetParameter(servicePtr,&param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"获取失败");
		WarnShow1(0,temp);
		return FALSE;
	}
	sprintf(paramValue,"%s",param.value);
	return TRUE;
}

