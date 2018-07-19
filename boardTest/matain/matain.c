#include <userint.h>
#include "excelHelper.h" 
#include "tpsHelper.h"
#include "resultSave.h"
#include "common.h"
#include "postData.h"
#include "sutCommon.h"
#include "httpPost.h"
#include "EutHelper.h"
#include "meter.h"
#include "ParamPanel.h"
#include "testFramework.h"
#include "testProject.h"
#include "resultSave.h"
#include "ParamSetGet.h"
#include "ParamExcelParse.h"
#include "StubNetService.h"
#include "MatainPanel.h"

HashTableType erroMsgHashTable=0;  

int CVICALLBACK MaintainChargingPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			//if(checkScanResult(panelHandle)==TRUE) 
			 //QuitUserInterface(1);
			 int *runFlag = (int*)callbackData;
			 *runFlag = FALSE;
		     break;
	}
	return 0;
}

int CVICALLBACK MaintainPanelStopButtonCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(control == PANEL_STOP)
			 {
				int *flag = (int *)callbackData;
				*flag = FALSE;
			 }
			 break;
	}
	return 0;  
}

extern BOOL CheckGunPlugined(EUT eut,int gunIndex,int *result);

METHODRET MatainChargingTest(TestGroup group,EUT eut,HashTableType hashTable,int msgHandler)
{
	METHODRET ret = TEST_RESULT_ALLPASS; 
	BOOL quitFlag = TRUE;
	BOOL quitButton = TRUE;
	int panelHandle = 0;
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	int gunIndex=0;
	
	
	panelHandle = LoadPanel(0,"MatainPanel.uir",PANEL);
	InstallPanelCallback(panelHandle,MaintainChargingPanelCallback,&quitFlag);
	InstallCtrlCallback(panelHandle,PANEL_STOP,MaintainPanelStopButtonCallback,&quitButton);
	DisplayPanel(panelHandle);
	
	SetCtrlVal(panelHandle,PANEL_MSG,"������");
	
	if(strcmp(group.groupName,"ǹ1��繦��")==0)
	{
		sprintf(startChargeCmd,"%s","1ǹ�����������");		
		sprintf(stopChargeCMD,"%s","1ǹ����ֹͣ���");
		gunIndex = 1;
	}else if(strcmp(group.groupName,"ǹ2��繦��")==0){
		sprintf(startChargeCmd,"%s","2ǹ�����������");		
		sprintf(stopChargeCMD,"%s","2ǹ����ֹͣ���");
		gunIndex = 2;
	}
	
	if(gunIndex ==0)
	{																												 
		SetCtrlVal(panelHandle,PANEL_MSG,"ǹ��Ŵ���"); 
		goto DONE;
	}
	
	int result = 0;
	
	CheckGunPlugined(eut,gunIndex,&result);
	
	if(result == TRUE)
	{
		SetCtrlVal(panelHandle,PANEL_GUNSTATUS,"�Ѳ�ǹ");
	}else{
		goto DONE;
	}
	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"�޷��������");
		APPEND_INFO(msgHandler,"�޷��������");
		APPEND_INFO_FORMAT(msgHandler,"�뿪%s",group.groupName);
		SetCtrlVal(panelHandle,PANEL_MSG,"����ʧ��");
		return TEST_RESULT_ALLPASS;
	}
	SetCtrlVal(panelHandle,PANEL_MSG,"�����ɹ�"); 
	double elapsed = 120; 
	double outTime = Timer();
	while(quitFlag && quitButton)
	{
		double currentTime = Timer();
		for(int i=1;i<=ListNumItems(group.subItems);i++)
		{
			TestItem item;
			ListGetItem(group.subItems,&item,i);

			PARAMETER param={0};
			sprintf(param.paramName,"%s",item.itemName_);
			RESULT itemResult={0};
			itemResult.index=item.itemId;
			itemResult.pass=RESULT_PASS;
					//sprintf(itemResult.recvString,"%s",param.value);
			if(ParamGetDepend(eut,item.itemName_,itemResult.recvString)==FALSE)
			{
				 APPEND_INFO_FORMAT(msgHandler,"%s��ȡʧ�ܹ�",item.itemName_);
			 	itemResult.pass=RESULT_FAIL;
			 	saveResult(hashTable,&itemResult);
			 	break;
			}else{
				APPEND_INFO_FORMAT(msgHandler,"%s��ȡ�ɹ���ֵΪ%s",item.itemName_,itemResult.recvString); 	
			}
			
			if(i ==1)
			{
				//SetCtrlVal (panelHandle, PANEL_VOLTAGE, atof(itemResult.recvString));  
				PlotStripChartPoint (panelHandle, PANEL_CURRENT, atof(itemResult.recvString));
			
			}else if(i==2){
				//SetCtrlVal (panelHandle, PANEL_CURRENT, atof(itemResult.recvString));
				PlotStripChartPoint (panelHandle, PANEL_VOLTAGE, atof(itemResult.recvString));   
			}else if(i==3)
			{
				PlotStripChartPoint (panelHandle, PANEL_POWER, atof(itemResult.recvString));
			}
			
			saveResult(hashTable,&itemResult);
			
		}
		Delay(0.5);
		/*if(currentTime-outTime > elapsed)
		{
			quitFlag = FALSE;
		}*/
		ProcessSystemEvents (); 	
	}
	
	SetCtrlVal(panelHandle,PANEL_MSG,"ֹͣ��........"); 
	
	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"�޷�ֹͣ���");
		APPEND_INFO(msgHandler,"�޷�ֹͣ���");
		APPEND_INFO_FORMAT(msgHandler,"�뿪%s",group.groupName);
		SetCtrlVal(panelHandle,PANEL_MSG,"ֹͣʧ��");
		WarnShow1(0,"��ȷ����������Ѿ�ֹͣ");
	}
	
	while(TRUE)
	{
		for(int i=1;i<=ListNumItems(group.subItems);i++)
		{
			TestItem item;
			ListGetItem(group.subItems,&item,i);

			PARAMETER param={0};
			sprintf(param.paramName,"%s",item.itemName_);
			RESULT itemResult={0};
			itemResult.index=item.itemId;
			itemResult.pass=RESULT_PASS;
					//sprintf(itemResult.recvString,"%s",param.value);
			if(ParamGetDepend(eut,item.itemName_,itemResult.recvString)==FALSE)
			{
				 APPEND_INFO_FORMAT(msgHandler,"%s��ȡʧ�ܹ�",item.itemName_);
			 	itemResult.pass=RESULT_FAIL;
			 	saveResult(hashTable,&itemResult);
			 	goto DONE;
			}else{
				APPEND_INFO_FORMAT(msgHandler,"%s��ȡ�ɹ���ֵΪ%s",item.itemName_,itemResult.recvString); 	
			}
			if(i ==1)
			{
				PlotStripChartPoint (panelHandle, PANEL_VOLTAGE, atof(itemResult.recvString));
				if(atof(itemResult.recvString)<5)
				{
					//itemResult.recvString
					goto DONE;
				}else if(quitFlag==FALSE){
					if(AlertDialogWithRet(0,"����","���δֹͣ�Ƿ�����˳�","���˳�","�˳�")==TRUE)
	  				{
		   				goto DONE;
	   				}else{
		
	   				}
				}
			
			}else if(i==2){
				//SetCtrlVal (panelHandle, PANEL_CURRENT, atof(itemResult.recvString));
				PlotStripChartPoint (panelHandle, PANEL_CURRENT, atof(itemResult.recvString));
			}
		}		
	
	}
	
	
	
	
	
