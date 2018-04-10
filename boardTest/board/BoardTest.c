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
	HashTableCreate(10,C_STRING_KEY,0,256,&boardCmdHashTable);
    return 0;	
}

HRESULT getBoardCmd(char *key,char *value)
{
   int found=0; 
   HashTableFindItem(boardCmdHashTable,key,&found);
   if(found==1)
   {
   	   HashTableGetItem(boardCmdHashTable,key,value,256);
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

int find_string(char str[], char substr[])
{
    int count = 0,i,j,check;
    int len = strlen(str);
    int sublen = strlen(substr);
    for(i = 0; i < len; i++)
    {
        check = 1;
        for(j = 0; j + i < len && j < sublen; j++)
        {
            if(str[i+j] != substr[j])
            {
                check = 0;
                break;
            }
        }
        if(check == 1)
        {
            count++;
            i = i + sublen;
        }
    }

    return count;
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
	
	if(cmd!=NULL)
	{
		if(sendCmdToBoard(config.portNum,cmd)==FALSE)
			return FALSE;
	}

	
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
	
	char resultBuffer[2048]={0};
	char cmd[512]={0};
	
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k60串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}	
	
	if(getBoardCmd(group.groupName,cmd)==FALSE)
		return TEST_RESULT_ERROR;		
	
	APPEND_INFO_FORMAT(msgPanel,"send:%s",cmd); 
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,cmd,resultBuffer,2048,15,msgPanel)==FALSE)
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
		
		if(strstr(item.standard_,"= 1")!=NULL)
		{
			if(itemResult.pass == RESULT_PASS)
			{
				sprintf(itemResult.recvString,"%s","1");
			}else{
				sprintf(itemResult.recvString,"%s","0");
			}		
		}else if(strstr(item.standard_,"= 0")!=NULL){
			
			if(itemResult.pass == RESULT_PASS)
			{
				sprintf(itemResult.recvString,"%s","0");
			}else{
				sprintf(itemResult.recvString,"%s","1");
			}		
		}
#if 0		
		if(strcmp(group.groupName,"di_全关")==0 || strcmp(group.groupName,"IODI防接反(打开防接反)")==0 )
		{
			if(itemResult.pass == RESULT_PASS)
			{
				sprintf(itemResult.recvString,"%s","0");
			}else{
				sprintf(itemResult.recvString,"%s","1");
			}
		}else if(strstr(group.groupName,"di_")!=NULL || strcmp(group.groupName,"IODI防接反(关闭防接反)")==0 )
		{
			if(itemResult.pass == RESULT_PASS)
			{
				sprintf(itemResult.recvString,"%s","1");
			}else{
				sprintf(itemResult.recvString,"%s","0");
			}
		}
#endif		
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

BOOL sendCmdToBoardAndGetResultWithMessageByCan(RSCONFIG config,char *cmd,char *resultBuffer,int maxResultBufferLen,double timeOutSeconds,int msgHandle)
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
			
			/*if(strstr(resultBuffer,"shell>")!=NULL)
			{
				break;
			}*/
		}
	}else{
		Delay(1);
	}

	CloseCom(config.portNum);	 
	return TRUE;
}

