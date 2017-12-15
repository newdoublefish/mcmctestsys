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
#include <userint.h>
#include "excelHelper.h" 
#include "sutCommon.h"
#include "convertTool.h"
#include "eutConfig.h"

static HashTableType tipsHashTable=0;
#define SHEET_RANGE_TIPS "A2:C2" 


static HRESULT onCellListenerGetTips(VARIANT *MyVariant,int row,int column)    
{
	char *temp;
	static char key[50]={0}; 
	if(column==1)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			//memcpy(tempInstruction.name,temp,strlen(temp)+1);
			memset(key,0,50);
			memcpy(key,temp,strlen(temp)+1);
			//printf("%s,",temp);
			CA_FreeMemory(temp);
		 }else
		 {
			return EXCEL_TASK_QUIT;
		 }
	}else if(column==2)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
		    //tempInstruction.operation.testType=(unsigned char)StrToUnsignedLong(temp);   
			if(tipsHashTable!=0)
			{
			   HashTableInsertItem(tipsHashTable,key,temp);
			}
			//printf("%s\n",temp); 
			CA_FreeMemory(temp);
		 }
	}
	return EXCEL_TASK_OK;
}


static HRESULT onStartGetTips(VARIANT *MyVariant,int row,int column)				
{
	//istList=ListCreate(sizeof(INSTRUCTION));
	HashTableCreate(10,C_STRING_KEY,0,80,&tipsHashTable);
    return 0;	
}

void getTipsFromExcel()   
{
	
	if(tipsHashTable==0)
	{	
	   SUT sut=GetSeletedSut();
	   EXCELTask task=createExcelTask(sut.configPath,"提示",SHEET_RANGE_TIPS,3);
	   task.onExcelTaskStartListener=(void *)onStartGetTips;
	   task.onCellListener=(void *)onCellListenerGetTips;
	   runExcelTask(task);
	}   
}

HRESULT getTip(char *key,char *value)
{
   int found=0; 
   getTipsFromExcel(); 
   HashTableFindItem(tipsHashTable,key,&found);
   if(found==1)
   {
   	   HashTableGetItem(tipsHashTable,key,value,80);
   }
   return found;
}

