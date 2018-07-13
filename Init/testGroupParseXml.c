#include "cvixml.h"
#include "cJSON.h" 
#include "common.h"
#include "testGroupInit.h"
#include "testGroupParseXml.h"
#include "tipsParse.h"
#include "toolbox.h" 
#include "relayProtocol.h" 
#include "convertTool.h" 

#define CASE_ELEM "case"
#define PROTOCOL_ELEM "protocol"
#define RELAY_ELEM "relays"

 
#ifndef xmlChk
#define xmlChk(f) if (xmlErr = (f), FAILED (xmlErr)) goto Error; else
#endif

extern ParseStandardValue(char *expression,VALUE_STANDARD *standard);

BOOL LoadCaseFromXml(ListType groupItemList,ListType itemList,CVIXMLElement currElem)
{
	CVIXMLElement currChildElem = 0;
	int len,numChildElems; 
	CVIXMLGetNumChildElements (currElem, &numChildElems); 
	for(int index=0;index<numChildElems;index++)
	{
		CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
		char *itemName = NULL,*itemValue = NULL;
		CVIXMLGetElementTagLength (currChildElem, &len);
		itemName = malloc (len + 1);
		CVIXMLGetElementTag (currChildElem, itemName);
		CVIXMLGetElementValueLength (currChildElem, &len);
		itemValue = malloc (len + 1);
		CVIXMLGetElementValue (currChildElem, itemValue);		
		TestItem item={0};
		
		int numberAttributes=0;
		CVIXMLGetNumAttributes(currChildElem,&numberAttributes);
		for(int i=0;i<numberAttributes;i++)
		{
			CVIXMLAttribute attribute;
			CVIXMLGetAttributeByIndex (currChildElem,i,&attribute);
			char *attributeName,*attributeValue;
			CVIXMLGetAttributeNameLength(attribute,&len);
			attributeName = malloc (len + 1);
			CVIXMLGetAttributeValueLength(attribute,&len);
			attributeValue = malloc (len + 1); 

			CVIXMLGetAttributeName(attribute,attributeName);
			CVIXMLGetAttributeValue(attribute,attributeValue);
			if(strcmp(attributeName,"id")==0)
			{
				item.itemId = atoi(attributeValue);	
			}
			if(strcmp(attributeName,"input")==0)
			{
				if(sscanf(attributeValue,"%lf",&item.input_Value)!=1)
				{
						
				}
				sprintf(item.inputValue_,"%s",attributeValue);
				
			}
			if(strcmp(attributeName,"standard")==0)
			{
				if(strstr(attributeValue,"NA")==NULL)
				{
					if(sscanf(attributeValue,"%lf",&item.standard_)==1)
					{
											
					}else{
						if(strstr(attributeValue,"NA")==NULL)
							ParseStandardValue(attributeValue,&item.standard_value);
					}
				}
				sprintf(item.standard_,"%s",attributeValue);
			}
			free(attributeName);
			free(attributeValue);
			CVIXMLDiscardAttribute(attribute);
		}
		sprintf(item.itemName_,"%s",itemValue);
		ListInsertItem(groupItemList,&item,END_OF_LIST);
		ListInsertItem(itemList,&item,END_OF_LIST);  
		free(itemName);
		free(itemValue);
		CVIXMLDiscardElement(currChildElem);
	}
	return TRUE;	

}

BOOL LoadGroupFromXml(ListType groupList,ListType itemList,CVIXMLElement currElem)
{
	CVIXMLElement currChildElem = 0;
	int len,numChildElems; 
	CVIXMLGetNumChildElements (currElem, &numChildElems); 
	for(int index=0;index<numChildElems;index++)
	{
		CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
		char *groupName = NULL;
		CVIXMLGetElementTagLength (currChildElem, &len);
		groupName = malloc (len + 1);
		CVIXMLGetElementTag (currChildElem, groupName);
		TestGroup group={0};
		
		int numberAttributes=0;
		CVIXMLGetNumAttributes(currChildElem,&numberAttributes);
		for(int i=0;i<numberAttributes;i++)
		{
			CVIXMLAttribute attribute;
			CVIXMLGetAttributeByIndex (currChildElem,i,&attribute);
			char *attributeName,*attributeValue;
			CVIXMLGetAttributeNameLength(attribute,&len);
			attributeName = malloc (len + 1);
			CVIXMLGetAttributeValueLength(attribute,&len);
			attributeValue = malloc (len + 1); 

			CVIXMLGetAttributeName(attribute,attributeName);
			CVIXMLGetAttributeValue(attribute,attributeValue);
			if(strcmp(attributeName,"name")==0)
			{
				CVIXMLGetAttributeValue (attribute,group.groupName);
			}else if(strcmp(attributeName,"type")==0)
			{
				CVIXMLGetAttributeValue (attribute,group.type);
			}else if(strcmp(attributeName,"tips")==0)
			{
				//CVIXMLGetAttributeValue (attribute,group.type);
				//if(tipsHashTable == 0)
				//	HashTableCreate(10,C_STRING_KEY,0,256,&tipsHashTable);  
				HashTableInsertItem(getTipsHashTable(),group.groupName,attributeValue);
				
			}
			free(attributeName);
			free(attributeValue);
			CVIXMLDiscardAttribute(attribute);
		}		
		group.groupId = ListNumItems(groupList);
		sprintf(group.nickName,"ÌõÀý×é%d_%s",group.groupId,group.groupName);
		group.subItems=ListCreate(sizeof(TestItem));
		LoadCaseFromXml(group.subItems,itemList,currChildElem);
		ListInsertItem(groupList,&group,END_OF_LIST);
		free(groupName);
		CVIXMLDiscardElement(currChildElem);
	}
	return TRUE;
	
}

