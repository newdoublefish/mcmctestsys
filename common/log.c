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

#include <formatio.h>
#include "toolbox.h"
#include <ansi_c.h>
#include "utility.h"
#include "debug.h"
#include "log.h"
#include "settingConfig.h"


static char logFileName[MAX_PATHNAME_LEN]={0}; 
static int logFile=-1;
static int logLock=0; 
static BOOL muti=TRUE;

//#define REC_LOG

char LOG_NAME[][6]=
{
   "DEBUG",
   "ERROR",
   "INFO",
   "WARING",
 };	


static int getTime(char *timeString)
{
	unsigned int year, month, day, hour, min, sec, weekDay;
	CVIAbsoluteTime absTime;
    GetCurrentCVIAbsoluteTime(&absTime);
    CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
                &min, &sec, 0, &weekDay);
	sprintf(timeString,"%d%02d%02d%02d%02d%02d,",year,month,day,hour,min,sec);
	return 1;
}

static int getLogFileName(char *fileName)
{
	unsigned int year, month, day, hour, min, sec, weekDay;
	CVIAbsoluteTime absTime;

	
	char dirName[MAX_PATHNAME_LEN]={0};
	GetProjectDir (dirName);
	
    GetCurrentCVIAbsoluteTime(&absTime);
    CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
		&min, &sec, 0, &weekDay);
   if(muti)	
      sprintf(fileName,"%s\\log_%d%02d%02d%02d%02d%02d.txt",dirName,year,month,day,hour,min,sec); //每一次单独记录
   else
	  sprintf(fileName,"%s\\log.txt",dirName);  //会对上一次的log文件进行覆盖

	return 1;
}

int initLogModule(void)
{
	int ret=-1;
#ifdef REC_LOG	
    getLogFileName(logFileName);
	/*logFile=OpenFile (logFileName, VAL_WRITE_ONLY, VAL_APPEND,
                             VAL_ASCII);
	if(logFile>0)
	{
	   ret=1;
	}*/	
	CmtNewLock ("", OPT_TL_PROCESS_EVENTS_WHILE_WAITING, &logLock);//创建锁
#endif	
	return ret;
}

int LOG_REC_WITHOUTTIME(char *logMsg)
{
	int ret=-1;
	//char message[1024]={0};
	SETTING s= GetSetting();
	//printf("%d",s.saveLog);
	if(s.saveLog==1)
	{	
	  if(logLock!=0)
	  {	  
	     CmtGetLock (logLock);
	     if(-1==logFile)
	     {   
	       logFile=OpenFile (logFileName, VAL_WRITE_ONLY, VAL_TRUNCATE,
                            VAL_ASCII);
	    }
	    if(logFile>0)
	    {
		  //getTime(message);
		  //sprintf(message,"%s%s",message,logMsg);
		  WriteFile (logFile, logMsg, strlen(logMsg));
	    }
	    CmtReleaseLock(logLock);
	  }
	}
	return 1;
	
}

int LOG_REC(char *logMsg)
{
	int ret=-1;
	char message[1024]={0};
	SETTING s= GetSetting();
	//printf("%d",s.saveLog);
	if(s.saveLog==1)
	{	
	  if(logLock!=0)
	  {	  
	     CmtGetLock (logLock);
	     if(-1==logFile)
	     {   
	       logFile=OpenFile (logFileName, VAL_WRITE_ONLY, VAL_TRUNCATE,
                            VAL_ASCII);
	    }
	    if(logFile>0)
	    {
		  getTime(message);
		  sprintf(message,"%s%s",message,logMsg);
		  WriteFile (logFile, message, strlen(message));
	    }
	    CmtReleaseLock(logLock);
	  }
	}
	return 1;
	
}

void deleteLogFile(void)
{
    if(logFile!=-1)
	{
	   CloseFile(logFile);
	   DeleteFile(logFileName);
	   logFile=-1;
	}	
}

void quitLogModule(void)
{
#ifdef REC_LOG
      CmtDiscardLock(logLock);
	  logLock=0;
	  if(logFile!=-1)
	  {	  
          
		  CloseFile (logFile);
		  logFile=-1;   
	  }	
#endif	  
	  
}


