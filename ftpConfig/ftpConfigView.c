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
#include "pwctrl.h"
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "ftpConfigPanel.h"
#include"ftpConfigBiz.h"
#include "reportDb.h"
#include "common.h"

static int panelHandle;
tFtpConfig gFtpConfig;

static void showFtpConfigs()
{
	gFtpConfig=getFtpConfig(); 
	SetCtrlVal(panelHandle,PANEL_SERVER,gFtpConfig.server);
	SetCtrlVal(panelHandle,PANEL_USERNAME,gFtpConfig.userName);
	SetCtrlVal(panelHandle,PANEL_PASSWORD,gFtpConfig.password);
	SetCtrlVal(panelHandle,PANEL_CHECKBOX_MODE,gFtpConfig.mode);
	SetCtrlVal(panelHandle,PANEL_REMOTEDIR,gFtpConfig.remoteDir);
}

int DisplayFtpConfigView (int panel)
{
	int password_ctrl=0;
	if ((panelHandle = LoadPanel (panel, "ftpConfigPanel.uir", PANEL)) < 0)
		return -1;
	if(initDb() == FALSE)
	{
		WarnShow1(0,"打开数据库失败");
	}	
	password_ctrl = PasswordCtrl_ConvertFromString (panelHandle,
				PANEL_PASSWORD);
	PasswordCtrl_SetAttribute (panelHandle, password_ctrl,
				ATTR_PASSWORD_MASK_CHARACTER, '*');		
	showFtpConfigs();  
	DisplayPanel (panelHandle);
	return 0;
}


tFtpConfig GetFtpConfig()
{
    return gFtpConfig;
}


int CVICALLBACK onFtpConfigSave (int panel, int control, int event,
								 void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			memset(gFtpConfig.server,strlen(gFtpConfig.server),0); 
			GetCtrlVal(panel,PANEL_SERVER,gFtpConfig.server);
			
			memset(gFtpConfig.userName,strlen(gFtpConfig.userName),0); 
			GetCtrlVal(panel,PANEL_USERNAME,gFtpConfig.userName);
			
			memset(gFtpConfig.password,strlen(gFtpConfig.password),0); 
			GetCtrlVal(panel,PANEL_PASSWORD,gFtpConfig.password);
			
			memset(gFtpConfig.remoteDir,strlen(gFtpConfig.remoteDir),0); 
			GetCtrlVal(panel,PANEL_REMOTEDIR,gFtpConfig.remoteDir);		
			
			GetCtrlVal(panel,PANEL_CHECKBOX_MODE,&gFtpConfig.mode);
			
			saveFtpConfig(gFtpConfig);
			
			break;
	}
	return 0;
}
