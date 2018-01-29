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
#include "policyEntity.h"
#include "MainPanel.h"
#include "testGroupInit.h"
#include "common.h"
#include "policyPanel.h"
#include "debug.h"

static int needToSave=0;//默认不需要保存

static int stategyPanel=0; //策略面板
static ListType groupList;

static ListType gCollectList;//集合链表
/*
集合链表在不同时期的作用。
1 在程序启动时，xml中存储的策略信息读取到了集合链表中。
2 在用户操作策略界面时，集合链表负责记录新建或者修改的集合信息，新建的集合会分配一个tag值，修改的集合通过tag值来进行索引。集合链表中的信息可能不完整。
3 存储阶段，通过TREE控件中的信息，以及集合链表生成一个新的集合链表。此时集合链表中的信息完整。
	
	
*/
static void showStrategyOnPanelByTree(void);
static void RefreshSrategyPanel(void);


int isStrategyNeedToSave(void)
{
    return needToSave;
}

/*
 *注意： 1 TREE中有一个tag属性，如果该TREE_ITEM是集合，那么tag表示该集合在集合链表中的序号。
 *        如果该TREE_ITEM是条例组，那么tag表示该组在条例组链表中的序号。  
 */
static void showTestItemOnPanelByTree()
{
   int i=0;
   //TestItem item; 
   TestGroup groupItem;
   char tag[MAX_TREE_ITEM_TAG_LEN]={0};
   ClearListCtrl(stategyPanel,PLCYPANEL_ITEMTREE);//先清空
   for(i=1;i<=ListNumItems(groupList);i++)
   {
	    ListGetItem(groupList,&groupItem,i);
		sprintf(tag,"%d",i);	//用tag来表示存储的集合信息，tag为链表中的存储位置   
		InsertTreeItem (stategyPanel, PLCYPANEL_ITEMTREE, VAL_SIBLING,0, VAL_LAST,
                    groupItem.nickName,tag, 0, i-1);
		SetTreeItemAttribute(stategyPanel, PLCYPANEL_ITEMTREE,i-1 ,ATTR_ENABLE_DROP,0);
   }
}

static HRESULT ShowSrategyByTree()
{
    Collect collect;
	HRESULT error = 0;
	char tag[MAX_TREE_ITEM_TAG_LEN];    
	int count=0,itemCount=0,parent=0;
	count=ListNumItems(gCollectList); 
	for(int i=1;i<=count;i++)
	{
		ListGetItem(gCollectList,&collect,i);
		memset(tag,MAX_TREE_ITEM_TAG_LEN,0);
		sprintf(tag,"%d",i);//TREE中有一个tag属性，如果该TREE_ITEM是集合，那么tag表示该集合在集合链表中的序号。
		                    //如果该TREE_ITEM是条例组，那么tag表示该组在条例组链表中的序号。
		//新建的集合，设置属性不能够拖动
	    InsertTreeItem (stategyPanel, PLCYPANEL_COLLECTTREE, VAL_SIBLING,0, VAL_LAST,
                    collect.name_, tag, 0, 0);
		GetNumListItems(stategyPanel, PLCYPANEL_COLLECTTREE,&itemCount);
		SetTreeItemAttribute(stategyPanel, PLCYPANEL_COLLECTTREE,itemCount-1 ,ATTR_ENABLE_DRAG,0);
		for(int j=1;j<=ListNumItems(collect.groups);j++)
		{
		    //注意：
	    	//1 tree组件从序号0开始,ListType从1开始 
		    //2看链表上最后一个节点是否有父节点，如果没有下一个节点就以该节点为父节点，如果有，下一个节点就以该节点的父节点为父节点
		    //TestItem item;
		    TestGroup groupItem;
			int itemId;
	    	ListGetItem(collect.groups,&itemId,j);
	    	GetNumListItems(stategyPanel, PLCYPANEL_COLLECTTREE,&itemCount);
	    	GetTreeItemParent(stategyPanel, PLCYPANEL_COLLECTTREE,itemCount-1,&parent);
	    	if(itemId>ListNumItems(groupList))
	    	{//获取的条例ID大于条例链表的个数，说明这个策略配置可能已经过时
		    	error=-1;
		    	return error;
		    }	   
		    ListGetItem(groupList,&groupItem,itemId); 
		    sprintf(tag,"%d",itemId);
		    if(parent==-1)
		    {
	    	    InsertTreeItem (stategyPanel, PLCYPANEL_COLLECTTREE, VAL_CHILD,itemCount-1, VAL_LAST,
                groupItem.nickName,tag, 0, 0);
		    }else{
	    	   InsertTreeItem (stategyPanel, PLCYPANEL_COLLECTTREE, VAL_SIBLING,itemCount-1, VAL_NEXT,
                        groupItem.nickName, tag, 0, 0);
		   }	
    	}	
	}	
	return error;
}

