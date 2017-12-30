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
#include "cvixml.h"
#include <cvirte.h>		
#include <userint.h>
#include <stdlib.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "resultSave.h"
#include "eutConfig.h"
#include "excelHelper.h"
#include "sutCommon.h"
#include "convertTool.h"
#include "testGroupInit.h"
#include "resultUtil.h"
#include "common.h"
#include "log.h"
#include "settingConfig.h"
#include "regexpr.h" 

#define SAVE_RANGE "A1:J1" 

#define SAVE_STEP_TABLE_TITLE   1
#define SAVE_STEP_TABLE_CONTENT  2
#define SAVE_STEP_NONE 0
static int saveStep=SAVE_STEP_TABLE_TITLE;





HRESULT ValidateFile(char *temp)
{
    HRESULT error=0;
	int fileSize; 
	if(FileExists(temp,&fileSize)==0)
	{
	    return error;
	}else 
	{
	    DeleteFile(temp);
	}	
	return error;
}

HRESULT WriteResultToSheet(ExcelObj_Worksheet sheetHandle,HashTableType hashTable)
{
	int valueColumn = 0,resultColumn=0,testItemNameColumn=0,standardColumn=0;
	ERRORINFO ErrorInfo; 
    HRESULT error = 0;
    int i=0, j=0;
	char itemName[100]={0};	
	char valueStr[100]={0};
	char * temp; //TODO:��temp���У����ٱ���,����ʹ�ù������
    ExcelObj_Range ExcelSingleCellRangeHandle = 0;
	ExcelObj_Range  ExcelRangeHandle = 0;
	int quitFlag=0;
	unsigned int perc;
	VARIANT MyVariant;
	VARIANT MyCellRangeV;  
	LPDISPATCH MyDispatch;
	//char *EXCEL_ARRAY_OF_CELLS="B1:F1";
	int empty=0;
	
	
    error = CA_VariantSetCString (&MyCellRangeV, SAVE_RANGE);
    error = Excel_WorksheetRange (sheetHandle, NULL, MyCellRangeV, CA_DEFAULT_VAL, &ExcelRangeHandle);
    CA_VariantClear(&MyCellRangeV);
	error = Excel_RangeActivate (ExcelRangeHandle, &ErrorInfo, NULL);
    while(1)
    {
		int index=-1;
		RESULT itemResult={0};
        for (j=0;j<10;j++)
        {
            // Ask for the ith by jth value of the range which returns a dispatch to a new single cell range
           		
			 
            error = Excel_RangeGetItem (ExcelRangeHandle, &ErrorInfo, CA_VariantInt (i+1), CA_VariantInt (j+1), &MyVariant);
            if (error<0) 
	        {	
		       SetWaitCursor (0);
		       return -1;
	        }	

            // Get the DISPATCH pointer
            error = CA_VariantGetDispatch (&MyVariant, &MyDispatch);
            if (error<0) 
	        {	
		       SetWaitCursor (0);
		       return -1;
	        }	

            
            // Create a new Range Object from DISPATCH pointer
            error = CA_CreateObjHandleFromIDispatch (MyDispatch, 0, &ExcelSingleCellRangeHandle);
            if (error<0) 
	        {	
		       SetWaitCursor (0);
		       return -1;
	        }	

            
            // Get the value of the Single Cell Range
            error = Excel_GetProperty (ExcelSingleCellRangeHandle, &ErrorInfo, Excel_RangeValue2, CAVT_VARIANT, &MyVariant);
            if (error<0) 
	        {	
		       SetWaitCursor (0);
		       return -1;
	        }
			
			if(j==0)
			{
			    if(CA_VariantHasCString(&MyVariant))
		     	{					
			    	CA_VariantGetCString(&MyVariant, &temp);
					if(strcmp(temp,"���")==0 || strcmp(temp,"������Ŀ")==0 )
					{
						saveStep=SAVE_STEP_TABLE_TITLE;
					}else{
					    //saveStep=SAVE_STEP_NONE; 
					}
					CA_FreeMemory(temp);
				}else if(CA_VariantHasDouble(&MyVariant))
			    {
				    //CA_VariantGetDouble(&MyVariant,&inputValue);
				    saveStep=SAVE_STEP_TABLE_CONTENT;
				}else{
				    //saveStep=SAVE_STEP_NONE;
				}			
			
			}
			
			if(saveStep ==SAVE_STEP_TABLE_TITLE)
			{
			    if(CA_VariantHasCString(&MyVariant))
		     	{					
			    	CA_VariantGetCString(&MyVariant, &temp);
					if(strcmp(temp,"�ź�����")==0 || strcmp(temp,"����")==0 || strcmp(temp,"��������")==0 )
					{
						//saveStep=SAVE_STEP_TABLE_TITLE;
						testItemNameColumn=j;
					}
					
					if(strcmp(temp,"����Ҫ��")==0)
					{
						//saveStep=SAVE_STEP_TABLE_TITLE;
						standardColumn=j;
					}
					
					if(strcmp(temp,"����")==0) 
					{
					     resultColumn=j;
					}
					
					if(strcmp(temp,"ʵ��ֵ")==0) 
					{
					    valueColumn=j;
					}
					CA_FreeMemory(temp);
				}
			}

			//if(saveStep == SAVE_STEP_NONE)
			//{
			   	 if(CA_VariantIsEmpty(&MyVariant))
				 {
				       empty++;
					   //break;
				 }else
				 {
					    
				        empty=0;
				 }
				 
				if(empty>100)
			    {
				   quitFlag=1;
				   break;
				   
			     }			 
			//}
			
			if(saveStep ==SAVE_STEP_TABLE_CONTENT)
			{
				
				
			
			
			if(j==testItemNameColumn) //��������
			{
				
			    if(CA_VariantHasCString(&MyVariant))
		     	{					
			    	CA_VariantGetCString(&MyVariant, &temp);
					memset(itemName,100,0);
					sprintf(itemName,"%s",temp);
					if(strcmp(temp,"end")==0)
					{
					   quitFlag=1;
					   break;
					}
					CA_FreeMemory(temp);
				}
				
				
			}

			/*if(j==2)
			{
			     if(CA_VariantIsEmpty(&MyVariant))
				 {
				       empty++;
					   break;
				 }else
				 {
					    
				        empty=0;
				 }			      
			}*/
			
			if(j==standardColumn)
			{
				double inputValue=0;
			   	if(CA_VariantIsEmpty(&MyVariant))
				{
					CA_VariantClear(&MyVariant);
                    ClearObjHandler (&ExcelSingleCellRangeHandle);	
				    break;
				}				
				if(CA_VariantHasDouble(&MyVariant))
			    {
				    CA_VariantGetDouble(&MyVariant,&inputValue);

			        memset(valueStr,0,100);
			        sprintf(valueStr,"%f",inputValue);
				}
			    if(CA_VariantHasCString(&MyVariant))
		     	{
			    	CA_VariantGetCString(&MyVariant, &temp);
					memset(valueStr,100,0);
					sprintf(valueStr,"%s",temp);
					inputValue=(double)String2UnsignedLong(temp);
					CA_FreeMemory(temp); 
				}
				//printf("itemName:%s,inputValue:%f,valueStr:%s\n",itemName,inputValue,valueStr);
				index=getTestCaseId(itemName,inputValue,valueStr);
				//index=getTestCaseId(itemName,);
				//printf("index=%d\n",index);
				if(index!=-1)
				{
				   //��ȡ���Խ��
					
					HashTableGetItem(hashTable,&index,&itemResult,sizeof(RESULT));
					//LOG_EVENT_FORMAT(LOG_INFO,"testCase:%s,result:%d",itemName,itemResult.pass); 
			    	/*printf("index=%d\n",itemResult.index);
				    printf("value=%f,pass=%d\n",itemResult.recevValue,itemResult.pass);
					*/	
				}			    
			}	
			
	    
			
			
			if(j==valueColumn)
			{
			     if(index!=-1)
				 {
					 //Excel_RangeSetItem (ExcelRangeHandle, &ErrorInfo, CA_VariantInt (i+1), CA_VariantInt (j+1), CA_VariantDouble (itemResult.recevValue) );
					 VARIANT vTemp; 
					 CA_VariantSetCString(&vTemp,itemResult.recvString); 
					 Excel_RangeSetItem (ExcelRangeHandle, &ErrorInfo, CA_VariantInt (i+1), CA_VariantInt (j+1),vTemp);
					 CA_VariantClear(&vTemp);
				 } 	 
			}
			
			if(j==resultColumn)
			{
			     if(index!=-1)
				 {
					 VARIANT vTemp;
					 if(itemResult.pass)
					 {
						 CA_VariantSetCString(&vTemp,"�ϸ�");    
					 }else
					 {
					     CA_VariantSetCString(&vTemp,"���ϸ�");
					 }	
					 Excel_RangeSetItem (ExcelRangeHandle, &ErrorInfo, CA_VariantInt (i+1), CA_VariantInt (j+1), vTemp); 
					 CA_VariantClear(&vTemp);
				 } 	 
			     
			}
			}
			
            CA_VariantClear(&MyVariant);
            ClearObjHandler (&ExcelSingleCellRangeHandle);	
		}	
		i++;	
		if(quitFlag==1)
		{
            CA_VariantClear(&MyVariant);
            ClearObjHandler (&ExcelSingleCellRangeHandle);	  
			break;
		}
    }  		

    CA_VariantClear(&MyVariant);
    CA_VariantClear(&MyCellRangeV);
    ClearObjHandler (&ExcelRangeHandle);
    ClearObjHandler (&ExcelSingleCellRangeHandle);
	return 0;

}