DONE:
	SetCtrlVal(panelHandle,PANEL_MSG,"�Ѿ�ֹͣ"); 
	while(quitFlag == TRUE)
	{
		ProcessSystemEvents (); 	
	}
	DiscardPanel(panelHandle);
	
	APPEND_INFO(msgHandler,"�뿪����");
	return ret;
}

TPS registerMatainChargingTestTps(void)
{
	TPS tps=newTps("matain_charging");
	tps.testFunction=MatainChargingTest;
	return tps;	
}

METHODRET InsulationTest2(TestGroup group,EUT eut,HashTableType hashTable,int msgHander);
TPS registerMatainInsulationTestTps(void)
{
	TPS tps=newTps("matain_insulation");
	tps.testFunction=InsulationTest2;
	return tps;	
}


// g 2 2
// g 7 2

int CVICALLBACK MaintainPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			//if(checkScanResult(panelHandle)==TRUE) 
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}


HRESULT getErrorMessage(int key,char *value)
{
   int found=0; 
   HashTableFindItem(erroMsgHashTable,&key,&found);
   if(found==1)
   {
   	   HashTableGetItem(erroMsgHashTable,&key,value,256);
   }
   return found;
}


METHODRET MatainTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"��ʼ����:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
#if 0	
		//TODO:STROK�ֽ���
		ListType list = ListCreate(sizeof(tErrorMesg));
		if(FALSE==GetPileErrorList(eut,"prd erec disp 20\r\n",3,list))
		{
			APPEND_INFO(msgPanel,"��ȡ�б�ʧ��");
			return ret;				
		}else{
			APPEND_INFO(msgPanel,"��ȡ�б�ɹ�");
		}
		APPEND_INFO_FORMAT(msgPanel,"�����б�����%d\n",ListNumItems(list));
		tErrorMesg msg={0};
		for(int i=1;i<=ListNumItems(list);i++)
		{
			 ListGetItem(list,&msg,i);
			 APPEND_INFO_FORMAT(msgPanel,"ǹ��%d,��������:%d,����ʱ��:%s\n",msg.gunIndex,msg.errorString,msg.errorTime);
		}
		ListDispose(list);
