#include <utility.h>
#include <ansi_c.h>
#include <cvintwrk.h>
#include <userint.h>
#include "ftpHelper.h"
#include "ftpConfigView.h"
#define MAX_FTP_PATHNAME_SIZE		260
#define MAX_FTP_FILENAME_SIZE		260
char server[50], username[50], password[50];
int passiveMode=1;

static void Error (int error)
{
	char s[50];

	if (error >= 0)
		return;
	sprintf (s, "ERROR %d recieved", error);
	MessagePopup (s, InetGetErrorMessage (error));
}

int ftpSendFile(char *pathName)
{
	int ftp_handle = -1; 
	int error=0;  
	char driveName[MAX_DRIVENAME_LEN]={0};
	char dirName[MAX_DIRNAME_LEN]={0};
	char fileName[MAX_FILENAME_LEN]={0};
	SplitPath (pathName, driveName, dirName, fileName);
	tFtpConfig config=GetFtpConfig();
	ftp_handle = InetFTPLogin (config.server, config.userName, config.password);
	if (ftp_handle < 0)
	{
		//MessagePopup ("An Error Occurred", "Can't Connect To Server");
		return -1;
	}	
	
	InetFTPSetPassiveMode (ftp_handle, config.mode);
	if ((error = InetFTPSendFile (ftp_handle, pathName, fileName,
				INET_FTP_FILE_TYPE_BINARY)) < 0)
	{
				Error (error);
				return -1;
	}


	
	if (ftp_handle >= 0)
		if ((error = InetFTPClose (ftp_handle)) < 0)
		{
				Error (error);
				return -1;
		}	
	return 0;
	
}


int sendFiles(char **fileArray,int fileNums)
{
	int ftp_handle = -1; 
	int error=0;  
	char local[MAX_FTP_PATHNAME_SIZE], remote[MAX_FTP_PATHNAME_SIZE] = "";
	ftp_handle = InetFTPLogin (server, username, password);
	if (ftp_handle < 0)
	{
		MessagePopup ("An Error Occurred", "Can't Connect To Server");
		return 0;
	}	
	
	InetFTPSetPassiveMode (ftp_handle, passiveMode);
	
	for(int i=0;i<fileNums;i++)
	{
		if ((error = InetFTPSendFile (ftp_handle, local, remote,
				INET_FTP_FILE_TYPE_BINARY)) < 0)
				Error (error);			
	}

	
	if (ftp_handle >= 0)
		if ((error = InetFTPClose (ftp_handle)) < 0)
				Error (error);	
	
}

