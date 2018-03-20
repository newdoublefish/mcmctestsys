#include <formatio.h>
#include <cvirte.h>		
#include <userint.h>
#include "reportDb.h"
#include "reportManagerPanel.h"
#include "ftpHelper.h"
#include "common.h"
#include "sutCommon.h"
#include "httpPost.h"

static int reportManagerPanel;

static void showRecord(ListType list)
{
	tAutoTestRecord record={0};
	char temp[50]={0};
	for(int i=0;i<ListNumItems(list);i++){
		
		char indexTag[10]={0};
		ListGetItem(list,&record,i);
		sprintf(indexTag,"%d",record.m_id);
       	InsertTreeItem (reportManagerPanel, PANEL_RECORDTREE, VAL_SIBLING,0,VAL_LAST,record.m_code,indexTag, 0, 0);  //TAG 用组在组链表中的序号		
		if(record.m_result>0)
			Fmt(temp,"%s","通过");
		else
			Fmt(temp,"%s","未通过");
		
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,1,ATTR_LABEL_TEXT,temp);
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,2,ATTR_LABEL_TEXT,record.m_createtime);
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,3,ATTR_LABEL_TEXT,record.m_createtime); 
		memset(temp,0,50);
		if(record.m_upload>0)
			Fmt(temp,"%s","已上传");
		else
			Fmt(temp,"%s","未上传");
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

static void CVICALLBACK ReportMenuItemCB(int panel, int controlID, int MenuItemID, 
									void *callbackData)
{
	int count=0,check=0;
	if(MenuItemID==1)
	{
		int ftpPanel= LoadPanel (reportManagerPanel, "reportManagerPanel.uir", FTP);
		DisplayPanel(ftpPanel);
		SetCtrlVal(ftpPanel,FTP_TEXTBOX,"开始上传\n");
		
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
						sprintf(type,"%s","安规");
					}else if(strcmp(sut.dbName,"board")==0)
					{
						sprintf(type,"%s","板级");
					}else if(strcmp(sut.dbName,"integrate")==0)
					{
						sprintf(type,"%s","整机");
					}
						
					//httpPost(record.m_code,type,remotePath,"锐速","阿豪",1);
					updateUpload(atoi(tag),1);
				    SetCtrlVal(ftpPanel,FTP_TEXTBOX,record.m_reportpath);
					SetCtrlVal(ftpPanel,FTP_TEXTBOX,"\n"); 
				}else{
					SetCtrlVal(ftpPanel,FTP_TEXTBOX,"Ftp上传出错\n");
					break;
				}
				
			}
		}
		SetCtrlAttribute(ftpPanel,FTP_COMMANDBUTTON,ATTR_DIMMED,0);
		SetCtrlVal(ftpPanel,FTP_TEXTBOX,"完成\n"); 
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
	}
	
}


static void configTreeAttribute(int panel,int tree)
{
	HideBuiltInCtrlMenuItem(panel, tree, VAL_EXPAND_SUBTREE); 
	HideBuiltInCtrlMenuItem(panel, tree, VAL_SORT);
	HideBuiltInCtrlMenuItem(panel, tree, VAL_COLLAPSE_SUBTREE);
	HideBuiltInCtrlMenuItem(panel, tree, VAL_EXPAND_ALL);
	HideBuiltInCtrlMenuItem(panel, tree, VAL_COLLAPSE_ALL);
	NewCtrlMenuItem(panel, tree, "上传", -1, ReportMenuItemCB, 0);
	NewCtrlMenuItem(panel, tree, "删除", -1, ReportMenuItemCB, 0);
	NewCtrlMenuItem(panel, tree, "打开报表", -1, ReportMenuItemCB, 0);
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
