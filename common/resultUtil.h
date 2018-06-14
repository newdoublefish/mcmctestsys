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
  int index;//�������
  double recevValue;
  char recvString[RESULT_RECEIVE_LEN];
  int pass;
} RESULT; //���� ���ڿ��ٱȽ�

HRESULT createResultHashTable(HashTableType *table);

HRESULT saveResult(HashTableType table,RESULT *resPtr);
HRESULT saveResultWithErrorCheck(HashTableType table,RESULT *resPtr);  

#ifdef __cplusplus
   }
#endif


#endif