static void RefreshSrategyPanel(void)
{
   ClearListCtrl(stategyPanel,PLCYPANEL_COLLECTTREE);  //清除控件内容
   showStrategyOnPanelByTree();
}

static void freeCollectList(ListType collectList)
{
	 for(int index=1;index<=ListNumItems(collectList);index++)
	 {
		   Collect collect;
		   ListGetItem(collectList,&collect,index);
	       ListDispose(collect.groups);
	 }
	 ListDispose(collectList);
}

static void showStrategyOnPanelByTree(void)
{
	ClearListCtrl(stategyPanel,PLCYPANEL_COLLECTTREE);//先清空
	if(ShowSrategyByTree()<0)
	{
	    //最好能进行提示并且清除策略链表以及策略文件
		WarnShow1(0,"策略过时或者出错，请重新分配!");
		ClearListCtrl(stategyPanel,PLCYPANEL_COLLECTTREE);//清空
		freeCollectList(gCollectList);
		gCollectList=ListCreate(sizeof(Collect));
		
		saveStrategy(stategyPanel,gCollectList);//清空文件
	    RefreshSrategyPanel();//刷新面板
	}	
}

void DisplayStrategyPanelAsTabPanel(int panelHandle)
{
	 stategyPanel = LoadPanel (panelHandle, "policyPanel.uir", PLCYPANEL); 
	 DisplayPanel(stategyPanel);
	 groupList=getItemList();
	 showTestItemOnPanelByTree();//显示测试条例组
	 gCollectList=getStrategy();  //从数据库中获取策略
	 showStrategyOnPanelByTree(); //显示策略

}


/*
 * 将TREE中的信息以及集合链表信息，同步成一个新的集合
   原因：因为TREE控件进行了操作，而集合链表中的信息却没有更。
   操作：如果要将TREE控件中的信息体现在集合链表中，执行此函数。
 */
