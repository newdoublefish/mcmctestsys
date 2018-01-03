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
#include "testFramework.h"
#include "AutoTestPanel.h"
#include "autoTest.h"
#include "common.h"
#include "resultSave.h"
#include "testGroupInit.h"
#include "policyConfig.h"
#include "resultUtil.h"
#include "settingConfig.h"
#include "debug.h"
#include "tipsParse.h"
#include "log.h"
#include "tpsHelper.h"
#include "reportDb.h" 
#include "regexpr.h"  
#include "mediaHelper.h"

static int autoPanelHandle=0;


static long totalSeconds;

static char startTime[100]={0};
extern int g_mainHWND; 


static char autoTestImageAbsoluteImagePath[MAX_PATHNAME_LEN];
#if 0
static char *suspendImagePath="\\resource\\image\\suspend_red.png";  
static char *startTestImagePath="\\resource\\image\\startTest.png";
static char *stopImagePath="\\resource\\image\\stop.png"; 
#else
static char *suspendImagePath="suspend_red.png";  
static char *startTestImagePath="startTest.png";
static char *stopImagePath="stop.png"; 
#endif

static TESTengine *engine;

static void operateTimer(int on);

static SETTING s;
static ENUMETestPanel gTestFlag=ENUM_TEST_PANEL_AUTO;

ENUMTestResult onObjectGroupTest(TestGroup testItem,TESTobject *_obj,TESTType type);
BOOL objectResultShow(TESTobject *obj,TestGroup group,int testGroupIndex,int *testItemIndex);


static void setButtonState(int state)
{
	 if(state==1)//���У�1 ���԰�ť�ɲ��Ա�Ϊֹͣ 2 ������Ч 3 ������Ч
	 {
	      SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_RESTBUTTON, ATTR_DIMMED ,1); 
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_BACK, ATTR_DIMMED ,1);
		  
		  
    	  //memset(autoTestImageAbsoluteImagePath,0,MAX_PATHNAME_LEN); 
	      //GetProjectDir (autoTestImageAbsoluteImagePath);
		  
		  //strcat(autoTestImageAbsoluteImagePath,stopImagePath); 
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_TEST, ATTR_IMAGE_FILE ,
                              stopImagePath);
	 }else if(state==2)//ֹͣ��1 ���԰�ť���ܲ��� 2 ������Ч 3 ������Ч    
	 {
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_TEST, ATTR_DIMMED ,1);  
	      SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_RESTBUTTON, ATTR_DIMMED ,0); 
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_BACK, ATTR_DIMMED ,0);  
		  
	 }else if(state==3) //���ã�1 ���԰�ť���ܲ��� 2 ������Ч 3 ������Ч
	 {
	      SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_RESTBUTTON, ATTR_DIMMED ,0); 
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_BACK, ATTR_DIMMED ,0);  
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_TEST, ATTR_DIMMED ,0); 
		  
    	  //memset(autoTestImageAbsoluteImagePath,0,MAX_PATHNAME_LEN); 
	      //GetProjectDir (autoTestImageAbsoluteImagePath);
		  
		  //strcat(autoTestImageAbsoluteImagePath,startTestImagePath); 
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_TEST, ATTR_IMAGE_FILE ,
                              startTestImagePath);  			  
		  
	 }	 
}

static void saveRecordToDb(TESTobject obj,char *startTime,char *reportFilePath)
{

	tAutoTestRecord record={0};
	sprintf(record.ProductId,"%s",obj.device.eutName);
	sprintf(record.m_date,"%s",startTime);
	sprintf(record.FtpAddress,"%s",reportFilePath);
	record.Result=1;
	record.m_update=0;
	sprintf(record.m_name,"%s","ray");
	insertReportRecord(record);	
}

static void saveAutoTestResult(TESTengine *t)
{
	 int panelHandle=LoadPanel(0,"AutoTestPanel.uir",PANEL_WA_2);
	 DisplayPanel(panelHandle);
	 char temp[100]={0};
	 char filePath[MAX_PATHNAME_LEN]={0};
	 SETTING set=GetSetting();
     for(int i=0;i<t->totalTestObject;i++)
	 {
		  
		 	
		 	 memset(temp,0,100);
		 	 sprintf(temp,"%d/%d",i+1,t->totalTestObject);
			SetCtrlVal(panelHandle,PANEL_WA_2_TEXTMSG_2,temp);
	     	 saveTestResult(startTime,t->objectArray[i].device.eutName,t->objectArray[i].resultHashTable,filePath);
			 if(set.saveToDb)    
			 	saveRecordToDb(t->objectArray[i],startTime,filePath);
	  
	 }
	 DiscardPanel(panelHandle);
	 WarnShow1(0,"    �������");
}