/*
dirName:Ŀ¼����
time:ʱ��
�����ļ�������Ŀ¼/dirName/time-result.xls

*/
#if 0
void saveTestResult(char *time,char *dirName,HashTableType hashTable)
{
    char temp[MAX_PATHNAME_LEN]={0};
	char fileName[MAX_PATHNAME_LEN]={0};   
	int fileSize;
	SETTING s=GetSetting();
	SUT system=GetSeletedSut();
	
                                                                                                                   
	if(FileExists(s.saveDir,&fileSize)==0)
	{	
	    MakeDir(temp);
	}

    sprintf(temp,"%s\\%s",s.saveDir,dirName); 	
	if(FileExists(temp,&fileSize)==0) //������
	{	
	    MakeDir(temp);
	}
	//printf("%d\n",s.reportInfoCustom);
	if(s.reportInfoCustom)
	{	
	   getUserCustiomName(fileName);
	   sprintf(temp,"%s\\%s\\%s_%s_%s%s",s.saveDir,dirName,dirName,time,fileName,".xls");
	}else
	{
	   sprintf(temp,"%s\\%s\\%s_%s%s",s.saveDir,dirName,dirName,time,".xls"); 
	}
	ValidateFile(temp);
	saveStep=SAVE_STEP_TABLE_TITLE;
	LOG_EVENT_FORMAT(LOG_INFO,"save eut %s test result start!\n",dirName);
	ExcelObj_Worksheet sheetHandle=GetWorkingSheet(system.reportFilePath,"Sheet1");//���sheet���,��ģ��·�����ĵ�
	WriteResultToSheet(sheetHandle,hashTable);//�������
    SaveWorkingSheet(temp);//����������� //������Ŀ��·��	
	ClearWorkingSheet (&sheetHandle);//��������/ 
	LOG_EVENT_FORMAT(LOG_INFO,"save %s test result finshed!\n",dirName);   
   
}
#else
void parseExcelCell(char *writeStr,char *temp,HashTableType hashTable)
{
	 RESULT itemResult;
	 int matched,position,matchedLen;
	 char tempBuffer[100]={0};
	 RegExpr_FindPatternInText("[0-9]+",1,temp,-1,1,1,&matched,&position,&matchedLen);
	 memcpy(tempBuffer,temp+position,matchedLen);
	 int id=atoi(tempBuffer);
	 HashTableGetItem(hashTable,&id,&itemResult,sizeof(RESULT)); 
	 RegExpr_FindPatternInText("{result}",1,temp,-1,1,1,&matched,&position,&matchedLen);
	//printf("result:%d,recvString:%s",itemResult.pass,itemResult.recvString);
	 TestItem item;
	 ListType itemList=getTestCaseList();
	 ListGetItem(itemList,&item,id);
	 
	 //printf("id:%d,item.itemId:%d,itemName:%s,itemType:%s\n",id,item.itemId,item.itemName_,item.itemType_);
	 
	 if(matched==1)
	 {
	 	//��д���
		 if(itemResult.pass==0)
		 {
		 	sprintf(writeStr,"%s","���ϸ�");
		 }else
		 {
		 	sprintf(writeStr,"%s","�ϸ�"); 
		 }
		 
	 }else
	 {
	 	//��дʵ��ֵ
		 sprintf(writeStr,"%s",itemResult.recvString);
	 }
	 
}

