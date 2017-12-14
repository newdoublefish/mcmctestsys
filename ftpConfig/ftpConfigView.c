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
#include <cvirte.h>		
#include <userint.h>
#include "ftpConfigPanel.h"
#include"ftpConfigBiz.h"

static int panelHandle;
tFtpConfig gFtpConfig;

static void showFtpConfigs()
{
	gFtpConfig=getFtpConfig(); 
	SetCtrlVal(panelHandle,PANEL_SERVER,gFtpConfig.server);
	SetCtrlVal(panelHandle,PANEL_USERNAME,gFtpConfig.userName);
	SetCtrlVal(panelHandle,PANEL_PASSWORD,gFtpConfig.password);
	SetCtrlVal(panelHandle,PANEL_CHECKBOX_MODE,gFtpConfig.mode);
}

int DisplayFtpConfigView (int panel)
{
	if ((panelHandle = LoadPanel (panel, "ftpConfigPanel.uir", PANEL)) < 0)
		return -1;
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
			
			GetCtrlVal(panel,PANEL_CHECKBOX_MODE,&gFtpConfig.mode);
			
			saveFtpConfig(gFtpConfig);
			
			break;
	}
	return 0;
}
