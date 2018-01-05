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

#include "excelHelper.h" 
#include <formatio.h>
#include <rs232.h>
#include "tpsHelper.h"
#include "resultSave.h"
#include "scpiHelper.h"
#include "sutCommon.h"
#include "common.h"
#include "regexpr.h"  
#include "AT9220Panel.h"
#include "mediaHelper.h"


static HashTableType resistProtocolHashTable=0; 
#define SHEET_RANGE_TIPS "A2:M2"

typedef struct {
	TestItem item;
	RESULT res;
}tTEST_RESULT;


static HRESULT onCellListenerResisProtocol(VARIANT *MyVariant,int row,int column)    
{
	char *temp;
	static tSCPICMD proc={0}; 
	if(column==0)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			//memcpy(tempInstruction.name,temp,strlen(temp)+1);
			memset(&proc,0,sizeof(tSCPICMD));
			memcpy(proc.name,temp,strlen(temp)+1);
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
			//memcpy(tempInstruction.name,temp,strlen(temp)+1);
			memcpy(proc.type,temp,strlen(temp)+1);
			//printf("%s,",temp);
			CA_FreeMemory(temp);
		 }		
	}else if(column==2)
	{
		
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.volt));
		}
	}else if(column==3)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.upper));
		}
	}else if(column==4)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.lower));
		}
	}else if(column==5)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.rtim));
		}
	}else if(column==6)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.ttim));
		}
	}else if(column==7)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.ftim));
		}
	}else if(column==8)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.arc));
		}
	}else if(column==9)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.freq));
		}
	}else if(column==10)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.wtim));
		}
	}else if(column==11)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.ramp));
		}
	}else if(column==12)
	{
		if(CA_VariantHasDouble(MyVariant))
	    {
			CA_VariantGetDouble(MyVariant,&(proc.range));
			if(resistProtocolHashTable!=0)
			{
			   HashTableInsertItem(resistProtocolHashTable,proc.name,&proc);
			}			
		}
	}
	return EXCEL_TASK_OK;
}

static HRESULT onStartResisProtocol(VARIANT *MyVariant,int row,int column)				
{
	//istList=ListCreate(sizeof(INSTRUCTION));
	//HashTableCreate(10,C_STRING_KEY,0,80,&tipsHashTable);
	if(resistProtocolHashTable==0)
		HashTableCreate(10,C_STRING_KEY,0,sizeof(tSCPICMD),&resistProtocolHashTable);
    return 0;	
}

void resisProtocolInit(char *name)
{
	 SUT sut=GetSeletedSut();
	 EXCELTask task=createExcelTask(sut.configPath,name,SHEET_RANGE_TIPS,13);
	 task.onExcelTaskStartListener=(void *)onStartResisProtocol;
	 task.onCellListener=(void *)onCellListenerResisProtocol;
	 runExcelTask(task);
}





void ComCallback(int portNumber, int eventMask, void *callbackdata)
{

//	if (eventMask & LWRS_RXFLAG)
	char	readBuf[256] = {0};
	int	    strLen=0;
	strLen = GetInQLen (portNumber);
	ComRd (portNumber, readBuf, strLen); 
	tTEST_RESULT *tr = (tTEST_RESULT *)callbackdata;
	tr->res.recevValue = readResistent(readBuf,tr->res.recvString);
	tr->res.pass = scpiResult(readBuf); 


}

tSCPICMD getResisiProc(char *itemName)
{
	tSCPICMD tpro={0};
	int foud=0;
	//printf("%s\n",itemName);
	HashTableFindItem(resistProtocolHashTable,itemName,&foud);
	if(foud)
	{
		HashTableGetItem(resistProtocolHashTable,itemName,&tpro,sizeof(tSCPICMD));
	}
	return tpro;
}

int loadAt9220Panel(tSCPICMD proc)
{
	char temp[20]={0};
	int panel = LoadPanel(0,"AT9220Panel.uir",AT9220);
	SetCtrlVal(panel,AT9220_type,proc.type);
	sprintf(temp,"%.2f kv",proc.volt);
	SetCtrlVal(panel,AT9220_voltage,temp);
	memset(temp,0,20);
	sprintf(temp,"%.2f",proc.upper);
	SetCtrlVal(panel,AT9220_upper,temp);
	memset(temp,0,20);
	sprintf(temp,"%.2f",proc.lower);
	SetCtrlVal(panel,AT9220_lower,temp);
	
	memset(temp,0,20);
	sprintf(temp,"%.2f s",proc.ttim);
	SetCtrlVal(panel,AT9220_time,temp);	
	DisplayPanel(panel);

	memset(temp,0,20);
	sprintf(temp,"%.0f s",proc.ttim);
	SetCtrlVal(panel,AT9220_TIME_LEFT,temp);	
	DisplayPanel(panel);	
	return panel;
}

void refreshAt9220Panel(int panel,int time)
{
		char temp[10]={0};
		if(time<0)
		{
			SetCtrlAttribute(panel,AT9220_TEXTMSG_4,ATTR_VISIBLE,1);
			return;
		}
		sprintf(temp,"%d s",time);
		SetCtrlVal(panel,AT9220_TIME_LEFT,temp);
}

void resistanceTestCallback(TestItem item,RESULT *resultPtr,int comPort)
{
	
	tSCPICMD proc=getResisiProc(item.itemName_);
	int panel = loadAt9220Panel(proc);
	scpiDispPage(comPort,MSETUP);
	Delay(0.2);
	scpiSendCmd(comPort,proc);
	scpiDispPage(comPort,MEASUREMENT);
	Delay(0.2);
	scpiStartTest(comPort);
	int outTime = proc.ttim+5;
	while(resultPtr->pass==-1)
	{

		refreshAt9220Panel(panel,outTime-5);
		Delay(1);
		ProcessSystemEvents ();
		outTime--;
		if(outTime<=0)
		{
			resultPtr->pass=0;
		}
	}
	scpiStopTest(comPort);
	if(resultPtr->pass==1)
	{
		soundSuccess();	
	}else{
		soundError();
	}
	
	DiscardPanel(panel);
	return;
}

METHODRET resistanceTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	char buffer[512]={0};
	int RS232Error=0;
	int flag=0;
	int eventMask = LWRS_RXFLAG;
	int eventChar = 10;//LN	

    RS232Error = OpenComConfig (eut.matainConfig.portNum,"",eut.matainConfig.baudRate, eut.matainConfig.parity,
                                        eut.matainConfig.dataBit, eut.matainConfig.stopBit, 0, 0);
	
	if(RS232Error<0)
	{
		  WarnShow1(0,"串口连接失败！");  
		  return TEST_RESULT_ERROR;
	}		 

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		//TestItem item;
		tTEST_RESULT tTestResult={0};
		
		ListGetItem(group.subItems,&tTestResult.item,i);
		tTestResult.res.index=tTestResult.item.itemId;
		tTestResult.res.pass=-1;
		InstallComCallback(eut.matainConfig.portNum,eventMask,0,eventChar,ComCallback, &tTestResult);
		resistanceTestCallback(tTestResult.item,&tTestResult.res,eut.matainConfig.portNum);
		saveResult(hashTable,&tTestResult.res);
	}
	
	CloseCom(eut.matainConfig.portNum);
	return ret;
}



TPS registerResistanceTestTPS(void)
{
	TPS tps=newTps("resistance");
	tps.autoTestFunction=resistanceTest;
	tps.protocolInit=resisProtocolInit;
	//tps.manualTestFunction=resistanceTest;
	return tps;
}