METHODRET CanTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;

	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k60串口",&resconfig))
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
		if(sendCmdToBoardAndGetResultWithMessageByCan(resconfig,item.inputValue_,buffer,512,1,msgPanel)==FALSE)
		{
			goto DONE;
		}
		
		/*if(strstr(buffer,"OK")!=NULL)
		{
			itemResult.pass =  RESULT_PASS;
		}*/
		if(strstr(buffer,"OK")!=NULL)
		{
			int okCnt = find_string(buffer,"OK");
			int standard = atof(item.standard_);
			if(okCnt == standard)
				itemResult.pass = RESULT_PASS;
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

METHODRET UartK60Test(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS; 
	char resultBuffer[512]={0};
	TestItem item={0};
	ListGetItem(group.subItems,&item,1);
	RESULT result={0};
	result.pass=RESULT_PASS;
	result.index=item.itemId;
	RSCONFIG K60config={0};
	RSCONFIG Rs485config={0}; 
	if(FALSE == getSerialConfig(eut.configList,"k60串口",&K60config))
	{
		result.pass=RESULT_FAIL;
		goto DONE;
	}

	if(FALSE == getSerialConfig(eut.configList,"RS485串口",&Rs485config))
	{
		result.pass=RESULT_FAIL;
		goto DONE;
	}	

	if(OpenComConfig (K60config.portNum,"",K60config.baudRate, K60config.parity,
                                        K60config.dataBit, K60config.stopBit, 0, 0)<0)
	{
		  WarnShow1(0,"k60 串口连接失败！");  
		result.pass=RESULT_FAIL;
		goto DONE;
	}
	
	if(OpenComConfig (Rs485config.portNum,"",Rs485config.baudRate, Rs485config.parity,
                                        Rs485config.dataBit, Rs485config.stopBit, 0, 0)<0)
	{
		  WarnShow1(0,"485 串口连接失败！");  
		result.pass=RESULT_FAIL;
		goto DONE;
	}	

	//step1 向K60发送uart命令
	APPEND_INFO_FORMAT(msgPanel,"K60 send:%s","uart");
	if(sendCmdToBoard(K60config.portNum,"uart")==FALSE){
		result.pass=RESULT_FAIL;
		goto DONE;
	}
		
	
	//step2 rs485回复
	if(ComRd(Rs485config.portNum,resultBuffer,512)<0)
	{
		result.pass=RESULT_FAIL;
		goto DONE;
	}
	APPEND_INFO_FORMAT(msgPanel,"RS485 recv:%s",resultBuffer); 
	if(strstr(resultBuffer,"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz123456789")==NULL)
	{
		result.pass=RESULT_FAIL;								
	}
	
	//step3 rs485发送 12345
	memset(resultBuffer,0,512);
	sprintf(resultBuffer,"%s","1234567890");
	APPEND_INFO_FORMAT(msgPanel,"RS485 send:%s",resultBuffer); 
	if(ComWrt (Rs485config.portNum,resultBuffer,strlen(resultBuffer))<0){
		result.pass=RESULT_FAIL;
		goto DONE;	
	}
	
	//step4 K60收到12345
		memset(resultBuffer,0,512);
		SetComTime(K60config.portNum,1);
	
		//int recvCnt=GetInQLen(eut.matainConfig.portNum);
		double elapsed = 20;
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
			recvLen=ComRd(K60config.portNum,buffer,128);
			if(recvLen<0)
			{
				return FALSE;
			}else if(recvLen > 0)
			{
				//strcat(resultBuffer,buffer);
				//sprintf(resultBuffer,"%s%s",resultBuffer,buffer);
				APPEND_INFO_FORMAT(msgPanel,"K60 recv:%s",buffer);
				memcpy(resultBuffer+totalRecvCnt,buffer,recvLen);
				totalRecvCnt+=recvLen;
				outTime = Timer();
			}
			
			if(strstr(resultBuffer,"shell>")!=NULL)
			{
				break;
			}
		}
		
		if(strstr(resultBuffer,"049 050 051 052 053 054 055 056 057 048")==NULL)
		{
			result.pass=RESULT_FAIL;		
		}
DONE:
	saveResult(hashTable,&result); 
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);
	CloseCom(K60config.portNum);
	CloseCom(Rs485config.portNum);	
	return ret;
}

TPS registerUartK60TestTPS(void)
{
	TPS tps=newTps("uart_k60");
	tps.testFunction=UartK60Test;
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
	if(FALSE == getSerialConfig(eut.configList,"k60串口",&resconfig))
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
	if(FALSE == getSerialConfig(eut.configList,"k60串口",&resconfig))
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
		double value=0;
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		if(i>4)
		{   
			char *temp = strstr(resultBuffer,"off");
			if(temp!=NULL)
				value = getAdcValue(temp,item.itemName_);
		}else{
			value = getAdcValue(resultBuffer,item.itemName_);
		}
		double standard = atof(item.standard_);
		if(value<standard +0.2 && value>standard-0.2)
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
		if(strstr(resultBuffer,"2017-04-12  15:30:47")!=NULL && strstr(resultBuffer,"2017-04-12  15:30:49")!=NULL)
		{
			itemResult.pass = RESULT_PASS; 	
			sprintf(itemResult.recvString,"%s","2017-04-12  15:30:49");
		}
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
	if(ptr2==NULL)
		goto DONE;
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

METHODRET LedDispCardTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
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



TPS registerLedDispCardTestTPS(void)
{
	TPS tps=newTps("card");
	tps.testFunction=LedDispCardTest;
	return tps;
}

METHODRET BoardDoTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	char resultBuffer[2048]={0};
	char cmd[512]={0};
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k60串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}
	
	if(getBoardCmd(group.groupName,cmd)==FALSE)
		return TEST_RESULT_ERROR;		
	
	APPEND_INFO_FORMAT(msgPanel,"send:%s",cmd); 	
	
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,cmd,resultBuffer,2048,15,msgPanel)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	//APPEND_INFO_FORMAT(msgPanel,"recv:%s",resultBuffer);
	manualTest(group,eut,hashTable);
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT result={0};
		HashTableGetItem(hashTable,&item.itemId,&result,sizeof(RESULT)); 
		if(result.pass == RESULT_PASS)
		{
			if(strcmp(group.groupName,"do_全亮")==0)
			{
				sprintf(result.recvString,"%s","1");
			}else{
				sprintf(result.recvString,"%s","0"); 
			}
		}else{
			if(strcmp(group.groupName,"do_全亮")==0)
			{
				sprintf(result.recvString,"%s","0");
			}else{
				sprintf(result.recvString,"%s","1"); 
			}			
		}
		saveResult(hashTable,&result); 
	}
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerBoardDoTestTPS(void)
{
	TPS tps=newTps("do");
	tps.testFunction=BoardDoTest;
	return tps;
}

