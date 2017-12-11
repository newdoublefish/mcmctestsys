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


static void setButtonState(int state)
{
	 if(state==1)//运行，1 测试按钮由测试变为停止 2 重置无效 3 返回无效
	 {
	      SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_RESTBUTTON, ATTR_DIMMED ,1); 
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_BACK, ATTR_DIMMED ,1);
		  
		  
    	  //memset(autoTestImageAbsoluteImagePath,0,MAX_PATHNAME_LEN); 
	      //GetProjectDir (autoTestImageAbsoluteImagePath);
		  
		  //strcat(autoTestImageAbsoluteImagePath,stopImagePath); 
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_TEST, ATTR_IMAGE_FILE ,
                              stopImagePath);
	 }else if(state==2)//停止，1 测试按钮不能操作 2 重置有效 3 返回有效    
	 {
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_TEST, ATTR_DIMMED ,1);  
	      SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_RESTBUTTON, ATTR_DIMMED ,0); 
		  SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_BACK, ATTR_DIMMED ,0);  
		  
	 }else if(state==3) //重置，1 测试按钮不能操作 2 重置有效 3 返回有效
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

static void saveAutoTestResult(TESTengine *t)
{
	 int panelHandle=LoadPanel(0,"AutoTestPanel.uir",PANEL_WA_2);
	 DisplayPanel(panelHandle);
	 char temp[100]={0};
     for(int i=0;i<t->totalTestObject;i++)
	 {
		  
		  memset(temp,0,100);
		  sprintf(temp,"%d/%d",i+1,t->totalTestObject);
		  SetCtrlVal(panelHandle,PANEL_WA_2_TEXTMSG_2,temp);
	      saveTestResult(startTime,t->objectArray[i].device.eutName,t->objectArray[i].resultHashTable);
	 }
	 DiscardPanel(panelHandle);
	 WarnShow1(0,"    保存完成");
}

static BOOL ontestBeginListener(TESTengine *t)
{
	BOOL ret=TRUE;

	//各个TPS测试预先准备的工作
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
	   WarnShow1(t->panelHandle,"已停止"); 
	   
	   
	}else if(t->testState==TEST_FINISH)
	{	
	   	
	   operateTimer(0);	
	   t->testState=TEST_IDLE; 
	   LOG_EVENT(LOG_INFO,"测试完成");
       WarnShow1(0,"测试完成");
	   SETTING set=getSetting();  
	   if(!set.autoSave)
	   {
	     if(GetConfigWarnPanelRet(0,"提示","是否保存测试结果","不保存","保存")==TRUE)
	     {
		     saveAutoTestResult(t); 
		     LOG_EVENT(LOG_INFO,"保存结果完成"); 
	     }
	   }
	   setButtonState(2);
	}else if(t->testState==TEST_RUN)
	{
       WarnShow1(t->panelHandle,"正在测试中");
	}  
	
}

static void objectPanelCreate(int *panel)
{
    *panel=LoadPanel (engine->panelHandle, "AutoTestPanel.uir", P_ITEMSHOW);
}

