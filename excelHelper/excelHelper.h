/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：excelHelper.c
 // 文件功能描述:
 //
 // 
 // 创建标识：曾衍仁 20141114
 //
 // 修改标识：曾衍仁  20150804
 // 修改描述：增加excel处理任务
 //-------------------------------------------------------------------------*/

#ifndef __excelHelper_H__
#define __excelHelper_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"
#include "excel2000.h"   

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
// Global functions

typedef struct
{
   char *fileName;	   //文件名
   char *sheetName;	   //sheet表名称
   char *activeRange;  //范围
   int columnNum;//列数
   void *onExcelTaskStartListener;//任务开始回调函数
   void *onCellListener;//读取单元格回调函数
   void *onRowListener; //读取一行回调函数
   void *onExcelTaskFinishListener;//解析完成回调函数
   void *onErrorListener;//解析出错
   void *dataPtr;           //辅助数据
}EXCELTask;	

#define ERROR_EXCEL_GET_SHEET  -1
#define ERROR_EXCEL_GET_SHEET_NAME -2
#define ERROR_EXCEL_ACTIVE_SHEET  -3
#define ERROR_EXCEL_NO_SPCIFIC_SHEET  -4 
#define ERROR_EXCEL_GET_VARIANT  -5 

#define EXCEL_TASK_OK  0
#define EXCEL_TASK_QUIT -1
#define EXCEL_TASK_ERROR -2


		

int GetAppInitExcelAppHandle(void);

ExcelObj_Worksheet GetWorkingSheet(char *fileName,char *sheetName);

HRESULT ClearWorkingSheet(ExcelObj_Worksheet *ExcelWorkbookHandlePtr);

HRESULT InitExcel(void);

HRESULT DeInitExcel(void);

HRESULT ClearObjHandler(CAObjHandle *objHandle);

HRESULT SaveWorkingSheet(char *fileName);

typedef HRESULT (*ON_EXCEL_TASK_START_LISTENER)(void);

typedef HRESULT (*ON_CELL_LISTENER)(VARIANT *myVariant,int row,int column); 

typedef HRESULT (*ON_ROW_LISTENER)(void);

typedef HRESULT (*ON_EXCEL_TASK_FINISH_LISTENER)(void);

typedef void (*ON_EXCEL_TASK_ERROR)(char *message,int data1,int data2);

HRESULT CreateExcelJob(char *fileName,char *sheetName,char *activeRange,int columnNum,void *onCellListener); 

EXCELTask createExcelTask(char *fileName,char *sheetName,char *activeRange,int columnNum);

HRESULT runExcelTask(EXCELTask task);

ExcelObj_Range InitExcelRangeHandle(ExcelObj_Worksheet sheetHandle,char *Range);

int readSingleExcelRow(char *fileName1,char *sheetName,char *range,VARIANT *array,int row,int columnNum);

VARIANT *readExcelRectangle(char *fileName1,char *sheetName,char *range,int columnNum,int rowNum);

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __excelHelper_H__ */