METHODRET BoardK64Test(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	char resultBuffer[2048]={0};
	char cmd[512]={0};
	
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k64串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}	
	
	if(getBoardCmd(group.groupName,cmd)==FALSE)
		return TEST_RESULT_ERROR;		
	
	APPEND_INFO_FORMAT(msgPanel,"send:%s",cmd); 
	
	if(sendCmdToBoardAndGetResultWithMessage(resconfig,cmd,resultBuffer,2048,5,msgPanel)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
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
		
		if(strcmp(item.itemName_,"电源板断电重启功能")==0)
		{
			char *ptr = resultBuffer+100;
			if(strstr(ptr,item.standard_)!=NULL)
			{
				 itemResult.pass = RESULT_PASS;
			}
		}
		
		if(strcmp(item.itemName_,"时钟测试(rtc))")==0){
			if(itemResult.pass == RESULT_PASS)
			{
				sprintf(itemResult.recvString,"%s",item.standard_);
			}
		}
		
		if(strcmp(group.groupName,"选枪按键测试(key)")==0)
		{
			if(itemResult.pass == RESULT_PASS)
			{
				sprintf(itemResult.recvString,"%s",item.standard_);
			}else{
				sprintf(itemResult.recvString,"%s","0");
			}
		}
		
		saveResult(hashTable,&itemResult);
	}
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerBoardk64TestTPS(void)
{
	TPS tps=newTps("board_k64");
	//tps.autoTestFunction=DemoTest;
	tps.testFunction=BoardK64Test;
	//tps.createTpsPanel=NULL;
	//tps.manualTestFunction=DemoTest;
	return tps;
}

METHODRET UsbTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	char resultBuffer[2048]={0};
	char cmd[20]={0};
	
	RSCONFIG resconfig={0};
	if(FALSE == getSerialConfig(eut.configList,"k64串口",&resconfig))
	{
		return TEST_RESULT_ALLPASS;
	}	
	
	int RS232Error=0; 
	
    RS232Error = OpenComConfig (resconfig.portNum,"",resconfig.baudRate, resconfig.parity,
                                        resconfig.dataBit, resconfig.stopBit, 0, 0);
	
	if(RS232Error<0)
	{
		  WarnShow1(0,"串口连接失败！");  
		  goto DONE;
	}
	
	WarnShow1(0,"请插入USB");
	
	if(resultBuffer!=NULL)
	{
		SetComTime(resconfig.portNum,1);
	
		//int recvCnt=GetInQLen(eut.matainConfig.portNum);
		double elapsed = 5;
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
			recvLen=ComRd(resconfig.portNum,buffer,128);
			if(recvLen<0)
			{
				return FALSE;
			}else if(recvLen > 0)
			{
				//strcat(resultBuffer,buffer);
				//sprintf(resultBuffer,"%s%s",resultBuffer,buffer);
				APPEND_INFO_FORMAT(msgPanel,"%s",buffer);
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

	CloseCom(resconfig.portNum);
	
	//printf("%s\n",resultBuffer);
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		if(strstr(resultBuffer,"OK")!=NULL)
		{
			if(strstr(resultBuffer,"NO")==NULL)
			{
				itemResult.pass = RESULT_PASS;   
			}
		}
		saveResult(hashTable,&itemResult);
	}
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerUsbTestTPS(void)
{
	TPS tps=newTps("usb");
	//tps.autoTestFunction=DemoTest;
	tps.testFunction=UsbTest;
	//tps.createTpsPanel=NULL;
	//tps.manualTestFunction=DemoTest;
	return tps;
}


METHODRET UpdateTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	char cmd[512]={0};
	if(getBoardCmd(group.groupName,cmd)==FALSE){
		WarnShow1(0,"未检测到自动烧写程序脚本,请手动烧写程序");
	}else{
		if(FileExists(cmd,NULL)<=0)
		{
			WarnShow1(0,"未检测到到自动烧写程序脚本,请手动烧写程序");				
		}else{
			system(cmd);
		}
	}
		
	manualTest(group,eut,hashTable);
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}



TPS registerUpdateTestTPS(void)
{
	TPS tps=newTps("update");
	tps.testFunction=UpdateTest;
	return tps;
}

METHODRET CodeInput(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	APPEND_INFO_FORMAT(msgPanel,"开始测试:%s",group.groupName); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	manualTest(group,eut,hashTable);
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		HashTableGetItem(hashTable,&item.itemId,&itemResult,sizeof(RESULT)); 	
		
		if(strcmp(item.standard_,"NA")!=0)
		{
			char temp[100]={0};
			sprintf(temp,"%s%s",item.standard_,itemResult.recvString);
			memset(itemResult.recvString,0,RESULT_RECEIVE_LEN);
			sprintf(itemResult.recvString,"%s",temp);
			saveResult(hashTable,&itemResult);
		}
	}
DONE:	
	APPEND_INFO_FORMAT(msgPanel,"%s测试完毕",group.groupName);	

	return ret;
}

TPS registerCodeInputTestTPS(void)
{
	TPS tps=newTps("codeinput");
	tps.testFunction=CodeInput;
	tps.createTpsPanel=NULL;
	return tps;
}





