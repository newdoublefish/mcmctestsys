/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：common.c
 // 文件功能描述:可以共用的面板 
 //
 // 
 // 创建标识：曾衍仁 20141114
 //
 // 修改标识：曾衍仁  20150814
 // 修改描述：
 //-------------------------------------------------------------------------*/
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "WarnPanel.h"
#include "settingConfig.h"

void WarnShow(char *meesage)
{
	BOOL ret;
    int ctrl=0, quit=0;
	int testPanel;
	//InstallPopup (panelHandle);
	testPanel=LoadPanel (0, "WarnPanel.uir", PANEL_WARN);  
	SetCtrlVal(testPanel,PANEL_WARN_TEXTMSG,meesage);
	InstallPopup(testPanel);
    while (!quit) {
        GetUserEvent (0, &testPanel, &ctrl);
        if (ctrl == PANEL_WARN_COMMANDBUTTON) {
             DiscardPanel (testPanel);
             quit=1;
			 ret=FALSE;			
         }
    }
}


void WarnShow1(int panel,char *meesage)
{
	BOOL ret;
    int ctrl=0, quit=0;
	int testPanel;
	//InstallPopup (panelHandle);
	testPanel=LoadPanel (panel, "WarnPanel.uir", PANEL_WARN);  
	SetCtrlVal(testPanel,PANEL_WARN_TEXTMSG,meesage);
	//InstallPopup(testPanel);
	DisplayPanel(testPanel);
    while (!quit) {
        GetUserEvent (0, &testPanel, &ctrl);
        if (ctrl == PANEL_WARN_COMMANDBUTTON) {
             DiscardPanel (testPanel);
             quit=1;
			 ret=FALSE;			
         }
    }
}


int CVICALLBACK TipTimerCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	double interVal;
	int *data;
	char temp[30]={0};
	SETTING s;  
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			SETTING s=GetSetting(); 
			data=(int *)callbackData;
			*data+=1;
			sprintf(temp,"%d",s.mentionAutoCloseTime-*data);
			SetCtrlVal(panel,MANUALTIPS_TIMERSTR,temp); 
			s=GetSetting();
			if(*data>=s.mentionAutoCloseTime)
				 DiscardPanel (panel); 
			break;
	}
	return 0;
}


void ShowManualTip(int panel,char *title,char *tip)
{
    int ctrl=0, quit=0,timeOut=0;
	int testPanel;
	double interVal;
	SETTING s=GetSetting();
	//InstallPopup (panelHandle);
	testPanel=LoadPanel (panel, "WarnPanel.uir", MANUALTIPS);  
	SetCtrlVal(testPanel,MANUALTIPS_GRROUPNAME,title);
	SetCtrlVal(testPanel,MANUALTIPS_TEXTBOX,tip); 
	
	if(s.mentionAutoCloseTime>0)
	   InstallCtrlCallback (testPanel,MANUALTIPS_TIMER,TipTimerCallback,&timeOut);
	//InstallPopup(testPanel);
	InstallPopup(testPanel);
    while (!quit) {
        GetUserEvent (0, &testPanel, &ctrl);
		if(timeOut>=s.mentionAutoCloseTime && s.mentionAutoCloseTime>0)
		{	
			quit=1;
		}
        if (ctrl == MANUALTIPS_COMMANDBUTTON) {
             DiscardPanel (testPanel); 
             quit=1;
         }
    }
	
	//DiscardPanel (testPanel);
	
	
}

BOOL GetWarnPanelRet(char *meesage)
{
	BOOL ret=FALSE;
    int ctrl=0, quit=0;
	int testPanel;
	//InstallPopup (panelHandle);
	testPanel=LoadPanel (0, "WarnPanel.uir", PANEL2);  
	SetCtrlVal(testPanel,PANEL2_TEXTMSG,meesage);
	InstallPopup(testPanel);
    while (!quit) {
        GetUserEvent (0, &testPanel, &ctrl);
        if (ctrl == PANEL2_SURE) {
             DiscardPanel (testPanel);
             quit=1;
			 ret=TRUE;			
         }
        if (ctrl == PANEL2_CANCEL) {
             DiscardPanel (testPanel);
             quit=1;
			 ret=FALSE;			
         }
		
    }
	return ret;
}

BOOL GetConfigWarnPanelRet(int panel,char *title,char *lableMessage,char *lableNegative,char *lablePositive)
{
	BOOL ret=FALSE;
    int ctrl=0, quit=0;
	int testPanel;
	//InstallPopup (panelHandle);
	testPanel=LoadPanel (0, "WarnPanel.uir", PANEL2);  
	//setPanelAttribute(testPanel,ATTR_TITLE,title);
	SetPanelAttribute(testPanel,ATTR_TITLE,title);
	SetCtrlVal(testPanel,PANEL2_TEXTMSG,lableMessage);
	SetCtrlAttribute(testPanel,PANEL2_CANCEL,ATTR_LABEL_TEXT,lableNegative);
	SetCtrlAttribute(testPanel,PANEL2_SURE,ATTR_LABEL_TEXT,lablePositive);  
	
	InstallPopup(testPanel);
    while (!quit) {
        GetUserEvent (0, &testPanel, &ctrl);
        if (ctrl == PANEL2_SURE) {
             DiscardPanel (testPanel);
             quit=1;
			 ret=TRUE;			
         }
        if (ctrl == PANEL2_CANCEL) {
             DiscardPanel (testPanel);
             quit=1;
			 ret=FALSE;			
         }
		
    }
	return ret;
}

