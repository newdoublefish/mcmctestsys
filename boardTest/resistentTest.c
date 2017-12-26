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
#include "scpiHelper.h"

typedef struct {
	TestItem item;
	result res;
}tTEST_RESULT;



void ComCallback(int portNumber, int eventMask, void *callbackdata)
{

//	if (eventMask & LWRS_RXFLAG)
	char	readBuf[256] = {0};
	int	    strLen=0;
	strLen = GetInQLen (portNumber);
	ComRd (portNumber, readBuf, strLen); 
	//printf("%s",readBuf);
	tTEST_RESULT *tr = (tTEST_RESULT *)callbackdata;
	sprintf(tr->res.recvString,"%s",readBuf);
	tr->res.pass = 1; 

}


void resistanceTestCallback(TestItem item,result *resultPtr,int comPort)
{

	int bytes=0;
	if(strcmp("ABCN相连对地",item.itemName_)==0)
	{
		scpiDispPage(comPort,MSETUP);
		Delay(0.2);
		scpiSetType(comPort,IR);
		Delay(0.2);
		scpiSetVolt(comPort,0.5);
		Delay(0.2);
		scpiSetTestTime(comPort,5);
		Delay(0.2);
		scpiDispPage(comPort,MEASUREMENT);
		Delay(0.2);
		scpiStartTest(comPort);
		return;
	}else if(strcmp("ABCN相连对DC12V",item.itemName_)==0) 
	{
		scpiDispPage(comPort,MSETUP);
		Delay(0.2);
		scpiSetType(comPort,IR);
		Delay(0.2);
		scpiSetVolt(comPort,0.5);
		Delay(0.2);
		scpiSetTestTime(comPort,5);
		Delay(0.2);
		scpiDispPage(comPort,MEASUREMENT);
		Delay(0.2); 
		scpiStartTest(comPort);
		return;
	}else if(strcmp("交流输入对直流输出",item.itemName_)==0) 
	{
		scpiDispPage(comPort,MSETUP);
		Delay(0.2);
		scpiSetType(comPort,IR);
		Delay(0.2);
		scpiSetVolt(comPort,1);
		Delay(0.2);
		scpiSetTestTime(comPort,5);
		Delay(0.2);
		scpiDispPage(comPort,MEASUREMENT);
		Delay(0.2); 
		scpiStartTest(comPort); 
		return;
	}else{
		resultPtr->pass=0;
	}
}

METHODRET resistanceTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	char buffer[512]={0};
	int RS232Error=0;
	int flag=0;
	int eventMask = LWRS_RXFLAG;
	int eventChar = 10;//LN	

    RS232Error = OpenComConfig (eut.matainConfig.portNum,"",eut.matainConfig.baudRate, eut.matainConfig.parity,
                                        eut.matainConfig.dataBit, eut.matainConfig.stopBit, 0, 0);
	
	 

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		//TestItem item;
		tTEST_RESULT tTestResult={0};
		
		ListGetItem(group.subItems,&tTestResult.item,i);
		tTestResult.res.index=tTestResult.item.itemId;
		tTestResult.res.pass=-1;
		InstallComCallback(eut.matainConfig.portNum,eventMask,0,eventChar,ComCallback, &tTestResult);
		resistanceTestCallback(tTestResult.item,&tTestResult.res,eut.matainConfig.portNum);
		while(tTestResult.res.pass==-1)
		{
			Delay(1);
			ProcessSystemEvents ();
		}
		saveResult(hashTable,&tTestResult.res);
	}
	
	CloseCom(eut.matainConfig.portNum);
	return ret;
}



TPS registerResistanceTestTPS(void)
{
	TPS tps=newTps("resistance");
	tps.autoTestFunction=resistanceTest;
	//tps.manualTestFunction=resistanceTest;
	return tps;
}
