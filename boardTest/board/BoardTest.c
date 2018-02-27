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
#include "regexpr.h" 
#include "EutHelper.h"
								  
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
		SetComTime(eut.matainConfig.portNum,1);
	
		//int recvCnt=GetInQLen(eut.matainConfig.portNum);
		double elapsed = timeOutSeconds;
		double outTime = Timer();
		int totalRecvCnt=0;
		while(1)
		{				
			double currentTime = Timer();	
			if(currentTime-outTime > elapsed)
			{
				break;
			}			
			char buffer[128]={0};
			int recvLen=0;
			recvLen=ComRd(eut.matainConfig.portNum,buffer,128);
			if(recvLen<0)
			{
				return FALSE;
			}else if(recvLen > 0)
			{
				//strcat(resultBuffer,buffer);
				//sprintf(resultBuffer,"%s%s",resultBuffer,buffer);
				memcpy(resultBuffer+totalRecvCnt,buffer,recvLen);
				totalRecvCnt+=recvLen;
				outTime = Timer();
			}
			
			if(strstr(resultBuffer,"shell>")!=NULL)
			{
				break;
			}
		}
	}else{
		Delay(1);
	}

	CloseCom(eut.matainConfig.portNum);	 
	return TRUE;
}

BOOL sendCmdToBoardAndGetResultWithMessage(RSCONFIG config,char *cmd,char *resultBuffer,int maxResultBufferLen,double timeOutSeconds,int msgHandle)
{
	int RS232Error=0; 
	
    RS232Error = OpenComConfig (config.portNum,"",config.baudRate, config.parity,
                                        config.dataBit, config.stopBit, 0, 0);
	
	if(RS232Error<0)
	{
		  WarnShow1(0,"串口连接失败！");  
		  return FALSE;
	}
	
	if(sendCmdToBoard(config.portNum,cmd)==FALSE)
		return FALSE;

	
	if(resultBuffer!=NULL)
	{
		SetComTime(config.portNum,1);
	
		//int recvCnt=GetInQLen(eut.matainConfig.portNum);
		double elapsed = timeOutSeconds;
		double outTime = Timer();
		int totalRecvCnt=0;
		while(1)
		{				
			ProcessSystemEvents();
			double currentTime = Timer();	
			if(currentTime-outTime > elapsed)
			{
				break;
			}			
			char buffer[129]={0};
			int recvLen=0;
			recvLen=ComRd(config.portNum,buffer,128);
			if(recvLen<0)
			{
				return FALSE;
			}else if(recvLen > 0)
			{
				//strcat(resultBuffer,buffer);
				//sprintf(resultBuffer,"%s%s",resultBuffer,buffer);
				APPEND_INFO_FORMAT(msgHandle,"%s",buffer);
				memcpy(resultBuffer+totalRecvCnt,buffer,recvLen);
				totalRecvCnt+=recvLen;
				outTime = Timer();
			}
			
			if(strstr(resultBuffer,"shell>")!=NULL)
			{
				break;
			}
		}
	}else{
		Delay(1);
	}

	CloseCom(config.portNum);	 
	return TRUE;
}
								  

METHODRET BoardTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	char resultBuffer[1024]={0};
	char cmd[20]={0};
	
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k64串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}	
	
	if(getBoardCmd(group.groupName,cmd)==FALSE)
		return TEST_RESULT_ERROR;		
	
	APPEND_INFO_FORMAT(msgPanel,"send:%s",cmd); 
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,cmd,resultBuffer,1024,15,msgPanel)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	
	//APPEND_INFO_FORMAT(msgPanel,"recv:%s",resultBuffer);
	
	//printf("%s\n",resultBuffer);

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

	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k64串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,group.type,NULL,0,0,msgPanel)==FALSE)
		goto DONE;
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		char buffer[512]={0};
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		if(sendCmdToBoardAndGetResultWithMessage(resconfig,item.inputValue_,buffer,512,15,msgPanel)==FALSE)
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

double getSpiAdcValue(char *source,char *spiAdc)
{
	 int matched,position,matchedLen;
	 char pattern[50]={0};
	 sprintf(pattern,"%s = [0-9]*.[0-9]* ",spiAdc);
	 //printf("%d\n",strlen(source));
	 RegExpr_FindPatternInText(pattern,0,source,strlen(source),1,1,&matched,&position,&matchedLen);
	 if(matched)
	 {	
		char temp[20]={0};
	 	memcpy(temp,source+position+strlen(spiAdc)+3,matchedLen-strlen(spiAdc)-4);
		return atof(temp);
	 }
	return 0;		
}

