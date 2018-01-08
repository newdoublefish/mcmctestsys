
#include <userint.h>
#include "excelHelper.h" 
#include "toolbox.h"  
#include "ParamRuleParse.h"
#include "sutCommon.h" 
#include "debug.h"

//static HashTableType tipsHashTable=0;
#define SHEET_RANGE_TIPS "A2:F2" 
ListType paramList;


static HRESULT onCellListenerGetTips(VARIANT *MyVariant,int row,int column)    
{
	char *temp;
	//static char key[50]={0}; 
	static PARAMETER param;
	if(column==0)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			//memcpy(tempInstruction.name,temp,strlen(temp)+1);
			memset(&param,0,sizeof(PARAMETER));
			sprintf(param.paramName,"%s",temp);
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
			CA_FreeMemory(temp);
		 }
		 if(CA_VariantHasDouble(MyVariant))
	     {
		    double value;
			CA_VariantGetDouble(MyVariant,&value);
			param.group = (int)value;
			
		 }		 
	}else if(column==2)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			CA_FreeMemory(temp);
		 }
		 if(CA_VariantHasDouble(MyVariant))
	     {
		    double value;
			CA_VariantGetDouble(MyVariant,&value);
			param.item = (int)value;
			
		 }		 
	}else if(column==3)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			CA_FreeMemory(temp);
		 }
		 if(CA_VariantHasDouble(MyVariant))
	     {
		    double value;
			CA_VariantGetDouble(MyVariant,&value);
			param.gunIndex = (int)value;
		 }			 
	}else if(column==4)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			CA_FreeMemory(temp);
		 }
		 if(CA_VariantHasDouble(MyVariant))
	     {
		    double value;
			CA_VariantGetDouble(MyVariant,&value);
			param.array = (int)value;
		 }		 
	}else if(column==5)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			CA_FreeMemory(temp);
		 }
		 if(CA_VariantHasDouble(MyVariant))
	     {
		    double value;
			CA_VariantGetDouble(MyVariant,&value);
			param.element = (int)value;
		 }
		 if(paramList!=0)
		 {
			 //printfParam(param);
			 ListInsertItem(paramList,&param,END_OF_LIST);
		 }
	}
	return EXCEL_TASK_OK;
}


static HRESULT onStartGetTips(VARIANT *MyVariant,int row,int column)				
{
	//istList=ListCreate(sizeof(INSTRUCTION));
	paramList = ListCreate(sizeof(PARAMETER)); 
    return 0;	
}



BOOL ParamProtocolInit(char *sheetName)   
{
	if(paramList!=0)
		return TRUE;
	SUT sut=GetSeletedSut();
	EXCELTask task=createExcelTask(sut.configPath,"param",SHEET_RANGE_TIPS,6);
	task.onExcelTaskStartListener=(void *)onStartGetTips;
	task.onCellListener=(void *)onCellListenerGetTips;
	runExcelTask(task);
	return TRUE;
	//printf("%d\n",ListNumItems(paramList));
}

BOOL getParameter(char *paramName,PARAMETER *paramPtr)
{
	for(int i=1;i<ListNumItems(paramList);i++)
	{
		ListGetItem(paramList,paramPtr,i);
		if(strcmp(paramName,paramPtr->paramName)==0)
		{
			
			return TRUE;
		}
				
	}
	return FALSE;
}

ListType getParamList()
{
	return paramList;
}
