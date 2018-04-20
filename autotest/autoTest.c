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
#include "testProject.h" 
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
#include "relayProtocol.h"
#include "relayHelper.h"
#include "TpsDefaultPanel.h"
#include "sutCommon.h"
#include "resultInfo.h"


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

static TESTengine *gEngine;
static tTestProject *gTestProject;

static void operateTimer(int on);

static SETTING s;
static ENUMETestPanel gTestFlag=ENUM_TEST_PANEL_AUTO;

ENUMTestResult onObjectGroupTest(TestGroup testItem,TESTobject *_obj,TESTType type);
BOOL objectResultShow(TESTobject *obj,TestGroup group,int testGroupIndex,int *testItemIndex);


static int getObjectTestProgress(TESTobject obj)
{
	int size=0;
	TESTengine *t=obj.enginePtr;
	HashTableGetAttribute(obj.resultHashTable,ATTR_HT_SIZE,&size);
	int perc = size*100/(t->totalTestGroupCount);
	return perc;

}
static void setPercentage(TESTobject *obj)
{
	int size=0;
	TESTengine *t=obj->enginePtr;
	HashTableGetAttribute(obj->resultHashTable,ATTR_HT_SIZE,&size);
	int perc = size*100/(t->totalTestGroupCount);
	char percStr[30]={0};
	Fmt(percStr,"%d%%",perc);
	SetCtrlVal(obj->panelHandle,P_ITEMSHOW_PERCSTR,percStr);
}

static int getAccuracy(TESTobject *obj)
{
	int                 error       = 0; 
	unsigned int i;
	HashTableIterator iter;	
	int passCnt = 0;
    for (error = HashTableIteratorCreate(obj->resultHashTable, &iter), i = 1;
                 error >= 0 && error != HASH_TABLE_END;
                 error = HashTableIteratorAdvance(obj->resultHashTable, iter), ++i)
    {
		RESULT result={0};
		HashTableIteratorGetItem(obj->resultHashTable,iter,&result.index,sizeof(int),&result,sizeof(RESULT));
		if(result.pass == RESULT_PASS)
		{
			passCnt++;
		}
	}
	TESTengine *t=obj->enginePtr; 
	int perc = passCnt*100/(t->totalTestGroupCount); 
	return perc;	
}

static void setAccuracy(TESTobject *obj)
{
	int                 error       = 0; 
	unsigned int i;
	HashTableIterator iter;	
	int passCnt = 0;
    for (error = HashTableIteratorCreate(obj->resultHashTable, &iter), i = 1;
                 error >= 0 && error != HASH_TABLE_END;
                 error = HashTableIteratorAdvance(obj->resultHashTable, iter), ++i)
    {
		RESULT result={0};
		HashTableIteratorGetItem(obj->resultHashTable,iter,&result.index,sizeof(int),&result,sizeof(RESULT));
		if(result.pass == RESULT_PASS)
		{
			passCnt++;
		}
	}
	TESTengine *t=obj->enginePtr; 
	int perc = passCnt*100/(t->totalTestGroupCount); 
	char percStr[30]={0};
	Fmt(percStr,"%d%%",perc);
	SetCtrlVal(obj->panelHandle,P_ITEMSHOW_ACCURACY,percStr);	
}


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

static void saveRecordToDb(TESTobject obj,char *productId,char *startTime,char *reportFilePath)
{
	tAutoTestRecord record={0};
	/*if(0==strlen(productId))
		sprintf(record.ProductId,"%s",obj.device.eutName);
	else
		sprintf(record.ProductId,"%s",productId);*/
	record.m_id = gTestProject->dbId;
	sprintf(record.m_code,"%s",gTestProject->projectName);	
	sprintf(record.m_createtime,"%s",startTime);
	sprintf(record.m_reportpath,"%s",reportFilePath);
	sprintf(record.m_projectpath,"%s",gTestProject->projectPath);
	sprintf(record.m_lasttest,"%d",getObjectTestProgress(obj));
	record.m_result=getAccuracy(&obj);
	record.m_upload=0;
	//sprintf(record.m_name,"%s","ray");
	updateRecord(record);	
}

void CreateReportFilePath(char *customName,char *fileName,TESTobject testObj)
{

	int fileSize;
	tTestProject *projectPtr=getCurrentProject(); 
	if(FileExists(projectPtr->projectDir,&fileSize)==0)
	{	
	    MakeDir(projectPtr->projectDir);
	}
	SUT sut=GetSeletedSut(); 	
	if(customName!=NULL)
	{
		sprintf(fileName,"%s\\%s_%s_%s%s",projectPtr->projectDir,customName,sut.systemName,startTime,".xlsx");		
	}else{
		sprintf(fileName,"%s\\%s_%s_%s%s",projectPtr->projectDir,gTestProject->projectName,sut.systemName,startTime,".xlsx");	
	}	

}


