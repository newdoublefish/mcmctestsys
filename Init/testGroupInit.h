#ifndef __TESTCASECOMMON_H__
#define __TESTCASECOMMON_H__

#include "cvidef.h"
#ifdef __cplusplus
    extern "C" {
#endif
#define MAX_GROUP_NAME_LEN 100  

		
#define MAX_TESTITEM_NAME_LEN 100
#define MAX_TESTITEM_DESCRIPTION_LEN 100  	
#define MAX_TESTITEM_TYPE_LEN 30  
		

#define MAX_TESTITEM_INPUTVALUE_LEN 30
#define MAX_TESTITEM_STANDARD_LEN 30 		


typedef struct
{
   double maxValue;          //���ֵ
   double minValue;			//��Сֵ
}VALUE_STANDARD;				//������׼

typedef struct 
{
    char groupName[MAX_TESTITEM_NAME_LEN];  //�����ƣ�ע�⣬Э��Ҳ�����������
	char nickName[MAX_TESTITEM_NAME_LEN];   //ϵͳ�Ὣ����ż��ϣ��������������Э��Ľ���
	int groupId;							//�����
	char type[MAX_TESTITEM_TYPE_LEN];		//�������� 422,429����
	ListType subItems;                      //������ 
}TestGroup;//������

typedef struct
{			
     char itemName_[MAX_TESTITEM_NAME_LEN];  //��������
	 int itemId;       //�������   ������Ϊ��ȫ�ֵ��������
	 
	 int paramId;     //��������ID
	 char description[MAX_TESTITEM_DESCRIPTION_LEN]; //��������������
	 char itemType_[MAX_TESTITEM_TYPE_LEN];  //�������ͣ������Ŀ
	 unsigned char inputValue_[MAX_TESTITEM_INPUTVALUE_LEN];  //����ֵ�ַ���������ʾ��
     double input_Value; //����ֵ����ʵ�ʱȽ�
	 unsigned char standard_[MAX_TESTITEM_STANDARD_LEN]; //��׼ֵ�ַ���������ʾ��
     VALUE_STANDARD standard_value;//��׼ֵ������ʾ
	 int testLevel; //���Եȼ������
}TestItem;//����������


HRESULT testGroupInit(char *filePath);

HRESULT testGroupDeinit(void); 

void printList(ListType list);

void SendSlideValue(unsigned int cmd,unsigned int value);

ListType getItemList(void); 

int getTestCaseId(char *itemName,double intputValue,char *valueString);

void getTestGroup(TestGroup *groupPtr,char *groupName);

void getGroupItemByName(TestGroup group,TestItem *item,char *itemName);

ListType getTestCaseList(void);


#ifdef __cplusplus
    }
#endif


#endif
 
