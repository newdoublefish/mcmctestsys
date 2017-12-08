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
#include <cvirte.h>		
#include <userint.h>
#include <pathctrl.h>
#include "cvixml.h"        
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include "policyEntity.h" 
#include "policyPanel.h"
#include "sutCommon.h"
#include "configPath.h"



static Collect tempCollect;   
//ListType collectList;
#define MAX_SIZE 1024

static char fileName[MAX_PATHNAME_LEN];  

static char *strategyFile="\\config\\Strategy.xml"; 
static char *xmlStrategyName="StrategyName";
static char *xmlStrategyTestType="StrategyTestType";     
static char *xmlCollects="Collects";
static char *xmlCollect="Collect";
static char *xmlCollectName="CollectName";
static char *xmlResultFrom="ResultFrom";
static char *xmlTestType="TestType";
static char *xmlMaxTestDevice="MaxTestDevice";
static char *xmlRetryCount="RetryCount";
static char *xmlGroups="Groups";
static char *xmlGroupId="GroupId";
static int first=0;

#ifndef xmlChk
#define xmlChk(f) if (xmlErr = (f), FAILED (xmlErr)) goto Error; else
#endif

static int ProcessXMLElement (CVIXMLElement element,ListType collectList)
{
    int                 error = 0;
    CVIXMLStatus        xmlErr = S_OK;
    int                 numChildElems, len;
    CVIXMLAttribute     currAttr = 0;
    CVIXMLElement       currChildElem = 0;
    char                *elemName = NULL, *elemValue = NULL;
	int itemCount;
	int parent;
	int itemId;
    /* Process child elements */
    xmlChk (CVIXMLGetNumChildElements (element, &numChildElems));
    for (int index = 0; index < numChildElems; ++index) {
        xmlChk (CVIXMLGetChildElementByIndex (element, index, &currChildElem));
        xmlChk (CVIXMLGetElementTagLength (currChildElem, &len));
        nullChk (elemName = malloc (len + 1));
        xmlChk (CVIXMLGetElementTag (currChildElem, elemName));
        xmlChk (CVIXMLGetElementValueLength (currChildElem, &len));
        nullChk (elemValue = malloc (len + 1));
        xmlChk (CVIXMLGetElementValue (currChildElem, elemValue));
        RemoveSurroundingWhiteSpace (elemValue);
		//添加代码
		if(strcmp(elemName,xmlCollectName)==0)
		{
			if(first==1)
			{
			      first=0;
				  tempCollect.groups=ListCreate(sizeof(int));
			}else
			{
				 ListInsertItem(collectList,&tempCollect,END_OF_LIST);
				 tempCollect.groups=ListCreate(sizeof(int));
				 
			}	
			//tempCollect.name=(char*)malloc(len+1);
			memset(tempCollect.name_,0,COLLECT_NAME_LEN);
			memcpy(tempCollect.name_,elemValue,len+1);
			
		}else if(strcmp(elemName,xmlResultFrom)==0) 
		{
			 tempCollect.resultDerive=atoi(elemValue);
		}else if(strcmp(elemName,xmlTestType)==0) 
		{
			 tempCollect.testType=atoi(elemValue);
			
		}else if(strcmp(elemName,xmlMaxTestDevice)==0) 
		{
			tempCollect.maxParallelDevice=atoi(elemValue);
			//ListInsertItem(collectList,&tempCollect,END_OF_LIST);
		}else if(strcmp(elemName,xmlRetryCount)==0) 
		{
			tempCollect.retryCount=atoi(elemValue);
					
		}else if(strcmp(elemName,xmlGroups)==0) 
		{
			
		}else if(strcmp(elemName,xmlGroupId)==0)
		{
			itemId=atoi(elemValue);
			ListInsertItem(tempCollect.groups,&itemId,END_OF_LIST);
		}	
        free (elemName);
        free (elemValue);
        elemName = elemValue = NULL;
        /* NOTE: Recursion! Make sure stack size can handle input data */
        errChk (ProcessXMLElement (currChildElem,collectList));
        CVIXMLDiscardElement (currChildElem);
        currChildElem = 0;
    }
Error:
    free (elemName);
    free (elemValue);
    CVIXMLDiscardAttribute (currAttr);
    CVIXMLDiscardElement (currChildElem);
    if (FAILED (xmlErr)) {
        char    errBuf[512];
        CVIXMLGetErrorString (xmlErr, errBuf, sizeof (errBuf));
        MessagePopup ("XML Error", errBuf);
    }
    return error;
}

