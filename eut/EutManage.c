#include "EutManage.h"
#include "toolbox.h"   
#include "EutManagePanel.h"
#include "sutCommon.h"  

ListType eutList;
ListType activityList;
//static char *deviceFile="\\Device.xml"; 
static char *deviceFile="\\config\\Device.xml";  

extern ConfigActivity RS422ConfigActivity(void);  
extern ConfigActivity BasicConfigActivity(void); 
extern ConfigActivity Relay422ConfigActivity(void); 
extern ConfigActivity NetConfigActivity(void);

#ifndef xmlChk
#define xmlChk(f) if (xmlErr = (f), FAILED (xmlErr)) goto Error; else
#endif

static ACTIVITY_FUNCTION activityFunctionGroup[]=
{
	BasicConfigActivity, 	
	RS422ConfigActivity,
	NetConfigActivity,
};

typedef struct{
	char name[20];
	ACTIVITY_FUNCTION func;
}tConfig;

static tConfig tConfigGroup[]={
	/*{"基本配置",BasicConfigActivity},
	{"安规测量仪",RS422ConfigActivity},
	{"继电器",RS422ConfigActivity}, 
	{"BMS模拟器",RS422ConfigActivity}, 	
	{"充电桩网络",NetConfigActivity},
	{"k60串口",RS422ConfigActivity}, 
	{"k64串口",RS422ConfigActivity},*/
	{"Basic",BasicConfigActivity},
	{"Serial",RS422ConfigActivity},
	{"Net",NetConfigActivity}, 
};



int getActivity(char *name,tActivity *tActivityPtr)
{
	 for(int i=1;i<=ListNumItems(activityList);i++)
	 {
	 	ListGetItem(activityList,tActivityPtr,i);
		if(strcmp(name,tActivityPtr->name)==0)
		{
			 return 1;
		}
		
	 }
	 return 0;
}

ListType ParseXmlElementsToListMap(CVIXMLElement currElem)
{
	ListType list=createMapList();
	int numberAttributes=0;
	CVIXMLGetNumAttributes (currElem,&numberAttributes);
	for(int attributeIndex=0;attributeIndex<numberAttributes;attributeIndex++)
	{
		Map map={0};
		CVIXMLAttribute attribute;
		CVIXMLGetAttributeByIndex (currElem,attributeIndex,&attribute);	
		CVIXMLGetAttributeName (attribute,map.key);
		CVIXMLGetAttributeValue (attribute,map.value);
		CVIXMLDiscardAttribute(attribute);
		addToMapList(list,map); 
	}
	/*int len,numChildElems,index;
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
	   Map map;
	   sprintf(map.key,"%s",elemName);
	   sprintf(map.value,"%s",elemValue);
	   
	   addToMapList(list,map);
	   free (elemName);
       free (elemValue);
	}*/
		
	
	return list;	
}

void ParseXML2EutList(CVIXMLElement currElem)
{
    CVIXMLDocument      document = 0;
    CVIXMLElement       currChildElem = 0;  
    int                 len,numChildElems;
    char                *elemName = NULL, *elemValue = NULL;
	EEUT eut;
	memset(&eut,0,sizeof(EEUT));
	eut.configList=ListCreate(sizeof(ConfigItem));
	
	CVIXMLGetNumChildElements (currElem, &numChildElems); 
	for(int i=0;i<numChildElems;i++)
	{
	//printf("%d\n",i);
	   CVIXMLGetChildElementByIndex (currElem, i, &currChildElem);
	   CVIXMLGetElementTagLength (currChildElem, &len);
	   elemName = malloc (len + 1);
	   CVIXMLGetElementTag (currChildElem, elemName);
	   CVIXMLGetElementValueLength (currChildElem, &len);
	   elemValue = malloc (len + 1);
	   CVIXMLGetElementValue (currChildElem, elemValue);
	   
	   //ConfigActivity activity;
	   tActivity ta={0};
	   if(getActivity(elemName,&ta)>0)
	   {
			ConfigItem item={0};
			sprintf(item.tagName,"%s",elemValue);
			sprintf(item.typeName,"%s",elemName); 
			item.configPtrHandle =(*(ON_CREATE_CONFIG)(ta.activity.onCreate))();
			ListType list=ParseXmlElementsToListMap(currChildElem);
			//(*(LOAD_EUT_SUB_CONFIG)activity.loadConfig)(currChildElem,item.configPtrHandle);
			(*(ON_LOAD_CONFIG)ta.activity.onLoadConfig)(list,item.configPtrHandle);
			disposeMapList(list);
			ListInsertItem(eut.configList,&item,END_OF_LIST);
				
	   }
	   //printf("%d\n",ListNumItems(eut.configList));
	   free (elemName);
       free (elemValue);
	   CVIXMLDiscardElement (currChildElem); //这里要释放掉
	   
	}
	ListInsertItem(eutList,&eut,END_OF_LIST);
}

