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
extern int g_mainHWND; //�Ȳ�����Ϣ
 

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
    //���Listbox����Ԥ�ȶ����ֵ�����������ļ������ȡ
	ClearListCtrl(panelHandle,PANEL_PART_LISTBOX_PARTS_SELECT);
	
	//�������ļ������ȡ����ϵͳ
	//SystemListInit();
	sutConfig=GetSystemConfig();
    //listCount= SystemListNumItems();
	
	//��ʾ����ϵͳ
	for(int index=1;index<=ListNumItems(sutConfig.sutList);index++)
	{
       //SystemListGetItem(&temp,index);
	   ListGetItem(sutConfig.sutList,&temp,index);
	   if(strlen(temp.nickName)>0)
	   		InsertListItem(panelHandle,PANEL_PART_LISTBOX_PARTS_SELECT,index-1,temp.nickName,0);  //�ñ��������һ��
		else
			InsertListItem(panelHandle,PANEL_PART_LISTBOX_PARTS_SELECT,index-1,temp.systemName,0); //���û�б�����ʾϵͳ����	
	}
	InstallCtrlCallback(panelHandle,PANEL_PART_LISTBOX_PARTS_SELECT,OnListPartClick,NULL);
	InstallPanelCallback(panelHandle,SutPanelCallback,NULL);
	tApplication t = getApplication();
	SetCtrlVal(panelHandle,PANEL_PART_VERSION,t.basic.version);
	SetPanelAttribute(panelHandle,ATTR_TITLE,t.basic.company);
    //��ʾ���
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
		  
		  //ItemInit(selectSut.configPath);   //��������������
		  //protocolInit();                   //����Э��
		  //ManualItemLen = ManualItemInit(project.configPath,ManuItem);
		  //StatusPanelInit (LRUstatus); 
		  //��ӡ���
		  //printList(items);
          //��ӡ���  
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