static int ConvertAndRenderXMLFile(const char *filePath,ListType collectList)
{
    int                 error = 0;
    CVIXMLStatus        xmlErr = S_OK;
    CVIXMLDocument      document = 0;
    CVIXMLElement       currElem = 0;
    int                 numCols, itemIndex, len;
    char                *elemName = NULL, *elemValue = NULL;
    errChk (SetWaitCursor (1));
    /* Load document and process it */
    xmlChk (CVIXMLLoadDocument (filePath, &document));
    xmlChk (CVIXMLGetRootElement (document, &currElem));
    xmlChk (CVIXMLGetElementTagLength (currElem, &len));
    nullChk (elemName = malloc (len + 1));
    xmlChk (CVIXMLGetElementTag (currElem, elemName));
    xmlChk (CVIXMLGetElementValueLength (currElem, &len));
    nullChk (elemValue = malloc (len + 1));
    xmlChk (CVIXMLGetElementValue (currElem, elemValue));
    RemoveSurroundingWhiteSpace (elemValue);
	first=1;
    errChk (ProcessXMLElement (currElem,collectList));
	if(first==0)
	{	
	   ListInsertItem(collectList,&tempCollect,END_OF_LIST); 
	}    
Error:
    SetWaitCursor (0);
    free (elemName);
    free (elemValue);
    CVIXMLDiscardElement (currElem);
    CVIXMLDiscardDocument (document);
    if (FAILED (xmlErr)) {
        char    errBuf[512];
        CVIXMLGetErrorString (xmlErr, errBuf, sizeof (errBuf));
        MessagePopup ("XML Error", errBuf);
    }
    return error;
}

ListType getStrategy(void)
{
	SUT s;
	s=GetSeletedSut();
	sprintf(fileName,"%s",s.strategyFile);
	ListType collectList; 
	collectList=ListCreate (sizeof(Collect)); 
	ConvertAndRenderXMLFile(fileName,collectList);   
	return collectList;
}

static int removeStrategyElements(CVIXMLElement element)
{
    int                 error = 0;
    CVIXMLStatus        xmlErr = S_OK;
    int                 numChildElems, len;
    CVIXMLAttribute     currAttr = 0;
    CVIXMLElement       currChildElem = 0;
    char                *elemName = NULL, *elemValue = NULL;
	char count;
    /* Process child elements */
    xmlChk (CVIXMLGetNumChildElements (element, &numChildElems));
   for (int index = 0; index < numChildElems; ++index) {
        xmlChk (CVIXMLGetChildElementByIndex (element, index, &currChildElem));
        xmlChk (CVIXMLGetElementTagLength (currChildElem, &len));
        nullChk (elemName = malloc (len + 1));
        xmlChk (CVIXMLGetElementTag (currChildElem, elemName));
        xmlChk (CVIXMLGetElementValueLength (currChildElem, &len));
        nullChk (elemValue = malloc (len + 1));
        xmlChk (CVIXMLGetElementValue (currChildElem, elemValue));
        RemoveSurroundingWhiteSpace (elemValue);
		if(strcmp(elemName,xmlCollect)==0)
		{
			while(numChildElems>0)
			{
			  xmlChk (CVIXMLGetChildElementByIndex (element, index, &currChildElem));	
		      CVIXMLRemoveElement(currChildElem); 
			  index=0;
			  xmlChk (CVIXMLGetNumChildElements (element, &numChildElems));
			}
			break;
		}else{
            errChk (removeStrategyElements (currChildElem));
		}
		free (elemName);
        free (elemValue);
        elemName = elemValue = NULL;
        CVIXMLDiscardElement (currChildElem);
        currChildElem = 0;
    }

Error:
    free (elemName);
    free (elemValue);
    CVIXMLDiscardAttribute (currAttr);
    CVIXMLDiscardElement (currChildElem);
    if (FAILED (xmlErr)) {
        char    errBuf[512];
        CVIXMLGetErrorString (xmlErr, errBuf, sizeof (errBuf));
        MessagePopup ("XML Error", errBuf);
    }
	return error;
}

