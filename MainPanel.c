#include "relayPanel.h"
#include <ansi_c.h>
#include "pwctrl.h"   //����ؼ�
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

static int panelMain = 0;	//�����

static int tabAuto =0 ;//�Զ�����tab

static int tabconfig;	//�������TAB
static int tabEutConfig;//EUT����
static int tabStrategyConfig;//��������tab
static int tabHardwareConfig;  //Ӳ������tab
static int tabSystemConfig;  //ϵͳ����tab



//��Ϣ����
int g_mainHWND = 0;   

int CVICALLBACK MainPnlMsgCallback(int panelHandle, int message,
                                      unsigned int* wParam,
                                      unsigned int* lParam,
                                      void* callbackData);


int main (int argc, char *argv[])
{
	if (InitCVIRTE (0, argv, 0) == 0)
		return -1;	/* out of memory */
	
	if ((panelMain = LoadPanel (0, "MainPanel.uir", PANEL_MAIN)) >= 0)		   //���������
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
	//�������
	//DisplayLoginPanel();
	DisplaySutConfigPanel();
	
	RunUserInterface ();
	DiscardPanel(panelMain);
	
	return 0;
}

//////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------���������
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
	
	
	if(*wParam==1) //�������������ɹ�
	{
		
		
	   //����������ȡ��ϣ���ȡ��������
		if(testInit(GetSeletedSut())<0)//��ʼ������  
		{
		   DisplaySutConfigPanel();  
		}else{	
			
		   //��ʾ�汾��
		   SetCtrlVal(panelMain,PANEL_MAIN_VERSION_VALUE,getVersion());
	       //��ʾ�����
		   DisplayPanel(panelMain);
		   //��ʾ�豸�������
		   DisplayEutPanelAsTabPanel(tabEutConfig);
		   //��ʾ�����������
		   DisplayStrategyPanelAsTabPanel(tabStrategyConfig);
		   //��ʾ����������
		   DisplaySettingPanelAsTabPanel(tabSystemConfig);
		   //��ʾӲ����Դ���
		   DisplayHardwarePanelAsTabPanel(tabHardwareConfig);

		}
	}else if(*wParam==2)  //����ɹ�,����ϵͳѡ��
	{
	    LOG_EVENT(LOG_INFO,"login success");
		DisplaySutConfigPanel(); 
	}else if(*wParam==3) //�˳��Զ����Խ���
	{
	    DisplayPanel(panelMain);
	}else if(*wParam==9)
	{
	    AppDeInit();//������ܻ�Ӱ�����ر�
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
				WarnShow("���ȱ�������!");
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
			AppDeInit();//������ܻ�Ӱ�����ر�
			

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


