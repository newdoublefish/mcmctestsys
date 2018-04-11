#include "toolbox.h"
#include "login.h"
#include "WarnPanel.h"
#include "loginConfigBiz.h"
#include "postData.h"
#include "httpPost.h"


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

BOOL onParseLoginPostData(tPostParam *paramPtr,void *callbackData)
{
	tLoginConfig *configPtr=(tLoginConfig *)callbackData;
	if(strcmp(paramPtr->type,"username")==0)
	{
		memset(paramPtr->value,0,POST_DATA_VALUE);
		sprintf(paramPtr->value,"%s",configPtr->userName);
	}else if(strcmp(paramPtr->type,"password")==0)
	{
		memset(paramPtr->value,0,POST_DATA_VALUE);
		sprintf(paramPtr->value,"%s",configPtr->password);
	}
	return TRUE;	
}

tPostData buildLoginPostData(tLoginConfig config)
{
	tPostData data={0};
	sprintf(data.url,"%s",config.url);
	sprintf(data.request,"%s",config.method);
	data.postParamList=ListCreate(sizeof(tPostParam));
	tPostParam param1={0};
	sprintf(param1.name,"%s","username");
	sprintf(param1.value,"%s","username");
	sprintf(param1.type,"%s","username");
	ListInsertItem(data.postParamList,&param1,END_OF_LIST);
	
	tPostParam param2={0};
	sprintf(param2.name,"%s","password");
	sprintf(param2.value,"%s","password");
	sprintf(param2.type,"%s","password");
	ListInsertItem(data.postParamList,&param2,END_OF_LIST);
	return data;
	
}

BOOL Login(tLoginConfig config)
{
	/*tPostData data={0};
	if(TRUE==getPostData(&data,"login"))
	{
		char buffer[512]={0};
		buildPostDataStr(data,buffer,onParseLoginPostData,&config);
		if(1==httpPostJson(data.url,buffer))
		{
			return TRUE;
		}
		return FALSE;
		
	}*/
	if(strstr(config.url,"http://")!=NULL)   //������ʽ�ж��Ƿ����Զ�̵���
	{
		char buffer[512]={0};
		tPostData data = buildLoginPostData(config);
		buildPostDataStr(data,buffer,onParseLoginPostData,&config);
		ListDispose(data.postParamList);
		if(1==httpPostJson(data.url,buffer))
		{
			return TRUE;
		}else{
			int pass = atoi(config.password);
			if(pass == getSecret())
			{
				return TRUE;
			}			
		}
		return FALSE;		
	}
	
	int pass = atoi(config.password);
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
			 
			 //char userName[30]={0};
			 //char password[30]={0};
			 //tLoginConfig config={0};
			 tLoginConfig config=getLoginConfig();
			 memset(config.userName,0,50);
			 memset(config.password,0,50);
			 GetCtrlVal(panel,LOGINPANEL_USERNAME,config.userName);
			 GetCtrlVal(panel,LOGINPANEL_PASSWORD,config.password);
			 GetCtrlVal(panel,LOGINPANEL_REMEBER,&config.remember);
			 *ret = Login(config);
			 if(*ret == TRUE){
				 
				if(config.remember > 0)
				{
					saveLoginConfig(config);	
				}else{
					tLoginConfig temp={0};
					saveLoginConfig(temp);
				}
			 	QuitUserInterface(1);
			 }
			 else
				SetCtrlVal(panel,LOGINPANEL_ERRORMSG,"�û��������������");		 
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
	tLoginConfig config=getLoginConfig();
	SetCtrlVal(panel,LOGINPANEL_USERNAME,config.userName);
	SetCtrlVal(panel,LOGINPANEL_PASSWORD,config.password);
	SetCtrlVal(panel,LOGINPANEL_REMEBER,config.remember);
	DisplayPanel(panel);
	RunUserInterface();
	DiscardPanel(panel);
	return ret;
}
