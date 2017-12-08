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
// Include files
#include "excel2000.h"    
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "excelHelper.h"
#include "sutCommon.h"
#include "common.h"




static ERRORINFO AppInitErrorInfo;
static ExcelObj_App   AppInitExcelAppExeHandle = 0; 
static ExcelObj_App   AppExcelAppHandle = 0;  
static ExcelObj_Workbooks  AppExcelWorkbooksHandle = 0; 
static ExcelObj_Workbook   AppExcelWorkbookHandle = 0;  
static ExcelObj_Sheets    AppExcelSheetsHandle = 0; 

static int excelLock=0;

 


HRESULT ClearObjHandler(CAObjHandle *objHandle)
{
    HRESULT error = 0;
    if ((objHandle) && (*objHandle))
    {
        error = CA_DiscardObjHandle (*objHandle);
        *objHandle = 0;
    }
    return error;    
} 


int GetAppInitExcelAppHandle()
{
     return AppExcelAppHandle;
}



HRESULT InitExcel(void)
{
	HRESULT error = 0;   
    SetWaitCursor (1);
	Excel_NewApp (NULL, 1, LOCALE_NEUTRAL, 0, &AppInitExcelAppExeHandle);     
	
    error = Excel_ActiveApp (NULL, 1, LOCALE_NEUTRAL, 0, &AppExcelAppHandle);
	SetWaitCursor (0); 
	if(error<0)
		return -1;
    
            // Make App Visible
    error = Excel_SetProperty (AppExcelAppHandle, NULL, Excel_AppVisible, CAVT_BOOL, VFALSE);
	if(error<0)
		return -1;
    MakeApplicationActive ();
	
	CmtNewLock ("excelLock", OPT_TL_PROCESS_EVENTS_WHILE_WAITING, &excelLock);//������   
	return error;
}

HRESULT AppInitClearObjHandler(CAObjHandle *objHandle)
{
    HRESULT error = 0;
    if ((objHandle) && (*objHandle))
    {
        error = CA_DiscardObjHandle (*objHandle);
        *objHandle = 0;
    }
    return error;    
}    


HRESULT DeInitExcel()
{
	HRESULT error = 0;    
	
	CmtDiscardLock (excelLock);  //�ͷ���
	
    if (AppExcelAppHandle)
    {   
            // Quit the Application
            error = Excel_AppQuit (AppExcelAppHandle, &AppInitErrorInfo);
            if (error < 0) return error;

        
             AppInitClearObjHandler (&AppExcelAppHandle);
    } 
	
	if(AppInitExcelAppExeHandle)
	{
            error = Excel_AppQuit (AppInitExcelAppExeHandle, &AppInitErrorInfo);
            if (error < 0) return error;
			 AppInitClearObjHandler (&AppInitExcelAppExeHandle);  
	
	}

	return error;
}


ExcelObj_Worksheet GetSheetHandle(char *sheetName)
{
	HRESULT error = 0;
	int exsitflag=0;
	int sheetCount=0;
	char **Name=NULL; 
	ExcelObj_Worksheet  AppExcelWorksheetHandle = 0;
	Excel_GetProperty (AppExcelSheetsHandle,NULL, Excel_SheetsCount, 
            CAVT_LONG, &sheetCount);
	
	//printf("sheetCount=%d\n",sheetCount);   
	
	for(int i=1;i<=sheetCount;i++)
	{
        error = Excel_SheetsItem (AppExcelSheetsHandle, NULL, CA_VariantInt(i), //ע�⣬�ڶ������Ϊ�������ñ�
                    &AppExcelWorksheetHandle);
        if (error<0) 
                return ERROR_EXCEL_GET_SHEET;   //��ȡsheetʧ��
		
		Name= (char **)malloc(100); 
		error = Excel_GetProperty (AppExcelWorksheetHandle,NULL, Excel_WorksheetName, CAVT_CSTRING, Name);
        if (error<0) 
		{	
			free(Name);
            return ERROR_EXCEL_GET_SHEET_NAME;   //��ȡ sheet ����ʧ��
		}
		if(strcmp(sheetName,*Name)==0)
		{
			exsitflag=1;
		    break;
		}	
		//printf("Name=%s\n",*Name);  
		free(Name);
	}	
	
	 error = Excel_WorksheetActivate (AppExcelWorksheetHandle, NULL); 
	 if(error<0)
		 return ERROR_EXCEL_ACTIVE_SHEET;
	 //return error;
	 
	 if(exsitflag==1)
	     return AppExcelWorksheetHandle;  //û��ָ����sheet
	 
	 return ERROR_EXCEL_NO_SPCIFIC_SHEET;
}


