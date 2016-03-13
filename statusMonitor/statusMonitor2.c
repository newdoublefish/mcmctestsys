#include "StatusMonitorPanel2.h"
#include <cvirte.h>	
#include "windows.h" 
#include <rs232.h>
#include <userint.h>
#include <pathctrl.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "WinBase.h"
#include "excelHelper.h"
#include "eutConfig.h"
#include "testGroupInit.h"
#include "statusCaseEntity.h" 
#include "sutCommon.h"
#include "sutEntity.h"
#include "matainHelper.h"

static int panelStateMonitor2;
static EUT selectedEut;
static int monitorPanel;
static int monitorItemNum;
static StatusLRU LRUstatus2[256];   
static BOOL monitorStopFlag=FALSE;
static int funcId=0; 

HRESULT DisplayStatusMonitorPanel2() 
{
   int number;
   if ((panelStateMonitor2 = LoadPanel (0, "StatusMonitorPanel2.uir", PANEL_STAT)) <= 0)		   //状态监视面板
   {
	  return -1; 
   }
   
   
   SUT project; //获取选择的被测系统
   project=GetSeletedSut();
   monitorItemNum=StatusItemInit(project.configPath,LRUstatus2); //分析状态监控项目
   
    ListType eutList=getEutList(); //获取uut链表   
	char temp[30]={0};
	EUT eut;
	int eutCount=ListNumItems(eutList);
	for(int eutIndex=1;eutIndex<=eutCount;eutIndex++)
	{
	    ListGetItem(eutList,&eut,eutIndex);
		sprintf(temp,"EUT%d",eutIndex);
	    InsertTreeItem (panelStateMonitor2, PANEL_STAT_TREE, VAL_SIBLING,0, VAL_LAST,
                    temp,0,0,0);
	    SetTreeCellAttribute(panelStateMonitor2,PANEL_STAT_TREE,eutIndex-1,1, ATTR_LABEL_TEXT,eut.eutName);
	}   
   
   DisplayPanel (panelStateMonitor2);   
   return 0;
}


void FillStatusPanel2(int panelHandle)
{
	
	     char temp[50]={0};
		 sprintf(temp,"EUT:%s",selectedEut.eutName);
	     SetPanelAttribute (panelHandle,ATTR_TITLE,temp); 
		 for(int j=0;j<monitorItemNum;j++)
		 {
			 int parent;
	         InsertTreeItem (panelHandle, PANEL_TREE, VAL_SIBLING,0, VAL_LAST,
                    LRUstatus2[j].LRUName, 0, 0, 0);
			 GetNumTreeItems(panelHandle,PANEL_TREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&parent);
			 parent--; 
		     for(int subJ=0;subJ<32;subJ++)
			 {
				  int itemCount;
				  GetNumListItems(panelHandle, PANEL_TREE,&itemCount);  
	    	      //InsertTreeItem (panelHandle, PANEL_TREE, VAL_CHILD,itemCount-1, VAL_NEXT,
                  //   LRUstatus[j].ItemName[subJ],0, 0, 0);
       		      InsertTreeItem (panelHandle, PANEL_TREE, VAL_CHILD,parent,VAL_LAST,
                    LRUstatus2[j].ItemName[subJ],0, 0, 0);
				  
			        
			 }	 
		 }	 
}


void ShowStatusMonitorResult2(int tabPanel,Scene scene)
{
    	
	for(int i=0;i<monitorItemNum;i++)
	{
	    int groupIndex=0;
		for(int temp=1;temp<i+1;temp++) //获取第几个字段的序号
		{
		   GetTreeItem(tabPanel,PANEL_TREE,VAL_SIBLING,0,groupIndex,VAL_NEXT,0,&groupIndex);   
		}	
	   
	   int gatherValue= scene.resultBuffer[i*4] << 24 | scene.resultBuffer[i*4+1] << 16 | scene.resultBuffer[i*4+2] << 8 | scene.resultBuffer[i*4+3];
	   char tempStr[100]={0}; 
	   sprintf(tempStr,"0x%x",gatherValue);
	   SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex, 1, ATTR_LABEL_TEXT,tempStr);  
	   
	    
	   groupIndex++;
	   for(int j=0;j<32;j++)
	   {
		    char temp[100]={0};
			GetTreeCellAttribute(tabPanel,PANEL_TREE,groupIndex+j,0,ATTR_LABEL_TEXT,temp);
			if(strcmp(temp,"保留")==0)
			{
			     SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 0, ATTR_LABEL_BGCOLOR,VAL_DK_GRAY); 

			}else{	
			
              if ((gatherValue & 0x1) == 0)
              {
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 0, ATTR_LABEL_BGCOLOR,VAL_GREEN);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 1, ATTR_LABEL_BGCOLOR,VAL_GREEN);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 1, ATTR_LABEL_TEXT,"0"); 
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 2, ATTR_LABEL_BGCOLOR,VAL_GREEN);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 2, ATTR_LABEL_TEXT,"正常");
				
              }
              else
              {
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 0, ATTR_LABEL_BGCOLOR,VAL_RED);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 1, ATTR_LABEL_BGCOLOR,VAL_RED);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 1, ATTR_LABEL_TEXT,"1");  
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 2, ATTR_LABEL_BGCOLOR,VAL_RED);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 2, ATTR_LABEL_TEXT,"故障");
              }
	        }
            gatherValue = gatherValue >> 1;	   
	   }	    
	}	

}

