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
#ifndef __POSTDATA_H__
#define __POSTDATA_H__ 

#ifdef __cplusplus
    extern "C" {
#endif

#define POST_DATA_VALUE 256
#define POST_DATA_BASE 50

typedef struct{
	char name[POST_DATA_BASE];
	char type[POST_DATA_BASE];
	char value[POST_DATA_VALUE];
	ListType postParamList;
}tPostParam;

typedef struct{
	char name[30];
	char url[POST_DATA_VALUE];
	char request[POST_DATA_BASE];
	int upload;
	ListType postParamList;
}tPostData;
typedef BOOL (*ON_FILL_DATA_PARAM)(tPostParam *,void *callbackData);

BOOL buildPostDataStr(tPostData data,char *buffer,void *callbackFunc,void *callbackData);
BOOL importPostProtocol(void);
BOOL getPostData(tPostData *postDataPtr,char *name);
ListType getPostDataSet();

#ifdef __cplusplus
    }
#endif

#endif
