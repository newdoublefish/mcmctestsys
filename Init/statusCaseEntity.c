#include "excel2000.h"    
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "statusCaseEntity.h"
#include "convertTool.h"
#include "StatusMonitorPanel.h"
#include "common.h"
#include "SutPanel.h"
#include "excelHelper.h"
#include "configPath.h"


ListType statusGroup=0;	   //条例组
ListType statusList=0;	   //条例

int StatusReadFromExcel(ListType g_myList,ExcelObj_Worksheet  ExcelWorksheetHandle,StatusLRU *LRU)
{
	
    HRESULT error = 0;
    int i, j;
	char *temp = NULL;	 //TODO:用temp就行，减少变量,上面使用过多变量
    ExcelObj_Range ExcelSingleCellRangeHandle = 0;
	StatusItem item;
	int quitFlag=0;
	unsigned int perc;
	//StatusLRU LRU[1024]; 
	VARIANT MyVariant; 
	LPDISPATCH MyDispatch;
	VARIANT MyCellRangeV;
	ERRORINFO ErrorInfo;
	ExcelObj_Range  ExcelRangeHandle = 0;
	char *EXCEL_ARRAY_OF_CELLS="A2:I2"; 
	int itemCount;  
	int num = 0;
	int serial = 0;
	
	SetWaitCursor (1);
    error = CA_VariantSetCString (&MyCellRangeV, EXCEL_ARRAY_OF_CELLS);
    error = Excel_WorksheetRange (ExcelWorksheetHandle, NULL, MyCellRangeV, CA_DEFAULT_VAL, &ExcelRangeHandle);
    CA_VariantClear(&MyCellRangeV);
    if (error<0) 
	{	
		SetWaitCursor (0);
		return -1;
	}	
    // Make range Active    
    error = Excel_RangeActivate (ExcelRangeHandle, &ErrorInfo, NULL);
    if (error<0) 
	{	
		SetWaitCursor (0);
		return -1;				 
	}	
	//获取条例数量
    //----------------------------------------------------------------
    // 1) Get each cell value in Range one at a time using an offset 
    //    from the range's top left cell
    //----------------------------------------------------------------
	i=0;
	ListType typeList=NULL;//类型链表，类型链表下面是组别，组别下面是单个条例
    while(1)
    {
        for (j=0;j<5;j++)
        {
            // Ask for the ith by jth value of the range which returns a dispatch to a new single cell range
            error = Excel_RangeGetItem (ExcelRangeHandle, &ErrorInfo, CA_VariantInt (i+1), CA_VariantInt (j+1), &MyVariant);
            // Get the DISPATCH pointer
            error = CA_VariantGetDispatch (&MyVariant, &MyDispatch);
            // Create a new Range Object from DISPATCH pointer
            error = CA_CreateObjHandleFromIDispatch (MyDispatch, 0, &ExcelSingleCellRangeHandle);
            // Get the value of the Single Cell Range
            error = Excel_GetProperty (ExcelSingleCellRangeHandle, &ErrorInfo, Excel_RangeValue2, CAVT_VARIANT, &MyVariant);
			if(j==0) //参数类型
			{
			    if(CA_VariantHasCString(&MyVariant))
		     	{					
					CA_VariantGetCString(&MyVariant, &temp);
					//printf("参数类型：%s\n",temp);
					LRU[serial].LRUName = temp;
				}
			}	
            if(j==3) //参数名称
			{
				if(CA_VariantIsEmpty(&MyVariant))
				{
					quitFlag=1;
					break;
				}	
				if(CA_VariantHasCString(&MyVariant))
				{
		            CA_VariantGetCString(&MyVariant, &temp);  
					//printf("参数名称：%s\n",temp);
					LRU[serial].ItemName[num]=temp;
					num ++;
				}
			}
			if(num == 32)
			{
				num = 0;
				serial ++;
			}
			CA_VariantClear (&MyVariant);
            ClearObjHandler (&ExcelSingleCellRangeHandle);	  
        }
		i++;
		if(quitFlag==1)
		{
			ListDispose(typeList);
            CA_VariantClear(&MyVariant);
            ClearObjHandler (&ExcelSingleCellRangeHandle);	  
			break;	   
		}	
    }
DONE:	
    SetWaitCursor (0);
	
    CA_VariantClear(&MyVariant);
    CA_VariantClear(&MyCellRangeV);
    // Free array of VARAINT
    // Free Range handles
    ClearObjHandler (&ExcelRangeHandle);
    ClearObjHandler (&ExcelSingleCellRangeHandle);
    return serial;
}

int StatusItemInit(char *filePath,StatusLRU *LRU )
{
	HRESULT error = 0;
	ExcelObj_Worksheet ExcelWorksheetHandle;
	int num;
	
	if(statusGroup!=0)
		ListDispose(statusGroup);
	statusGroup = ListCreate (sizeof(StatusItem));
	if(statusList!=0)
		ListDispose(statusList);
	statusList = ListCreate (sizeof(StatusItem));
	ExcelWorksheetHandle = GetWorkingSheet (filePath,"采集器状态");		//step1 获取测试条例的sheet句柄
	if(ExcelWorksheetHandle<0)
		return -1;
	num = StatusReadFromExcel (statusGroup,ExcelWorksheetHandle,LRU); //step2 读取所需信息
	ClearWorkingSheet (&ExcelWorksheetHandle); 					   		//step3释放sheet资源 
    return num;
}




