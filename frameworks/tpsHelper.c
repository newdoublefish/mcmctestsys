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
static ListType tpsList;
extern TPS registerDemoTestTPS(void);
extern TPS registerframeTestTPS(void);
extern TPS registerCanTestTPS(void);
extern TPS registerSpiFlashTestTPS(void);
extern TPS registerResistanceTestTPS(void);
extern TPS registerVoltageTestTPS(void);


static REGISTER_TPS_FUNCTION registerTpsFunctionGroup[]=
{
	//registerDemoTestTPS,
	//registerframeTestTPS,
	//registerCanTestTPS,
	//registerSpiFlashTestTPS,
	registerResistanceTestTPS,
	registerVoltageTestTPS,
};


int initTps()
{
     if(tpsList==0)
		 tpsList=ListCreate(sizeof(TPS));

	 for(int i=0;i<sizeof(registerTpsFunctionGroup)/4;i++)
	 {
		 TPS tps=(*(registerTpsFunctionGroup[i]))();
		 ListInsertItem(tpsList,&tps,END_OF_LIST);

	 }
	 
	 
	 return 1;
	 
}


TEST_METHOD tpsSimuTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	   for(int k=1;k<=ListNumItems(group.subItems);k++)
	   {
		  result itemResult;
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
			   tpsPtr->manualTestFunction=(TEST_METHOD)tpsSimuTest;			 
			 }
		     return 1;
		 }
	 }
	 return 0;
}

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

TPS newTps(char *name)
{
	
	TPS tps;
	memset(&tps,0,sizeof(TPS));
	sprintf(tps.tpsName,"%s",name);
	tps.autoTestFunction=NULL;
	tps.manualTestFunction=NULL;
	tps.protocolInit=NULL;
	tps.testPrepare=NULL;
	return tps;    
}


