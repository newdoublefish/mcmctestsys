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
#include<stdio.h>
#include "cvi_db.h"
#include <utility.h>
#include <formatio.h>
#include "sutCommon.h"


#include "reportDb.h"   
static int dBHandle;//数据源句柄
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
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//与数据库中的表建立连接 
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
			fileHandle= DBActivateSQL(dBHandle,SQLCommand);//与数据库中的表建立连接  
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
		GetProjectDir (dirTemp);//获取工程目录路径
		sprintf(DBConStr,"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=%s\\%s;Persist Security Info=False",dirTemp,LOCAL_DATABASE_NAME);//获取数据源所在路径
		if((dBHandle = DBConnect (DBConStr))<=0)//建立数据源连接
			return FALSE;
		SUT sut=GetSeletedSut();  	
		if(createTableifNotExist(sut.dbName)==FALSE)
			return FALSE;
		initFlag = TRUE;
	}
	return TRUE;
}

int getInsertdRecordId(int *id)
{
	char SQLCommand[250];
	int  fileHandle=0;
	int  dbstatus[8]={0};
	tAutoTestRecord record={0};	
	//*listPtr=ListCreate(sizeof(tAutoTestRecord));
	SUT sut=GetSeletedSut(); 
	Fmt(SQLCommand,"select @@identity as %s","m_id");
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//与数据库中的表建立连接 
	//num=DBNumberOfRecords (fileHandle);//获取当前数据库中的记录
	
	DBBindColInt(fileHandle,1,&record.m_id,&dbstatus[0]);
	
	
	while(DBFetchNext(fileHandle)==0)
	{
		//printf("record %s,%d\n",record.ProductId,record.id);
		//ListInsertItem(list,&record,END_OF_LIST);
		//printf("%d\n",record.m_id);
		*id = record.m_id;
	}
	DBDeactivateSQL (fileHandle); 
	
	return 0;
}

int insertReportRecord(tAutoTestRecord record)
{
	char SQLCommand[250];
	int  dbstatus[8];
	int  fileHandle;
	SUT sut=GetSeletedSut();
	Fmt(SQLCommand,"SELECT * FROM %s",sut.dbName);
	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//与数据库中的表建立连接
	record.m_id=-1;
	bindRecord(&record,fileHandle,dbstatus);

	DBCreateRecord (fileHandle); //创建记录  								s
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
	Fmt(SQLCommand,"SELECT * FROM %s order by m_id desc",sut.dbName);
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//与数据库中的表建立连接 
	//num=DBNumberOfRecords (fileHandle);//获取当前数据库中的记录
	
	
	bindRecord(&record,fileHandle,dbstatus);
	
	while(DBFetchNext(fileHandle)==0)
	{
		//printf("record %s,%d\n",record.ProductId,record.id);
		ListInsertItem(list,&record,END_OF_LIST);
	}
	DBDeactivateSQL (fileHandle); 
	
	return 0;
}

int updateRecord(tAutoTestRecord recordToUpdate)
{
	char SQLCommand[250];
	int  dbstatus[8];
	int  fileHandle;
	SUT sut=GetSeletedSut();
	//int id = 0;
	tAutoTestRecord record={0};
	//getInsertdRecordId(&id);
	
	Fmt(SQLCommand,"SELECT * FROM %s where m_id=%d",sut.dbName,recordToUpdate.m_id);
	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//与数据库中的表建立连接
	record.m_id =-1; //等于-1不bind
	bindRecord(&record,fileHandle,dbstatus);	
	
	while(DBFetchNext(fileHandle)==0)
	{
		record=recordToUpdate;
		if(DBPutRecord (fileHandle)!=DB_SUCCESS)
		{

			DBDeactivateSQL (fileHandle); 
			return -1; 
		}
	}
	DBDeactivateSQL (fileHandle); 	
	return 1;	
}


int updateUpload(int id,short flag)
{
	char SQLCommand[250];
	int  dbstatus[8];
	int  fileHandle;
	tAutoTestRecord record={0};
		SUT sut=GetSeletedSut(); 
	Fmt(SQLCommand,"SELECT * FROM %s where m_id=%d",sut.dbName,id);
	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//与数据库中的表建立连接
	record.m_id =-1; //等于-1不bind
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
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//与数据库中的表建立连接 
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
	
	fileHandle= DBActivateSQL(dBHandle,SQLCommand);//与数据库中的表建立连接
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