int OpenExcel(char *fileName)
{	
	HRESULT error = 0;   
	
    if (!AppExcelWorkbooksHandle)
    {
         // Get Workbooks    
         error = Excel_GetProperty (AppExcelAppHandle, NULL, Excel_AppWorkbooks, 
                    CAVT_OBJHANDLE, &AppExcelWorkbooksHandle);
         if (error<0) 
                return -1;
    
          // Open existing Workbook
          error = Excel_WorkbooksOpen (AppExcelWorkbooksHandle, NULL, fileName, CA_DEFAULT_VAL,
                                             CA_DEFAULT_VAL, CA_DEFAULT_VAL,
                                             CA_DEFAULT_VAL, CA_DEFAULT_VAL,
                                             CA_DEFAULT_VAL, CA_DEFAULT_VAL,
                                             CA_DEFAULT_VAL, CA_DEFAULT_VAL,
                                             CA_DEFAULT_VAL, CA_DEFAULT_VAL,
                                             CA_DEFAULT_VAL, &AppExcelWorkbookHandle);
          if (error<0) 
                return -1;  

                // Get Active Workbook Sheets
           error = Excel_GetProperty (AppExcelAppHandle, NULL, Excel_AppSheets, 
                    CAVT_OBJHANDLE, &AppExcelSheetsHandle);
          if (error<0) 
                    return -1;  
		  
		 /* error=GetSheet(sheetName);
          if (error<0) 
                    return -1; */ 
       }
	
	 return 0;
  
}



ExcelObj_Worksheet GetWorkingSheet(char *fileName,char *sheetName)
{
	//char fileName[MAX_PATHNAME_LEN];
	CmtGetLock (excelLock);   
	ExcelObj_Worksheet  AppExcelWorksheetHandle;
	/*GetProjectDir (fileName);  
	strcat(fileName,configPath);  
    System s=GetSeletedSystem();
	strcat(fileName,s.configPath);*/  
	
    OpenExcel(fileName);
	AppExcelWorksheetHandle=GetSheetHandle(sheetName);
	if(AppExcelWorksheetHandle<0)
	{
	    CmtReleaseLock (excelLock); 
	}	
	return AppExcelWorksheetHandle;
}

HRESULT ClearWorkingSheet(ExcelObj_Worksheet *ExcelWorkbookHandlePtr)
{
	int error=0;
    ClearObjHandler (ExcelWorkbookHandlePtr);
    ClearObjHandler (&AppExcelSheetsHandle);
	AppExcelSheetsHandle=0;
    if (AppExcelWorkbookHandle) 
    {
          error = Excel_WorkbookClose (AppExcelWorkbookHandle, NULL, CA_VariantBool (VFALSE), 
              CA_DEFAULT_VAL, CA_VariantBool (VFALSE));
          if (error < 0)
              return error;
           ClearObjHandler (&AppExcelWorkbookHandle);
		   AppExcelWorkbookHandle=0;
     }
	ClearObjHandler (&AppExcelWorkbooksHandle);
	AppExcelWorkbooksHandle=0;
	CmtReleaseLock (excelLock); 
	return 0;
}

