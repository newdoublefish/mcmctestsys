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
#include <userint.h>  
#include <stdlib.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include "MainPanel.h"
#include "SettingPanel.h"
#include "settingConfig.h"
#include "log.h"

static int settingPanel=0;
static BOOL settingInit=FALSE; 

SETTING gSetting; 

HRESULT  ShowSettingInfo(SETTING s)
{
   SetCtrlVal(settingPanel,SET_PANEL_CBTIPS,s.showTipsWhenError);
   SetCtrlVal(settingPanel,SET_PANEL_CBVOICE,s.voice);
   SetCtrlVal(settingPanel,SET_PANEL_SAVEDIR,s.saveDir);  
   SetCtrlVal(settingPanel,SET_PANEL_CBLOG,s.saveLog);
   SetCtrlVal(settingPanel,SET_PANEL_SIMUTESTCB,s.simuTest); 
   SetCtrlVal(settingPanel,SET_PANEL_NUMERIC,s.maxComunPerRowInAutoTestPanel);
   SetCtrlVal(settingPanel,SET_PANEL_CBCOLLECTMENTION,s.collectTestMention); 
   SetCtrlVal(settingPanel,SET_PANEL_AUTOSAVE,s.autoSave); 
   SetCtrlVal(settingPanel,SET_PANEL_RELAY_ENABLE,s.relayEnable); 
   SetCtrlVal(settingPanel,SET_PANEL_RING_CLOSETIME,s.mentionAutoCloseTime);
   SetCtrlVal(settingPanel,SET_PANEL_RETESTNUM,s.reTestCnt);
   //printf("sssss:%d\n",s.reportInfoCustom);
   SetCtrlVal(settingPanel,SET_PANEL_RPINFOCB,s.reportInfoCustom); 
   int i=0;
   GetIndexFromValue(settingPanel,SET_PANEL_RING,&i,s.methodToGetdirFile); 
   SetCtrlIndex(settingPanel,SET_PANEL_RING,i);//���ں� 
   return 0;
}

HRESULT DisplaySettingPanelAsTabPanel(int panel)
{

	settingPanel = LoadPanel (panel, "SettingPanel.uir", SET_PANEL); 
	DisplayPanel(settingPanel);

	gSetting=getSetting();
	ShowSettingInfo(gSetting);
	return 0;
}



int CVICALLBACK SETTINGSAVECALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char temp[100]={0};  
	switch (event)
	{
		case EVENT_COMMIT:
			int flag=0;
			GetCtrlVal(settingPanel,SET_PANEL_CBTIPS,&flag);
			gSetting.showTipsWhenError=flag;
			GetCtrlVal(settingPanel,SET_PANEL_CBVOICE,&flag);
		    gSetting.voice=flag;
			GetCtrlVal(settingPanel,SET_PANEL_CBCOLLECTMENTION,&flag);
		    gSetting.collectTestMention=flag;
			GetCtrlVal(settingPanel,SET_PANEL_AUTOSAVE,&flag);
		    gSetting.autoSave=flag;
			GetCtrlVal(settingPanel,SET_PANEL_RELAY_ENABLE,&flag);
		    gSetting.relayEnable=flag;			
			GetCtrlVal(settingPanel,SET_PANEL_CBLOG,&flag);
		    gSetting.saveLog=flag;
			GetCtrlVal(settingPanel,SET_PANEL_SIMUTESTCB,&flag);
		    gSetting.simuTest=flag;			
			GetCtrlVal(settingPanel,SET_PANEL_RPINFOCB,&flag);
		    gSetting.reportInfoCustom=flag;
			GetCtrlVal(settingPanel,SET_PANEL_NUMERIC,&flag);
		    gSetting.maxComunPerRowInAutoTestPanel=flag;
			GetCtrlVal(settingPanel,SET_PANEL_SAVEDIR,temp);
			memset(gSetting.saveDir,strlen(gSetting.saveDir),0); 
			sprintf(gSetting.saveDir,"%s",temp);
			
	        GetCtrlIndex(settingPanel,SET_PANEL_RING,&flag);
	        GetValueFromIndex(settingPanel,SET_PANEL_RING,flag,&gSetting.methodToGetdirFile);
			
			GetCtrlVal(settingPanel,SET_PANEL_RING_CLOSETIME,&flag);
			gSetting.mentionAutoCloseTime=flag;
			
			GetCtrlVal(settingPanel,SET_PANEL_RETESTNUM,&flag);
		    gSetting.reTestCnt=flag;			
			//printf("%d\n",flag);
			
			saveSetting(gSetting);
			if(gSetting.saveLog==0)
			{
			   deleteLogFile(); 
			}	
        	//SettingModeInit();
	        ShowSettingInfo(gSetting);
			
			break;
	}
	return 0;
}

int CVICALLBACK DIRSELECTED (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char currentDirectory[MAX_PATHNAME_LEN]={0};
    char newDirectory[MAX_PATHNAME_LEN]={0};		

	switch (event)
	{
		case EVENT_COMMIT:
		GetProjectDir(currentDirectory);
        if (DirSelectPopup (currentDirectory, "Select Directory", 1, 0,
                                newDirectory) != VAL_NO_DIRECTORY_SELECTED)
         {
			SetCtrlVal(settingPanel,SET_PANEL_SAVEDIR,newDirectory); 
		 }
                

			break;
	}
	return 0;
}

SETTING GetSetting()  
{
	if(settingInit==FALSE)	  //û�г�ʼ���Ϳ��ܱ�����
	{
	    gSetting=getSetting();  
		settingInit==TRUE;
		
	}	
	//printf("---%d\n",gSetting.reportInfoCustom);
	return  gSetting;
}