void writeResultToExcelSheet(ExcelObj_Range rangeHandler,HashTableType hashTable)
{
	HRESULT error = 0;
	ERRORINFO ErrorInfo; 
	VARIANT MyVariant;
	VARIANT MyCellRangeV;  
	LPDISPATCH MyDispatch;
	ExcelObj_Range ExcelSingleCellRangeHandle = 0; 
	char valueStr[100]={0};
	char * temp; //TODO:��temp���У����ٱ���,����ʹ�ù������  
	int i=0, j=0;
	int matched,position,matchedLen; 
	int empty=0;
	int testCaseCnt=ListNumItems(getTestCaseList());
	int currentSaveCnt=0;
	while(1)
	{
		int index=-1;
		RESULT itemResult;
        for (j=0;j<10;j++)
		{
            error = Excel_RangeGetItem (rangeHandler, &ErrorInfo, CA_VariantInt (i+1), CA_VariantInt (j+1), &MyVariant);
            if (error<0) 
	        {	
				return;
	        }	

            // Get the DISPATCH pointer
            error = CA_VariantGetDispatch (&MyVariant, &MyDispatch);
            if (error<0) 
	        {	
		       return;
	        }
			
            error = CA_CreateObjHandleFromIDispatch (MyDispatch, 0, &ExcelSingleCellRangeHandle);
            if (error<0) 
	        {	
		       return;
	        }	

            
            // Get the value of the Single Cell Range
            error = Excel_GetProperty (ExcelSingleCellRangeHandle, &ErrorInfo, Excel_RangeValue2, CAVT_VARIANT, &MyVariant);
            if (error<0) 
	        {	
		       return;
	        }			
			
			if(CA_VariantHasCString(&MyVariant))
		    {		
				empty=0;
			    CA_VariantGetCString(&MyVariant, &temp);
				if((temp[0]>='a') && (temp[0]<='z'))   //Ϊ�˼��ٴ����ж�,�ӿ�洢�ٶ�
				{
					RegExpr_FindPatternInText("{value}|{result}[0-9]+",1,temp,-1,1,1,&matched,&position,&matchedLen);
					if(matched==1)
					{
						//memcpy(valueStr,temp+position,matchedLen);  
					
						char writeStr[100]={0};
						parseExcelCell(writeStr,temp,hashTable);
						VARIANT vTemp; 
						CA_VariantSetCString(&vTemp,writeStr); 
						Excel_RangeSetItem (rangeHandler, &ErrorInfo, CA_VariantInt (i+1), CA_VariantInt (j+1),vTemp);
						CA_VariantClear(&vTemp);
						currentSaveCnt++;
						//printf("%d,%d\n",currentSaveCnt/2,testCaseCnt);
					}
					CA_FreeMemory(temp); 
				}
			}else
			{
				empty++;
				if(empty>50)
					break;
			}
            CA_VariantClear(&MyVariant);
            ClearObjHandler (&ExcelSingleCellRangeHandle);				
		}
		i++;
		if(empty>50)
		{
			CA_VariantClear(&MyVariant);
            ClearObjHandler (&ExcelSingleCellRangeHandle);	
			break;
		}
	}
}

