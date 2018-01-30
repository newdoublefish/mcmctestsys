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
#include "policyEntity.h"
#include "MainPanel.h"
#include "testGroupInit.h"
#include "common.h"
#include "policyPanel.h"
#include "debug.h"

static int needToSave=0;//Ĭ�ϲ���Ҫ����

static int stategyPanel=0; //�������
static ListType groupList;

static ListType gCollectList;//��������
/*
���������ڲ�ͬʱ�ڵ����á�
1 �ڳ�������ʱ��xml�д洢�Ĳ�����Ϣ��ȡ���˼��������С�
2 ���û��������Խ���ʱ�������������¼�½������޸ĵļ�����Ϣ���½��ļ��ϻ����һ��tagֵ���޸ĵļ���ͨ��tagֵ���������������������е���Ϣ���ܲ�������
3 �洢�׶Σ�ͨ��TREE�ؼ��е���Ϣ���Լ�������������һ���µļ���������ʱ���������е���Ϣ������
	
	
*/
static void showStrategyOnPanelByTree(void);
static void RefreshSrategyPanel(void);


int isStrategyNeedToSave(void)
{
    return needToSave;
}

/*
 *ע�⣺ 1 TREE����һ��tag���ԣ������TREE_ITEM�Ǽ��ϣ���ôtag��ʾ�ü����ڼ��������е���š�
 *        �����TREE_ITEM�������飬��ôtag��ʾ�����������������е���š�  
 */
