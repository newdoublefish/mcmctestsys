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
#include "windows.h" 
#include <userint.h>
#include <pathctrl.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "testFramework.h"
#include "AutoTestPanel.h" 
#include "convertTool.h"
#include "threadFramework.h"
#include "debug.h"
#include "resultUtil.h"
 

#include "log.h"


BOOL JudgeTestProcess(TESTengine *t)
{
	BOOL ret=FALSE;
	if(t->testState==TEST_STOP)
	{
	     ret=FALSE;
	}else
	{
	     ret=TRUE;
	}	
	return ret;
}


void reSizeTestPanel(TESTengine *t)
{
    int monitorHight;
    int monitorWidth; //屏幕宽度
	int panelRows;  //面板显示多少行
	int deviceCount;  //测试设备个数
	int panelHeight=0;
	int panelWidth=0;
    int panelCurrentRows;
	int maxCountsPerRow=t->maxObjectPanelCountPerRow;
	
	GetPanelAttribute (t->panelHandle, ATTR_WIDTH, &monitorWidth);
    GetPanelAttribute (t->panelHandle, ATTR_HEIGHT, &monitorHight);
	
	deviceCount= t->totalTestObject;
	panelRows=deviceCount/maxCountsPerRow;//
	
	if(panelRows*maxCountsPerRow<deviceCount)
	{
	   panelRows=panelRows+1;
	}						   //防止显示不足

	//panelHeight=(monitorHight-100)/panelRows;
	 panelHeight=(monitorHight)/panelRows;
	if(deviceCount<maxCountsPerRow)
	{
	    panelWidth=(monitorWidth)/deviceCount; 
	}else
	{	
	    panelWidth=(monitorWidth)/maxCountsPerRow;
	}
	
	
	for(int deviceNum=1;deviceNum<=deviceCount;deviceNum++)
	{
		char temp[30];
		int width=0;
		int hight=0;
		panelCurrentRows=deviceNum/maxCountsPerRow;
		if(panelCurrentRows*maxCountsPerRow<deviceNum)
		{
		    panelCurrentRows=panelCurrentRows+1;
		}
		
		//if(t->objectArray[deviceNum-1].onObjectPanelCreateListener!=NULL)
		//{	
	    	//(*(ON_OBJECT_PANEL_CREATE_LISTENER)(t->objectArray[deviceNum-1].onObjectPanelCreateListener))(&(t->objectArray[deviceNum-1].panelHandle)); //创建测试面板
	      	SetPanelSize(t->objectArray[deviceNum-1].panelHandle,panelHeight,panelWidth);
		
	    	hight=(panelCurrentRows-1)*panelHeight;
	    	if(deviceNum%maxCountsPerRow==0)
	    	{
				if(maxCountsPerRow==1)
				{
				   width=panelWidth;
				}else
				{	
				   width+=panelWidth;
				}   
		    }else
		    {	
		       width= panelWidth*(deviceNum%maxCountsPerRow-1);
	    	}
            SetPanelPos(t->objectArray[deviceNum-1].panelHandle,hight, width);
		
            SetPanelAttribute (t->objectArray[deviceNum-1].panelHandle, ATTR_SCALE_CONTENTS_ON_RESIZE,
                               1);
            SetPanelAttribute (t->objectArray[deviceNum-1].panelHandle, ATTR_MIN_WIDTH_FOR_SCALING,
                               1);
            SetPanelAttribute (t->objectArray[deviceNum-1].panelHandle, ATTR_MIN_HEIGHT_FOR_SCALING,
			                  1);
	    	//if(t->objectArray[deviceNum-1].onObjectPanelInitListener!=NULL)
			//   (*(ON_OBJECT_PANEL_INIT_LISTENER)(t->objectArray[deviceNum-1].onObjectPanelInitListener))(&(t->objectArray[deviceNum-1]));	//显示内容
		
	    	//DisplayPanel(t->objectArray[deviceNum-1].panelHandle);	   //统一显示，没有停滞感 
		//}
	}

}


