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
#include "login.h"

#define SAVE_RANGE "A1:K1" 

#define SAVE_STEP_TABLE_TITLE   1
#define SAVE_STEP_TABLE_CONTENT  2
#define SAVE_STEP_NONE 0
static int saveStep=SAVE_STEP_TABLE_TITLE;

static void getResultRecordTime(char *timeBuffer);





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
	char * temp; //TODO:用temp就行，减少变量,上面使用过多变量
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
					if(strcmp(temp,"序号")==0 || strcmp(temp,"测试项目")==0 )
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
					if(strcmp(temp,"信号名称")==0 || strcmp(temp,"参数")==0 || strcmp(temp,"参数名称")==0 )
					{
						//saveStep=SAVE_STEP_TABLE_TITLE;
						testItemNameColumn=j;
					}
					
					if(strcmp(temp,"技术要求")==0)
					{
						//saveStep=SAVE_STEP_TABLE_TITLE;
						standardColumn=j;
					}
					
					if(strcmp(temp,"结论")==0) 
					{
					     resultColumn=j;
					}
					
					if(strcmp(temp,"实测值")==0) 
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
				
				
			
			
			if(j==testItemNameColumn) //参数名称
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
				   //获取测试结果
					
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
						 CA_VariantSetCString(&vTemp,"合格");    
					 }else
					 {
					     CA_VariantSetCString(&vTemp,"不合格");
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
dirName:目录名字
time:时间
生成文件，配置目录/dirName/time-result.xls

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
	if(FileExists(temp,&fileSize)==0) //不存在
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
	ExcelObj_Worksheet sheetHandle=GetWorkingSheet(system.reportFilePath,"Sheet1");//获得sheet句柄,打开模板路劲的文档
	WriteResultToSheet(sheetHandle,hashTable);//操作句柄
    SaveWorkingSheet(temp);//保存更改内容 //保存在目标路径	
	ClearWorkingSheet (&sheetHandle);//清除句柄，/ 
	LOG_EVENT_FORMAT(LOG_INFO,"save %s test result finshed!\n",dirName);   
   
}
#else
void parseExcelCell(char *writeStr,char *temp,HashTableType hashTable)
{
	 RESULT itemResult;
	 char tempBuffer[100]={0};
	 int matched,position,matchedLen;
	 
	 RegExpr_FindPatternInText("[0-9]+",1,temp,-1,1,1,&matched,&position,&matchedLen);
	 memcpy(tempBuffer,temp+position,matchedLen);
	 int id=atoi(tempBuffer);
	 int found = 0;
	  HashTableFindItem(hashTable,&id,&found);
	 if(found<=0)
		return;	 
	 HashTableGetItem(hashTable,&id,&itemResult,sizeof(RESULT)); 
	 if(0==RegExpr_FindPatternInText("{result}",1,temp,-1,1,1,&matched,&position,&matchedLen))
	 {
	 	if(matched==1)
	 	{
	 		//填写结果
		 	if(itemResult.pass==0)
		 	{
		 		sprintf(writeStr,"%s","不合格");
		 	}else
		 	{
		 		sprintf(writeStr,"%s","合格"); 
		 	}
		}
	 }
	 
	 if(strstr(temp,"value")!=NULL)
	 {
	 	sprintf(writeStr,"%s",itemResult.recvString);
	 }
	 

	 
	 if(strstr(temp,"time")!=NULL)
	 {
		char date[30]={0};
		getResultRecordTime(date);
		sprintf(writeStr,"%s",date);
	 }
	 
	/*RegExpr_FindPatternInText("{value}",1,temp,-1,1,1,&matched,&position,&matchedLen);	 
	if(matched==1)
	{
	 	//填写实测值
		 sprintf(writeStr,"%s",itemResult.recvString);
	}*/
}

BOOL getResultSet(char *writeStr,char *temp,HashTableType hashTable)
{
	char delims[] = ",";
  	char *result = NULL;
	int atLeastOneResult=0;//至少有一个值
   	result = strtok( temp, delims );
	BOOL resultSet=TRUE;
	RESULT itemResult;
	int id;
   	while( result != NULL ) {
      	 //printf("%s\n",result);
		 id=atoi(result);
		 int found = 0;
		 HashTableFindItem(hashTable,&id,&found);
		 if(found>0)
		 {
			 atLeastOneResult++;
		 	HashTableGetItem(hashTable,&id,&itemResult,sizeof(RESULT));
		 	if(itemResult.index==id)
		 	{
		 		 if(0==itemResult.pass)
			 	{
			 		resultSet=FALSE;
					break;
			 	}
		 	} 
		 } 		 
       	 result = strtok( NULL, delims );
 	}
	
	if(atLeastOneResult>0)
	{

		if(resultSet==FALSE)
		{
			sprintf(writeStr,"%s","不合格");
		}else
		{
			sprintf(writeStr,"%s","合格"); 
		}
	}
	return TRUE;
}

