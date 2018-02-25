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
#include <userint.h>
#include "excelHelper.h" 
#include "toolbox.h"  
#include "BiboExcelParse.h"
#include "sutCommon.h" 
#include "debug.h"
#include "convertTool.h"								  
#include <rs232.h>
#include "tpsHelper.h"
#include "resultSave.h"
#include "common.h"
								  
#define SHEET_RANGE_TIPS "A2:B2"   
								  
static HashTableType boardCmdHashTable=0;

static HRESULT onCellListenerGetBoardCmds(VARIANT *MyVariant,int row,int column)    
{
	char *temp;
	static char key[50]={0}; 
	if(column==0)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			//memcpy(tempInstruction.name,temp,strlen(temp)+1);
			memset(key,0,50);
			memcpy(key,temp,strlen(temp)+1);
			//printf("%s,",temp);
			CA_FreeMemory(temp);
		 }else
		 {
			return EXCEL_TASK_QUIT;
		 }
	}else if(column==1)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
		    //tempInstruction.operation.testType=(unsigned char)StrToUnsignedLong(temp);   
			if(boardCmdHashTable!=0)
			{
			   HashTableInsertItem(boardCmdHashTable,key,temp);
			}
			//printf("%s\n",temp); 
			CA_FreeMemory(temp);
		 }
	}
	return EXCEL_TASK_OK;
}

static HRESULT onStartGetBoardCmds(VARIANT *MyVariant,int row,int column)				
{
	//istList=ListCreate(sizeof(INSTRUCTION));
	HashTableCreate(10,C_STRING_KEY,0,20,&boardCmdHashTable);
    return 0;	
}

HRESULT getBoardCmd(char *key,char *value)
{
   int found=0; 
   HashTableFindItem(boardCmdHashTable,key,&found);
   if(found==1)
   {
   	   HashTableGetItem(boardCmdHashTable,key,value,20);
   }
   return found;
}



BOOL BoardProtocolInit(char *sheetName)   
{
	if(boardCmdHashTable!=0)
		return TRUE;
	SUT sut=GetSeletedSut();
	EXCELTask task=createExcelTask(sut.configPath,sheetName,SHEET_RANGE_TIPS,3);
	task.onExcelTaskStartListener=(void *)onStartGetBoardCmds;
	task.onCellListener=(void *)onCellListenerGetBoardCmds;
	runExcelTask(task);
	return TRUE;
	//printf("%d\n",ListNumItems(paramList));
}
								  

BOOL sendCmdToBoard(int port,char *cmd)
{
	char str[100]={0};
	sprintf(str,"%s\r\n",cmd);
	int sndLen = strlen(str);
	if(ComWrt (port,str,strlen(str))!=sndLen)
		return FALSE;
	//FlushOutQ(port);
	return TRUE;
}

BOOL sendCmdToBoardAndGetResult(EUT eut,char *cmd,char *resultBuffer,int maxResultBufferLen,double timeOutSeconds)
{
	int RS232Error=0; 
	
    RS232Error = OpenComConfig (eut.matainConfig.portNum,"",eut.matainConfig.baudRate, eut.matainConfig.parity,
                                        eut.matainConfig.dataBit, eut.matainConfig.stopBit, 0, 0);
	
	if(RS232Error<0)
	{
		  WarnShow1(0,"串口连接失败！");  
		  return FALSE;
	}
	
	if(sendCmdToBoard(eut.matainConfig.portNum,cmd)==FALSE)
		return FALSE;

	if(resultBuffer!=NULL)
	{
		SetComTime(eut.matainConfig.portNum,timeOutSeconds);
	
		//int recvCnt=GetInQLen(eut.matainConfig.portNum);
		if(ComRd(eut.matainConfig.portNum,resultBuffer,maxResultBufferLen)<0)
		{
			return FALSE;
		}
	}else{
		Delay(1);
	}

	CloseCom(eut.matainConfig.portNum);	 
	return TRUE;
}
								  

METHODRET BoardTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	char resultBuffer[512]={0};
	char cmd[20]={0};
	
	if(getBoardCmd(group.groupName,cmd)==FALSE)
		return TEST_RESULT_ERROR;		
	
	APPEND_INFO_FORMAT(msgPanel,"cmd:%s",cmd); 
	
	if(sendCmdToBoardAndGetResult(eut,cmd,resultBuffer,512,5)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		if(strstr(resultBuffer,item.standard_)!=NULL)
		{
			itemResult.pass = RESULT_PASS;   
		}
		saveResult(hashTable,&itemResult);
	}
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerBoardTestTPS(void)
{
	TPS tps=newTps("board");
	//tps.autoTestFunction=DemoTest;
	tps.testFunction=BoardTest;
	tps.protocolInit=BoardProtocolInit;
	//tps.createTpsPanel=NULL;
	//tps.manualTestFunction=DemoTest;
	return tps;
}

METHODRET CanTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	if(sendCmdToBoardAndGetResult(eut,group.type,NULL,0,0)==FALSE)
		goto DONE;
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		char buffer[512]={0};
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		if(sendCmdToBoardAndGetResult(eut,item.inputValue_,buffer,512,3)==FALSE)
		{
			goto DONE;
		}
		
		if(strstr(buffer,"OK")!=NULL)
		{
			itemResult.pass =  RESULT_PASS;
		}
		
		saveResult(hashTable,&itemResult);
	}

DONE:
	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}

TPS registerCanTestTPS(void)
{
	TPS tps=newTps("can");
	tps.testFunction=CanTest;
	return tps;
}