void saveResultToFile(TESTobject testObj)
{
	char filePath[MAX_PATHNAME_LEN]={0};
	char stubNum[100] = {0};
	SETTING set=GetSetting();
	if(set.reportInfoCustom)
	{			
		getUserCustiomName(stubNum);
		CreateReportFilePath(stubNum,filePath,testObj);
	}else{
		CreateReportFilePath(NULL,filePath,testObj); 
	}
	saveResultToExcelFile(filePath,testObj.resultHashTable);
	if(0==FileExists(filePath,0))
	{
		WarnShow1(0,"文件保存失败");
	}
	if(set.saveToDb)
	{
		//if(strlen(stubNum)>0)
			saveRecordToDb(testObj,stubNum,startTime,filePath);	
		//else
		//	saveRecordToDb(testObj,NULL,startTime,filePath);	
	}
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
			/*if(set.reportInfoCustom)
			{			
				memset(temp,0,100);
				getUserCustiomName(temp);
				CreateReportFilePath(temp,filePath,t->objectArray[i]);
			}else{
				CreateReportFilePath(NULL,filePath,t->objectArray[i]); 
			}
			
	     	 saveTestResult(startTime,t->objectArray[i].device.eutName,t->objectArray[i].resultHashTable,filePath);
			 if(set.saveToDb)    
			 	saveRecordToDb(t->objectArray[i],startTime,filePath);*/
			saveResultToFile(t->objectArray[i]);
	  
	 }
	 DiscardPanel(panelHandle);
	 WarnShow1(0,"保存完成");
}

static BOOL ontestBeginListener(TESTengine *t)
{
	BOOL ret=TRUE;

	//各个TPS测试预先准备的工作
	//TpsPrepareTest(); 
	return ret;
}	

static void ontestFinishListener(TESTengine *t)
{
	if(t->testState==TEST_STOP)
	{
	   //operateTimer(0);	
	   
	   t->testState=TEST_IDLE;
	   setButtonState(2); 
	   WarnShow1(t->panelHandle,"已停止"); 
	   
	   
	}else if(t->testState==TEST_FINISH)
	{	
	   	
	   //operateTimer(0);	
	   t->testState=TEST_IDLE; 
	   LOG_EVENT(LOG_INFO,"测试完成");
       WarnShow1(0,"测试完成");
	   /*SETTING set=getSetting();  
	   if(!set.autoSave)
	   {
	     if(GetConfigWarnPanelRet(0,"提示","是否保存测试结果","不保存","保存")==TRUE)
	     {
		     saveAutoTestResult(t); 
		     LOG_EVENT(LOG_INFO,"保存结果完成"); 
	     }
	   }*/
	   setButtonState(2);
	}else if(t->testState==TEST_RUN)
	{
       WarnShow1(t->panelHandle,"正在测试中");
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
	
	if(gEngine->testState==TEST_RUN)
	{
       WarnShow1(panel,"正在测试中");
	   return;
		
	} 
	gEngine->testState=TEST_RUN;
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
		ListGetItem(gEngine->itemList,&group,groupId);
		//printf("groupName:%s\n",group.groupName);
		//测试单项
		TESTobject *_obj=getTestObjectByPanelHandle(panel,gEngine);
		if(_obj!=NULL)
		{
			
			int testItemIndex=0;
			_obj->seq.beginCollect = collectId;
			_obj->seq.beginGroup = testGroupIndex;	
			ENUMTestResult ret = onObjectGroupTest(group,_obj,TYPE_AUTO); 
			gEngine->currentCollect = collectId;
			gEngine->testState = TEST_IDLE;
			
			if(ret != TEST_RESULT_QUIT)
				objectResultShow(_obj,group,testGroupIndex,&testItemIndex);
		}
	}
	gEngine->testState=TEST_IDLE;
	//printf("-------tag:%d,numDescendents:%d,numParents:%d\n",getNumFromTag(tag),numDescendents,numParents); 
}


//TODO:最好不要有这种情况，因为在多台设备并行测试时候不好操作，所幸，这里只测试一台。
static void CVICALLBACK onMenuSingleObjectTestCB(int panel, int controlID, int MenuItemID, 
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
		ListGetItem(gEngine->itemList,&group,groupId);
		//printf("groupName:%s\n",group.groupName);
		//测试单项
		TESTobject *_obj=getTestObjectByPanelHandle(panel,gEngine);

		if(_obj!=NULL)
		{
			_obj->seq.beginCollect = collectId;
			_obj->seq.beginGroup = testGroupIndex;	
			setButtonState(1); 
			SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0时0分0秒"); 
			//operateTimer(1); 			
			runSingleObject(gEngine,_obj);
		}
	}
	//printf("-------tag:%d,numDescendents:%d,numParents:%d\n",getNumFromTag(tag),numDescendents,numParents); 
}