void generateReportFilePath(char *time,char *dirName,char *temp)
{
	char fileName[MAX_PATHNAME_LEN]={0};   
	int fileSize;
	SETTING s=GetSetting();
	if(FileExists(s.saveDir,&fileSize)==0)
	{	
	    MakeDir(temp);
	}

    sprintf(temp,"%s\\%s",s.saveDir,dirName); 	
	if(FileExists(temp,&fileSize)==0) //������
	{	
	    MakeDir(temp);
	}
	//printf("%d\n",s.reportInfoCustom);
	if(s.reportInfoCustom)
	{	
	   getUserCustiomName(fileName);
	   sprintf(temp,"%s\\%s\\%s_%s_%s%s",s.saveDir,dirName,dirName,time,fileName,".xlsx");
	}else
	{
	   sprintf(temp,"%s\\%s\\%s_%s%s",s.saveDir,dirName,dirName,time,".xlsx"); 
	}
	ValidateFile(temp);	
}

void saveTestResult(char *time,char *dirName,HashTableType hashTable,char *fileName)
{
    char temp[MAX_PATHNAME_LEN]={0};
	SUT system=GetSeletedSut();
	generateReportFilePath(time,dirName,temp);
	saveStep=SAVE_STEP_TABLE_TITLE;
	LOG_EVENT_FORMAT(LOG_INFO,"save eut %s test result start!\n",dirName);
	ExcelObj_Worksheet sheetHandle=GetWorkingSheet(system.reportFilePath,"Sheet1");//���sheet���,��ģ��·�����ĵ�
	ExcelObj_Range rangeHandler=InitExcelRangeHandle(sheetHandle,SAVE_RANGE);
	
	writeResultToExcelSheet(rangeHandler,hashTable);
	
	//WriteResultToSheet(sheetHandle,hashTable);//�������
	
    SaveWorkingSheet(temp);//����������� //������Ŀ��·��	
	ClearObjHandler (&rangeHandler); //������  
	ClearWorkingSheet (&sheetHandle);//��������/ 
	LOG_EVENT_FORMAT(LOG_INFO,"save %s test result finshed!\n",dirName);  
	if(fileName!=NULL)
		sprintf(fileName,"%s",temp);
   
}
#endif


