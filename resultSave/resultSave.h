#ifndef __RESULTSAVE_H__
#define __RESULTSAVE_H__
#include <stdlib.h> 
#include "toolbox.h"

#ifdef __cplusplus
   extern "C" {
#endif
	   
void saveTestResult(char *time,char *dirName,HashTableType hashTable);

#ifdef __cplusplus
   }
#endif

#endif
