#include "EutConfigPanel.h"
#include <cvirte.h>		
#include <userint.h>
#include "EutManage.h"
#include "eutEntity.h"

static int panelHandle;
static EEUT selectedEut;
static ListType generateEutList=0;

ListType getGenerteEutList()
{
	return generateEutList;
}

void initEutConfigPanel()
{
	ListType list=getEEutList();
	ClearListCtrl(panelHandle,PANEL_TREE);
	
	if(generateEutList!=0)
	{
		ListDispose(generateEutList);		
	}
	
	generateEutList=ListCreate(sizeof(EUT));

	for(int i=1;i<=ListNumItems(list);i++)
	{
		EEUT eeut={0};
		EUT eut={0};
		ListGetItem(list,&eeut,i);
		
		for(int j=1;j<=ListNumItems(eeut.configList);j++)
		{
			ConfigItem configItem={0};
			ListGetItem(eeut.configList,&configItem,j);
			if(strcmp(configItem.tagName,"»ù±¾ÅäÖÃ")==0)
			{
				EutBasicInfo *info=(EutBasicInfo *)configItem.configPtrHandle;
				eut.index = info->id;
				sprintf(eut.eutName,"%s",info->eutName);
				eut.enable=info->enable;
				
				char temp[30]={0};
				sprintf(temp,"±àºÅ£º%d",info->id);
	    		InsertTreeItem (panelHandle, PANEL_TREE, VAL_SIBLING,0, VAL_LAST,
                    temp,0,0,0);
	    		SetTreeCellAttribute(panelHandle,PANEL_TREE,i-1,1, ATTR_LABEL_TEXT,info->eutName);	
			}else if(strcmp(configItem.tagName,"°²¹æ²âÁ¿ÒÇ")==0)
			{
				RSCONFIG *config=(RSCONFIG *)configItem.configPtrHandle;
				memcpy(&eut.matainConfig,config,sizeof(RSCONFIG));
			}else if(strcmp(configItem.tagName,"¼ÌµçÆ÷")==0)
			{
				RSCONFIG *config=(RSCONFIG *)configItem.configPtrHandle;
				memcpy(&eut.relayConfig,config,sizeof(RSCONFIG));
			}else if(strcmp(configItem.tagName,"BMSÄ£ÄâÆ÷")==0)
			{
				RSCONFIG *config=(RSCONFIG *)configItem.configPtrHandle;
				memcpy(&eut.bmsConfig,config,sizeof(RSCONFIG));
			}else if(strcmp(configItem.tagName,"³äµç×®ÍøÂç")==0)
			{
				NETCONFIG *config=(NETCONFIG *)configItem.configPtrHandle;
				memcpy(&eut.chargingPile,config,sizeof(NETCONFIG));
			}
		}
		ListInsertItem(generateEutList,&eut,END_OF_LIST);
	}	
	
	/*for(int i=1;i<=ListNumItems(list);i++)
	{
	
		EEUT eeut={0};
		ListGetItem(list,&eeut,i);
		ConfigItem configItem={0};
		ListGetItem(eeut.configList,&configItem,1);
		EutBasicInfo *info=(EutBasicInfo *)configItem.configPtrHandle;
		char temp[30]={0};
		sprintf(temp,"±àºÅ£º%d",info->id);
	    InsertTreeItem (panelHandle, PANEL_TREE, VAL_SIBLING,0, VAL_LAST,
                    temp,0,0,0);
	    SetTreeCellAttribute(panelHandle,PANEL_TREE,i-1,1, ATTR_LABEL_TEXT,info->eutName);		
		//char temp[20]={0};
		//sprintf(temp,"EEUT:%d",i);
		//InsertListItem(panelHandle,PANEL_LISTBOX,i-1,temp,0);  
	}*/
	
}

int DisplayEutManagePanel(int panel)
{
	//if (InitCVIRTE (0, argv, 0) == 0)
	//	return -1;	/* out of memory */
	if ((panelHandle = LoadPanel (panel, "EutConfigPanel.uir", PANEL)) < 0)
		return -1;
	//InitEutConfig();
	initEutConfigPanel();
	//saveEutList();
	DisplayPanel (panelHandle);
	//RunUserInterface ();
	//DiscardPanel (panelHandle);
	return 0;
}


