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
extern TESTengine *gEngine; 
// 都数据池
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

METHODRET MatainTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
#if 0	
		//TODO:STROK分解结果
		ListType list = ListCreate(sizeof(tErrorMesg));
		if(FALSE==GetPileErrorList(eut,"prd erec disp 20\r\n",3,list))
		{
			APPEND_INFO(msgPanel,"获取列表失败");
			return ret;				
		}else{
			APPEND_INFO(msgPanel,"获取列表成功");
		}
		APPEND_INFO_FORMAT(msgPanel,"故障列表数量%d\n",ListNumItems(list));
		tErrorMesg msg={0};
		for(int i=1;i<=ListNumItems(list);i++)
		{
			 ListGetItem(list,&msg,i);
			 APPEND_INFO_FORMAT(msgPanel,"枪：%d,故障内容:%d,故障时间:%s\n",msg.gunIndex,msg.errorString,msg.errorTime);
		}
		ListDispose(list);
#else
		int panelHandle=LoadPanel(0,"ParamPanel.uir",MATAIN); 
		InstallPanelCallback(panelHandle,MaintainPanelCallback,NULL);
		ListType recordList = ListCreate(sizeof(tRecordMesg));
		if(FALSE==GetPileRecordList(eut,"prd crec disp 1\r\n",3,recordList))
		{
			APPEND_INFO(msgPanel,"获取列表失败");
			return ret;				
		}else{
			APPEND_INFO(msgPanel,"获取列表成功");
		}
		APPEND_INFO_FORMAT(msgPanel,"充电记录数量%d\n",ListNumItems(recordList));
		SetCtrlVal(panelHandle,MATAIN_TEXTBOX,"获取充电记录成功");
		tRecordMesg recordMsg={0};
		for(int i=1;i<=ListNumItems(recordList);i++)
		{
			 ListGetItem(recordList,&recordMsg,i);
			 APPEND_INFO_FORMAT(msgPanel,"枪：%d,故障原因1:%d,故障原因2:%d\n",recordMsg.gunIndex,recordMsg.reseason1,recordMsg.reseason2);
			 char temp[100]={0};
			 sprintf(temp,"枪：%d,故障原因1:%d,故障原因2:%d\n",recordMsg.gunIndex,recordMsg.reseason1,recordMsg.reseason2);
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
	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);		
	return ret;
}



TPS registerMatainTPS(void)
{
	TPS tps=newTps("matain");
	tps.testFunction=MatainTest;
	return tps;
}


METHODRET DemoTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;
		if(AlertDialogWithRet(0,"请选择","请确认结果是否正确","错误","正确")==TRUE)
		{
			itemResult.pass=1;	
		}else
			itemResult.pass=0;
		APPEND_INFO_FORMAT(msgPanel,"测试条例:%s的测试结果为：%s",item.itemName_,(itemResult.pass==1)?"合格":"不合格");
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s","helloworld");
		saveResult(hashTable,&itemResult);
		
	}
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);		
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
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
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
			char *buffer=NULL;
			buildPostDataStr(data,&buffer,NULL,NULL);
			if(1==httpPostJson(data.url,buffer))
			{
				itemResult.pass = RESULT_PASS;
			}
			if(buffer!=NULL)
				free(buffer);
		}
		saveResult(hashTable,&itemResult);
		
	}
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);		
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
	//APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"电表",&resconfig))
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
				SetCtrlVal(msgPanel,METER_MSG,"电压值:");
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
				SetCtrlVal(msgPanel,METER_MSG,"电流值:");
				SetCtrlVal(msgPanel,METER_MSG,itemResult.recvString);
				SetCtrlVal(msgPanel,METER_MSG,"\n");				
			}
			saveResult(hashTable,&itemResult);
		
		}
	}
	//APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);
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