BOOL LoadProtocolItemsFromXml(ListType protocolItemList,CVIXMLElement currElem)
{
	CVIXMLElement currChildElem = 0;
	int len,numChildElems; 
	CVIXMLGetNumChildElements (currElem, &numChildElems); 
	for(int index=0;index<numChildElems;index++)
	{
		CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
		tProtocolItem item = {0}; 
		CVIXMLGetElementTagLength (currChildElem, &len);
		item.itemName = malloc (len + 1);
		CVIXMLGetElementTag (currChildElem, item.itemName);
		//printf("%s\n",item.itemName);
		//free(groupName);
		HashTableCreate(10,C_STRING_KEY,0,256,&item.attributeHash); 
		int numberAttributes=0;
		CVIXMLGetNumAttributes(currChildElem,&numberAttributes);
		for(int i=0;i<numberAttributes;i++)
		{
			CVIXMLAttribute attribute;
			CVIXMLGetAttributeByIndex (currChildElem,i,&attribute);
			char *attributeName,*attributeValue;
			CVIXMLGetAttributeNameLength(attribute,&len);
			attributeName = malloc (len + 1);
			CVIXMLGetAttributeValueLength(attribute,&len);
			attributeValue = malloc (len + 1); 

			CVIXMLGetAttributeName(attribute,attributeName);
			CVIXMLGetAttributeValue(attribute,attributeValue);
			
			HashTableInsertItem(item.attributeHash,attributeName,attributeValue);
			//printf("%s %s\n",attributeName,attributeValue);
			free(attributeName);
			free(attributeValue);
			CVIXMLDiscardAttribute(attribute);
		}
		ListInsertItem(protocolItemList,&item,END_OF_LIST);
		CVIXMLDiscardElement(currChildElem);
	}
	return TRUE;
}

BOOL isExsitsProtocol(ListType protocolList,char *protocolName)
{
	tProtocol protocol;
	for(int i=1;i<=ListNumItems(protocolList);i++)
	{
		ListGetItem(protocolList,&protocol,i);
		if(strcmp(protocol.protocolName,protocolName)==0)
		{
			return TRUE;
		}
	}
	return FALSE;
}

BOOL LoadProtocolFromXml(ListType protocolList,CVIXMLElement currElem)
{
	CVIXMLElement currChildElem = 0;
	int len,numChildElems; 
	CVIXMLGetNumChildElements (currElem, &numChildElems); 
	for(int index=0;index<numChildElems;index++)
	{
		CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
		tProtocol protocol = {0}; 
		CVIXMLGetElementTagLength (currChildElem, &len);
		protocol.protocolName = malloc (len + 1);
		CVIXMLGetElementTag (currChildElem, protocol.protocolName);
		//printf("%s\n",protocol.protocolName);
		if(isExsitsProtocol(protocolList,protocol.protocolName)==FALSE)
		{
			protocol.protocolItems = ListCreate(sizeof(tProtocolItem));
			LoadProtocolItemsFromXml(protocol.protocolItems,currChildElem);
			ListInsertItem(protocolList,&protocol,END_OF_LIST);
		}else{
			free(protocol.protocolName);
		}
		//free(groupName);
		CVIXMLDiscardElement(currChildElem);
	}
	return TRUE;
}