static BOOL ontestBeginListener(TESTengine *t)
{
	BOOL ret=TRUE;

	//����TPS����Ԥ��׼���Ĺ���
	TpsPrepareTest(); 
	return ret;
}	

static void ontestFinishListener(TESTengine *t)
{
	if(t->testState==TEST_STOP)
	{
	   operateTimer(0);	
	   
	   t->testState=TEST_IDLE;
	   setButtonState(2); 
	   WarnShow1(t->panelHandle,"��ֹͣ"); 
	   
	   
	}else if(t->testState==TEST_FINISH)
	{	
	   	
	   operateTimer(0);	
	   t->testState=TEST_IDLE; 
	   LOG_EVENT(LOG_INFO,"�������");
       WarnShow1(0,"�������");
	   /*SETTING set=getSetting();  
	   if(!set.autoSave)
	   {
	     if(GetConfigWarnPanelRet(0,"��ʾ","�Ƿ񱣴���Խ��","������","����")==TRUE)
	     {
		     saveAutoTestResult(t); 
		     LOG_EVENT(LOG_INFO,"���������"); 
	     }
	   }*/
	   setButtonState(2);
	}else if(t->testState==TEST_RUN)
	{
       WarnShow1(t->panelHandle,"���ڲ�����");
	}  
	
}

static int getNumFromTag(char *tag)
{
	int matched,position,matchedLen; 
	RegExpr_FindPatternInText("[0-9]*[0-9]",0,tag,strlen(tag),1,1,&matched,&position,&matchedLen);
	 if(matched)
	 {	
		char temp[20]={0};
	 	memcpy(temp,tag+position,matchedLen);
		//printf(":%s\n",temp);
	 	return atoi(temp);
	 }
	 return -1;
}

static void CVICALLBACK onMenuSingleItemTestCB(int panel, int controlID, int MenuItemID, 
									void *callbackData)
{
	
	char groupTag[32]={0}; 
	int selected=0;
	int numDescendents=0;
	int parent=0;
	GetActiveTreeItem(panel,P_ITEMSHOW_TREE,&selected);
	GetTreeItemNumDescendents(panel,P_ITEMSHOW_TREE,selected,&numDescendents);
	GetTreeItemParent(panel,P_ITEMSHOW_TREE,selected,&parent);
	GetTreeItemTag(panel,P_ITEMSHOW_TREE,selected,groupTag); 
	//printf("selected:%d,numDescendents:%d,parent:%d\n",selected,numDescendents,parent);
	if(parent!=-1 && numDescendents!=0)
	{
		int collectSelected=0;
		int testGroupIndex=0; 
		char collectTag[32]={0};
		GetTreeItemTag(panel,P_ITEMSHOW_TREE,parent,collectTag);
		//printf("selected:%d,collectSelected:%d\n",selected,parent);
		//printf("collect:%d,group:%d\n",getNumFromTag(collectTag),getNumFromTag(groupTag));
		int numberOfChildren;
		GetTreeItemNumChildren(panel,controlID,parent,&numberOfChildren);
		int childStart=parent+1;
		if(childStart==selected)
		{
			testGroupIndex=1;	
		}else{
			for(int i=1;i<=numberOfChildren-1;i++)
			{
				GetTreeItem(panel,controlID,VAL_SIBLING,childStart,childStart,VAL_NEXT,0,&childStart);
				//printf("%d\n",childStart);
				testGroupIndex=i+1;
				if(childStart==selected)
					break;
			}
		}
		//printf("testGroupIndex:%d\n",testGroupIndex);
		int collectId = getNumFromTag(collectTag);
		int groupId = getNumFromTag(groupTag);
		TestGroup group={0}; 
		ListGetItem(engine->itemList,&group,groupId);
		//printf("groupName:%s\n",group.groupName);
		//���Ե���
		TESTobject *_obj=getTestObjectByPanelHandle(panel,engine);
		if(_obj!=NULL)
		{
			
			int testItemIndex=0;
			ENUMTestResult ret = onObjectGroupTest(group,_obj,TYPE_AUTO); 
			engine->currentCollect = collectId;
			
			if(ret != TEST_RESULT_QUIT)
				objectResultShow(_obj,group,testGroupIndex,&testItemIndex);
		}
	}
	//printf("-------tag:%d,numDescendents:%d,numParents:%d\n",getNumFromTag(tag),numDescendents,numParents); 
}



