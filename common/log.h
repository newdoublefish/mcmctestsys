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

#ifndef __log_H__
#define __log_H__

#ifdef __cplusplus
    extern "C" {
#endif

//==============================================================================
// Include files

#include "cvidef.h"

//==============================================================================
// Constants

//==============================================================================
// Types

//==============================================================================
// External variables

//==============================================================================
// Global functions
		
extern char LOG_NAME[][6];
#define LOG_DEBUG 0
#define LOG_ERROR 1
#define LOG_INFO  2
#define LOG_WARING 3

int LOG_REC(char *logMsg);   		
#define REC_LOG
#ifdef REC_LOG

#define LOG_FORMAT(format,...)   \
		do{ 		 \
		    char temp[2048]={0};          \
			sprintf((char*)temp,"_"format"\n",##__VA_ARGS__);	   \
			LOG_REC((char *)temp);							\
	     }while(0)

#define LOG_EVENT(level,msg)   \
		do{ 		 \
		    char temp[2048]={0};          \
			sprintf((char*)temp,"[%s]func:%s(FILE:%s,LINE:%d):%s\n",LOG_NAME[level],__FUNCTION__,__FILE__,__LINE__,(char*)msg);	   \
			LOG_REC((char *)temp);							\
	     }while(0)

#define LOG_EVENT_FORMAT(level,format,...)   \
		do{ 		 \
		    char temp[2048]={0};          \
			sprintf((char*)temp,"[%s]func:%s,(FILE:%s,LINE:%d):"format"\n",LOG_NAME[level],__FUNCTION__,__FILE__,__LINE__,##__VA_ARGS__);	   \
			LOG_REC((char *)temp);							\
	     }while(0)
#else
#define LOG_EVENT(level,msg)
#define LOG_EVENT_FORMAT(level,format,...)
#endif


int initLogModule(void);
void quitLogModule(void);
void deleteLogFile(void);
void initLogPath(void);


#ifdef __cplusplus
    }
#endif

#endif  /* ndef __log_H__ */
