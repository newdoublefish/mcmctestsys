#include "toolbox.h"
#include "login.h"
#include "WarnPanel.h"


int CVICALLBACK LoginPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			//if(checkScanResult(panelHandle)==TRUE) 
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}

static unsigned int getSecret()
{
	unsigned int year, month, day, hour, min, sec, weekDay;
	CVIAbsoluteTime absTime;
    GetCurrentCVIAbsoluteTime(&absTime);
    CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
                &min, &sec, 0, &weekDay);
	return ((year*3+13-7) + (month*342))*44;
}

BOOL Login(char *userName,char *password)
{
	int pass = atoi(password);
	if(pass == getSecret())
	{
		return TRUE;
	}
	return FALSE;	
}

int CVICALLBACK onLoginCtrlCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			 BOOL *ret = (BOOL *)callbackData;
			 
			 char userName[30]={0};
			 char password[30]={0};
			 GetCtrlVal(panel,LOGINPANEL_USERNAME,userName);
			 GetCtrlVal(panel,LOGINPANEL_PASSWORD,password);
			 *ret = Login(userName,password);
			 if(*ret == TRUE)
			 	QuitUserInterface(1);
			 else
				SetCtrlVal(panel,LOGINPANEL_ERRORMSG,"用户名或者密码错误");		 
		     break;
	}
	return 0;
}

BOOL DisplayLoginPanel(void)
{
	BOOL ret=FALSE;
	int panel = LoadPanel(0,"WarnPanel.uir",LOGINPANEL);
	InstallCtrlCallback(panel,LOGINPANEL_COMMANDBUTTON,onLoginCtrlCallBack,&ret); 
	InstallPanelCallback(panel,LoginPanelCallback,NULL); 
	DisplayPanel(panel);
	RunUserInterface();
	DiscardPanel(panel);
	return ret;
}
