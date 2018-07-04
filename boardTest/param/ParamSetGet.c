/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ㶫������綯����Ӫ�ɷ����޹�˾��Ȩ���У�δ����Ȩ��
 // ��ֹ���������п�����ʹ�á�
 //
 // �ļ�����
 // �ļ���������: 
 //
 // 
 // ������ʶ�������� 
 //
 // �޸ı�ʶ��
 // �޸�������
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
		
			sprintf(temp,"%s:%s",paramName,"�޸ò�������");
			LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
			WarnShow1(0,temp);
			ret = FALSE;
			break;
		}
		sprintf(param.value,"%s",paramValue); 
		if(ConfigParameter(servicePtr,param)<0)
		{	
			sprintf(temp,"%s:%s",paramName,"����ʧ��");
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
			sprintf(temp,"%s:%s",paramName,"�޸ò�������");
			LOG_EVENT_FORMAT(LOG_INFO,"%s",temp); 
			WarnShow1(0,temp);
			
			ret=FALSE;
			break; 
		}
	
		//printfParam(param);
		if(GetParameter(servicePtr,&param)<0)
		{	
			sprintf(temp,"%s:%s",paramName,"��ȡʧ��");
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
		WarnShow1(0,"�����ʧ��!");
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
			//WarnShow1(0,"�����ʧ��!");
			ret=FALSE;
		}else{
			//printf("%d\n",strlen(servicePtr->packet));
			ret=TRUE;
			break;
		}
		//
	}while(retryCnt--);
	return ret;	
}

BOOL CommandGetResultWithRetry(EUT eut,char *command,int retryCnt,char **resultBuffer)
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
			//WarnShow1(0,"�����ʧ��!");
			ret=FALSE;
		}else{
			//printf("%d\n",strlen(servicePtr->packet));
			if(*resultBuffer!=NULL)
			{
				*resultBuffer = servicePtr->packet;	
			}
			ret=TRUE;
			break;
		}
		//
	}while(retryCnt--);
	return ret;	
}


void ParsePileErrorBuffer(char *buffer,ListType list)
{
	char	*tok;
	tErrorMesg msg={0};
	for (tok = strtok(buffer, "\n");tok;tok = strtok(NULL, "\n"))   //һ��һ�ж�
	{
		if(strstr(tok,"��ʼʱ��")!=NULL)
		{
			char temp1[20]={0},temp2[20]={0},temp3[20]={0};
			sscanf(tok,"%s %s %s",temp1,temp2,temp3);
			sprintf(msg.errorTime,"%s %s",temp2,temp3);
		}else if(strstr(tok,"����ID")!=NULL)
		{
			char temp1[20]={0};
			sscanf(tok,"%s %d %s",temp1,&msg.errorid,msg.errorString);
		}else if(strstr(tok,"ǹ���")!=NULL)
		{
			char temp1[20]={0};
			sscanf(tok,"%s %d",temp1,&msg.gunIndex);
			if(list!=0)
			{
				ListInsertItem(list,&msg,END_OF_LIST);
			}
		}
	}	

}

BOOL GetPileErrorList(EUT eut,char *command,int retryCnt,ListType list)
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
			//WarnShow1(0,"�����ʧ��!");
			ret=FALSE;
		}else{
			//printf("%s\n",servicePtr->packet);   
			ParsePileErrorBuffer(servicePtr->packet,list);
			
			ret=TRUE;
			break;
		}
		//
	}while(retryCnt--);
	return ret;	
}


void ParsePileRecordBuffer(char *buffer,ListType list)
{
	char	*tok;
	tRecordMesg msg={0};
	for (tok = strtok(buffer, "\n");tok;tok = strtok(NULL, "\n"))   //һ��һ�ж�
	{
		if(strstr(tok,"ǹ��")!=NULL)
		{
			char temp1[20]={0},temp2[20]={0};
			memset(&msg,0,sizeof(tRecordMesg));
			sscanf(tok,"%s %s",temp1,temp2);
			msg.gunIndex = atoi(temp2);
		}else if(strstr(tok,"������ԭ��1")!=NULL)
		{
			char temp1[20]={0},temp2[20]={0};
			sscanf(tok,"%s %s",temp1,temp2);
			msg.reseason1 = atoi(temp2); 
		}else if(strstr(tok,"������ԭ��2")!=NULL)
		{
			char temp1[20]={0},temp2[20]={0};
			sscanf(tok,"%s %s",temp1,temp2);
			msg.reseason2 = atoi(temp2);
			if(list!=0)
			{
				ListInsertItem(list,&msg,END_OF_LIST);
			}
		}
	}	

}

BOOL GetPileRecordList(EUT eut,char *command,int retryCnt,ListType list)
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
			//WarnShow1(0,"�����ʧ��!");
			ret=FALSE;
		}else{
			//printf("%s\n",servicePtr->packet);   
			ParsePileRecordBuffer(servicePtr->packet,list);
			
			ret=TRUE;
			break;
		}
		//
	}while(retryCnt--);
	return ret;	
}

void ParseDataPoolGroupBuffer(char *buffer,ListType list)
{
	char	*tok;
	tDataPoolItem msg={0};
	//printf("%s\n",buffer);
	for (tok = strtok(buffer, "\n");tok;tok = strtok(NULL, "\n"))   //һ��һ�ж�
	{
		if(strstr(tok,"begin")== NULL && strstr(tok,"��ǰ���")== NULL)
		{
		   //printf("%s\n",tok);
		   char temp1[5]={0},temp2[5]={0},temp3[5]={0};
		   sscanf(tok,"%s %s %s    %s %s",temp1,temp2,temp3,msg.name,msg.status); 
		   //printf("%s %s\n",msg.name,msg.status);
		   msg.gunIndex = atoi(temp1);
		   ListInsertItem(list,&msg,END_OF_LIST);
		}
	}	

}

BOOL GetDataPoolGroupList(EUT eut,int group,int array,int retryCnt,ListType list)
{
	BOOL ret=TRUE; 
	do
	{
		tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	    if(servicePtr==NULL)
	    {
		     return FALSE;
	    }	
		char cmd[50]={0};
		sprintf(cmd,"g %d\r\n",group);
		if(startCommand(servicePtr,cmd)<0)
			ret = FALSE;
		memset(cmd,0,50);
		sprintf(cmd,"i %d\r\n",array);
		if(startCommand(servicePtr,cmd)<0)
			ret = FALSE;
		memset(cmd,0,50);
		sprintf(cmd,"%s","v\r\n"); 
		if(startCommand(servicePtr,cmd)<0)
		{
			onStubDisConnected(servicePtr);
			//WarnShow1(0,"�����ʧ��!");
			ret=FALSE;
		}else{
			//printf("%s\n",servicePtr->packet);   
			ParseDataPoolGroupBuffer(servicePtr->packet,list);
			
			ret=TRUE;
			break;
		}
		//
	}while(retryCnt--);
	return ret;	
}