static ListType refreshCollectList()
{
	int itemCount=0,parent=-1,firstLoop=1,tagInt=0;
	Collect tempCollect={0},newCollect={0};  
	char tag[MAX_TREE_ITEM_TAG_LEN]={0};
	char temp[100]={0};
	ListType newCollectList=ListCreate(sizeof(Collect));
	GetNumTreeItems(stategyPanel,PLCYPANEL_COLLECTTREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&itemCount);
	for(int index=0;index<itemCount;index++) //控件TREE序号是从0开始的
	{
	   memset(tag,MAX_TREE_ITEM_TAG_LEN,0);
	   GetTreeItemParent(stategyPanel,PLCYPANEL_COLLECTTREE,index,&parent);
	   GetTreeItemTag(stategyPanel, PLCYPANEL_COLLECTTREE,index,tag);
	   tagInt=atoi(tag);    
	   if(parent==-1)
	   {
		   if(firstLoop==1)  //是否为第一个循环，第一个循环由于集合还没有建立，所有不插入到链表中
		   {
		       firstLoop=0;  
		   }else
		   {
		       ListInsertItem(newCollectList,&newCollect,END_OF_LIST); //不为第一个循环，插入到链表中 
		   }
		   memset(&newCollect,0,sizeof(Collect)); 
		   newCollect.groups=ListCreate(sizeof(int));   
		   ListGetItem(gCollectList,&tempCollect,tagInt);
		   //newCollect.name = (char*)malloc(strlen(tempCollect.name)+1);   //加1防止乱码 

		   memset(newCollect.name_,0,COLLECT_NAME_LEN);
		   memcpy(newCollect.name_,tempCollect.name_,strlen(tempCollect.name_)+1);  

		   newCollect.tType=tempCollect.tType;
		   newCollect.testType=tempCollect.testType;
		   newCollect.maxParallelDevice=tempCollect.maxParallelDevice;
		   newCollect.retryCount=tempCollect.retryCount;
		   newCollect.tipFlag = tempCollect.tipFlag;
		   newCollect.tipsCloseTime=tempCollect.tipsCloseTime;
	   }else{
	       //添加组ID到集合里面
		   if(newCollect.groups!=0)
		   		ListInsertItem(newCollect.groups,&tagInt,END_OF_LIST);  
	   }
	}
	if(firstLoop==0)  
	{	
	     ListInsertItem(newCollectList,&newCollect,END_OF_LIST);   //防止只有一个集合，保存的时候，集合不添加进链表中 
	}
	freeCollectList(gCollectList);
	return newCollectList;
	
}

int CVICALLBACK OnSaveStrategy (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	unsigned int wParam1=0;
    unsigned int lParam1=0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			
			 gCollectList=refreshCollectList();
			// printfCollectList(gCollectList);
			//保存成XML
		     saveStrategy(stategyPanel,gCollectList);
			 RefreshSrategyPanel();
			 needToSave=0;//不需要保存
			break;
	}
	return 0;
}

static void AddCollect(int panel)
{
	char temp[100]={0};
	char tag[MAX_TREE_ITEM_TAG_LEN]={0}; 
	Collect collect;
	TESTTYPE selectedType;
	ENUM_TEST_TYPE testType;
	int itemCount;
	int count;																		    
	//名称
   	GetCtrlVal(panel,COL_DETAIL_NAME,temp);
	//collect.name=(char *)malloc(strlen(temp)+1);
	memset(collect.name_,0,COLLECT_NAME_LEN);
	memcpy(collect.name_,temp,strlen(temp)+1);
	memset(temp,100,0);
	//测试类型
	GetCtrlIndex(panel,COL_DETAIL_TESTTYPE,&selectedType);
	collect.testType=selectedType;
	//数据源
	GetCtrlIndex(panel,COL_DETAIL_DERIVETYPE,&testType);
	collect.tType=testType;
	//最大支持设备数
   	GetCtrlVal(panel,COL_DETAIL_MAXRUNDEVICE,temp);																													  
	collect.maxParallelDevice=atoi(temp);
	memset(temp,100,0);
	//重测次数
   	GetCtrlVal(panel,COL_DETAIL_RETRYSTRING,temp);																													  
	collect.retryCount=atoi(temp);
	//tipFlag	
	//memset(temp,100,0);	
	GetCtrlVal(panel,COL_DETAIL_TIP_MENTION,&collect.tipFlag);
	//collect.tipFlag = atoi(temp);
	
	//tipCloseTime
	GetCtrlVal(panel,COL_DETAIL_TIP_CLOSE_TIME,&collect.tipsCloseTime);
	
	
	
	
	collect.groups=ListCreate(sizeof(TestItem));
	//CollectListInsertItem(&collect); 
	ListInsertItem(gCollectList,&collect,END_OF_LIST);
	count=ListNumItems(gCollectList);//GetCollectListItemCount();
	sprintf(tag,"%d",count);//用tag来表示存储的集合信息，tag为链表中的存储位置
	//新建的集合，设置属性不能够拖动
	InsertTreeItem (stategyPanel, PLCYPANEL_COLLECTTREE, VAL_SIBLING,0, VAL_LAST,
                    collect.name_, tag, 0, 0);
	GetNumListItems(stategyPanel, PLCYPANEL_COLLECTTREE,&itemCount);
	SetTreeItemAttribute(stategyPanel, PLCYPANEL_COLLECTTREE,itemCount-1 ,ATTR_ENABLE_DRAG,0);
}