METHODRET SpiAdcTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	char resultBuffer[512]={0};
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k64串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}	
	
	APPEND_INFO_FORMAT(msgPanel,"send:%s",group.type); 
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,group.type,resultBuffer,512,15,msgPanel)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	
	//APPEND_INFO_FORMAT(msgPanel,"recv:%s",resultBuffer);
	
	//printf("%s\n",resultBuffer);

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		double value = getSpiAdcValue(resultBuffer,item.itemName_);
		double standard = atof(item.standard_);
		if(value<standard +0.05 && value>standard-0.05)
		{
			itemResult.pass = RESULT_PASS;	
		}
		sprintf(itemResult.recvString,"%0.2f",value);
		/*if(strstr(resultBuffer,item.standard_)!=NULL)
		{
			itemResult.pass = RESULT_PASS;   
		}*/
		saveResult(hashTable,&itemResult);
	}
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerSpiAdcTestTPS(void)
{
	TPS tps=newTps("spiadc");
	tps.testFunction=SpiAdcTest;
	return tps;
}

double getAdcValue(char *source,char *adc)
{
	 int matched,position,matchedLen;
	 char pattern[50]={0};
	 sprintf(pattern,"%s: [0-9]*.[0-9]* ",adc);
	 //printf("%d\n",strlen(source));
	 RegExpr_FindPatternInText(pattern,0,source,strlen(source),1,1,&matched,&position,&matchedLen);
	 if(matched)
	 {	
		char temp[20]={0};
	 	memcpy(temp,source+position+strlen(adc)+2,matchedLen-strlen(adc)-4);
		return atof(temp);
	 }
	return 0;		
}

METHODRET AdcTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	char resultBuffer[1024]={0};
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k64串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}	
	
	APPEND_INFO_FORMAT(msgPanel,"send:%s",group.type); 
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,group.type,resultBuffer,1024,15,msgPanel)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	
	APPEND_INFO_FORMAT(msgPanel,"recv:%s",resultBuffer);
	
	//printf("%s\n",resultBuffer);

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		double value = getAdcValue(resultBuffer,item.itemName_);
		double standard = atof(item.standard_);
		if(value<standard +0.05 && value>standard-0.05)
		{
			itemResult.pass = RESULT_PASS;	
		}
		sprintf(itemResult.recvString,"%0.2f",value);
		/*if(strstr(resultBuffer,item.standard_)!=NULL)
		{
			itemResult.pass = RESULT_PASS;   
		}*/
		saveResult(hashTable,&itemResult);
	}
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerAdcTestTPS(void)
{
	TPS tps=newTps("adc");
	tps.testFunction=AdcTest;
	return tps;
}

METHODRET RtcTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	char resultBuffer[512]={0};
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k64串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}	
	
	APPEND_INFO_FORMAT(msgPanel,"send:%s","rtc w"); 
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,"rtc w",resultBuffer,512,15,msgPanel)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	//APPEND_INFO_FORMAT(msgPanel,"recv:%s",resultBuffer);
	
	memset(resultBuffer,0,512);
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,"rtc r",resultBuffer,512,15,msgPanel)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	//APPEND_INFO_FORMAT(msgPanel,"recv:%s",resultBuffer);	
	
	//printf("%s\n",resultBuffer);

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		itemResult.pass = RESULT_PASS;
		saveResult(hashTable,&itemResult);
	}
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerRTCTestTPS(void)
{
	TPS tps=newTps("rtc");
	tps.testFunction=RtcTest;
	return tps;
}

METHODRET RS485Test(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	char resultBuffer[512]={0};
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k64串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}	
	
	APPEND_INFO_FORMAT(msgPanel,"send:%s","uart"); 
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,"uart",resultBuffer,512,15,msgPanel)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	//APPEND_INFO_FORMAT(msgPanel,"recv:%s",resultBuffer);
	
	//printf("%s\n",resultBuffer);
	char *ptr1,*ptr2;
	ptr1 = resultBuffer;
	ptr2=strstr(resultBuffer,"CARD 2 TEST");
	*ptr2 = '\0';
	ptr2=ptr2+1;
	//printf("ptr1 %s\n",ptr1);
	//printf("ptr2 %s\n",ptr2); 

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		if(i==1)
		{
			if(strstr(ptr1,"NO")==NULL)
			{
				itemResult.pass = RESULT_PASS;	
				
			}
		}else{
			if(strstr(ptr2,"NO")==NULL)
			{
				itemResult.pass = RESULT_PASS;	
			}			
		}
		
		if(itemResult.pass == RESULT_PASS)
		{
			sprintf(itemResult.recvString,"%s","OK");
		}else{
			sprintf(itemResult.recvString,"%s","NO"); 
		}

		
		saveResult(hashTable,&itemResult);
	}
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerRS485TestTPS(void)
{
	TPS tps=newTps("485");
	tps.testFunction=RS485Test;
	return tps;
}

extern METHODRET manualTest(TestGroup group,EUT eut,HashTableType hashTable);
METHODRET LedDispTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	char resultBuffer[512]={0};
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k64串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}
	APPEND_INFO_FORMAT(msgPanel,"send:%s",group.type); 
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,group.type,resultBuffer,512,15,msgPanel)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	//APPEND_INFO_FORMAT(msgPanel,"recv:%s",resultBuffer);
	manualTest(group,eut,hashTable);
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerLedDispTestTPS(void)
{
	TPS tps=newTps("disp");
	tps.testFunction=LedDispTest;
	return tps;
}






