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
#include <tcpsupp.h>
#include "tpsHelper.h"
#include "resultSave.h"
#include "ParamExcelParse.h"
#include "StubNetService.h"
#include "common.h"
#include "ParamPanel.h"
#include "BiboExcelParse.h"
#include "relayHelper.h"

int CVICALLBACK showPramSetCtrlCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}

void showParamSetPanel(ListType paramsToSet)
{
	int panelHandle = LoadPanel(0,"ParamPanel.uir",PANEL);
	InsertTableRows (panelHandle,PANEL_TABLE,-1,ListNumItems(paramsToSet),VAL_USE_MASTER_CELL_TYPE); 
	InstallCtrlCallback(panelHandle,PANEL_COMMANDBUTTON,showPramSetCtrlCallback,&paramsToSet);
	for(int i=1;i<=ListNumItems(paramsToSet);i++)
	{
		PARAMETER param={0};
		ListGetItem(paramsToSet,&param,i);
		SetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(1,i),param.paramName);
		SetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(2,i),param.value);
		
	}
	SetActiveTableCell (panelHandle, PANEL_TABLE, MakePoint(2,1));
	FakeKeystroke(VAL_F2_VKEY); 
	DisplayPanel(panelHandle);
	RunUserInterface (); 
	for(int i=1;i<=ListNumItems(paramsToSet);i++)
	{
		PARAMETER param={0};
		ListGetItem(paramsToSet,&param,i);
		GetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(2,i),param.value);
		ListReplaceItem(paramsToSet,&param,i);
		
	}	
	DiscardPanel(panelHandle);
}

METHODRET ParamTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;
	
	ListType paramsToSet=ListCreate(sizeof(PARAMETER));
	ListType paramsToFetch=0;
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER param={0};
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"无该参数配置");
			goto DONE;
		}
		ListInsertItem(paramsToSet,&param,END_OF_LIST);
	}
	
	paramsToFetch = ListCopy(paramsToSet);
	
	showParamSetPanel(paramsToSet);  
	
	for(int i=1;i<=ListNumItems(paramsToSet);i++)
	{
		PARAMETER paramSet={0},paramGet={0};
		ListGetItem(paramsToSet,&paramSet,i);
		ListGetItem(paramsToFetch,&paramGet,i);
		ConfigParameter(servicePtr,paramSet);
		if(GetParameter(servicePtr,&paramGet)>=0)
		{
			ListReplaceItem(paramsToFetch,&paramGet,i);
		}else{
			WarnShow1(0,"设置错误!");
		}
	}
																		 
	//showParamSetPanel(paramsToFetch); 
	
	
	//设置参数
	//TODO:读回参数并对比
	//showParamSetPanel(paramsToSet);
	//showParamSetPanel(paramsToFetch);

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER paramSet={0};
		PARAMETER paramGet={0};
		ListGetItem(paramsToSet,&paramSet,i);
		ListGetItem(paramsToFetch,&paramGet,i); 
		RESULT itemResult;
		itemResult.index=item.itemId;
		if(strcmp(paramSet.value,paramGet.value)==0)
		{
			itemResult.pass=1;
		}else{
			itemResult.pass=0;
		}
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s",paramGet.value);
		saveResult(hashTable,&itemResult);
	}	
DONE:	
	ListDispose(paramsToSet);
	ListDispose(paramsToFetch);
	return ret;
}



TPS registerParamTestTPS(void)
{
	TPS tps=newTps("param");
	tps.autoTestFunction=ParamTest;
	tps.protocolInit=ParamProtocolInit;
	return tps;
}


METHODRET ParamCheckTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;
	*/
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;	
	ListType paramsToFetch=ListCreate(sizeof(PARAMETER));

	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER param={0};
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"无该参数配置");
			goto DONE;
		}
		ListInsertItem(paramsToFetch,&param,END_OF_LIST);
	}
	//TODO:读回参数
	//showParamSetPanel(paramsToFetch);
	//设置参数
	//发送自检命令
	startCommand(servicePtr,"product self_test\r\n");

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER paramGet={0};
		ListGetItem(paramsToFetch,&paramGet,i); 
		///printfParam(paramGet);
		
		if(GetParameter(servicePtr,&paramGet)<0)
		{	
			WarnShow1(0,"获取失败");
			goto DONE;
		}
		RESULT itemResult;
		itemResult.index=item.itemId;
		if(strcmp("true",paramGet.value)==0)
		{
			itemResult.pass=1;
		}else{
			itemResult.pass=0;
		}
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		//printf("----------%s\n",paramGet.value);
		sprintf(itemResult.recvString,"%s",paramGet.value);
		saveResult(hashTable,&itemResult);
	}	
