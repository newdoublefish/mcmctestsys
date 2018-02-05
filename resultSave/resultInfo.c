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
#include "resultInfo.h"
#include "settingEntity.h" 
#include "testGroupInit.h"
#include "resultUtil.h"

BOOL loadResultInfo(HashTableType hashTable){

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
	for(int i=1;i<=itemCnt;i++)
	{
		int found=0;
		RESULT itemResult;
		if(HashTableFindItem(hashTable,&i,&found)>=0)
		{
			if(found>0)
			{
				HashTableGetItem(hashTable,&i,&itemResult,sizeof(RESULT));
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

BOOL saveResultInfo(char *deviceName,HashTableType hashTable){
	char fileName[MAX_PATHNAME_LEN];
	int fileSize=0;
	SETTING s=getSetting();
	GetProjectDir (fileName);
	
	if(FileExists(s.saveDir,&fileSize)==0)
	{	
	    MakeDir(s.saveDir);
	}

    sprintf(fileName,"%s\\%s",s.saveDir,deviceName); 	
	if(FileExists(fileName,&fileSize)==0) //不存在
	{	
	    MakeDir(fileName);
	}
	
	sprintf(fileName,"%s\\%s%s",fileName,deviceName,".xml");
	
    CVIXMLDocument      document = 0;
    CVIXMLElement       currElem = 0 ,currChildElem = 0;  
    int                 len,numChildElems;
    char                *elemName = NULL, *elemValue = NULL;
	CVIXMLNewDocument("Data",&document);
    CVIXMLGetRootElement (document, &currElem);
	CVIXMLNewElement(currElem,-1,"Device", &currChildElem);
	
	CVIXMLAddAttribute(currChildElem,"name",deviceName);
	
	saveResultToXml(currChildElem,hashTable);

	
    free (elemName);
    free (elemValue);
	CVIXMLSaveDocument (document, 0, fileName);
    CVIXMLDiscardElement (currChildElem);
	CVIXMLDiscardElement (currElem); 
    CVIXMLDiscardDocument (document);	
	return TRUE;
}
