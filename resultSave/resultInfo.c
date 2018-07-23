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
#include "cvixml.h"
#include "toolbox.h"

#include "settingEntity.h" 
#include "testGroupInit.h"
#include "resultUtil.h"
#include "testFramework.h"
#include "resultInfo.h"   
#include "testProject.h"


void ParseXmlResult(CVIXMLElement currElem,HashTableType hashTable)
{
    CVIXMLElement       currChildElem = 0;  
    int                 len,numChildElems;
    char                *elemName = NULL, *elemValue = NULL;
	CVIXMLGetNumChildElements (currElem, &numChildElems);
	for(int index=0;index<numChildElems;index++){
		
		CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
		char *childElemName = NULL;
		CVIXMLGetElementTagLength (currChildElem, &len);
   		childElemName = malloc (len + 1);
    	CVIXMLGetElementTag (currChildElem, childElemName);
		if(strcmp(childElemName,"Result")==0)
		{
				int numberAttributes=0;
				CVIXMLGetNumAttributes(currChildElem,&numberAttributes);
				RESULT result={0};
				for(int i=0;i<numberAttributes;i++)
				{
					char temp[30]={0},value[50]={0};
					CVIXMLAttribute attribute;
					CVIXMLGetAttributeByIndex (currChildElem,i,&attribute);
					CVIXMLGetAttributeName (attribute,temp);
					CVIXMLGetAttributeValue (attribute,value); 
					if(strcmp(temp,"id")==0)
					{
						result.index = atoi(value);							
					}else if(strcmp(temp,"value")==0)
					{
						sprintf(result.recvString,"%s",value);
					}else if(strcmp(temp,"pass")==0)
					{
						//result.pass = atoi(value);
						if(strcmp(value,"合格")==0)
						{
							result.pass = RESULT_PASS;
						}else{
							result.pass = RESULT_FAIL;
						}
					}  
					CVIXMLDiscardAttribute(attribute);
				}
				saveResult(hashTable,&result);
		}
		CVIXMLDiscardElement (currChildElem);
	}
}

BOOL loadResultInfo(char *fileName,char *deviceName,HashTableType hashTable){
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

	if(strcmp(elemName,"Data")==0)
	{
		CVIXMLAttribute attr;
		CVIXMLGetAttributeByName(currElem,"testProject",&attr);
		//CVIXMLGetAttributeValue (attr,testProjectName);
		CVIXMLGetNumChildElements (currElem, &numChildElems); 
		for(int index=0;index<numChildElems;index++){
			CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);	
			char *childElemName = NULL;
		    CVIXMLGetElementTagLength (currChildElem, &len);
   			childElemName = malloc (len + 1);
    		CVIXMLGetElementTag (currChildElem, childElemName);
			if(strcmp("Device",childElemName)==0)
			{
				int numberAttributes=0;
				CVIXMLGetNumAttributes(currChildElem,&numberAttributes);
				for(int i=0;i<numberAttributes;i++)
				{
					char name[30]={0};
					CVIXMLAttribute attribute;
					CVIXMLGetAttributeByIndex (currChildElem,i,&attribute);
					CVIXMLGetAttributeValue (attribute,name);
					//if(strcmp(name,deviceName)==0)
					//{
						ParseXmlResult(currChildElem,hashTable);					
					//}
					CVIXMLDiscardAttribute(attribute);
				}
				
			}
			free (childElemName);
			CVIXMLDiscardElement (currChildElem);
		}
		CVIXMLDiscardAttribute(attr);
	}
	
    free (elemName);
    free (elemValue);
    CVIXMLDiscardElement (currElem);
    CVIXMLDiscardDocument (document);		
	return TRUE;
} 


#if 0
void saveEutList()
{
	char fileName[MAX_PATHNAME_LEN];
	SETTING set=getSetting();
	GetProjectDir (fileName);
	//strcat(fileName,deviceFile);
	
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

	CVIXMLGetNumChildElements (currElem, &numChildElems); 

	//removeEutElement(currElem);
	/*for(int i=1;i<=ListNumItems(eutList);i++)
	{
		CVIXMLNewElement(currElem,i-1,"Results", &currChildElem); 
		EEUT eut;
		ListGetItem(eutList,&eut,i);
		saveEut2Xml(eut,currChildElem);
		CVIXMLDiscardElement (currChildElem);
	}  */
	
    free (elemName);
    free (elemValue);
	CVIXMLSaveDocument (document, 0, NULL);
    CVIXMLDiscardElement (currElem);
    CVIXMLDiscardDocument (document);	
}
#endif

void saveResultToXml(CVIXMLElement currElem,HashTableType hashTable)
{
	
	ListType testItemList = getTestCaseList();
	int itemCnt = ListNumItems(testItemList);
	TestItem item={0}; 
	RESULT itemResult={0};   
	for(int i=1;i<=itemCnt;i++)
	{
		int found=0;
		
		ListGetItem(testItemList,&item,i);
		
		
		if(HashTableFindItem(hashTable,&item.itemId,&found)>=0)
		{
			if(found>0)
			{
				HashTableGetItem(hashTable,&item.itemId,&itemResult,sizeof(RESULT));
				CVIXMLElement currChildElem = 0;
				CVIXMLNewElement(currElem,-1,"Result", &currChildElem);
				char temp[10]={0};
				sprintf(temp,"%d",itemResult.index);
				CVIXMLAddAttribute(currChildElem,"id",temp);
				
				CVIXMLAddAttribute(currChildElem,"value",itemResult.recvString); 
				if(itemResult.pass == RESULT_PASS)
				{
					CVIXMLAddAttribute(currChildElem,"pass","合格"); 					
				}else{
					CVIXMLAddAttribute(currChildElem,"pass","不合格"); 
				}
				
				
				CVIXMLDiscardElement (currChildElem);
			}
		}
	}
}

BOOL saveResultInfo(TESTengine *gEngine,char *testProjectName){
	char fileName[MAX_PATHNAME_LEN];
	int fileSize=0;
	//SETTING s=getSetting();
	GetProjectDir (fileName);
	tTestProject *projectPtr=getCurrentProject();  	
	if(FileExists(projectPtr->projectDir,&fileSize)==0)
	{	
	    MakeDir(projectPtr->projectDir);
	}

    /*sprintf(fileName,"%s\\%s",s.saveDir,deviceName); 	
	if(FileExists(fileName,&fileSize)==0) //不存在
	{	
	    MakeDir(fileName);
	}*/
	
	sprintf(fileName,"%s\\%s%s",projectPtr->projectDir,testProjectName,".xml");
	
    CVIXMLDocument      document = 0;
    CVIXMLElement       currElem = 0 ,currChildElem = 0;  
    int                 len,numChildElems;
    char                *elemName = NULL, *elemValue = NULL;
	CVIXMLNewDocument("Data",&document);
    CVIXMLGetRootElement (document, &currElem);
	CVIXMLAddAttribute(currElem,"testProject",testProjectName);
	for(int i=0;i<gEngine->totalTestObject;i++)
	{
		CVIXMLNewElement(currElem,-1,"Device", &currChildElem);
		CVIXMLAddAttribute(currChildElem,"name",gEngine->objectArray[i].device.eutName);
		saveResultToXml(currChildElem,gEngine->objectArray[i].resultHashTable);
	}
    free (elemName);
    free (elemValue);
	CVIXMLSaveDocument (document, 0, fileName);
    CVIXMLDiscardElement (currChildElem);
	CVIXMLDiscardElement (currElem); 
    CVIXMLDiscardDocument (document);	
	return TRUE;
}
