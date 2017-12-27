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
#include "excelHelper.h" 
#include <formatio.h>
#include <rs232.h>
#include "tpsHelper.h"
#include "resultSave.h"
#include "scpiHelper.h"
#include "sutCommon.h"
#include "common.h"


static HashTableType voltageProtocolHashTable=0; 
#define SHEET_RANGE_TIPS "A2:M2"




static HRESULT onCellListenerVoltageProtocol(VARIANT *MyVariant,int row,int column)    
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
			if(voltageProtocolHashTable!=0)
			{
			   HashTableInsertItem(voltageProtocolHashTable,proc.name,&proc);
			}			
		}
	}
	return EXCEL_TASK_OK;
}

static HRESULT onStartVoltageProtocol(VARIANT *MyVariant,int row,int column)				
{
	//istList=ListCreate(sizeof(INSTRUCTION));
	//HashTableCreate(10,C_STRING_KEY,0,80,&tipsHashTable);
	if(voltageProtocolHashTable==0)
		HashTableCreate(10,C_STRING_KEY,0,sizeof(tSCPICMD),&voltageProtocolHashTable);
    return 0;	
}

void voltageProtocolInit(char *name)
{
	 SUT sut=GetSeletedSut();
	 EXCELTask task=createExcelTask(sut.configPath,name,SHEET_RANGE_TIPS,13);
	 task.onExcelTaskStartListener=(void *)onStartVoltageProtocol;
	 task.onCellListener=(void *)onCellListenerVoltageProtocol;
	 runExcelTask(task);
}



void voltageComCallback(int portNumber, int eventMask, void *callbackdata)
{

//	if (eventMask & LWRS_RXFLAG)
	char	readBuf[256] = {0};
	int	    strLen=0;
	strLen = GetInQLen (portNumber);
	ComRd (portNumber, readBuf, strLen); 
	//printf("%s",readBuf);
	tTEST_RESULT *tr = (tTEST_RESULT *)callbackdata;
	tr->res.pass=0;
	tr->res.recevValue = readElectricCurrent(readBuf,tr->res.recvString);
	if(strstr(tr->res.recvString,"mA"))
	{
		if(tr->res.recevValue<10)
			tr->res.pass=1;
	}else if(strstr(tr->res.recvString,"uA")){
		tr->res.pass=1;
	}
}

tSCPICMD getVoltageProc(char *itemName)
{
	tSCPICMD tpro={0};
	int foud=0;
	//printf("%s\n",itemName);
	HashTableFindItem(voltageProtocolHashTable,itemName,&foud);
	if(foud)
	{
		HashTableGetItem(voltageProtocolHashTable,itemName,&tpro,sizeof(tSCPICMD));
	}
	return tpro;
}


void voltageTestCallback(TestItem item,result *resultPtr,int comPort)
{
	tSCPICMD proc=getVoltageProc(item.itemName_);
	scpiDispPage(comPort,MSETUP);
	Delay(0.2);
	scpiSendCmd(comPort,proc);
	scpiDispPage(comPort,MEASUREMENT);
	Delay(0.2);
	scpiStartTest(comPort);
	int outTime = proc.ttim+5;
	while(resultPtr->pass==-1)
	{
		Delay(1);
		ProcessSystemEvents ();
		outTime--;
		if(outTime<=0)
		{
			resultPtr->pass=0;
		}
	}	
	return;
}

METHODRET voltageTest(TestGroup group,EUT eut,HashTableType hashTable)
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
		  WarnShow("��������ʧ�ܣ�");
		  return TEST_RESULT_ERROR;
	}	
	
	 

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		//TestItem item;
		tTEST_RESULT tTestResult={0};
		
		ListGetItem(group.subItems,&tTestResult.item,i);
		tTestResult.res.index=tTestResult.item.itemId;
		tTestResult.res.pass=-1;
		InstallComCallback(eut.matainConfig.portNum,eventMask,0,eventChar,voltageComCallback, &tTestResult);
		voltageTestCallback(tTestResult.item,&tTestResult.res,eut.matainConfig.portNum);
		saveResult(hashTable,&tTestResult.res) ;
	}
	
	CloseCom(eut.matainConfig.portNum);
	return TEST_RESULT_ALLPASS;
}



TPS registerVoltageTestTPS(void)
{
	TPS tps=newTps("voltage");
	tps.autoTestFunction=voltageTest;
	tps.protocolInit=voltageProtocolInit;
	//tps.manualTestFunction=resistanceTest;
	return tps;
}
