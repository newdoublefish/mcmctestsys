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
#ifndef __DEBUG_H__
#define   __DEBUG_H__

#include<stdio.h> 

#ifdef __cplusplus
    extern "C" {
#endif


#define DEBUG
#ifdef DEBUG
#define PRINT_LEVEL(level,format,...)  \
        do   \
		{	  \
		   fprintf(stdout,"[%s]func:%s,(FILE:%s,LINE:%d):"format"\n",level,__FUNCTION__,__FILE__,__LINE__,##__VA_ARGS__); \
		}while(0)
		
#define PRINT(format,...)  \
        do   \
		{	  \
		   fprintf(stdout,"func:%s,(FILE:%s,LINE:%d):"format"\n",__FUNCTION__,__FILE__,__LINE__,##__VA_ARGS__); \
		}while(0)	
		
#else
#define LOG_LEVEL(level,format,...)		
#define LOG(format,...) 
#endif
		
		
#ifdef __cplusplus
	}
#endif
		

#endif
