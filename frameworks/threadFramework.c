/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：testTaskFramework.c
 // 文件功能描述: 多线程框架
 //
 // 
 // 创建标识：曾衍仁 20150716
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/

#include <cvirte.h>		
#include <userint.h>
#include <stdlib.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "threadFramework.h"
#include "log.h"
#include "debug.h"

#if 0
tTask *createTask(void)
{
   tTask *task;
   task=(tTask*)malloc(sizeof(tTask));
   task->objList=ListCreate(sizeof(tObject));
   task->maxParallelObject=0;
   return task;
}

tObject *createObject(int index,ThreadFunctionPtr taskCallBack)
{
	 tObject *object=(tObject *)malloc(sizeof(tObject));
	 object->taskCallBack=taskCallBack;
	 object->objectId=index;
	 return object;
}

HRESULT addObjectToTask(tTask *task,tObject *object)
{
   ListInsertItem(task->objList,object,END_OF_LIST);	
   return TRUE;
}


void CheckAllthreadFinished(int poolHandle,ListType objList)
{
	 for(int i=1;i<=ListNumItems(objList);i++) 
	 {
		 
		 tObject object; 
		 ListGetItem(objList,&object,i);
	 	 CmtWaitForThreadPoolFunctionCompletion(poolHandle,object.funcId,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
		 CmtReleaseThreadPoolFunctionID(poolHandle,object.funcId);
	 }	 
}


HRESULT runTask(tTask *task)
{
    int poolHandle=0,objectNum;
	if(CmtNewThreadPool (ListNumItems(task->objList),&poolHandle)<0)//创建线程池
	{
		 LOG_EVENT(LOG_INFO,"创建线程失败");
	     return FALSE;
	}
	
	for(int objectCount=1;objectCount<=ListNumItems(task->objList);objectCount+=task->maxParallelObject)
	{
	       ListType testingObjList=ListCreate(sizeof(tObject));
		   for(int tempCount=objectCount;tempCount<objectCount+task->maxParallelObject;tempCount++)
		   {
				tObject object;
				ListGetItem(task->objList,&object,tempCount);
		        CmtScheduleThreadPoolFunctionAdv (poolHandle,object.taskCallBack,
                    (void*)object.objectId, THREAD_PRIORITY_NORMAL,NULL, (EVENT_TP_THREAD_FUNCTION_BEGIN | EVENT_TP_THREAD_FUNCTION_END),
                         (void*)object.objectId, CmtGetCurrentThreadID(),&(object.funcId));
				ListInsertItem(testingObjList,&object,END_OF_LIST);
		   }
		   CheckAllthreadFinished(poolHandle,testingObjList);
		   if(testingObjList!=0)
		      ListDispose(testingObjList);
	}
	ListDispose(task->objList);
	CmtDiscardThreadPool (poolHandle);  
	free(task);
	return TRUE;
}
#else
tTask createTask(void)
{
   tTask task;
   memset(&task,sizeof(task),0); 
   task.objList=ListCreate(sizeof(tObject));
   task.maxParallelObject=0;
   return task;
}

tObject createObject(void *data,ThreadFunctionPtr taskCallBack)
{
	 tObject object;
	 memset(&object,sizeof(object),0);
	 object.taskCallBack=taskCallBack;
	 object.objectId=data;
	 return object;
}

HRESULT addObjectToTask(tTask *task,tObject *object)
{
   ListInsertItem(task->objList,object,END_OF_LIST);	
   return TRUE;
}


void CheckAllthreadFinished(int poolHandle,ListType objList)
{
	 for(int i=1;i<=ListNumItems(objList);i++) 
	 {
		 
		 tObject object; 
		 ListGetItem(objList,&object,i);
	 	 CmtWaitForThreadPoolFunctionCompletion(poolHandle,object.funcId,OPT_TP_PROCESS_EVENTS_WHILE_WAITING);
		 CmtReleaseThreadPoolFunctionID(poolHandle,object.funcId);
	 }	 
}


HRESULT runTask(tTask task)
{
	LOG_EVENT(LOG_INFO,"Enter runTask!");   
    int poolHandle=0,objectNum=0;
	if(CmtNewThreadPool (ListNumItems(task.objList),&poolHandle)<0)//创建线程池
	{
		 LOG_EVENT(LOG_ERROR,"creaate ThreadPool error!");
	     return FALSE;
	}
	objectNum=ListNumItems(task.objList);
	if(objectNum<=0)
	{
		LOG_EVENT(LOG_ERROR,"no object for test!"); 
	    return FALSE;
	}	
	
	for(int objectCount=1;objectCount<=ListNumItems(task.objList);objectCount+=task.maxParallelObject)
	{
	       ListType testingObjList=ListCreate(sizeof(tObject));
		   for(int tempCount=objectCount;tempCount<objectCount+task.maxParallelObject;tempCount++)
		   {
			    if(tempCount>objectNum)
				{
				   break;
				}	
				tObject object;
				ListGetItem(task.objList,&object,tempCount);
		        CmtScheduleThreadPoolFunctionAdv (poolHandle,object.taskCallBack,
                    (void*)object.objectId, THREAD_PRIORITY_NORMAL,NULL, (EVENT_TP_THREAD_FUNCTION_BEGIN | EVENT_TP_THREAD_FUNCTION_END),
                         (void*)object.objectId, CmtGetCurrentThreadID(),&(object.funcId));
				ListInsertItem(testingObjList,&object,END_OF_LIST);
		   }
		   CheckAllthreadFinished(poolHandle,testingObjList);
		   if(testingObjList!=0)
		      ListDispose(testingObjList);
	}
	ListDispose(task.objList);
	CmtDiscardThreadPool (poolHandle);  
	LOG_EVENT(LOG_INFO,"Quit runTask!"); 
	return TRUE;
}

#endif

