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
#include "ParamSetGet.h"

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
	{
		return 	TEST_RESULT_ERROR;
	}
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
	{
		return 	TEST_RESULT_ERROR;
	}
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
	if(startCommand(servicePtr,"product self_test\r\n")<0)
		goto DONE;
	

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;		
#if 0		
		PARAMETER paramGet={0};
		ListGetItem(paramsToFetch,&paramGet,i); 
		///printfParam(paramGet);
		
		if(GetParameter(servicePtr,&paramGet)<0)
		{	
			WarnShow1(0,"获取失败");
			goto DONE;
		}

		if(strcmp(group.groupName,"硬件自检")==0)
		{
			if(strcmp("true",paramGet.value)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}
		}else if(strcmp(group.groupName,"电表时间")==0)
		{
			itemResult.pass=1;
		}else{
			itemResult.pass=1;	
		}
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		//printf("----------%s\n",paramGet.value);
		sprintf(itemResult.recvString,"%s",paramGet.value);
#else	
		itemResult.pass=1;
		ParamGet(servicePtr,item.itemName_,itemResult.recvString);
		//ParamGetDepend(eut,item.itemName_,itemResult.recvString);
#endif		
		saveResult(hashTable,&itemResult);
	}	
DONE:	
	ListDispose(paramsToFetch);
	onStubDisConnected(servicePtr);	
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
	{
		return 	TEST_RESULT_ERROR;
	}	
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
	onStubDisConnected(servicePtr);	
	return ret;
}

TPS registerParamTemperatureTps(void)
{
	TPS tps=newTps("temperature");
	tps.autoTestFunction=ParamTemperatureTest;
	tps.protocolInit=ParamProtocolInit;
	return tps;			
}

#define BIBO_DEBUG
METHODRET ParamBiboTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	if(strcmp(group.groupName,"枪1功率分配")==0)
	{
		sprintf(startChargeCmd,"%s","1枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","1枪调试停止充电");
	}else if(strcmp(group.groupName,"枪2功率分配")==0){
		sprintf(startChargeCmd,"%s","2枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","2枪调试停止充电");	
	}
	
		
#ifndef BIBO_DEBUG	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"无法启动充电！");
		return ret;
	}
#endif	
	//60A需求
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item1={0};
		ListGetItem(group.subItems,&item1,i);
		RESULT itemResult1={0}; 
		itemResult1.index = item1.itemId;
		itemResult1.pass=0;		
		if(i==1)
		{
			WarnShow1(0,"设置电流为60A"); //后续可以用485控制来操作
			if(FALSE==AlertDialogWithRet(0,"waring","电源模块1，2启动","错误","正确"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}
		}else if(i==2){
		
			WarnShow1(0,"设置电流为100A"); //后续可以用485控制来操作
			if(FALSE==AlertDialogWithRet(0,"waring","电源模块1，2启动","错误","正确"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}			
		}else if(i==3){
		
			WarnShow1(0,"设置电流为120A"); //后续可以用485控制来操作
			if(FALSE==AlertDialogWithRet(0,"waring","电源模块1，2启动","错误","正确"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}			
		}
	
		//char BI[10]={0};

#ifndef BIBO_DEBUG	
		if(FALSE==ParamGetDepend(eut,"BI",itemResult1.recvString ))
		{
			
		}
#endif
		//PRINT("INPUTVALUE:%x\n",HexStrToUnsignedInt(item1.inputValue_));
		unsigned int standard= HexStrToUnsignedInt(item1.inputValue_);
		unsigned int bi = HexStrToUnsignedInt(itemResult1.recvString);
		unsigned int result = bi & standard;

		if(result == standard)
		{
			itemResult1.pass=1; 		
		}else{
			itemResult1.pass=0;
		}
ERROR:		
		//sprintf(itemResult1.recvString,"%s",BI);
		saveResult(hashTable,&itemResult1);
	}		
	
	//100A需求

#ifndef BIBO_DEBUG	
	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"无法启动充电！");
	}
#endif	
	
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

