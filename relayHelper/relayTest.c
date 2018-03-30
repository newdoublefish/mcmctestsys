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
#include "common.h"
#include "relayHelper.h"

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
		if((int)item.input_Value > 0)
			WarnAlert(0,"延时中....,按确定跳过",(int)item.input_Value);
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
	tps.createTpsPanel=NULL;
	//tps.manualTestFunction=DemoTest;
	return tps;
}

METHODRET OptoCouplerTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		itemResult.pass=RESULT_PASS;
		/*if((int)item.input_Value > 0)
			WarnAlert(0,"延时中....,按确定跳过",(int)item.input_Value);*/
		unsigned int value=0;
		CheckDoCheckOptoCoupler(eut.relayConfig,&value,0x0000FFFF);
		sprintf(itemResult.recvString,"0x%x",value);	
		saveResult(hashTable,&itemResult);
		
	}
	return ret;
}



TPS registerOptoCouplerTestTPS(void)
{
	TPS tps=newTps("OptoCoupler");
	tps.autoTestFunction=OptoCouplerTest;
	tps.createTpsPanel=NULL;
	//tps.manualTestFunction=DemoTest;
	return tps;
}