BOOL LoadRelayFromXml(CVIXMLElement currElem)
{
	CVIXMLElement currChildElem = 0;
	int len,numChildElems; 
	CVIXMLGetNumChildElements (currElem, &numChildElems); 
	for(int index=0;index<numChildElems;index++)
	{
		CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
		RelayOperate operate={0};
		CVIXMLGetElementValue (currChildElem, operate.key);
		int numberAttributes=0;
		CVIXMLGetNumAttributes(currChildElem,&numberAttributes);
		for(int i=0;i<numberAttributes;i++)
		{
			CVIXMLAttribute attribute;
			CVIXMLGetAttributeByIndex (currChildElem,i,&attribute);
			char *attributeName,*attributeValue;
			CVIXMLGetAttributeNameLength(attribute,&len);
			attributeName = malloc (len + 1);
			CVIXMLGetAttributeValueLength(attribute,&len);
			attributeValue = malloc (len + 1); 

			CVIXMLGetAttributeName(attribute,attributeName);
			CVIXMLGetAttributeValue(attribute,attributeValue);
			
			if(strcmp(attributeName,"before")==0)
			{
				if(strlen(attributeValue)==10)
				{
					if(strstr(attributeValue,"0x")!=NULL)
					{
						unsigned char array[20]={0};
					 	int len=String2ByteArray(attributeValue,array);
					 	if(len==4)
					 	{
					 		unsigned int ret=0;
				     		for(int i=0;i<len;i++)
				     		{
					   			ret=ret<<8 | array[i];
				     		}
							operate.beforeTestMask = ret;
					 	}
					}
				}
			}else if(strcmp(attributeName,"after")==0)
			{
				if(strlen(attributeValue)==10)
				{
					if(strstr(attributeValue,"0x")!=NULL)
					{
						unsigned char array[20]={0};
					 	int len=String2ByteArray(attributeValue,array);
					 	if(len==4)
					 	{
					 		unsigned int ret=0;
				     		for(int i=0;i<len;i++)
				     		{
					   			ret=ret<<8 | array[i];
				     		}
							operate.afterTestMask = ret;
					 	}
					}
				}				
			}else if(strcmp(attributeName,"mask")==0)
			{
				if(strlen(attributeValue)==10)
				{
					if(strstr(attributeValue,"0x")!=NULL)
					{
						unsigned char array[20]={0};
					 	int len=String2ByteArray(attributeValue,array);
					 	if(len==4)
					 	{
					 		unsigned int ret=0;
				     		for(int i=0;i<len;i++)
				     		{
					   			ret=ret<<8 | array[i];
				     		}
							operate.mask = ret;
					 	}
					}
				}				
			}
			free(attributeName);
			free(attributeValue);
			CVIXMLDiscardAttribute(attribute);
		}	
		//printf("%s,%x,%x,%x",operate.key,operate.beforeTestMask,operate.afterTestMask,operate.mask);
		HashTableInsertItem(getRelayHashTable(),operate.key,&operate);
		//free(groupName);
		CVIXMLDiscardElement(currChildElem);
	}
	return TRUE;
}

BOOL LoadTestGroupFromXml(ListType groupList,ListType itemList,ListType protocolList,char *filePath)
{
	CVIXMLStatus		xmlErr = S_OK;
	CVIXMLDocument      document = 0;
	CVIXMLElement       currElem = 0 ,currChildElem = 0;
	int                 len,numChildElems;
	char                *elemName = NULL, *elemValue = NULL;
	/* Load document and process it */
	xmlChk(CVIXMLLoadDocument (filePath, &document));
	CVIXMLGetRootElement (document, &currElem);
	CVIXMLGetElementTagLength (currElem, &len);
	elemName = malloc (len + 1);
	CVIXMLGetElementTag (currElem, elemName);
	CVIXMLGetElementValueLength (currElem, &len);
	elemValue = malloc (len + 1);

	CVIXMLGetElementValue (currElem, elemValue);

	if(strcmp(elemName,"app")==0)
	{
		CVIXMLGetNumChildElements (currElem, &numChildElems);
		for(int index=0;index<numChildElems;index++){
			CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
			//loadPostDataParam(currChildElem,postDataSet);
			char *currChildElemName = NULL;
			CVIXMLGetElementTagLength (currChildElem, &len); 
			currChildElemName = malloc (len + 1); 
			CVIXMLGetElementTag (currChildElem, currChildElemName);
			if(strcmp(currChildElemName,CASE_ELEM)==0)
			{
				LoadGroupFromXml(groupList,itemList,currChildElem);
			}else if(strcmp(currChildElemName,PROTOCOL_ELEM)==0)
			{
				LoadProtocolFromXml(protocolList,currChildElem);
			}else if(strcmp(currChildElemName,RELAY_ELEM)==0)
			{
				LoadRelayFromXml(currChildElem);
			}
			free(currChildElemName);
			CVIXMLDiscardElement(currChildElem);
		}
	}
Error: 
	free (elemName);
	free (elemValue);
	CVIXMLDiscardElement (currElem);
	CVIXMLDiscardDocument (document);
	if (FAILED (xmlErr)) {
		char	errBuf[512];
		CVIXMLGetErrorString (xmlErr, errBuf, sizeof (errBuf));
		//MessagePopup ("XML Error", errBuf);
		WarnShow1(0,errBuf);
		return FALSE;
	}	
	return TRUE;
}
