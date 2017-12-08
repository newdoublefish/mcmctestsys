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
   char name_[COLLECT_NAME_LEN];  //集合名称
   TESTTYPE testType; //测试类型
   RESULTDERIVE resultDerive; //测试方式
   int maxParallelDevice; //最大并行测试设备数
   int retryCount;//重测次数
   ListType groups; //条例组链表
}Collect;
		
ListType getStrategy(void);

void saveStrategy(int panelHandle,ListType collectList);

void printfCollectList(ListType collectList);   

#ifdef __cplusplus
    }
#endif

#endif