#else
		int panelHandle=LoadPanel(0,"MatainPanel.uir",MATAIN); 
		InstallPanelCallback(panelHandle,MaintainPanelCallback,NULL);
		ListType recordList = ListCreate(sizeof(tRecordMesg));
		if(FALSE==GetPileRecordList(eut,"prd crec disp 1\r\n",3,recordList))
		{
			APPEND_INFO(msgPanel,"��ȡ�б�ʧ��");
			return ret;				
		}else{
			APPEND_INFO(msgPanel,"��ȡ�б�ɹ�");
		}
		APPEND_INFO_FORMAT(msgPanel,"����¼����%d\n",ListNumItems(recordList));
		SetCtrlVal(panelHandle,MATAIN_TEXTBOX,"��ȡ����¼�ɹ�");
		tRecordMesg recordMsg={0};
		for(int i=1;i<=ListNumItems(recordList);i++)
		{
			 ListGetItem(recordList,&recordMsg,i);
			 APPEND_INFO_FORMAT(msgPanel,"ǹ��%d,����ԭ��1:%d,����ԭ��2:%d\n",recordMsg.gunIndex,recordMsg.reseason1,recordMsg.reseason2);
			 char temp[1024]={0};
			 char resean1[256]={0};
			 char resean2[256]={0};
			 getErrorMessage(recordMsg.reseason1,resean1);
			 getErrorMessage(recordMsg.reseason2,resean2);
			 sprintf(temp,"ǹ��%d\n������1:%d,����ԭ��1:%s\n������2:%d,����ԭ��2:%s\n",recordMsg.gunIndex,recordMsg.reseason1,resean1,recordMsg.reseason2,resean2);
			 SetCtrlVal(panelHandle,MATAIN_TEXTBOX,temp); 
		}
		ListDispose(recordList);
		
		
		ListType list = ListCreate(sizeof(tDataPoolItem));
		GetDataPoolGroupList(eut,2,2,3,list); 
		GetDataPoolGroupList(eut,7,2,3,list);

		InsertTableRows (panelHandle,MATAIN_TABLE,-1,ListNumItems(list),VAL_USE_MASTER_CELL_TYPE);
		tDataPoolItem msg;
		int showCnt = 1;
		for(int i=1;i<=ListNumItems(list);i++)
		{
			 ListGetItem(list,&msg,i);
			 APPEND_INFO_FORMAT(msgPanel,"%s %s",msg.name,msg.status);
			 char temp[5]={0};
			 sprintf(temp,"%d",msg.gunIndex);
			 if(strstr(msg.status,"true")==NULL)
				 continue;
			 SetTableCellVal(panelHandle,MATAIN_TABLE,MakePoint(1,showCnt),temp);
			 SetTableCellVal(panelHandle,MATAIN_TABLE,MakePoint(2,showCnt),msg.name);
			 SetTableCellVal(panelHandle,MATAIN_TABLE,MakePoint(3,showCnt),msg.status);
			 showCnt++;
		}
		DisplayPanel(panelHandle);
		RunUserInterface();
		DiscardPanel(panelHandle);
		ListDispose(list);
