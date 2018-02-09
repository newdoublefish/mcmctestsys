#include "toolbox.h"
#include"testProject.h"
#include "MainPanel.h"
#include "settingConfig.h"


int CVICALLBACK projectPanelCtrlCallback (int panel, int control, int event,
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

int CVICALLBACK ProjectPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			//if(checkScanResult(panelHandle)==TRUE) 
			 BOOL *flag = (BOOL *)callbackData;
			 *flag = FALSE;
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}

BOOL newTestProject(tTestProject *project)
{
	//tTestProject t={0};
	BOOL ret=TRUE;
	int panelHandle = LoadPanel(0,"MainPanel.uir",NEW_TEST);
	InstallCtrlCallback(panelHandle,NEW_TEST_SURE,projectPanelCtrlCallback,NULL);
	InstallPanelCallback(panelHandle,ProjectPanelCallback,&ret);
	DisplayPanel(panelHandle);
	RunUserInterface();
	if(ret==TRUE)
	{
		GetCtrlVal(panelHandle,NEW_TEST_STRING,project->projectName);
		SETTING s=getSetting();
		sprintf(project->projectPath,"%s",s.saveDir);
	}
	DiscardPanel(panelHandle);
	return ret;
}

BOOL loadTestProject(tTestProject *project)
{
	SETTING s=getSetting();
	if(FileSelectPopup (s.saveDir, "*.xml", "*.xml", "请选择要导入的测试", VAL_LOAD_BUTTON, 0, 0, 1,1,project->projectPath)!=VAL_NO_FILE_SELECTED)
	{
		//DisplayAutoTestPanelWithTestData(getItemList(),getEutList(),GetCollectList(),ENUM_TEST_PANEL_AUTO,filePath);  
		return TRUE;
	}
	return FALSE;
}


BOOL saveTestProjectInfo(tTestProject info)
{
	return TRUE;
}


