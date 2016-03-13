/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ��ݺ��º��տƼ��ɷ����޹�˾��Ȩ���У�δ����Ȩ����ֹ����
 // �����п�����ʹ�á�
 //
 // �ļ�����policyEntity.h
 // �ļ���������:���Զ�ȡ���洢
 //
 // 
 // ������ʶ�������� 20141114
 //
 // �޸ı�ʶ��������  20150814
 // �޸��������Ż�
 //-------------------------------------------------------------------------*/
#ifndef __POLICYENTITY_H__
#define __POLICYENTITY_H__ 

#ifdef __cplusplus
    extern "C" {
#endif

#define COLLECT_NAME_LEN 30
		
typedef enum  
{ 

    PARALLEL = 0,
    SERIAL = 1, 
}TESTTYPE;


typedef enum
{

    MAINTAINPORT=0,
	FILEONDISK=1,
} RESULTDERIVE;	

typedef struct
{
   char name_[COLLECT_NAME_LEN];  //��������
   TESTTYPE testType; //��������
   RESULTDERIVE resultDerive; //���Է�ʽ
   int maxParallelDevice; //����в����豸��
   int retryCount;//�ز����
   ListType groups; //����������
}Collect;
		
ListType getStrategy(void);

void saveStrategy(int panelHandle,ListType collectList);

void printfCollectList(ListType collectList);   

#ifdef __cplusplus
    }
#endif

#endif