HRESULT SaveWorkingSheet(char *fileName)
{
	  HRESULT error = 0;
	  VARIANT MyVariant;
      if (AppExcelWorkbookHandle) 
	  {
            error = CA_VariantSetCString(&MyVariant, fileName);
            error = Excel_WorkbookSaveAs (AppExcelWorkbookHandle, NULL, MyVariant,
                                                  CA_DEFAULT_VAL, CA_DEFAULT_VAL,
                                                  CA_DEFAULT_VAL, CA_DEFAULT_VAL,
                                                  CA_DEFAULT_VAL, ExcelConst_xlNoChange,
                                                  CA_DEFAULT_VAL, CA_DEFAULT_VAL,
                                                  CA_DEFAULT_VAL, CA_DEFAULT_VAL);
             CA_VariantClear(&MyVariant);
	    	  
	  }
      return 0;

}


EXCELTask createExcelTask(char *fileName,char *sheetName,char *activeRange,int columnNum)
{
    EXCELTask task;
	memset(&task,0,sizeof(task));
	task.fileName=fileName;
	task.sheetName=sheetName;
	task.activeRange=activeRange;
	task.columnNum=columnNum;
	task.onCellListener=NULL;
	task.onRowListener=NULL;
	task.onExcelTaskStartListener=NULL;
	task.onExcelTaskFinishListener=NULL;
	task.onErrorListener=NULL;
	return task;
}


