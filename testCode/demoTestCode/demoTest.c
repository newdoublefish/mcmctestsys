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
#include "postData.h"
#include "sutCommon.h"
#include "httpPost.h"

METHODRET DemoTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;
		if(AlertDialogWithRet(0,"请选择","请确认结果是否正确","错误","正确")==TRUE)
		{
			itemResult.pass=1;	
		}else
			itemResult.pass=0;
		APPEND_INFO_FORMAT(msgPanel,"测试条例:%s的测试结果为：%s",item.itemName_,(itemResult.pass==1)?"合格":"不合格");
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s","helloworld");
		saveResult(hashTable,&itemResult);
		
	}
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);		
	return ret;
}



TPS registerDemoTestTPS(void)
{
	TPS tps=newTps("demoType");
	//tps.autoTestFunction=DemoTest;
	tps.testFunction=DemoTest;
	tps.createTpsPanel=NULL;
	//tps.manualTestFunction=DemoTest;
	return tps;
}

METHODRET HttpPostTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		tPostData data;
		if(TRUE==getPostData(&data,item.itemName_))
		{
			char buffer[512]={0};
			buildPostDataStr(data,buffer,NULL);
			if(1==httpPostJson(data.url,buffer))
			{
				itemResult.pass = RESULT_PASS;
			}
		}
		saveResult(hashTable,&itemResult);
		
	}
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);		
	return ret;
}



TPS registerHttpPostTPS(void)
{
	TPS tps=newTps("post");
	//tps.autoTestFunction=DemoTest;
	tps.testFunction=HttpPostTest;
	tps.createTpsPanel=NULL;
	//tps.manualTestFunction=DemoTest;
	return tps;
}