void disPlayTestPanel(TESTengine *t)
{
    int monitorHight;
    int monitorWidth; //屏幕宽度
	int panelRows;  //面板显示多少行
	int deviceCount;  //测试设备个数
	int panelHeight=0;
	int panelWidth=0;
    int panelCurrentRows;
	int maxCountsPerRow=t->maxObjectPanelCountPerRow; 
	
	GetPanelAttribute (t->panelHandle, ATTR_WIDTH, &monitorWidth);
    GetPanelAttribute (t->panelHandle, ATTR_HEIGHT, &monitorHight);
	
	deviceCount= t->totalTestObject;
	if(deviceCount==0)
	{
		return;
	}
	panelRows=deviceCount/maxCountsPerRow;//每行最多显示4个
	
	//PRINT("monitorWidth:%d,monitorHight:%d\n",monitorWidth,monitorHight);
	
	if(panelRows*maxCountsPerRow<deviceCount)
	{
	   panelRows=panelRows+1;
	}						   //防止显示不足

	//panelHeight=(monitorHight-100)/panelRows;
	 panelHeight=(monitorHight)/panelRows;
	if(deviceCount<maxCountsPerRow)
	{
	    panelWidth=(monitorWidth)/deviceCount; 
	}else
	{	
	    panelWidth=(monitorWidth)/maxCountsPerRow;
	}
	
	//PRINT("panelWidth:%d,panelHeight:%d\n",panelWidth,panelHeight);  
	
	
	for(int deviceNum=1;deviceNum<=deviceCount;deviceNum++)
	{
		char temp[30];
		int width=0;
		int hight=0;
		panelCurrentRows=deviceNum/maxCountsPerRow;
		if(panelCurrentRows*maxCountsPerRow<deviceNum)
		{
		    panelCurrentRows=panelCurrentRows+1;
		}
		
		if(t->objectArray[deviceNum-1].panelHandle!=0)  //注意：如果存在则先释放
		{	
		    DiscardPanel(t->objectArray[deviceNum-1].panelHandle);
		}  
		if(t->objectArray[deviceNum-1].onObjectPanelCreateListener!=NULL)
		{	
	    	(*(ON_OBJECT_PANEL_CREATE_LISTENER)(t->objectArray[deviceNum-1].onObjectPanelCreateListener))(&(t->objectArray[deviceNum-1].panelHandle)); //创建测试面板
	      	SetPanelSize(t->objectArray[deviceNum-1].panelHandle,panelHeight,panelWidth);
		
	    	hight=(panelCurrentRows-1)*panelHeight;
	    	if(deviceNum%maxCountsPerRow==0)
	    	{
			   //width= panelWidth*3;
				if(maxCountsPerRow==1)
				{
				   width=panelWidth;
				}else
				{	
				   width+=panelWidth;
				}   
		    }else
		    {	
		       width= panelWidth*(deviceNum%maxCountsPerRow-1);
	    	}
			//PRINT("devieNum:%d,deviceCount:%d,testPanel hight:%d,width:%d\n",deviceNum,deviceCount,hight,width);
            SetPanelPos(t->objectArray[deviceNum-1].panelHandle,hight, width);
		
            SetPanelAttribute (t->objectArray[deviceNum-1].panelHandle, ATTR_SCALE_CONTENTS_ON_RESIZE,
                               1);
            SetPanelAttribute (t->objectArray[deviceNum-1].panelHandle, ATTR_MIN_WIDTH_FOR_SCALING,
                               1);
            SetPanelAttribute (t->objectArray[deviceNum-1].panelHandle, ATTR_MIN_HEIGHT_FOR_SCALING,
			                  1);
	    	if(t->objectArray[deviceNum-1].onObjectPanelInitListener!=NULL)
			   (*(ON_OBJECT_PANEL_INIT_LISTENER)(t->objectArray[deviceNum-1].onObjectPanelInitListener))(&(t->objectArray[deviceNum-1]));	//显示内容
		
	    	DisplayPanel(t->objectArray[deviceNum-1].panelHandle);	   //统一显示，没有停滞感 
		}
	}
}




