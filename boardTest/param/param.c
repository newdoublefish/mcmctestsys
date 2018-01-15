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
			WarnShow1(0,"�޸ò�������");
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
			WarnShow1(0,"���ô���!");
		}
	}
																		 
	//showParamSetPanel(paramsToFetch); 
	
	
	//���ò���
	//TODO:���ز������Ա�
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
			WarnShow1(0,"�޸ò�������");
			goto DONE;
		}
		ListInsertItem(paramsToFetch,&param,END_OF_LIST);
	}
	//TODO:���ز���
	//showParamSetPanel(paramsToFetch);
	//���ò���
	//�����Լ�����
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
			WarnShow1(0,"��ȡʧ��");
			goto DONE;
		}
		RESULT itemResult;
		itemResult.index=item.itemId;
		if(strcmp(group.groupName,"Ӳ���Լ�")==0)
		{
			if(strcmp("true",paramGet.value)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}
		}else if(strcmp(group.groupName,"���ʱ��")==0)
		{
			itemResult.pass=1;
		}else{
			itemResult.pass=1;	
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
			WarnShow1(0,"�޸ò�������");
			goto DONE;
		}
		ListInsertItem(paramsToFetch,&param,END_OF_LIST);
	}
	//TODO:���ز���
	//showParamSetPanel(paramsToFetch);
	//���ò���

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER paramGet={0};
		ListGetItem(paramsToFetch,&paramGet,i); 
		///printfParam(paramGet);
		
		if(GetParameter(servicePtr,&paramGet)<0)
		{	
			WarnShow1(0,"��ȡʧ��");
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
	{
		return 	TEST_RESULT_ERROR;
	};	

	return ret;
}


TPS registerBiboTestTps(void)
{
	TPS tps=newTps("BIBO");
	tps.autoTestFunction=ParamBiboTest;
	tps.protocolInit=BiboProtocolInit;
	return tps;			
}

