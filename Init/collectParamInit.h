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
#ifndef __COLLECTPARAMINIT_H__
#define __COLLECTPARAMINIT_H__
#include "toolbox.h"
#ifdef __cplusplus
   extern "C" {
#endif
#define MAX_COLLECT_PARAM_NAME_LEN 50
typedef struct
{
      char type[MAX_COLLECT_PARAM_NAME_LEN];
	  int id;
	  char name[MAX_COLLECT_PARAM_NAME_LEN];
	  int paramLen;
	  int storeLen;
	  int rate;
	  int dataType;
}CollectParam;

HRESULT collectParamInit(char *filePath);

ListType getCollectParamList(void);

HashTableType getCollcetParamHashType(void);

CollectParam getCollectParamByHashTable(char *name);

#ifdef __cplusplus
   {
#endif	   

#endif
