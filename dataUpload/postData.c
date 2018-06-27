#include "cvixml.h"
#include "cJSON.h" 
#include "toolbox.h"
#include "postData.h"
#include "sutCommon.h"
#include "log.h"
#include "common.h"

ListType postDataSet=0;
BOOL importPostProtocol(void);

ListType getPostDataSet(void)
{
	importPostProtocol();
	return postDataSet;
}


BOOL loadParam(CVIXMLElement currElem,ListType paramList)
{
	CVIXMLElement  currChildElem = 0,childElem = 0;
	int                 len,numChildElems;
	char                *elemName = NULL, *elemValue = NULL;
	CVIXMLGetNumChildElements (currElem, &numChildElems);
	for(int index=0;index<numChildElems;index++)
	{
		CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
		
		int numCurrChildElems = 0;
		CVIXMLGetNumChildElements (currChildElem, &numCurrChildElems);
		tPostParam paramBase={0}; 
		paramBase.postParamList = ListCreate(sizeof(tPostParam));
		for(int i = 0;i<numCurrChildElems;i++)
		{
			 CVIXMLGetChildElementByIndex (currChildElem, i, &childElem);
			char *childElemName = NULL;
			char *childElemValue = NULL;
			CVIXMLGetElementTagLength (childElem, &len);
			childElemName = malloc (len + 1);
			CVIXMLGetElementTag (childElem, childElemName);
			CVIXMLGetElementValueLength (childElem, &len);
		
			
			if(strcmp("param",childElemName)==0)
			{
				tPostParam param={0};
				int numberAttributes=0;
				CVIXMLGetNumAttributes(childElem,&numberAttributes);
				for(int i=0;i<numberAttributes;i++)
				{
					char name[30]={0};
					CVIXMLAttribute attribute;
					CVIXMLGetAttributeByIndex (childElem,i,&attribute);
					CVIXMLGetAttributeName(attribute,name);
					//CVIXMLGetAttributeValue (attribute,name);
					if(strcmp(name,"name")==0)
					{
						//ParseXmlResult(currChildElem,hashTable);
						CVIXMLGetAttributeValue (attribute,param.name);
					}
					if(strcmp(name,"type")==0)
					{
						//ParseXmlResult(currChildElem,hashTable);
						CVIXMLGetAttributeValue (attribute,param.type);
					}					
					CVIXMLDiscardAttribute(attribute);
				}

				childElemValue = malloc (len + 1);
			    CVIXMLGetElementValue (childElem, childElemValue);	
				sprintf(param.value,"%s",childElemValue);
				ListInsertItem(paramBase.postParamList,&param,END_OF_LIST);
			}			 
			 
		}
		ListInsertItem(paramList,&paramBase,END_OF_LIST);
	}
	return TRUE;
}