DONE:	
	ListDispose(paramsToFetch);
	return ret;
}



TPS registerParamsCheckTps(void)
{
	TPS tps=newTps("paramCheck");
	tps.autoTestFunction=ParamCheckTest;
	tps.protocolInit=ParamProtocolInit;
	return tps;			
}

METHODRET ParamTemperatureTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;
	*/
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;	
	ListType paramsToFetch=ListCreate(sizeof(PARAMETER));

	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER param={0};
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"无该参数配置");
			goto DONE;
		}
		ListInsertItem(paramsToFetch,&param,END_OF_LIST);
	}
	//TODO:读回参数
	//showParamSetPanel(paramsToFetch);
	//设置参数

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER paramGet={0};
		ListGetItem(paramsToFetch,&paramGet,i); 
		///printfParam(paramGet);
		
		if(GetParameter(servicePtr,&paramGet)<0)
		{	
			WarnShow1(0,"获取失败");
			goto DONE;
		}
		RESULT itemResult;
		itemResult.index=item.itemId;
		/*if(strcmp("true",paramGet.value)==0)
		{
			itemResult.pass=1;
		}else{
			itemResult.pass=0;
		}*/
		itemResult.pass=1;
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		//printf("----------%s\n",paramGet.value);
		sprintf(itemResult.recvString,"%s",paramGet.value);
		saveResult(hashTable,&itemResult);
	}	
DONE:	
	ListDispose(paramsToFetch);
	return ret;
}

TPS registerParamTemperatureTps(void)
{
	TPS tps=newTps("temperature");
	tps.autoTestFunction=ParamTemperatureTest;
	tps.protocolInit=ParamProtocolInit;
	return tps;			
}


METHODRET ParamBiboTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;
	*/
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;	

	return ret;
}


TPS registerBiboTestTps(void)
{
	TPS tps=newTps("BIBO");
	tps.autoTestFunction=ParamBiboTest;
	tps.protocolInit=BiboProtocolInit;
	return tps;			
}

//枪二维码 "http://www.gdmcmc.cn/qrcode.html?qrcode=881011000851";
//电表地址 strAddress="1711424989000065";

int CVICALLBACK ParamScanCtrlCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_VAL_CHANGED:
			 //QuitUserInterface(1);
			 if(control == SCANPANEL_SCAN1)
			 {
			 	char temp[100]={0};
				GetCtrlVal(panel,control,temp);
				if(strstr(temp,"http")!=NULL)
				{
					if(strlen(temp)>=52)
					{
							char *strPtr=strstr(temp,"=");
							if(strPtr!=NULL)
							{
								strPtr=strPtr+1;
								SetCtrlVal(panel,control,strPtr);
								char *tempPtr=(char*)callbackData;
								memcpy(tempPtr,strPtr,strlen(strPtr)-1);
							}										
							SetActiveCtrl(panel,SCANPANEL_SCAN2);
						//printf("%s\n",temp);
					}
				}
			 
			 } else if(control == SCANPANEL_SCAN2)
			 {
			 	char temp[100]={0};
				GetCtrlVal(panel,control,temp);
				if(strstr(temp,"http")!=NULL)
				{
					if(strlen(temp)>=52)
					{
							char *strPtr=strstr(temp,"=");
							if(strPtr!=NULL)
							{
								strPtr=strPtr+1;
								SetCtrlVal(panel,control,strPtr);
								char *tempPtr=(char*)callbackData;
								memcpy(tempPtr,strPtr,strlen(strPtr)-1);
							}										
							SetActiveCtrl(panel,SCANPANEL_SCAN3);
						//printf("%s\n",temp);
					}
				}
			 
			 }else if(control == SCANPANEL_SCAN3)
			 {
			 	char temp[100]={0};
				GetCtrlVal(panel,control,temp);
				if(strlen(temp)>=16){
					SetCtrlVal(panel,control,temp+4);
					SetActiveCtrl(panel,SCANPANEL_SCAN4);
				}
				
			 
			 }else if(control == SCANPANEL_SCAN4){
				char temp[100]={0};
				GetCtrlVal(panel,control,temp);
				if(strlen(temp)>=16){
					SetCtrlVal(panel,control,temp+4);
					QuitUserInterface(1);
				}				
			 }
		     break;
	}
	return 0;
}

