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
#ifndef __REPORT_DB_H__
#define __REPORT_DB_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "toolbox.h"   
typedef struct
{
	char ProductId[30];
	char FtpAddress[100];
	unsigned short Result;
	unsigned int id;
	unsigned short m_update;
	char m_date[30];
	char m_name[30];
}tAutoTestRecord;


int initDb(void);
int insertReportRecord(tAutoTestRecord record);
int getAllRecord(ListType list);
int deleteById(int id);
int updateUpload(int id,short flag);   
void printRecord(tAutoTestRecord record);
#ifdef __cplusplus
    }
#endif

#endif