static int CVICALLBACK ThreadProc2 (void *unused)
//DWORD ThreadProc2(LPVOID num)
{
	BOOL ret;
	char str[32];
	char STR[10];
	char string[50];

	char temp[100]={0};
	int sendCount=0;
	int recvCount=0;
	
	sprintf (string,"维护状态：COM%d",selectedEut.matainConfig.portNum);
	SetCtrlVal (monitorPanel,PANEL_TEXTMSG_MAINTAN,string);

	
	
	monitorStopFlag=FALSE;
	while(monitorStopFlag!=TRUE)
	{   
		
	        ret = OpenComConfig (selectedEut.matainConfig.portNum, "", selectedEut.matainConfig.baudRate, selectedEut.matainConfig.parity,
													 selectedEut.matainConfig.dataBit,selectedEut.matainConfig.stopBit, 512, 512);	
			if(ret<0)
				continue;
    	    Scene scene;
	        scene = createStatusMonitorScene();
	        scene.comPort = selectedEut.matainConfig.portNum; 
			
			
			sendCount++;
			memset(string,0,50);
			Fmt(string,"%d",sendCount);
			SetCtrlVal (monitorPanel,PANEL_SENDCOUNT,string);
			
	     	ret=runSceneMatainProcess(&scene);

		
	    	if(ret==TRUE)
			{	
		    	recvCount++;
				memset(string,0,50);
		    	Fmt(string,"%d",recvCount);
				SetCtrlVal (monitorPanel,PANEL_RECVCOUNT,string); 
				SetCtrlAttribute (monitorPanel, PANEL_TEXTMSG_MAINTAN, ATTR_TEXT_BGCOLOR, VAL_GREEN);
				ShowStatusMonitorResult2(monitorPanel,scene); 
			}	
		    else
			{	
			    SetCtrlAttribute (monitorPanel, PANEL_TEXTMSG_MAINTAN, ATTR_TEXT_BGCOLOR, VAL_RED);
			    releaseScene(&scene); 
				continue;
			}
			  
 
		   releaseScene(&scene);
		   CloseCom (selectedEut.matainConfig.portNum); 
		   //if(monitorStopFlag!=TRUE)
		   Delay(1);
	
	}
	

	return 0;
} 

void enterMonitorEut(EUT eut)
{
   if ((monitorPanel = LoadPanel (0, "StatusMonitorPanel2.uir", PANEL)) <= 0)		   
   {
	  return; 
   }
   
   FillStatusPanel2(monitorPanel);
   DisplayPanel (monitorPanel);
   

   
   CmtScheduleThreadPoolFunctionAdv (DEFAULT_THREAD_POOL_HANDLE,
                                              ThreadProc2,
                                              NULL, THREAD_PRIORITY_NORMAL,
                                              NULL,
                                              EVENT_TP_THREAD_FUNCTION_END, NULL,
                                              CmtGetCurrentThreadID(),&funcId);  
   
   Delay(0.5);
   

   
    
}


int CVICALLBACK MONITOREUTSELECTCALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int selectedIndex=0;
			GetActiveTreeItem(panelStateMonitor2,PANEL_STAT_TREE,&selectedIndex);
			selectedIndex=selectedIndex+1;
			ListType eutList=getEutList ();
			ListGetItem (eutList,&selectedEut,selectedIndex); 
			enterMonitorEut(selectedEut);
			

			break;
	}
	return 0;
}

int CVICALLBACK MONITORQUITCALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			monitorStopFlag=TRUE;
            CmtWaitForThreadPoolFunctionCompletion(DEFAULT_THREAD_POOL_HANDLE,funcId,OPT_TP_PROCESS_EVENTS_WHILE_WAITING); 
            CmtReleaseThreadPoolFunctionID(DEFAULT_THREAD_POOL_HANDLE,funcId); 
            DiscardPanel(monitorPanel);			
			

			break;
	}
	return 0;
}

int CVICALLBACK QUITSTATUSMONITOR2 (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			DiscardPanel(panelStateMonitor2);

			break;
	}
	return 0;
}