BOOL GetConfigWarnPanelRetWithQuitFlag(int panel,char *title,char *lableMessage,char *lableNegative,char *lablePositive,int *flag)
{
	BOOL ret=FALSE;
    int ctrl=0, quit=0;
	int testPanel;
	//InstallPopup (panelHandle);
	testPanel=LoadPanel (0, "WarnPanel.uir", PANEL2);  
	//setPanelAttribute(testPanel,ATTR_TITLE,title);
	SetPanelAttribute(testPanel,ATTR_TITLE,title);
	SetCtrlVal(testPanel,PANEL2_TEXTMSG,lableMessage);
	SetCtrlAttribute(testPanel,PANEL2_CANCEL,ATTR_LABEL_TEXT,lableNegative);
	SetCtrlAttribute(testPanel,PANEL2_SURE,ATTR_LABEL_TEXT,lablePositive);  
	
	InstallPopup(testPanel);
    while (!quit) {
        GetUserEvent (0, &testPanel, &ctrl);
        if (ctrl == PANEL2_SURE) {
             DiscardPanel (testPanel);
             quit=1;
			 ret=TRUE;			
         }
        if (ctrl == PANEL2_CANCEL) {
             DiscardPanel (testPanel);
             quit=1;
			 ret=FALSE;			
         }
		
		if(*flag!=2)
		{	
		   quit=1;
		}
		
    }
	return ret;
}


int displaySlideProgressWithTextBox(char *title)
{
	int handle=0;
   	handle=LoadPanel (0, "WarnPanel.uir", PROGRESS_2); 
	//SetCtrlVal(handle,PROGRESS_RINGSLIDE,title);  
	SetCtrlAttribute(handle,PROGRESS_2_RINGSLIDE,ATTR_LABEL_TEXT,title);
	DisplayPanel(handle);  
	return handle;
}

void slideProgressShowWithTextBox(int panelHandle,int perc,char *description,double delayTime)
{
	
	
	if(perc>0)
	{
	   char temp[20]={0};	
	   SetCtrlAttribute(panelHandle,PROGRESS_2_RINGSLIDE,ATTR_CTRL_INDEX,perc);
	   Fmt(temp,"%d%%",perc);
	   SetCtrlVal(panelHandle,PROGRESS_2_TEXTMSG,temp);
		   
	}   

	if(description!=NULL)
	{
	  SetCtrlVal(panelHandle,PROGRESS_2_TEXTBOX,description);
	}  
	if(delayTime>0)
		Delay(delayTime);
}

void slideProgressShow(int panelHandle,int perc,char *message,double delayTime)
{
	SetCtrlAttribute(panelHandle,PROGRESS_RINGSLIDE,ATTR_CTRL_INDEX,perc);
	SetCtrlVal(panelHandle,PROGRESS_TEXTMSG,message);
	if(delayTime>0)
		Delay(delayTime);
}

int displaySlideProgress(char *title)
{
	int handle=0;
   	handle=LoadPanel (0, "WarnPanel.uir", PROGRESS); 
	//SetCtrlVal(handle,PROGRESS_RINGSLIDE,title);  
	SetCtrlAttribute(handle,PROGRESS_RINGSLIDE,ATTR_LABEL_TEXT,title);
	DisplayPanel(handle);  
	return handle;
}

void refreshSlideProgress(int panel,int perc)
{
   SetCtrlAttribute(panel,PROGRESS_RINGSLIDE,ATTR_CTRL_INDEX,perc);
   
}


void disposeSlideProgress(int handle)
{
	 DiscardPanel(handle);  
}

void disposeSlideProgressWithButton(int handle)
{
	 
	 int ctrl=0, quit=0;
	 SetCtrlAttribute(handle,PROGRESS_2_COMMANDBUTTON,ATTR_VISIBLE,1); 
	 while(!quit)
	 {  GetUserEvent (0, &handle, &ctrl);
        if(ctrl==PROGRESS_2_COMMANDBUTTON)	
		{
		   DiscardPanel(handle);  
		   quit=1;
		}	
	 }	 
	 
}


BOOL getUserCustiomName(char *fileName)
{
    int flag=1; 
	int panelHandle;
	int ctrl=0;
	if ((panelHandle = LoadPanel (0, "WarnPanel.uir", NAMEPANEL)) < 0)
		return -1;
	InstallPopup(panelHandle);
	while(flag)
	{
	    GetUserEvent(0,&panelHandle,&ctrl);
		if(ctrl==NAMEPANEL_FILENAMEBT)
		{
		    flag=0;
			GetCtrlVal(panelHandle,NAMEPANEL_FILENAMESTRING,fileName);
		}
	
	}
	DiscardPanel(panelHandle);
	return 1;
	 
}