int CVICALLBACK ParaPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}


METHODRET ParaScanTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	char stubName[20]={0};
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;
	
	int panelHandle=LoadPanel(0,"ParamPanel.uir",SCANPANEL);
	
	InstallPanelCallback(panelHandle,ParaPanelCallback,NULL);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN1,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN2,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN3,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN4,ParamScanCtrlCallback,stubName); 	
	SetActiveCtrl(panelHandle,SCANPANEL_SCAN1);
	DisplayPanel(panelHandle); 
	RunUserInterface();
	int stubNameLen=strlen(stubName);
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		PARAMETER param={0};
		ListGetItem(group.subItems,&item,i);
		
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		if(stubNameLen>0)
			itemResult.pass=1;
		if(i==1)
		{
			GetCtrlVal(panelHandle,SCANPANEL_SCAN1,itemResult.recvString); 
		}else if(i==2){
			GetCtrlVal(panelHandle,SCANPANEL_SCAN2,itemResult.recvString);
		}else if(i==3)
		{
			if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"无该参数配置");
				continue;
			}
			sprintf(itemResult.recvString,"%s",stubName);
			sprintf(param.value,"00000%s",itemResult.recvString);
			if(ConfigParameter(servicePtr,param)<0)
				continue;			
			
			
		}else if(i==4)
		{
			if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"无该参数配置");
				continue;
			}
			GetCtrlVal(panelHandle,SCANPANEL_SCAN3,itemResult.recvString);
			sprintf(param.value,"%s",itemResult.recvString);
			if(ConfigParameter(servicePtr,param)<0)
				continue;
			
			
			
		}else if(i==5)
		{
			if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"无该参数配置");
				continue;
			}
						
			GetCtrlVal(panelHandle,SCANPANEL_SCAN4,itemResult.recvString);
			sprintf(param.value,"%s",itemResult.recvString);
			if(ConfigParameter(servicePtr,param)<0)
				continue;			
		}
		saveResult(hashTable,&itemResult);
	}	
	
	DiscardPanel(panelHandle);
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;*/	

	return ret;
}

TPS registerScanTestTps(void)
{
	TPS tps=newTps("scan");
	tps.autoTestFunction=ParaScanTest;
	return tps;	
}

METHODRET InverseWarnTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	int flag1=0;
	int flag2=0;
	int flag3=0;
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;
	*/
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;
	
	TestItem item;
	PARAMETER param={0};
	ListGetItem(group.subItems,&item,1);
	if(FALSE==getParameter(item.itemName_,&param))
	{
		WarnShow1(0,"无该参数配置");
	}
	if(GetParameter(servicePtr,&param)<0)
	{	
		WarnShow1(0,"获取失败");
		//goto DONE;
	}
	printf("-----------------%s\n",param.value);
	
	if(strcmp(param.value,"false")==0)
	{
		flag1=1;
	}
	
	OpenDo(eut.relayConfig,30);
	
	Delay(2);
	
	if(GetParameter(servicePtr,&param)<0)
	{	
		WarnShow1(0,"获取失败");
		//goto DONE;
	}
	printf("-----------------%s\n",param.value); 
	
	if(strcmp(param.value,"true")==0)
	{
		flag2=1;
	}	
	
	//showTips(0,"检查项目","请检查面板显示应充电枪故障指示灯显示应该为X"); 

	if(AlertDialogWithRet(0,"枪检查","请检查面板显示应充电枪故障指示灯显示应该为X","错误","正确")==FALSE)
	{
		flag2=0;
	}else{
		//goto DONE;
	}
	

	CloseDo(eut.relayConfig,30);
	Delay(2); 
	
	if(GetParameter(servicePtr,&param)<0)
	{	
		WarnShow1(0,"获取失败");
		//goto DONE;
	}
	printf("-----------------%s\n",param.value); 
	
	if(strcmp(param.value,"false")==0)
	{
		flag3=1;
	}	
	
	/*if(AlertDialogWithRet(0,"枪检查","请检查面板显示应充电枪故障指示灯显示应该消失","错误","正确")==FALSE)
	{
		flag3=0;
	}*/
	
	//合上开关31
	
	//printf("-----------------%s\n",param.value);
	printf("%d,%d,%d\n",flag1,flag2,flag3);