static void objectPanelCreate(int *panel)
{
    *panel=LoadPanel (engine->panelHandle, "AutoTestPanel.uir", P_ITEMSHOW);
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_EXPAND_SUBTREE); 
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_SEARCH);
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_SORT);  
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_COLLAPSE_SUBTREE);
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_EXPAND_ALL);
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_COLLAPSE_ALL);
	NewCtrlMenuItem(*panel, P_ITEMSHOW_TREE, "�������", -1, onMenuSingleItemTestCB, 0);

	int i=0;
}

BOOL objectResultShow(TESTobject *obj,TestGroup group,int testGroupIndex,int *testItemIndex)
{
	BOOL flag=TRUE;
	int reTestFlag=1;
    //todo:�������
	//��ȡ��collect�ľ���index
	int collectIndex=0;
	TESTengine *t=obj->enginePtr;
	 
	for(int i=1;i<t->currentCollect;i++)
    {		
	    //GetTreeItem(device.panelHandle,P_ITEMSHOW_TREE,VAL_SIBLING,0,collectIndex,VAL_NEXT,0,&collectIndex);
		GetTreeItem(obj->panelHandle,P_ITEMSHOW_TREE,VAL_SIBLING,0,collectIndex,VAL_NEXT,0,&collectIndex); 
		//printf("collectIndex=%d\n",collectIndex); 
	}
	//printf("collectNum=%d,collectIndex=%d\n",collectNum,collectIndex);   
	
	//��ȡcollect��������Index
	//printf("groupName=%s\n",group.groupName);
	
    int groupCount;
	GetNumTreeItems(obj->panelHandle,P_ITEMSHOW_TREE,VAL_CHILD,collectIndex,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&groupCount);	
	int groupIndex=collectIndex+1;
	//PRINT("groupIndex=%d\n",groupIndex); 
	
	int  perc=(obj->totalFinishedTestGroupCount)*100/(t->totalTestGroupCount);
	
	char percStr[30]={0};
	Fmt(percStr,"%d%%",perc);
	SetCtrlVal(obj->panelHandle,P_ITEMSHOW_PERCSTR,percStr);
	
	//PRINT("total:%d,current:%d,%s",t->totalTestGroupCount,obj->totalFinishedTestGroupCount,percStr);
	
	for(int i=1;i<=groupCount;i++)
	{
		//��ȡ����������
		//printf("groupIndex=%d\n",groupIndex);   
		/*if(testGroupIndex!=i)
		{	
			printf("continue groupCount:%d,testGroupIndex:%d,i:%d\n",groupCount,testGroupIndex,i);
			continue;
		}	
		
		PRINT("--------testGroupIndex:%d",testGroupIndex);*/ 
		
		int itemCount;
		GetNumTreeItems(obj->panelHandle,P_ITEMSHOW_TREE,VAL_CHILD,groupIndex,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&itemCount);
		
		//SetActiveTreeItem(device.panelHandle,P_ITEMSHOW_TREE,groupIndex,VAL_REPLACE_SELECTION_WITH_ITEM);
		int itemIndex=groupIndex+1;
		
		//��ȡ����
		char groupIndexStr[32]={0}; 
		GetTreeItemTag(obj->panelHandle,P_ITEMSHOW_TREE,groupIndex,groupIndexStr);
			
		int groupNum=atoi(groupIndexStr);
		//PRINT("group.groupId:%d,groupNum:%d\n",group.groupId,groupNum);
		
		//PRINT("i:%d,testGroupIndex:%d\n",i,testGroupIndex); 	
		if(/*groupNum==group.groupId*/i==testGroupIndex)
		{	
			//PRINT("----------select i:%d,testGroupIndex:%d\n",i,testGroupIndex);
		    ENUMTestResult groupResult;
		    //HashTableGetItem(device.resultHashTable,&groupNum,&groupResult,sizeof(result));
		
		    int k=1;
		    //if(itemCount!=ListNumItems(groupResult.subResults))
		    //{
		       //MessagePopup("hello","wrong");
		    //}
			if(itemCount!=ListNumItems(group.subItems))
			{
				MessagePopup("hello","wrong");
			}
		    for(int j=itemIndex;j<itemIndex+itemCount;j++)
		    {
		      //TODO:��ȡtag,��ɻ�ȡ�����ţ��ӽ����HASHTABLE��������ҵ���ֵ
			//����ɫ��setActivie
		   //printf("itemIndex=%d\n",j);
			
			    RESULT itemResult;
				TestItem subItem;
			
		    	ListGetItem(group.subItems,&subItem,k++); 
				
				HashTableGetItem(obj->resultHashTable,&(subItem.itemId),&itemResult,sizeof(RESULT));
				
			   if(j<*testItemIndex+groupIndex+1)
			   {
			       continue;
			   }	   
			   char tempStr[100]={0}; 
			   
			   sprintf(tempStr,"%s",itemResult.recvString);
			   //printf("readHashTable:%d\n",subItem.index);
			   
		      if(!JudgeTestProcess((TESTengine *)(obj->enginePtr)))  //���Ϊ��ɣ����������ã�������   
		      {
			         goto DONE;
		      }			   
				
		       if(itemResult.pass)
		       {	   
		          SetActiveTreeItem(obj->panelHandle,P_ITEMSHOW_TREE,j,VAL_REPLACE_SELECTION_WITH_ITEM);  //ѡ�е�ǰ������
		          SetTreeItemAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
                  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,1,ATTR_LABEL_BGCOLOR ,VAL_GREEN);	
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,2,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
				  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_TEXT ,tempStr);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_TEXT ,"�ϸ�");

				  
		       }else
		       {
		          SetActiveTreeItem(obj->panelHandle,P_ITEMSHOW_TREE,j,VAL_REPLACE_SELECTION_WITH_ITEM);  //ѡ�е�ǰ������
		          SetTreeItemAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,ATTR_LABEL_BGCOLOR ,VAL_RED);
                  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,1,ATTR_LABEL_BGCOLOR ,VAL_RED);	
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,2,ATTR_LABEL_BGCOLOR ,VAL_RED);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_BGCOLOR ,VAL_RED);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_BGCOLOR ,VAL_RED);
				  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_TEXT ,tempStr);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_TEXT ,"���ϸ�");
				   *testItemIndex=j-groupIndex-1; 
				   //printf("j:%d,testItemIndex:%d,acture:%d\n",j,*testItemIndex,*testItemIndex+groupIndex+1);  
				  
				  if(s.showTipsWhenError)
				  {	  

					 if(reTestFlag ==1)
					 {	 
						 char showText[100]={0};
				         sprintf(showText,"%s:�Ƿ��ز�?",subItem.itemName_);
				        //flag=ShowReTestTips(showText);
					     flag=GetConfigWarnPanelRetWithQuitFlag(obj->panelHandle,"��ʾ",showText,"�ز�","���ز�",(int *)&(t->testState));	 
				        if(flag==FALSE)
				        {
				           goto DONE; //������������ز�
				         }else{
					      reTestFlag=0;  //��ֹҪ�ز�̫��
					     }
					 }
				  }	
		       }	   
		   
	    	}
		}
		GetTreeItem(obj->panelHandle,P_ITEMSHOW_TREE,VAL_SIBLING,groupIndex,groupIndex,VAL_NEXT,0,&groupIndex); 
	}
  DONE:	
	return flag;
}



