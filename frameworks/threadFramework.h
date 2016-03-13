/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ��ݺ��º��տƼ��ɷ����޹�˾��Ȩ���У�δ����Ȩ����ֹ����
 // �����п�����ʹ�á�
 //
 // �ļ�����testTaskFramework.h
 // �ļ���������: ���߳̿��
 //
 // 
 // ������ʶ�������� 20150716
 //
 // �޸ı�ʶ��
 // �޸�������
 //-------------------------------------------------------------------------*/

#ifndef __TESTTASKFRAMEWORK_H__
#define __TESTTASKFRAMEWORK_H__

#include "toolbox.h"
#ifdef __cplusplus
    extern "C" {
#endif

typedef struct 
{
   void *objectId;//������
   int funcId;//�̱߳��
   ThreadFunctionPtr taskCallBack;//����ص�����
}tObject;//���Զ���

typedef struct
{
	int maxParallelObject;
    ListType objList;//���Զ�������
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
		

