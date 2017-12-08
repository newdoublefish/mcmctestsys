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

#ifndef __TESTTASKFRAMEWORK_H__
#define __TESTTASKFRAMEWORK_H__

#include "toolbox.h"
#ifdef __cplusplus
    extern "C" {
#endif

typedef struct 
{
   void *objectId;//任务编号
   int funcId;//线程编号
   ThreadFunctionPtr taskCallBack;//任务回调函数
}tObject;//测试对象

typedef struct
{
	int maxParallelObject;
    ListType objList;//测试对象链表
}tTask;	

#if 0
tTask *createTask(void);
tObject *createObject(int index,ThreadFunctionPtr taskCallBack);
HRESULT addObjectToTask(tTask *task,tObject *object);
HRESULT runTask(tTask *task);
#else
tTask createTask(void);
tObject createObject(void *index,ThreadFunctionPtr taskCallBack);
HRESULT addObjectToTask(tTask *task,tObject *object);
HRESULT runTask(tTask task);



#endif

		
#ifdef __cplusplus
    }
#endif
#endif
		

