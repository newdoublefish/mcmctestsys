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
	int m_id;
	unsigned char m_code[50];
	unsigned char m_createtime[50];
	unsigned char m_lasttest[50];
	unsigned short m_upload;
	int m_result; 
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