void showConfigList(EEUT eut,int panelHandle)
{
	ListType list=eut.configList;
	for(int i=1;i<=ListNumItems(list);i++)
	{
		ConfigItem item={0};
		ListGetItem(list,&item,i);
		tActivity ta={0};
		getActivity(item.tagName,&ta);
		int panelHandle1=(*(LOAD_CONFIG_PANEL)(ta.activity.onLoadPanel))(item.configPtrHandle);//(*LOAD_CONFIG_PANEL(activity.loadPanel))();
		//DisplayPanel(panelHandle);
		InsertListItem(panelHandle,EUTPANEL_LISTBOX,i-1,item.tagName,0);
		InsertPanelAsTabPage(panelHandle,EUTPANEL_TAB,i-1,panelHandle1);  
	}
}

void showEutConfig(EEUT eut)
{
	int panelHandle = LoadPanel (0, "EutConfigPanel.uir", EUTPANEL);
	SetCtrlAttribute(panelHandle,EUTPANEL_TAB,ATTR_TABS_VISIBLE,0);
	showConfigList(eut,panelHandle);
 	DisplayPanel(panelHandle);
}

void RefreshSelectedEutConfig(int panelHandle)
{
	ListType list=selectedEut.configList;
	for(int i=1;i<=ListNumItems(list);i++)
	{
		ConfigItem item;
		ListGetItem(list,&item,i);
		int configPanel;
		//ConfigActivity activity;
		tActivity ta;
		GetPanelHandleFromTabPage(panelHandle,EUTPANEL_TAB,i-1,&configPanel);
		getActivity(item.tagName,&ta);
		(*(GET_CHANGE_CONFIG)(ta.activity.onPanelChange))(configPanel,item.configPtrHandle);
	}
}

int CVICALLBACK onClickEutConfigCtrl (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		
		case EVENT_COMMIT:
			if(control== PANEL_TREE)
			{
				int selected=0;
				/*GetCtrlIndex(panel,control,&selected);
				ListType list=getEutList();
				ListGetItem(list,&selectedEut,selected+1);*/
				ListType list=getEEutList(); 
				GetActiveTreeItem(panel,control,&selected);
				ListGetItem(list,&selectedEut,selected+1);
				showEutConfig(selectedEut);
				
			}else if(control==PANEL_SAVE)
			{
				saveEutList();	
			}else if(control==PANEL_COMMANDBUTTON)
			{
				QuitUserInterface(1);
			}else if(control==PANEL_ADD)
			{
				//memset(&selectedEut,0,sizeof(EEUT));
				addNewToEutList();
				initEutConfigPanel();
				
			}
			break;
	}
	return 0;
}

int CVICALLBACK onClickEutSubConfigCtrl (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:

			if(control== EUTPANEL_SURE)
			{
				RefreshSelectedEutConfig(panel);
				DiscardPanel(panel); 
				initEutConfigPanel();
				return 0;
			}else if(control==EUTPANEL_LISTBOX);
			{
				int selected=0;
				if(panel>0)
				{	
					GetCtrlIndex(panel,EUTPANEL_LISTBOX,&selected);
					SetActiveTabPage(panel,EUTPANEL_TAB,selected);
				}
			}
			
			if(control == EUTPANEL_COMMANDBUTTON_2)
			{
				DiscardPanel(panel);
			}
			break;
		case EVENT_VAL_CHANGED:
			if(control==EUTPANEL_LISTBOX);
			{
				int selected=0;
				if(panel>0)
				{	
					GetCtrlIndex(panel,EUTPANEL_LISTBOX,&selected);
					SetActiveTabPage(panel,EUTPANEL_TAB,selected);
				}
			}
			break;
	}
	return 0;
}

int CVICALLBACK OnEutDelete (int panel, int control, int event,
						  void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			int selected=0;
			GetCtrlIndex(panel,PANEL_TREE,&selected);
			//printf("%d\n",selected);
			ListType list=getEEutList(); 
			//printf("%d\n",ListNumItems(list));
			EEUT eeut;
			ListRemoveItem(list,&eeut,selected+1);
			initEutConfigPanel();
			
			
			break;
	}
	return 0;
}


