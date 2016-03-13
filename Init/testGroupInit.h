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
   double maxValue;          //最大值
   double minValue;			//最小值
}VALUE_STANDARD;				//技术标准

typedef struct 
{
    char groupName[MAX_TESTITEM_NAME_LEN];  //组名称，注意，协议也是用这个名称
	char nickName[MAX_TESTITEM_NAME_LEN];   //系统会将组序号加上，不能用这个来做协议的解析
	int groupId;							//组序号
	char type[MAX_TESTITEM_TYPE_LEN];		//测试类型 422,429，振动
	ListType subItems;                      //子条例 
}TestGroup;//测试组

typedef struct
{			
     char itemName_[MAX_TESTITEM_NAME_LEN];  //条例名称
	 int itemId;       //条例序号   这个序号为在全局的条例序号
	 
	 int paramId;     //条例参数ID
	 char description[MAX_TESTITEM_DESCRIPTION_LEN]; //用于描述该条例
	 char itemType_[MAX_TESTITEM_TYPE_LEN];  //参数类型，检查项目
	 unsigned char inputValue_[MAX_TESTITEM_INPUTVALUE_LEN];  //输入值字符串用于显示用
     double input_Value; //输入值用于实际比较
	 unsigned char standard_[MAX_TESTITEM_STANDARD_LEN]; //标准值字符串用于显示用
     VALUE_STANDARD standard_value;//标准值用于显示
	 int testLevel; //测试等级，组别
}TestItem;//测试是条例


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
 
