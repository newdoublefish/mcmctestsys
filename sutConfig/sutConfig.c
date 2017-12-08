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
#include "statusCaseEntity.h"
#include "sutCommon.h"
#include "protocolHelper.h"
#include "testInit.h"
#include "debug.h"

int sutConfigPanelHandle;   
extern int g_mainHWND; //�Ȳ�����Ϣ
 

//ListType sutList;  
SUTCONFIG sutConfig;
SUT selectSut;

void DisplaySutConfigPanel()
{
	SUT temp; 
    sutConfigPanelHandle = LoadPanel (0, "SutPanel.uir", PANEL_PART); 
    //���Listbox����Ԥ�ȶ����ֵ�����������ļ������ȡ
	ClearListCtrl(sutConfigPanelHandle,PANEL_PART_LISTBOX_PARTS_SELECT);
	
	//�������ļ������ȡ����ϵͳ
	//SystemListInit();
	sutConfig=GetSystemConfig();
    //listCount= SystemListNumItems();
	
	//��ʾ����ϵͳ
	for(int index=1;index<=ListNumItems(sutConfig.sutList);index++)
	{
       //SystemListGetItem(&temp,index);
	   ListGetItem(sutConfig.sutList,&temp,index);
	   InsertListItem(sutConfigPanelHandle,PANEL_PART_LISTBOX_PARTS_SELECT,index-1,temp.systemName,0);    
	}
    //��ʾ���
    DisplayPanel(sutConfigPanelHandle);
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
		  
		  //ItemInit(selectSut.configPath);   //��������������
		  //protocolInit();                   //����Э��
		  //ManualItemLen = ManualItemInit(project.configPath,ManuItem);
		  //StatusPanelInit (LRUstatus); 
		  //��ӡ���
		  //printList(items);
          //��ӡ���  
		  //if(testInit(selectSut))

		  DiscardPanel (sutConfigPanelHandle); 
		  PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1);
 

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
			break;
	}
	return 0;
}
