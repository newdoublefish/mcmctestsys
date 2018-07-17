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
	
	SetCtrlVal(panelHandle,PANEL_MSG,"启动中");
	
	if(strcmp(group.groupName,"枪1充电功能")==0)
	{
		sprintf(startChargeCmd,"%s","1枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","1枪调试停止充电");
		gunIndex = 1;
	}else if(strcmp(group.groupName,"枪2充电功能")==0){
		sprintf(startChargeCmd,"%s","2枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","2枪调试停止充电");
		gunIndex = 2;
	}
	
	if(gunIndex ==0)
	{																												 
		SetCtrlVal(panelHandle,PANEL_MSG,"枪序号错误"); 
		goto DONE;
	}
	
	int result = 0;
	
	CheckGunPlugined(eut,gunIndex,&result);
	
	if(result == TRUE)
	{
		SetCtrlVal(panelHandle,PANEL_GUNSTATUS,"已插枪");
	}else{
		goto DONE;
	}
	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"无法启动充电");
		APPEND_INFO(msgHandler,"无法启动充电");
		APPEND_INFO_FORMAT(msgHandler,"离开%s",group.groupName);
		SetCtrlVal(panelHandle,PANEL_MSG,"启动失败");
		return TEST_RESULT_ALLPASS;
	}
	SetCtrlVal(panelHandle,PANEL_MSG,"启动成功"); 
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
				 APPEND_INFO_FORMAT(msgHandler,"%s获取失败功",item.itemName_);
			 	itemResult.pass=RESULT_FAIL;
			 	saveResult(hashTable,&itemResult);
			 	break;
			}else{
				APPEND_INFO_FORMAT(msgHandler,"%s获取成功，值为%s",item.itemName_,itemResult.recvString); 	
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
	
	SetCtrlVal(panelHandle,PANEL_MSG,"停止中........"); 
	
	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"无法停止充电");
		APPEND_INFO(msgHandler,"无法停止充电");
		APPEND_INFO_FORMAT(msgHandler,"离开%s",group.groupName);
		SetCtrlVal(panelHandle,PANEL_MSG,"停止失败");
		WarnShow1(0,"请确保充电流程已经停止");
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
				 APPEND_INFO_FORMAT(msgHandler,"%s获取失败功",item.itemName_);
			 	itemResult.pass=RESULT_FAIL;
			 	saveResult(hashTable,&itemResult);
			 	goto DONE;
			}else{
				APPEND_INFO_FORMAT(msgHandler,"%s获取成功，值为%s",item.itemName_,itemResult.recvString); 	
			}
			if(i ==1)
			{
				PlotStripChartPoint (panelHandle, PANEL_VOLTAGE, atof(itemResult.recvString));
				if(atof(itemResult.recvString)<5)
				{
					//itemResult.recvString
					goto DONE;
				}else if(quitFlag==FALSE){
					if(AlertDialogWithRet(0,"警告","充电未停止是否继续退出","不退出","退出")==TRUE)
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
	SetCtrlVal(panelHandle,PANEL_MSG,"已经停止"); 
	while(quitFlag == TRUE)
	{
		ProcessSystemEvents (); 	
	}
	DiscardPanel(panelHandle);
	
	APPEND_INFO(msgHandler,"离开测试");
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
