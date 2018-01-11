#include <userint.h>
#include "excelHelper.h" 
#include "toolbox.h"  
#include "BiboExcelParse.h"
#include "sutCommon.h" 
#include "debug.h"

#define SHEET_RANGE_TIPS "A2:C2" 
ListType gBiboList;

static HRESULT onCellListenerGetBibo(VARIANT *MyVariant,int row,int column)    
{
	char *temp;
	static tBIBO bibo;
	if(column==0)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			//memcpy(tempInstruction.name,temp,strlen(temp)+1);
			memset(&bibo,0,sizeof(tBIBO));
			memcpy(bibo.paramName,temp,strlen(temp)+1);
			//printf("%s,",temp);
			CA_FreeMemory(temp);
		 }else
		 {
			return EXCEL_TASK_QUIT;
		 }
	}else if(column==1)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
		    //tempInstruction.operation.testType=(unsigned char)StrToUnsignedLong(temp);   
			//printf("%s\n",temp); 
			CA_FreeMemory(temp);
		 }
	}else if(column==2)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
		    //tempInstruction.operation.testType=(unsigned char)StrToUnsignedLong(temp);   
			if(gBiboList!=0)
			{
			   ListInsertItem(gBiboList,&bibo,END_OF_LIST);
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
	gBiboList = ListCreate(sizeof(tBIBO)); 
    return 0;	
}

BOOL getBibo(char *paramName,tBIBO *biboPtr)
{
	for(int i=1;i<ListNumItems(gBiboList);i++)
	{
		ListGetItem(gBiboList,biboPtr,i);
		if(strcmp(paramName,biboPtr->paramName)==0)
		{
			
			return TRUE;
		}
				
	}
	return FALSE;
}

BOOL BiboProtocolInit(char *sheetName)   
{
	if(gBiboList!=0)
		return TRUE;
	SUT sut=GetSeletedSut();
	EXCELTask task=createExcelTask(sut.configPath,sheetName,SHEET_RANGE_TIPS,3);
	task.onExcelTaskStartListener=(void *)onStartGetTips;
	task.onCellListener=(void *)onCellListenerGetBibo;
	runExcelTask(task);
	return TRUE;
	//printf("%d\n",ListNumItems(paramList));
}