TESTobject *createAndaddTestObject(TESTengine *t,EUT eut)
{
	  
	  TESTobject * obj= &(t->objectArray[t->totalTestObject]);
	  memcpy(&(obj->device),&eut,sizeof(EUT));
	  obj->panelHandle=0;
	  obj->enginePtr=t;
	  obj->onObjectPanelInitListener=NULL;
	  obj->onResultShowListener=NULL;
	  obj->onObjectPanelCreateListener=NULL; 
	  obj->onObjectTestErrorListener=NULL;
	  obj->onObjectGroupTestListener=NULL;
	  obj->onObjectCollectTestListener=NULL;
	  obj->onObjectTestFinish=NULL;
	  
	  obj->objectIndex=t->totalTestObject;
	  obj->totalFinishedTestGroupCount=0;
	  
	  obj->errorFlag=0;
	  createResultHashTable(&(obj->resultHashTable));
	  //LOG_LEVEL("Debug","%x\n",obj->resultHashTable);   
	  t->totalTestObject++;
	  return obj;
}

int CalcTotalTestItemsByHash(ListType itemList,ListType collectList)
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
	 HashTableDispose(table);
	 return  totalTestItems;
}

int CalcTotalTestItems(ListType itemList,ListType collectList)
{
	int totalTestItems=0;
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
			  totalTestItems+=ListNumItems(testGroup.subItems);
		 }
	 }
	 return  totalTestItems;
}

TESTengine *createTestEngine(int panelHandle,ListType itemList,ListType collectList)
{
     TESTengine *t=(TESTengine *)malloc(sizeof(TESTengine));
	 memset(t,sizeof(TESTengine),0);
	 t->collectList=collectList;
	 t->itemList=itemList;
	 t->panelHandle=panelHandle;
	 t->totalTestObject=0;
	 t->currentCollect=0;
	 t->testState=TEST_IDLE;
	 t->onTestBeginListener=NULL;
	 t->onTestFinishListener=NULL;
	 t->totalTestGroupCount=0;
	 t->reTestCnt=1;
	 t->totalTestGroupCount= CalcTotalTestItemsByHash(itemList,collectList);
	 return t;
}


