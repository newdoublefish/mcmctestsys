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
	
	printfParam(param);
	if(GetParameter(servicePtr,&param)<0)
	{	
		sprintf(temp,"%s:%s",paramName,"��ȡʧ��");
		WarnShow1(0,temp);
		return FALSE;
	}
	sprintf(paramValue,"%s",param.value);
	return TRUE;
}

