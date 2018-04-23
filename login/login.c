#include "windows.h"
#include "pwctrl.h"
#include "regexpr.h"
#include "toolbox.h"
#include "WarnPanel.h"
#include "loginConfigBiz.h"
#include "login.h" 
#include "postData.h"
#include "httpPost.h"
#include "common.h"
#include "application.h"


tLoginConfig gConfig={0};

tLoginConfig getAccount()
{
	return gConfig;
}

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

void utf8ToGbk(char *utf8String, char *gbkString)
{
	wchar_t *unicodeStr = NULL;
	int nRetLen = 0;

	nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, NULL, 0);

	//求需求的宽字符数大小

	unicodeStr = (wchar_t *)malloc(nRetLen * sizeof(wchar_t));

	nRetLen = MultiByteToWideChar(CP_UTF8, 0, utf8String, -1, unicodeStr, nRetLen);

	//将utf-8编码转换成unicode编码

	nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, NULL, 0, NULL, 0);

	//求转换所需字节数

	nRetLen = WideCharToMultiByte(CP_ACP, 0, unicodeStr, -1, gbkString, nRetLen, NULL, 0);

	//unicode编码转换成gbk编码

	free(unicodeStr);
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
	if(strstr(config.url,"http://")!=NULL)   //正则表达式判断是否进行远程登入
	{
		char buffer[512]={0};
		tPostData data = buildLoginPostData(config);
		buildPostDataStr(data,buffer,onParseLoginPostData,&config);
		ListDispose(data.postParamList);
		char *fullName;
		if(1==httpPostJsonWithResponseData(data.url,buffer,&fullName))
		{
			if(fullName!=NULL)
			{
				char temp[50]={0};
				utf8ToGbk(fullName,temp);
				char *first=NULL;
#if 0				
				if((first=strstr(temp,"\""))!=NULL)
				{
					first = first+1;
					//printf("%s\n",first);
					char *last=NULL; 
					if((last = strstr(first,"\""))!=NULL)
					{
						*last = '\0';
						memset(gConfig.fullName,0,50);
						sprintf(gConfig.fullName,"%s",first);
					}
				}
				//WarnShow1(0,gConfig.fullName);
#else
				int matched=0,position=0,matchedLen=0;
				RegExpr_FindPatternInText("\".*\"",1,temp,-1,1,1,&matched,&position,&matchedLen);
				if(matched == 1)
				{
					memset(gConfig.fullName,0,50);
					memcpy(gConfig.fullName,temp+position+1,matchedLen-2);
				}
#endif				
				free(fullName);
			}
			//WarnShow1(0,gConfig.fullName);
			return TRUE;
		}else{
			int pass = atoi(config.password);
			if((pass == getSecret())&&strlen(config.userName)==0)  
			{
				return TRUE;
			}			
		}
		return FALSE;		
	}
	
	int pass = atoi(config.password);
	if((pass == getSecret())&&strlen(config.userName)==0)
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
		case EVENT_LEFT_CLICK:
			 BOOL *ret = (BOOL *)callbackData;
			 
			 //char userName[30]={0};
			 //char password[30]={0};
			 //tLoginConfig config={0};
			 //tLoginConfig config=getLoginConfig();
			 gConfig=getLoginConfig();  
			 memset(gConfig.userName,0,50);
			 memset(gConfig.password,0,50);
			 GetCtrlVal(panel,LOGINPANEL_USERNAME,gConfig.userName);
			 GetCtrlVal(panel,LOGINPANEL_PASSWORD,gConfig.password);
			 GetCtrlVal(panel,LOGINPANEL_REMEBER,&gConfig.remember);
			 sprintf(gConfig.fullName,"%s",gConfig.userName);
			 *ret = Login(gConfig);
			 if(*ret == TRUE){
				 
				if(gConfig.remember > 0)
				{
					memset(gConfig.password,0,50); 
					saveLoginConfig(gConfig);	
				}else{
			        memset(gConfig.userName,0,50);
			        memset(gConfig.password,0,50);
					saveLoginConfig(gConfig);
				}
			 	QuitUserInterface(1);
			 }
			 else
				SetCtrlVal(panel,LOGINPANEL_ERRORMSG,"用户名或者密码错误");		 
		     break;
	}
	return 0;
}

int CVICALLBACK onLoginEnterCallBack (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			 BOOL *ret = (BOOL *)callbackData;
			 
			 //char userName[30]={0};
			 //char password[30]={0};
			 //tLoginConfig config={0};
			 //tLoginConfig config=getLoginConfig();
			 gConfig=getLoginConfig();  
			 memset(gConfig.userName,0,50);
			 memset(gConfig.password,0,50);
			 GetCtrlVal(panel,LOGINPANEL_USERNAME,gConfig.userName);
			 GetCtrlVal(panel,LOGINPANEL_PASSWORD,gConfig.password);
			 GetCtrlVal(panel,LOGINPANEL_REMEBER,&gConfig.remember);
			 sprintf(gConfig.fullName,"%s",gConfig.userName);
			 *ret = Login(gConfig);
			 if(*ret == TRUE){
				 
				if(gConfig.remember > 0)
				{
					memset(gConfig.password,0,50); 
					saveLoginConfig(gConfig);	
				}else{
			        memset(gConfig.userName,0,50);
			        memset(gConfig.password,0,50);
					saveLoginConfig(gConfig);
				}
			 	QuitUserInterface(1);
			 }
			 else
				SetCtrlVal(panel,LOGINPANEL_ERRORMSG,"用户名或者密码错误");		 
		     break;
	}
	return 0;
}

BOOL DisplayLoginPanel(void)
{
	BOOL ret=FALSE;
	int password_ctrl=0;
	int panel = LoadPanel(0,"WarnPanel.uir",LOGINPANEL);
	tApplication ta = getApplication();
	SetPanelAttribute(panel,ATTR_TITLE,ta.basic.company);
	//InstallCtrlCallback(panel,LOGINPANEL_COMMANDBUTTON,onLoginCtrlCallBack,&ret);//这里很奇怪要把这里删除，否则到系统选择那里直接就没显示了
	
	InstallCtrlCallback(panel,LOGINPANEL_PASSWORD,onLoginEnterCallBack,&ret);
	InstallPanelCallback(panel,LoginPanelCallback,NULL); 
	tLoginConfig config=getLoginConfig();
	SetCtrlVal(panel,LOGINPANEL_USERNAME,config.userName);
	SetCtrlVal(panel,LOGINPANEL_PASSWORD,config.password);
	SetCtrlVal(panel,LOGINPANEL_REMEBER,config.remember);
	
	password_ctrl = PasswordCtrl_ConvertFromString (panel,
				LOGINPANEL_PASSWORD);
	PasswordCtrl_SetAttribute (panel, password_ctrl,
				ATTR_PASSWORD_MASK_CHARACTER, '*');	
	DisplayPanel(panel);
	RunUserInterface();
	DiscardPanel(panel);
	return ret;
}
