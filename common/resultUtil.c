#include <cvirte.h>		
#include <userint.h>
#include <stdlib.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "resultUtil.h"


HRESULT createResultHashTable(HashTableType *tablePtr)
{
   HashTableCreate(10,FIXED_SIZE_KEY,sizeof(int),sizeof(result),tablePtr);
   return 0;
}

HRESULT saveResult(HashTableType table,result *resPtr)
{
	int found; 
	/*HashTableFindItem(table,indexPtr,&found); 
	//printf("testItem.index=%d",testItem.index);
	if(found==0)//û�������һ���������ͷ��ڴ�
	{
	   groupResult.dataLen=4*ListNumItems(testItem.subItems);
	   groupResult.data=(char *)malloc(groupResult.dataLen);
	}else
	{
	   HashTableGetItem(device.resultHashTable,&(testItem.index),&groupResult,sizeof(result));
	}*/
	HashTableInsertItem(table,&(resPtr->index),resPtr);
	return 0;

}

