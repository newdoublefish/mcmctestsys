#include "cvixml.h"
#include "cJSON.h" 
#include "toolbox.h"
#include "testGroupInit.h"
#include "testGroupParseXml.h"

extern HashTableType tipsHashTable;

#define CASE_ELEM "case"
#define PROTOCOL_ELEM "protocol"

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
				if(sscanf(attributeValue,"%lf",&item.standard_)==1)
				{
											
				}else{
					if(strstr(attributeValue,"NA")==NULL)
						ParseStandardValue(attributeValue,&item.standard_value);
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
				HashTableInsertItem(tipsHashTable,group.groupName,attributeValue);
				
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
		printf("%s\n",item.itemName);
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
			printf("%s %s\n",attributeName,attributeValue);
			free(attributeName);
			free(attributeValue);
			CVIXMLDiscardAttribute(attribute);
		}
		ListInsertItem(protocolItemList,&item,END_OF_LIST);
		CVIXMLDiscardElement(currChildElem);
	}
	return TRUE;
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
		printf("%s\n",protocol.protocolName);
		protocol.protocolItems = ListCreate(sizeof(tProtocolItem));
		LoadProtocolItemsFromXml(protocol.protocolItems,currChildElem);
		ListInsertItem(protocolList,&protocol,END_OF_LIST);
		//free(groupName);
		CVIXMLDiscardElement(currChildElem);
	}
	return TRUE;
}

BOOL LoadTestGroupFromXml(ListType groupList,ListType itemList,ListType protocolList,char *filePath)
{
	CVIXMLDocument      document = 0;
	CVIXMLElement       currElem = 0 ,currChildElem = 0;
	int                 len,numChildElems;
	char                *elemName = NULL, *elemValue = NULL;
	/* Load document and process it */
	CVIXMLLoadDocument (filePath, &document);
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
			}
			free(currChildElemName);
			CVIXMLDiscardElement(currChildElem);
		}
	}
	free (elemName);
	free (elemValue);
	CVIXMLDiscardElement (currElem);
	CVIXMLDiscardDocument (document);
	return TRUE;
}
