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
	int m_id;
	unsigned char m_code[50];
	unsigned char m_createtime[50];
	unsigned char m_lasttest[50];
	unsigned short m_upload;
	unsigned short m_result; 
	unsigned char m_projectpath[250];
	unsigned char m_reportpath[250];
}tAutoTestRecord;


int initDb(void);
tAutoTestRecord getRecordById(int id);
int insertReportRecord(tAutoTestRecord record);
int getAllRecord(ListType list);
int deleteById(int id);
int updateUpload(int id,short flag);   
void printRecord(tAutoTestRecord record);
int updateRecord(tAutoTestRecord record);
int getInsertdRecordId(int *id);
#ifdef __cplusplus
    }
#endif

#endif