//ǹ��ά�� "http://www.gdmcmc.cn/qrcode.html?qrcode=881011000851";
//����ַ strAddress="1711424989000065";

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
	{
		return 	TEST_RESULT_ERROR;
	}
	
	int panelHandle=LoadPanel(0,"ParamPanel.uir",SCANPANEL);
	
	InstallPanelCallback(panelHandle,ParaPanelCallback,NULL);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN1,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN2,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN3,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN4,ParamScanCtrlCallback,stubName); 	
	SetActiveCtrl(panelHandle,SCANPANEL_SCAN1);
	DisplayPanel(panelHandle); 
	RunUserInterface();
	HidePanel(panelHandle);
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
				WarnShow1(0,"�޸ò�������");
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
				WarnShow1(0,"�޸ò�������");
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
				WarnShow1(0,"�޸ò�������");
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
	{
		return 	TEST_RESULT_ERROR;
	}
	
	TestItem item;
	PARAMETER param={0};
	ListGetItem(group.subItems,&item,1);
	if(FALSE==getParameter(item.itemName_,&param))
	{
		WarnShow1(0,"�޸ò�������");
		return TEST_RESULT_ERROR;
	}
	if(GetParameter(servicePtr,&param)<0)
	{	
		WarnShow1(0,"��ȡʧ��");
		
	}
	//printf("-----------------%s\n",param.value);
	
	if(strcmp(param.value,"false")==0)
	{
		flag1=1;
	}
	
	if(OpenDo(eut.relayConfig,30)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	if(OpenDo(eut.relayConfig,2)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	if(OpenDo(eut.relayConfig,3)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}	
	
	Delay(2);
	
	if(GetParameter(servicePtr,&param)<0)
	{	
		WarnShow1(0,"��ȡʧ��");
		//goto DONE;
	}
	//printf("-----------------%s\n",param.value); 
	
	if(strcmp(param.value,"true")==0)
	{
		flag2=1;
	}	
	
	//showTips(0,"�����Ŀ","���������ʾӦ���ǹ����ָʾ����ʾӦ��ΪX"); 

	if(AlertDialogWithRet(0,"ǹ���","���������ʾӦ���ǹ����ָʾ����ʾӦ��ΪX","����","��ȷ")==FALSE)
	{
		flag2=0;
	}else{
		//goto DONE;
	}
	

	if(CloseDo(eut.relayConfig,30)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	if(CloseDo(eut.relayConfig,2)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	if(CloseDo(eut.relayConfig,3)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}	
	Delay(2); 
	
	if(GetParameter(servicePtr,&param)<0)
	{	
		WarnShow1(0,"��ȡʧ��");
		//goto DONE;
	}
	//printf("-----------------%s\n",param.value); 
	
	if(strcmp(param.value,"false")==0)
	{
		flag3=1;
	}	
	
	if(AlertDialogWithRet(0,"ǹ���","���������ʾӦ���ǹ����ָʾ����ʾӦ����ʧ","����","��ȷ")==FALSE)
	{
		flag3=0;
	}
	
	//���Ͽ���31
	
	//printf("-----------------%s\n",param.value);
	//("%d,%d,%d\n",flag1,flag2,flag3);
DONE:	
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	itemResult.pass = flag1 & flag2 & flag3;
	if(itemResult.pass)
		sprintf(itemResult.recvString,"%s","�ϸ�");
	else
		sprintf(itemResult.recvString,"%s","���ϸ�");
	saveResult(hashTable,&itemResult);
	
	if(CloseDo(eut.relayConfig,30)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	if(CloseDo(eut.relayConfig,2)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	if(CloseDo(eut.relayConfig,3)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}		

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
	if(OpenDo(eut.relayConfig,29)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	if(CloseDo(eut.relayConfig,28)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	BOOL flag=TRUE;
	if(AlertDialogWithRet(0,"ǹ���","��ȷ�Ͽ����������","����","��ȷ")==FALSE)
	{
		flag=FALSE;
		goto DONE;
	}
	
	//OpenDo(eut.relayConfig,29);
	
	WarnShow1(0,"��رճ�磡��");
	if(OpenDo(eut.relayConfig,28)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	if(AlertDialogWithRet(0,"ǹ���","��ȷ�ϲ����������","����","��ȷ")==FALSE)
	{
		flag=FALSE;
	}
DONE:	
	if(CloseDo(eut.relayConfig,29)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	if(CloseDo(eut.relayConfig,28)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
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
	{
		return 	TEST_RESULT_ERROR;
	}
	//���ģ��ȷ��
	TestItem item1;
	ListGetItem(group.subItems,&item1,1);
	RESULT itemResult1={0};
	itemResult1.index=item1.itemId;
	itemResult1.pass=1; 
	if(AlertDialogWithRet(0,"ǹ���","��ȷ�ϳ������������","����","��ȷ")==FALSE)
	{
		flag=FALSE;
		itemResult1.pass=0;
		saveResult(hashTable,&itemResult1);
		return TEST_RESULT_ALLPASS;
	}
	
	saveResult(hashTable,&itemResult1);	
	
	if(OpenDo(eut.relayConfig,31)==FALSE)
	{
		return 	TEST_RESULT_ERROR;
	}

	WarnShow1(0,"����ȷ����ʼ����");
	
	ListType paramsToGet=ListCreate(sizeof(PARAMETER));
	ListType paramsToSet=ListCreate(sizeof(PARAMETER));
	for(int i=2;i<=4;i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);

		PARAMETER param={0};
		
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"�޸ò�������");
			goto DONE;
		}
		ListInsertItem(paramsToSet,&param,END_OF_LIST);
		if(GetParameter(servicePtr,&param)<0)
		{	
			WarnShow1(0,"��ȡʧ��");
			goto DONE;
		}			
		//printf("----------------------------%s\n",param.value);
		ListInsertItem(paramsToGet,&param,END_OF_LIST);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		sprintf(itemResult.recvString,"%s",param.value);
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
	//ListDispose(paramsToSet);
	//ListDispose(paramsToGet);
	if(CloseDo(eut.relayConfig,31)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}	
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
		WarnShow1(0,"�޸ò�������");
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
	
	WarnAlert(0,"������",3);

	
	if(OpenDo(eut.relayConfig,1)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	
	
	WarnAlert(0,"��ȷ���豸�Ѿ�����",30);
	
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
		if(strcmp(item.itemName_,"����У��")==0){
			if(startCommand(servicePtr,"prd coeff ins\r\n")<0)
			{
				WarnShow1(0,"����У��ʧ��");
				itemResult.pass=0;
			}
		}else if(strcmp(item.itemName_,"���Ź����")==0){ 
			if(startCommand(servicePtr,"product watch_dog\r\n")<0)
			{
				WarnShow1(0,"���Ź����");
				itemResult.pass=0;
			}
			onStubDisConnected(servicePtr);
			servicePtr=NULL;
			WarnAlert(0,"���Ժ󣡣���",30);
			servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
			if(servicePtr==NULL)
			{
				itemResult.pass=0;
				return 	TEST_RESULT_ERROR;
			}			
		}else if(strcmp(item.itemName_,"������λ")==0){
			PARAMETER param={0};
			if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"�޸ò�������");
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
			WarnAlert(0,"���Ժ󣡣���",30);
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