#endif
	
	APPEND_INFO_FORMAT(msgPanel,"%s�������",group.groupName);		
	return ret;
}

static HRESULT onStartErrorMessage(VARIANT *MyVariant,int row,int column)				
{
	//istList=ListCreate(sizeof(INSTRUCTION));
	HashTableCreate(10,FIXED_SIZE_KEY,sizeof(int),256,&erroMsgHashTable);
    return 0;	
}

static HRESULT onCellListenerErrorMessage(VARIANT *MyVariant,int row,int column)    
{
	char *temp;
	static int key; 
	if(column==0)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			//memcpy(tempInstruction.name,temp,strlen(temp)+1);
			char keystr[50]={0};;
			memcpy(keystr,temp,strlen(temp)+1);
			key = atoi(keystr);
			//printf("%s,",temp);
			CA_FreeMemory(temp);
		 }else
		 {
			return EXCEL_TASK_QUIT;
		 }
	}else if(column==1)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
		    //tempInstruction.operation.testType=(unsigned char)StrToUnsignedLong(temp);   
			if(erroMsgHashTable!=0)
			{
			   HashTableInsertItem(erroMsgHashTable,&key,temp);
			}
			//printf("%s\n",temp); 
			CA_FreeMemory(temp);
		 }
	}
	return EXCEL_TASK_OK;
}


#define SHEET_RANGE_TIPS "A2:B2"
BOOL MatainErrorMsgProtocolInit(char *sheetName)
{
	if(erroMsgHashTable!=0)
		return TRUE;
	SUT sut=GetSeletedSut();
	EXCELTask task=createExcelTask(sut.configPath,sheetName,SHEET_RANGE_TIPS,2);
	task.onExcelTaskStartListener=(void *)onStartErrorMessage;
	task.onCellListener=(void *)onCellListenerErrorMessage;
	runExcelTask(task);
	return TRUE;	
}





TPS registerMatainTPS(void)
{
	TPS tps=newTps("matain");
	tps.testFunction=MatainTest;
	tps.protocolInit = MatainErrorMsgProtocolInit;
	return tps;
}


METHODRET MatainWatchChargingInfoTest(TestGroup group,EUT eut,HashTableType hashTable,int msgHandler)
{
	METHODRET ret = TEST_RESULT_ALLPASS; 
	BOOL quitFlag = TRUE;
	int panelHandle = 0;


	
	
	panelHandle = LoadPanel(0,"MatainPanel.uir",WATCH);
	if(strcmp(group.groupName,"ǹ1��繦��")==0)
	{
		WarnAlert(0,"����APP����ǹ1���,����Ѿ���������ȷ������",30);
		
	}else if(strcmp(group.groupName,"ǹ2��繦��")==0){
		WarnAlert(0,"����APP����ǹ2���,����Ѿ���������ȷ������",30);
	}
	SetPanelAttribute(panelHandle,ATTR_TITLE,group.groupName);   
	InstallPanelCallback(panelHandle,MaintainChargingPanelCallback,&quitFlag);
	DisplayPanel(panelHandle);
	float current=0;
	float voltage=0;
	while(quitFlag)
	{
		double currentTime = Timer();
		for(int i=1;i<=ListNumItems(group.subItems);i++)
		{
			TestItem item;
			ListGetItem(group.subItems,&item,i);

			PARAMETER param={0};
			sprintf(param.paramName,"%s",item.itemName_);
			RESULT itemResult={0};
			itemResult.index=item.itemId;
			itemResult.pass=RESULT_PASS;
					//sprintf(itemResult.recvString,"%s",param.value);
			if(ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3)==FALSE)
			{
				 APPEND_INFO_FORMAT(msgHandler,"%s��ȡʧ�ܹ�",item.itemName_);
			 	itemResult.pass=RESULT_FAIL;
			 	saveResult(hashTable,&itemResult);
			 	goto DONE;
			}else{
				APPEND_INFO_FORMAT(msgHandler,"%s��ȡ�ɹ���ֵΪ%s",item.itemName_,itemResult.recvString); 	
			}
			
			if(i ==1)
			{
				//SetCtrlVal (panelHandle, PANEL_VOLTAGE, atof(itemResult.recvString));  
				current =  atof(itemResult.recvString);
				PlotStripChartPoint (panelHandle, WATCH_CURRENT, current);
				SetCtrlVal(panelHandle,WATCH_CURRENT_TX,itemResult.recvString);
			
			}else if(i==2){
				voltage = atof(itemResult.recvString);
				//SetCtrlVal (panelHandle, PANEL_CURRENT, atof(itemResult.recvString));
				PlotStripChartPoint (panelHandle, WATCH_VOLTAGE, voltage);
				SetCtrlVal(panelHandle,WATCH_VOLTAGE_TX,itemResult.recvString);
			}else if(i==3)
			{
				char temp[20]={0};
				sprintf(temp,"%0.2f",current*voltage);
				PlotStripChartPoint (panelHandle, WATCH_POWER, current*voltage);
				SetCtrlVal(panelHandle,WATCH_POWER_TX,temp);
			}
			
			saveResult(hashTable,&itemResult);
			
		}
		Delay(0.5);
		/*if(currentTime-outTime > elapsed)
		{
			quitFlag = FALSE;
		}*/
		ProcessSystemEvents (); 	
	}