static int CVICALLBACK DoTestFunction(void * data)
{
	
	 Collect collect={0};
	 int currentCollect=-1;
	 TESTobject *obj=(TESTobject *)data;
	 if(obj->errorFlag==1)
	 {
	     goto Done;
	 }	 
	 
	 currentCollect= obj->seq.beginCollect;
	 ListGetItem(((TESTengine *)(obj->enginePtr))->collectList,&collect,currentCollect);
	 
	
    for(;obj->seq.beginGroup<=ListNumItems(collect.groups);obj->seq.beginGroup++)
	{
		//PRINT("collect:%d,group:%d\n",currentCollect,i);
		BOOL retryFlag=FALSE;
		int groupIndex;
		int itemIndex=0;  //重第几个条例开始显示
		TestGroup testItem;   
		ListGetItem(collect.groups,&groupIndex,obj->seq.beginGroup);//获取组的ID号
		ListGetItem(((TESTengine *)(obj->enginePtr))->itemList,&testItem,groupIndex);//获取组
		obj->totalFinishedTestGroupCount+=1;  //已完成测试组数量加1

		//int retryCount=0;
		do
		{	
			  
			  
			  //float startTime=Timer();
			  //float endTime=startTime;
			  
			  int retryCount=0;
			  int maxRetryCount=0;
			  if(collect.retryCount>0)
			  {
			      maxRetryCount=collect.retryCount;
			  }else
			  {
			      maxRetryCount=((TESTengine *)(obj->enginePtr))->reTestCnt;   
			  }
			  
			  while(retryCount<maxRetryCount) //在20S内会循环测试，直到测试正确，超出20S跳出。
			  {	  
				 //printf("%d,%d\n",retryCount,((TESTengine *)(obj->enginePtr))->reTestCnt);
		         if(!JudgeTestProcess((TESTengine *)(obj->enginePtr)))  //如果为完成，或者是重置，则跳出   
		         {
			         goto Done;
		         }	
				  
		         /*if(DoItemTest(obj,testItem)==TRUE)
				 {
				     break;
				 }	 //单组测试*/
				 //TEST_METHOD method=getTestMethod(testItem.itemType); //获取测试方法
				 //METHODRET testRet= (*method)(testItem,obj->device,obj->resultHashTable);
				 ENUMTestResult testRet= TEST_ERROR;    
				 if(obj->onObjectGroupTestListener!=NULL)
				 {
					   //LOG_LEVEL("Debug","%x\n",obj->resultHashTable);
					   testRet=(*(ON_OBJECT_GROUP_TEST_LISTENER)(obj->onObjectGroupTestListener))(testItem,obj,collect.tType);	
			     }		   
				 if(testRet==TEST_RESULT_ALL_PASS)
				 {
				     break; //测试成功,不在进行测试，否则重测
				 }else if(testRet==TEST_ERROR) 
				 {
					 if(obj->onObjectTestErrorListener!=NULL)
						   (*(ON_OBJECT_TEST_ERROR_LISTENER)(obj->onObjectTestErrorListener))(obj);
					 LOG_EVENT_FORMAT(LOG_WARING,"UUT:%s:集合:%s,项目:%s,%s\n",obj->device.eutName,collect.name_,testItem.nickName,"测试故障");
				     goto Done;
				 }
				 retryCount++;
				 //MessagePopup();
				 //endTime=Timer();
			  }
			  if(obj->onResultShowListener!=NULL)
			       retryFlag=! (*(ON_RESULT_SHOW_LISTENER)(obj->onResultShowListener))(obj,testItem,obj->seq.beginGroup,&itemIndex);
			  
		}while(retryFlag);

		
	}
Done:	
	 if(obj->onObjectTestFinish!=NULL)  
	 {  
		 TESTengine *t=(TESTengine *)(obj->enginePtr);
	     if(ListNumItems(t->collectList)==currentCollect)
	     {
	 		 (*(ON_OBJECT_TEST_FINISH_LISTENER)(obj->onObjectTestFinish))(obj); 
	     }
	 }
	 return 0;
}

void runSingleObject(TESTengine *t,TESTobject *testObject){
	if(t->totalTestObject==0)
	{
	    return;
	}
	
	if(t->testState!=TEST_IDLE)
	{
	    goto Done;
	}
	
	if(t->onTestBeginListener!=NULL)
	{	
	   if((*(ON_TEST_BEGIN_LISTENER)(t->onTestBeginListener))(t)==FALSE)
	   {
		  t->testState=TEST_FINISH;   
		  LOG_EVENT(LOG_ERROR,"测试初始化出错"); 
	      goto Done;
	   }	   
	   
	}
	t->testState=TEST_RUN;
	Collect currentCollet;
	int collectNumToTest=0;
	if(testObject->seq.collectCountToTest<=0)
	{
		collectNumToTest = ListNumItems(t->collectList);
	}/*else if(testObject->seq.collectCountToTest > ListNumItems(t->collectList))
	{
		collectNumToTest = ListNumItems(t->collectList);
	}*/else {
		collectNumToTest = testObject->seq.collectCountToTest;
	}
	
	for(int collectNum=testObject->seq.beginCollect;collectNum<=ListNumItems(t->collectList);collectNum++)
	{
		//currentCollectNum=collectNum;
		if(collectNumToTest-- ==0)
		{
			break;
		}
		Collect currentCollect;
        if(!JudgeTestProcess(t))  //如果为完成，或者是重置，则跳出   
	    {
		    goto Done;
		}	
		
		t->currentCollect=collectNum;
		memset(&currentCollect,sizeof(Collect),0);
		ListGetItem(t->collectList,&currentCollect,collectNum);
		tTask task=createTask();
		task.maxParallelObject=currentCollect.maxParallelDevice;
		if(collectNum!=testObject->seq.beginCollect) //从指定组和集合开始测试，如果不为指定组合，那就从组1开始测试。
		{
			
			testObject->seq.beginGroup = 1;
			testObject->seq.beginCollect = collectNum;
		}
		
		tObject object=createObject((void *)(testObject),DoTestFunction);
		addObjectToTask(&task,&object);
		//}
		runTask(task);
        if(!JudgeTestProcess(t))  //如果为完成，或者是重置，则跳出   
	    {
		    goto Done;
		}	
		
	}
	t->testState=TEST_FINISH;
Done:
	if(t->onTestFinishListener!=NULL) 
	   (*(ON_TEST_FINISH_LISTENER)(t->onTestFinishListener))(t);	
}