BOOL loadPostDataParam(CVIXMLElement currElem,ListType postParamList)
{
	CVIXMLElement  currChildElem = 0;
	int                 len,numChildElems;
	char                *elemName = NULL, *elemValue = NULL;
	/* Load document and process it */
	CVIXMLGetElementTagLength (currElem, &len);
	elemName = malloc (len + 1);
	CVIXMLGetElementTag (currElem, elemName);
	CVIXMLGetElementValueLength (currElem, &len);
	elemValue = malloc (len + 1);

	CVIXMLGetElementValue (currElem, elemValue);

	if(strcmp(elemName,"post")==0)
	{
		tPostData postData={0};
		CVIXMLAttribute attr;
		CVIXMLGetAttributeByName(currElem,"name",&attr);
		CVIXMLGetAttributeValue (attr,postData.name);
		CVIXMLDiscardAttribute(attr);

		CVIXMLGetAttributeByName(currElem,"url",&attr);
		CVIXMLGetAttributeValue (attr,postData.url);
		CVIXMLDiscardAttribute(attr);

		CVIXMLGetAttributeByName(currElem,"request",&attr);
		CVIXMLGetAttributeValue (attr,postData.request);
		CVIXMLDiscardAttribute(attr);
		
		char upload[20]={0};
		CVIXMLGetAttributeByName(currElem,"upload",&attr);
		CVIXMLGetAttributeValue (attr,upload);
		CVIXMLDiscardAttribute(attr);
		
		if(strcmp(upload,"True")!=0)
		{
			postData.upload = 0;
		}else{
			postData.upload = 1;
		}
		//	goto DONE;
		
		
		postData.postParamList = ListCreate(sizeof(tPostParam));

		/*CVIXMLAttribute attr;
		CVIXMLGetAttributeByName(currElem,"testProject",&attr);
		CVIXMLGetAttributeValue (attr,testProjectName);*/
		CVIXMLGetNumChildElements (currElem, &numChildElems);
		for(int index=0;index<numChildElems;index++){
			CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
			char *childElemName = NULL;
			char *childElemValue = NULL;
			CVIXMLGetElementTagLength (currChildElem, &len);
			childElemName = malloc (len + 1);
			CVIXMLGetElementTag (currChildElem, childElemName);
			CVIXMLGetElementValueLength (currChildElem, &len);
		
			if(strcmp("param",childElemName)==0)
			{
				tPostParam param={0};
				int numberAttributes=0;
				CVIXMLGetNumAttributes(currChildElem,&numberAttributes);
				for(int i=0;i<numberAttributes;i++)
				{
					char name[30]={0};
					CVIXMLAttribute attribute;
					CVIXMLGetAttributeByIndex (currChildElem,i,&attribute);
					CVIXMLGetAttributeName(attribute,name);
					//CVIXMLGetAttributeValue (attribute,name);
					if(strcmp(name,"name")==0)
					{
						//ParseXmlResult(currChildElem,hashTable);
						CVIXMLGetAttributeValue (attribute,param.name);
					}
					if(strcmp(name,"type")==0)
					{
						//ParseXmlResult(currChildElem,hashTable);
						CVIXMLGetAttributeValue (attribute,param.type);
					}					
					CVIXMLDiscardAttribute(attribute);
				}

				childElemValue = malloc (len + 1);
			    CVIXMLGetElementValue (currChildElem, childElemValue);	
				sprintf(param.value,"%s",childElemValue);
				ListInsertItem(postData.postParamList,&param,END_OF_LIST);
			}else if(strcmp("array",childElemName)==0) 
			{
				int numCurrChildElems=0;
				tPostParam param={0};
				int numberAttributes=0;
				CVIXMLGetNumAttributes(currChildElem,&numberAttributes);
				for(int i=0;i<numberAttributes;i++)
				{
					char name[30]={0};
					CVIXMLAttribute attribute;
					CVIXMLGetAttributeByIndex (currChildElem,i,&attribute);
					CVIXMLGetAttributeName(attribute,name);
					//CVIXMLGetAttributeValue (attribute,name);
					if(strcmp(name,"name")==0)
					{
						//ParseXmlResult(currChildElem,hashTable);
						CVIXMLGetAttributeValue (attribute,param.name);
					}
					if(strcmp(name,"type")==0)
					{
						//ParseXmlResult(currChildElem,hashTable);
						CVIXMLGetAttributeValue (attribute,param.type);
					}					
					CVIXMLDiscardAttribute(attribute);
				}				
				CVIXMLGetNumChildElements (currChildElem, &numCurrChildElems);	
				if(numCurrChildElems>0)
				{
					param.postParamList = ListCreate(sizeof(tPostParam));
					loadParam(currChildElem,param.postParamList);
					ListInsertItem(postData.postParamList,&param,END_OF_LIST);
				}								
			}
			if(childElemName!=NULL)
				free (childElemName);
			if(childElemValue!=NULL)
				free (childElemValue);
			CVIXMLDiscardElement (currChildElem);
		}
		ListInsertItem(postDataSet,&postData,END_OF_LIST);
	}
DONE:
	free (elemName);
	free (elemValue);
	return TRUE;
}

BOOL loadPostDataFromXml(char *fileName,ListType postDataSet)
{
	CVIXMLDocument      document = 0;
	CVIXMLElement       currElem = 0 ,currChildElem = 0;
	int                 len,numChildElems;
	char                *elemName = NULL, *elemValue = NULL;
	/* Load document and process it */
	CVIXMLLoadDocument (fileName, &document);
	CVIXMLGetRootElement (document, &currElem);
	CVIXMLGetElementTagLength (currElem, &len);
	elemName = malloc (len + 1);
	CVIXMLGetElementTag (currElem, elemName);
	CVIXMLGetElementValueLength (currElem, &len);
	elemValue = malloc (len + 1);

	CVIXMLGetElementValue (currElem, elemValue);

	if(strcmp(elemName,"postset")==0)
	{
		CVIXMLGetNumChildElements (currElem, &numChildElems);
		for(int index=0;index<numChildElems;index++){
			CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
			loadPostDataParam(currChildElem,postDataSet);
			CVIXMLDiscardElement(currChildElem);

		}
	}

	free (elemName);
	free (elemValue);
	CVIXMLDiscardElement (currElem);
	CVIXMLDiscardDocument (document);
	return TRUE;
}

extern BOOL getResultJson(char *name,cJSON *data);
extern BOOL getGroupResultJson(char *name,cJSON *data);