static void ShowAddNewCollect()
{
	BOOL ret=FALSE;
    int ctrl=0, quit=0;
	int testPanel;
	//InstallPopup (panelHandle);
	
	testPanel=LoadPanel (0, "PolicyPanel.uir", COL_DETAIL); 
	InstallPopup(testPanel);
    while (!quit) {
        GetUserEvent (0, &testPanel, &ctrl);
        if (ctrl == COL_DETAIL_NEWUUTSURE) {
             
			 AddCollect(testPanel);
			 needToSave=1;//需要保存
             quit=1;
			 DiscardPanel (testPanel);
			 ret=FALSE;			//增加
           }else if(ctrl==COL_DETAIL_NEWUUTCANCEL)
	       {
              DiscardPanel (testPanel);
              quit=1;
			  ret=TRUE;			//取消
				    
			}	
        }
}

int CVICALLBACK OnAddCollect (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			
			ShowAddNewCollect();
			break;
	}
	return 0;
}

int CVICALLBACK OnAddTestItemToCollect (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int sourceIndex;
	int destinationIndex;
	int parent;
	int destinationCount;
	
	switch (event)
	{
		case EVENT_COMMIT:
			needToSave=1;
			GetNumTreeItems(stategyPanel,PLCYPANEL_COLLECTTREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&destinationCount);
			if(destinationCount!=0)
			{	
			GetActiveTreeItem(stategyPanel,PLCYPANEL_ITEMTREE,&sourceIndex);
			GetActiveTreeItem(stategyPanel,PLCYPANEL_COLLECTTREE,&destinationIndex);
			GetTreeItemParent(stategyPanel,PLCYPANEL_COLLECTTREE,destinationIndex,&parent); 
//			PRINT("destinationIndex:%d,parent:%d",destinationIndex,parent);
			if(parent!=-1)
			{
//			     destinationIndex=parent;
				CopyTreeItem (stategyPanel, PLCYPANEL_ITEMTREE,sourceIndex,stategyPanel, PLCYPANEL_COLLECTTREE,
                	VAL_SIBLING, destinationIndex, VAL_NEXT);					
			}else{
				CopyTreeItem (stategyPanel, PLCYPANEL_ITEMTREE,sourceIndex,stategyPanel, PLCYPANEL_COLLECTTREE,
                	VAL_CHILD, destinationIndex, VAL_LAST);				
			}


			}else
			{
			    MessagePopup("警告","当前没有可用的集合！");
			}	
			break;
	}
	return 0;
}

int CVICALLBACK OnDelete (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int selectedIndex;
	int parent;
	int childNum;
	int index;
	 
	switch (event)
	{
		case EVENT_COMMIT:
			needToSave=1;
			GetActiveTreeItem (stategyPanel, PLCYPANEL_COLLECTTREE, &selectedIndex);
			DeleteListItem(stategyPanel, PLCYPANEL_COLLECTTREE,selectedIndex,1);   
			break;
	}
	return 0;
}



