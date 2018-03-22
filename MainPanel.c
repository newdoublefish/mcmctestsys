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
#include <ansi_c.h>
#include "pwctrl.h"   //密码控件
#include <cvirte.h>		
#include <userint.h>
#include "MainPanel.h"
#include "sutCommon.h"
#include "eutConfig.h"
#include "policyConfig.h" 
#include "testProject.h"  
#include "autoTest.h"
#include "common.h"
#include "appInit.h"
#include "testGroupInit.h"
#include "settingConfig.h"
#include "ftpConfigView.h"
#include "testInit.h"
#include "log.h"
#include "login.h"
#include "reportDb.h"


#define COPY_RIGHT "CopyRight 2018  GuangDong Thousands of Cities & Charging Stations E-Vehicles Operating Co., Ltd."


static int panelMain = 0;	//主面板

static int tabAuto =0 ;//自动测试tab

static int tabconfig;	//配置面板TAB
static int tabEutConfig;//EUT设置
static int tabStrategyConfig;//策略配置tab
static int tabHardwareConfig;  //硬件配置tab
static int tabSystemConfig;  //系统配置tab

//tTestProject gProject={0};



//消息传递
int g_mainHWND = 0;   

int CVICALLBACK MainPnlMsgCallback(int panelHandle, int message,
                                      unsigned int* wParam,
                                      unsigned int* lParam,
                                      void* callbackData);

static int mainPanelWidth=0;
static int mainPanelHeight=0;
static int headerHeight=0;
#define  MAINPANEL_HEADER_BUTTON_PADDINGSIZE 30
#define  MAINPANEL_PADDING_TOP 25
#define  MAINPANEL_PADDING_LEFT 150
#define  MAINPANLE_FOOTER_HEIGHT 40
static void adjustHeader(int panel)
{
	int buttonSize = 0;
	GetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_WIDTH,&buttonSize);
	//设置tst图标 左边第一个
	SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_TOP,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_LEFT,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	//SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_WIDTH,imageHeight);
	//SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_HEIGHT,imageHeight);	//正方形
	
	//设置config图标 左边第二个
	SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_TOP,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_LEFT,MAINPANEL_HEADER_BUTTON_PADDINGSIZE+buttonSize+MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	//SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_WIDTH,imageHeight);
	//SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_HEIGHT,imageHeight);	//正方形
	
	//设置PANEL_MAIN_QUIT  右起第一个
	SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_TOP,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_LEFT,mainPanelWidth-2*buttonSize-2*MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	//SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_WIDTH,imageHeight);
	//SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_HEIGHT,imageHeight);	//正方形		
	
	//设置PANEL_MAIN_QUIT 右起第二个
	SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_TOP,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_LEFT,mainPanelWidth-buttonSize-MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	//SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_WIDTH,imageHeight);
	//SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_HEIGHT,imageHeight);	//正方形
	headerHeight=MAINPANEL_HEADER_BUTTON_PADDINGSIZE+buttonSize;
}

static void adjustBody(int panel)
{
	int bodyHeight=mainPanelHeight-headerHeight-MAINPANLE_FOOTER_HEIGHT;
	int bodyWidth = mainPanelWidth;
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_TOP,headerHeight);
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_LEFT,0);
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_WIDTH,bodyWidth);
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_HEIGHT,bodyHeight);	//正方形
}

static void adustFooter(int panel)
{
	int width=0;
	SetCtrlAttribute(panel,PANEL_MAIN_VERSION_VALUE,ATTR_TOP,mainPanelHeight-MAINPANLE_FOOTER_HEIGHT+10);
	GetCtrlAttribute(panel,PANEL_MAIN_VERSION_VALUE,ATTR_WIDTH,&width);
	SetCtrlAttribute(panel,PANEL_MAIN_VERSION_VALUE,ATTR_LEFT,mainPanelWidth-width);
	
	SetCtrlAttribute(panel,PANEL_MAIN_COPYRIGHT,ATTR_TOP,mainPanelHeight-MAINPANLE_FOOTER_HEIGHT+10);
	SetCtrlAttribute(panel,PANEL_MAIN_COPYRIGHT,ATTR_LEFT,10);
}

void adjustPanelSize(int panel)
{
	int monitor=0,height=0,width=0;
	//主面板
	GetMonitorFromPanel (panel, &monitor);
	GetMonitorAttribute (monitor, ATTR_HEIGHT, &height);
	GetMonitorAttribute (monitor, ATTR_WIDTH, &width);
	mainPanelWidth=width-2*MAINPANEL_PADDING_LEFT;//两边一样长
	mainPanelHeight=height-2*MAINPANEL_PADDING_TOP;
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

	adjustHeader(panel);
	
	adjustBody(panel);

	adustFooter(panel);
}

