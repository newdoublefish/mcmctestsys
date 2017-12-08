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

static int mainPanelWidth=0;
static int mainPanelHeight=0;
static int paddingTops=25;
static int paddingLeft=100;
void adjustPanelSize(int panel)
{
	int monitor=0,height=0,width=0;	
	int footHeight=40;
	GetMonitorFromPanel (panel, &monitor);
	GetMonitorAttribute (monitor, ATTR_HEIGHT, &height);
	GetMonitorAttribute (monitor, ATTR_WIDTH, &width);
	mainPanelWidth=width-2*paddingLeft;//两边一样长
	mainPanelHeight=height-2*paddingTops;
	//设置主面板大小
	SetPanelAttribute(panel,ATTR_WIDTH,mainPanelWidth);
	SetPanelAttribute(panel,ATTR_HEIGHT,mainPanelHeight);
	SetPanelAttribute(panel,ATTR_TOP,VAL_AUTO_CENTER);//自动居中
	SetPanelAttribute(panel,ATTR_LEFT,VAL_AUTO_CENTER);
	//设置背景色大小
	SetCtrlAttribute(panel,PANEL_MAIN_DECROTE_HEADER,ATTR_TOP,0);
	SetCtrlAttribute(panel,PANEL_MAIN_DECROTE_HEADER,ATTR_LEFT,0);
	SetCtrlAttribute(panel,PANEL_MAIN_DECROTE_HEADER,ATTR_WIDTH,mainPanelWidth);
	SetCtrlAttribute(panel,PANEL_MAIN_DECROTE_HEADER,ATTR_HEIGHT,mainPanelHeight);
	//设置TAB的显示区域
	//header和body的关系是1比2
	//设置tst图标
	int imagePaddingSize=30;
	int headerHeight = mainPanelHeight/4;
	int headerWidth=mainPanelWidth;
	int imageHeight=headerHeight-2*imagePaddingSize;			 
	SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_TOP,imagePaddingSize);
	SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_LEFT,imagePaddingSize);
	SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_WIDTH,imageHeight);
	SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_HEIGHT,imageHeight);	//正方形
	
	//设置config图标
	SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_TOP,imagePaddingSize);
	SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_LEFT,imagePaddingSize+imageHeight+imagePaddingSize);
	SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_WIDTH,imageHeight);
	SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_HEIGHT,imageHeight);	//正方形
	
	//设置PANEL_MAIN_QUIT
	SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_TOP,imagePaddingSize);
	SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_LEFT,mainPanelWidth-2*imageHeight-2*imagePaddingSize);
	SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_WIDTH,imageHeight);
	SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_HEIGHT,imageHeight);	//正方形		
	
	//设置PANEL_MAIN_QUIT
	SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_TOP,imagePaddingSize);
	SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_LEFT,mainPanelWidth-imageHeight-imagePaddingSize);
	SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_WIDTH,imageHeight);
	SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_HEIGHT,imageHeight);	//正方形
	
	//设置body tab
	int bodyHeight=mainPanelHeight*3/4;
	int bodyWidth = mainPanelWidth;
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_TOP,headerHeight-footHeight);
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_LEFT,0);
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_WIDTH,bodyWidth);
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_HEIGHT,bodyHeight);	//正方形
	
	//设置footer
	SetCtrlAttribute(panel,PANEL_MAIN_VERSION_VALUE,ATTR_TOP,mainPanelHeight-40);
	GetCtrlAttribute(panel,PANEL_MAIN_VERSION_VALUE,ATTR_WIDTH,&width);
	SetCtrlAttribute(panel,PANEL_MAIN_VERSION_VALUE,ATTR_LEFT,mainPanelWidth-width-5);
	
	
	
	
	
	
	//printf("%d,%d,%d\n",monitor,height,width);
}

int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	
	if ((panelMain = LoadPanel (0, "MainPanel.uir", PANEL_MAIN)) >= 0)		   //测试主面板
	{
		adjustPanelSize(panelMain);
		if(GetPanelHandleFromTabPage(panelMain,PANEL_MAIN_TAB_MAIN,1,&tabconfig) == 0)
		{
			alignToParentPanel(tabconfig,TABPANEL_2_TAB);    		
			if(GetPanelHandleFromTabPage(tabconfig,TABPANEL_2_TAB,2,&tabEutConfig) < 0)
				return -1;
			if(GetPanelHandleFromTabPage(tabconfig,TABPANEL_2_TAB,1,&tabStrategyConfig) < 0)
				return -1;
			if(GetPanelHandleFromTabPage(tabconfig,TABPANEL_2_TAB,0,&tabHardwareConfig) < 0)
				return -1;  //tabSoftSetting
			if(GetPanelHandleFromTabPage(tabconfig,TABPANEL_2_TAB,3,&tabSystemConfig) < 0)
				return -1;  //tabSoftSetting
			
		}
		if(GetPanelHandleFromTabPage(panelMain,PANEL_MAIN_TAB_MAIN,0,&tabAuto) == 0)
		{
			
		}		
		
		SetCtrlAttribute(panelMain,PANEL_MAIN_TAB_MAIN,ATTR_TABS_VISIBLE,0);
		//SetCtrlAttribute(panelMain,ATTR_WIDTH,100);
		

		

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
		   char version[50]={0};
		   sprintf(version,"版本号:%s",getVersion());
		   SetCtrlVal(panelMain,PANEL_MAIN_VERSION_VALUE,version);
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