DONE:	
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	itemResult.pass = flag1 & flag2 & flag3;
	if(itemResult.pass)
		sprintf(itemResult.recvString,"%s","合格");
	else
		sprintf(itemResult.recvString,"%s","不合格");
	saveResult(hashTable,&itemResult);	

	return ret;
}

TPS registerInverseWarnTestTps(void)
{
	TPS tps=newTps("inverseWarn");
	tps.autoTestFunction=InverseWarnTest;
	return tps;	
}

METHODRET InsulationTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	TestItem item;
	ListGetItem(group.subItems,&item,1);
	//step1
	OpenDo(eut.relayConfig,29);
	CloseDo(eut.relayConfig,28);
	BOOL flag=TRUE;
	if(AlertDialogWithRet(0,"枪检查","请确认可以正常充电","错误","正确")==FALSE)
	{
		flag=FALSE;
	}
	
	//OpenDo(eut.relayConfig,29);
	OpenDo(eut.relayConfig,28);
	if(AlertDialogWithRet(0,"枪检查","请确认不能正常充电","错误","正确")==FALSE)
	{
		flag=FALSE;
	}
	CloseDo(eut.relayConfig,29);
	CloseDo(eut.relayConfig,28);
	
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	itemResult.pass=flag;
	saveResult(hashTable,&itemResult);
	return ret;
}


TPS registerInsulationTestTestTps(void)
{
	TPS tps=newTps("insulation");
	tps.autoTestFunction=InsulationTest;
	return tps;	
}

METHODRET ChargingTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	BOOL flag=TRUE;
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;
	//充电模块确认
	TestItem item1;
	ListGetItem(group.subItems,&item1,1);
	RESULT itemResult1={0};
	itemResult1.index=item1.itemId;
	itemResult1.pass=1; 
	if(AlertDialogWithRet(0,"枪检查","请确认充电流程已启动","错误","正确")==FALSE)
	{
		flag=FALSE;
		itemResult1.pass=0;
		saveResult(hashTable,&itemResult1);
		goto DONE;
	}
	
	saveResult(hashTable,&itemResult1);	
	
	OpenDo(eut.relayConfig,31);

	WarnShow1(0,"按下确定开始读数");
	
	ListType paramsToGet=ListCreate(sizeof(PARAMETER));
	ListType paramsToSet=ListCreate(sizeof(PARAMETER));
	for(int i=2;i<=4;i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);

		PARAMETER param={0};
		ListInsertItem(paramsToSet,&param,END_OF_LIST);
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"无该参数配置");
			//goto DONE;
		}
		if(GetParameter(servicePtr,&param)<0)
		{	
			WarnShow1(0,"获取失败");
				//goto DONE;
		}			
		printf("----------------------------%s\n",param.value);
		ListInsertItem(paramsToGet,&param,END_OF_LIST);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		sprintf(itemResult.recvString,"%s",param.value);
		float strValue=atof(param.value);
		if(i==2)
		{
			if(strValue<352 && strValue>348)
			{
				itemResult.pass=1;
			}
		}else if(i==3)
		{
			if(strValue<2.87 && strValue>2.67)
			{
				itemResult.pass=1;
			}
		}
		saveResult(hashTable,&itemResult);			
	}
		
	//ListType paramsToSet=ListCopy(paramsToGet);
	showParamSetPanel(paramsToSet);
	
	for(int i=1;i<=ListNumItems(paramsToSet);i++)
	{
		TestItem item;
		PARAMETER param={0};
		ListGetItem(paramsToSet,&param,i);
		ListGetItem(group.subItems,&item,i+4);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		sprintf(itemResult.recvString,"%s",param.value);
		float strValue=atof(param.value);
		if(i==1)
		{
			if(strValue<352 && strValue>348)
			{
				itemResult.pass=1;
			}
		}else if(i==2)
		{
			if(strValue<2.87 && strValue>2.67)
			{
				itemResult.pass=1;
			}
		}
		saveResult(hashTable,&itemResult);			
	}		
		
		
	

	ListDispose(paramsToSet);
	ListDispose(paramsToGet);
	CloseDo(eut.relayConfig,31); 
DONE:	
	return ret;
}

TPS registerChargingTestTestTps(void)
{
	TPS tps=newTps("charging");
	tps.autoTestFunction=ChargingTest;
	return tps;	
}

