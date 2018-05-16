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
#include "common.h"
#include "postData.h"
#include "sutCommon.h"
#include "httpPost.h"
#include "EutHelper.h"
#include "meter.h"
#include "ParamPanel.h"


METHODRET DemoTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"��ʼ����:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;
		if(AlertDialogWithRet(0,"��ѡ��","��ȷ�Ͻ���Ƿ���ȷ","����","��ȷ")==TRUE)
		{
			itemResult.pass=1;	
		}else
			itemResult.pass=0;
		APPEND_INFO_FORMAT(msgPanel,"��������:%s�Ĳ��Խ��Ϊ��%s",item.itemName_,(itemResult.pass==1)?"�ϸ�":"���ϸ�");
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s","helloworld");
		saveResult(hashTable,&itemResult);
		
	}
	APPEND_INFO_FORMAT(msgPanel,"%s�������",group.groupName);		
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
	APPEND_INFO_FORMAT(msgPanel,"��ʼ����:%s",group.groupName); 
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
			buildPostDataStr(data,buffer,NULL,NULL);
			if(1==httpPostJson(data.url,buffer))
			{
				itemResult.pass = RESULT_PASS;
			}
		}
		saveResult(hashTable,&itemResult);
		
	}
	APPEND_INFO_FORMAT(msgPanel,"%s�������",group.groupName);		
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

static int CVICALLBACK MeterPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			int *quit = (int*)callbackData;	
			*quit = 0;
		     break;
	}
	return 0;
}


METHODRET MeterTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	//APPEND_INFO_FORMAT(msgPanel,"��ʼ����:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"���",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}
	//char address[] = "333333333333";
	
	//InstallCtrlCallback(msgPanel,
	int quit = 1; 
	InstallPanelCallback(msgPanel,MeterPanelCallback,&quit);
	
	while(quit)
	{
		for(int i=1;i<=ListNumItems(group.subItems);i++)
		{
			TestItem item;
			ListGetItem(group.subItems,&item,i);
			RESULT itemResult={0};
			itemResult.index=item.itemId;
			float value=0;
			char address[] = "AAAAAAAAAAAA";
			if(TRUE==getMeterDlt2007Voltage(resconfig,address,&value))
			{
				itemResult.pass = RESULT_PASS;
				sprintf(itemResult.recvString,"%f",value);
				SetCtrlVal (msgPanel, METER_VOLTAGE, value);
				SetCtrlVal(msgPanel,METER_MSG,"��ѹֵ:");
				SetCtrlVal(msgPanel,METER_MSG,itemResult.recvString);
				SetCtrlVal(msgPanel,METER_MSG,"\n");
			}
			
			if(quit == 0)
				continue;
			
			if(TRUE == getMeterDlt2007Current(resconfig,address,&value))
			{
				itemResult.pass = RESULT_PASS;
				sprintf(itemResult.recvString,"%f",value);
				SetCtrlVal (msgPanel, METER_CURRENT, value);
				SetCtrlVal(msgPanel,METER_MSG,"����ֵ:");
				SetCtrlVal(msgPanel,METER_MSG,itemResult.recvString);
				SetCtrlVal(msgPanel,METER_MSG,"\n");				
			}
			saveResult(hashTable,&itemResult);
		
		}
	}
	//APPEND_INFO_FORMAT(msgPanel,"%s�������",group.groupName);		
	return ret;
}


int CreateMeterTpsPanel(char *groupName)
{
	int panelHandle =  LoadPanel(0,"ParamPanel.uir",METER);
	SetPanelAttribute(panelHandle,ATTR_TITLE,groupName);
	//SetCtrlVal(panelHandle,PANEL_TITLE,groupName);
	//InstallPanelCallback(panelHandle,oTpsPanelCallBack,NULL);  
	return panelHandle;
}



TPS registerMeterTPS(void)
{
	TPS tps=newTps("meter");
	tps.testFunction=MeterTest;
	tps.createTpsPanel=CreateMeterTpsPanel;
	//tps.manualTestFunction=DemoTest;
	return tps;
}
