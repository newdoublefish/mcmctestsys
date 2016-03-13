#ifndef __RESULTUTIL_H__
#define __RESULTUTIL_H__ 
#include <stdlib.h> 
#include "toolbox.h"

#ifdef __cplusplus
   extern "C" {
#endif

#define RESULT_RECEIVE_LEN 50
	   
typedef struct
{
  int index;//�������
  double recevValue;
  char recvString[RESULT_RECEIVE_LEN];
  int pass;
} result; //���� ���ڿ��ٱȽ�

HRESULT createResultHashTable(HashTableType *table);

HRESULT saveResult(HashTableType table,result *resPtr);

#ifdef __cplusplus
   }
#endif


#endif
