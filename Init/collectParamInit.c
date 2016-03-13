/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：collectParamInit.c
 // 文件功能描述:
 //
 // 
 // 创建标识：曾衍仁 20141114
 //
 // 修改标识：曾衍仁  20150804
 // 修改描述：增加excel处理任务
 //-------------------------------------------------------------------------*/
#include "excel2000.h"    
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "convertTool.h"
#include "common.h"
#include "SutPanel.h"
#include "excelHelper.h"
#include "configPath.h"
#include "log.h"
#include "collectParamInit.h"
#include "sutCommon.h"

#define COLLECT_PARAM_SHEET "A2:G2"
static int collectParamCount;
static int tempCollectParamCount;
static HashTableType collectParamHashTable=0;
static ListType collectParamList=0;
static CollectParam collectParam;
static int slidePanelCollectParamHandle;

static HRESULT callbackForCollectParamCount(VARIANT *MyVariant,int row,int column)
{
	HRESULT ret=0;
	if(column==2) //参数名称
	{
				
		if(CA_VariantIsEmpty(MyVariant))
		{
			return -1;
		}else
		{
		    collectParamCount++;
		}	
	}
	return ret;
}


static HRESULT  onExcelTaskStartListenerCollectParam(void)
{
	
	tempCollectParamCount=0;
	if(0==collectParamHashTable)
	{
	    HashTableCreate(10,C_STRING_KEY,0,sizeof(CollectParam),&collectParamHashTable);  
	}else
	{
	    HashTableDispose(collectParamHashTable);
		HashTableCreate(10,C_STRING_KEY,0,sizeof(CollectParam),&collectParamHashTable);  
	}
	
	if(0==collectParamList)
	{
	    collectParamList=ListCreate(sizeof(CollectParam));
	}
	
	
	
	return 0;
}

static HRESULT showProgressCollectParam(char *itemName,int error)
{
	
	int perc;
	   
	tempCollectParamCount++; 
    perc=tempCollectParamCount*100/collectParamCount;

	
	if(error>0)
	{	
	   char description[100]={0};
	   Fmt(description,"%s     %s\n",itemName,"error");
	   slideProgressShowWithTextBox(slidePanelCollectParamHandle,perc,description,0.01);
	}else{
	   slideProgressShowWithTextBox(slidePanelCollectParamHandle,perc,NULL,0.01);	
	}
	return 0;
}

static HRESULT onCellListenerCollectParam(VARIANT *MyVariant,int row,int column)   
{
	 static quitFlag=0;
	 char *temp;
 	 if(column==0) //组名称
	 {
		
		if(CA_VariantHasCString(MyVariant))
		{	
			memset(collectParam.type,0,MAX_COLLECT_PARAM_NAME_LEN); 
			CA_VariantGetCString(MyVariant, &temp);
			memcpy(collectParam.type,temp,strlen(temp)+1); //组名赋值 
			CA_FreeMemory(temp);
        }
				
	 }else if(column==1) //参数Id
	 {
		 
	    if(CA_VariantHasDouble(MyVariant))
	    {
			double tt;
		    CA_VariantGetDouble(MyVariant,&tt);
			//sprintf(collectParam.id,"%d",(int)tt);
			collectParam.id=(int)tt;
		}
	 }else if(column==2) //param name
	 {
		if(CA_VariantIsEmpty(MyVariant))
		{
			quitFlag=1;
			return EXCEL_TASK_QUIT;  //如果没有参数了，则认为可以退出
		}		 
		if(CA_VariantHasCString(MyVariant))
		{	
			memset(collectParam.name,0,MAX_COLLECT_PARAM_NAME_LEN); 
			CA_VariantGetCString(MyVariant, &temp);
			memcpy(collectParam.name,temp,strlen(temp)+1); //组名赋值 
			CA_FreeMemory(temp);
        }
				
	 }else if(column==3) //paramLen
	 {
	    if(CA_VariantHasDouble(MyVariant))
	    {
			double tt;
		    CA_VariantGetDouble(MyVariant,&tt);
			collectParam.paramLen=(int)tt;
		}
	 }else if(column==4) //storeLen
	 {
	    if(CA_VariantHasDouble(MyVariant))
	    {
			double tt;
		    CA_VariantGetDouble(MyVariant,&tt);
			collectParam.storeLen=(int)tt; 
		}
	 }else if(column==5) //rate
	 {
	    if(CA_VariantHasDouble(MyVariant))
	    {
			double tt;
		    CA_VariantGetDouble(MyVariant,&tt);
			collectParam.rate=(int)tt; 
		}

		
	 }else if(column==6)
	 {
	    if(CA_VariantHasDouble(MyVariant))
	    {
			double tt;
		    CA_VariantGetDouble(MyVariant,&tt);
			collectParam.dataType=(int)tt; 
			if(collectParam.dataType==429)
			{
			    collectParam.dataType=1;
			}else
			{
			    collectParam.dataType=0;
			}
		}		 
	    HashTableInsertItem(collectParamHashTable,collectParam.name,&collectParam);
		ListInsertItem(collectParamList,&collectParam,END_OF_LIST);
		showProgressCollectParam(collectParam.name,0);//显示进度 
	 }	 
	 return EXCEL_TASK_OK; 
}

