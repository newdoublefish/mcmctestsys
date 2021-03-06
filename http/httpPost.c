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
	LOG_EVENT_FORMAT(LOG_ERROR,"post:%s",jsonData);
	LOG_EVENT_FORMAT(LOG_ERROR,"post url:%s",url); 
	
	ret=parsePostResponse(result);
	LOG_EVENT_FORMAT(LOG_ERROR,"recv:%s",result);
	/*if(result!=NULL)
		free(result);*/
	CDotNetDiscardHandle(class1);
	//CDotNetFreeMemory(class1);
	Close_postdll();			
	return ret;	
}


int parsePostResponseUserName(char *response,char **resData)
{
	int ret=0;
	cJSON *root = NULL;
	cJSON *data=NULL;
	cJSON *item = NULL;
	cJSON *subItem = NULL;
	
	root = cJSON_Parse(response);
	item = cJSON_GetObjectItem(root, "status");
	//printf("%s\n",cJSON_Print(item));
	if(strstr(cJSON_Print(item),"success")!=NULL)
	{
		ret=1;
	}
	
	item = cJSON_GetObjectItem(root, "data");
	//printf(cJSON_Print(item));
	
	subItem = cJSON_GetObjectItem(item, "username");
	//printf("%s",cJSON_Print(item)); 
	if(*resData!=NULL && ret==1)
	{
		int len = strlen(cJSON_Print(subItem))+1;
		*resData = malloc(len);
		memset(*resData,0,len);
		sprintf(*resData,"%s",cJSON_Print(subItem));
	}
	//printf(cJSON_Print(subItem));
	
	cJSON_Delete(root);
	return ret;
}

int httpPostJsonWithResponseData(char *url,char *jsonData,char **data)
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
	LOG_EVENT_FORMAT(LOG_ERROR,"post:%s",jsonData);
	LOG_EVENT_FORMAT(LOG_ERROR,"post url:%s",url); 
	
	//ret=parsePostResponseUserName(result,data);
	*data = (char *)malloc(strlen(result)+1);
	sprintf(*data,"%s",result);
	LOG_EVENT_FORMAT(LOG_ERROR,"recv:%s",result);
	/*if(result!=NULL)
		free(result);*/
	CDotNetDiscardHandle(class1);
	//CDotNetFreeMemory(class1);
	Close_postdll();			
	return ret;	
}


int httpPostJsonWithResponseUserName(char *url,char *jsonData,char **data)
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
	LOG_EVENT_FORMAT(LOG_ERROR,"post:%s",jsonData);
	LOG_EVENT_FORMAT(LOG_ERROR,"post url:%s",url); 
	
	ret=parsePostResponseUserName(result,data);
	LOG_EVENT_FORMAT(LOG_ERROR,"recv:%s",result);
	/*if(result!=NULL)
		free(result);*/
	CDotNetDiscardHandle(class1);
	//CDotNetFreeMemory(class1);
	Close_postdll();			
	return ret;	
}

