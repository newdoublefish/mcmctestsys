#include "cvixml.h"
#include "cJSON.h" 
#include "toolbox.h"
#include "postData.h"
#include "sutCommon.h"
#include "log.h"

ListType postDataSet=0;

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
			childElemValue = malloc (len + 1);

			CVIXMLGetElementValue (currChildElem, childElemValue);			
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
				sprintf(param.value,"%s",childElemValue);
				ListInsertItem(postData.postParamList,&param,END_OF_LIST);

			}
			free (childElemName);
			free (childElemValue);
			CVIXMLDiscardElement (currChildElem);
		}
		ListInsertItem(postDataSet,&postData,END_OF_LIST);
	}

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


BOOL buildPostDataStr(tPostData postData,char *buffer,char *callbackFunc)
{
	cJSON *root = NULL;                                                                                
	cJSON *data =NULL;                                                                                 
	root = cJSON_CreateObject();                                                                       
	data = cJSON_CreateObject();                                                                       
	cJSON_AddStringToObject(root,"request",postData.request);                                                  
	for(int i=1;i<=ListNumItems(postData.postParamList);i++)
	{	     
		tPostParam param ={0};
		ListGetItem(postData.postParamList,&param,i);                                                                                              
		if(callbackFunc!=NULL)
			(*(ON_FILL_DATA_PARAM)(callbackFunc))(&param);
		cJSON_AddStringToObject(data,param.name,param.value); 
	}                                                  
	cJSON_AddItemToObject(root,"data",data);                                                           
	                                                                                                   
	//printf("%s\n",cJSON_Print(root));                                                                  
	if(buffer!=NULL)
		sprintf(buffer,"%s",cJSON_Print(root));                                                                                                   
	//parseJson(cJSON_Print(root));                                                                      
	cJSON_Delete(root);
	return TRUE;                                                                                
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
		loadPostDataFromXml(sut.postFilePath,postDataSet);

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

