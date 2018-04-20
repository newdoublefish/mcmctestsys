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
#include "postData.h"
#include "resultUtil.h"
#include "resultInfo.h"
#include "login.h"
#include "common.h"

static int reportManagerPanel;
static HashTableType resultHashTable=0;

static void showRecord(ListType list)
{
	tAutoTestRecord record={0};
	char temp[50]={0};
	for(int i=0;i<ListNumItems(list);i++){
		
		char indexTag[10]={0};
		ListGetItem(list,&record,i+1);
		sprintf(indexTag,"%d",record.m_id);
       	InsertTreeItem (reportManagerPanel, PANEL_RECORDTREE, VAL_SIBLING,0,VAL_LAST,record.m_code,indexTag, 0, 0);  //TAG 用组在组链表中的序号		
		/*if(record.m_result>0)
			Fmt(temp,"%s","通过");
		else
			Fmt(temp,"%s","未通过");*/
		sprintf(temp,"%s%%",record.m_lasttest);
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,1,ATTR_LABEL_TEXT,temp); //显示测试进度
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,3,ATTR_LABEL_TEXT,record.m_createtime); //显示测试时间
		memset(temp,0,50);
		sprintf(temp,"%d%%",record.m_result);		
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,2,ATTR_LABEL_TEXT,temp); //显示合格率
		 
		if(FileExists(record.m_reportpath,NULL)==1)
		{
			SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,4,ATTR_LABEL_TEXT,"已生成");		
		}else{
			SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,4,ATTR_LABEL_TEXT,"未生成");			
		}
		memset(temp,0,50);
		if(record.m_upload>0)
			Fmt(temp,"%s","已上传");
		else
			Fmt(temp,"%s","未上传");
		SetTreeCellAttribute(reportManagerPanel,PANEL_RECORDTREE,i,5,ATTR_LABEL_TEXT,temp);
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

BOOL onParsePostData(tPostParam *paramPtr,void *callbackData)
{
	if(strcmp(paramPtr->type,"result")==0)
	{
		RESULT itemResult={0};
		int index=atoi(paramPtr->value);
		HashTableGetItem(resultHashTable,&index,&itemResult,sizeof(RESULT));
		memset(paramPtr->value,0,POST_DATA_VALUE);
		sprintf(paramPtr->value,"%s",itemResult.recvString);
	}else if(strcmp(paramPtr->type,"projectName")==0)
	{
		tAutoTestRecord *recordPtr=(tAutoTestRecord *)callbackData;
		memset(paramPtr->value,0,POST_DATA_VALUE);
		sprintf(paramPtr->value,"%s",recordPtr->m_code);
		//sprintf(paramPtr->value,"%s",itemResult.recvString);
	}else if(strcmp(paramPtr->type,"ftpPath")==0)
	{
		tAutoTestRecord *recordPtr=(tAutoTestRecord *)callbackData;
		memset(paramPtr->value,0,POST_DATA_VALUE);
		sprintf(paramPtr->value,"%s",recordPtr->m_reportpath);
		//sprintf(paramPtr->value,"%s",itemResult.recvString);
	}else if(strcmp(paramPtr->type,"username")==0)
    {
		tLoginConfig config = getAccount(); 
		if(strlen(config.userName)!=0)
		{
			memset(paramPtr->value,0,POST_DATA_VALUE);
			sprintf(paramPtr->value,"%s",config.userName);
		}
	}else if(strcmp(paramPtr->type,"passpercent")==0)
	{
		tAutoTestRecord *recordPtr=(tAutoTestRecord *)callbackData; 
		sprintf(paramPtr->value,"%d",recordPtr->m_result);
	}else if(strcmp(paramPtr->type,"pubdate")==0)
	{
		tAutoTestRecord *recordPtr=(tAutoTestRecord *)callbackData; 
		sprintf(paramPtr->value,"%s",recordPtr->m_createtime);
	}

	return TRUE;	
}

BOOL postData(int ftpPanel,tAutoTestRecord record)
{
	BOOL ret=TRUE;
	ListType list=getPostDataSet();
	for(int i=1;i<=ListNumItems(list);i++)
	{
		tPostData data={0};
		char buffer[512]={0}; 
		ListGetItem(list,&data,i);
		if(data.upload == 0 )
			continue;
		Fmt(buffer,"上传数据 %s ",data.name);
		SetCtrlVal(ftpPanel,FTP_TEXTBOX,buffer);
		memset(buffer,0,512);
		buildPostDataStr(data,buffer,onParsePostData,&record);
		if(1!=httpPostJson(data.url,buffer))
		{
			//return FALSE;
			ret = FALSE;
			SetCtrlVal(ftpPanel,FTP_TEXTBOX,"上传失败\n"); 	
		}else{
		   	SetCtrlVal(ftpPanel,FTP_TEXTBOX,"上传成功\n");
		}
	}
	return ret;	
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
		WarnShow1(panel,"未选中任何项目");
		return;
	}
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
					memset(record.m_reportpath,0,250);
					sprintf(record.m_reportpath,"%s",remotePath);
					//httpPost(record.m_code,type,remotePath,"锐速","阿豪",1);
					if(FileExists(record.m_projectpath,NULL)==1)
					{
						if(resultHashTable!=0)
						{
							HashTableDispose(resultHashTable);
						}
						createResultHashTable(&resultHashTable);
						loadResultInfo(record.m_projectpath,NULL,resultHashTable);					
						if(postData(ftpPanel,record))
						{
							updateUpload(atoi(tag),1);
					    	SetCtrlVal(ftpPanel,FTP_TEXTBOX,record.m_reportpath);
							SetCtrlVal(ftpPanel,FTP_TEXTBOX,"\n");
						}else{
							updateUpload(atoi(tag),0);
							SetCtrlVal(ftpPanel,FTP_TEXTBOX,"http post 上传出错\n");
						}
					
						if(resultHashTable!=0)
						{
							HashTableDispose(resultHashTable);
							resultHashTable=0;
						}
					}else{
						SetCtrlVal(ftpPanel,FTP_TEXTBOX,"工程文件丢失\n");
					}                                                           
					
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
		if(FALSE==AlertDialogWithRet(0,"警告","确认删除?","取消","确认"))
		{
			return;
		}		
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
				if(AlertDialogWithRet(panel,"警告","确认打开当前测试","取消","继续")==FALSE)
				{
					return;
				}
				
				if(setProjectPath(record.m_projectpath)==TRUE)
				{
					setTestProjectDbId(atoi(tag));
					unsigned int wParam1=4;   
					unsigned int lParam1=0;   
					PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1);
					QuitUserInterface (0); 
					return;//一定要return，否则会出问题
				}else{
					WarnShow1(panel,"无法打开当前测试！！");
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
	NewCtrlMenuItem(panel, tree, "上传", -1, ReportMenuItemCB, 0);
	NewCtrlMenuItem(panel, tree, "删除", -1, ReportMenuItemCB, 0);
	NewCtrlMenuItem(panel, tree, "打开测试", -1, ReportMenuItemCB, 0);
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
