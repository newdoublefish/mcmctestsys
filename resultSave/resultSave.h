#ifndef __RESULTSAVE_H__
#define __RESULTSAVE_H__
#include <stdlib.h> 
#include "toolbox.h"

#ifdef __cplusplus
   extern "C" {
#endif
#include "testFramework.h"	   
//void saveTestResult(char *time,char *dirName,HashTableType hashTable);
void saveTestResult(char *time,char *dirName,HashTableType hashTable,char *fileName); 
void saveResultToExcelFile(char *fileName,TESTobject testObj);

#ifdef __cplusplus
   }
#endif

#endif