static BOOL objectResultShow(TESTobject *obj,TestGroup group,int testGroupIndex,int *testItemIndex)
{
	BOOL flag=TRUE;
	int reTestFlag=1;
    //todo:单项测试
	//获取到collect的具体index
	int collectIndex=0;
	TESTengine *t=obj->enginePtr;
	 
	for(int i=1;i<t->currentCollect;i++)
    {		
	    //GetTreeItem(device.panelHandle,P_ITEMSHOW_TREE,VAL_SIBLING,0,collectIndex,VAL_NEXT,0,&collectIndex);
		GetTreeItem(obj->panelHandle,P_ITEMSHOW_TREE,VAL_SIBLING,0,collectIndex,VAL_NEXT,0,&collectIndex); 
		//printf("collectIndex=%d\n",collectIndex); 
	}
	//printf("collectNum=%d,collectIndex=%d\n",collectNum,collectIndex);   
	
	//获取collect下面的组的Index
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
		//获取条例的数量
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
		
		//获取组结果
		char groupIndexStr[32]={0}; 
		GetTreeItemTag(obj->panelHandle,P_ITEMSHOW_TREE,groupIndex,groupIndexStr);
			
		int groupNum=atoi(groupIndexStr);
		//PRINT("group.groupId:%d,groupNum:%d\n",group.groupId,groupNum);
		
		//PRINT("i:%d,testGroupIndex:%d\n",i,testGroupIndex); 	
		if(/*groupNum==group.groupId*/i==testGroupIndex)
		{	
			//PRINT("----------select i:%d,testGroupIndex:%d\n",i,testGroupIndex);
		    result groupResult;
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
		      //TODO:获取tag,便可获取组的序号，从结果的HASHTABLE里面可以找到该值
			//上颜色，setActivie
		   //printf("itemIndex=%d\n",j);
			
			    result itemResult;
				TestItem subItem;
			
		    	ListGetItem(group.subItems,&subItem,k++); 
				
				HashTableGetItem(obj->resultHashTable,&(subItem.itemId),&itemResult,sizeof(result));
				
			   if(j<*testItemIndex+groupIndex+1)
			   {
			       continue;
			   }	   
			   char tempStr[100]={0}; 
			   
			   sprintf(tempStr,"%s",itemResult.recvString);
			   //printf("readHashTable:%d\n",subItem.index);
			   
		      if(!JudgeTestProcess((TESTengine *)(obj->enginePtr)))  //如果为完成，或者是重置，则跳出   
		      {
			         goto DONE;
		      }			   
				
		       if(itemResult.pass)
		       {	   
		          SetActiveTreeItem(obj->panelHandle,P_ITEMSHOW_TREE,j,VAL_REPLACE_SELECTION_WITH_ITEM);  //选中当前测试项
		          SetTreeItemAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
                  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,1,ATTR_LABEL_BGCOLOR ,VAL_GREEN);	
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,2,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
				  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_TEXT ,tempStr);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_TEXT ,"合格");

				  
		       }else
		       {
		          SetActiveTreeItem(obj->panelHandle,P_ITEMSHOW_TREE,j,VAL_REPLACE_SELECTION_WITH_ITEM);  //选中当前测试项
		          SetTreeItemAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,ATTR_LABEL_BGCOLOR ,VAL_RED);
                  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,1,ATTR_LABEL_BGCOLOR ,VAL_RED);	
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,2,ATTR_LABEL_BGCOLOR ,VAL_RED);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_BGCOLOR ,VAL_RED);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_BGCOLOR ,VAL_RED);
				  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_TEXT ,tempStr);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_TEXT ,"不合格");
				   *testItemIndex=j-groupIndex-1; 
				   //printf("j:%d,testItemIndex:%d,acture:%d\n",j,*testItemIndex,*testItemIndex+groupIndex+1);  
				  
				  if(s.showTipsWhenError)
				  {	  

					 if(reTestFlag ==1)
					 {	 
						 char showText[100]={0};
				         sprintf(showText,"%s:是否重测?",subItem.itemName_);
				        //flag=ShowReTestTips(showText);
					     flag=GetConfigWarnPanelRetWithQuitFlag(obj->panelHandle,"提示",showText,"重测","不重测",(int *)&(t->testState));	 
				        if(flag==FALSE)
				        {
				           goto DONE; //条例出错进行重测
				         }else{
					      reTestFlag=0;  //防止要重测太多
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
	
	
	Fmt(temp,"设备编号:%d:%s",_obj->device.index,_obj->device.eutName);
	SetCtrlAttribute(panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_TEXT,temp);
    SetCtrlAttribute (panelHandle, P_ITEMSHOW_TREE,ATTR_WIDTH,panelWidth);
    SetCtrlAttribute (panelHandle, P_ITEMSHOW_TREE,ATTR_HEIGHT,panelHeight-30); 
	

	int test=ListNumItems(t->collectList);
	
	for(int count=1;count<=ListNumItems(t->collectList);count++)
	{
		  ListGetItem(t->collectList,&collect,count);
		  InsertTreeItem (panelHandle, P_ITEMSHOW_TREE, VAL_SIBLING,0, VAL_LAST,
                    collect.name_ ,0, 0, 0);
		 
		  groupList=collect.groups;
		
		  GetNumTreeItems(panelHandle,P_ITEMSHOW_TREE,VAL_ALL,0,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&parent);
		//parent=ListNumItems(P_ITEMSHOW_TREE)-1;
		  parent--;
		  
		  for(int count1=1;count1<=ListNumItems(groupList);count1++)
		  {
			  int num;
			  char groupIndexStr[32]={0};  
		      ListGetItem(groupList,&num,count1);//获取组的ID号
			 
			  ListGetItem(t->itemList,&group,num);//获取组
			  sprintf(groupIndexStr,"%d",count1);
			  
			  //PRINT("groupIndexStr=%s",groupIndexStr);
			  
       		  InsertTreeItem (panelHandle, P_ITEMSHOW_TREE, VAL_CHILD,parent,VAL_LAST,
                    group.nickName,groupIndexStr, 0, 0);  //TAG 用组在组链表中的序号
			  
			 
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
		

}

static void objectTestError(TESTobject *_obj)
{
	char temp[30]={0};
	Fmt(temp,"设备%d:%s 故障",_obj->objectIndex,_obj->device.eutName);
	SetCtrlAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_TEXT,temp);    
	SetCtrlAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_COLOR,VAL_RED); 
}

static void objectTestFinish(TESTobject *_obj)
{
	/*char temp[30]={0};
	Fmt(temp,"设备%d:%s 故障",_obj->objectIndex,_obj->device.eutName);
	SetCtrlAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_TEXT,temp);    
	SetCtrlAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,ATTR_LABEL_COLOR,VAL_RED);*/
	SETTING set=getSetting();
	if(set.autoSave)
	{
	   //MessagePopup("helloWorld","finish");
	   saveTestResult(startTime,_obj->device.eutName,_obj->resultHashTable); 
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
		sprintf(startTime,"%d年%02d月%02d日%2d时%02d分%02d秒",year,month,day,hour,min,sec);
		SetCtrlAttribute(autoPanelHandle,PANEL_AUTO_TIMER,ATTR_INTERVAL,0.5);  //开始 
	}else
	{
	    //SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0时0分0秒"); 
	}	
	SetCtrlAttribute(autoPanelHandle,PANEL_AUTO_TIMER,ATTR_ENABLED,on); 
	//SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0分0秒");
}


TESTresult onObjectGroupTest(TestGroup testItem,TESTobject *_obj)
{
	 	TESTresult ret=TEST_ALL_PASS;
		
		SETTING set=getSetting();
		if(set.collectTestMention)
		{	
		   char temp[100]={0};
		   char temp1[100]={0};
		   if(getTip(testItem.groupName,temp1)>0)
		   {
		       sprintf(temp,"%s:%s,准备测试!",_obj->device.eutName,temp1);   
		   }else{	   
		       sprintf(temp,"%s:%s,准备测试!",_obj->device.eutName,testItem.groupName);
		   }
		   //WarnShow1(0,temp);
		   ShowManualTip(0,testItem.groupName,temp1);  
		}   
		
	    /*TEST_METHOD method=getTestMethod(testItem); //获取测试方法
		//LOG_LEVEL("Debug","%x\n",_obj->resultHashTable);
		METHODRET testRet= (*method)(testItem,_obj->device,_obj->resultHashTable);*/
		TPS tps;
		//memset(&tps,0,sizeof(tps));
		if(getTps(testItem.type,&tps))
		{
			//printf("%s",tps.tpsName);
			METHODRET testRet= (*(TEST_METHOD)(tps.autoTestFunction))(testItem,_obj->device,_obj->resultHashTable);
		   	if(testRet==TEST_RESULT_ALLPASS)
		    	ret=TEST_ALL_PASS;
	     	else if(testRet==TEST_RESULT_ERROR)
		    	ret=TEST_ERROR;
	     	else if(testRet=TEST_RESULT_SOMEPASS)
		    	ret=TEST_SOME_PASS; 
		}else
		{
		    WarnShow1(0,"不支持的TPS类型"); 
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
	//第一个图标
	SetCtrlAttribute(panel,PANEL_AUTO_TEST,ATTR_TOP,AUTOTEST_BUTTON_PADDING_SIZE);
	SetCtrlAttribute(panel,PANEL_AUTO_TEST,ATTR_LEFT,autoPanelWidth-(3*AUTOTEST_BUTTON_PADDING_SIZE+3*buttonSize));
	//第二个图标
	SetCtrlAttribute(panel,PANEL_AUTO_RESTBUTTON,ATTR_TOP,AUTOTEST_BUTTON_PADDING_SIZE);
	SetCtrlAttribute(panel,PANEL_AUTO_RESTBUTTON,ATTR_LEFT,autoPanelWidth-(2*AUTOTEST_BUTTON_PADDING_SIZE+2*buttonSize));	
	//第三个图标
	SetCtrlAttribute(panel,PANEL_AUTO_BACK,ATTR_TOP,AUTOTEST_BUTTON_PADDING_SIZE);
	SetCtrlAttribute(panel,PANEL_AUTO_BACK,ATTR_LEFT,autoPanelWidth-(AUTOTEST_BUTTON_PADDING_SIZE+buttonSize));	
	//设置测试时间LABEL
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
	SetCtrlAttribute(panel,PANEL_AUTO_TAB_AUTO,ATTR_HEIGHT,bodyHeight);	//正方形	

}

static void adjustPanelSize(int panel)
{
	int monitor=0,height=0,width=0;
	//主面板
	GetMonitorFromPanel (panel, &monitor);
	GetMonitorAttribute (monitor, ATTR_HEIGHT, &height);
	GetMonitorAttribute (monitor, ATTR_WIDTH, &width);
	autoPanelWidth=width-2*AUTOTEST_PANEL_PADDING_LEFT;//两边一样长
	autoPanelHeight=height-2*AUTOTEST_PANEL_PADDING_TOP;
	//设置主面板大小
	SetPanelAttribute(panel,ATTR_WIDTH,autoPanelWidth);
	SetPanelAttribute(panel,ATTR_HEIGHT,autoPanelHeight);
	SetPanelAttribute(panel,ATTR_TOP,VAL_AUTO_CENTER);//自动居中
	SetPanelAttribute(panel,ATTR_LEFT,VAL_AUTO_CENTER);
	//设置背景色大小
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_TOP,0);
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_LEFT,0);
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_WIDTH,autoPanelWidth);
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_HEIGHT,autoPanelHeight);
	SetCtrlAttribute(panel,PANEL_AUTO_BACKGROUD,ATTR_CTRL_VAL,"");
	
	//设置header
	adjustHeader(panel);
	adjustBody(panel);
}


