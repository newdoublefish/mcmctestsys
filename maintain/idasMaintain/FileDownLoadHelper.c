 /*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：FileDownLoadHelper.c
 // 文件功能描述：文件下载
 //
 // 
 // 创建标识：曾衍仁 20150930
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
#include <rs232.h>
#include <formatio.h>
#include "eutConfig.h"
#include "FileDownLoadHelper.h"
#include "matainHelper.h"
#include "SettingConfig.h"
#include "DownLoadFileHelperPanel.h"
#include "regexpr.h"     
#define TMP_DATA_PATH "\\tmpData\\"  

int strcompare(char *buffer1,char *buffer2)
{
	int matched,position,matchedLen;  
    char temp1[MAX_PATHNAME_LEN]={0};
	char temp2[MAX_PATHNAME_LEN]={0};
	RegExpr_FindPatternInText("[_][0-9]+[.]",1,buffer1,-1,1,1,&matched,&position,&matchedLen);
	memcpy(temp1,buffer1+position+1,matchedLen-2);
	RegExpr_FindPatternInText("[_][0-9]+[.]",1,buffer2,-1,1,1,&matched,&position,&matchedLen);
	memcpy(temp2,buffer2+position+1,matchedLen-2);	
	return strcmp(temp1,temp2);
	
}

void fileSelectPanel(char *dirInfoPath,char *selectedFile)
{
	    int file=0;
		long filesize=0;
		long readCount=0;
		long hasReadSize=0;
		char *buffer;
		int max=0;
		int maxItem=0;
		int itemCount=0;
		int ctrl=0, quit=0;   

		
		//GetFileSize (path, &filesize);
		
		int panelHandle=LoadPanel (0, "DownLoadFileHelperPanel.uir", PANEL); 
		
		
		FileExists (dirInfoPath, &filesize);

		if(filesize>0)
		{
		     char *buffer=(char *)malloc(filesize);
		
			
	         file=OpenFile (dirInfoPath, VAL_READ_ONLY, VAL_APPEND,
                            VAL_ASCII);
	    	 while(hasReadSize<filesize)
		     {   	
		        readCount=ReadFile(file,buffer+hasReadSize,filesize);
		        hasReadSize+=readCount;
		     } 
			 
			 int start=0;
			 int index=0;
			 while(index<filesize)
			 {
			     if(buffer[index]=='\0')
				 {
					 if(strlen(buffer+start)>0)
					 {
						
						InsertTreeItem (panelHandle, PANEL_TREE, VAL_SIBLING,0, VAL_LAST,
                                buffer+start,0,0,0);
						if(max==0)
						{
						   max=start;
						   //ATTR_MARK_REFLECT
							//SetTreeItemAttribute(panelHandle, PANEL_TREE,itemCount-1,ATTR_LABEL_BGCOLOR ,VAL_RED);
						   //SetActiveTreeItem(panelHandle,PANEL_TREE,itemCount-1,VAL_REPLACE_SELECTION_WITH_ITEM);
						   maxItem=itemCount;
						}else
						{
						    if(strcompare(buffer+max,buffer+start)<0)
							{
							   max=start;
							   //SetTreeItemAttribute(panelHandle, PANEL_TREE,itemCount-1,ATTR_LABEL_BGCOLOR ,VAL_RED); 
							   //SetActiveTreeItem(panelHandle,PANEL_TREE,itemCount-1,VAL_REPLACE_SELECTION_WITH_ITEM);
							   maxItem=itemCount;
							}	
						}
						itemCount++;   
				        //printf("%s\n",buffer+start); 
					 }
					 start=index+1;
				 }	
				 index++;
			 }	 
			 
		
		SetTreeItemAttribute(panelHandle, PANEL_TREE,maxItem,ATTR_LABEL_BGCOLOR ,VAL_GREEN);
		SetActiveTreeItem(panelHandle,PANEL_TREE,maxItem,VAL_REPLACE_SELECTION_WITH_ITEM); 
		CloseFile(file); 
#if 1	
		InstallPopup(panelHandle); 
        while (!quit) {//显示面板
          GetUserEvent (0, &panelHandle, &ctrl);
          if (ctrl == PANEL_DIRNEXT) {
			 
             DiscardPanel (panelHandle);
             quit=1;
          }
      }
#else
		
    	DiscardPanel (panelHandle);//不显示面板	
#endif		
	   //sprintf(selectedFile,"%s",buffer+max);
	   memcpy(selectedFile,buffer+max,strlen(buffer+max));
	   free(buffer);  
	}	
}


void setProcessImage(int panelHandle,int fileIndex)
{
	char imagePath[MAX_PATHNAME_LEN]={0};  
	sprintf(imagePath,"%d.ico",fileIndex);
	//SetCtrlAttribute (panelHandle, TIP_PICTURE_2, ATTR_IMAGE_FILE ,
     //imagePath);
	DisplayImageFile (panelHandle,TIP_PICTURE_2,imagePath);
}

static FD_RET downloadNewestFile2(char *srcDir,EUT eut,char *localFile)
{
	FD_RET ret=FALSE;
	BOOL matainResult=FALSE;
	int fileSize=0;
	int fileHandle=0;
	//step1 下载目录
	char dirInfoFileNameTemp[MAX_PATHNAME_LEN]={0};
	char dirInfoFileName[MAX_PATHNAME_LEN]={0}; 
	char selectedFile[MAX_PATHNAME_LEN]={0}; 
	char srcFile[MAX_PATHNAME_LEN]={0}; 
	char dstFile[MAX_PATHNAME_LEN]={0}; 
	GetProjectDir (dirInfoFileName); 
    sprintf(dirInfoFileName,"%s%s%s",dirInfoFileName,TMP_DATA_PATH,eut.eutName); 
	
	if(FileExists("tmpData",&fileSize)==0)
	{
	     MakeDir("tmpData");
	}
	
	if(FileExists(dirInfoFileName,&fileSize)==0) //不存在
	{	
	    MakeDir(dirInfoFileName);
	}
	sprintf(dirInfoFileNameTemp,"host:%s\\dirInfo.txt",dirInfoFileName); //创建目录文件
	sprintf(dirInfoFileName,"%s\\dirInfo.txt",dirInfoFileName); //创建目录文件   
	
	//printf("dirInfoFileNameTemp:%s\n",dirInfoFileNameTemp);
	//printf("srcFile:%s\n",srcDir);
	if(OpenComConfig (eut.matainConfig.portNum, NULL, eut.matainConfig.baudRate,eut.matainConfig.parity,eut.matainConfig.dataBit,eut.matainConfig.stopBit, 1024, 1024)<0)
	{
	    return FD_COMMON_CANNOT_OPEN; 
	}	

	Scene getDirScene=createDirInfoGet(srcDir,dirInfoFileNameTemp);
	getDirScene.noDateTimeOut=0;
	getDirScene.comPort=eut.matainConfig.portNum;
	getDirScene.saveFileDsctiption=0;
	matainResult=runSceneMatainProcess(&getDirScene);
	CloseCom(eut.matainConfig.portNum); 
	releaseScene(&getDirScene);
	if(matainResult!=TRUE)
	{	
		return FD_ERROR;
	}

	

	//step2 选择下载文件
	fileSelectPanel(dirInfoFileName,selectedFile);
	
	sprintf(srcFile,"%s%s",srcDir,selectedFile);//机载设备文件路径
	GetProjectDir (dstFile);
	sprintf(dstFile,"%s%s%s\\%s",dstFile,TMP_DATA_PATH,eut.eutName,selectedFile); 
	sprintf(localFile,"%s",dstFile);
	 //下载数据文件
	if(OpenComConfig (eut.matainConfig.portNum, NULL, eut.matainConfig.baudRate,eut.matainConfig.parity,eut.matainConfig.dataBit,eut.matainConfig.stopBit, 1024, 1024)<0)
	{
	    return FD_COMMON_CANNOT_OPEN; 
	}	
	//Scene scene= createFileReadScene(downLoadFile,1);
	Scene downLoadFileScene=createQauFileDownLoad(srcFile,dstFile); 
	downLoadFileScene.noDateTimeOut=0;
	downLoadFileScene.comPort=eut.matainConfig.portNum;
	//scene422.saveFileDsctiption=file;
	matainResult=runSceneMatainProcess(&downLoadFileScene);	
	CloseCom(eut.matainConfig.portNum);
	releaseScene(&downLoadFileScene); 
	//CloseCom(eut.matainConfig.portNum); 
	if(matainResult!=TRUE)
	{
		return FD_ERROR;
	}
	
	
	//step3 查看下载是否完成
	 int quitFlag=1;
	 int tipPanel=LoadPanel (0, "DownLoadFileHelperPanel.uir", TIP); 
	 DisplayPanel(tipPanel);
	 SetCtrlVal(tipPanel,TIP_TEXTMSG_2,selectedFile);
	 setProcessImage(tipPanel,0);
	 int fileIndex=0;
	 //增加超时退出
	 double startTimer=Timer();
	 ret=FD_SUCCESS;
	 while((quitFlag==1)&&(matainResult==TRUE)) 
	 {	 
		 Delay(1);
     	if(OpenComConfig (eut.matainConfig.portNum, NULL, eut.matainConfig.baudRate,eut.matainConfig.parity,eut.matainConfig.dataBit,eut.matainConfig.stopBit, 1024, 1024)<0)
    	{
	       return FD_COMMON_CANNOT_OPEN; 
     	}
	
	    Scene checkScene=createCheckDownLoad();
	    checkScene.comPort=eut.matainConfig.portNum;
	    matainResult=runSceneMatainProcess(&checkScene);
        CloseCom(eut.matainConfig.portNum);
	    
		if(checkScene.resultBuffer[0]==0x0C)
		{
		    quitFlag=0;
		}
		releaseScene(&checkScene);
		setProcessImage(tipPanel,fileIndex);
		fileIndex++;
		if(fileIndex>=7)
			fileIndex=0;
		
		if(Timer()-startTimer>150)   ////2分半钟还没有下载完数据就退出
		{	
			ret=FD_ERROR;
			break;
		}
			
	 }
	 DiscardPanel(tipPanel);
	if(matainResult!=TRUE)
		return FD_ERROR;	
	
	return ret;
}


static FD_RET downloadNewestFile1(char *srcDir,EUT eut,char *localFile)
{
	FD_RET ret=FALSE;
	BOOL matainResult=FALSE;
	int fileSize=0;
	int fileHandle=0;
	//step1 下载目录
	char dirInfoFileName[MAX_PATHNAME_LEN]={0}; 
	char selectedFile[MAX_PATHNAME_LEN]={0}; 
	char srcFile[MAX_PATHNAME_LEN]={0}; 
	char dstFile[MAX_PATHNAME_LEN]={0}; 
	GetProjectDir (dirInfoFileName); 
    sprintf(dirInfoFileName,"%s%s%s",dirInfoFileName,TMP_DATA_PATH,eut.eutName); 
	
	if(FileExists(dirInfoFileName,&fileSize)==0) //不存在
	{	
	    MakeDir(dirInfoFileName);
	}
	sprintf(dirInfoFileName,"%s\\dirInfo.txt",dirInfoFileName); //创建目录文件
	
	if(OpenComConfig (eut.matainConfig.portNum, NULL, eut.matainConfig.baudRate,eut.matainConfig.parity,eut.matainConfig.dataBit,eut.matainConfig.stopBit, 1024, 1024)<0)
	{
	    return FD_COMMON_CANNOT_OPEN; 
	}	
	fileHandle=OpenFile (dirInfoFileName, VAL_WRITE_ONLY, VAL_TRUNCATE,
                            VAL_ASCII);
	Scene fileReadscene= createFileReadScene(srcDir,1);
	fileReadscene.noDateTimeOut=0;
	fileReadscene.comPort=eut.matainConfig.portNum;
	fileReadscene.saveFileDsctiption=fileHandle;
	matainResult=runSceneMatainProcess(&fileReadscene);	
	CloseCom(eut.matainConfig.portNum);
	CloseFile(fileHandle);
	releaseScene(&fileReadscene);
	if(matainResult!=TRUE)
		return FD_ERROR;
	
	//step2 选择下载文件
	fileSelectPanel(dirInfoFileName,selectedFile);
	
	sprintf(srcFile,"%s%s",srcDir,selectedFile);//机载设备文件路径
	GetProjectDir (dstFile);
	sprintf(dstFile,"%s%s%s\\%s",dstFile,TMP_DATA_PATH,eut.eutName,selectedFile); 
	sprintf(localFile,"%s",dstFile);
	 //下载数据文件
	if(OpenComConfig (eut.matainConfig.portNum, NULL, eut.matainConfig.baudRate,eut.matainConfig.parity,eut.matainConfig.dataBit,eut.matainConfig.stopBit, 1024, 1024)<0)
	{
	    return FD_COMMON_CANNOT_OPEN; 
	}	
	//Scene scene= createFileReadScene(downLoadFile,1);
	Scene downLoadFileScene=createQauFileDownLoad(srcFile,dstFile); 
	downLoadFileScene.noDateTimeOut=0;
	downLoadFileScene.comPort=eut.matainConfig.portNum;
	//scene422.saveFileDsctiption=file;
	matainResult=runSceneMatainProcess(&downLoadFileScene);	
	CloseCom(eut.matainConfig.portNum);
	releaseScene(&downLoadFileScene); 	
	if(matainResult!=TRUE)
		return FD_ERROR;
	
	//step3 查看下载是否完成
	 int quitFlag=1;
	 int tipPanel=LoadPanel (0, "DownLoadFileHelperPanel.uir", TIP); 
	 DisplayPanel(tipPanel);
	 SetCtrlVal(tipPanel,TIP_TEXTMSG_2,selectedFile);
	 setProcessImage(tipPanel,0);
	 int fileIndex=0;
	 while((quitFlag==1)&&(matainResult==TRUE))
	 {	 
		 Delay(1);
     	if(OpenComConfig (eut.matainConfig.portNum, NULL, eut.matainConfig.baudRate,eut.matainConfig.parity,eut.matainConfig.dataBit,eut.matainConfig.stopBit, 1024, 1024)<0)
    	{
	       return FD_COMMON_CANNOT_OPEN; 
     	}
	
	    Scene checkScene=createCheckDownLoad();
	    checkScene.comPort=eut.matainConfig.portNum;
	    matainResult=runSceneMatainProcess(&checkScene);
        CloseCom(eut.matainConfig.portNum);
	    
		if(checkScene.resultBuffer[0]==0x0C)
		{
		    quitFlag=0;
		}
		releaseScene(&checkScene);
		setProcessImage(tipPanel,fileIndex);
		fileIndex++;
		if(fileIndex>=7)
			fileIndex=0;
			
	 }
	 DiscardPanel(tipPanel);
	if(matainResult!=TRUE)
		return FD_ERROR;	
	
	return FD_SUCCESS;
}

FD_RET downloadNewestFile(char *srcDir,EUT eut,char *localFile) 
{
    SETTING s=GetSetting();
	FD_RET ret=FALSE;
	
	if(s.methodToGetdirFile==0) //串口
	{
	     ret=downloadNewestFile1(srcDir,eut,localFile);  
	}else if(s.methodToGetdirFile==1)//网口
	{
	     ret=downloadNewestFile2(srcDir,eut,localFile);
	}
	
	return ret;
		
}
