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
#include "StatusMonitorPanel.h"
#include "eutConfig.h"
#include "testGroupInit.h"
#include "statusCaseEntity.h" 
#include "sutCommon.h"
#include "sutEntity.h"
#include "matainHelper.h"


static int panelStateMonitor = 0;	//״̬������   


static int quitThreadCount;

static int num=0;
StatusLRU LRUstatus[256];
HANDLE TPG_ThreadHandle[8];
int TPG_StopThread;
static int currentPanel=0;
DWORD ThreadProc(LPVOID num);
void FillStatusPanel(int panelHandle)
{
		 for(int j=0;j<num;j++)
		 {
			 int parent;
	         InsertTreeItem (panelHandle, PANEL_TREE, VAL_SIBLING,0, VAL_LAST,
                    LRUstatus[j].LRUName, 0, 0, 0);
			 GetNumTreeItems(panelHandle,PANEL_TREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&parent);
			 parent--; 
		     for(int subJ=0;subJ<32;subJ++)
			 {
				  int itemCount;
				  GetNumListItems(panelHandle, PANEL_TREE,&itemCount);  
	    	      //InsertTreeItem (panelHandle, PANEL_TREE, VAL_CHILD,itemCount-1, VAL_NEXT,
                  //   LRUstatus[j].ItemName[subJ],0, 0, 0);
       		      InsertTreeItem (panelHandle, PANEL_TREE, VAL_CHILD,parent,VAL_LAST,
                    LRUstatus[j].ItemName[subJ],0, 0, 0);
				  
			        
			 }	 
		 }	 
}

HRESULT DisplayStatusMonitorPanel() 
{
   int number;
   if ((panelStateMonitor = LoadPanel (0, "StatusMonitorPanel.uir", PANEL_STAT)) <= 0)		   //״̬�������
   {
	  return -1; 
   }
   ListType eutList=getEutList(); //��ȡuut����
   
   SUT project; //��ȡѡ��ı���ϵͳ
   project=GetSeletedSut();
   num=StatusItemInit(project.configPath,LRUstatus); //����״̬�����Ŀ
   

   
   for(int i=1;i<=ListNumItems(eutList);i++)   //����eut�豸����������Ӧ������panel
   {
		EUT eut;
	    ListGetItem (eutList,&eut,i);
		int panelHandle = LoadPanel (0, "StatusMonitorPanel.uir", PANEL);
		
		FillStatusPanel(panelHandle);
		
        SetActiveTreeItem (panelHandle, PANEL_TREE, 0, VAL_REPLACE_SELECTION_WITH_ITEM);  
		InsertPanelAsTabPage (panelStateMonitor,PANEL_STAT_TAB_STAT,i-1,panelHandle);
		SetTabPageAttribute (panelStateMonitor,PANEL_STAT_TAB_STAT,i-1 ,ATTR_LABEL_TEXT, eut.eutName);
   }
   TPG_StopThread = 0;  
   for(int eutIndex=1;eutIndex<=ListNumItems(eutList);eutIndex++)
   {
	    DWORD ThreadID; 
   		TPG_ThreadHandle[eutIndex-1] = CreateThread(0,0,(LPTHREAD_START_ROUTINE)ThreadProc ,(LPVOID)&eutIndex,0,(LPDWORD)&ThreadID);//ThreadCreate(eutIndex-1);
		
		 Delay(0.5);
   }
  
   DisplayPanel (panelStateMonitor);   
   return 0;
}

int CVICALLBACK QUITSTATUSMONITOR (int panel, int control, int event,  //�˳�״̬���ӽ���
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			//�ͷ�����TAB
			//ThreadDestroy();
			TPG_StopThread=1;
			ListType eutList=getEutList();
			int temp= ListNumItems(eutList);
			while(quitThreadCount!=temp);
			DiscardPanel(panelStateMonitor);
			for(int i=0;i<temp;i++)
			{	
			  CloseHandle(TPG_ThreadHandle[i]);  
			}
			
			//CmtDiscardLock(statusLock);
			quitThreadCount=0;
			currentPanel=0;
			break;
	}
	return 0;
}



