/*---------------------------------------------------------------------------
 // 版权声明：本文件由广东万城万充电动车运营股份有限公司版权所有，未经授权，
 // 禁止第三方进行拷贝和使用。
 //
 // 文件名：
 // 文件功能描述: 
 //
 // 
 // 创建标识：曾衍仁 
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
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
   HashTableCreate(10,FIXED_SIZE_KEY,sizeof(int),sizeof(RESULT),tablePtr);
   return 0;
}

HRESULT saveResult(HashTableType table,RESULT *resPtr)
{
	int found; 
	/*HashTableFindItem(table,indexPtr,&found); 
	//printf("testItem.index=%d",testItem.index);
	if(found==0)//没有则插入一个，不用释放内存
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

HRESULT saveResultWithErrorCheck(HashTableType table,RESULT *resPtr)
{
	int found; 
	/*HashTableFindItem(table,indexPtr,&found); 
	//printf("testItem.index=%d",testItem.index);
	if(found==0)//没有则插入一个，不用释放内存
	{
	   groupResult.dataLen=4*ListNumItems(testItem.subItems);
	   groupResult.data=(char *)malloc(groupResult.dataLen);
	}else
	{
	   HashTableGetItem(device.resultHashTable,&(testItem.index),&groupResult,sizeof(result));
	}*/
	if(strlen(resPtr->recvString)==0)
	{
		resPtr->pass = 0;
	}	
	HashTableInsertItem(table,&(resPtr->index),resPtr);
	return 0;

}

