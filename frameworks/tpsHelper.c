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
#include "toolbox.h"
#include "tpsHelper.h"
#include "settingConfig.h"
#include "resultUtil.h" 
#include "testGroupInit.h"
#include "TpsDefaultPanel.h"
#include "ansi_c.h"
static ListType tpsList;
extern TPS registerDemoTestTPS(void);
extern TPS registerHttpPostTPS(void);
extern TPS registerframeTestTPS(void);
extern TPS registerCanTestTPS(void);
extern TPS registerSpiFlashTestTPS(void);
extern TPS registerResistanceTestTPS(void);
extern TPS registerVoltageTestTPS(void);
extern TPS registerRs232TestTPS(void);
extern TPS registerParamTestTPS(void);  
extern TPS registerParamsCheckTps(void);
extern TPS registerParamTemperatureTps(void);
extern TPS registerBIBOTestTps(void);
extern TPS registerPowerDistributeTestTps(void);
extern TPS registerScanTestTps(void);
extern TPS registerInverseWarnTestTps(void);
extern TPS registerInsulationTestTestTps(void);
extern TPS registerChargingTestTestTps(void);
extern TPS registerTimeSetTestTps(void);
extern TPS registerRelayTestTPS(void);
extern TPS registerStubCmdTestTps(void);
extern TPS registerStubIoTestTps(void);
extern TPS registerCtrlBoxSetTestTps(void);
extern TPS registerPowerModuleTestTps(void);
extern TPS registerGunSelectedTestTps(void);
extern TPS registerNetWorkCheckTestTps(void);
extern TPS registerBoardTestTPS(void);
extern TPS registerCanTestTPS(void);
extern TPS registerSpiAdcTestTPS(void);
extern TPS registerAdcTestTPS(void);
extern TPS registerRTCTestTPS(void);
extern TPS registerRS485TestTPS(void);
extern TPS registerLedDispTestTPS(void);
extern TPS registerBoardDoTestTPS(void);
extern TPS registerUartK60TestTPS(void);
extern TPS registerBoardk64TestTPS(void);
extern TPS registerUsbTestTPS(void);  
extern TPS registerLedDispCardTestTPS(void);
extern TPS registerUpdateTestTPS(void);
extern TPS registerParamsSetTps(void);
extern TPS register120KWPowerModuleTestTps(void);
extern TPS registerCodeInputTestTPS(void);



static REGISTER_TPS_FUNCTION registerTpsFunctionGroup[]=
{
	registerDemoTestTPS,
	registerHttpPostTPS,
	//可靠性
	registerResistanceTestTPS,  //可靠性，安规仪
	//整机测试
	registerParamTestTPS,
	registerParamsCheckTps,
	registerParamsSetTps,
	registerParamTemperatureTps,
	registerPowerDistributeTestTps,	 //功率分配
	registerScanTestTps,	  //扫码测试
	registerInverseWarnTestTps,//反接告警
	registerInsulationTestTestTps,//绝缘检测
	registerChargingTestTestTps,//充电流程
	registerTimeSetTestTps,//时间设置
	registerRelayTestTPS,
	registerStubCmdTestTps,
	registerStubIoTestTps,//手动解锁
	registerBIBOTestTps,//BIBO
	registerCtrlBoxSetTestTps,//控制盒编号设置
	registerPowerModuleTestTps,//电源模块测试
	registerGunSelectedTestTps,//枪选择
	registerNetWorkCheckTestTps,//网络检查
	register120KWPowerModuleTestTps, 
	//板级测试
	registerBoardTestTPS,
	registerCanTestTPS,
	registerUartK60TestTPS, 
	registerBoardDoTestTPS,   
	registerSpiAdcTestTPS,
	registerAdcTestTPS,
	registerBoardk64TestTPS,  
	registerRTCTestTPS,
	registerRS485TestTPS,
	registerLedDispTestTPS,
	registerLedDispCardTestTPS,
	registerUsbTestTPS, 
	registerUpdateTestTPS, 
	
	registerCodeInputTestTPS,//条形码录入
};


int initTps()
{
     if(tpsList==0)
		 tpsList=ListCreate(sizeof(TPS));

	 for(int i=0;i<sizeof(registerTpsFunctionGroup)/4;i++)
	 {
		 
		 TPS tps=(*(registerTpsFunctionGroup[i]))();
		 if(isTypeGroupExsit(tps.tpsName)) 
		 	ListInsertItem(tpsList,&tps,END_OF_LIST);

	 }
	 
	 
	 return 1;
	 
}


