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
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "WarnPanel.h"
#include "settingConfig.h"

void alignToParentPanel(int parent,int ctrl)
{
	int parentTabWidth,parentTabHeight,width,height;
	GetPanelAttribute(parent,ATTR_WIDTH,&parentTabWidth);
	GetPanelAttribute(parent,ATTR_HEIGHT,&parentTabHeight); 
	SetCtrlAttribute(parent,ctrl,ATTR_WIDTH,parentTabWidth);
	SetCtrlAttribute(parent,ctrl,ATTR_HEIGHT,parentTabHeight);	
}


int CVICALLBACK onWarn1CtrlCallBack (int panel, int control, int event,
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

void WarnShow1(int panel,char *meesage)
{
	int pnl;
	pnl=LoadPanel (panel, "WarnPanel.uir", PANEL_WARN);  
	SetCtrlVal(pnl,PANEL_WARN_TEXTMSG,meesage);
	DisplayPanel(pnl);
	InstallCtrlCallback(pnl,PANEL_WARN_COMMANDBUTTON,onWarn1CtrlCallBack,NULL);
	RunUserInterface();
	DiscardPanel(pnl);
}


int CVICALLBACK tipTimerCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_TIMER_TICK:
			
			char temp[30]={0};			
			SETTING s=GetSetting(); 
			int *data=(int *)callbackData;
			*data+=1;
			sprintf(temp,"%d",s.mentionAutoCloseTime-*data);
			SetCtrlVal(panel,MANUALTIPS_TIMERSTR,temp); 
			if(*data>=s.mentionAutoCloseTime)
				 QuitUserInterface(1); 
			break;
		case EVENT_COMMIT:
			BOOL *ret = (BOOL *)callbackData; 
			if(control==MANUALTIPS_SURE)
			{	
				*ret=TRUE;
			}else if(control==MANUALTIPS_CANCEL){
				*ret=FALSE;			
			}
			QuitUserInterface(1);
			break;
	}
	return 0;
}


BOOL showTips(int panel,char *title,char *tip)
{
	BOOL ret=1;
    int timeOut=0;
	int testPanel;
	SETTING s=GetSetting();
	//InstallPopup (panelHandle);
	testPanel=LoadPanel (panel, "WarnPanel.uir", MANUALTIPS);  
	SetCtrlVal(testPanel,MANUALTIPS_GRROUPNAME,title);
	SetCtrlVal(testPanel,MANUALTIPS_TEXTBOX,tip); 
	
	if(s.mentionAutoCloseTime>0)
	   InstallCtrlCallback (testPanel,MANUALTIPS_TIMER,tipTimerCallback,&timeOut);
	InstallCtrlCallback(testPanel,MANUALTIPS_SURE,tipTimerCallback,&ret);
	InstallCtrlCallback(testPanel,MANUALTIPS_CANCEL,tipTimerCallback,&ret);
	DisplayPanel(testPanel);
	RunUserInterface();
	DiscardPanel(testPanel);
	//printf("%d\n",ret);
	return ret;
}




int CVICALLBACK AlertDialogCtrlCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			BOOL *ret=(BOOL *)callbackData;
        	if (control == PANEL2_SURE) {
				 *ret=TRUE;			
         	}else if (control == PANEL2_CANCEL) {
			 	 *ret=FALSE;			
        	 }			
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}

BOOL AlertDialogWithRet(int panel,char *title,char *lableMessage,char *lableNegative,char *lablePositive)
{
	BOOL ret=FALSE;
    int ctrl=0, quit=0;
	int testPanel;
	testPanel=LoadPanel (0, "WarnPanel.uir", PANEL2);  
	SetPanelAttribute(testPanel,ATTR_TITLE,title);
	SetCtrlVal(testPanel,PANEL2_TEXTMSG,lableMessage);
	SetCtrlAttribute(testPanel,PANEL2_CANCEL,ATTR_LABEL_TEXT,lableNegative);
	SetCtrlAttribute(testPanel,PANEL2_SURE,ATTR_LABEL_TEXT,lablePositive);
	InstallCtrlCallback(testPanel,PANEL2_SURE,AlertDialogCtrlCallBack,&ret);
	InstallCtrlCallback(testPanel,PANEL2_CANCEL,AlertDialogCtrlCallBack,&ret);
	DisplayPanel(testPanel);
	RunUserInterface();
	DiscardPanel(testPanel);
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





