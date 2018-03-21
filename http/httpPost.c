#include "postdll.h"
#include"httpPost.h"
#include "toolbox.h"
#include "cJSON.h"
#include "log.h"


BOOL isRegisterd=FALSE;

BOOL registerPostDLL()
{
	if(TRUE==isRegisterd)
		return TRUE;
	char fileName[MAX_PATHNAME_LEN]; 
	GetProjectDir (fileName); 
	strcat(fileName,"\\postdll.dll"); 
	
	if(CDotNetRegisterAssemblyPath("postdll, Version=1.0.0.0, Culture=neutral, PublicKeyToken=null",fileName)<0)
		return FALSE;
	isRegisterd=TRUE;
	return TRUE;
		
}

BOOL buildPostString(char *buffer,char *recordName,char *type,char *fileName,char *location,char *person,int pass)
{
	cJSON *root = NULL;
	cJSON *data =NULL;
	root = cJSON_CreateObject();
	data = cJSON_CreateObject(); 
	cJSON_AddStringToObject(root,"request","report");
	cJSON_AddStringToObject(data,"code",recordName);  
	cJSON_AddStringToObject(data,"type",type);
	if(pass==1)
		cJSON_AddStringToObject(data,"result","true");
	else
		cJSON_AddStringToObject(data,"result","false");
	cJSON_AddStringToObject(data,"report",fileName);
	cJSON_AddStringToObject(data,"factory",location);
	cJSON_AddStringToObject(data,"person",person);
	cJSON_AddStringToObject(data,"approved","false"); 
	cJSON_AddItemToObject(root,"data",data);
	sprintf(buffer,"%s",cJSON_Print(root));
	cJSON_Delete(root);
	return TRUE;
}

int httpPost(char *recordName,char *type,char *fileName,char *location,char *person,int pass)
{
	if(FALSE==registerPostDLL())
		return -1;
	Initialize_postdll();
	postdll_Class1 class1;  
	postdll_Class1__Create(&class1,NULL);
	char sendBuffer[256]={0};
	char *result;
	buildPostString(sendBuffer,recordName,type,fileName,location,person,pass);
	postdll_Class1_PostUrl("http://127.0.0.1:80/poll/add/", sendBuffer,&result,NULL);
	//printf("%s",result);
	CDotNetFreeMemory(class1);
	Close_postdll();			
	return 0;	
}

int parsePostResponse(char *response)
{
	int ret=0;
	cJSON *root = NULL;
	cJSON *data=NULL;
	cJSON *item = NULL;
	
	root = cJSON_Parse(response);
	item = cJSON_GetObjectItem(root, "status");
	//printf("%s\n",cJSON_Print(item));
	if(strstr(cJSON_Print(item),"success")!=NULL)
	{
		ret=1;
	}
	cJSON_Delete(root);
	return ret;
}

int httpPostJson(char *url,char *jsonData)
{
	int ret=0;
	if(FALSE==registerPostDLL())
	{
		LOG_EVENT_FORMAT(LOG_ERROR,"%s","register post dll error,check whether you have install .net framework");
		return -1;
	}
	Initialize_postdll();
	postdll_Class1 class1;  
	postdll_Class1__Create(&class1,NULL);
	char *result=NULL;
	postdll_Class1_PostUrl(url, jsonData,&result,NULL);
	LOG_EVENT_FORMAT(LOG_ERROR,"post url:%s",url); 
	LOG_EVENT_FORMAT(LOG_ERROR,"post:%s",jsonData);
	ret=parsePostResponse(result);
	LOG_EVENT_FORMAT(LOG_ERROR,"recv:%s",result);
	/*if(result!=NULL)
		free(result);*/
	CDotNetFreeMemory(class1);
	Close_postdll();			
	return ret;	
}

