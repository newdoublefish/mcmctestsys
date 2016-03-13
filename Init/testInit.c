/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：TestInit.c
 // 文件功能描述:
 //
 // 
 // 创建标识：曾衍仁 20150815

 //-------------------------------------------------------------------------*/
#include <cvirte.h>		
#include <userint.h>
#include <pathctrl.h>
#include "cvixml.h"        
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "testGroupInit.h"  
#include "sutCommon.h"
#include "protocolHelper.h"
#include "collectParamInit.h"
#include "log.h"
#include "common.h"
#include "frameInit.h"
#include "tpsHelper.h"



static HRESULT showProgressValidity(int panel,char *itemName,int perc,char *status)
{
	char description[100]={0};
  
	if(status!=NULL)
	{	
	   Fmt(description,"%s      %s\n",itemName,status);
	   slideProgressShowWithTextBox(panel,perc,description,0.01);
	}else
	{
	   slideProgressShowWithTextBox(panel,perc,NULL,0.01);
	}	
	
	return 0;
}

BOOL validityTestGroup()
{
	BOOL ret=FALSE;
	int perc; 
	CollectParam collectParam; 
    ListType testCaseList=getTestCaseList();
	HashTableType collectHashTable=getCollcetParamHashType();
	int panel=displaySlideProgressWithTextBox("校验采集条例表");
	int count=ListNumItems(testCaseList);
	slideProgressShowWithTextBox(panel,-1,"开始校验采集条例表\n",1);
	for(int i=1;i<=count;i++)
	{
			int found;
		    TestItem testItem;
			ListGetItem(testCaseList,&testItem,i);
			HashTableFindItem(collectHashTable,testItem.itemName_,&found); 
			perc=i*100/count; 
			if(found!=1)
			{	
			   //printf("itemName:%s,found:%d\n",testItem.itemName_,found);
			   showProgressValidity(panel,testItem.itemName_,perc,"erro 参数名称错误");
			   ret=-1;
			}else
			{
			    HashTableGetItem(collectHashTable,testItem.itemName_,&collectParam,sizeof(CollectParam)); 
				if(collectParam.id!=testItem.paramId)
				{
				    showProgressValidity(panel,testItem.itemName_,perc,"erro 参数ID错误");  
				}	
			}	
			showProgressValidity(panel,testItem.itemName_,perc,NULL); 
			
			
	}
	slideProgressShowWithTextBox(panel,-1,"校验采集条例表完成\n",1);  
	if(ret<0)
	{
	   disposeSlideProgressWithButton(panel); 
	}else{	
	   disposeSlideProgress(panel);
	}   
	return ret;
}


HRESULT testInit(SUT selectedsut)
{
	//printf("%s",selectedsut.systemName);
	if(strcmp(selectedsut.systemName,"1305")==0)
	{	
#if 0	
	if(collectParamInit(selectedsut.configPath)<0)
	{
	   LOG_EVENT(LOG_ERROR,"collectParam analyse error");   
	   return -1;
	}
	//return -1; 
	initFrame();
	
	//CollectParam param = getCollectParamByHashTable("模拟电压量6(桨叶动载荷值)");
	//printf("id:%d\n",param.id);
#endif
	}
	
	
	
    if(testGroupInit(selectedsut.configPath)<0)   //解析测试用例表
	{
		
		LOG_EVENT(LOG_ERROR,"testCase analyse error");   
	    return -1;
	}
	
	
	
	/*
	if(validityTestGroup()<0)
	{
	    //return -1;
	}*/	
	
	initTps();
#if 1	
	if(protocolInit()<0)                   //解析协议
	{
		LOG_EVENT(LOG_ERROR,"protocol analyse error"); 
	    return -1;
	}	
#endif
	//ManualItemLen = ManualItemInit(project.configPath,ManuItem);	
	//StatusPanelInit (LRUstatus); 	
	
	LOG_EVENT(LOG_INFO,"TEST init success!");
	
	return TRUE;
}

HRESULT testDeinit()
{
	//todo:释放空间
	testGroupDeinit();
    return TRUE;
}