METHODRET ACTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	tTestProject *projectPtr = getCurrentProject();  
	TESTobject obj = gEngine->objectArray[0];
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName);
	APPEND_INFO_FORMAT(msgPanel,"测试次数:%d",gEngine->reTestCnt);
	APPEND_INFO_FORMAT(msgPanel,"报告存放地址:%s",projectPtr->projectDir);
	char prex[20]={0};
	int startNum =0;
	int totalCnt =0;
	char gunPrex[20]={0};
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		if(i==1)
		{
			sprintf(prex,"%s",item.inputValue_);
			sprintf(gunPrex,"%s",item.standard_);
		}else if(i==2)
		{
			startNum = atoi(item.inputValue_);
		}else if(i==3){
		    totalCnt=atoi(item.inputValue_);
		}
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		itemResult.pass = RESULT_PASS;
		saveResult(hashTable,&itemResult);
	}
	
	for(int i=0;i<totalCnt;i++)
	{
		RESULT re1={0};
		re1.index=1;
		RESULT re4={0};
		re4.index=4;
		RESULT re5={0};
		re5.index=5;
		RESULT re6={0};
		re6.index=6;
		RESULT re7={0};
		re7.index=7;		
		sprintf(re1.recvString,"%s%d",prex,startNum+i);
		APPEND_INFO_FORMAT(msgPanel,"铭牌编号:%s",re1.recvString);
		sprintf(re4.recvString,"%s%d%d",gunPrex,startNum+i,1);
		sprintf(re5.recvString,"%s%d%d",gunPrex,startNum+i,2);  
		sprintf(re6.recvString,"%s%d%d",gunPrex,startNum+i,3);  
		sprintf(re7.recvString,"%s%d%d",gunPrex,startNum+i,4);
		APPEND_INFO_FORMAT(msgPanel,"%s,%s,%s,%s",re4.recvString,re5.recvString,re6.recvString,re7.recvString); 
		saveResult(hashTable,&re1); 
		saveResult(hashTable,&re4); 
		saveResult(hashTable,&re5); 
		saveResult(hashTable,&re6); 
		saveResult(hashTable,&re7); 
		char fileName[256]={0};
		sprintf(fileName,"%s\\%s-整机测试.xlsx",projectPtr->projectDir,re1.recvString);
		saveResultToExcelFile(fileName,obj);
		APPEND_INFO_FORMAT(msgPanel,"fileName:%s",fileName); 
	}
	APPEND_INFO_FORMAT(msgPanel,"%s,%s,%d,%d\n",prex,gunPrex,startNum,totalCnt);
	APPEND_INFO_FORMAT(msgPanel,"结束测试:%s",group.groupName);
	if(AlertDialogWithRet(0,"请选择","请确认结果是否正确","错误","正确")==TRUE)
	{
			
	}	
	return ret;		
}


TPS registerACTPS(void)
{
	TPS tps=newTps("ac");
	tps.testFunction=ACTest;
	return tps;
}

#define SHEET_RANGE_TIPS "B1:J1" 
ListType reportList=0;

typedef struct{
	char nameplate[30];
	char sim[100];
	char controller[30];
	char ammeter1[30];
	char ammeter2[30];
	char gunIndex1[30];
	char gunIndex2[30];
	char vendor[30];
	char sn[30];
}tReport;


static HRESULT onCellListenerGetReports(VARIANT *MyVariant,int row,int column)    
{
	char *temp;
	//static char key[50]={0}; 
	static tReport param;
	if(column==0)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			//memcpy(tempInstruction.name,temp,strlen(temp)+1);
			memset(&param,0,sizeof(tReport));
			sprintf(param.nameplate,"%s",temp);
			//printf("%s,",temp);
			CA_FreeMemory(temp);
		 }else
		 {
			return EXCEL_TASK_QUIT;
		 }
	}else if(column==2)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			sprintf(param.sim,"%s",temp);
			CA_FreeMemory(temp);
		 }
	}else if(column==3)
	{	 
		if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			sprintf(param.controller,"%s",temp);
			CA_FreeMemory(temp);
		 }
	}else if(column==4)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			sprintf(param.ammeter1,"%s",temp);
			CA_FreeMemory(temp);
		 }
	}else if(column==5)
	{
		if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			sprintf(param.ammeter2,"%s",temp);
			CA_FreeMemory(temp);
		 }
	}else if(column==6)
	{
		if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			sprintf(param.gunIndex1,"%s",temp);
			CA_FreeMemory(temp);
		 }
	}else if(column==7)
	{
		if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			sprintf(param.gunIndex2,"%s",temp);
			CA_FreeMemory(temp);
		 }
	}else if(column==8)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			sprintf(param.vendor,"%s",temp);
			CA_FreeMemory(temp);
		 }
	}else if(column==9)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			sprintf(param.sn,"%s",temp);
			CA_FreeMemory(temp);
		 }
		 if(reportList!=0)
		 {
			 //printfParam(param);
			 ListInsertItem(reportList,&param,END_OF_LIST);
		 }
	}
	return EXCEL_TASK_OK;
}


static HRESULT onStartGetReports(VARIANT *MyVariant,int row,int column)				
{
	//istList=ListCreate(sizeof(INSTRUCTION));
	reportList = ListCreate(sizeof(tReport)); 
    return 0;	
}


