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

    AUTO=0,
	MANUAL=1,
} ENUM_TEST_TYPE;	

typedef struct
{
   char name_[COLLECT_NAME_LEN];  //��������
   TESTTYPE testType; //��������
   ENUM_TEST_TYPE tType; //���Է�ʽ(�ֶ����Զ�)
   int maxParallelDevice; //����в����豸��
   int retryCount;//�ز����
   ListType groups; //����������
   int tipFlag;
   int tipsCloseTime;
}Collect;
		
ListType getStrategy(void);

void saveStrategy(int panelHandle,ListType collectList);

void printfCollectList(ListType collectList);   

#ifdef __cplusplus
    }
#endif

#endif
