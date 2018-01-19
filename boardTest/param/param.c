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
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}
	//�����Լ�����
	if(startCommand(servicePtr,"product self_test\r\n")<0)
		goto DONE;
	

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;		
		itemResult.pass=1;
		if(ParamGet(servicePtr,item.itemName_,itemResult.recvString)<0)
		{
			goto DONE;
		}
		/*if(strcmp(group.groupName,"Ӳ���Լ�")==0)
		{
			if(strcmp("true",itemResult.recvString)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}		
		}*/
		itemResult.pass=1;
		
		if(strcmp(item.standard_,"NA")!=0)
		{
			/*if(strcmp(item.standard_,itemResult.recvString)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}*/
			if(strstr(item.standard_,itemResult.recvString)!=NULL)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}
		}
		
		if(atof(item.standard_)>0.0001)
		{
			if(atof(itemResult.recvString)>(atof(item.standard_)-0.5))
			{
				 itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}
		}
		saveResult(hashTable,&itemResult);
	}
	return ret;
DONE:	
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

//#define BIBO_DEBUG
METHODRET ParamBiboTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	char tips60A[20]={0};
	char tips100A[20]={0};
	char tips120A[30]={0};
	if(strcmp(group.groupName,"ǹ1���ʷ���")==0)
	{
		sprintf(startChargeCmd,"%s","1ǹ�����������");		
		sprintf(stopChargeCMD,"%s","1ǹ����ֹͣ���");
		sprintf(tips60A,"%s","��Դģ��1��2����");
		sprintf(tips100A,"%s","��Դģ��1��2��3����");
		sprintf(tips120A,"%s","��Դģ��1��2��3��6 ����");
	}else if(strcmp(group.groupName,"ǹ2���ʷ���")==0){
		sprintf(startChargeCmd,"%s","2ǹ�����������");		
		sprintf(stopChargeCMD,"%s","2ǹ����ֹͣ���");
		sprintf(tips60A,"%s","��Դģ��4��5����");
		sprintf(tips100A,"%s","��Դģ��4��5��6����");
		sprintf(tips120A,"%s","��Դģ��3��4��5��6 ����");	
	}
	
		
#ifndef BIBO_DEBUG	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"�޷�������磡");
		return ret;
	}
#endif	
	WarnShow1(0,"��ȷ����������Ѿ���ʼ����BMS��ѹ�ȶ�");
	//60A����
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item1={0};
		ListGetItem(group.subItems,&item1,i);
		RESULT itemResult1={0}; 
		itemResult1.index = item1.itemId;
		itemResult1.pass=0;		
		if(i==1)
		{
			WarnShow1(0,"���õ���Ϊ60A"); //����������485����������
			if(FALSE==AlertDialogWithRet(0,"waring",tips60A,"����","��ȷ"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}
		}else if(i==2){
		
			WarnShow1(0,"���õ���Ϊ100A"); //����������485����������
			if(FALSE==AlertDialogWithRet(0,"waring",tips100A,"����","��ȷ"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}			
		}else if(i==3){
		
			WarnShow1(0,"���õ���Ϊ120A"); //����������485����������
			if(FALSE==AlertDialogWithRet(0,"waring",tips120A,"����","��ȷ"))
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
		//PRINT("%x\n",itemResult1.recvString);
		unsigned int standard= HexStrToUnsignedInt(item1.inputValue_);
		unsigned int bi = atoi(itemResult1.recvString);
		unsigned int result = bi & standard;
		memset(itemResult1.recvString,0,RESULT_RECEIVE_LEN);
		sprintf(itemResult1.recvString,"0x%x",bi);

		if(result == standard)
		{
			itemResult1.pass=1; 		
		}else{
			itemResult1.pass=0;
		}
		
		//sprintf(itemResult1.recvString,"%s",BI);
		saveResult(hashTable,&itemResult1);
	}		
ERROR:	
	//100A����

#ifndef BIBO_DEBUG	
	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"�޷�ֹͣ���,�밴��ͣ��ť");
	}
	WarnShow1(0,"��ȴ���ȷ����������Ѿ�ֹͣ");
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

//ǹ��ά�� "http://www.gdmcmc.cn/qrcode.html?qrcode=881011000851";
//����ַ strAddress="1711424989000065";