void LoadEutListFromXml(ListType eutList,char *filePath)
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

	CVIXMLGetNumChildElements (currElem, &numChildElems); 
	for(int i=0;i<numChildElems;i++)
	{
	   CVIXMLGetChildElementByIndex (currElem, i, &currChildElem);
	   CVIXMLGetElementTagLength (currChildElem, &len);
	   elemName = malloc (len + 1);
	   CVIXMLGetElementTag (currChildElem, elemName);
	   CVIXMLGetElementValueLength (currChildElem, &len);
	   elemValue = malloc (len + 1);
	   CVIXMLGetElementValue (currChildElem, elemValue);
	   //printf("%s\n",elemName);
	   ParseXML2EutList(currChildElem);
	   CVIXMLDiscardElement(currChildElem);  //这一步必须要执行
	}
	free(elemName);
	free(elemValue);
	
	CVIXMLDiscardElement (currElem); 
	CVIXMLDiscardDocument (document);
}


void InitActivity()
{
	if(activityList==0)
	{
		activityList=ListCreate(sizeof(tActivity));
		/*for(int i=0;i<sizeof(activityFunctionGroup)/sizeof(ACTIVITY_FUNCTION);i++)
		{
			ConfigActivity activity=(*(activityFunctionGroup[i]))();
		 	ListInsertItem(activityList,&activity,END_OF_LIST);				
		}*/
		for(int i=0;i<sizeof(tConfigGroup)/sizeof(tConfig);i++)
		{
			tActivity ta={0};
			sprintf(ta.name,"%s",tConfigGroup[i].name);
			ta.activity=(*(tConfigGroup[i].func))();
		 	ListInsertItem(activityList,&ta,END_OF_LIST);				
		}				
	}
}

void LoadEutList()
{
	if(eutList==0)
	{
		eutList=ListCreate(sizeof(EEUT));
	}

	InitActivity();
	char fileName[MAX_PATHNAME_LEN];
	//GetProjectDir (fileName);
	//strcat(fileName,deviceFile);
	SUT s;
	s=GetSeletedSut();
	sprintf(fileName,"%s",s.deviceFile);	
	LoadEutListFromXml(eutList,fileName);
}

ListType getEEutList()
{
	if(eutList==0)
	{
		LoadEutList();
	}
	
	return eutList;
}

/** @brief XML删除UUT
 *  @param[in]  无
 *  @param[out] 无
 *  @return     int
 *  @note       修订记录  :
*/

static int removeEutElement(CVIXMLElement element)
{
    int                 error = 0;
    CVIXMLStatus        xmlErr = S_OK;
    int                 index, numAttrs, numChildElems, childItemIndex, len;
    CVIXMLAttribute     currAttr = 0;
    CVIXMLElement       currChildElem = 0;
    
	char count;

    /* Process child elements */
   CVIXMLGetNumChildElements (element, &numChildElems);
   for (index = 0; index < numChildElems; ++index) {
	    char  *elemName = NULL; 
        CVIXMLGetChildElementByIndex (element, 0, &currChildElem);
        CVIXMLGetElementTagLength (currChildElem, &len);
        elemName = malloc (len + 1);
        CVIXMLGetElementTag (currChildElem, elemName);
        CVIXMLGetElementValueLength (currChildElem, &len);
		if(strcmp(elemName,"Device")==0)
		{
			CVIXMLRemoveElement(currChildElem); 
		}
		free (elemName);
        elemName  = NULL;
		
        CVIXMLDiscardElement (currChildElem);
        currChildElem = 0;
    }
	return error;
}