//֧���߳�ͬ��
//��GetWorkSheet������ȡ��
//ClearWorkingSheet�ͷ���
HRESULT runExcelTask(EXCELTask task)
{
    HRESULT ret = ret=EXCEL_TASK_OK;

	ExcelObj_Worksheet ExcelWorksheetHandle=0;
	
    ExcelObj_Range ExcelSingleCellRangeHandle = 0;
	
	VARIANT MyVariant; 
	LPDISPATCH MyDispatch;
	VARIANT MyCellRangeV;
	ERRORINFO ErrorInfo;
	ExcelObj_Range  ExcelRangeHandle = 0;
	//char *EXCEL_ARRAY_OF_CELLS=activeRange//;"A2:H2"; 
	int rowCounter=0;
	
	ExcelWorksheetHandle=GetWorkingSheet(task.fileName,task.sheetName);//���sheet���
	if(ExcelWorksheetHandle<0)
	{
		  char temp[100]={0};
		  Fmt(temp,"��ȡ��(%s)ʧ��",task.sheetName);
		  ret=EXCEL_TASK_ERROR;
		  WarnShow(temp);
	      goto DONE;
	}	
	
    ret = CA_VariantSetCString (&MyCellRangeV, task.activeRange);
    if (ret<0) 
	{	
		ClearWorkingSheet (&ExcelWorksheetHandle);
		ret=EXCEL_TASK_ERROR;  
		goto DONE;  
	}	
	
    ret = Excel_WorksheetRange (ExcelWorksheetHandle, NULL, MyCellRangeV, CA_DEFAULT_VAL, &ExcelRangeHandle);
    if (ret<0) 
	{	
		CA_VariantClear(&MyCellRangeV); 
		ClearWorkingSheet (&ExcelWorksheetHandle);
		ret=EXCEL_TASK_ERROR;  
		goto DONE;
	}	
	
    CA_VariantClear(&MyCellRangeV);
	
	ret = Excel_RangeActivate (ExcelRangeHandle, &ErrorInfo, NULL); 
    if (ret<0) 
	{	
		CA_VariantClear(&MyCellRangeV); 
		ClearObjHandler (&ExcelRangeHandle); 
		ClearWorkingSheet (&ExcelWorksheetHandle);
		ret=EXCEL_TASK_ERROR;  
		goto DONE; 
	}	
	
	if(task.onExcelTaskStartListener!=NULL)
	{	
		(*(ON_EXCEL_TASK_START_LISTENER)(task.onExcelTaskStartListener))();

	}

	while(1)
	{
		
		for(int k=0;k<task.columnNum;k++)
		{	
            ret = Excel_RangeGetItem (ExcelRangeHandle, &ErrorInfo, CA_VariantInt (rowCounter+1), CA_VariantInt (k+1), &MyVariant);
            if (ret<0) 
	        {
			   ret=EXCEL_TASK_ERROR; 	
		       goto DONE; 
	        }	

            // Get the DISPATCH pointer
            ret = CA_VariantGetDispatch (&MyVariant, &MyDispatch);
            if (ret<0) 
	        {
			   ret=EXCEL_TASK_ERROR; 	
		       goto DONE;
	        }	
            // Create a new Range Object from DISPATCH pointer
            ret = CA_CreateObjHandleFromIDispatch (MyDispatch, 0, &ExcelSingleCellRangeHandle);
            if (ret<0) 
	        {	
			   ret=EXCEL_TASK_ERROR; 	
		       goto DONE; 
	        }	

            
            // Get the value of the Single Cell Range
            ret = Excel_GetProperty (ExcelSingleCellRangeHandle, &ErrorInfo, Excel_RangeValue2, CAVT_VARIANT, &MyVariant);
            if (ret<0) 
	        {	
			   ret=EXCEL_TASK_ERROR;
		       goto DONE; 
	        }
			
			
			if(task.onCellListener!=NULL)
			{	
				ret= (*(ON_CELL_LISTENER)(task.onCellListener))(&MyVariant,rowCounter,k);
			    if(ret==EXCEL_TASK_ERROR)
			    {
					 CA_VariantClear(&MyVariant); 
			         goto DONE;
			    }else if(ret==EXCEL_TASK_QUIT)
				{
					 ret=EXCEL_TASK_OK;
					 CA_VariantClear(&MyVariant); 
				     goto DONE;
				}	
			} 
			CA_VariantClear(&MyVariant);  
			ClearObjHandler (&ExcelSingleCellRangeHandle);   
			ExcelSingleCellRangeHandle=0;
				
		} //end for()
		rowCounter++; 
		if(task.onRowListener!=NULL)
		{
		     (*(ON_ROW_LISTENER)(task.onRowListener))();
		}	
	}//end while(1)	
DONE:

	if(task.onExcelTaskFinishListener!=NULL)
	{	
		(*(ON_EXCEL_TASK_FINISH_LISTENER)(task.onExcelTaskFinishListener))();

	}

	if(ExcelSingleCellRangeHandle>0)
		ClearObjHandler (&ExcelSingleCellRangeHandle);
    if(ExcelRangeHandle>0)
        ClearObjHandler (&ExcelRangeHandle);
	if(ExcelWorksheetHandle>0)
	    ClearWorkingSheet (&ExcelWorksheetHandle); //step3�ͷ�sheet��Դ 
   
	
    return ret;       
}

ExcelObj_Range InitExcelRangeHandle(ExcelObj_Worksheet sheetHandle,char *Range)
{
	ERRORINFO ErrorInfo; 
    HRESULT error = 0;
	ExcelObj_Range ExcelRangeHandle;


	VARIANT MyVariant;
	VARIANT MyCellRangeV;  
	//char *EXCEL_ARRAY_OF_CELLS="B1:F1";

    error = CA_VariantSetCString (&MyCellRangeV, Range);
    error = Excel_WorksheetRange (sheetHandle, NULL, MyCellRangeV, CA_DEFAULT_VAL, &ExcelRangeHandle);
    CA_VariantClear(&MyCellRangeV);
	error = Excel_RangeActivate (ExcelRangeHandle, &ErrorInfo, NULL);
	return ExcelRangeHandle;
}


