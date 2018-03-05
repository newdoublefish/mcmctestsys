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
#include<stdio.h>
#include "cvi_db.h"
#include <utility.h>
#include <formatio.h>
#include "sutCommon.h"


#include "reportDb.h"   
static int dBHandle;//����Դ���
#define LOCAL_DATABASE_NAME "autotest.mdb" 
//#define DATABASE_TABLE_NAME "abc"
BOOL initFlag = FALSE;

int err_report(int i)					 
{
	char *message;
	message=DBErrorMessage ();
	return 0;
}

BOOL createTableifNotExist(char *tableName)
{
	char SQLCommand[250]={0};

	int  fileHandle; 
	Fmt(SQLCommand,"SELECT * FROM %s",tableName); 	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı������� 
	if(fileHandle<0)
	{
		memset(SQLCommand,0,250);
		sprintf(SQLCommand,"create table %s(m_id integer identity(1,1) primary key,\
			m_code varchar(50),\
			m_createtime varchar(50),\
			m_lasttest varchar(50),\
			m_upload bit,\
			m_result bit,\
			m_projectpath varchar(250),\
			m_reportpath varchar(250));",tableName);
			fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı�������  
			if(fileHandle<0)
				return FALSE;
	}
	DBDeactivateSQL (fileHandle); 
	return TRUE;
}

BOOL bindRecord(tAutoTestRecord *record,int fileHandle,int *dbstatus)
{

	if(record->m_id >= 0)
		DBBindColInt(fileHandle,1,&record->m_id,&dbstatus[0]); 
	DBBindColChar(fileHandle,2,50,record->m_code,&dbstatus[1],"");
	DBBindColChar(fileHandle,3,50,record->m_createtime,&dbstatus[2],"");
	DBBindColChar(fileHandle,4,50,record->m_lasttest,&dbstatus[3],"");	
	DBBindColShort(fileHandle,5,&record->m_upload,&dbstatus[4]);
	DBBindColShort(fileHandle,6,&record->m_result,&dbstatus[5]);
	DBBindColChar(fileHandle,7,250,record->m_projectpath,&dbstatus[6],""); 
	DBBindColChar(fileHandle,8,250,record->m_reportpath,&dbstatus[7],""); 
	return TRUE;
}

int initDb()
{
	if(initFlag == FALSE)
	{
		char dirTemp[MAX_PATHNAME_LEN];
		char DBConStr[256];	
		DBInit (DB_INIT_MULTITHREADED);
		GetProjectDir (dirTemp);//��ȡ����Ŀ¼·��
		sprintf(DBConStr,"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s\\%s;Persist Security Info=False",dirTemp,LOCAL_DATABASE_NAME);//��ȡ����Դ����·��
		if((dBHandle = DBConnect (DBConStr))<=0)//��������Դ����
			return FALSE;
		SUT sut=GetSeletedSut();  	
		if(createTableifNotExist(sut.dbName)==FALSE)
			return FALSE;
		initFlag = TRUE;
	}
	return TRUE;
}

int insertReportRecord(tAutoTestRecord record)
{
	char SQLCommand[250];
	int  dbstatus[8];
	int  fileHandle;
	SUT sut=GetSeletedSut();
	Fmt(SQLCommand,"SELECT * FROM %s",sut.dbName);
	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı�������
	record.m_id=-1;
	bindRecord(&record,fileHandle,dbstatus);

	DBCreateRecord (fileHandle); //������¼  								s
	if(DBPutRecord (fileHandle)!=DB_SUCCESS)
	{
		DBDeactivateSQL (fileHandle); 
		return -1; 
	}	
	return 1;
}

int getAllRecord(ListType list)
{
	char SQLCommand[250];
	int  fileHandle=0;
	int  dbstatus[8]={0};
	tAutoTestRecord record={0};	
	if(list==0)
		return -1;
	//*listPtr=ListCreate(sizeof(tAutoTestRecord));
		SUT sut=GetSeletedSut(); 
	Fmt(SQLCommand,"SELECT * FROM %s",sut.dbName);
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı������� 
	//num=DBNumberOfRecords (fileHandle);//��ȡ��ǰ���ݿ��еļ�¼
	
	
	bindRecord(&record,fileHandle,dbstatus);
	
	while(DBFetchNext(fileHandle)==0)
	{
		//printf("record %s,%d\n",record.ProductId,record.id);
		ListInsertItem(list,&record,END_OF_LIST);
	}
	DBDeactivateSQL (fileHandle); 
	
	return 0;
}

int updateUpload(int id,short flag)
{
	char SQLCommand[250];
	int  dbstatus[8];
	int  fileHandle;
	tAutoTestRecord record={0};
		SUT sut=GetSeletedSut(); 
	Fmt(SQLCommand,"SELECT * FROM %s where m_id=%d",sut.dbName,id);
	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı�������
	record.m_id =-1; //����-1��bind
	bindRecord(&record,fileHandle,dbstatus);	
	
	while(DBFetchNext(fileHandle)==0)
	{
		//printRecord(record);
		record.m_upload=flag;		
		if(DBPutRecord (fileHandle)!=DB_SUCCESS)
		{

			DBDeactivateSQL (fileHandle); 
			return -1; 
		}
	}
	DBDeactivateSQL (fileHandle); 	
	return 1;	
}

int deleteById(int id)
{
	char SQLCommand[250];
	int  fileHandle=0;
		SUT sut=GetSeletedSut(); 
		
	Fmt(SQLCommand,"SELECT * FROM %s where m_id=%d",sut.dbName,id);
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı������� 
	DBDeleteRecord(fileHandle);
	DBDeactivateSQL (fileHandle); 
	return 0;		
}


tAutoTestRecord getRecordById(int id)
{
	char SQLCommand[250];
	int  dbstatus[8];
	int  fileHandle;
	tAutoTestRecord record={0};
		SUT sut=GetSeletedSut(); 
	Fmt(SQLCommand,"SELECT * FROM %s where m_id=%d",sut.dbName,id);
	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı�������
	record.m_id = id;
	bindRecord(&record,fileHandle,dbstatus);	
	while(DBFetchNext(fileHandle)==0)
	{
		//printRecord(record);
	}	
	DBDeactivateSQL (fileHandle); 	
	return record;
}

/*void printRecord(tAutoTestRecord record)
{
	printf("------------------------------------s\n");
	printf("id:%d,productId:%s,result:%d\n update:%d,date:%s,name:%s,ftpAddress:%s\n",
		   record.id,record.ProductId,record.Result,record.m_update,record.m_date,record.m_name,record.FtpAddress);
}*/
