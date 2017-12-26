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
#include "tpsHelper.h"
#include "resultSave.h"

METHODRET frameTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		result itemResult;
		itemResult.index=item.itemId;
		itemResult.pass=1%2;
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s","ͨ��");
		saveResult(hashTable,&itemResult);
		
	}
		
	return ret;
}



TPS registerframeTestTPS(void)
{
	TPS tps=newTps("fram");
	tps.autoTestFunction=frameTest;
	tps.manualTestFunction=frameTest;
	return tps;
}