BOOL buildPostDataStr(tPostData postData,char **buffer,void *callbackFunc,void *callbackData)
{
	cJSON *root = NULL;                                                                                
	cJSON *data =NULL;                                                                                 
	root = cJSON_CreateObject();                                                                       
	data = cJSON_CreateObject();
	cJSON *array = NULL;
	cJSON *item = NULL;
	cJSON_AddStringToObject(root,"request",postData.request);                                                  
	for(int i=1;i<=ListNumItems(postData.postParamList);i++)
	{	     
		tPostParam param ={0};
		ListGetItem(postData.postParamList,&param,i);  
		if(param.postParamList==0)
		{
			if(callbackFunc!=NULL)
			{
				tPostItem pItem={0};
				pItem.type = realloc(pItem.type,strlen(param.type)+1);
				sprintf(pItem.type,"%s",param.type);
				pItem.value = realloc(pItem.value,strlen(param.value)+1);
				sprintf(pItem.value,"%s",param.value);
				if(strcmp(pItem.type,"resultJson")==0)
				{
					getGroupResultJson(param.name,data);
				}else{
					(*(ON_FILL_DATA_PARAM)(callbackFunc))(&pItem,callbackData);
					cJSON_AddStringToObject(data,param.name,pItem.value);
				}
				if(pItem.type!=NULL)
					free(pItem.type);
				if(pItem.value!=NULL)
					free(pItem.value);
				 
			}else
				cJSON_AddStringToObject(data,param.name,param.value);
		}else if(strcmp(param.type,"array")==0)
		{
			array = cJSON_CreateArray();
			cJSON_AddItemToObject(data,param.name,array);
			for(int arrayIndex=1;arrayIndex<=ListNumItems(param.postParamList);arrayIndex++)
			{
				tPostParam arrayParam = {0};
				ListGetItem(param.postParamList,&arrayParam,arrayIndex);
				item = cJSON_CreateObject();
				cJSON_AddItemToArray(array,item);
				//cJSON_AddItemToObject(array,"bom",item);
				for(int itemIndex=1;itemIndex<=ListNumItems(arrayParam.postParamList);itemIndex++)
				{
					tPostParam itemParam = {0};
					ListGetItem(arrayParam.postParamList,&itemParam,itemIndex);
					//if(callbackFunc!=NULL)
					//	(*(ON_FILL_DATA_PARAM)(callbackFunc))(&itemParam,callbackData);
					//cJSON_AddStringToObject(item,itemParam.name,itemParam.value); 
					if(callbackFunc!=NULL)
					{
						tPostItem pItem={0};
						pItem.type = realloc(pItem.type,strlen(itemParam.type)+1);
						sprintf(pItem.type,"%s",itemParam.type);
						pItem.value = realloc(pItem.value,strlen(itemParam.value)+1);
						sprintf(pItem.value,"%s",itemParam.value);
						(*(ON_FILL_DATA_PARAM)(callbackFunc))(&pItem,callbackData);
						cJSON_AddStringToObject(item,itemParam.name,pItem.value);
						if(pItem.type!=NULL)
							free(pItem.type);
						if(pItem.value!=NULL)
							free(pItem.value);
							 
					}else
						cJSON_AddStringToObject(item,itemParam.name,itemParam.value);					
					
				}
			}
		}
	}                                                  
	cJSON_AddItemToObject(root,"data",data);                                                           
	                                                                                                   
	//printf("%s\n",cJSON_Print(root));                                                                  
	/*if(buffer!=NULL)
		sprintf(buffer,"%s",cJSON_Print(root));*/
	*buffer = malloc(strlen(cJSON_Print(root))+1);
	sprintf(*buffer,"%s",cJSON_Print(root));
	//parseJson(cJSON_Print(root));                                                                      
	cJSON_Delete(root);
	return TRUE;                                                                                
}

void printParams(ListType list)
{
	for(int i=1;i<=ListNumItems(list);i++)
	{
		tPostParam tpp ={0};
		ListGetItem(list,&tpp,i);
		printf("--------param:%s,%s\n",tpp.name,tpp.value);
		if(tpp.postParamList!=0)
		{
			printf("%d",ListNumItems(tpp.postParamList));
			printParams(tpp.postParamList);
		}
		
	}
}

void printPostDataSet(ListType list)
{
	for(int i=1;i<=ListNumItems(list);i++)
	{
		printf("--------------------------------------");
		tPostData data = {0};
		ListGetItem(list,&data,i);
		printf("post name = %s\n",data.name);
		printParams(data.postParamList);
	}
}

void printPostDataJson(ListType postDataSet)
{
	for(int i=1;i<=ListNumItems(postDataSet);i++)
	{
		char *buffer=NULL;
		tPostData data={0};
		ListGetItem(postDataSet,&data,i);
		buildPostDataStr(data,&buffer,NULL,NULL);
		printf("%s",buffer);
		if(buffer!=NULL)
			free(buffer);
	}
}

BOOL importPostProtocol(void)
{
	if(0==postDataSet)
	{
		postDataSet = ListCreate(sizeof(tPostData));
		/*char fileName[MAX_PATHNAME_LEN]= {0};
		GetProjectDir(fileName);
		strcat(fileName,"\\post.xml");*/
		SUT sut=GetSeletedSut();
		if(FileExists(sut.postFilePath,NULL)==1)
		{
			loadPostDataFromXml(sut.postFilePath,postDataSet);
			//printPostDataSet(postDataSet);
			//printPostDataJson(postDataSet);
			
		}else{
			WarnShow1(0,"找不到post文件");
		}

	}
	return TRUE;
}

BOOL getPostData(tPostData *postDataPtr,char *name)
{
	if(0==postDataSet)
	{
		if(FALSE==importPostProtocol())
		{
			return FALSE;
		}
	}

	for(int i=1;i<=ListNumItems(postDataSet);i++)
	{
		ListGetItem(postDataSet,postDataPtr,i);
		if(strcmp(name,postDataPtr->name)==0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