static void objectPanelInit(TESTobject *_obj)
{
	
	int deviceCount;
	TestGroup group;
	TestItem subTestItem; 
	Collect collect;
	ListType groupList;
	ListType subList;
	int parent;
	int subParent; 
	int index;
	char temp[30]={0}; 
	int panelHandle;
	int panelWidth=0,panelHeight=0;
	TESTengine *t=(TESTengine *)(_obj->enginePtr);
	panelHandle=_obj->panelHandle;
	
	GetPanelAttribute (panelHandle, ATTR_WIDTH, &panelWidth);
    GetPanelAttribute (panelHandle, ATTR_HEIGHT, &panelHeight);
	
	
	Fmt(temp,"�豸���:%d:%s",_obj->device.index,_obj->device.eutName);
	SetCtrlAttribute(panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_TEXT,temp);
    SetCtrlAttribute (panelHandle, P_ITEMSHOW_TREE,ATTR_WIDTH,panelWidth);
    SetCtrlAttribute (panelHandle, P_ITEMSHOW_TREE,ATTR_HEIGHT,panelHeight-30); 
	

	int test=ListNumItems(t->collectList);
	
	for(int count=1;count<=ListNumItems(t->collectList);count++)
	{
		  ListGetItem(t->collectList,&collect,count);
		  memset(temp,0,30);
		  sprintf(temp,"%d",count);
		  InsertTreeItem (panelHandle, P_ITEMSHOW_TREE, VAL_SIBLING,0, VAL_LAST,
                    collect.name_ ,temp, 0, 0);
		 
		  groupList=collect.groups;
		
		  GetNumTreeItems(panelHandle,P_ITEMSHOW_TREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&parent);
		//parent=ListNumItems(P_ITEMSHOW_TREE)-1;
		  parent--;
		  
		  for(int count1=1;count1<=ListNumItems(groupList);count1++)
		  {
			  int num;
			  char groupIndexStr[32]={0};  
		      ListGetItem(groupList,&num,count1);//��ȡ���ID��
			 
			  ListGetItem(t->itemList,&group,num);//��ȡ��
			  sprintf(groupIndexStr,"%d",num);
			  
			  //PRINT("groupIndexStr=%s\n",groupIndexStr);
			  
       		  InsertTreeItem (panelHandle, P_ITEMSHOW_TREE, VAL_CHILD,parent,VAL_LAST,
                    group.nickName,groupIndexStr, 0, 0);  //TAG �������������е����
			  
			 
			 subList=group.subItems;
		     GetNumTreeItems(panelHandle,P_ITEMSHOW_TREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&subParent);
		     subParent--;
	      	for(int count2=1;count2<=ListNumItems(subList);count2++)
		    {
			   //char temp[100]={0};
			   TestItem testItem;
		       ListGetItem(subList,&testItem,count2);
       		   InsertTreeItem (panelHandle, P_ITEMSHOW_TREE, VAL_CHILD,subParent,VAL_LAST,
                    testItem.itemName_,0, 0, 0);
			   GetNumTreeItems(panelHandle,P_ITEMSHOW_TREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&index);
			   //sprintf(temp,"%f",testItem.input_Value);
			   SetTreeCellAttribute(panelHandle,P_ITEMSHOW_TREE,index-1,1, ATTR_LABEL_TEXT,testItem.inputValue_);
			   SetTreeCellAttribute(panelHandle,P_ITEMSHOW_TREE,index-1,2, ATTR_LABEL_TEXT,testItem.standard_); 
			}  
		}	
	}
	SETTING set=getSetting();
	SetTreeCellAttribute (panelHandle,P_ITEMSHOW_TREE, VAL_ALL_OBJECTS ,  VAL_ALL_OBJECTS , ATTR_LABEL_POINT_SIZE,set.frontSize);
		

}