static void strategyToXmlElement(CVIXMLElement element,ListType collectList)
{
    CVIXMLElement  currElem = 0;
	CVIXMLElement  currChildElem = 0;  
	CVIXMLElement  currSubChildElem = 0; 
	CVIXMLElement  currSuperChildElem=0;
	int itemCount;
	Collect tempInfo;
	char temp[100]={0};
	int parent;
	int first=1;
	int collectCount=0;
	char tag[MAX_TREE_ITEM_TAG_LEN];  
	int tagInt;
	int groupCount=0;

	for(int index=1;index<=ListNumItems(collectList);index++)
	{
	        ListGetItem(collectList,&tempInfo,index);
	    	CVIXMLNewElement(element,collectCount++,"Collect", &currElem);
	    	//写  CollectName
	    	CVIXMLNewElement(currElem,0,"CollectName",&currChildElem);
	    	CVIXMLSetElementValue (currChildElem, tempInfo.name_);
	    	//写   ResultFrom
	    	sprintf(temp,"%d",tempInfo.resultDerive);
	    	CVIXMLNewElement(currElem,1,"ResultFrom",&currChildElem);
	    	CVIXMLSetElementValue (currChildElem, temp);
	    	memset(temp,100,0);
		    //写   TestType  
	    	sprintf(temp,"%d",tempInfo.testType);
	    	CVIXMLNewElement(currElem,2,"TestType",&currChildElem);
	    	CVIXMLSetElementValue (currChildElem, temp);
	    	memset(temp,100,0);   
	    	//写   MaxTestDevice  
	    	sprintf(temp,"%d",tempInfo.maxParallelDevice);
	    	CVIXMLNewElement(currElem,3,"MaxTestDevice",&currChildElem);
	    	CVIXMLSetElementValue (currChildElem, temp);
	    	memset(temp,100,0);		//

	    	//写   RetryCount  
	    	sprintf(temp,"%d",tempInfo.retryCount);
	    	CVIXMLNewElement(currElem,4,"RetryCount",&currChildElem);
	    	CVIXMLSetElementValue (currChildElem, temp);
	    	memset(temp,100,0);		//			
			
			CVIXMLNewElement(currElem,5,"Groups",&currChildElem); 	
			for(int groupIndex=1;groupIndex<=ListNumItems(tempInfo.groups);groupIndex++)
			{
				int groupId=0;
				ListGetItem(tempInfo.groups,&groupId,groupIndex);
				memset(temp,100,0);
		        sprintf(temp,"%d",groupId);
			    CVIXMLNewElement(currChildElem,groupIndex-1,"GroupId",&currSuperChildElem);
	            CVIXMLSetElementValue (currSuperChildElem, temp);
			}	
	}	
}