BOOL checkScanResult(int panel)
{
	char gun1[128]={0},gun2[128]={0},biao1[128]={0},biao2[128]={0};
	GetCtrlVal(panel,SCANPANEL_SCAN1,gun1);
	GetCtrlVal(panel,SCANPANEL_SCAN2,gun2);
	GetCtrlVal(panel,SCANPANEL_SCAN3,biao1); 
	GetCtrlVal(panel,SCANPANEL_SCAN4,biao2);
	if(strcmp(gun1,gun2)==0)
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"����:ǹ1ǹ2��ַһ��");
		return FALSE;
	}
	if(strcmp(biao1,biao2)==0)
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"����:��1��2��ַһ��");
		return FALSE;
	}
	
	if((strlen(gun1)!=strlen(gun2)) | (strlen(gun1)!=12))
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"����:ǹ1ǹ2��ַ���ô���");
		return FALSE;		
	}
	
	if((strlen(biao1)!=strlen(biao2)) | (strlen(biao2)!=12)) 
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"����:��1��2��ַ���ô���");
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
			//if(checkScanResult(panelHandle)==TRUE) 
			 BOOL *operateFlag = (BOOL *)callbackData;
			 *operateFlag = FALSE;
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}

//#define DEBUG_PARAM_SCAN_TEST
METHODRET ParaScanTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	BOOL operateFlag = TRUE;
	char stubName[20]={0};
#ifndef DEBUG_PARAM_SCAN_TEST	
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return ret;
	}
#endif	
	
	int panelHandle=LoadPanel(0,"ParamPanel.uir",SCANPANEL);
	
	InstallPanelCallback(panelHandle,ParaPanelCallback,&operateFlag);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN1,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN2,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN3,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN4,ParamScanCtrlCallback,stubName); 
	InstallCtrlCallback(panelHandle,SCANPANEL_COMMANDBUTTON,ParamScanCtrlCallback,NULL);
	SetActiveCtrl(panelHandle,SCANPANEL_SCAN1);
	DisplayPanel(panelHandle); 
	RunUserInterface();
	HidePanel(panelHandle);
	if(FALSE == operateFlag)
	{
		DiscardPanel(panelHandle);
		return ret;
	}
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
		if(1==i)
		{
			GetCtrlVal(panelHandle,SCANPANEL_SCAN1,itemResult.recvString); 
		}else if(2==i){
			GetCtrlVal(panelHandle,SCANPANEL_SCAN2,itemResult.recvString);
		}else if(3==i)
		{
			/*if(FALSE==getParameter(item.itemName_,&param))
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
			sprintf(itemResult.recvString,"%s",param.value);*/
			char setValue[20]={0};
			sprintf(setValue,"00000%s",stubName);
			if(ParamSet(servicePtr,item.itemName_,setValue)<0)
			{
				goto DONE;
			}
			if(ParamGet(servicePtr,item.itemName_,itemResult.recvString)<0)
			{
				goto DONE;
			}
			if(strcmp(itemResult.recvString,setValue)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}			
			
			
		}else if(i==4)
		{
			/*if(FALSE==getParameter(item.itemName_,&param))
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
			sprintf(itemResult.recvString,"%s",param.value);*/
			
			char setValue[20]={0};
			GetCtrlVal(panelHandle,SCANPANEL_SCAN3,setValue);
			if(ParamSet(servicePtr,item.itemName_,setValue)<0)
			{
				goto DONE;
			}
			if(ParamGet(servicePtr,item.itemName_,itemResult.recvString)<0)
			{
				goto DONE;
			}
			if(strcmp(itemResult.recvString,setValue)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}			
			
						
	
		}else if(i==5)
		{
			char setValue[20]={0};
			GetCtrlVal(panelHandle,SCANPANEL_SCAN4,setValue);
			if(ParamSet(servicePtr,item.itemName_,setValue)<0)
			{
				goto DONE;
			}
			if(ParamGet(servicePtr,item.itemName_,itemResult.recvString)<0)
			{
				goto DONE;
			}
			if(strcmp(itemResult.recvString,setValue)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}			
		}
		saveResult(hashTable,&itemResult);
	}
	DiscardPanel(panelHandle); 
	return ret;
#endif	
DONE:	
	DiscardPanel(panelHandle);
#ifndef DEBUG_PARAM_SCAN_TEST	
	onStubDisConnected(servicePtr);
