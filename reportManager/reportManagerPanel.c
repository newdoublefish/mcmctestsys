#include "windows.h" 
#include <formatio.h>
#include <cvirte.h>		
#include <userint.h>
#include "toolbox.h"
#include "reportDb.h"
#include "reportManagerPanel.h"
#include "ftpHelper.h"
#include "common.h"
#include "sutCommon.h"
#include "httpPost.h"
#include "testProject.h"

static int reportManagerPanel;

static void showRecord(ListType list)
{
	tAutoTestRecord record={0};
	char temp[50]={0};
	for(int i=0;i<ListNumItems(list);i++){
		
		char indexTag[10]={0};
		ListGetItem(list,&record,i);
		sprintf(indexTag,"%d",record.m_id);
       	InsertTreeItem (reportManagerPanel, PANEL_RECORDTREE, VAL_SIBLING,0,VAL_LAST,record.m_code,indexTag, 0, 0);  //TAG �������������е����		
		if(record.m_result>0)
			Fmt(temp,"%s","ͨ��");
		else
			Fmt(temp,"%s","δͨ��");
		
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,1,ATTR_LABEL_TEXT,temp);
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,2,ATTR_LABEL_TEXT,record.m_createtime);
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,3,ATTR_LABEL_TEXT,record.m_createtime); 
		memset(temp,0,50);
		if(record.m_upload>0)
			Fmt(temp,"%s","���ϴ�");
		else
			Fmt(temp,"%s","δ�ϴ�");
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,4,ATTR_LABEL_TEXT,temp);
	}
}

static void refreshRecordTree()
{
	
	ClearListCtrl(reportManagerPanel,PANEL_RECORDTREE);
	ListType list=ListCreate(sizeof(tAutoTestRecord));
	getAllRecord(list);
	showRecord(list);
	ListDispose(list);
}
extern int g_mainHWND;
static void CVICALLBACK ReportMenuItemCB(int panel, int controlID, int MenuItemID, 
									void *callbackData)
{
	int count=0,check=0;
	int checkNum=0;
	GetNumCheckedItems(panel,controlID,&checkNum);
	if(checkNum<=0)
	{
		WarnShow1(panel,"δѡ���κ���Ŀ");
		return;
	}
	if(MenuItemID==1)
	{
		int ftpPanel= LoadPanel (reportManagerPanel, "reportManagerPanel.uir", FTP);
		DisplayPanel(ftpPanel);
		SetCtrlVal(ftpPanel,FTP_TEXTBOX,"��ʼ�ϴ�\n");
		
		GetNumListItems(panel,controlID,&count);
		for(int i=0;i<count;i++)
		{
			IsListItemChecked(panel,controlID,i,&check);
			
			if(check)
			{
				char tag[32]={0}; 
				GetTreeItemTag(reportManagerPanel,controlID,i,tag);
				tAutoTestRecord record=getRecordById(atoi(tag));
				//printRecord(record);
				
				char remotePath[256]={0};
				if(ftpSendFile(record.m_reportpath,remotePath)>=0)
				{	
					char type[50]={0};
					SUT sut=GetSeletedSut();
					if(strcmp(sut.dbName,"reliability")==0)
					{
						sprintf(type,"%s","����");
					}else if(strcmp(sut.dbName,"board")==0)
					{
						sprintf(type,"%s","�弶");
					}else if(strcmp(sut.dbName,"integrate")==0)
					{
						sprintf(type,"%s","����");
					}
						
					//httpPost(record.m_code,type,remotePath,"����","����",1);
					updateUpload(atoi(tag),1);
				    SetCtrlVal(ftpPanel,FTP_TEXTBOX,record.m_reportpath);
					SetCtrlVal(ftpPanel,FTP_TEXTBOX,"\n"); 
				}else{
					SetCtrlVal(ftpPanel,FTP_TEXTBOX,"Ftp�ϴ�����\n");
					break;
				}
				
			}
		}
		SetCtrlAttribute(ftpPanel,FTP_COMMANDBUTTON,ATTR_DIMMED,0);
		SetCtrlVal(ftpPanel,FTP_TEXTBOX,"���\n"); 
		refreshRecordTree();
	}else if(MenuItemID==2)
	{
		GetNumListItems(panel,controlID,&count);
		for(int i=0;i<count;i++)
		{
			IsListItemChecked(panel,controlID,i,&check);
			
			if(check)
			{
				char tag[32]={0}; 
				GetTreeItemTag(reportManagerPanel,controlID,i,tag);
				//printf("row:%d,checked:%d\n",i,check);
				//printf("id %d\n",atoi(tag));
				deleteById(atoi(tag));
			}
		}
		refreshRecordTree();
	}else if(MenuItemID==3)
	{
		GetNumListItems(panel,controlID,&count);
		for(int i=0;i<count;i++)
		{
			IsListItemChecked(panel,controlID,i,&check);
			
			if(check)
			{
				char tag[32]={0}; 
				GetTreeItemTag(reportManagerPanel,controlID,i,tag);
				tAutoTestRecord record=getRecordById(atoi(tag));
				//printf("%s\n",record.m_projectpath);
				if(AlertDialogWithRet(panel,"����","ȷ�ϴ򿪵�ǰ����","ȡ��","����")==FALSE)
				{
					return;
				}
				
				if(setProjectPath(record.m_projectpath)==TRUE)
				{
					unsigned int wParam1=4;   
					unsigned int lParam1=0;   
					PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1);
					QuitUserInterface (0); 
					return;//һ��Ҫreturn������������
				}else{
					WarnShow1(panel,"�޷��򿪵�ǰ���ԣ���");
				}
			}
		}
	}
	
}


static void configTreeAttribute(int panel,int tree)
{
	HideBuiltInCtrlMenuItem(panel, tree, VAL_EXPAND_SUBTREE); 
	HideBuiltInCtrlMenuItem(panel, tree, VAL_SORT);
	HideBuiltInCtrlMenuItem(panel, tree, VAL_COLLAPSE_SUBTREE);
	HideBuiltInCtrlMenuItem(panel, tree, VAL_EXPAND_ALL);
	HideBuiltInCtrlMenuItem(panel, tree, VAL_COLLAPSE_ALL);
	NewCtrlMenuItem(panel, tree, "�ϴ�", -1, ReportMenuItemCB, 0);
	NewCtrlMenuItem(panel, tree, "ɾ��", -1, ReportMenuItemCB, 0);
	NewCtrlMenuItem(panel, tree, "�򿪲���", -1, ReportMenuItemCB, 0);
}



int DisplayManagerPanel()
{
	if ((reportManagerPanel = LoadPanel (0, "reportManagerPanel.uir", PANEL)) < 0)
		return -1;
	configTreeAttribute(reportManagerPanel,PANEL_RECORDTREE);
	refreshRecordTree();
	DisplayPanel (reportManagerPanel);
	RunUserInterface ();
	DiscardPanel (reportManagerPanel);
	return 0;
}



int CVICALLBACK ReportPanelCallbak (int panel, int event, void *callbackData,
									int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_GOT_FOCUS:

			break;
		case EVENT_LOST_FOCUS:

			break;
		case EVENT_CLOSE:
		    QuitUserInterface (0);
			break;
	}
	return 0;
}

int CVICALLBACK onFtpFinishClick (int panel, int control, int event,
								  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DiscardPanel(panel);
			break;
	}
	return 0;
}
