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
	//����tstͼ�� ��ߵ�һ��
	SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_TOP,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_LEFT,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	//SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_WIDTH,imageHeight);
	//SetCtrlAttribute(panel,PANEL_MAIN_TST,ATTR_HEIGHT,imageHeight);	//������
	
	//����configͼ�� ��ߵڶ���
	SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_TOP,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_LEFT,MAINPANEL_HEADER_BUTTON_PADDINGSIZE+buttonSize+MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	//SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_WIDTH,imageHeight);
	//SetCtrlAttribute(panel,PANEL_MAIN_CONF,ATTR_HEIGHT,imageHeight);	//������
	
	//����PANEL_MAIN_QUIT  �����һ��
	SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_TOP,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_LEFT,mainPanelWidth-2*buttonSize-2*MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	//SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_WIDTH,imageHeight);
	//SetCtrlAttribute(panel,PANEL_MAIN_BACK,ATTR_HEIGHT,imageHeight);	//������		
	
	//����PANEL_MAIN_QUIT ����ڶ���
	SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_TOP,MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_LEFT,mainPanelWidth-buttonSize-MAINPANEL_HEADER_BUTTON_PADDINGSIZE);
	//SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_WIDTH,imageHeight);
	//SetCtrlAttribute(panel,PANEL_MAIN_QUIT,ATTR_HEIGHT,imageHeight);	//������
	headerHeight=MAINPANEL_HEADER_BUTTON_PADDINGSIZE+buttonSize;
}

static void adjustBody(int panel)
{
	int bodyHeight=mainPanelHeight-headerHeight-MAINPANLE_FOOTER_HEIGHT;
	int bodyWidth = mainPanelWidth;
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_TOP,headerHeight);
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_LEFT,0);
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_WIDTH,bodyWidth);
	SetCtrlAttribute(panel,PANEL_MAIN_TAB_MAIN,ATTR_HEIGHT,bodyHeight);	//������
}

static void adustFooter(int panel)
{
	int width=0;
	SetCtrlAttribute(panel,PANEL_MAIN_VERSION_VALUE,ATTR_TOP,mainPanelHeight-MAINPANLE_FOOTER_HEIGHT+10);
	GetCtrlAttribute(panel,PANEL_MAIN_VERSION_VALUE,ATTR_WIDTH,&width);
	SetCtrlAttribute(panel,PANEL_MAIN_VERSION_VALUE,ATTR_LEFT,mainPanelWidth-width);	
}

void adjustPanelSize(int panel)
{
	int monitor=0,height=0,width=0;
	//�����
	GetMonitorFromPanel (panel, &monitor);
	GetMonitorAttribute (monitor, ATTR_HEIGHT, &height);
	GetMonitorAttribute (monitor, ATTR_WIDTH, &width);
	mainPanelWidth=width-2*MAINPANEL_PADDING_LEFT;//����һ����
	mainPanelHeight=height-2*MAINPANEL_PADDING_TOP;
	//����������С
	SetPanelAttribute(panel,ATTR_WIDTH,mainPanelWidth);
	SetPanelAttribute(panel,ATTR_HEIGHT,mainPanelHeight);
	SetPanelAttribute(panel,ATTR_TOP,VAL_AUTO_CENTER);//�Զ�����
	SetPanelAttribute(panel,ATTR_LEFT,VAL_AUTO_CENTER);
	//���ñ���ɫ��С
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
	GetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_STATE,ATTR_WIDTH,&buttonSize);
	paddingLeft = (panelWidth - 3*buttonSize)/4;
	paddingTop = (panelHeight-buttonSize)/2;
	
	//��һ����ť
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_STATE,ATTR_TOP,paddingTop);
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_STATE,ATTR_LEFT,paddingLeft);
	//�ڶ�����ť
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_AUTO,ATTR_TOP,paddingTop);
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_AUTO,ATTR_LEFT,2*paddingLeft+buttonSize);	
	//��������ť
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_INSTR,ATTR_TOP,paddingTop);
	SetCtrlAttribute(tabPanel,TABPANEL_PICTUREBUTTON_INSTR,ATTR_LEFT,3*paddingLeft+2*buttonSize);		
}

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
		if(GetPanelHandleFromTabPage(panelMain,PANEL_MAIN_TAB_MAIN,0,&tabAuto) == 0)
		{
			
		}		
		
		SetCtrlAttribute(panelMain,PANEL_MAIN_TAB_MAIN,ATTR_TABS_VISIBLE,0);
		//SetCtrlAttribute(panelMain,ATTR_WIDTH,100);
		adjustPanelSize(panelMain); 
		alignToParentPanel(tabconfig,TABPANEL_2_TAB);
		adjustTstTab(tabAuto);
		

		

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
		   char version[50]={0};
		   sprintf(version,"�汾��:%s",getVersion());
		   SetCtrlVal(panelMain,PANEL_MAIN_VERSION_VALUE,version);
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
			//DisplayInstructionPanel();
			break;
	}
	return 0;
}