static void showTestItemOnPanelByTree()
{
   int i=0;
   //TestItem item; 
   TestGroup groupItem;
   char tag[MAX_TREE_ITEM_TAG_LEN]={0};
   ClearListCtrl(stategyPanel,PLCYPANEL_ITEMTREE);//�����
   for(i=1;i<=ListNumItems(groupList);i++)
   {
	    ListGetItem(groupList,&groupItem,i);
		sprintf(tag,"%d",i);	//��tag����ʾ�洢�ļ�����Ϣ��tagΪ�����еĴ洢λ��   
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
		sprintf(tag,"%d",i);//TREE����һ��tag���ԣ������TREE_ITEM�Ǽ��ϣ���ôtag��ʾ�ü����ڼ��������е���š�
		                    //�����TREE_ITEM�������飬��ôtag��ʾ�����������������е���š�
		//�½��ļ��ϣ��������Բ��ܹ��϶�
	    InsertTreeItem (stategyPanel, PLCYPANEL_COLLECTTREE, VAL_SIBLING,0, VAL_LAST,
                    collect.name_, tag, 0, 0);
		GetNumListItems(stategyPanel, PLCYPANEL_COLLECTTREE,&itemCount);
		SetTreeItemAttribute(stategyPanel, PLCYPANEL_COLLECTTREE,itemCount-1 ,ATTR_ENABLE_DRAG,0);
		for(int j=1;j<=ListNumItems(collect.groups);j++)
		{
		    //ע�⣺
	    	//1 tree��������0��ʼ,ListType��1��ʼ 
		    //2�����������һ���ڵ��Ƿ��и��ڵ㣬���û����һ���ڵ���Ըýڵ�Ϊ���ڵ㣬����У���һ���ڵ���Ըýڵ�ĸ��ڵ�Ϊ���ڵ�
		    //TestItem item;
		    TestGroup groupItem;
			int itemId;
	    	ListGetItem(collect.groups,&itemId,j);
	    	GetNumListItems(stategyPanel, PLCYPANEL_COLLECTTREE,&itemCount);
	    	GetTreeItemParent(stategyPanel, PLCYPANEL_COLLECTTREE,itemCount-1,&parent);
	    	if(itemId>ListNumItems(groupList))
	    	{//��ȡ������ID������������ĸ�����˵������������ÿ����Ѿ���ʱ
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
   ClearListCtrl(stategyPanel,PLCYPANEL_COLLECTTREE);  //����ؼ�����
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
	ClearListCtrl(stategyPanel,PLCYPANEL_COLLECTTREE);//�����
	if(ShowSrategyByTree()<0)
	{
	    //����ܽ�����ʾ����������������Լ������ļ�
		WarnShow1(0,"���Թ�ʱ���߳��������·���!");
		ClearListCtrl(stategyPanel,PLCYPANEL_COLLECTTREE);//���
		freeCollectList(gCollectList);
		gCollectList=ListCreate(sizeof(Collect));
		
		saveStrategy(stategyPanel,gCollectList);//����ļ�
	    RefreshSrategyPanel();//ˢ�����
	}	
}

void DisplayStrategyPanelAsTabPanel(int panelHandle)
{
	 stategyPanel = LoadPanel (panelHandle, "policyPanel.uir", PLCYPANEL); 
	 DisplayPanel(stategyPanel);
	 groupList=getItemList();
	 showTestItemOnPanelByTree();//��ʾ����������
	 gCollectList=getStrategy();  //�����ݿ��л�ȡ����
	 showStrategyOnPanelByTree(); //��ʾ����

}


/*
 * ��TREE�е���Ϣ�Լ�����������Ϣ��ͬ����һ���µļ���
   ԭ����ΪTREE�ؼ������˲����������������е���Ϣȴû�и���
   ���������Ҫ��TREE�ؼ��е���Ϣ�����ڼ��������У�ִ�д˺�����
 */
static ListType refreshCollectList()
{
	int itemCount=0,parent=-1,firstLoop=1,tagInt=0;
	Collect tempCollect={0},newCollect={0};  
	char tag[MAX_TREE_ITEM_TAG_LEN]={0};
	char temp[100]={0};
	ListType newCollectList=ListCreate(sizeof(Collect));
	GetNumTreeItems(stategyPanel,PLCYPANEL_COLLECTTREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&itemCount);
	for(int index=0;index<itemCount;index++) //�ؼ�TREE����Ǵ�0��ʼ��
	{
	   memset(tag,MAX_TREE_ITEM_TAG_LEN,0);
	   GetTreeItemParent(stategyPanel,PLCYPANEL_COLLECTTREE,index,&parent);
	   GetTreeItemTag(stategyPanel, PLCYPANEL_COLLECTTREE,index,tag);
	   tagInt=atoi(tag);    
	   if(parent==-1)
	   {
		   if(firstLoop==1)  //�Ƿ�Ϊ��һ��ѭ������һ��ѭ�����ڼ��ϻ�û�н��������в����뵽������
		   {
		       firstLoop=0;  
		   }else
		   {
		       ListInsertItem(newCollectList,&newCollect,END_OF_LIST); //��Ϊ��һ��ѭ�������뵽������ 
		   }
		   memset(&newCollect,0,sizeof(Collect)); 
		   newCollect.groups=ListCreate(sizeof(int));   
		   ListGetItem(gCollectList,&tempCollect,tagInt);
		   //newCollect.name = (char*)malloc(strlen(tempCollect.name)+1);   //��1��ֹ���� 

		   memset(newCollect.name_,0,COLLECT_NAME_LEN);
		   memcpy(newCollect.name_,tempCollect.name_,strlen(tempCollect.name_)+1);  

		   newCollect.tType=tempCollect.tType;
		   newCollect.testType=tempCollect.testType;
		   newCollect.maxParallelDevice=tempCollect.maxParallelDevice;
		   newCollect.retryCount=tempCollect.retryCount;
		   newCollect.tipFlag = tempCollect.tipFlag;
		   newCollect.tipsCloseTime=tempCollect.tipsCloseTime;
	   }else{
	       //�����ID����������
		   if(newCollect.groups!=0)
		   		ListInsertItem(newCollect.groups,&tagInt,END_OF_LIST);  
	   }
	}
	if(firstLoop==0)  
	{	
	     ListInsertItem(newCollectList,&newCollect,END_OF_LIST);   //��ֹֻ��һ�����ϣ������ʱ�򣬼��ϲ���ӽ������� 
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
			//�����XML
		     saveStrategy(stategyPanel,gCollectList);
			 RefreshSrategyPanel();
			 needToSave=0;//����Ҫ����
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
	//����
   	GetCtrlVal(panel,COL_DETAIL_NAME,temp);
	//collect.name=(char *)malloc(strlen(temp)+1);
	memset(collect.name_,0,COLLECT_NAME_LEN);
	memcpy(collect.name_,temp,strlen(temp)+1);
	memset(temp,100,0);
	//��������
	GetCtrlIndex(panel,COL_DETAIL_TESTTYPE,&selectedType);
	collect.testType=selectedType;
	//����Դ
	GetCtrlIndex(panel,COL_DETAIL_DERIVETYPE,&testType);
	collect.tType=testType;
	//���֧���豸��
   	GetCtrlVal(panel,COL_DETAIL_MAXRUNDEVICE,temp);																													  
	collect.maxParallelDevice=atoi(temp);
	memset(temp,100,0);
	//�ز����
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
	sprintf(tag,"%d",count);//��tag����ʾ�洢�ļ�����Ϣ��tagΪ�����еĴ洢λ��
	//�½��ļ��ϣ��������Բ��ܹ��϶�
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
			 needToSave=1;//��Ҫ����
             quit=1;
			 DiscardPanel (testPanel);
			 ret=FALSE;			//����
           }else if(ctrl==COL_DETAIL_NEWUUTCANCEL)
	       {
              DiscardPanel (testPanel);
              quit=1;
			  ret=TRUE;			//ȡ��
				    
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
			if(control ==  PLCYPANEL_BTN_BELOW)
			{
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
			    MessagePopup("����","��ǰû�п��õļ��ϣ�");
				}
			}else if(control ==  PLCYPANEL_BTN_ABOVE)
			{
			
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
                		VAL_SIBLING, destinationIndex, VAL_PREV);					
					}else{
						CopyTreeItem (stategyPanel, PLCYPANEL_ITEMTREE,sourceIndex,stategyPanel, PLCYPANEL_COLLECTTREE,
                			VAL_CHILD, destinationIndex, VAL_LAST);				
					}


				}else
				{
			    MessagePopup("����","��ǰû�п��õļ��ϣ�");
				}			
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
	//��ȡѡ�еļ���
	
	ListGetItem(gCollectList,&collect,selectedIndex);
			
	//��ȡ����
   	GetCtrlVal(panel,COL_DETAIL_NAME,temp);
	/*if(strlen(collect.name_)>0)
	{
	    free(collect.name);
	}	
	collect.name=(char *)malloc(strlen(temp)+1);*/
	memset(collect.name_,0,COLLECT_NAME_LEN);
	memcpy(collect.name_,temp,strlen(temp)+1);

	//��ȡ��������
	GetCtrlIndex(panel,COL_DETAIL_TESTTYPE,&selectedType);
	collect.testType=selectedType;
	//��ȡ����Դ
	GetCtrlIndex(panel,COL_DETAIL_DERIVETYPE,&testType);
	collect.tType=testType;
	//��ȡ���֧���豸��
	memset(temp,100,0); 
   	GetCtrlVal(panel,COL_DETAIL_MAXRUNDEVICE,temp);
	collect.maxParallelDevice=atoi(temp);
	//��ȡ�ز����
	memset(temp,100,0); 
   	GetCtrlVal(panel,COL_DETAIL_RETRYSTRING,temp);
	collect.retryCount=atoi(temp);	
	
	//memset(temp,100,0);	
	GetCtrlVal(panel,COL_DETAIL_TIP_MENTION,&collect.tipFlag);
	//collect.tipFlag = atoi(temp);
	
	//tipCloseTime
	GetCtrlVal(panel,COL_DETAIL_TIP_CLOSE_TIME,&collect.tipsCloseTime);	
	//�滻���ĺ�ļ���
	//CollectListReplaceItem(&collect,selectedIndex);
	ListReplaceItem(gCollectList,&collect,selectedIndex);		
			
	//������ָ������ڽ������滻���ĺ������
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
	
	int buttonFlag=0;//�޸�
	
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
	 
	 SetCtrlAttribute (testPanel, COL_DETAIL_NEWUUTSURE, ATTR_LABEL_TEXT ,"�޸�"); 
	 InstallPopup(testPanel);
     while (!quit) {
        GetUserEvent (0, &testPanel, &ctrl);
        if (ctrl == COL_DETAIL_NEWUUTSURE) {
			
			if(buttonFlag==0)//�޸�
			{
			    buttonFlag=1;//ȷ��
                SetCtrlAttribute(testPanel,COL_DETAIL_NAME,ATTR_DIMMED,0);
	            SetCtrlAttribute(testPanel,COL_DETAIL_TESTTYPE,ATTR_DIMMED,0);
	            SetCtrlAttribute(testPanel,COL_DETAIL_DERIVETYPE,ATTR_DIMMED,0);
				
	            SetCtrlAttribute(testPanel,COL_DETAIL_MAXRUNDEVICE,ATTR_DIMMED,0);
				SetCtrlAttribute(testPanel,COL_DETAIL_RETRYSTRING,ATTR_DIMMED,0); 
				SetCtrlAttribute(testPanel,COL_DETAIL_TIP_MENTION,ATTR_DIMMED,0); 
	 			SetCtrlAttribute(testPanel,COL_DETAIL_TIP_CLOSE_TIME,ATTR_DIMMED,0); 
	 
	            SetCtrlAttribute (testPanel, COL_DETAIL_NEWUUTSURE, ATTR_LABEL_TEXT ,"ȷ��"); 
			}else if(buttonFlag==1)//ȷ��
			{
			   //TODO:�����޸���Ϣ
			   SaveCollectInfo(testPanel);
			   //todo:ˢ���б�
			   
			   needToSave=1;//��Ҫ����  
			   buttonFlag=0;//�޸�
               
			   SetCtrlAttribute(testPanel,COL_DETAIL_NAME,ATTR_DIMMED,1); 
	           SetCtrlAttribute(testPanel,COL_DETAIL_TESTTYPE,ATTR_DIMMED,1);
	           SetCtrlAttribute(testPanel,COL_DETAIL_DERIVETYPE,ATTR_DIMMED,1);
	           SetCtrlAttribute(testPanel,COL_DETAIL_MAXRUNDEVICE,ATTR_DIMMED,1);
			   SetCtrlAttribute(testPanel,COL_DETAIL_RETRYSTRING,ATTR_DIMMED,1); 
			   SetCtrlAttribute(testPanel,COL_DETAIL_TIP_MENTION,ATTR_DIMMED,1); 
	 		   SetCtrlAttribute(testPanel,COL_DETAIL_TIP_CLOSE_TIME,ATTR_DIMMED,1); 			   
	 
	           SetCtrlAttribute (testPanel, COL_DETAIL_NEWUUTSURE, ATTR_LABEL_TEXT ,"�޸�"); 
			   
			   quit=1; 
			   DiscardPanel (testPanel);
			
			}	
           }else if(ctrl==COL_DETAIL_NEWUUTCANCEL)
	       {
              DiscardPanel (testPanel);
              quit=1;
			  ret=TRUE;			//ȡ��
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
		    sprintf(temp,"%s","��");
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
			    MessagePopup("����","��ǰû�п��õļ��ϣ�");
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
