/******************************************************************************

                  版权所有 (C), 2014-2020, 广州航新第一设计所                  

 ******************************************************************************/
/** @file    
 *  @version 初稿
 *  @author  曾衍仁
 *  @date    2015.1.22
 *  @brief   此文件主要是负责被测系统实体操作，包括被测系统链表的维护，被测系统t数据库存储，读取等操作。
 *  @note    修订记录:

 ******************************************************************************/

#include <cvirte.h>		
#include <userint.h>
#include <pathctrl.h>
#include "cvixml.h"        
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "sutEntity.h"
#include "debug.h"

static char *configFile="\\config\\Config.xml";  
static char *config="\\config\\project\\";
static SUT project;

static void XMLtoSut(CVIXMLElement currElem,SUTCONFIG *sutConfigPtr)
{
	int len,numChildElems,index ;
	CVIXMLElement       currChildElem = 0;
    char  *elemName = NULL, *elemValue = NULL;	
	CVIXMLGetNumChildElements (currElem, &numChildElems);     
	for(index=0;index<numChildElems;index++)
	{
	   CVIXMLGetChildElementByIndex (currElem, index, &currChildElem);
	   CVIXMLGetElementTagLength (currChildElem, &len);
	   elemName = malloc (len + 1);
	   CVIXMLGetElementTag (currChildElem, elemName);
	   CVIXMLGetElementValueLength (currChildElem, &len);
	   elemValue = malloc (len + 1);
	   CVIXMLGetElementValue (currChildElem, elemValue);
	   if(strcmp(elemName,"Version")==0)
	   {
	       //PRINT("Version:%s",elemValue);
		   strcpy(sutConfigPtr->version,elemValue); 
		   //PRINT("Version:%s",sutConfigPtr->version);
	   }else if(strcmp(elemName,"ProjectName")==0)
	   {
		  memset(&project,0,sizeof(SUT));
	      strcpy(project.systemName,elemValue);
	   }else if(strcmp(elemName,"ConfigPath")==0)
	   {
		  GetProjectDir(project.configPath);
		  strcat(project.configPath,config);
	   	  strcat(project.configPath,elemValue);  
	   }else if(strcmp(elemName,"StrategyPath")==0)
	   {
		  GetProjectDir(project.strategyFile);
		  strcat(project.strategyFile,config);
	   	  strcat(project.strategyFile,elemValue);  
		  
	   }else if(strcmp(elemName,"ReportPath")==0)
	   {
		  GetProjectDir(project.reportFilePath);
		  strcat(project.reportFilePath,config);
	   	  strcat(project.reportFilePath,elemValue);  
		  ListInsertItem(sutConfigPtr->sutList,&project,END_OF_LIST);
		  
	   }
	   XMLtoSut(currChildElem,sutConfigPtr);
	   free (elemName);
       free (elemValue);
	   
	}  
	CVIXMLDiscardElement (currElem);
}

void printEutList(ListType list)
{
   for(int i=1;i<=ListNumItems(list);i++)
   {
       SUT sut;
	   ListGetItem(list,&sut,i);
	   PRINT("name:%s,configPath:%s,strategyPath:%s,reportPath:%s",sut.systemName,sut.configPath,sut.strategyFile,sut.reportFilePath);
	   
   }	   
}

/** @brief  获取被测系统链表    
 *  @param[in]  无
 *  @param[out] 无
 *  @return     int
 *  @note       修订记录  :
*/

SUTCONFIG GetSystemConfig(void)
{
	char fileName[MAX_PATHNAME_LEN];
	SUTCONFIG sutConfig;
	memset(&sutConfig,0,sizeof(SUTCONFIG));
	//sutConfig.sutList=0;
	SUT temp;
	int len,numChildElems,index ;
	CVIXMLDocument      document = 0;
    CVIXMLElement       currElem = 0;
    CVIXMLAttribute     currAttr = 0;
    CVIXMLElement       currChildElem = 0;
    char  *elemName = NULL, *elemValue = NULL;	
	sutConfig.sutList=ListCreate (sizeof(SUT));
	
	GetProjectDir (fileName);
	strcat(fileName,configFile);
    CVIXMLLoadDocument (fileName, &document);
    CVIXMLGetRootElement (document, &currElem);

	XMLtoSut(currElem,&sutConfig);
	
    CVIXMLDiscardDocument (document);
	//printEutList(sutConfig.sutList);
	return sutConfig;
}



 