BOOL checkScanResult(int panel)
{
	char gun1[20]={0},gun2[20]={0},biao1[20]={0},biao2[20]={0};
	GetCtrlVal(panel,SCANPANEL_SCAN1,gun1);
	GetCtrlVal(panel,SCANPANEL_SCAN2,gun2);
	GetCtrlVal(panel,SCANPANEL_SCAN3,biao1); 
	GetCtrlVal(panel,SCANPANEL_SCAN4,biao2);
	if(strcmp(gun1,gun2)==0)
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"错误:枪1枪2地址一致");
		return FALSE;
	}
	if(strcmp(biao1,biao2)==0)
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"错误:表1表2地址一致");
		return FALSE;
	}	
	return TRUE;
}

int CVICALLBACK ParamScanCtrlCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(control == SCANPANEL_COMMANDBUTTON)
			 {
				 if(checkScanResult(panel)==TRUE)
			 	   QuitUserInterface(1);
			 }
			 break;
			
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
					//QuitUserInterface(1);
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
			if(checkScanResult(panelHandle)==TRUE) 
			 	QuitUserInterface(1);
		     break;
	}
	return 0;
}

#define DEBUG_PARAM_SCAN_TEST
METHODRET ParaScanTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	char stubName[20]={0};
#ifndef DEBUG_PARAM_SCAN_TEST	
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}
#endif	
	
	int panelHandle=LoadPanel(0,"ParamPanel.uir",SCANPANEL);
	
	InstallPanelCallback(panelHandle,ParaPanelCallback,NULL);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN1,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN2,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN3,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN4,ParamScanCtrlCallback,stubName); 
	InstallCtrlCallback(panelHandle,SCANPANEL_COMMANDBUTTON,ParamScanCtrlCallback,NULL);
	SetActiveCtrl(panelHandle,SCANPANEL_SCAN1);
	DisplayPanel(panelHandle); 
	RunUserInterface();
	HidePanel(panelHandle);
#ifdef DEBUG_PARAM_SCAN_TEST	
	goto DONE;
#else	
	int stubNameLen=strlen(stubName);
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		PARAMETER param={0};
		ListGetItem(group.subItems,&item,i);
		
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		itemResult.pass=0;
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
			sprintf(itemResult.recvString,"00000%s",stubName);
			sprintf(param.value,"%s",itemResult.recvString);
			if(ConfigParameter(servicePtr,param)<0)
				continue;
			if(GetParameter(servicePtr,&param)<0){
				continue;
			}
			
			if(strcmp(itemResult.recvString,param.value)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}
			sprintf(itemResult.recvString,"%s",param.value);			
			
			
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
			if(GetParameter(servicePtr,&param)<0){
				continue;
			}
			
			if(strcmp(itemResult.recvString,param.value)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}
			sprintf(itemResult.recvString,"%s",param.value);
			
						
	
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
			if(GetParameter(servicePtr,&param)<0){
				continue;
			}
			
			if(strcmp(itemResult.recvString,param.value)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}
			sprintf(itemResult.recvString,"%s",param.value);			
		}
		saveResult(hashTable,&itemResult);
	}
#endif	
DONE:	
	
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
	int flag1=-1;
	int flag2=-1;
	int flag3=-1;
	
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return TEST_RESULT_ERROR;
	}
	
	TestItem item={0};
	ListGetItem(group.subItems,&item,1); 
	char queryResult[20]={0};
	if(ParamGet(servicePtr,item.itemName_,queryResult)<0)
	{
		goto DONE;
	}
	
	if(strcmp(queryResult,"false")==0)
	{
		flag1=1;
	}else{
		flag1=0;
	}
	
	if(OperatDoSet(eut.relayConfig,RELAY(31)|RELAY(3)|RELAY(4)|RELAY(2))==FALSE)
	{
		goto DONE;
	}
	
	//WarnAlert(0,);
	WarnAlert(0,"延时中",30);
	
	memset(queryResult,0,20);
	if(ParamGet(servicePtr,item.itemName_,queryResult)<0)
	{
		goto DONE;
	}

	if(strcmp(queryResult,"true")==0)
	{
		flag2=1;
	}else{
		flag2=0;
	}	
	
	if(AlertDialogWithRet(0,"枪检查","请检查面板显示应充电枪故障指示灯显示应该为X","错误","正确")==FALSE)
	{
		flag2=0;
	}
	

	if(OperatDoSet(eut.relayConfig,RELAY(2))==FALSE)
	{
		goto DONE;
	}
	
	WarnAlert(0,"延时中",30); 
	
	memset(queryResult,0,20);
	if(ParamGet(servicePtr,item.itemName_,queryResult)<0)
	{
		goto DONE;
	}
	
	if(strcmp(queryResult,"false")==0)
	{
		flag3=1;
	}else{
		flag3=0;
	}	
	
	if(AlertDialogWithRet(0,"枪检查","请检查面板显示应充电枪故障指示灯显示应该消失","错误","正确")==FALSE)
	{
		flag3=0;
	}
	