int readSingleExcelRow(char *fileName1,char *sheetName,char *range,VARIANT *array,int row,int columnNum)
{
	 char fileName[MAX_PATHNAME_LEN]; 
	 GetProjectDir (fileName);
	 strcat(fileName,"\\demo.xls");
	 ExcelObj_Worksheet sheetHandle=GetWorkingSheet(fileName1,sheetName);  //step2 ��ȡseetHandle�����fileNameΪ����·��
	 if(sheetHandle<0)
	 {
	 	 return -1;//wrong sheetName
	 }
	 
	 ExcelObj_Range rangeHandler=InitExcelRangeHandle(sheetHandle,range);  //step3 ��ʼ�� ExcelRangeHandle����A1��D1��Ϊ��Χ 
	 if(rangeHandler<0)
	 {
	 	return -2;//initRange error
	 }
	 
	 for(int i=1;i<=columnNum;i++)
	 {	 
		 VARIANT MyCellRangeV;  
		 LPDISPATCH MyDispatch;
		 ExcelObj_Range ExcelSingleCellRangeHandle = 0; 
		 Excel_RangeGetItem (rangeHandler, NULL, CA_VariantInt (row), CA_VariantInt (i), &(array[i-1]));
		 CA_VariantGetDispatch (&(array[i-1]), &MyDispatch);
		 CA_CreateObjHandleFromIDispatch (MyDispatch, 0, &ExcelSingleCellRangeHandle); 
		 Excel_GetProperty (ExcelSingleCellRangeHandle, NULL, Excel_RangeValue2, CAVT_VARIANT, &(array[i-1])); 
		 ClearObjHandler(&ExcelSingleCellRangeHandle);
	 }
	 ClearObjHandler (&rangeHandler); 
	 ClearWorkingSheet (&sheetHandle);//step6��������/
	 return 0;
	
}

VARIANT *readExcelRectangle(char *fileName1,char *sheetName,char *range,int columnNum,int rowNum)
{
	 int totoalRowNum;
	 char fileName[MAX_PATHNAME_LEN]; 
	 GetProjectDir (fileName);
	 strcat(fileName,"\\demo.xls");

	 VARIANT *array=(VARIANT *)malloc(sizeof(VARIANT)*columnNum*rowNum);

	 
	 ExcelObj_Worksheet sheetHandle=GetWorkingSheet(fileName1,sheetName);  //step2 ��ȡseetHandle�����fileNameΪ����·��
	 if(sheetHandle<0)
	 {
	 	 return NULL;//wrong sheetName
	 }
	 
	 ExcelObj_Range rangeHandler=InitExcelRangeHandle(sheetHandle,range);  //step3 ��ʼ�� ExcelRangeHandle����A1��D1��Ϊ��Χ 
	 if(rangeHandler<0)
	 {
	 	return NULL;//initRange error
	 }
	 
	 for(int j=1;j<=rowNum;j++)
	 {	 
	 	for(int i=1;i<=columnNum;i++)
	 	{	 
			VARIANT MyCellRangeV;  
		 	LPDISPATCH MyDispatch;
		 	ExcelObj_Range ExcelSingleCellRangeHandle = 0; 
		 	Excel_RangeGetItem (rangeHandler, NULL, CA_VariantInt (j), CA_VariantInt (i), &(array[(j-1)*columnNum+i-1]));
		 	CA_VariantGetDispatch (&(array[(j-1)*columnNum+i-1]), &MyDispatch);
		 	CA_CreateObjHandleFromIDispatch (MyDispatch, 0, &ExcelSingleCellRangeHandle); 
		 	Excel_GetProperty (ExcelSingleCellRangeHandle, NULL, Excel_RangeValue2, CAVT_VARIANT, &(array[(j-1)*columnNum+i-1])); 
		 	ClearObjHandler(&ExcelSingleCellRangeHandle);
	 
		}
	 }
	 ClearObjHandler (&rangeHandler); 
	 ClearWorkingSheet (&sheetHandle);//step6��������/
	 return array;
}