DONE:
	DiscardPanel(panelHandle);
	
	APPEND_INFO(msgHandler,"�뿪����");
	return ret;
}

TPS registerMatainWatchChargingTestTps(void)
{
	TPS tps=newTps("matain_watch");
	tps.testFunction=MatainWatchChargingInfoTest;
	return tps;	
}



int CVICALLBACK MatainCodeInputPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			//if(checkScanResult(panelHandle)==TRUE) 
			 //QuitUserInterface(1);
			 int *runFlag = (int*)callbackData;
			 *runFlag = FALSE;
			 QuitUserInterface(0);
		     break;
	}
	return 0;
}

int CVICALLBACK MatainCodeInputButtonCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			 QuitUserInterface(0); 
			 break;
	}
	return 0;  
}


METHODRET MatainCodeInput(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"��ʼ����:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	int quitFlag = TRUE;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		//HashTableGetItem(hashTable,&item.itemId,&itemResult,sizeof(RESULT)); 
		int panelHandle = LoadPanel(0,"MatainPanel.uir",PANEL_CODE);
		SetPanelAttribute(panelHandle,ATTR_TITLE,item.itemName_);
		InstallPanelCallback(panelHandle,MatainCodeInputPanelCallback,&quitFlag);
		InstallCtrlCallback(panelHandle,PANEL_CODE_COMMANDBUTTON,MatainCodeInputButtonCallback,NULL);
		DisplayPanel(panelHandle);
		RunUserInterface();
		char setValue[50]={0};
		GetCtrlVal(panelHandle,PANEL_CODE_STRING,setValue);
		DiscardPanel(panelHandle);
		if(quitFlag == TRUE)
		{

			if(FALSE==ParamSetDependWithRetry(eut,item.itemName_,setValue,3))
			{
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(msgPanel,"%s ���óɹ���ֵΪ%s",item.itemName_,setValue);
			}
			APPEND_INFO(msgPanel,"��ʱ2s"); 
			Delay(2);
			if(FALSE == ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3))
			{
				APPEND_INFO_FORMAT(msgPanel,"%s ��ȡʧ��",item.itemName_);
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(msgPanel,"%s ��ȡ�ɹ���ֵΪ%s",item.itemName_,itemResult.recvString);	
			}
			if(strcmp(itemResult.recvString,setValue)==0)
			{
				itemResult.pass=RESULT_PASS;
			}else{
				itemResult.pass=RESULT_FAIL;
			}						
			
		}
DONE:		
		
		saveResult(hashTable,&itemResult);  
	}
	APPEND_INFO_FORMAT(msgPanel,"%s�������",group.groupName);	

	return ret;
}

TPS registerMatainCodeInputTestTPS(void)
{
	TPS tps=newTps("matain_codeinput");
	tps.testFunction=MatainCodeInput;
	return tps;
}
