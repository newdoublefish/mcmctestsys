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

int sutConfigPanelHandle;   
extern int g_mainHWND; //先不发消息
 

//ListType sutList;  
SUTCONFIG sutConfig;
SUT selectSut;

void DisplaySutConfigPanel()
{
	SUT temp; 
    sutConfigPanelHandle = LoadPanel (0, "SutPanel.uir", PANEL_PART); 
    //清楚Listbox里面预先定义的值，都从配置文件里面获取
	ClearListCtrl(sutConfigPanelHandle,PANEL_PART_LISTBOX_PARTS_SELECT);
	
	//从配置文件里面获取被测系统
	//SystemListInit();
	sutConfig=GetSystemConfig();
    //listCount= SystemListNumItems();
	
	//显示被测系统
	for(int index=1;index<=ListNumItems(sutConfig.sutList);index++)
	{
       //SystemListGetItem(&temp,index);
	   ListGetItem(sutConfig.sutList,&temp,index);
	   InsertListItem(sutConfigPanelHandle,PANEL_PART_LISTBOX_PARTS_SELECT,index-1,temp.systemName,0);    
	}
    //显示面板
    DisplayPanel(sutConfigPanelHandle);
	RunUserInterface();
	DiscardPanel (sutConfigPanelHandle); 
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
		  GetCtrlIndex(sutConfigPanelHandle,PANEL_PART_LISTBOX_PARTS_SELECT,&index);   
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
			unsigned int wParam1=9;
            unsigned int lParam1=0;
			PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1); 
			QuitUserInterface(1);
			break;
	}
	return 0;
}