#endif	
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
	WarnAlert(0,"��ʱ��",30);
	
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
	
	if(AlertDialogWithRet(0,"ǹ���","���������ʾӦ���ǹ����ָʾ����ʾӦ��ΪX","����","��ȷ")==FALSE)
	{
		flag2=0;
	}
	

	if(OperatDoSet(eut.relayConfig,RELAY(2))==FALSE)
	{
		goto DONE;
	}
	
	WarnAlert(0,"��ʱ��",30); 
	
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
	
	if(AlertDialogWithRet(0,"ǹ���","���������ʾӦ���ǹ����ָʾ����ʾӦ����ʧ","����","��ȷ")==FALSE)
	{
		flag3=0;
	}
	
	
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	itemResult.pass = flag1 & flag2 & flag3;
	sprintf(itemResult.recvString,"{%d,%d,%d}",flag1,flag2,flag3);
	saveResult(hashTable,&itemResult);
	
	if(OperatDoSet(eut.relayConfig,RELAY(2))==FALSE)
	{
		
	}
	return ret;
DONE:
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
	BOOL flag=TRUE; 
	METHODRET ret = TEST_RESULT_ALLPASS;
	TestItem item;
	ListGetItem(group.subItems,&item,1);
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	char isolateCMD[20]={0};
	if(strcmp(item.itemName_,"ǹ1��Ե���")==0)
	{
		sprintf(startChargeCmd,"%s","1ǹ�����������");		
		sprintf(stopChargeCMD,"%s","1ǹ����ֹͣ���");
		sprintf(isolateCMD,"%s","1ǹ��Ե�����");
	}else if(strcmp(item.itemName_,"ǹ2��Ե���")==0){
		sprintf(startChargeCmd,"%s","2ǹ�����������");		
		sprintf(stopChargeCMD,"%s","2ǹ����ֹͣ���");
		sprintf(isolateCMD,"%s","2ǹ��Ե�����");
	}

	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(30)|RELAY(2)))
	{
		goto DONE;
	}

	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"�޷�������磡");
		flag=FALSE; 
		goto DONE;
	}
	
	if(AlertDialogWithRet(0,"ǹ���","��ȷ�ϳ�繦������","����","��ȷ")==FALSE)
	{
		flag=FALSE;
		goto DONE;
	}
	
	//��ȡ���
	char inSoResult[10]={0};
	if(FALSE==ParamGetDepend(eut,isolateCMD,inSoResult))
	{
		WarnShow1(0,"��ȡ��Ե�����ʧ��"); 
	}
	
	//printf("%s\n",inSoResult);
	

	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"�޷�ֹͣ��磡�밴��ֹͣͣ"); 
		goto DONE;
	}	
	
	//WarnAlert(0,"��ʱ��",30);
	WarnShow1(0,"��һ������");
	
	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(29) | RELAY(30)| RELAY(2)))
	{
		goto DONE;
	}
	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"�޷�������磡");
		flag=FALSE; 
		goto DONE;
	}	

	if(AlertDialogWithRet(0,"ǹ���","��ȷ�ϳ�繦���쳣,Ӧ���Զ�ֹͣ���","����","��ȷ")==FALSE)
	{
		flag=FALSE;
	}
	
	char inSoResult1[10]={0};
	if(FALSE==ParamGetDepend(eut,isolateCMD,inSoResult1))
	{
		WarnShow1(0,"��ȡ��Ե�����ʧ��"); 
	}
	
	//printf("%s\n",inSoResult1);	
	
	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"�޷�ֹͣ��磡�밴��ֹͣͣ"); 
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
	WarnShow1(0,"��ȷ����������Ѿ�ֹͣ");	
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
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}*/
	//���ģ��ȷ��
	TestItem item1;
	ListGetItem(group.subItems,&item1,1);
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	if(strcmp(group.groupName,"ǹ1��繦��")==0)
	{
		sprintf(startChargeCmd,"%s","1ǹ�����������");		
		sprintf(stopChargeCMD,"%s","1ǹ����ֹͣ���");
	}else if(strcmp(group.groupName,"ǹ2��繦��")==0){
		sprintf(startChargeCmd,"%s","2ǹ�����������");		
		sprintf(stopChargeCMD,"%s","2ǹ����ֹͣ���");	
	}	
	
	RESULT itemResult1={0};
	itemResult1.index=item1.itemId;
	itemResult1.pass=1;
	
	/*if(ParamSet(servicePtr,"1ǹ�����������","1")==FALSE)
	{
		goto DONE;
	}
	onStubDisConnected(servicePtr);*/
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"�޷��������");
		return TEST_RESULT_ALLPASS;
	}
	
	if(AlertDialogWithRet(0,"ǹ���","��ȷ�ϳ������������,���ȷ�ϼӸ���","����","��ȷ")==FALSE)
	{
		flag=FALSE;
		itemResult1.pass=0;
		saveResult(hashTable,&itemResult1);
		//return TEST_RESULT_ALLPASS;
		ParamSetDepend(eut,stopChargeCMD,"1");	
		return TEST_RESULT_ALLPASS;
	}
	saveResult(hashTable,&itemResult1);
	
	//�Ӹ���
	if(OpenDo(eut.relayConfig,31)==FALSE)
	{
		return 	TEST_RESULT_ERROR;
	}	
	
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}*/

	WarnShow1(0,"����ȷ����ʼ����");

	ListType paramsToSet=ListCreate(sizeof(PARAMETER));
	for(int i=2;i<=4;i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);

		PARAMETER param={0};
		sprintf(param.paramName,"%s",item.itemName_);
		ListInsertItem(paramsToSet,&param,END_OF_LIST); 
		/*
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"�޸ò�������");
			goto DONE;
		}
		
		if(GetParameter(servicePtr,&param)<0)
		{	
			WarnShow1(0,"��ȡʧ��");
			goto DONE;
		}			
		//printf("----------------------------%s\n",param.value);
		ListInsertItem(paramsToGet,&param,END_OF_LIST);*/
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		itemResult.pass=0;
		//sprintf(itemResult.recvString,"%s",param.value);
		if(ParamGetDepend(eut,item.itemName_,itemResult.recvString)==FALSE)
		{
			 itemResult.pass=0;
			 saveResult(hashTable,&itemResult);
			 goto DONE;
		}
		float strValue=atof(itemResult.recvString);
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
			}else{
				itemResult.pass=0;
			}
		}else if(i==1)
		{
			if(strValue<2.87 && strValue>2.67)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}
		}
		saveResult(hashTable,&itemResult);			
	}
	
	
	