static HRESULT onFinishListenerCollectParam()
{

	  
	  collectParamCount=0;
	  //printGroupList();
	  return 0;
} 

static void listCollectParams(HashTableType table)
{
   int status = 0;
   HashTableIterator iter;
   char paramName[30];
   CollectParam param;

   /* ... */

   for (status = HashTableIteratorCreate(table, &iter);status >= 0 && status != HASH_TABLE_END;status = HashTableIteratorAdvance(table, iter)) 
   {
     status = HashTableIteratorGetItem(table, iter, &paramName, MAX_COLLECT_PARAM_NAME_LEN, &param, sizeof(CollectParam)); 
	 printf("type:%s,id:%d,name:%s,paramLen:%d,storeLen:%d,reate:%d\n",param.type,param.id,param.name,param.paramLen,param.storeLen,param.rate);
/* do something with key and value */ 

   }
   HashTableIteratorDispose(table, iter); 
}



HRESULT collectParamInit(char *filePath)
{
	HRESULT error = 0;
	
	slidePanelCollectParamHandle=displaySlideProgressWithTextBox("正在分析参数采集表"); 
	slideProgressShowWithTextBox(slidePanelCollectParamHandle,-1,"开始分析采集表\n",1);
	
	EXCELTask task1=createExcelTask(filePath,"飞参采集表",COLLECT_PARAM_SHEET,6); 
	task1.onCellListener=(void *)callbackForCollectParamCount;
	if(runExcelTask(task1)<0)  //获取到测试条例的数量
	{	
		return -1;
	}	
	

	EXCELTask task2=createExcelTask(filePath,"飞参采集表",COLLECT_PARAM_SHEET,7); 
	task2.onExcelTaskStartListener=(void *)onExcelTaskStartListenerCollectParam;
	task2.onCellListener=(void *)onCellListenerCollectParam;
	//task2.onRowListener=(void *)onRowListenerTestCase;
	task2.onExcelTaskFinishListener=(void *)onFinishListenerCollectParam; 
	if(runExcelTask(task2)<0)  //获取到测试条例的数量	
	{
	    return -1;
	}
	
	//listCollectParams(collectParamHashTable);
	slideProgressShowWithTextBox(slidePanelCollectParamHandle,-1,"采集表分析完成\n",1);
	disposeSlideProgress(slidePanelCollectParamHandle); 
	
    //printf("count:%d\n",ListNumItems(collectParamList));
	
    return error;
}

CollectParam getCollectParamByHashTable(char *name)
{
	    CollectParam param;
		int found;
		memset(&param,0,sizeof(CollectParam));
	    HashTableFindItem(collectParamHashTable,name,&found);
	    if(found==1)
	    {
	      ListType valueList;
		  HashTableGetItem(collectParamHashTable,name,&param,sizeof(CollectParam)); 
	    }
		return param;
}


HashTableType getCollcetParamHashType(void)
{
    return collectParamHashTable;
}

ListType getCollectParamList(void)
{
    return  collectParamList;
}
