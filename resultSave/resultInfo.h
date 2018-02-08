#ifndef __RESULT_INFO_H__
#define __RESULT_INFO_H__
#include <stdlib.h> 
#include "toolbox.h"

#ifdef __cplusplus
   extern "C" {
#endif
	   
//void saveTestResult(char *time,char *dirName,HashTableType hashTable);
BOOL loadResultInfo(char *fileName,char *deviceName,HashTableType hashTable,char *testProjectName); 
BOOL saveResultInfo(TESTengine *gEngine,char *testProjectName);

#ifdef __cplusplus
   }
#endif
#endif