DONE:	
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	itemResult.pass = flag1 & flag2 & flag3;
	sprintf(itemResult.recvString,"{%d,%d,%d}",flag1,flag2,flag3);
	saveResult(hashTable,&itemResult);
	
	if(OperatDoSet(eut.relayConfig,RELAY(2))==FALSE)
	{
		
	}		
	onStubDisConnected(servicePtr);
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
#if 1
	BOOL flag=TRUE; 
	METHODRET ret = TEST_RESULT_ALLPASS;
	TestItem item;
	ListGetItem(group.subItems,&item,1);
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	char isolateCMD[20]={0};
	if(strcmp(item.itemName_,"枪1绝缘检测")==0)
	{
		sprintf(startChargeCmd,"%s","1枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","1枪调试停止充电");
		sprintf(isolateCMD,"%s","1枪绝缘检测结果");
	}else if(strcmp(item.itemName_,"枪2绝缘检测")==0){
		sprintf(startChargeCmd,"%s","2枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","2枪调试停止充电");
		sprintf(isolateCMD,"%s","2枪绝缘检测结果");
	}

	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(30)|RELAY(2)))
	{
		goto DONE;
	}

	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"无法启动充电！");
		flag=FALSE; 
		goto DONE;
	}
	
	if(AlertDialogWithRet(0,"枪检查","请确认充电功能正常","错误","正确")==FALSE)
	{
		flag=FALSE;
		goto DONE;
	}
	
	//读取结果
	char inSoResult[10]={0};
	if(FALSE==ParamGetDepend(eut,isolateCMD,inSoResult))
	{
		WarnShow1(0,"获取绝缘检测结果失败"); 
	}
	
	printf("%s\n",inSoResult);
	

	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"无法停止充电！请按急停停止"); 
		goto DONE;
	}	
	
	//WarnAlert(0,"延时中",30);
	WarnShow1(0,"下一步测试");
	
	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(29) | RELAY(30)| RELAY(2)))
	{
		goto DONE;
	}
	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"无法启动充电！");
		flag=FALSE; 
		goto DONE;
	}	

	if(AlertDialogWithRet(0,"枪检查","请确认充电功能异常","错误","正确")==FALSE)
	{
		flag=FALSE;
	}
	
	char inSoResult1[10]={0};
	if(FALSE==ParamGetDepend(eut,isolateCMD,inSoResult1))
	{
		WarnShow1(0,"获取绝缘检测结果失败"); 
		goto DONE;
	}
	
	printf("%s\n",inSoResult1);	
	
	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"无法停止充电！请按急停停止"); 
		goto DONE;
	}	
DONE:	
	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(2)))
	{
	}
	
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	itemResult.pass=flag;
	saveResult(hashTable,&itemResult);
	return ret;	
#else	
	METHODRET ret = TEST_RESULT_ALLPASS;
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}	
	TestItem item;
	ListGetItem(group.subItems,&item,1);
	//step1  合上继电器30，断开29，保持电源打开 
	if(OperatDoSet(eut.relayConfig,0x20000002)==FALSE)
	{
		goto DONE;
	}
	//step2 等待10S
	WarnAlert(0,"延时中",10);

	//step3 启动充电流程 
	if(ParamSet(servicePtr,"1枪调试启动充电","1")==FALSE)
	{
		goto DONE;
	}
	//step4 等待确认
	onStubDisConnected(servicePtr); 
	WarnShow1(0,"请确认开始下一步");
	servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		goto DONE;
	}		
	//step5 读取绝缘检测结果
	char resistentStr[20]={0};
	ParamGet(servicePtr,"1枪绝缘电阻正",resistentStr);
	int resistent1=atoi(resistentStr);
	
	memset(resistentStr,0,20);
	ParamGet(servicePtr,"1枪绝缘电阻负",resistentStr);
	int resistent2=atoi(resistentStr);
	int resistentJudge=0;
	if(resistent1<resistent2)
	{
		//TODO:计算结果
	}else{
		//TODO:
	}
	PRINT("--------------------%d,%d\n",resistent1,resistent2);

	//step6 关闭充电 
	if(ParamSet(servicePtr,"1枪调试停止充电","1")==FALSE)
	{
		goto DONE; 
	}
	
	//step1  合上继电器30，合上29,保持电源打开
	if(OperatDoSet(eut.relayConfig,0x30000002)==FALSE)
	{
		goto DONE; 
	}
