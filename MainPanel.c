#include "relayPanel.h"
#include <ansi_c.h>
#include "pwctrl.h"   //密码控件
#include <cvirte.h>		
#include <userint.h>
#include "MainPanel.h"
#include "sutCommon.h"
#include "eutConfig.h"
#include "policyConfig.h"  
#include "autoTest.h"
#include "common.h"
#include "appInit.h"
#include "testGroupInit.h"
#include "settingConfig.h"
#include "statusMonitor2.h"
#include "hardwareConfig.h"
#include "testInit.h"
#include "log.h"
#include "relayInstructionSet.h"

static int panelMain = 0;	//主面板

static int tabAuto =0 ;//自动测试tab

static int tabconfig;	//配置面板TAB
static int tabEutConfig;//EUT设置
static int tabStrategyConfig;//策略配置tab
static int tabHardwareConfig;  //硬件配置tab
static int tabSystemConfig;  //系统配置tab



//消息传递
int g_mainHWND = 0;   

int CVICALLBACK MainPnlMsgCallback(int panelHandle, int message,
                                      unsigned int* wParam,
                                      unsigned int* lParam,
                                      void* callbackData);


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	
	if ((panelMain = LoadPanel (0, "MainPanel.uir", PANEL_MAIN)) >= 0)		   //测试主面板
	{
		if(GetPanelHandleFromTabPage(panelMain,PANEL_MAIN_TAB_MAIN,1,&tabconfig) == 0)
		{
			if(GetPanelHandleFromTabPage(tabconfig,TABPANEL_2_TAB,2,&tabEutConfig) < 0)
				return -1;
			if(GetPanelHandleFromTabPage(tabconfig,TABPANEL_2_TAB,1,&tabStrategyConfig) < 0)
				return -1;
			if(GetPanelHandleFromTabPage(tabconfig,TABPANEL_2_TAB,0,&tabHardwareConfig) < 0)
				return -1;  //tabSoftSetting
			if(GetPanelHandleFromTabPage(tabconfig,TABPANEL_2_TAB,3,&tabSystemConfig) < 0)
				return -1;  //tabSoftSetting
			
		}
		
		SetCtrlAttribute(panelMain,PANEL_MAIN_TAB_MAIN,ATTR_TABS_VISIBLE,0);
	}

	InstallWinMsgCallback (panelMain, 9678,
                                      MainPnlMsgCallback,
                                      VAL_MODE_IN_QUEUE,
                                      NULL, &g_mainHWND);
	EnterAppInit();
	//登入界面
	//DisplayLoginPanel();
	DisplaySutConfigPanel();
	
	RunUserInterface ();
	DiscardPanel(panelMain);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------主界面操作
//---------------------------------------------------------------TABPANEL1
int CVICALLBACK PICTUREBUTTON_STATE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplayStatusMonitorPanel2();
			break;
	}
	return 0;
}



int CVICALLBACK PICTUREBUTTON_AUTO (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			HidePanel(panelMain); 
			DisplayAutoTestPanel(getItemList(),getEutList(),GetCollectList());
			break;
	}
	return 0;
}



//extern 
int CVICALLBACK MainPnlMsgCallback (int panelHandle, int message,
                                    unsigned int* wParam, unsigned int* lParam,
                                    void* callbackData)
{  
	
	
	if(*wParam==1) //测试用例分析成功
	{
		
		
	   //测试条例读取完毕，获取测试条例
		if(testInit(GetSeletedSut())<0)//初始化测试  
		{
		   DisplaySutConfigPanel();  
		}else{	
			
		   //显示版本号
		   SetCtrlVal(panelMain,PANEL_MAIN_VERSION_VALUE,getVersion());
	       //显示主面板
		   DisplayPanel(panelMain);
		   //显示设备配置面板
		   DisplayEutPanelAsTabPanel(tabEutConfig);
		   //显示策略配置面板
		   DisplayStrategyPanelAsTabPanel(tabStrategyConfig);
		   //显示软件设置面板
		   DisplaySettingPanelAsTabPanel(tabSystemConfig);
		   //显示硬件资源面板
		   DisplayHardwarePanelAsTabPanel(tabHardwareConfig);

		}
	}else if(*wParam==2)  //登入成功,进行系统选择
	{
	    LOG_EVENT(LOG_INFO,"login success");
		DisplaySutConfigPanel(); 
	}else if(*wParam==3) //退出自动测试界面
	{
	    DisplayPanel(panelMain);
	}else if(*wParam==9)
	{
	    AppDeInit();//这里可能会影响程序关闭
		QuitUserInterface(0); 
	}	
	
	return 0; 

}

int CVICALLBACK CLICKTAB (int panel, int control, int event,
		void *callbackData, int tabIndex, int eventData2)
{

	switch (event)
	{
		case EVENT_ACTIVE_TAB_CHANGE:
			if(isStrategyNeedToSave())
			{
				WarnShow("请先保存配置!");
			    SetActiveTabPage(tabconfig,TABPANEL_2_TAB,tabIndex); 
			}
			break;
	}
	return 0;

     
}

int CVICALLBACK EnableTst (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetActiveTabPage (panelMain, PANEL_MAIN_TAB_MAIN, 0);
			break;
	}
	return 0;
}


int CVICALLBACK EnableConf (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			SetActiveTabPage (panelMain, PANEL_MAIN_TAB_MAIN, 1);
			break;
	}
	return 0;
}

int CVICALLBACK BackMain (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplaySutConfigPanel();
			break;
	}
	return 0;
}

int CVICALLBACK QUITAPP (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			
			LOG_EVENT(LOG_INFO,"quit app!");  
			AppDeInit();//这里可能会影响程序关闭
			

			QuitUserInterface(0); 
			break;
	}
	return 0;
}

int CVICALLBACK PICTUREBUTTON_INSTRUCTIONCALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DisplayInstructionPanel();
			break;
	}
	return 0;
}