static void objectTestError(TESTobject *_obj)
{
	char temp[30]={0};
	Fmt(temp,"�豸%d:%s ����",_obj->objectIndex,_obj->device.eutName);
	SetCtrlAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_TEXT,temp);    
	SetCtrlAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_COLOR,VAL_RED); 
}


static void objectTestFinish(TESTobject *_obj)
{
	/*char temp[30]={0};
	Fmt(temp,"�豸%d:%s ����",_obj->objectIndex,_obj->device.eutName);
	SetCtrlAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_TEXT,temp);    
	SetCtrlAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_COLOR,VAL_RED);*/
	//tAutoTestRecord record;
	SETTING set=getSetting();
	if(set.autoSave)
	{
	   char temp[MAX_PATHNAME_LEN]={0}; 
	   //TODO:����Ҫ����
	   saveTestResult(startTime,_obj->device.eutName,_obj->resultHashTable,temp);
	   if(set.saveToDb)
	   	saveRecordToDb(*_obj,startTime,temp);
	}
}

static void operateTimer(int on)
{
	unsigned int year, month, day, hour, min, sec, weekDay;
	CVIAbsoluteTime absTime;
	if(on)
	{
	    totalSeconds=0;
		memset(startTime,100,0);
		GetCurrentCVIAbsoluteTime(&absTime);
        CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
                &min, &sec, 0, &weekDay);
		sprintf(startTime,"%d��%02d��%02d��%2dʱ%02d��%02d��",year,month,day,hour,min,sec);
		SetCtrlAttribute(autoPanelHandle,PANEL_AUTO_TIMER,ATTR_INTERVAL,0.5);  //��ʼ 
	}else
	{
	    //SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0ʱ0��0��"); 
	}	
	SetCtrlAttribute(autoPanelHandle,PANEL_AUTO_TIMER,ATTR_ENABLED,on); 
	//SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0��0��");
}

