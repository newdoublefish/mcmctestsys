#include "tpsHelper.h"
#include "resultSave.h"

METHODRET DemoTest(TestGroup group,EUT eut,HashTableType hashTable)
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
		sprintf(itemResult.recvString,"%s","helloworld");
		saveResult(hashTable,&itemResult);
		
	}
		
	return ret;
}



TPS registerDemoTestTPS(void)
{
	TPS tps=newTps("demoType");
	tps.testFunction=(void *)DemoTest;
	return tps;
}
