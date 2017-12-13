#include<stdio.h>
#include "cvi_db.h"
#include <utility.h>
#include <formatio.h>


#include "reportDb.h"   
static int dBHandle;//����Դ���
#define LOCAL_DATABASE_NAME "autotest.mdb" 
#define DATABASE_TABLE_NAME "abc"

int err_report(int i)					 
{
	char *message;
	message=DBErrorMessage ();
	return 0;
}

int initDb()
{
	char dirTemp[MAX_PATHNAME_LEN];
	char DBConStr[256];	
	DBInit (DB_INIT_MULTITHREADED);
	GetProjectDir (dirTemp);//��ȡ����Ŀ¼·��
	sprintf(DBConStr,"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s\\%s;Persist Security Info=False",dirTemp,LOCAL_DATABASE_NAME);//��ȡ����Դ����·��
	dBHandle = DBConnect (DBConStr);//��������Դ����
	return 0;
}

int insertReportRecord(tAutoTestRecord record)
{
#if 1	
	char SQLCommand[250];
	int  dbstatus[7];
	int  fileHandle;
	Fmt(SQLCommand,"SELECT * FROM %s",DATABASE_TABLE_NAME);
	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı�������
	if(DBBindColChar(fileHandle,1,30,record.ProductId,&dbstatus[0],"")!=DB_SUCCESS)
		return(err_report(1));
	DBBindColChar(fileHandle,2,50,record.FtpAddress,&dbstatus[1],"");
	DBBindColShort(fileHandle,3,&record.Result,&dbstatus[2]);
	DBBindColShort(fileHandle,5,&record.m_update,&dbstatus[3]);
	DBBindColChar(fileHandle,6,30,record.m_date,&dbstatus[4],""); 
	DBBindColChar(fileHandle,7,30,record.m_name,&dbstatus[5],""); 
	
	DBCreateRecord (fileHandle); //������¼  								s
	if(DBPutRecord (fileHandle)!=DB_SUCCESS)
	{
		DBDeactivateSQL (fileHandle); 
		return -1; 
	}	
	return 1;
#else
	int hstmt1;
	hstmt1=DBPrepareSQL(dBHandle,"insert into abc values(?,?,?,?,?,?)");
	DBCreateParamChar(hstmt1,"ProductId",DB_PARAM_INPUT,record.ProductId,30);
	DBCreateParamChar(hstmt1,"FtpAddress",DB_PARAM_INPUT,record.FtpAddress,50);
	DBCreateParamShort(hstmt1,"Result",DB_PARAM_INPUT,record.Result);
	DBCreateParamShort(hstmt1,"m_update",DB_PARAM_INPUT,record.m_update);
	DBCreateParamChar(hstmt1,"m_date",DB_PARAM_INPUT,record.m_date,30); 
	DBCreateParamChar(hstmt1,"m_name",DB_PARAM_INPUT,record.m_name,30);
	//DBCreateParamInt(hstmt1,"id",DB_PARAM_INPUT,record.id);
	DBExecutePreparedSQL(hstmt1);
	DBClosePreparedSQL(hstmt1);	
	return 1;
#endif	
}

int getAllRecord(ListType list)
{
	char SQLCommand[250];
	int  fileHandle=0;
	int  dbstatus[7]={0};
	tAutoTestRecord record={0};	
	if(list==0)
		return -1;
	//*listPtr=ListCreate(sizeof(tAutoTestRecord));
	Fmt(SQLCommand,"SELECT * FROM %s",DATABASE_TABLE_NAME);
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı������� 
	//num=DBNumberOfRecords (fileHandle);//��ȡ��ǰ���ݿ��еļ�¼
	
	//printf("%d\n",num);
	DBBindColChar(fileHandle,1,30,record.ProductId,&dbstatus[0],"");
	DBBindColChar(fileHandle,2,50,record.FtpAddress,&dbstatus[1],"");
	DBBindColShort(fileHandle,3,&record.Result,&dbstatus[2]);
	DBBindColShort(fileHandle,4,&record.id,&dbstatus[3]); 
	DBBindColShort(fileHandle,5,&record.m_update,&dbstatus[4]);
	DBBindColChar(fileHandle,6,30,record.m_date,&dbstatus[5],""); 
	DBBindColChar(fileHandle,7,30,record.m_name,&dbstatus[6],""); 
	
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
	int  dbstatus[7];
	int  fileHandle;
	tAutoTestRecord record={0};
	Fmt(SQLCommand,"SELECT * FROM %s where id=%d",DATABASE_TABLE_NAME,id);
	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı�������
	DBBindColChar(fileHandle,1,30,record.ProductId,&dbstatus[0],"");
	DBBindColChar(fileHandle,2,50,record.FtpAddress,&dbstatus[1],"");
	DBBindColShort(fileHandle,3,&record.Result,&dbstatus[2]);
	//DBBindColShort(fileHandle,4,&record.id,&dbstatus[3]);
	DBBindColShort(fileHandle,5,&record.m_update,&dbstatus[4]);
	DBBindColChar(fileHandle,6,30,record.m_date,&dbstatus[5],""); 
	DBBindColChar(fileHandle,7,30,record.m_name,&dbstatus[6],"");
	
	while(DBFetchNext(fileHandle)==0)
	{
		//printRecord(record);
		record.m_update=flag;		
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
	Fmt(SQLCommand,"SELECT * FROM %s where id=%d",DATABASE_TABLE_NAME,id);
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//�����ݿ��еı������� 
	DBDeleteRecord(fileHandle);
	DBDeactivateSQL (fileHandle); 
	return 0;		
}

void printRecord(tAutoTestRecord record)
{
	printf("------------------------------------s\n");
	printf("id:%d,productId:%s,result:%d\n update:%d,date:%s,name:%s,ftpAddress:%s\n",
		   record.id,record.ProductId,record.Result,record.m_update,record.m_date,record.m_name,record.FtpAddress);
}
