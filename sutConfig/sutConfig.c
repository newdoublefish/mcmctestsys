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
#include <cvirte.h>		
#include <userint.h>
#include <pathctrl.h>
#include "cvixml.h"        
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "sutEntity.h"
#include "SutPanel.h"  
#include "convertTool.h"
#include "testGroupInit.h"
#include "sutCommon.h"
#include "protocolHelper.h"
#include "testInit.h"
#include "debug.h"
#include "application.h"

//int sutConfigPanelHandle;   
extern int g_mainHWND; //先不发消息
 

//ListType sutList;  
SUTCONFIG sutConfig;
SUT selectSut;

int CVICALLBACK OnListPartClick (int panelHandle, int controlID, int event, void *callbackData, int eventData1, int eventData2)
{
	int index=0;
	unsigned int wParam1=1;
    unsigned int lParam1=0;	
	switch (event)
	{
		/*case EVENT_LEFT_DOUBLE_CLICK:

			GetCtrlIndex(panelHandle,controlID,&index);
		    wParam1=1;
		    ListGetItem(sutConfig.sutList,&selectSut,index+1);	
		    PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1);
 		    QuitUserInterface(1);
			break;*/
		case EVENT_COMMIT:

			GetCtrlIndex(panelHandle,controlID,&index);
		    wParam1=1;
		    ListGetItem(sutConfig.sutList,&selectSut,index+1);	
		    PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1);
 		    QuitUserInterface(1);
			break;			
	}
	return 0;			
}

int CVICALLBACK SutPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			unsigned int wParam1=9;
            unsigned int lParam1=0;
			PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1); 
			QuitUserInterface(1);
		     break;
	}
	return 0;
}


void DisplaySutConfigPanel()
{
	SUT temp; 
    int panelHandle = LoadPanel (0, "SutPanel.uir", PANEL_PART); 
    //清楚Listbox里面预先定义的值，都从配置文件里面获取
	ClearListCtrl(panelHandle,PANEL_PART_LISTBOX_PARTS_SELECT);
	
	//从配置文件里面获取被测系统
	//SystemListInit();
	sutConfig=GetSystemConfig();
    //listCount= SystemListNumItems();
	
	//显示被测系统
	for(int index=1;index<=ListNumItems(sutConfig.sutList);index++)
	{
       //SystemListGetItem(&temp,index);
	   ListGetItem(sutConfig.sutList,&temp,index);
	   if(strlen(temp.nickName)>0)
	   		InsertListItem(panelHandle,PANEL_PART_LISTBOX_PARTS_SELECT,index-1,temp.nickName,0);  //用别名好理解一点
		else
			InsertListItem(panelHandle,PANEL_PART_LISTBOX_PARTS_SELECT,index-1,temp.systemName,0); //如果没有别名显示系统名称	
	}
	InstallCtrlCallback(panelHandle,PANEL_PART_LISTBOX_PARTS_SELECT,OnListPartClick,NULL);
	InstallPanelCallback(panelHandle,SutPanelCallback,NULL);
	tApplication t = getApplication();
	SetCtrlVal(panelHandle,PANEL_PART_VERSION,t.basic.version);
	SetPanelAttribute(panelHandle,ATTR_TITLE,t.basic.company);
    //显示面板
    DisplayPanel(panelHandle);
	RunUserInterface();
	DiscardPanel (panelHandle); 
}


int CVICALLBACK SYSTEM_SELECTE_NEXT (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int index;
	int j;
	unsigned int wParam1=1;
    unsigned int lParam1=0;
	TestItem item;
	TestItem subItem;
	HRESULT error = 0; 
	
	switch (event)
	{
		case EVENT_COMMIT:
		  GetCtrlIndex(panel,PANEL_PART_LISTBOX_PARTS_SELECT,&index);   
		  wParam1=1;
		  ListGetItem(sutConfig.sutList,&selectSut,index+1);
		  //printf("%s\n",selectSut.dbName);
		  
		  //ItemInit(selectSut.configPath);   //解析测试用例表
		  //protocolInit();                   //解析协议
		  //ManualItemLen = ManualItemInit(project.configPath,ManuItem);
		  //StatusPanelInit (LRUstatus); 
		  //打印结果
		  //printList(items);
          //打印结果  
		  //if(testInit(selectSut))
		  /*
		  if(strstr(selectSut.reportFilePath,"exe")!=NULL)
		  {
			    char fileName[MAX_PATHNAME_LEN]; 
				GetProjectDir (fileName);
				
		    	//LaunchExecutable (selectSut.configPath);
				//system(selectSut.configPath);
				char temp[MAX_PATHNAME_LEN]={0};
				sprintf(temp,"%s%s",selectSut.configPath,".exe");
				CopyFile(selectSut.configPath,temp);
				system(temp);
				DeleteFile(temp);
			    unsigned int wParam1=9;
                unsigned int lParam1=0;
			    PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1); 
			    QuitUserInterface(1);		  
		  }
		  */
		  
		  PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1);
 		  QuitUserInterface(1);

		  break;
	}
	return 0;
}

SUT GetSeletedSut(void)
{
    return  selectSut;
}

char *getVersion(void)
{
	//PRINT("VERSION:%s",sutConfig.version);
    return sutConfig.version;
}



int CVICALLBACK oNsutQuit (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			
			break;
	}
	return 0;
}