extern METHODRET manualTest(TestGroup group,EUT eut,HashTableType hashTable);
ENUMTestResult onObjectGroupTest(TestGroup testItem,TESTobject *_obj,TESTType type)
{
	 	ENUMTestResult ret=TEST_RESULT_ALL_PASS;
		BOOL testFlag=TRUE;
		METHODRET testRet= TEST_ERROR; 
		
		SETTING set=getSetting();
		if(set.collectTestMention || gTestFlag == ENUM_TEST_PANEL_MANUAL)
		{	
		   char temp[256]={0};
		   char temp1[256]={0};
		   if(getTip(testItem.groupName,temp1)>0)
		   {
		       sprintf(temp,"%s:%s,׼������!",_obj->device.eutName,temp1);   
		   }else{	   
		       sprintf(temp,"%s:%s,׼������!",_obj->device.eutName,testItem.groupName);
		   }
		   //WarnShow1(0,temp);
		   //TESTengine *engine = (TESTengine*)_obj->enginePtr;
		   //ShowManualTip(0,testItem.groupName,temp1);
		   //��ʾ�����Կ����Ƿ������һ�����
		   testFlag=showTips(0,testItem.groupName,temp1);
		} 
		
		if(FALSE == testFlag)
			return TEST_RESULT_QUIT;//ע�ⷵ��ֵ���Ӧ������������ֹͣ����ʱ����ERROR
		
	    /*TEST_METHOD method=getTestMethod(testItem); //��ȡ���Է���
		//LOG_LEVEL("Debug","%x\n",_obj->resultHashTable);
		METHODRET testRet= (*method)(testItem,_obj->device,_obj->resultHashTable);*/
		TPS tps;
		//memset(&tps,0,sizeof(tps));
		if(getTps(testItem.type,&tps))
		{
			//printf("%s",tps.tpsName);
			
			if(type == TYPE_AUTO){
				if(tps.autoTestFunction==NULL)
				{
					WarnShow1(0,"û�ж����Զ����Եķ���"); 
				}else{
					testRet = (*(TEST_METHOD)(tps.autoTestFunction))(testItem,_obj->device,_obj->resultHashTable);
				}
			}else{
				if(tps.manualTestFunction==NULL)
				{
					//WarnShow1(0,"û�ж����ֶ����Եķ���");
					testRet =manualTest(testItem,_obj->device,_obj->resultHashTable);
				}else{
					testRet = (*(TEST_METHOD)(tps.manualTestFunction))(testItem,_obj->device,_obj->resultHashTable); 
					PRINT("%d\n",testRet);
				}
				
			} 
		   	if(testRet==TEST_RESULT_ALLPASS)
		    	ret=TEST_RESULT_ALL_PASS;
	     	else if(testRet==TEST_RESULT_ERROR)
		    	ret=TEST_ERROR;
	     	else if(testRet=TEST_RESULT_SOMEPASS)
		    	ret=TEST_RESULT_SOME_PASS; 
		}else
		{
		    WarnShow1(0,"��֧�ֵ�TPS���ͣ����˹���д�������ݺͽ��"); 
			//��֧�ֵ���Ĭ�ϵ���������
			testRet =manualTest(testItem,_obj->device,_obj->resultHashTable);
		   	if(testRet==TEST_RESULT_ALLPASS)
		    	ret=TEST_RESULT_ALL_PASS;
	     	else if(testRet==TEST_RESULT_ERROR)
		    	ret=TEST_ERROR;
	     	else if(testRet=TEST_RESULT_SOMEPASS)
		    	ret=TEST_RESULT_SOME_PASS; 			
		}
		return ret;
}

#define  AUTOTEST_PANEL_PADDING_TOP 25
#define  AUTOTEST_PANEL_PADDING_LEFT 150
#define  AUTOTEST_BUTTON_PADDING_SIZE 30
static int autoPanelWidth=0;
static int autoPanelHeight=0;
static int headerHeight=0; 

static void adjustHeader(int panel)
{
	int buttonSize=0;
	GetCtrlAttribute(panel,PANEL_AUTO_TEST,ATTR_WIDTH,&buttonSize);
	//��һ��ͼ��
	SetCtrlAttribute(panel,PANEL_AUTO_TEST,ATTR_TOP,AUTOTEST_BUTTON_PADDING_SIZE);
	SetCtrlAttribute(panel,PANEL_AUTO_TEST,ATTR_LEFT,autoPanelWidth-(4*AUTOTEST_BUTTON_PADDING_SIZE+4*buttonSize));
	//�ڶ���ͼ��
	SetCtrlAttribute(panel,PANEL_AUTO_GENREPORT,ATTR_TOP,AUTOTEST_BUTTON_PADDING_SIZE);
	SetCtrlAttribute(panel,PANEL_AUTO_GENREPORT,ATTR_LEFT,autoPanelWidth-(3*AUTOTEST_BUTTON_PADDING_SIZE+3*buttonSize));	
	//������ͼ��
	SetCtrlAttribute(panel,PANEL_AUTO_RESTBUTTON,ATTR_TOP,AUTOTEST_BUTTON_PADDING_SIZE);
	SetCtrlAttribute(panel,PANEL_AUTO_RESTBUTTON,ATTR_LEFT,autoPanelWidth-(2*AUTOTEST_BUTTON_PADDING_SIZE+2*buttonSize));	
	//���ĸ�ͼ��
	SetCtrlAttribute(panel,PANEL_AUTO_BACK,ATTR_TOP,AUTOTEST_BUTTON_PADDING_SIZE);
	SetCtrlAttribute(panel,PANEL_AUTO_BACK,ATTR_LEFT,autoPanelWidth-(AUTOTEST_BUTTON_PADDING_SIZE+buttonSize));	
	//���ò���ʱ��LABEL
	int width=0,height=0; 
	SetCtrlAttribute(panel,PANEL_AUTO_TEXTMSG,ATTR_LEFT,AUTOTEST_BUTTON_PADDING_SIZE);
	GetCtrlAttribute(panel,PANEL_AUTO_TEXTMSG,ATTR_WIDTH,&width);
	GetCtrlAttribute(panel,PANEL_AUTO_TEXTMSG,ATTR_HEIGHT,&height);
	SetCtrlAttribute(panel,PANEL_AUTO_TEXTMSG,ATTR_TOP,buttonSize+AUTOTEST_BUTTON_PADDING_SIZE-height);
	headerHeight=AUTOTEST_BUTTON_PADDING_SIZE+buttonSize;
	
	int tickWidth=0,tickHeight=0;
	SetCtrlAttribute(panel,PANEL_AUTO_TIMER,ATTR_LEFT,2*AUTOTEST_BUTTON_PADDING_SIZE+width);
	GetCtrlAttribute(panel,PANEL_AUTO_TIMER,ATTR_WIDTH,&tickWidth);
	GetCtrlAttribute(panel,PANEL_AUTO_TIMER,ATTR_HEIGHT,&tickHeight);
	SetCtrlAttribute(panel,PANEL_AUTO_TIMER,ATTR_TOP,buttonSize+AUTOTEST_BUTTON_PADDING_SIZE-tickHeight);	
	
	
	
}


