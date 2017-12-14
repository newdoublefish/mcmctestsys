/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ㶫������綯����Ӫ�ɷ����޹�˾��Ȩ���У�δ����Ȩ��
 // ��ֹ���������п�����ʹ�á�
 //
 // �ļ�����
 // �ļ���������: 
 //
 // 
 // ������ʶ�������� 
 //
 // �޸ı�ʶ��
 // �޸�������
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
tAutoTestRecord getRecordById(int id);
int insertReportRecord(tAutoTestRecord record);
int getAllRecord(ListType list);
int deleteById(int id);
int updateUpload(int id,short flag);   
void printRecord(tAutoTestRecord record);
#ifdef __cplusplus
    }
#endif

#endif
