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
    int monitorWidth; //��Ļ���
	int panelRows;  //�����ʾ������
	int deviceCount;  //�����豸����
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
	}						   //��ֹ��ʾ����

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
	    	//(*(ON_OBJECT_PANEL_CREATE_LISTENER)(t->objectArray[deviceNum-1].onObjectPanelCreateListener))(&(t->objectArray[deviceNum-1].panelHandle)); //�����������
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
			//   (*(ON_OBJECT_PANEL_INIT_LISTENER)(t->objectArray[deviceNum-1].onObjectPanelInitListener))(&(t->objectArray[deviceNum-1]));	//��ʾ����
		
	    	//DisplayPanel(t->objectArray[deviceNum-1].panelHandle);	   //ͳһ��ʾ��û��ͣ�͸� 
		//}
	}

}


void disPlayTestPanel(TESTengine *t)
{
    int monitorHight;
    int monitorWidth; //��Ļ���
	int panelRows;  //�����ʾ������
	int deviceCount;  //�����豸����
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
	panelRows=deviceCount/maxCountsPerRow;//ÿ�������ʾ4��
	
	//PRINT("monitorWidth:%d,monitorHight:%d\n",monitorWidth,monitorHight);
	
	if(panelRows*maxCountsPerRow<deviceCount)
	{
	   panelRows=panelRows+1;
	}						   //��ֹ��ʾ����

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
		
		if(t->objectArray[deviceNum-1].panelHandle!=0)  //ע�⣺������������ͷ�
		{	
		    DiscardPanel(t->objectArray[deviceNum-1].panelHandle);
		}  
		if(t->objectArray[deviceNum-1].onObjectPanelCreateListener!=NULL)
		{	
	    	(*(ON_OBJECT_PANEL_CREATE_LISTENER)(t->objectArray[deviceNum-1].onObjectPanelCreateListener))(&(t->objectArray[deviceNum-1].panelHandle)); //�����������
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
			   (*(ON_OBJECT_PANEL_INIT_LISTENER)(t->objectArray[deviceNum-1].onObjectPanelInitListener))(&(t->objectArray[deviceNum-1]));	//��ʾ����
		
	    	DisplayPanel(t->objectArray[deviceNum-1].panelHandle);	   //ͳһ��ʾ��û��ͣ�͸� 
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
	 for(int i=1;i<=ListNumItems(collectList);i++)
	 {
	     Collect collect;
		 ListGetItem(collectList,&collect,i);
		 t->totalTestGroupCount+=ListNumItems(collect.groups);
	 }	 
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
	 
	 currentCollect= ((TESTengine *)(obj->enginePtr))->currentCollect;
	 ListGetItem(((TESTengine *)(obj->enginePtr))->collectList,&collect,currentCollect);
	 
	 //printf("type:%d\n",collect.testType);
	 if(collect.testType==2)//�ɲβ���
	 {
		 if(obj->onObjectCollectTestListener!=NULL)
		 {
		     TESTresult testRet= TEST_ERROR; 
			 testRet=(*(ON_OBJECT_COLLECT_TEST_LISTENER)(obj->onObjectCollectTestListener))(collect,obj);
			 
			 for(int i=1;i<=ListNumItems(collect.groups);i++)  //һ��һ����ʾ���
			 {
				int groupIndex;
				int itemIndex=0;
			    TestGroup group;
	          	ListGetItem(collect.groups,&groupIndex,i);//��ȡ���ID��
	         	ListGetItem(((TESTengine *)(obj->enginePtr))->itemList,&group,groupIndex);//��ȡ��
			    if(obj->onResultShowListener!=NULL)
			       (*(ON_RESULT_SHOW_LISTENER)(obj->onResultShowListener))(obj,group,i,&itemIndex);				
			 }
		 }
	     goto Done;
	 }
	
    for(int i=1;i<=ListNumItems(collect.groups);i++)
	{
		BOOL retryFlag=FALSE;
		int groupIndex;
		int itemIndex=0;  //�صڼ���������ʼ��ʾ
		TestGroup testItem;   
		ListGetItem(collect.groups,&groupIndex,i);//��ȡ���ID��
		ListGetItem(((TESTengine *)(obj->enginePtr))->itemList,&testItem,groupIndex);//��ȡ��
		obj->totalFinishedTestGroupCount+=1;  //����ɲ�����������1
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
			  
			  while(retryCount<maxRetryCount) //��20S�ڻ�ѭ�����ԣ�ֱ��������ȷ������20S������
			  {	  
				 //printf("%d,%d\n",retryCount,((TESTengine *)(obj->enginePtr))->reTestCnt);
		         if(!JudgeTestProcess((TESTengine *)(obj->enginePtr)))  //���Ϊ��ɣ����������ã�������   
		         {
			         goto Done;
		         }	
				  
		         /*if(DoItemTest(obj,testItem)==TRUE)
				 {
				     break;
				 }	 //�������*/
				 //TEST_METHOD method=getTestMethod(testItem.itemType); //��ȡ���Է���
				 //METHODRET testRet= (*method)(testItem,obj->device,obj->resultHashTable);
				 TESTresult testRet= TEST_ERROR;    
				 if(obj->onObjectGroupTestListener!=NULL)
				 {
					   //LOG_LEVEL("Debug","%x\n",obj->resultHashTable);
					   testRet=(*(ON_OBJECT_GROUP_TEST_LISTENER)(obj->onObjectGroupTestListener))(testItem,obj);	
			     }		   
				 if(testRet==TEST_ALL_PASS)
				 {
				     break; //���Գɹ�,���ڽ��в��ԣ������ز�
				 }else if(testRet==TEST_ERROR) 
				 {
					 if(obj->onObjectTestErrorListener!=NULL)
						   (*(ON_OBJECT_TEST_ERROR_LISTENER)(obj->onObjectTestErrorListener))(obj);
					 LOG_EVENT_FORMAT(LOG_WARING,"UUT:%s:����:%s,��Ŀ:%s,%s\n",obj->device.eutName,collect.name_,testItem.nickName,"���Թ���");
				     goto Done;
				 }
				 retryCount++;
				 //MessagePopup();
				 //endTime=Timer();
			  }
			  if(obj->onResultShowListener!=NULL)
			       retryFlag=! (*(ON_RESULT_SHOW_LISTENER)(obj->onResultShowListener))(obj,testItem,i,&itemIndex);
			  
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

void runTestEngine(TESTengine *t)
{
	/*��ȡ�������*/
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
		  LOG_EVENT(LOG_ERROR,"���Գ�ʼ������"); 
	      goto Done;
	   }	   
	   
	}
	t->testState=TEST_RUN;
	Collect currentCollet;
	for(int collectNum=1;collectNum<=ListNumItems(t->collectList);collectNum++)
	{
		//currentCollectNum=collectNum;
		Collect currentCollect;
        if(!JudgeTestProcess(t))  //���Ϊ��ɣ����������ã�������   
	    {
		    goto Done;
		}	
		
		t->currentCollect=collectNum;
		memset(&currentCollect,sizeof(Collect),0);
		ListGetItem(t->collectList,&currentCollect,collectNum);
		/*��ȡ�豸���*/   
		tTask task=createTask();
		task.maxParallelObject=currentCollect.maxParallelDevice;
		for(int deviceIndex=1;deviceIndex<=t->totalTestObject;deviceIndex++)
		{
			  tObject object=createObject((void *)(&(t->objectArray[deviceIndex-1])),DoTestFunction);
			  addObjectToTask(&task,&object);
		}
		runTask(task);
        if(!JudgeTestProcess(t))  //���Ϊ��ɣ����������ã�������   
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
	    t->objectArray[i].onObjectPanelCreateListener=NULL;   //��Ȼ��Ҫ�Ż�
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





 
