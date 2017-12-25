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
#include <formatio.h>
#include <rs232.h>
#include "tpsHelper.h"
#include "resultSave.h"

METHODRET CanTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
/*	for(int i=1;i<=ListNumItems(group.subItems);i++)
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
*/
	char buffer[512]={0};
	int RS232Error=0;
    RS232Error = OpenComConfig (eut.matainConfig.portNum,"",eut.matainConfig.baudRate, eut.matainConfig.parity,
                                        eut.matainConfig.dataBit, eut.matainConfig.stopBit, 0, 0);
	Fmt(buffer,"can 0\n");
	int bytes = ComWrt (eut.matainConfig.portNum, "can 0\n", StringLength(buffer));
	Delay (1);
	bytes=ComRd (eut.matainConfig.portNum , buffer, 512);
	printf("%s\n",buffer);
	char *findStr=strstr(buffer,"CAN 02 R = OK");
	//if(findStr!=NULL)
	//	printf("testStr1:%s\n",st);	
	
	CloseCom(eut.matainConfig.portNum);
	return ret;
}



TPS registerCanTestTPS(void)
{
	TPS tps=newTps("can");
	tps.autoTestFunction=CanTest;
	tps.manualTestFunction=CanTest;
	return tps;
}