static void SaveCollectInfo(int panel)
{
	int selectedIndex;
	int parent;  
    char tag[MAX_TREE_ITEM_TAG_LEN];
	Collect collect; 
	char temp[100]={0}; 
	TESTTYPE selectedType;
	ENUM_TEST_TYPE testType;
	GetActiveTreeItem (stategyPanel, PLCYPANEL_COLLECTTREE, &selectedIndex);  
	GetTreeItemParent(stategyPanel,PLCYPANEL_COLLECTTREE,selectedIndex,&parent);
	if(parent!=-1)
	{
		selectedIndex=parent;
	}else
	{
		parent=selectedIndex;
	}	
	//printf("----1\n");
	//printfCollectList(gCollectList);
	
	GetTreeItemTag(stategyPanel, PLCYPANEL_COLLECTTREE,selectedIndex,tag);
	selectedIndex=atoi(tag);
	//获取选中的集合
	
	ListGetItem(gCollectList,&collect,selectedIndex);
			
	//获取名称
   	GetCtrlVal(panel,COL_DETAIL_NAME,temp);
	/*if(strlen(collect.name_)>0)
	{
	    free(collect.name);
	}	
	collect.name=(char *)malloc(strlen(temp)+1);*/
	memset(collect.name_,0,COLLECT_NAME_LEN);
	memcpy(collect.name_,temp,strlen(temp)+1);

	//获取测试类型
	GetCtrlIndex(panel,COL_DETAIL_TESTTYPE,&selectedType);
	collect.testType=selectedType;
	//获取数据源
	GetCtrlIndex(panel,COL_DETAIL_DERIVETYPE,&testType);
	collect.tType=testType;
	//获取最大支持设备数
	memset(temp,100,0); 
   	GetCtrlVal(panel,COL_DETAIL_MAXRUNDEVICE,temp);
	collect.maxParallelDevice=atoi(temp);
	//获取重测次数
	memset(temp,100,0); 
   	GetCtrlVal(panel,COL_DETAIL_RETRYSTRING,temp);
	collect.retryCount=atoi(temp);	
	
	//memset(temp,100,0);	
	GetCtrlVal(panel,COL_DETAIL_TIP_MENTION,&collect.tipFlag);
	//collect.tipFlag = atoi(temp);
	
	//tipCloseTime
	GetCtrlVal(panel,COL_DETAIL_TIP_CLOSE_TIME,&collect.tipsCloseTime);	
	//替换更改后的集合
	//CollectListReplaceItem(&collect,selectedIndex);
	ListReplaceItem(gCollectList,&collect,selectedIndex);		
			
	//如果名字更改了在界面上替换更改后的名字
	SetTreeCellAttribute(stategyPanel, PLCYPANEL_COLLECTTREE,parent,0,ATTR_LABEL_TEXT,collect.name_);
	//printf("----2\n");
	//printfCollectList(gCollectList);
}
static void ShowCollectInfo(Collect collect)
{
	char temp[100]={0};
	BOOL ret=FALSE;
    int ctrl=0, quit=0;
	int testPanel;
	//InstallPopup (panelHandle);
	
	int buttonFlag=0;//修改
	
	 testPanel=LoadPanel (0, "PolicyPanel.uir", COL_DETAIL);
	
	 SetCtrlVal(testPanel,COL_DETAIL_NAME,collect.name_);
	 SetCtrlIndex(testPanel,COL_DETAIL_TESTTYPE,collect.testType);
     SetCtrlIndex(testPanel,COL_DETAIL_DERIVETYPE,collect.tType);
	 sprintf(temp,"%d",collect.maxParallelDevice);
	 SetCtrlVal(testPanel,COL_DETAIL_MAXRUNDEVICE,temp);
	 memset(temp,0,100);
	 sprintf(temp,"%d",collect.retryCount);
	 SetCtrlVal(testPanel,COL_DETAIL_RETRYSTRING,temp);	 
	 
	 //memset(temp,0,100);
	 //sprintf(temp,"%d",collect.tipFlag);
	 SetCtrlVal(testPanel,COL_DETAIL_TIP_MENTION,collect.tipFlag);	 
	 
	 //memset(temp,0,100);
	 //sprintf(temp,"%d",collect.tipsCloseTime);
	 SetCtrlVal(testPanel,COL_DETAIL_TIP_CLOSE_TIME,collect.tipsCloseTime);	 	 

     SetCtrlAttribute(testPanel,COL_DETAIL_NAME,ATTR_DIMMED,1);
	 SetCtrlAttribute(testPanel,COL_DETAIL_TESTTYPE,ATTR_DIMMED,1);
	 SetCtrlAttribute(testPanel,COL_DETAIL_DERIVETYPE,ATTR_DIMMED,1);
	 SetCtrlAttribute(testPanel,COL_DETAIL_MAXRUNDEVICE,ATTR_DIMMED,1);
	 SetCtrlAttribute(testPanel,COL_DETAIL_RETRYSTRING,ATTR_DIMMED,1);
	 SetCtrlAttribute(testPanel,COL_DETAIL_TIP_MENTION,ATTR_DIMMED,1); 
	 SetCtrlAttribute(testPanel,COL_DETAIL_TIP_CLOSE_TIME,ATTR_DIMMED,1); 	 
	 
	 SetCtrlAttribute (testPanel, COL_DETAIL_NEWUUTSURE, ATTR_LABEL_TEXT ,"修改"); 
	 InstallPopup(testPanel);
     while (!quit) {
        GetUserEvent (0, &testPanel, &ctrl);
        if (ctrl == COL_DETAIL_NEWUUTSURE) {
			
			if(buttonFlag==0)//修改
			{
			    buttonFlag=1;//确定
                SetCtrlAttribute(testPanel,COL_DETAIL_NAME,ATTR_DIMMED,0);
	            SetCtrlAttribute(testPanel,COL_DETAIL_TESTTYPE,ATTR_DIMMED,0);
	            SetCtrlAttribute(testPanel,COL_DETAIL_DERIVETYPE,ATTR_DIMMED,0);
				
	            SetCtrlAttribute(testPanel,COL_DETAIL_MAXRUNDEVICE,ATTR_DIMMED,0);
				SetCtrlAttribute(testPanel,COL_DETAIL_RETRYSTRING,ATTR_DIMMED,0); 
				SetCtrlAttribute(testPanel,COL_DETAIL_TIP_MENTION,ATTR_DIMMED,0); 
	 			SetCtrlAttribute(testPanel,COL_DETAIL_TIP_CLOSE_TIME,ATTR_DIMMED,0); 
	 
	            SetCtrlAttribute (testPanel, COL_DETAIL_NEWUUTSURE, ATTR_LABEL_TEXT ,"确定"); 
			}else if(buttonFlag==1)//确定
			{
			   //TODO:保存修改信息
			   SaveCollectInfo(testPanel);
			   //todo:刷新列表
			   
			   needToSave=1;//需要保存  
			   buttonFlag=0;//修改
               
			   SetCtrlAttribute(testPanel,COL_DETAIL_NAME,ATTR_DIMMED,1); 
	           SetCtrlAttribute(testPanel,COL_DETAIL_TESTTYPE,ATTR_DIMMED,1);
	           SetCtrlAttribute(testPanel,COL_DETAIL_DERIVETYPE,ATTR_DIMMED,1);
	           SetCtrlAttribute(testPanel,COL_DETAIL_MAXRUNDEVICE,ATTR_DIMMED,1);
			   SetCtrlAttribute(testPanel,COL_DETAIL_RETRYSTRING,ATTR_DIMMED,1); 
			   SetCtrlAttribute(testPanel,COL_DETAIL_TIP_MENTION,ATTR_DIMMED,1); 
	 		   SetCtrlAttribute(testPanel,COL_DETAIL_TIP_CLOSE_TIME,ATTR_DIMMED,1); 			   
	 
	           SetCtrlAttribute (testPanel, COL_DETAIL_NEWUUTSURE, ATTR_LABEL_TEXT ,"修改"); 
			   
			   quit=1; 
			   DiscardPanel (testPanel);
			
			}	
           }else if(ctrl==COL_DETAIL_NEWUUTCANCEL)
	       {
              DiscardPanel (testPanel);
              quit=1;
			  ret=TRUE;			//取消
			}	
        }
}


