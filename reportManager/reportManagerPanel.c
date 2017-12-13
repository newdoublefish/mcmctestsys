#include <formatio.h>
#include <cvirte.h>		
#include <userint.h>
#include "reportDb.h"
#include "reportManagerPanel.h"

static int reportManagerPanel;

static void showRecord(ListType list)
{
	tAutoTestRecord record={0};
	char temp[50]={0};
	for(int i=0;i<ListNumItems(list);i++){
		
		char indexTag[10]={0};
		ListGetItem(list,&record,i);
		sprintf(indexTag,"%d",record.id);
       	InsertTreeItem (reportManagerPanel, PANEL_RECORDTREE, VAL_SIBLING,0,VAL_LAST,record.ProductId,indexTag, 0, 0);  //TAG 用组在组链表中的序号		
		if(record.Result>0)
			Fmt(temp,"%s","通过");
		else
			Fmt(temp,"%s","未通过");
		
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,1,ATTR_LABEL_TEXT,temp);
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,2,ATTR_LABEL_TEXT,record.m_date);
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,3,ATTR_LABEL_TEXT,record.m_name); 
		memset(temp,0,50);
		if(record.m_update>0)
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
		
		GetNumListItems(panel,controlID,&count);
		for(int i=0;i<count;i++)
		{
			IsListItemChecked(panel,controlID,i,&check);
			
			if(check)
			{
				char tag[32]={0}; 
				GetTreeItemTag(reportManagerPanel,controlID,i,tag);
				updateUpload(atoi(tag),1);
			}
		}
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
				printf("row:%d,checked:%d\n",i,check);
				printf("id %d\n",atoi(tag));
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