DONE:
	ListDispose(paramsToSet);
	//ListDispose(paramsToGet);
	
	if(ParamSetDepend(eut,stopChargeCMD,"1")==FALSE)
	{
		WarnShow1(0,"����ֹͣ���,�밴�¼�ͣ��ť");		
	}
	
	WarnShow1(0,"��ȷ���Ѿ�ֹͣ���");
	
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

METHODRET StubIoTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	if(strcmp(group.groupName,"ǹ1�ֶ���������")==0)
	{
		sprintf(startChargeCmd,"%s","1ǹ�����������");		
		sprintf(stopChargeCMD,"%s","1ǹ����ֹͣ���");
	}else if(strcmp(group.groupName,"ǹ2�ֶ���������")==0){
		sprintf(startChargeCmd,"%s","2ǹ�����������");		
		sprintf(stopChargeCMD,"%s","2ǹ����ֹͣ���");	
	}
	TestItem item={0};
	ListGetItem(group.subItems,&item,END_OF_LIST);
	RESULT result={0};
	result.index = item.itemId;
	result.pass = 0;
	
	if(ParamSetDepend(eut,startChargeCmd,"1")==FALSE)
	{
		WarnShow1(0,"�޷��������");
		goto DONE;
	}
	
	if(AlertDialogWithRet(0,"��ѡ��","��ȷ���ܹ��ֶ�����,���ֹͣ","����","��ȷ")==TRUE)
	{
		result.pass=1;	
	}else{
		if(ParamSetDepend(eut,stopChargeCMD,"1")==FALSE)
		{
			WarnShow1(0,"�޷�ֹͣ�������");
			goto DONE;
		}		
	}
	
	WarnShow1(0,"��ȷ��������̽��������û�У����¼�ͣ");
DONE:	
	saveResult(hashTable,&result); 		
	return ret;
	
}


TPS registerStubIoTestTps(void)
{
	TPS tps=newTps("unlock");
	tps.autoTestFunction=StubIoTest;
	return tps;	
}