static void objectPanelCreate(int *panel)
{
    *panel=LoadPanel (gEngine->panelHandle, "AutoTestPanel.uir", P_ITEMSHOW);
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_EXPAND_SUBTREE); 
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_SEARCH);
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_SORT);  
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_COLLAPSE_SUBTREE);
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_EXPAND_ALL);
	HideBuiltInCtrlMenuItem(*panel, P_ITEMSHOW_TREE, VAL_COLLAPSE_ALL);
	NewCtrlMenuItem(*panel, P_ITEMSHOW_TREE, "单项测试", -1, onMenuSingleItemTestCB, 0);
	if(gTestFlag==ENUM_TEST_PANEL_AUTO)
		NewCtrlMenuItem(*panel, P_ITEMSHOW_TREE, "从此项开始测试", -1, onMenuSingleObjectTestCB, 0);

	int i=0;
}

BOOL objectResultShow(TESTobject *obj,TestGroup group,int testGroupIndex,int *testItemIndex)
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
	
	//int  perc=(obj->totalFinishedTestGroupCount)*100/(t->totalTestGroupCount);
	/*int size=0;
	HashTableGetAttribute(obj->resultHashTable,ATTR_HT_SIZE,&size);
	int perc = size*100/(t->totalTestGroupCount);
	char percStr[30]={0};
	Fmt(percStr,"%d%%",perc);
	SetCtrlVal(obj->panelHandle,P_ITEMSHOW_PERCSTR,percStr); */
	setPercentage(obj);
	setAccuracy(obj);
	
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
		      //TODO:获取tag,便可获取组的序号，从结果的HASHTABLE里面可以找到该值
			//上颜色，setActivie
		   //printf("itemIndex=%d\n",j);
			
			    RESULT itemResult;
				TestItem subItem;
			
		    	ListGetItem(group.subItems,&subItem,k++); 
				
			   int found = 0;
			   HashTableFindItem(obj->resultHashTable,&subItem.itemId,&found);
			   
			   if(found<=0)
				   continue;
				
				HashTableGetItem(obj->resultHashTable,&(subItem.itemId),&itemResult,sizeof(RESULT));
				
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
                  /*SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,1,ATTR_LABEL_BGCOLOR ,VAL_GREEN);	
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,2,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_BGCOLOR ,VAL_GREEN);*/
				  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,VAL_ALL_OBJECTS,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
				  SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_TEXT ,tempStr);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,4,ATTR_LABEL_TEXT ,"合格");

				  
		       }else
		       {
		          SetActiveTreeItem(obj->panelHandle,P_ITEMSHOW_TREE,j,VAL_REPLACE_SELECTION_WITH_ITEM);  //选中当前测试项
		          SetTreeItemAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,ATTR_LABEL_BGCOLOR ,VAL_RED);
                  /*SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,1,ATTR_LABEL_BGCOLOR ,VAL_RED);	
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,2,ATTR_LABEL_BGCOLOR ,VAL_RED);
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,3,ATTR_LABEL_BGCOLOR ,VAL_RED);*/
		          SetTreeCellAttribute(obj->panelHandle, P_ITEMSHOW_TREE,j,VAL_ALL_OBJECTS,ATTR_LABEL_BGCOLOR ,VAL_RED);
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
		      ListGetItem(groupList,&num,count1);//获取组的ID号
			 
			  ListGetItem(t->itemList,&group,num);//获取组
			  sprintf(groupIndexStr,"%d",num);
			  
			  //PRINT("groupIndexStr=%s\n",groupIndexStr);
			  
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
			   int found = 0;
			   HashTableFindItem(_obj->resultHashTable,&testItem.itemId,&found);
			   if(found > 0)
			   {
			   		RESULT result ={0};
					HashTableGetItem(_obj->resultHashTable,&testItem.itemId,&result,sizeof(RESULT));
					if(result.pass)
		       		{	   
		          		//SetActiveTreeItem(_obj->panelHandle,P_ITEMSHOW_TREE,j,VAL_REPLACE_SELECTION_WITH_ITEM);  //选中当前测试项
		          		SetTreeItemAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,index-1,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
                  		SetTreeCellAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,index-1,VAL_ALL_OBJECTS,ATTR_LABEL_BGCOLOR ,VAL_GREEN);	
				  		SetTreeCellAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,index-1,3,ATTR_LABEL_TEXT ,result.recvString);
		          		SetTreeCellAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,index-1,4,ATTR_LABEL_TEXT ,"合格");

				  
		      		}else
		       		{
		         		//SetActiveTreeItem(_obj->panelHandle,P_ITEMSHOW_TREE,j,VAL_REPLACE_SELECTION_WITH_ITEM);  //选中当前测试项
		          		SetTreeItemAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,index-1,ATTR_LABEL_BGCOLOR ,VAL_RED);
                  		SetTreeCellAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,index-1,VAL_ALL_OBJECTS,ATTR_LABEL_BGCOLOR ,VAL_RED);	
				  		SetTreeCellAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,index-1,3,ATTR_LABEL_TEXT ,result.recvString);
		          		SetTreeCellAttribute(_obj->panelHandle, P_ITEMSHOW_TREE,index-1,4,ATTR_LABEL_TEXT ,"不合格");
					
			   		}
			   
				}  			   
			}  
		}	
	}
	setPercentage(_obj);
	setAccuracy(_obj);
	SETTING set=getSetting();
	SetTreeCellAttribute (panelHandle,P_ITEMSHOW_TREE, VAL_ALL_OBJECTS ,  VAL_ALL_OBJECTS , ATTR_LABEL_POINT_SIZE,set.frontSize);
		

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
	//tAutoTestRecord record;
	SETTING set=getSetting();
	if(set.autoSave)
	{
	   /*char temp[MAX_PATHNAME_LEN]={0}; 
	   //TODO:后续要加上
	   saveTestResult(startTime,_obj->device.eutName,_obj->resultHashTable,temp);
	   if(set.saveToDb)
	   	saveRecordToDb(*_obj,startTime,temp);*/
	   saveResultToFile(*_obj);
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
		//sprintf(startTime,"%d年%02d月%02d日%2d时%02d分%02d秒",year,month,day,hour,min,sec);
		sprintf(startTime,"%d%02d%02d%02d%02d",year,month,day,hour,min); 
		SetCtrlAttribute(autoPanelHandle,PANEL_AUTO_TIMER,ATTR_INTERVAL,0.5);  //开始 
	}else
	{
	    //SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0时0分0秒"); 
	}	
	SetCtrlAttribute(autoPanelHandle,PANEL_AUTO_TIMER,ATTR_ENABLED,on); 
	//SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0分0秒");
}

