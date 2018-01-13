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
#include "tpsHelper.h"
#include "resultSave.h"

METHODRET RelayTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;
		itemResult.pass=1;
		if(item.input_Value>0)
			Delay(item.input_Value);
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s","OK");
		saveResult(hashTable,&itemResult);
		
	}
	return ret;
}



TPS registerRelayTestTPS(void)
{
	TPS tps=newTps("Relay");
	tps.autoTestFunction=RelayTest;
	//tps.manualTestFunction=DemoTest;
	return tps;
}