static void  adjustTstTab(int tabPanel)
{
	int buttonSize=0;
	int panelWidth=0,panelHeight=0;
	int paddingLeft=0,paddingTop=0;
	GetPanelAttribute(tabPanel,ATTR_WIDTH,&panelWidth);
	GetPanelAttribute(tabPanel,ATTR_HEIGHT,&panelHeight);
	GetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_MANUAL,ATTR_WIDTH,&buttonSize);
	paddingLeft = (panelWidth - 3*buttonSize)/4;
	paddingTop = (panelHeight-buttonSize)/2;
	
	//第一个按钮
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_MANUAL,ATTR_TOP,paddingTop);
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_MANUAL,ATTR_LEFT,paddingLeft);
	//第二个按钮
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_AUTO,ATTR_TOP,paddingTop);
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_AUTO,ATTR_LEFT,2*paddingLeft+buttonSize);	
	//第三个按钮
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_INSTR,ATTR_TOP,paddingTop);
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_INSTR,ATTR_LEFT,3*paddingLeft+2*buttonSize);		
}

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
		if(GetPanelHandleFromTabPage(panelMain,PANEL_MAIN_TAB_MAIN,0,&tabAuto) == 0)
		{
			
		}		
		
		SetCtrlAttribute(panelMain,PANEL_MAIN_TAB_MAIN,ATTR_TABS_VISIBLE,0);
		//SetCtrlAttribute(panelMain,ATTR_WIDTH,100);
		SetCtrlAttribute (panelMain, PANEL_MAIN_BACK, ATTR_IMAGE_FILE ,
                              "BACKTOTEST.png");  		
		adjustPanelSize(panelMain); 
		alignToParentPanel(tabconfig,TABPANEL_2_TAB);
		SetCtrlAttribute (tabAuto, TABPANEL_PICTUREBUTTON_AUTO, ATTR_IMAGE_FILE ,
                              "NEWTEST.png");  
		SetCtrlAttribute (tabAuto, TABPANEL_PICTUREBUTTON_MANUAL, ATTR_IMAGE_FILE ,
                              "LOADTEST.png"); 
		SetCtrlAttribute (tabAuto, TABPANEL_PICTUREBUTTON_INSTR, ATTR_IMAGE_FILE ,
                              "TESTRECORD.png"); 
		adjustTstTab(tabAuto);
		

		

	}

	InstallWinMsgCallback (panelMain, 9678,
                                      MainPnlMsgCallback,
                                      VAL_MODE_IN_QUEUE,
                                      NULL, &g_mainHWND);
	EnterAppInit();
	
		
	//登入界面
	//DisplayLoginPanel();
	if(DisplayLoginPanel()==TRUE)
	{
		DisplaySutConfigPanel();
		RunUserInterface ();
	}
	
	
	
	DiscardPanel(panelMain);
	AppDeInit();//这里可能会影响程序关闭     	
	
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
			/*char filePath[MAX_PATHNAME_LEN]={0};
			if(FileSelectPopup (NULL, "*.xml", "*.xml", "请选择要导入的记录文件", VAL_LOAD_BUTTON, 0, 0, 1,1,filePath)!=VAL_NO_FILE_SELECTED)
			{
				WarnShow1(0,filePath);
				DisplayAutoTestPanelWithTestData(getItemList(),getEutList(),GetCollectList(),ENUM_TEST_PANEL_AUTO,filePath);  
			}*/
			//tTestProject project={0};
			if(loadTestProject())
			{
				initLogPath();
				SetCtrlAttribute(panelMain,PANEL_MAIN_BACK,ATTR_VISIBLE,1); 
				//memcpy(&gProject,&project,sizeof(tTestProject));
				HidePanel(panelMain);
				DisplayAutoTestPanelWithTestData(getItemList(),getEutList(),GetCollectList(),ENUM_TEST_PANEL_AUTO,getCurrentProject());
				//printf("%s,%s",gProject.projectName,gProject.projectPath);
				
			}
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
			if(newTestProject())
			{
				initLogPath();
				SetCtrlAttribute(panelMain,PANEL_MAIN_BACK,ATTR_VISIBLE,1);
				HidePanel(panelMain);
				DisplayAutoTestPanel(getItemList(),getEutList(),GetCollectList(),ENUM_TEST_PANEL_AUTO,getCurrentProject());
			}
			break;
	}
	return 0;
}


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
		   SetCtrlVal(panelMain,PANEL_MAIN_COPYRIGHT,COPY_RIGHT);
	       //显示主面板
		   DisplayPanel(panelMain);
		   //显示设备配置面板
		   DisplayEutPanelAsTabPanel(tabEutConfig);
		   //显示策略配置面板
		   DisplayStrategyPanelAsTabPanel(tabStrategyConfig);
		   //显示软件设置面板
		   DisplaySettingPanelAsTabPanel(tabSystemConfig);
		   //显示硬件资源面板
		   DisplayFtpConfigView(tabHardwareConfig);

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
	}else if(*wParam==4)//开启测试
	{
	    initLogPath();
		SetCtrlAttribute(panelMain,PANEL_MAIN_BACK,ATTR_VISIBLE,1); 
		HidePanel(panelMain);
		DisplayAutoTestPanelWithTestData(getItemList(),getEutList(),GetCollectList(),ENUM_TEST_PANEL_AUTO,getCurrentProject());
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
				WarnShow1(0,"请先保存配置!");
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
			//DisplaySutConfigPanel();
			DisplayAutoTestPanelWithTestData(getItemList(),getEutList(),GetCollectList(),ENUM_TEST_PANEL_AUTO,getCurrentProject());
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
			
			

			QuitUserInterface(0); 
			break;
	}
	return 0;
}

extern int DisplayManagerPanel(void); 

int CVICALLBACK PICTUREBUTTON_INSTRUCTIONCALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//DisplayInstructionPanel();
			DisplayManagerPanel();
			break;
	}
	return 0;
}