void SetActiveTestGroup(TESTobject *_obj)
{
	//获取到collect的具体index
	int collectIndex=0;

	 
	for(int i=1;i<_obj->seq.beginCollect;i++)
    {		
		GetTreeItem(_obj->panelHandle,P_ITEMSHOW_TREE,VAL_SIBLING,0,collectIndex,VAL_NEXT,0,&collectIndex); 
	}
    int groupCount;
	GetNumTreeItems(_obj->panelHandle,P_ITEMSHOW_TREE,VAL_CHILD,collectIndex,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&groupCount);	
	int groupIndex=collectIndex+1;
	//PRINT("groupIndex=%d\n",groupIndex); 
	//PRINT("total:%d,current:%d,%s",t->totalTestGroupCount,obj->totalFinishedTestGroupCount,percStr);
	
	for(int i=1;i<=groupCount;i++)
	{
		//获取条例的数量
		int itemCount;
		GetNumTreeItems(_obj->panelHandle,P_ITEMSHOW_TREE,VAL_CHILD,groupIndex,VAL_FIRST,VAL_NEXT_PLUS_SELF,0,&itemCount);
		
		//SetActiveTreeItem(_obj->panelHandle,P_ITEMSHOW_TREE,groupIndex,VAL_REPLACE_SELECTION_WITH_ITEM);
		//break;
		int itemIndex=groupIndex+1;
		
		//获取组结果
		char groupIndexStr[32]={0}; 
		GetTreeItemTag(_obj->panelHandle,P_ITEMSHOW_TREE,groupIndex,groupIndexStr);
			
		int groupNum=atoi(groupIndexStr);
		if(i==_obj->seq.beginGroup)
		{	
			SetActiveTreeItem(_obj->panelHandle,P_ITEMSHOW_TREE,itemIndex,VAL_REPLACE_SELECTION_WITH_ITEM);
		}
		GetTreeItem(_obj->panelHandle,P_ITEMSHOW_TREE,VAL_SIBLING,groupIndex,groupIndex,VAL_NEXT,0,&groupIndex);
	}
}