/*	
	//step2 启动充电流程 
	if(ParamSet(servicePtr,"1枪调试启动充电","1")==FALSE)
	{
		goto DONE;
	}
	//step2 人工判断能否正常充电 
	if(AlertDialogWithRet(0,"枪检查","请确认不能正常启动充电","错误","正确")==FALSE)
	{
		//flag=0;
	}
	
	if(ParamSet(servicePtr,"1枪调试停止充电","1")==FALSE)
	{
		goto DONE;
	}	
	
*/	
DONE:
	onStubDisConnected(servicePtr); 
	if(ParamSet(servicePtr,"1枪调试停止充电","1")==FALSE)
	{
		//goto DONE; 
	}

	if(OperatDoSet(eut.relayConfig,0x00000002)==FALSE)
	{
		//return TEST_RESULT_ERROR; 
	}
	WarnAlert(0,"延时中",10);
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	//itemResult.pass=flag;
	saveResult(hashTable,&itemResult);
	return ret;
#endif	
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
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}*/
	//充电模块确认
	TestItem item1;
	ListGetItem(group.subItems,&item1,1);
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	if(strcmp(group.groupName,"枪1充电功能")==0)
	{
		sprintf(startChargeCmd,"%s","1枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","1枪调试停止充电");
	}else if(strcmp(group.groupName,"枪2充电功能")==0){
		sprintf(startChargeCmd,"%s","2枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","2枪调试停止充电");	
	}	
	
	RESULT itemResult1={0};
	itemResult1.index=item1.itemId;
	itemResult1.pass=1;
	
	/*if(ParamSet(servicePtr,"1枪调试启动充电","1")==FALSE)
	{
		goto DONE;
	}
	onStubDisConnected(servicePtr);*/
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"无法启动充电");
		return TEST_RESULT_ALLPASS;
	}
	
	if(AlertDialogWithRet(0,"枪检查","请确认充电流程已启动,点击确认加负载","错误","正确")==FALSE)
	{
		flag=FALSE;
		itemResult1.pass=0;
		saveResult(hashTable,&itemResult1);
		//return TEST_RESULT_ALLPASS;
		ParamSetDepend(eut,stopChargeCMD,"1");	
		return TEST_RESULT_ALLPASS;
	}
	saveResult(hashTable,&itemResult1);
	
	//加负载
	if(OpenDo(eut.relayConfig,31)==FALSE)
	{
		return 	TEST_RESULT_ERROR;
	}	
	
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}

	WarnShow1(0,"按下确定开始读数");

	ListType paramsToSet=ListCreate(sizeof(PARAMETER));
	for(int i=2;i<=4;i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);

		PARAMETER param={0};
		ListInsertItem(paramsToSet,&param,END_OF_LIST); 
		/*
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"无该参数配置");
			goto DONE;
		}
		
		if(GetParameter(servicePtr,&param)<0)
		{	
			WarnShow1(0,"获取失败");
			goto DONE;
		}			
		//printf("----------------------------%s\n",param.value);
		ListInsertItem(paramsToGet,&param,END_OF_LIST);*/
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		//sprintf(itemResult.recvString,"%s",param.value);
		if(ParamGet(servicePtr,item.itemName_,itemResult.recvString)==FALSE)
		{
			 itemResult.pass=0;
			 saveResult(hashTable,&itemResult);
			 continue;
		}
		float strValue=atof(param.value);
		if(i==3)
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
		}else{
			itemResult.pass=1; 
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
		itemResult.pass=1;
		if(i==2)
		{
			if(strValue<352 && strValue>348)
			{
				itemResult.pass=1;
			}
		}else if(i==1)
		{
			if(strValue<2.87 && strValue>2.67)
			{
				itemResult.pass=1;
			}
		}
		saveResult(hashTable,&itemResult);			
	}
	
	
	
DONE:
	ListDispose(paramsToSet);
	//ListDispose(paramsToGet);
	
	if(ParamSet(servicePtr,stopChargeCMD,"1")==FALSE)
	{
		
	}
	
	if(CloseDo(eut.relayConfig,31)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	onStubDisConnected(servicePtr);	
	return ret;
}