static int saveStrategyElements(CVIXMLElement element ,ListType collectList)
{
    int                 error = 0;
    CVIXMLStatus        xmlErr = S_OK;
    int                 index, numChildElems , len;
    CVIXMLAttribute     currAttr = 0;
    CVIXMLElement       currChildElem = 0;
    char                *elemName = NULL, *elemValue = NULL;
	char count;

    /* Process child elements */
    xmlChk (CVIXMLGetNumChildElements (element, &numChildElems));
   for (index = 0; index < numChildElems; ++index) {
        xmlChk (CVIXMLGetChildElementByIndex (element, index, &currChildElem));
        xmlChk (CVIXMLGetElementTagLength (currChildElem, &len));
        nullChk (elemName = malloc (len + 1));
        xmlChk (CVIXMLGetElementTag (currChildElem, elemName));
        xmlChk (CVIXMLGetElementValueLength (currChildElem, &len));
        nullChk (elemValue = malloc (len + 1));
        xmlChk (CVIXMLGetElementValue (currChildElem, elemValue));
        RemoveSurroundingWhiteSpace (elemValue);
		if(strcmp(elemName,xmlCollects)==0)
		{
		    strategyToXmlElement(currChildElem,collectList);	
		}
		saveStrategyElements(currChildElem,collectList);
		free (elemName);
        free (elemValue);
        elemName = elemValue = NULL;
        CVIXMLDiscardElement (currChildElem);
        currChildElem = 0;
    }
Error:
    free (elemName);
    free (elemValue);
    CVIXMLDiscardAttribute (currAttr);
    CVIXMLDiscardElement (currChildElem);
    if (FAILED (xmlErr)) {
        char    errBuf[512];
        CVIXMLGetErrorString (xmlErr, errBuf, sizeof (errBuf));
        MessagePopup ("XML Error", errBuf);
    }
	return error;
}

static void saveStrategyToXml(char *filePath,ListType collectList)
{
    int                 error = 0;
    CVIXMLStatus        xmlErr = S_OK;
    CVIXMLDocument      document = 0;
    CVIXMLElement       currElem = 0;
    int                 numCols, itemIndex, len;
    char                *elemName = NULL, *elemValue = NULL;
	CVIXMLElement       currChildElem = 0; 
	int numChildElems=0,index=0;

    errChk (SetWaitCursor (1));

    /* Load document and process it */
    xmlChk (CVIXMLLoadDocument (filePath, &document));
    xmlChk (CVIXMLGetRootElement (document, &currElem));

    xmlChk (CVIXMLGetElementTagLength (currElem, &len));
    nullChk (elemName = malloc (len + 1));
    xmlChk (CVIXMLGetElementTag (currElem, elemName));
    xmlChk (CVIXMLGetElementValueLength (currElem, &len));
    nullChk (elemValue = malloc (len + 1));
    xmlChk (CVIXMLGetElementValue (currElem, elemValue));
    RemoveSurroundingWhiteSpace (elemValue);
    removeStrategyElements(currElem);   //step1 先删除所有数据 
	saveStrategyElements(currElem,collectList); //step2 保存数据
Error:
    SetWaitCursor (0);
    free (elemName);
    free (elemValue);
	CVIXMLSaveDocument (document, 0, NULL);
    CVIXMLDiscardElement (currElem);
    CVIXMLDiscardDocument (document);
    if (FAILED (xmlErr)) {
        char    errBuf[512];
        CVIXMLGetErrorString (xmlErr, errBuf, sizeof (errBuf));
        MessagePopup ("XML Error", errBuf);
    }
}

void printfCollectList(ListType collectList)
{
	printf("-------------------------------------------listCount=%d\n",ListNumItems(collectList));
	for(int i=1;i<=ListNumItems(collectList);i++)
	{	
	   Collect collect;
	   ListGetItem(collectList,&collect,i);
	   printf("name=%s,type=%d,resultDerive=%d,maxDevice=%d\n",collect.name_,collect.testType,collect.resultDerive,collect.maxParallelDevice);
	   for(int index=1;index<=ListNumItems(collect.groups);index++)
	   {
		   int id=0;
		   ListGetItem(collect.groups,&id,index);
	       printf("itemId=%d\n",id);
	   }	   
	}    
}

void saveStrategy(int panelHandle,ListType collectList)
{
    saveStrategyToXml(fileName,collectList);
}





