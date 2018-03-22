#include "toolbox.h"
#include"testProject.h"
#include "MainPanel.h"
#include "settingConfig.h"
#include "common.h"

tTestProject gProject={0};


int CVICALLBACK projectPanelCtrlCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(control ==  NEW_TEST_SURE)
			{
			 	QuitUserInterface(1);
			}else if(control == NEW_TEST_BROWSE)
			{
				char currentDirectory[MAX_PATHNAME_LEN]={0};
    			char newDirectory[MAX_PATHNAME_LEN]={0};
				SETTING s=getSetting();
				if (DirSelectPopup (s.saveDir, "Select Directory", 1, 0,
                                newDirectory) != VAL_NO_DIRECTORY_SELECTED)
         		 {
					  SetCtrlVal(panel,NEW_TEST_DIR,newDirectory); 
		 		 }				
			}
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

BOOL newTestProject()
{
	//tTestProject t={0};
	BOOL ret=TRUE;
	memset(&gProject,0,sizeof(tTestProject));
	int panelHandle = LoadPanel(0,"MainPanel.uir",NEW_TEST);
	InstallCtrlCallback(panelHandle,NEW_TEST_SURE,projectPanelCtrlCallback,NULL);
	InstallCtrlCallback(panelHandle,NEW_TEST_BROWSE,projectPanelCtrlCallback,NULL);  
	InstallPanelCallback(panelHandle,ProjectPanelCallback,&ret);
	SETTING s=getSetting();
	SetCtrlVal(panelHandle,NEW_TEST_DIR,s.saveDir);	
	DisplayPanel(panelHandle);
	RunUserInterface();
	if(ret==TRUE)
	{
		char newDir[MAX_PATHNAME_LEN]={0};
		GetCtrlVal(panelHandle,NEW_TEST_STRING,gProject.projectName);
		GetCtrlVal(panelHandle,NEW_TEST_DIR,newDir);
		int flag = 0;                                      		
		GetCtrlVal(panelHandle,NEW_TEST_CBMAKEDIR,&flag);
		DiscardPanel(panelHandle); 


		if(flag == 1)
		{
			sprintf(gProject.projectDir,"%s\\%s",newDir,gProject.projectName);
			int tempRet = MakeDir(gProject.projectDir);
			if(tempRet!=0)
			{
				if(tempRet==-9)
				{
					if(FALSE==AlertDialogWithRetAutoClose(0,"警告","试图覆盖已存在的测试，是否继续","不继续","继续",0))
					{
						return FALSE;			
					}
				}else{
					return FALSE;
				}
			}
		}else{
			sprintf(gProject.projectDir,"%s",newDir);
		}
		//sprintf(newDir,"%s\\%s", 
		//SETTING s=getSetting();
		sprintf(gProject.projectPath,"%s\\%s%s",gProject.projectDir,gProject.projectName,".xml");
	}else{
		DiscardPanel(panelHandle); 
	}
	
	return ret;
}

BOOL setProjectPath(char *path)
{
	if(FileExists(path,NULL)==1)
	{
		memset(&gProject,0,sizeof(tTestProject)); 
		sprintf(gProject.projectPath,"%s",path);
		char driveName[MAX_PATHNAME_LEN];  
		char directoryName[MAX_PATHNAME_LEN];  
		char fileName[MAX_PATHNAME_LEN];  
		SplitPath (gProject.projectPath, driveName,directoryName,fileName);
		char *token=strstr(fileName,".xml");
		if(token!=NULL)
		{
			memcpy(gProject.projectName,fileName,token-fileName);
		}
		int dirNameLen = strlen(directoryName);
		if(directoryName[dirNameLen-1]=='\\')
		{
			directoryName[dirNameLen-1]='\0';	
		}
		sprintf(gProject.projectDir,"%s%s",driveName,directoryName);
		return TRUE;
	}
	return FALSE;
}

BOOL loadTestProject()
{
	SETTING s=getSetting();
	memset(&gProject,0,sizeof(tTestProject));  
	if(FileSelectPopup (s.saveDir, "*.xml", "*.xml", "请选择要导入的测试", VAL_LOAD_BUTTON, 0, 0, 1,1,gProject.projectPath)!=VAL_NO_FILE_SELECTED)
	{
		//DisplayAutoTestPanelWithTestData(getItemList(),getEutList(),GetCollectList(),ENUM_TEST_PANEL_AUTO,filePath); 
		
		char driveName[MAX_PATHNAME_LEN];  
		char directoryName[MAX_PATHNAME_LEN];  
		char fileName[MAX_PATHNAME_LEN];  
		SplitPath (gProject.projectPath, driveName,directoryName,fileName);
		char *token=strstr(fileName,".xml");
		if(token!=NULL)
		{
			memcpy(gProject.projectName,fileName,token-fileName);
		}
		int dirNameLen = strlen(directoryName);
		if(directoryName[dirNameLen-1]=='\\')
		{
			directoryName[dirNameLen-1]='\0';	
		}
		sprintf(gProject.projectDir,"%s%s",driveName,directoryName);
		return TRUE;
	}
	return FALSE;
}

tTestProject *getCurrentProject()
{
	return  &gProject;
}


BOOL saveTestProjectInfo(tTestProject info)
{
	return TRUE;
}


