#include <userint.h>
#include "excelHelper.h" 
#include "sutCommon.h"
#include "convertTool.h"
#include "eutConfig.h"
#include "relayProtocol.h"
#include "debug.h"

static HashTableType relayHashTable=0;
#define SHEET_RANGE_TIPS "A2:B2" 

int getRelayMask(char *key,unsigned int *mask){
   int found=0; 
   parseRelayProtocol(); 
   HashTableFindItem(relayHashTable,key,&found);
   if(found==1)
   {
   	   HashTableGetItem(relayHashTable,key,mask,4);
   }
   return found;
}

static HRESULT onCellListenerGetRelay(VARIANT *MyVariant,int row,int column)    
{
	char *temp;
	static char key[50]={0}; 
	if(column==0)
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
	}else if(column==1)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
		    //tempInstruction.operation.testType=(unsigned char)StrToUnsignedLong(temp); 
			if(strlen(temp)==10)
			{
				if(strstr(temp,"0x")!=NULL)
				{
					 unsigned char array[20]={0};
					 int len=String2ByteArray(temp,array);
					 if(len==4)
					 {
					 	unsigned int ret=0;
				     	for(int i=0;i<len;i++)
				     	{
					   		ret=ret<<8 | array[i];
				     	}
					 
					 	if(relayHashTable!=0){
					 		 HashTableInsertItem(relayHashTable,key,&ret);
					 	}
					 }
				}
			}
			//printf("%s\n",temp); 
			CA_FreeMemory(temp);
		 }
	}
	return EXCEL_TASK_OK;
}


static HRESULT onStartGetRelay(VARIANT *MyVariant,int row,int column)				
{
	//istList=ListCreate(sizeof(INSTRUCTION));
	HashTableCreate(10,C_STRING_KEY,0,4,&relayHashTable);
    return 0;	
}


void parseRelayProtocol(void){
	if(relayHashTable==0)
	{	
	   SUT sut=GetSeletedSut();
	   EXCELTask task=createExcelTask(sut.configPath,"¼ÌµçÆ÷",SHEET_RANGE_TIPS,2);
	   task.onExcelTaskStartListener=(void *)onStartGetRelay;
	   task.onCellListener=(void *)onCellListenerGetRelay;
	   runExcelTask(task);
	}
}