void runTestEngine(TESTengine *t)
{
	/*获取集合序号*/
	if(t->totalTestObject==0)
	{
	    return;
	}
	
	if(t->testState!=TEST_IDLE)
	{
	    goto Done;
	}
	
	if(t->onTestBeginListener!=NULL)
	{	
	   if((*(ON_TEST_BEGIN_LISTENER)(t->onTestBeginListener))(t)==FALSE)
	   {
		  t->testState=TEST_FINISH;   
		  LOG_EVENT(LOG_ERROR,"测试初始化出错"); 
	      goto Done;
	   }	   
	   
	}
	t->testState=TEST_RUN;
	Collect currentCollet;
	for(int collectNum=1;collectNum<=ListNumItems(t->collectList);collectNum++)
	{
		//currentCollectNum=collectNum;
		Collect currentCollect;
        if(!JudgeTestProcess(t))  //如果为完成，或者是重置，则跳出   
	    {
		    goto Done;
		}	
		
		t->currentCollect=collectNum;
		memset(&currentCollect,sizeof(Collect),0);
		ListGetItem(t->collectList,&currentCollect,collectNum);
		/*获取设备序号*/   
		tTask task=createTask();
		task.maxParallelObject=currentCollect.maxParallelDevice;
		for(int deviceIndex=1;deviceIndex<=t->totalTestObject;deviceIndex++)
		{
			  t->objectArray[deviceIndex-1].seq.beginCollect=collectNum;
			  t->objectArray[deviceIndex-1].seq.beginGroup=1;
			  tObject object=createObject((void *)(&(t->objectArray[deviceIndex-1])),DoTestFunction);
			  addObjectToTask(&task,&object);
		}
		runTask(task);
        if(!JudgeTestProcess(t))  //如果为完成，或者是重置，则跳出   
	    {
		    goto Done;
		}	
		
	}
	t->testState=TEST_FINISH;
Done:
	if(t->onTestFinishListener!=NULL) 
	   (*(ON_TEST_FINISH_LISTENER)(t->onTestFinishListener))(t);
}

void reSetEngine(TESTengine *t)
{
	for(int i=0;i<t->totalTestObject;i++)
	{
		t->objectArray[i].errorFlag=0;
		t->objectArray[i].totalFinishedTestGroupCount=0;
	}	
    disPlayTestPanel(t);  
}

void releaseTestEngine(TESTengine *t)
{
    for(int i=0;i<t->totalTestObject;i++)
	{
		if(t->objectArray[i].resultHashTable!=NULL)
	       HashTableDispose(t->objectArray[i].resultHashTable);
    	t->objectArray[i].enginePtr=NULL;
	    t->objectArray[i].onObjectPanelCreateListener=NULL;   //任然需要优化
        t->objectArray[i].onObjectPanelInitListener=NULL;;   
     	t->objectArray[i].onResultShowListener=NULL;
		t->objectArray[i].onObjectGroupTestListener=NULL;
		t->objectArray[i].onObjectCollectTestListener=NULL;
		t->objectArray[i].onObjectTestErrorListener=NULL;
		t->objectArray[i].onObjectTestFinish=NULL;
		
		
		if(t->objectArray[i].panelHandle!=0)
		   DiscardPanel(t->objectArray[i].panelHandle);
	}
	free(t);
}

TESTobject *getTestObjectByPanelHandle(int objPanelHandle,TESTengine *engine){
	for(int i=0;i<8;i++)
	{
		if(engine->objectArray[i].panelHandle == objPanelHandle)
		{
			return &(engine->objectArray[i]);
		}
	
	}
	return NULL;
}





 
