#ifndef __SUTENTITY_H__
#define  __SUTENTITY_H__
#include <stdlib.h>
#include <userint.h>  
#include "toolbox.h" 

#ifdef __cplusplus
    extern "C" {
#endif

#define SYSTEM_NAME_LEN 50
#define CONFIG_PATH_LEN 30	
#define VERSION_LEN 30
		
typedef struct
{
   char systemName[SYSTEM_NAME_LEN];//ϵͳ����
   char nickName[SYSTEM_NAME_LEN];//����  
   char configPath[MAX_PATHNAME_LEN];//�����������·��
   char strategyFile[MAX_PATHNAME_LEN];//���Դ��·��
   char deviceFile[MAX_PATHNAME_LEN];//���Դ��·��    
   char reportFilePath[MAX_PATHNAME_LEN];//����ģ����·�� 
   char dbName[MAX_PATHNAME_LEN];//���ݿ����
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