int CVICALLBACK OnCollectClick (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int selectedIndex;
	int parent;  
    char tag[MAX_TREE_ITEM_TAG_LEN];
	Collect collect;
	switch (event)
	{
		case EVENT_LEFT_DOUBLE_CLICK:
			GetActiveTreeItem (stategyPanel, PLCYPANEL_COLLECTTREE, &selectedIndex);  
			GetTreeItemParent(stategyPanel,PLCYPANEL_COLLECTTREE,selectedIndex,&parent);
			if(parent!=-1)
			{
			    selectedIndex=parent;
			}else
			{
			    parent=selectedIndex;
			}	
			GetTreeItemTag(stategyPanel, PLCYPANEL_COLLECTTREE,selectedIndex,tag);
			selectedIndex=atoi(tag);
			//CollectListGetItem(&collect,selectedIndex);
			ListGetItem(gCollectList,&collect,selectedIndex);
			ShowCollectInfo(collect);
			break;
	}
	return 0;
}


static void showItemInfo(TestGroup group)
{
	char temp[50]={0};
	int panelHandle;
	TestItem item;
	int ctrl=0, quit=0;   
	panelHandle=LoadPanel (0, "PolicyPanel.uir", PANEL); 
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		ListGetItem(group.subItems,&item,i);  
		InsertTreeItem (panelHandle, PANEL_TREE, VAL_SIBLING,0, VAL_LAST,
                    item.itemName_,NULL, 0, i-1);
		
		memset(temp,0,50);
		if(item.paramId>=0)
		{
		    sprintf(temp,"%d",item.paramId);
		}else
		{
		    sprintf(temp,"%s","无");
		}	
		//SetTreeCellAttribute(panelHandle,PANEL_TREE,i-1,1, ATTR_LABEL_TEXT,temp); //id
		SetTreeCellAttribute(panelHandle,PANEL_TREE,i-1,1, ATTR_LABEL_TEXT,item.inputValue_); //inputValue
		//item.
		
        SetTreeCellAttribute(panelHandle,PANEL_TREE,i-1,2, ATTR_LABEL_TEXT,item.standard_);  //standardValue		
	}
	SetCtrlAttribute(panelHandle, PANEL_TREE,ATTR_LABEL_TEXT,group.nickName);  
	InstallPopup(panelHandle);
	while (!quit) {
		GetUserEvent (0, &panelHandle, &ctrl); 
		if (ctrl == PANEL_SURE) 
		{
			quit=1;
			break;
		}	
	}
	DiscardPanel (panelHandle); 
}

