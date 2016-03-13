#ifndef __STATUSCASEENTITY_H__
#define __STATUSCASEENTITY_H__

#include "cvidef.h"


/*typedef struct
{
   double maxValue;          //最大值
   double minValue;			 //最小值
}VALUE_STANDARD;			 //技术标准 */



typedef struct
{			
     char *FamilyName;       //参数类型名称
	 int FamilyID;           //参数类型ID
	 int ItemSerial;         //序号
	 char *ItemName;         //参数名称
	 int ParaByteLen;        //参数位长
	 int PostValue;			 //状态接收值
}StatusItem;

typedef struct
{			
	 char *LRUName;         //参数名称
	 char *ItemName[32];	//状态项目
}StatusLRU;

int StatusItemInit(char *filePath,StatusLRU *LRU );
 
#ifdef __cplusplus
    {
#endif
		
#endif