static void getResultRecordTime(char *timeBuffer)
{
	unsigned int year, month, day, hour, min, sec, weekDay;
	CVIAbsoluteTime absTime;
	GetCurrentCVIAbsoluteTime(&absTime);
    CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
                &min, &sec, 0, &weekDay);
	//sprintf(timeBuffer,"%02d-%02d-%02d %02d:%02d:%02d",year-2000,month,day,hour,min,sec);
	//sprintf(timeBuffer,"%02d-%02d-%02d %02d:%02d:%02d",year-2000,month,day,hour,min,sec);
	sprintf(timeBuffer,"%02d-%02d-%02d",year-2000,month,day);

}

void writeResultToExcelSheet(ExcelObj_Range rangeHandler,TESTobject testObj)
{
	HRESULT error = 0;
	ERRORINFO ErrorInfo; 
	VARIANT MyVariant;
	VARIANT MyCellRangeV;  
	LPDISPATCH MyDispatch;
	ExcelObj_Range ExcelSingleCellRangeHandle = 0; 
	char valueStr[100]={0};
	char * temp; //TODO:用temp就行，减少变量,上面使用过多变量  
	int i=0, j=0;
	int matched,position,matchedLen; 
	int empty=0;
	int testCaseCnt=ListNumItems(getTestCaseList());
	int currentSaveCnt=0;
	char date[30]={0};
	getResultRecordTime(date);
	
	while(1)
	{
		int index=-1;
		RESULT itemResult;
        for (j=0;j<11;j++)
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
				if((temp[0]>='a') && (temp[0]<='z'))   //为了减少错误判断,加快存储速度
				{
					char writeStr[100]={0};
					BOOL writeFlag =FALSE;
					RegExpr_FindPatternInText("{passrate}|{username}|{value}|{result}|{time}[0-9]+",1,temp,-1,1,1,&matched,&position,&matchedLen);
					if(matched==1)
					{
						parseExcelCell(writeStr,temp,testObj.resultHashTable);
						writeFlag=TRUE;
					}else{
						RegExpr_FindPatternInText("result\{.*\}",1,temp,-1,1,1,&matched,&position,&matchedLen);
						if(matched==1)
						{
							char tempBuffer[100]={0};
							memcpy(tempBuffer,temp+position+7,matchedLen-8);
							//printf("%s",tempBuffer);
							if(TRUE==getResultSet(writeStr,tempBuffer,testObj.resultHashTable))
								writeFlag=TRUE; 
						}/*else if(0==strcmp(temp,"time"))
						{
							//getResultRecordTime(writeStr);
							sprintf(writeStr,"%s",date);
							writeFlag=TRUE; 
						}*/

					    if(strstr(temp,"username")!=NULL)
	 					{
		 					tLoginConfig acount=getAccount(); 
							//printf("%s",acount.fullName);
		 					sprintf(writeStr,"%s",acount.fullName);
							writeFlag=TRUE;
	 					}
						
						if(strstr(temp,"passrate")!=NULL)
						{
							sprintf(writeStr,"%d%%",testObj.passRate);
						    writeFlag=TRUE;
						}
						
					
					}
					
					if(TRUE==writeFlag)
					{
						VARIANT vTemp; 
						CA_VariantSetCString(&vTemp,writeStr); 
						Excel_RangeSetItem (rangeHandler, &ErrorInfo, CA_VariantInt (i+1), CA_VariantInt (j+1),vTemp);
						CA_VariantClear(&vTemp);
						currentSaveCnt++;					
						CA_FreeMemory(temp);
					}
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
	if(FileExists(temp,&fileSize)==0) //不存在
	{	
	    MakeDir(temp);
	}
	//printf("%d\n",s.reportInfoCustom);
	
	SUT sut=GetSeletedSut();
	if(s.reportInfoCustom)
	{	
	   getUserCustiomName(fileName);
	   sprintf(temp,"%s\\%s\\%s_%s_%s%s",s.saveDir,dirName,fileName,sut.systemName,time,".xlsx");
	}else
	{
	   sprintf(temp,"%s\\%s\\%s_%s%s",s.saveDir,dirName,sut.systemName,time,".xlsx"); 
	}
	ValidateFile(temp);	
}


void saveResultToExcelFile(char *fileName,TESTobject testObj)
{
	SUT system=GetSeletedSut();
	ExcelObj_Worksheet sheetHandle=GetWorkingSheet(system.reportFilePath,"Sheet1");//获得sheet句柄,打开模板路劲的文档
	ExcelObj_Range rangeHandler=InitExcelRangeHandle(sheetHandle,SAVE_RANGE);
	writeResultToExcelSheet(rangeHandler,testObj);
    SaveWorkingSheet(fileName);//保存更改内容 //保存在目标路径	
	ClearObjHandler (&rangeHandler); //清除句柄  
	ClearWorkingSheet (&sheetHandle);//清除句柄，/ 
}
#endif