int CVICALLBACK ONCLICKITEMTREE (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
    char tag[MAX_TREE_ITEM_TAG_LEN];     
	int sourceIndex;
	switch (event)
	{
		case EVENT_LEFT_DOUBLE_CLICK:
			GetActiveTreeItem(panel,PLCYPANEL_ITEMTREE,&sourceIndex);
			GetTreeItemTag(stategyPanel, PLCYPANEL_ITEMTREE,sourceIndex,tag);
			sourceIndex=atoi(tag);
			TestGroup group;  
			ListGetItem(groupList,&group,sourceIndex);
			showItemInfo(group);
			break;
		case EVENT_RIGHT_DOUBLE_CLICK:
			int sourceIndex;
	        int destinationIndex;
	        int parent;
	        int destinationCount;
			needToSave=1;
			GetNumTreeItems(stategyPanel,PLCYPANEL_COLLECTTREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&destinationCount);
			if(destinationCount!=0)
			{	
			GetActiveTreeItem(stategyPanel,PLCYPANEL_ITEMTREE,&sourceIndex);
			GetActiveTreeItem(stategyPanel,PLCYPANEL_COLLECTTREE,&destinationIndex);
			GetTreeItemParent(stategyPanel,PLCYPANEL_COLLECTTREE,destinationIndex,&parent); 
			if(parent!=-1)
			{
			     destinationIndex=parent;
			}	
			CopyTreeItem (stategyPanel, PLCYPANEL_ITEMTREE,sourceIndex,stategyPanel, PLCYPANEL_COLLECTTREE,
                 VAL_CHILD, destinationIndex, VAL_LAST);
			}else
			{
			    MessagePopup("警告","当前没有可用的集合！");
			}	
			break;
			break;
	}
	return 0;
}

ListType GetCollectList(void)
{
   return gCollectList;
}