static void adjustBody(int panel)
{
	int bodyHeight=autoPanelHeight-headerHeight;
	int bodyWidth = autoPanelWidth;
	SetCtrlAttribute(panel,PANEL_AUTO_TAB_AUTO,ATTR_TOP,headerHeight);
	SetCtrlAttribute(panel,PANEL_AUTO_TAB_AUTO,ATTR_LEFT,0);
	SetCtrlAttribute(panel,PANEL_AUTO_TAB_AUTO,ATTR_WIDTH,bodyWidth);
	SetCtrlAttribute(panel,PANEL_AUTO_TAB_AUTO,ATTR_HEIGHT,bodyHeight);	//������	

}

static void adjustPanelSize(int panel)
{
	int monitor=0,height=0,width=0;
	//�����
	GetMonitorFromPanel (panel, &monitor);
	GetMonitorAttribute (monitor, ATTR_HEIGHT, &height);
	GetMonitorAttribute (monitor, ATTR_WIDTH, &width);
	autoPanelWidth=width-2*AUTOTEST_PANEL_PADDING_LEFT;//����һ����
	autoPanelHeight=height-2*AUTOTEST_PANEL_PADDING_TOP;
	//����������С
	SetPanelAttribute(panel,ATTR_WIDTH,autoPanelWidth);
	SetPanelAttribute(panel,ATTR_HEIGHT,autoPanelHeight);
	SetPanelAttribute(panel,ATTR_TOP,VAL_AUTO_CENTER);//�Զ�����
	SetPanelAttribute(panel,ATTR_LEFT,VAL_AUTO_CENTER);
	//���ñ���ɫ��С
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_TOP,0);
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_LEFT,0);
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_WIDTH,autoPanelWidth);
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_HEIGHT,autoPanelHeight);
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_CTRL_VAL,"");
	
	//����header
	adjustHeader(panel);
	adjustBody(panel);
}


