/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ㶫������綯����Ӫ�ɷ����޹�˾��Ȩ���У�δ����Ȩ��
 // ��ֹ���������п�����ʹ�á�
 //
 // �ļ�����
 // �ļ���������: 
 //
 // 
 // ������ʶ�������� 
 //
 // �޸ı�ʶ��
 // �޸�������
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