void ShowStatusMonitorResult(int tabPanel,Scene scene)
{
    	
	for(int i=0;i<num;i++)
	{
	    int groupIndex=0;
		for(int temp=1;temp<i+1;temp++) //��ȡ�ڼ����ֶε����
		{
		   GetTreeItem(tabPanel,PANEL_TREE,VAL_SIBLING,0,groupIndex,VAL_NEXT,0,&groupIndex);   
		}	
	   
	   int gatherValue= scene.resultBuffer[i*4] << 24 | scene.resultBuffer[i*4+1] << 16 | scene.resultBuffer[i*4+2] << 8 | scene.resultBuffer[i*4+3];
	   
	    
	   groupIndex++;
	   for(int j=0;j<32;j++)
	   {
            if ((gatherValue & 0x1) == 0)
            {
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 0, ATTR_LABEL_BGCOLOR,VAL_GREEN);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 1, ATTR_LABEL_BGCOLOR,VAL_GREEN);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 2, ATTR_LABEL_BGCOLOR,VAL_GREEN);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 2, ATTR_LABEL_TEXT,"����");
				
            }
            else
            {
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 0, ATTR_LABEL_BGCOLOR,VAL_RED);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 1, ATTR_LABEL_BGCOLOR,VAL_RED);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 2, ATTR_LABEL_BGCOLOR,VAL_RED);
				SetTreeCellAttribute (tabPanel, PANEL_TREE, groupIndex+j, 2, ATTR_LABEL_TEXT,"����");
            }
            gatherValue = gatherValue >> 1;	   
	   }	    
	}	

}


DWORD ThreadProc(LPVOID num)
{
	EUT eut;
	int ret;
	char str[32];
	char STR[10];
	char string[50];
	int tabPanel;
	int val;
	int devIndex=*(int*)num-1;
	char temp[100]={0};
	
	GetPanelHandleFromTabPage(panelStateMonitor, PANEL_STAT_TAB_STAT, devIndex, &tabPanel);
	ListType eutList=getEutList ();					 
	ListGetItem (eutList,&eut,devIndex+1); 
		
	sprintf (string,"ά��״̬��COM%d",eut.matainConfig.portNum);
	SetCtrlVal (tabPanel,PANEL_TEXTMSG_MAINTAN,string);
	ret = OpenComConfig (eut.matainConfig.portNum, "", eut.matainConfig.baudRate, eut.matainConfig.parity,
													 eut.matainConfig.dataBit,eut.matainConfig.stopBit, 512, 512);
	
	
	
	while(!TPG_StopThread)
	{   
		if(currentPanel==tabPanel)
		{	
    	    Scene scene;
	        scene = createStatusMonitorScene();
	        scene.comPort = eut.matainConfig.portNum; 
		
	     	runSceneMatainProcess(&scene);

		
	    	if(ret == 0)				    
		    	SetCtrlAttribute (tabPanel, PANEL_TEXTMSG_MAINTAN, ATTR_TEXT_BGCOLOR, VAL_GREEN);
		    else
			    SetCtrlAttribute (tabPanel, PANEL_TEXTMSG_MAINTAN, ATTR_TEXT_BGCOLOR, VAL_RED);
		
			 ShowStatusMonitorResult(tabPanel,scene);  
 
		   releaseScene(&scene);
		}else
		{
		   Delay(0.5);
		}	
	}
	CloseCom (eut.matainConfig.portNum);
	quitThreadCount++;
	return 0;
}   

int CVICALLBACK TAB_STAT_CLICK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_ACTIVE_TAB_CHANGE:
			
			int index;
			GetActiveTabPage(panelStateMonitor,PANEL_STAT_TAB_STAT, &index);
			GetPanelHandleFromTabPage(panelStateMonitor,PANEL_STAT_TAB_STAT,index,&currentPanel);
			
			break;
	}
	return 0;
}