void DisplayAutoTestPanel(ListType groupList,ListType deviceList,ListType collectList)
{
	
	int tempPanel=0;
	if ((autoPanelHandle = LoadPanel (0, "AutoTestPanel.uir", PANEL_AUTO)) < 0)
		return;

   	if(GetPanelHandleFromTabPage(autoPanelHandle,PANEL_AUTO_TAB_AUTO,0,&tempPanel) < 0)
  	{
	        return;
   	}
	
	adjustPanelSize(autoPanelHandle);
	DisplayPanel(autoPanelHandle);	
	
	s=GetSetting();
	
	if(s.collectTestMention) //如果允许提示，解析提示
	{
		getTipsFromExcel();
	}
	
	engine=createTestEngine(tempPanel,groupList,collectList);//step1 创建基于策略的测试
	engine->onTestBeginListener=(void *)ontestBeginListener;   //测试启动回调函数
	engine->onTestFinishListener=(void *)ontestFinishListener; //测试完成后回调函数
	SETTING s=GetSetting();
	engine->maxObjectPanelCountPerRow=s.maxComunPerRowInAutoTestPanel;
	engine->reTestCnt=s.reTestCnt;
	

	for(int eutIndex=1;eutIndex<=ListNumItems(deviceList);eutIndex++)
	{
		EUT eut;

		ListGetItem(deviceList,&eut,eutIndex);
		if(eut.enable==0) //不参与测试的不添加进自动测试里面
			continue;
	    TESTobject *obj=createAndaddTestObject(engine,eut);   //step2 创建并且添加测试对象
		obj->onObjectPanelCreateListener=(void *)objectPanelCreate; //测试对象面板创建
		obj->onObjectPanelInitListener=(void *)objectPanelInit;   //测试对象面板初始化，主要是显示测试项目
		obj->onObjectGroupTestListener=(void *)onObjectGroupTest; //测试函数
		obj->onResultShowListener=(void *)objectResultShow;	   //测试对象结果显示
		obj->onObjectTestErrorListener=(void *)objectTestError;//测试对象出错
		obj->onObjectTestFinish=(void *)objectTestFinish;
		//obj->ON_OBJECT_TEST_FINISH_LISTENER
	}
	
	disPlayTestPanel(engine); //step3 显示测试对象面板

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
				  SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0时0分0秒"); 
				  operateTimer(1); 
			      runTestEngine(engine);//step4 测试
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
			SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0时0分0秒");    
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
			sprintf(temp,"%d时%d分%d秒",hour,min,sec);
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
/*
 tabTop:122
 TEST 1309-929=380
 REST 1309-1062=247
 BACK 1309-1189=120
 

*/
#if 1
static void reFreshAutoTestPanel(int panel)
{
			/*int monitorWidth,monitorHight,monitorTop,moinitorLeft;
			
			GetPanelAttribute (panel, ATTR_WIDTH, &monitorWidth);
            GetPanelAttribute (panel, ATTR_HEIGHT, &monitorHight);
			GetPanelAttribute (panel, ATTR_TOP, &monitorTop);
            GetPanelAttribute (panel, ATTR_LEFT, &moinitorLeft);
			
			//printf("monitortop=%d,monitorwidth=%d,monitorheight=%d,monitorleft=%d\n",monitorTop,monitorWidth,monitorHight,moinitorLeft);  
			
			//设置背景位置
			SetCtrlAttribute(panel,PANEL_AUTO_PICTURE_3,ATTR_TOP,0);
			SetCtrlAttribute(panel,PANEL_AUTO_PICTURE_3,ATTR_HEIGHT,monitorHight);
			SetCtrlAttribute(panel,PANEL_AUTO_PICTURE_3,ATTR_WIDTH,monitorWidth);
			SetCtrlAttribute(panel,PANEL_AUTO_PICTURE_3,ATTR_LEFT,0);
			
			//设置按键位置
			int top,width,height,left=0;
			SetCtrlAttribute(panel,PANEL_AUTO_TEST,ATTR_LEFT,monitorWidth-380);
			SetCtrlAttribute(panel,PANEL_AUTO_BACK,ATTR_LEFT,monitorWidth-120);
			SetCtrlAttribute(panel,PANEL_AUTO_RESTBUTTON,ATTR_LEFT,monitorWidth-247);

			//设置TAB位置
			SetCtrlAttribute(panel,PANEL_AUTO_TAB_AUTO,ATTR_HEIGHT,monitorHight-122-30);
			SetCtrlAttribute(panel,PANEL_AUTO_TAB_AUTO,ATTR_WIDTH,monitorWidth);
			SetCtrlAttribute(panel,PANEL_AUTO_TAB_AUTO,ATTR_LEFT,0);*/
			adjustPanelSize(autoPanelHandle);
			//各个子面板充值
			reSetEngine(engine);
			//reSizeTestPanel(engine);
}
#endif


int  CVICALLBACK onPanelAutoCall(int panel, int event, void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_PANEL_SIZE:
            //printf("----------zooming");
			if(engine->testState==TEST_IDLE)   //IDLE状态下才能进行大小调整
			   reFreshAutoTestPanel(panel);
			break;	
	}
	return 0;
}