TPS registerChargingTestTestTps(void)
{
	TPS tps=newTps("charging");
	tps.autoTestFunction=ChargingTest;
	return tps;	
}

static void getSysTime(char *timeBuffer)
{
	unsigned int year, month, day, hour, min, sec, weekDay;
	CVIAbsoluteTime absTime;
	GetCurrentCVIAbsoluteTime(&absTime);
    CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
                &min, &sec, 0, &weekDay);
	//sprintf(timeBuffer,"%02d-%02d-%02d %02d:%02d:%02d",year-2000,month,day,hour,min,sec);
	sprintf(timeBuffer,"%02d-%02d-%02d %02d:%02d:%02d:0000",year-2000,month,day,hour,min,sec);

}

METHODRET TimeSetTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	PARAMETER param={0};
	RESULT itemResult={0};
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}	
	TestItem item;
	ListGetItem(group.subItems,&item,1);
	if(FALSE==getParameter(item.itemName_,&param))
	{
		WarnShow1(0,"无该参数配置");
	}
	char tempStr[50]={0};
	//printf("%s\n",item.inputValue_);
	if(strcmp("NA",item.inputValue_)!=0){
		sprintf(tempStr,"%s",item.inputValue_);			
	}else{
		getSysTime(tempStr);
	}
	sprintf(param.value,"%s",tempStr);
	itemResult.index=item.itemId;
	if(ConfigParameter(servicePtr,param)>=0)
	{
		 itemResult.pass =1;
	}else{
		itemResult.pass=0;
	}
	sprintf(itemResult.recvString,"%s",param.value);
	saveResult(hashTable,&itemResult);
	onStubDisConnected(servicePtr);
	if(CloseDo(eut.relayConfig,1)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	WarnAlert(0,"重启中",3);

	
	if(OpenDo(eut.relayConfig,1)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	
	
	WarnAlert(0,"请确认设备已经器动",30);
	
	//printf("-------------------------set:%s\n",param.value);
	TestItem item2;
	RESULT itemResult2={0};
	ListGetItem(group.subItems,&item2,2);	
	memset(param.value,0,128);
	servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}	
	GetParameter(servicePtr,&param);
	itemResult2.index = item2.itemId;
	itemResult2.pass = 1;
	sprintf(itemResult2.recvString,"%s",param.value);
	saveResult(hashTable,&itemResult2); 	
	return ret;
}

TPS registerTimeSetTestTps(void)
{
	TPS tps=newTps("timeSet");
	tps.autoTestFunction=TimeSetTest;
	return tps;	
}

METHODRET StubCmdTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;
		itemResult.pass=1;
		if(strcmp(item.itemName_,"参数校正")==0){
			if(startCommand(servicePtr,"prd coeff ins\r\n")<0)
			{
				WarnShow1(0,"参数校正失败");
				itemResult.pass=0;
			}
		}else if(strcmp(item.itemName_,"看门狗检查")==0){ 
			if(startCommand(servicePtr,"product watch_dog\r\n")<0)
			{
				WarnShow1(0,"看门狗检查");
				itemResult.pass=0;
			}
			onStubDisConnected(servicePtr);
			servicePtr=NULL;
			WarnAlert(0,"请稍后！！！",30);
			servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
			if(servicePtr==NULL)
			{
				itemResult.pass=0;
				return 	TEST_RESULT_ERROR;
			}			
		}else if(strcmp(item.itemName_,"交流复位")==0){
			PARAMETER param={0};
			if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"无该参数配置");
			}
			sprintf(param.value,"%s","1");
			if(ConfigParameter(servicePtr,param)>=0)
			{
		 		itemResult.pass =1;
			}else{
				itemResult.pass=0;
			}
			onStubDisConnected(servicePtr);
			servicePtr=NULL;
			WarnAlert(0,"请稍后！！！",30);
			servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
			if(servicePtr==NULL)
			{
				itemResult.pass=0;
				return 	TEST_RESULT_ERROR;
			}			
		}
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s","OK");
		saveResult(hashTable,&itemResult);
		
	}
	return ret;
}

TPS registerStubCmdTestTps(void)
{
	TPS tps=newTps("StubCmd");
	tps.autoTestFunction=StubCmdTest;
	return tps;	
}

