#ifndef __SUTENTITY_H__
#define  __SUTENTITY_H__
#include <stdlib.h>
#include <userint.h>  
#include "toolbox.h" 

#ifdef __cplusplus
    extern "C" {
#endif

#define SYSTEM_NAME_LEN 30
#define CONFIG_PATH_LEN 30	
#define VERSION_LEN 30
		
typedef struct
{
   char systemName[SYSTEM_NAME_LEN];//系统名称
   char nickName[SYSTEM_NAME_LEN];//别名  
   char configPath[MAX_PATHNAME_LEN];//测试条例存放路径
   char strategyFile[MAX_PATHNAME_LEN];//策略存放路径
   char deviceFile[MAX_PATHNAME_LEN];//策略存放路径    
   char reportFilePath[MAX_PATHNAME_LEN];//报表模板存放路径 
   char dbName[MAX_PATHNAME_LEN];//数据库表名
   char postFilePath[MAX_PATHNAME_LEN];
} SUT;

typedef struct
{
    ListType sutList;
	char version[VERSION_LEN];
}SUTCONFIG;	

SUTCONFIG GetSystemConfig(void);

char *getVersion(void);

#ifdef __cplusplus
	}
#endif


#endif