extern METHODRET manualTest(TestGroup group,EUT eut,HashTableType hashTable);
ENUMTestResult onObjectGroupTest(TestGroup testItem,TESTobject *_obj,TESTType type)
{
	 	ENUMTestResult ret=TEST_RESULT_ALL_PASS;
		BOOL testFlag=TRUE;
		METHODRET testRet= TEST_ERROR; 
		
		SetActiveTestGroup(_obj);
		
		SETTING set=getSetting();
		//提示
		if(set.collectTestMention || gTestFlag == ENUM_TEST_PANEL_MANUAL)
		{	
		   char temp[256]={0};
		   char temp1[256]={0};
		   if(getTip(testItem.groupName,temp1)>0)
		   {
		       sprintf(temp,"%s:%s,准备测试!",_obj->device.eutName,temp1);   
		   }else{	   
		       sprintf(temp,"%s:%s,准备测试!",_obj->device.eutName,testItem.groupName);
		   }
		   //WarnShow1(0,temp);
		   TESTengine *engine = (TESTengine*)_obj->enginePtr;
		   //ShowManualTip(0,testItem.groupName,temp1);
		   //提示面板可以控制是否进行下一项测试
		   Collect collect={0};
		   ListGetItem(engine->collectList,&collect,engine->currentCollect); 
		   if(collect.tipFlag==1)
		   {
		   		testFlag=showTips(0,testItem.groupName,temp1,&((TESTengine*)_obj->enginePtr)->testState,collect.tipsCloseTime);
		   }
		} 
		

		
		if(FALSE == testFlag)
			return TEST_RESULT_QUIT;//注意返回值这里，应该让整个引擎停止，暂时返回ERROR
		
		if(((TESTengine*)_obj->enginePtr)->testState==TEST_STOP){
			 return TEST_RESULT_QUIT;
		}		
		LOG_EVENT_FORMAT(LOG_INFO,"-----Enter GroupTest:%s-----",testItem.groupName);  		
		//继电器操作
		RelayOperate operate={0};
		if(set.relayEnable)
		{
			if(getRelayMask(testItem.groupName,&operate)){
		
			//PRINT("%x\n",mask);
			//OperatDoSet(_obj->device.testInstrument
				if(OperatDoSet(_obj->device.relayConfig,operate.beforeTestMask,operate.mask)==FALSE)
				{
					WarnShow1(0,"操作继电器失败");
					((TESTengine*)_obj->enginePtr)->testState=TEST_STOP;
				}
			}
		}
		
		if(((TESTengine*)_obj->enginePtr)->testState==TEST_STOP){
			 goto DONE;
		}
		
		for(int itemIndex=1;itemIndex<=ListNumItems(testItem.subItems);itemIndex++)
		{
			  TestItem item={0};
			  ListGetItem(testItem.subItems,&item,itemIndex);
			  RESULT result={0};
			  result.index = item.itemId;
			  saveResult(_obj->resultHashTable,&result);
		}
		
	    /*TEST_METHOD method=getTestMethod(testItem); //获取测试方法
		//LOG_LEVEL("Debug","%x\n",_obj->resultHashTable);
		METHODRET testRet= (*method)(testItem,_obj->device,_obj->resultHashTable);*/
		TPS tps;
		//memset(&tps,0,sizeof(tps));
		 
		if(getTps(testItem.type,&tps))
		{
			//printf("%s",tps.tpsName);
			
			if(set.showProcess){
				if(tps.createTpsPanel!=NULL)
				{
					 tps.tpsPanelHandle = (*(tps.createTpsPanel))(testItem.groupName);
				}
			
				if(tps.tpsPanelHandle>0)
				{
					DisplayPanel(tps.tpsPanelHandle);
				}
			}

			if(type == TYPE_AUTO){
				
				if(tps.testFunction!=NULL)
				{
					testRet = (*(TEST_FUNCTION)(tps.testFunction))(testItem,_obj->device,_obj->resultHashTable,tps.tpsPanelHandle); 
				}else if(tps.autoTestFunction!=NULL)
				{
				
					testRet = (*(TEST_METHOD)(tps.autoTestFunction))(testItem,_obj->device,_obj->resultHashTable);
				}
			}else{
				/*if(tps.manualTestFunction==NULL)
				{
					//WarnShow1(0,"没有定义手动测试的方法");
					testRet =manualTest(testItem,_obj->device,_obj->resultHashTable);
				}else{
					testRet = (*(TEST_METHOD)(tps.manualTestFunction))(testItem,_obj->device,_obj->resultHashTable); 
					//PRINT("%d\n",testRet);
				}*/
				testRet =manualTest(testItem,_obj->device,_obj->resultHashTable); 
				
			}
			if(set.showProcess){   			
				if(tps.tpsPanelHandle > 0)
				{
					//RunUserInterface();
					Delay(2);
					DiscardPanel(tps.tpsPanelHandle);
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
		    //WarnShow1(0,"不支持的TPS类型，请人工填写测试数据和结果"); 
			//不支持的用默认弹框来测试
			testRet =manualTest(testItem,_obj->device,_obj->resultHashTable);
		   	if(testRet==TEST_RESULT_ALLPASS)
		    	ret=TEST_RESULT_ALL_PASS;
	     	else if(testRet==TEST_RESULT_ERROR)
		    	ret=TEST_ERROR;
	     	else if(testRet=TEST_RESULT_SOMEPASS)
		    	ret=TEST_RESULT_SOME_PASS; 			
		}
		
		if(set.relayEnable)
		{
			if(getRelayMask(testItem.groupName,&operate)){
				if(OperatDoSet(_obj->device.relayConfig,operate.afterTestMask,operate.mask)==FALSE)
				{
					WarnShow1(0,"操作继电器失败");
					((TESTengine*)_obj->enginePtr)->testState=TEST_STOP;
				}
			}
		}
		saveResultInfo(gEngine,gTestProject->projectName);   //TODO:暂时这样处理，这样操作文件过于频繁
DONE:		
		LOG_EVENT_FORMAT(LOG_INFO,"-----Leave GroupTest:%s-----",testItem.groupName);   		
		
		if(((TESTengine*)_obj->enginePtr)->testState==TEST_STOP){
			 return TEST_ERROR;
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
	SetCtrlAttribute(panel,PANEL_AUTO_TEST,ATTR_LEFT,autoPanelWidth-(4*AUTOTEST_BUTTON_PADDING_SIZE+4*buttonSize));
	//第二个图标
	SetCtrlAttribute(panel,PANEL_AUTO_GENREPORT,ATTR_TOP,AUTOTEST_BUTTON_PADDING_SIZE);
	SetCtrlAttribute(panel,PANEL_AUTO_GENREPORT,ATTR_LEFT,autoPanelWidth-(3*AUTOTEST_BUTTON_PADDING_SIZE+3*buttonSize));	
	//第三个图标
	SetCtrlAttribute(panel,PANEL_AUTO_RESTBUTTON,ATTR_TOP,AUTOTEST_BUTTON_PADDING_SIZE);
	SetCtrlAttribute(panel,PANEL_AUTO_RESTBUTTON,ATTR_LEFT,autoPanelWidth-(2*AUTOTEST_BUTTON_PADDING_SIZE+2*buttonSize));	
	//第四个图标
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

int CheckResultTable(ListType itemList,ListType collectList,HashTableType resultHashTable)
{
	int totalTestItems=0;
	HashTableType table=0; 
	HashTableCreate(10,FIXED_SIZE_KEY,sizeof(int),sizeof(int),&table);
	 for(int i=1;i<=ListNumItems(collectList);i++)
	 {
	     Collect collect;
		 ListGetItem(collectList,&collect,i);
		 //t->totalTestGroupCount+=ListNumItems(collect.groups);
		 for(int j=1;j<=ListNumItems(collect.groups);j++)
		 {
		 	  int groupIndex=0;

			  ListGetItem(collect.groups,&groupIndex,j);//获取组的ID号
			  TestGroup testGroup={0};
		      ListGetItem(itemList,&testGroup,groupIndex);//获取组	
			  for(int z=1;z<=ListNumItems(testGroup.subItems);z++)
			  {
				  TestItem item={0};
				  ListGetItem(testGroup.subItems,&item,z);
				  HashTableInsertItem(table,&item.itemId,&item.itemId);		  		
			  }
			  //totalTestItems+=ListNumItems(testGroup.subItems);
		 }
	 }
     HashTableGetAttribute(table,ATTR_HT_SIZE,&totalTestItems);
	
	 HashTableIterator iter;
	 int status = 0;

	int found = 0;
	int key;  
	RESULT result={0};	 
	ListType removeItemList = ListCreate(sizeof(int));
	for (status = HashTableIteratorCreate(resultHashTable, &iter);status >= 0 && status != HASH_TABLE_END;status = HashTableIteratorAdvance(resultHashTable, iter)) 
    {

		status = HashTableIteratorGetItem(resultHashTable, iter, &key, sizeof(int), &result, sizeof(RESULT)); 
		/* do something with key and value */ 

		HashTableFindItem(table,&key,&found);
		if(found<=0)
		{
			//HashTableRemoveItem(resultHashTable,&key,&result,sizeof(RESULT));
			ListInsertItem(removeItemList,&key,END_OF_LIST);
		}

	}

	HashTableIteratorDispose(resultHashTable, iter); 
	
	for(int i=1;i<=ListNumItems(removeItemList);i++)
	{
		//int key=0;
		//RESULT result={0};
		ListGetItem(removeItemList,&key,i);
		//HashTableGetItem(resultHashTable,&key,&result,sizeof(RESULT));
		int ret = HashTableRemoveItem(resultHashTable,&key,&result,sizeof(RESULT));			
	}
	
	ListDispose(removeItemList);	
	HashTableDispose(table);
	return  totalTestItems;
}



void DisplayAutoTestPanelWithTestData(ListType groupList,ListType deviceList,ListType collectList,ENUMETestPanel type,tTestProject *testProjectPtr)
{
	
	//memcpy(&gTestProject,&testProject,sizeof(tTestProject));
	gTestProject=testProjectPtr;
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
#if 0 //协议解析不放到这里，放到tps初始化后面	
	s=GetSetting();
	
	if(s.collectTestMention) //如果允许提示，解析提示
	{
		getTipsFromExcel();
	}
#endif	
	gEngine=createTestEngine(tempPanel,groupList,collectList);//step1 创建基于策略的测试
	gEngine->onTestBeginListener=(void *)ontestBeginListener;   //测试启动回调函数
	gEngine->onTestFinishListener=(void *)ontestFinishListener; //测试完成后回调函数
	SETTING s=GetSetting();
	gEngine->maxObjectPanelCountPerRow=s.maxComunPerRowInAutoTestPanel;
	gEngine->reTestCnt=s.reTestCnt;


	for(int eutIndex=1;eutIndex<=ListNumItems(deviceList);eutIndex++)
	{
		EUT eut;

		ListGetItem(deviceList,&eut,eutIndex);
		if(eut.enable==0) //不参与测试的不添加进自动测试里面
			continue;
	    TESTobject *obj=createAndaddTestObject(gEngine,eut);   //step2 创建并且添加测试对象
		obj->onObjectPanelCreateListener=(void *)objectPanelCreate; //测试对象面板创建
		obj->onObjectPanelInitListener=(void *)objectPanelInit;   //测试对象面板初始化，主要是显示测试项目
		obj->onObjectGroupTestListener=(void *)onObjectGroupTest; //测试函数
		obj->onResultShowListener=(void *)objectResultShow;	   //测试对象结果显示
		obj->onObjectTestErrorListener=(void *)objectTestError;//测试对象出错
		obj->onObjectTestFinish=(void *)objectTestFinish;
		loadResultInfo(gTestProject->projectPath,obj->device.eutName,obj->resultHashTable);
		//TODO 删除不属于当前集合的测试项
		CheckResultTable(groupList,collectList,obj->resultHashTable);
	}
	SUT sut=GetSeletedSut();
	char currentSutName[50]={0};
	sprintf(currentSutName,"当前测试项目:%s",sut.nickName);
	SetCtrlVal(autoPanelHandle,PANEL_AUTO_CURRENT_SUT,currentSutName);
	SetCtrlVal(autoPanelHandle,PANEL_AUTO_TEST_PROJECT,gTestProject->projectName);	
	disPlayTestPanel(gEngine); //step3 显示测试对象面板
	operateTimer(1);

}

BOOL createNewTestProject()
{
	return TRUE;	
}

void DisplayAutoTestPanel(ListType groupList,ListType deviceList,ListType collectList,ENUMETestPanel type,tTestProject *testProjectPtr)
{
	
	/*if(FALSE==createNewTestProject())
	{
		return;		
	}*/
	//memcpy(&gTestProject,&testProject,sizeof(tTestProject));	
	gTestProject= testProjectPtr;
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
	SUT sut=GetSeletedSut();
	char currentSutName[50]={0};
	sprintf(currentSutName,"当前测试项目:%s",sut.nickName);
	SetCtrlVal(autoPanelHandle,PANEL_AUTO_CURRENT_SUT,currentSutName);	
	SetCtrlVal(autoPanelHandle,PANEL_AUTO_TEST_PROJECT,gTestProject->projectName);	
	adjustPanelSize(autoPanelHandle);
	DisplayPanel(autoPanelHandle);	
#if 0 //协议解析不放到这里，放到tps初始化后面	
	s=GetSetting();
	
	if(s.collectTestMention) //如果允许提示，解析提示
	{
		getTipsFromExcel();
	}
#endif	
	gEngine=createTestEngine(tempPanel,groupList,collectList);//step1 创建基于策略的测试
	gEngine->onTestBeginListener=(void *)ontestBeginListener;   //测试启动回调函数
	gEngine->onTestFinishListener=(void *)ontestFinishListener; //测试完成后回调函数
	SETTING s=GetSetting();
	gEngine->maxObjectPanelCountPerRow=s.maxComunPerRowInAutoTestPanel;
	gEngine->reTestCnt=s.reTestCnt;


	for(int eutIndex=1;eutIndex<=ListNumItems(deviceList);eutIndex++)
	{
		EUT eut;

		ListGetItem(deviceList,&eut,eutIndex);
		if(eut.enable==0) //不参与测试的不添加进自动测试里面
			continue;
	    TESTobject *obj=createAndaddTestObject(gEngine,eut);   //step2 创建并且添加测试对象
		obj->onObjectPanelCreateListener=(void *)objectPanelCreate; //测试对象面板创建
		obj->onObjectPanelInitListener=(void *)objectPanelInit;   //测试对象面板初始化，主要是显示测试项目
		obj->onObjectGroupTestListener=(void *)onObjectGroupTest; //测试函数
		obj->onResultShowListener=(void *)objectResultShow;	   //测试对象结果显示
		obj->onObjectTestErrorListener=(void *)objectTestError;//测试对象出错
		obj->onObjectTestFinish=(void *)objectTestFinish;
		//obj->ON_OBJECT_TEST_FINISH_LISTENER
	}
			
	disPlayTestPanel(gEngine); //step3 显示测试对象面板
	operateTimer(1);

}






int CVICALLBACK STARTTEST (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			 if(gEngine->testState==TEST_IDLE)
			 {	   
				if(gEngine->totalTestObject!=0)
				{	
				  setButtonState(1); 
				  SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0时0分0秒"); 
				  //operateTimer(1); 
			      runTestEngine(gEngine);//step4 测试
				}
			 }else
			 {
			    gEngine->testState=TEST_STOP;
			 } 
		     break;
	}
	return 0;
}

void resetResult(TESTengine *engine)
{
	/*Tint groupCnt=0;
	int itemCnt=0;
	estGroup testGroup={0};
	TestItem testItem={0};
	groupCnt = ListNumItems(engine->itemList);
	for(int i=1;i<=groupCnt;i++)
	{
		ListGetItem(engine->itemList,&testGroup,i);
		itemCnt = ListNumItems(testGroup.subItems);
		for(int j=1;j<=itemCnt;j++)
		{
			ListGetItem(testGroup.subItems,&testItem,j);
			RESULT result = {0};
			result.index = testItem.itemId;
			result.pass = 0;
			for(int z=0;z<engine->totalTestObject;z++)
			{
				saveResult(engine->objectArray[z].resultHashTable,&result); 
			}
						
		}
	}*/
	for(int z=0;z<gEngine->totalTestObject;z++)
	{
		HashTableClear(gEngine->objectArray[z].resultHashTable);	
	}
}


int CVICALLBACK RESET (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	
	switch (event)
	{
		case EVENT_COMMIT:
			//operateTimer(0);
			if(AlertDialogWithRet(0,"警告","重置会导致所有结果丢失，是否继续","继续","不继续"))
			{
				return 0;
			}
		
			resetResult(gEngine);
			reSetEngine(gEngine);
			
			setButtonState(3);
			SetCtrlVal (autoPanelHandle, PANEL_AUTO_TEXTMSG_3,"0时0分0秒");    
		    SetCtrlAttribute (autoPanelHandle, PANEL_AUTO_TEST, ATTR_DIMMED ,0); 
			WarnShow1(0,"重置完成");
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
			//ReleaseStubNetService();//断开与充电桩的连接
			if(gEngine->testState == TEST_RUN)
			{
				WarnShow1(0,"正在测试中！稍后再试");
				return 0;
			}
			saveResultInfo(gEngine,gTestProject->projectName);
			releaseTestEngine(gEngine);
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
	reSetEngine(gEngine);
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
	     if(AlertDialogWithRet(0,"提示","是否保存测试结果","不保存","保存")==TRUE)
	     {
			 if(gTestFlag == ENUM_TEST_PANEL_MANUAL) //手动测试，这个时间要自己生成
			 {
				unsigned int year, month, day, hour, min, sec, weekDay;
				CVIAbsoluteTime absTime;
				memset(startTime,0,100);
				GetCurrentCVIAbsoluteTime(&absTime);
        		CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
                	&min, &sec, 0, &weekDay);
				sprintf(startTime,"%d年%02d月%02d日%2d时%02d分%02d秒",year,month,day,hour,min,sec); 
			 }
		     saveAutoTestResult(gEngine);
			 //soundFinish();
			 
		     LOG_EVENT(LOG_INFO,"完成"); 
	     }
				 
		break;
	}
	return 0;
}
