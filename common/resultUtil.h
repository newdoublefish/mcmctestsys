#ifndef __RESULTUTIL_H__
#define __RESULTUTIL_H__ 
#include <stdlib.h> 
#include "toolbox.h"

#ifdef __cplusplus
   extern "C" {
#endif

#define RESULT_RECEIVE_LEN 50
	   
#define RESULT_PASS 1
#define RESULT_FAIL 0
#define RESULT_NON  2	   
	   
typedef struct
{
  int index;//条例序号
  double recevValue;
  char recvString[RESULT_RECEIVE_LEN];
  int pass;
} RESULT; //组结果 用于快速比较

HRESULT createResultHashTable(HashTableType *table);

HRESULT saveResult(HashTableType table,RESULT *resPtr);

#ifdef __cplusplus
   }
#endif


#endif
