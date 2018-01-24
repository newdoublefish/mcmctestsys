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
#include "toolbox.h"
#include "tpsHelper.h"
#include "settingConfig.h"
#include "resultUtil.h" 
#include "testGroupInit.h"
#include "TpsDefaultPanel.h"
static ListType tpsList;
extern TPS registerDemoTestTPS(void);
extern TPS registerframeTestTPS(void);
extern TPS registerCanTestTPS(void);
extern TPS registerSpiFlashTestTPS(void);
extern TPS registerResistanceTestTPS(void);
extern TPS registerVoltageTestTPS(void);
extern TPS registerRs232TestTPS(void);
extern TPS registerParamTestTPS(void);  
extern TPS registerParamsCheckTps(void);
extern TPS registerParamTemperatureTps(void);
extern TPS registerBiboTestTps(void);
extern TPS registerScanTestTps(void);
extern TPS registerInverseWarnTestTps(void);
extern TPS registerInsulationTestTestTps(void);
extern TPS registerChargingTestTestTps(void);
extern TPS registerTimeSetTestTps(void);
extern TPS registerRelayTestTPS(void);
extern TPS registerStubCmdTestTps(void);
extern TPS registerStubIoTestTps(void);


static REGISTER_TPS_FUNCTION registerTpsFunctionGroup[]=
{
	registerDemoTestTPS,
	//registerframeTestTPS,
	//registerCanTestTPS,
	//registerSpiFlashTestTPS,
	registerResistanceTestTPS,
	registerRs232TestTPS,
	registerParamTestTPS,
	registerParamsCheckTps,
	registerParamTemperatureTps,
	registerBiboTestTps,	 //���ʷ���
	registerScanTestTps,	  //ɨ�����
	registerInverseWarnTestTps,//���Ӹ澯
	registerInsulationTestTestTps,//��Ե���
	registerChargingTestTestTps,//�������
	registerTimeSetTestTps,//ʱ������
	registerRelayTestTPS,
	registerStubCmdTestTps,
	registerStubIoTestTps,//�ֶ�����
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
		  itemResult.pass=1;

		  memset(itemResult.recvString,0,20);
		  sprintf(itemResult.recvString,"%s","pass");
		  HexString2UpperCase(itemResult.recvString); //ת����Сд 
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
		  itemResult.pass=1;

		  memset(itemResult.recvString,0,20);
		  sprintf(itemResult.recvString,"%s","pass");
		  HexString2UpperCase(itemResult.recvString); //ת����Сд 
		  itemResult.index=subItem.itemId;
		  saveResult(hashTable,&itemResult);
	   }	 
	 return TEST_RESULT_ALLPASS;
}


int getTps(char *tpsName,TPS *tpsPtr)
{
	 SETTING st=GetSetting(); 
	 int tpsCnt=ListNumItems(tpsList);
	 memset(tpsPtr,0,sizeof(TPS));
	 for(int i=1;i<=tpsCnt;i++)
	 {
		 ListGetItem(tpsList,tpsPtr,i);
		 //printf("getTps:current tpsName:%s\n",tpsPtr->tpsName);
		 if(strcmp(tpsName,tpsPtr->tpsName)==0)
		 {
			 if(st.simuTest>0){
			   tpsPtr->autoTestFunction=(TEST_METHOD)tpsSimuTest;
			    tpsPtr->testFunction=(TEST_FUNCTION)tpsSimuFunction;
//			   tpsPtr->manualTestFunction=(TEST_METHOD)tpsSimuTest;			 
			 }
		     return 1;
		 }
	 }
	 return 0;
}

//TODO:�ù����Ƿ�Ӧ�ô���
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