TEST_METHOD tpsSimuTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	   for(int k=1;k<=ListNumItems(group.subItems);k++)
	   {
		  RESULT itemResult;
		  TestItem subItem;
		  ListGetItem(group.subItems,&subItem,k); 
		  itemResult.pass=RESULT_PASS;

		  memset(itemResult.recvString,0,20);
		  sprintf(itemResult.recvString,"%s","pass");
		  HexString2UpperCase(itemResult.recvString); //转换大小写 
		  itemResult.index=subItem.itemId;
		  saveResult(hashTable,&itemResult);
	   }	 
	 return TEST_RESULT_ALLPASS;
}

TEST_METHOD tpsSimuFunction(TestGroup group,EUT eut,HashTableType hashTable,int panelHandle)
{
	   for(int k=1;k<=ListNumItems(group.subItems);k++)
	   {
		  RESULT itemResult;
		  TestItem subItem;
		  ListGetItem(group.subItems,&subItem,k);
		  int r = rand();
		  if(r%2==0)
		  {
		  	itemResult.pass=RESULT_PASS;
		  }else{
		  	itemResult.pass=RESULT_FAIL;
		  }

		  memset(itemResult.recvString,0,20);
		  sprintf(itemResult.recvString,"%s",subItem.itemName_);
		  HexString2UpperCase(itemResult.recvString); //转换大小写 
		  itemResult.index=subItem.itemId;
		  saveResult(hashTable,&itemResult);
	   }	 
	 return TEST_RESULT_ALLPASS;
}


int getTps(char *tpsName,TPS *tpsPtr)
{
	 SETTING s=GetSetting(); 
	 int tpsCnt=ListNumItems(tpsList);
	 memset(tpsPtr,0,sizeof(TPS));
	if(s.simuTest>0){
		tpsPtr->testFunction=(TEST_FUNCTION)tpsSimuFunction;
		tpsPtr->createTpsPanel=NULL;
		return 1;
	}
		     	 
	 for(int i=1;i<=tpsCnt;i++)
	 {
		 ListGetItem(tpsList,tpsPtr,i);
		 //printf("getTps:current tpsName:%s\n",tpsPtr->tpsName);
		 if(strcmp(tpsName,tpsPtr->tpsName)==0)
		 {
		     return 1;
		 }
	 }
	 return 0;
}

//TODO:该功能是否应该存在
void TpsPrepareTest(void)
{
	SETTING st=GetSetting();  
	if(st.simuTest>0)
		 return;
	int tpsCnt=ListNumItems(tpsList);
	TPS tps;
    for(int i=1;i<=tpsCnt;i++)
	{
	    ListGetItem(tpsList,&tps,i);
		if(tps.testPrepare!=NULL)
		{
		     (*(TEST_PREPARE)(tps.testPrepare))();
		}
	}
}

void TpsInitProtocol()
{
	int tpsCnt=ListNumItems(tpsList);
	TPS tps;
    for(int i=1;i<=tpsCnt;i++)
	{
	    ListGetItem(tpsList,&tps,i);
		if(tps.protocolInit!=NULL)
		{
		     (*(PROTOCOL_INIT)(tps.protocolInit))(tps.tpsName);
		}
	}	
}

void AppendInfo(int tpsPanelHandle,char *msg)
{
	if(tpsPanelHandle > 0)
		SetCtrlVal(tpsPanelHandle,PANEL_TEXTBOX,msg);	
}

int CVICALLBACK oTpsPanelCallBack (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}

int CreateTpsPanel(char *groupName)
{
	int panelHandle =  LoadPanel(0,"TpsDefaultPanel.uir",PANEL);
	SetCtrlVal(panelHandle,PANEL_TITLE,groupName);
	//InstallPanelCallback(panelHandle,oTpsPanelCallBack,NULL);  
	return panelHandle;
}


TPS newTps(char *name)
{
	TPS tps;
	memset(&tps,0,sizeof(TPS));
	sprintf(tps.tpsName,"%s",name);
	tps.autoTestFunction=NULL;
	tps.protocolInit=NULL;
	tps.testPrepare=NULL;
	tps.onTestFinish=NULL;
	tps.createTpsPanel=CreateTpsPanel;
	tps.testFunction=NULL;
	tps.tpsPanelHandle=-1;
	return tps;    
}