void saveListMapToXml(ListType list,CVIXMLElement currElem)
{
	CVIXMLElement  currChildElem = 0; 
	Map map;
	for(int i=1;i<=ListNumItems(list);i++)
	{
		ListGetItem(list,&map,i);
		CVIXMLAddAttribute(currElem,map.key,map.value);
	}
}

void saveEut2Xml(EEUT eut,CVIXMLElement currElem)
{
	
	ConfigItem item={0};  
	for(int j=1;j<=ListNumItems(eut.configList);j++)
	{
		ListGetItem(eut.configList,&item,j);
		//ConfigActivity activity;
		tActivity ta={0};
		//getActivity(item.tagName,&ta);
		//ListGetItem(activityList,&ta,j);
	   	if(getActivity(item.typeName,&ta)>0)
	   	{
			CVIXMLElement currChildElem; 
			CVIXMLNewElement(currElem,j-1,ta.activity.configName, &currChildElem); 
			//printf("configName:%s\n",item.tagName);
			//item.configPtrHandle = (*(SAVE_EUT_SUB_CONFIG)activity.saveConfig)(currChildElem,item.configPtrHandle);
			ListType mapList=createMapList();
			(*(ON_SAVE_CONFIG)ta.activity.onSaveConfig)(mapList,item.configPtrHandle);
			saveListMapToXml(mapList,currChildElem);
			disposeMapList(mapList);
			CVIXMLSetElementValue (currChildElem,item.tagName);
			CVIXMLDiscardElement(currChildElem);
				
	   	}
			
	}
}

void addNewToEutList()
{
	/*if(ListNumItems(eutList)==0)
	{
		eutList=ListCreate(sizeof(EEUT));	
	}*/
	
	EEUT eut={0};
	if(ListNumItems(eutList)==0)
	{
		return;
	}
	
	ListGetItem(eutList,&eut,1);
	
	EEUT eutNew={0};
	eutNew.configList=ListCreate(sizeof(ConfigItem));
	
	for(int i=1;i<=ListNumItems(eut.configList);i++)
	{
		tActivity ta={0};
		ConfigItem item={0}; 
		ListGetItem(eut.configList,&item,i);
		if(0==getActivity(item.typeName,&ta))
		{
			continue;
		}
		ConfigItem itemNew={0};
		sprintf(itemNew.tagName,"%s",item.tagName);
		sprintf(itemNew.typeName,"%s",item.typeName);
		itemNew.configPtrHandle =(*(ON_CREATE_CONFIG)(ta.activity.onCreate))();
		ListInsertItem(eutNew.configList,&itemNew,END_OF_LIST);		
	
	}
	
	ListInsertItem(eutList,&eutNew,END_OF_LIST);
/*	
	EEUT eut={0};
	eut.configList=ListCreate(sizeof(ConfigItem));
		
	for(int i=1;i<=ListNumItems(activityList);i++)
	{
			tActivity ta={0};
			ConfigItem item={0};  
			ListGetItem(activityList,&ta,i);
			sprintf(item.tagName,"%s",ta.name);
			item.configPtrHandle =(*(ON_CREATE_CONFIG)(ta.activity.onCreate))();
			ListInsertItem(eut.configList,&item,END_OF_LIST);
	}
	ListInsertItem(eutList,&eut,END_OF_LIST);
*/
}

void saveEutList()
{
	/*char fileName[MAX_PATHNAME_LEN];
	GetProjectDir (fileName);
	strcat(fileName,deviceFile);*/
	char fileName[MAX_PATHNAME_LEN];
	//GetProjectDir (fileName);
	//strcat(fileName,deviceFile);
	SUT s;
	s=GetSeletedSut();
	sprintf(fileName,"%s",s.deviceFile);	
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

	removeEutElement(currElem);
	for(int i=1;i<=ListNumItems(eutList);i++)
	{
		CVIXMLNewElement(currElem,i-1,"Device", &currChildElem); 
		EEUT eut;
		ListGetItem(eutList,&eut,i);
		saveEut2Xml(eut,currChildElem);
		CVIXMLDiscardElement (currChildElem); 
	}
	
    free (elemName);
    free (elemValue);
	CVIXMLSaveDocument (document, 0, NULL);
    CVIXMLDiscardElement (currElem);
    CVIXMLDiscardDocument (document);	
}

ConfigActivity newConfigActivity()
{
	ConfigActivity activity={0};
	return activity;
}