BOOL ReportProtocolInit(char *sheetName)
{
	if(reportList!=0)
		return TRUE;
	SUT sut=GetSeletedSut();
	EXCELTask task=createExcelTask(sut.configPath,sheetName,SHEET_RANGE_TIPS,10);
	task.onExcelTaskStartListener=(void *)onStartGetReports;
	task.onCellListener=(void *)onCellListenerGetReports;
	runExcelTask(task);
	return TRUE;	
}

METHODRET ReportTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	tReport report = {0};
	TESTobject obj = gEngine->objectArray[0];
	tTestProject *projectPtr = getCurrentProject(); 
	//printf("%d\n",ListNumItems(reportList));
	
	for(int i=1;i<=ListNumItems(reportList);i++)
	{
		int flag = 1;
		ListGetItem(reportList,&report,i);  
		for(int j=1;j<=ListNumItems(group.subItems);j++)
		{
			TestItem item;
			ListGetItem(group.subItems,&item,j);
			RESULT itemResult={0};
			itemResult.index=item.itemId;
			if(itemResult.index == 1)
			{
				sprintf(itemResult.recvString,"%s", report.nameplate); 
			}else if(itemResult.index == 2)
			{
				sprintf(itemResult.recvString,"%s", report.sim); 
			}else if(itemResult.index == 3)
			{
				sprintf(itemResult.recvString,"%s", report.ammeter1); 
			}else if(itemResult.index == 4)
			{
				sprintf(itemResult.recvString,"%s", report.ammeter2); 
			}else if(itemResult.index == 5)
			{
				sprintf(itemResult.recvString,"%s", report.gunIndex1); 
			}else if(itemResult.index == 6)
			{
				sprintf(itemResult.recvString,"%s", report.gunIndex2); 
			}else if(itemResult.index == 7)
			{
				sprintf(itemResult.recvString,"%s", report.vendor); 
			}else if(itemResult.index == 8)
			{
				sprintf(itemResult.recvString,"%s", report.controller); 
			}else if(itemResult.index == 9)
			{
				sprintf(itemResult.recvString,"%s", report.sn); 
			}else{
				float standard = atof(item.inputValue_);
				float tolerance = atof(item.standard_);
				float value = standard + Random(0,tolerance);
				sprintf(itemResult.recvString,"%0.2f",value);
			}
			
			if(strlen(itemResult.recvString)==0)
			{
				flag = 0;
			}
			saveResult(hashTable,&itemResult);
		
		}		
		/*
				RESULT result1={0};
		result1.index = 1;
		sprintf(result1.recvString,"%s", report.nameplate);
		saveResult(hashTable,&result1);
				RESULT result2={0};
		result2.index = 2;
		sprintf(result2.recvString,"%s", report.sim);
		saveResult(hashTable,&result2);
				RESULT result3={0};
		result3.index = 3;
		sprintf(result3.recvString,"%s", report.ammeter1);
		saveResult(hashTable,&result3);
				RESULT result4={0};
		result4.index = 4;
		sprintf(result4.recvString,"%s", report.ammeter2);
		saveResult(hashTable,&result4);
				RESULT result5={0};
		result5.index = 5;
		sprintf(result5.recvString,"%s", report.gunIndex1);
		saveResult(hashTable,&result5);
				RESULT result6={0};
		result6.index = 6;
		sprintf(result6.recvString,"%s", report.gunIndex2);
		saveResult(hashTable,&result6);
				RESULT result7={0};
		result7.index = 7;
		sprintf(result7.recvString,"%s", report.vendor);
		saveResult(hashTable,&result7);
				RESULT result8={0};
		result8.index = 8;
		sprintf(result8.recvString,"%s", report.controller);
		saveResult(hashTable,&result8);
				RESULT result9={0}; 
		result9.index = 9;
		sprintf(result9.recvString,"%s", report.sn);
		saveResult(hashTable,&result9);			
		*/
		char fileName[256]={0};
		if(flag ==0)
		{
			sprintf(fileName,"%s\\%s-整机测试-不完整.xlsx",projectPtr->projectDir,report.nameplate);
		}else{
			sprintf(fileName,"%s\\%s-整机测试.xlsx",projectPtr->projectDir,report.nameplate);
		}
		saveResultToExcelFile(fileName,obj);
		APPEND_INFO_FORMAT(msgPanel,"%d fileName:%s",i,fileName);
		//break;
	}
	return ret;
}


TPS registerReportTPS(void)
{
	TPS tps=newTps("report");
	tps.testFunction=ReportTest;
	tps.protocolInit = ReportProtocolInit;
	return tps;
}
