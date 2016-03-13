/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ��ݺ��º��տƼ��ɷ����޹�˾��Ȩ���У�δ����Ȩ����ֹ����
 // �����п�����ʹ�á�
 //
 // �ļ�����excelHelper.c
 // �ļ���������:
 //
 // 
 // ������ʶ�������� 20141114
 //
 // �޸ı�ʶ��������  20150804
 // �޸�����������excel��������
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
   char *fileName;	   //�ļ���
   char *sheetName;	   //sheet������
   char *activeRange;  //��Χ
   int columnNum;//����
   void *onExcelTaskStartListener;//����ʼ�ص�����
   void *onCellListener;//��ȡ��Ԫ��ص�����
   void *onRowListener; //��ȡһ�лص�����
   void *onExcelTaskFinishListener;//������ɻص�����
   void *onErrorListener;//��������
   void *dataPtr;           //��������
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

ExcelObj_Range InitExcelRangeHandle(ExcelObj_Worksheet sheetHandle,char *Range) ;

#ifdef __cplusplus
    }
#endif

#endif  /* ndef __excelHelper_H__ */
