/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ㶫������綯����Ӫ�ɷ����޹�˾��Ȩ���У�δ����Ȩ��
 // ��ֹ���������п�����ʹ�á�
 //
 // �ļ�����
 // �ļ���������: 
 //
 // 
 // ������ʶ�������� 
 //
 // �޸ı�ʶ��
 // �޸�������
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
#include "tipsParse.h"



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
	int panel=displaySlideProgressWithTextBox("У��ɼ�������");
	int count=ListNumItems(testCaseList);
	slideProgressShowWithTextBox(panel,-1,"��ʼУ��ɼ�������\n",1);
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
			   showProgressValidity(panel,testItem.itemName_,perc,"erro �������ƴ���");
			   ret=-1;
			}else
			{
			    HashTableGetItem(collectHashTable,testItem.itemName_,&collectParam,sizeof(CollectParam)); 
				if(collectParam.id!=testItem.paramId)
				{
				    showProgressValidity(panel,testItem.itemName_,perc,"erro ����ID����");  
				}	
			}	
			showProgressValidity(panel,testItem.itemName_,perc,NULL); 
			
			
	}
	slideProgressShowWithTextBox(panel,-1,"У��ɼ����������\n",1);  
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
	
	//CollectParam param = getCollectParamByHashTable("ģ���ѹ��6(��Ҷ���غ�ֵ)");
	//printf("id:%d\n",param.id);
#endif
	}
	
	
	
    if(testGroupInit(selectedsut.configPath)<0)   //��������������
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
	
	getTipsFromExcel();
#if 0	
	if(protocolInit()<0)                   //����Э��
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
	//todo:�ͷſռ�
	testGroupDeinit();
    return TRUE;
}