void DisplayAutoTestPanel(ListType groupList,ListType deviceList,ListType collectList,ENUMETestPanel type)
{
	
	int tempPanel=0;
	if ((autoPanelHandle = LoadPanel (0, "AutoTestPanel.uir", PANEL_AUTO)) < 0)
		return;

   	if(GetPanelHandleFromTabPage(autoPanelHandle,PANEL_AUTO_TAB_AUTO,0,&tempPanel) < 0)
  	{
	        return;
   	}
	
	gTestFlag = type; 
	if(type == ENUM_TEST_PANEL_MANUAL){
		
		SetCtrlAttribute(autoPanelHandle,PANEL_AUTO_TEST,ATTR_VISIBLE,0);
		SetCtrlAttribute(autoPanelHandle,PANEL_AUTO_TEXTMSG,ATTR_VISIBLE,0);		
		SetCtrlAttribute(autoPanelHandle,PANEL_AUTO_TEXTMSG_3,ATTR_VISIBLE,0);		
	}
	
	adjustPanelSize(autoPanelHandle);
	DisplayPanel(autoPanelHandle);	
#if 0 //Э��������ŵ�����ŵ�tps��ʼ������	
	s=GetSetting();
	
	if(s.collectTestMention) //���������ʾ��������ʾ
	{
		getTipsFromExcel();
	}
#endif	
	engine=createTestEngine(tempPanel,groupList,collectList);//step1 �������ڲ��ԵĲ���
	engine->onTestBeginListener=(void *)ontestBeginListener;   //���������ص�����
	engine->onTestFinishListener=(void *)ontestFinishListener; //������ɺ�ص�����
	SETTING s=GetSetting();
	engine->maxObjectPanelCountPerRow=s.maxComunPerRowInAutoTestPanel;
	engine->reTestCnt=s.reTestCnt;


	for(int eutIndex=1;eutIndex<=ListNumItems(deviceList);eutIndex++)
	{
		EUT eut;

		ListGetItem(deviceList,&eut,eutIndex);
		if(eut.enable==0) //��������ԵĲ���ӽ��Զ���������
			continue;
	    TESTobject *obj=createAndaddTestObject(engine,eut);   //step2 ����������Ӳ��Զ���
		obj->onObjectPanelCreateListener=(void *)objectPanelCreate; //���Զ�����崴��
		obj->onObjectPanelInitListener=(void *)objectPanelInit;   //���Զ�������ʼ������Ҫ����ʾ������Ŀ
		obj->onObjectGroupTestListener=(void *)onObjectGroupTest; //���Ժ���
		obj->onResultShowListener=(void *)objectResultShow;	   //���Զ�������ʾ
		obj->onObjectTestErrorListener=(void *)objectTestError;//���Զ������
		obj->onObjectTestFinish=(void *)objectTestFinish;
		//obj->ON_OBJECT_TEST_FINISH_LISTENER
	}
	
	disPlayTestPanel(engine); //step3 ��ʾ���Զ������

}




int CVICALLBACK STARTTEST (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			 if(engine->testState==TEST_IDLE)
			 {	   
				if(engine->totalTestObject!=0)
				{	
				  setButtonState(1); 
				  SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0ʱ0��0��"); 
				  operateTimer(1); 
			      runTestEngine(engine);//step4 ����
				}
			 }else
			 {
			    engine->testState=TEST_STOP;
			 } 
		     break;
	}
	return 0;
}


int CVICALLBACK RESET (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			operateTimer(0);
			reSetEngine(engine);
			setButtonState(3);
			SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0ʱ0��0��");    
		    SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_TEST, ATTR_DIMMED ,0); 
	}
	return 0;
}



int CVICALLBACK TESTTIMECALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int hour,min,sec;
	char temp[100]={0};
	switch (event)
	{
		case EVENT_TIMER_TICK:
			totalSeconds++;
			hour=(totalSeconds/2)/3600;
			min=((totalSeconds/2)%3600)/60;
			sec=((totalSeconds/2)%3600)%60;
			sprintf(temp,"%dʱ%d��%d��",hour,min,sec);
			SetCtrlVal (panel, PANEL_AUTO_TEXTMSG_3,temp);
			break;
	}
	return 0;
}

int CVICALLBACK QUITAUTOTEST (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
    unsigned int wParam1=3;
    unsigned int lParam1=0;
	
	switch (event)
	{
		case EVENT_COMMIT:
			releaseTestEngine(engine);
			DiscardPanel(panel); 
			PostMessage ((HWND)g_mainHWND, 9678, wParam1, lParam1);  
			break;
	}
	return 0;
}

#if 1
static void reFreshAutoTestPanel(int panel)
{
	adjustPanelSize(autoPanelHandle);
	reSetEngine(engine);
}
#endif


int  CVICALLBACK onPanelAutoCall(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_PANEL_SIZE:
			break;	
	}
	return 0;
}




int CVICALLBACK genReport (int panel, int control, int event,
						   void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
	     if(GetConfigWarnPanelRet(0,"��ʾ","�Ƿ񱣴���Խ��","������","����")==TRUE)
	     {
			 if(gTestFlag == ENUM_TEST_PANEL_MANUAL) //�ֶ����ԣ����ʱ��Ҫ�Լ�����
			 {
				unsigned int year, month, day, hour, min, sec, weekDay;
				CVIAbsoluteTime absTime;
				memset(startTime,0,100);
				GetCurrentCVIAbsoluteTime(&absTime);
        		CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
                	&min, &sec, 0, &weekDay);
				sprintf(startTime,"%d��%02d��%02d��%2dʱ%02d��%02d��",year,month,day,hour,min,sec); 
			 	
			 }
		     saveAutoTestResult(engine);
			 //soundFinish();
			 
		     LOG_EVENT(LOG_INFO,"���������"); 
	     }
				 
		break;
	}
	return 0;
}
