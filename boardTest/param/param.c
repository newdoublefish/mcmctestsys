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
#include <tcpsupp.h>
#include "tpsHelper.h"
#include "resultSave.h"
#include "ParamExcelParse.h"
#include "StubNetService.h"
#include "common.h"
#include "ParamPanel.h"
#include "BiboExcelParse.h"
#include "relayHelper.h"
#include "ParamSetGet.h"
#include "BmsHelper.h"


BOOL CheckGunPlugined(EUT eut,int gunIndex,int *result)
{
	char voltage[10]={0};
	char gunVoltage[20]={0};
	if(gunIndex==1)
	{
		sprintf(gunVoltage,"%s","ǹ1��ǹ���ӵ�ѹ");
	}else if(gunIndex==2) {
		sprintf(gunVoltage,"%s","ǹ2��ǹ���ӵ�ѹ"); 
	}else{
		return FALSE;
	}
	if(FALSE==ParamGetDepend(eut,gunVoltage,voltage))
	{
		return FALSE;
	}
	float vol = atof(voltage);
	if(vol>3.5 && vol < 4)
	{
		*result=1;
	}else{
		*result=0;
	}
	
	//if(vol
	return TRUE;	
}


static void getSysTime(char *timeBuffer)
{
	unsigned int year, month, day, hour, min, sec, weekDay;
	CVIAbsoluteTime absTime;
	GetCurrentCVIAbsoluteTime(&absTime);
    CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
                &min, &sec, 0, &weekDay);
	//sprintf(timeBuffer,"%02d-%02d-%02d %02d:%02d:%02d",year-2000,month,day,hour,min,sec);
	sprintf(timeBuffer,"%02d-%02d-%02d %02d:%02d:%02d:0000",year-2000,month,day,hour,min,sec);

}

static BOOL getYearMonthDay(char *temp,unsigned int *yearPtr,unsigned int*month,unsigned int *day)
{
		char delims[] = "-";
  		char *result = NULL;
   		result = strtok(temp, delims );
		int cnt=0;
   		while( result != NULL ) {
      	 	//printf("%s\n",result);
			if(cnt==0)
			{
				*yearPtr=atoi(result)+2000;
			}else if(cnt==1)
			{
				*month=atoi(result);
			}else if(cnt==2)
			{
				 *day=atoi(result);
			}
       	 	result = strtok( NULL, delims );
			cnt++;
 		}
		if(cnt!=3)
			return FALSE;
		return TRUE;
}

static BOOL getHourMinSec(char *temp,unsigned int *hourPtr,unsigned int*minPtr,unsigned int *secPtr)
{
		char delims[] = ":";
  		char *result = NULL;
   		result = strtok(temp, delims );
		int cnt=0;
   		while( result != NULL ) {
			if(cnt==0)
			{
				*hourPtr=atoi(result);
			}else if(cnt==1)
			{
				*minPtr=atoi(result);
			}else if(cnt==2)
			{
				 *secPtr=atoi(result);
			}
       	 	result = strtok( NULL, delims );
			cnt++;
	
 		}
		if(cnt!=4)
			return FALSE;
		return TRUE;
}


static BOOL timeStrToCviTime(const char *time,CVIAbsoluteTime *absTime)
{
		unsigned int year=0, month=0, day=0, hour=0, min=0, sec=0, weekDay=0;
		char yearMonthDay[20]={0};
		char hourMinSec[20]={0};
		char delims[] = " ";
  		char *result = NULL;
   		result = strtok(time, delims );
   		while( result != NULL ) {
      	 	
			if(strstr(result,"-")!=NULL)
			{
				sprintf(yearMonthDay,"%s",result);
			}else if(strstr(result,":")!=NULL)
			{
				sprintf(hourMinSec,"%s",result);
			} 
       	 	result = strtok( NULL, delims );
 		}
		if(FALSE==getYearMonthDay(yearMonthDay,&year, &month, &day))
		{
			return FALSE;
		}
		if(FALSE==getHourMinSec(hourMinSec,&hour, &min, &sec))
		{
			return FALSE;
		}
		if(CVIAbsoluteTimeFromLocalCalendar(year,month,day,hour,min,sec,0,absTime)<0)
		{
			return FALSE;
		}
		return TRUE;
}

static BOOL compareTimeWithTolerance(char *time1,char *time2,double intrv,int *result)
{
	char temp1[30]={0};
	char temp2[30]={0};
	sprintf(temp1,"%s",time1);
	sprintf(temp2,"%s",time2);
	CVIAbsoluteTime absTime1; 
	CVIAbsoluteTime absTime2;
	if(FALSE==timeStrToCviTime(temp1,&absTime1))
	{
		return FALSE;
	}
	if(FALSE==timeStrToCviTime(temp2,&absTime2))
	{
		return FALSE;
	}
	CVITimeInterval interval;
	CVITimeIntervalFromSeconds(intrv,&interval);
	if(CompareCVIAbsoluteTimesWithTolerance(absTime1,absTime2,interval,result)<0)
		return FALSE;
	return TRUE;
}

int CVICALLBACK showPramSetCtrlCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}

void showParamSetPanel(ListType paramsToSet)
{
	int panelHandle = LoadPanel(0,"ParamPanel.uir",PANEL);
	InsertTableRows (panelHandle,PANEL_TABLE,-1,ListNumItems(paramsToSet),VAL_USE_MASTER_CELL_TYPE); 
	InstallCtrlCallback(panelHandle,PANEL_COMMANDBUTTON,showPramSetCtrlCallback,&paramsToSet);
	for(int i=1;i<=ListNumItems(paramsToSet);i++)
	{
		PARAMETER param={0};
		ListGetItem(paramsToSet,&param,i);
		SetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(1,i),param.paramName);
		SetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(2,i),param.value);
		
	}
	SetActiveTableCell (panelHandle, PANEL_TABLE, MakePoint(2,1));
	FakeKeystroke(VAL_F2_VKEY); 
	DisplayPanel(panelHandle);
	RunUserInterface (); 
	for(int i=1;i<=ListNumItems(paramsToSet);i++)
	{
		PARAMETER param={0};
		ListGetItem(paramsToSet,&param,i);
		GetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(2,i),param.value);
		ListReplaceItem(paramsToSet,&param,i);
		
	}	
	DiscardPanel(panelHandle);
}

METHODRET ParamTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}
	ListType paramsToSet=ListCreate(sizeof(PARAMETER));
	ListType paramsToFetch=0;
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER param={0};
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"�޸ò�������");
			goto DONE;
		}
		ListInsertItem(paramsToSet,&param,END_OF_LIST);
	}
	
	paramsToFetch = ListCopy(paramsToSet);
	
	showParamSetPanel(paramsToSet);  
	
	for(int i=1;i<=ListNumItems(paramsToSet);i++)
	{
		PARAMETER paramSet={0},paramGet={0};
		ListGetItem(paramsToSet,&paramSet,i);
		ListGetItem(paramsToFetch,&paramGet,i);
		ConfigParameter(servicePtr,paramSet);
		if(GetParameter(servicePtr,&paramGet)>=0)
		{
			ListReplaceItem(paramsToFetch,&paramGet,i);
		}else{
			WarnShow1(0,"���ô���!");
		}
	}
																		 
	//showParamSetPanel(paramsToFetch); 
	
	
	//���ò���
	//TODO:���ز������Ա�
	//showParamSetPanel(paramsToSet);
	//showParamSetPanel(paramsToFetch);

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER paramSet={0};
		PARAMETER paramGet={0};
		ListGetItem(paramsToSet,&paramSet,i);
		ListGetItem(paramsToFetch,&paramGet,i); 
		RESULT itemResult;
		itemResult.index=item.itemId;
		if(strcmp(paramSet.value,paramGet.value)==0)
		{
			itemResult.pass=RESULT_PASS;
		}else{
			itemResult.pass=RESULT_FAIL;
		}
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s",paramGet.value);
		saveResult(hashTable,&itemResult);
	}	
DONE:	
	ListDispose(paramsToSet);
	ListDispose(paramsToFetch);
	return ret;
}



TPS registerParamTestTPS(void)
{
	TPS tps=newTps("param");
	tps.autoTestFunction=ParamTest;
	tps.protocolInit=ParamProtocolInit;
	tps.createTpsPanel=NULL;
	return tps;
}


METHODRET ParamCheckTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	APPEND_INFO(masgHandle,"�������"); 
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}
	//�����Լ�����
	if(startCommand(servicePtr,"product self_test\r\n")<0)
		goto DONE;  */
	
	if(strcmp("Ӳ���Լ�",group.groupName)==0)
	{
		if(FALSE==CommandSend(eut,"product self_test\r\n"))
		{
			APPEND_INFO(masgHandle,"�Լ������ʧ��");
			return ret;				
		}else{
			APPEND_INFO(masgHandle,"�Լ�����ͳɹ�");
		}
	}
	

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;		
		itemResult.pass=RESULT_PASS;
		
		if(strcmp(item.itemName_,"�Žڵ�")==0 || strcmp(item.itemName_,"������")==0 || strcmp(item.itemName_,"��ͣ��ť")==0)
		{
			
			double elapsed = 10;
			double outTime = Timer();
			itemResult.pass=RESULT_FAIL;
			while(itemResult.pass!=RESULT_PASS)
			{
				double currentTime = Timer();	
				if(currentTime-outTime > elapsed)
				{
					itemResult.pass=RESULT_FAIL;
					break;
				}				
				memset(itemResult.recvString,0,RESULT_RECEIVE_LEN);
				if(ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3)==FALSE)
				{
					APPEND_INFO(masgHandle,"��ȡ���ʧ��");				
					goto DONE;
				}else{
					APPEND_INFO_FORMAT(masgHandle,"%s",itemResult.recvString);
			
				}
			
				if(strstr(item.standard_,itemResult.recvString)!=NULL)
				{
					itemResult.pass=RESULT_PASS;
				}else{
					itemResult.pass=RESULT_FAIL;
				}
				
				ProcessSystemEvents ();
			}
			
			saveResult(hashTable,&itemResult);   			
			continue;					
		}
		
		if(strcmp(item.itemName_,"���Ʊ��")==0)
		{
		
			if(FALSE==ParamGetDepend(eut,"װ�ñ��",itemResult.recvString))
			{	
				APPEND_INFO_FORMAT(masgHandle,"%s:��ȡʧ��",item.itemName_); 
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandle,"%s,%s:��ȡ�ɹ�",item.itemName_,itemResult.recvString); 
			}
			char minPai[20]={0};
			memcpy(minPai,itemResult.recvString+10,7);
			memset(itemResult.recvString,0,RESULT_RECEIVE_LEN); 
			sprintf(itemResult.recvString,"%s%s","ZB",minPai);
			APPEND_INFO_FORMAT(masgHandle,"���Ʊ��λ:%s",itemResult.recvString);
			saveResult(hashTable,&itemResult);  
			continue;
		}
		
		if(FALSE==ParamGetDepend(eut,item.itemName_,itemResult.recvString))
		{	
			APPEND_INFO_FORMAT(masgHandle,"%s:��ȡʧ��",item.itemName_); 
			goto DONE;
		}else{
			APPEND_INFO_FORMAT(masgHandle,"%s,%s:��ȡ�ɹ�",item.itemName_,itemResult.recvString); 
		}
		itemResult.pass=RESULT_PASS;
		

		if(strcmp(group.groupName,"����汾���")==0){
			
			if(strcmp(item.standard_,"NA")!=0)
			{
				if(strcmp(item.standard_,itemResult.recvString)==0)
				{
					itemResult.pass=RESULT_PASS;
				}else{
					itemResult.pass=RESULT_FAIL;
				}
			}			
		}else if(strcmp(group.groupName,"���ʱ��")==0)
		{
			char sysTime[30]={0};
			getSysTime(sysTime);
			int compareResult=0;
			int tolerance=atoi(item.standard_);
			APPEND_INFO_FORMAT(masgHandle,"%s��ʱ��Ϊ:%s",item.itemName_,itemResult.recvString);
			APPEND_INFO_FORMAT(masgHandle,"��ǰϵͳʱ��:%s",sysTime);
			if(FALSE==compareTimeWithTolerance(sysTime,itemResult.recvString,tolerance,&compareResult))
			{
				APPEND_INFO(masgHandle,"ʱ���ʽ����");
				itemResult.pass=RESULT_FAIL;
			}else{
				if(compareResult==0)
				{
					APPEND_INFO(masgHandle,"ʱ������Χ��");
					itemResult.pass=RESULT_PASS;
				}else{
					APPEND_INFO(masgHandle,"ʱ�䲻����Χ��"); 
					itemResult.pass=RESULT_FAIL;
				}
			}									
		}else if(strstr(item.itemName_,"��ǹ���ӵ�ѹ")!=NULL){
			float vol = atof(itemResult.recvString);
			if(atof(item.standard_)>0.0001)
			{
				if(vol>(atof(item.standard_)-0.5) && vol <atof(item.standard_))
				{
					 itemResult.pass=RESULT_PASS;
				}else{
					itemResult.pass=RESULT_FAIL;
				}
			}					
		}else if(strcmp(item.standard_,"NA")!=0)
		{
			if(strstr(item.standard_,itemResult.recvString)!=NULL)
			{
				itemResult.pass=RESULT_PASS;
			}else{
				itemResult.pass=RESULT_FAIL;
			}
		}

		saveResult(hashTable,&itemResult);
	}
DONE:	
	//onStubDisConnected(servicePtr);
	APPEND_INFO(masgHandle,"�˳�����");
	return ret;
}



TPS registerParamsCheckTps(void)
{
	TPS tps=newTps("paramCheck");
	tps.testFunction=ParamCheckTest;
	tps.protocolInit=ParamProtocolInit;
	return tps;			
}

METHODRET ParamTemperatureTest(TestGroup group,EUT eut,HashTableType hashTable,int msgPanel)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
		return 	TEST_RESULT_ALLPASS;
	*/
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		APPEND_INFO(msgPanel,"��������ʧ��,�˳�����");
		return 	TEST_RESULT_ERROR;
	}
	APPEND_INFO(msgPanel,"�������ӳɹ�");
/*	ListType paramsToFetch=ListCreate(sizeof(PARAMETER));

	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER param={0};
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"�޸ò�������");
		
			goto DONE;
		}else{
		
		}
		ListInsertItem(paramsToFetch,&param,END_OF_LIST);
	}*/
	//TODO:���ز���
	//showParamSetPanel(paramsToFetch);
	//���ò���

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		RESULT itemResult;
		ListGetItem(group.subItems,&item,i);  
		itemResult.index=item.itemId;		
		
		/*PARAMETER paramGet={0};
		//ListGetItem(paramsToFetch,&paramGet,i); 
		///printfParam(paramGet);
		
		if(GetParameter(servicePtr,&paramGet)<0)
		{	
			WarnShow1(0,"��ȡʧ��");
			goto DONE;
		}*/
		
		if(FALSE==ParamGetDepend(eut,item.itemName_,itemResult.recvString))
		{
			APPEND_INFO_FORMAT(msgPanel,"%s:��ȡʧ��",msgPanel);		
		}else{
			APPEND_INFO_FORMAT(msgPanel,"%s:��ȡ�ɹ���ֵΪ%s",msgPanel,itemResult.recvString);			
		}

		itemResult.pass=RESULT_PASS;
		saveResult(hashTable,&itemResult);
	}	
DONE:	
	//ListDispose(paramsToFetch);
	onStubDisConnected(servicePtr);	
	return ret;
}

TPS registerParamTemperatureTps(void)
{
	TPS tps=newTps("temperature");
	tps.testFunction=ParamTemperatureTest;
	tps.protocolInit=ParamProtocolInit;
	return tps;			
}

#define BMS_CTRL

METHODRET PowerDistributeTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"�������");
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		APPEND_INFO(masgHandle,"��������ʧ��,�˳�����");
		return 	TEST_RESULT_ERROR;
	}	
	METHODRET ret = TEST_RESULT_ALLPASS;
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	char tips60A[20]={0};
	char tips100A[20]={0};
	char tips120A[30]={0};
	char voltage[20]={0};
	if(strcmp(group.groupName,"ǹ1���ʷ���")==0)
	{
		sprintf(startChargeCmd,"%s","1ǹ�����������");		
		sprintf(stopChargeCMD,"%s","1ǹ����ֹͣ���");
		sprintf(tips60A,"%s","��Դģ��1��2����");
		sprintf(tips100A,"%s","��Դģ��1��2��3����");
		sprintf(tips120A,"%s","��Դģ��1��2��3��6 ����");
	}else if(strcmp(group.groupName,"ǹ2���ʷ���")==0){
		sprintf(startChargeCmd,"%s","2ǹ�����������");		
		sprintf(stopChargeCMD,"%s","2ǹ����ֹͣ���");
		sprintf(tips60A,"%s","��Դģ��4��5����");
		sprintf(tips100A,"%s","��Դģ��4��5��6����");
		sprintf(tips120A,"%s","��Դģ��3��4��5��6 ����");	
	}
	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		APPEND_INFO(masgHandle,"���������������ʧ��"); 
		WarnShow1(0,"���������������ʧ�ܣ�");
		return ret;
	}else{
		APPEND_INFO(masgHandle,"�ѳɹ����������������");
	}
	
	if(FALSE==AlertDialogWithRet(0,"waring","������������̣����ҵ�ѹ�Ѿ��ȶ�","��","��"))
	{
			//getStubNetService(ip,port);
		return ret;
	}	
	
	//60A����
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item1={0};
		ListGetItem(group.subItems,&item1,i);
		RESULT itemResult1={0}; 
		itemResult1.index = item1.itemId;
		itemResult1.pass=RESULT_FAIL;		
		if(i==1)
		{

#ifdef BMS_CTRL			
			tBmsItem itemSet={
				0x120B,
				600,
			};
			if(FALSE == BmsSetItem(eut.bmsConfig,itemSet))
			{
				APPEND_INFO(masgHandle,"BMS �����������ʧ��"); 
				WarnShow1(0,"���ֶ����õ���Ϊ60A");				
			}
			APPEND_INFO(masgHandle,"BMS ��������Ѿ��ɹ�����Ϊ60A");
			if(FALSE==AlertDialogWithRet(0,"waring",tips60A,"����","��ȷ"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}			
			
#else
			WarnShow1(0,"���õ���Ϊ60A"); //����������485����������
			APPEND_INFO(masgHandle,"���õ���Ϊ60A");
			if(FALSE==AlertDialogWithRet(0,"waring",tips60A,"����","��ȷ"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}
#endif			
		}else if(i==2){
		
#ifdef BMS_CTRL			
			tBmsItem itemSet={
				0x120B,
				1000,
			};
			if(FALSE == BmsSetItem(eut.bmsConfig,itemSet))
			{
				APPEND_INFO(masgHandle,"BMS �����������ʧ��"); 
				WarnShow1(0,"���ֶ����õ���Ϊ100A");				
			}
			APPEND_INFO(masgHandle,"BMS ��������Ѿ��ɹ�����Ϊ100A");
			if(FALSE==AlertDialogWithRet(0,"waring",tips100A,"����","��ȷ"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}			
			
#else			
			WarnShow1(0,"���õ���Ϊ100A"); //����������485����������
			APPEND_INFO(masgHandle,"���õ���Ϊ100A");
			if(FALSE==AlertDialogWithRet(0,"waring",tips100A,"����","��ȷ"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}
#endif			
		}else if(i==3){
		

#ifdef BMS_CTRL			
			tBmsItem itemSet={
				0x120B,
				1200,
			};
			if(FALSE == BmsSetItem(eut.bmsConfig,itemSet))
			{
				APPEND_INFO(masgHandle,"BMS �����������ʧ��"); 
				WarnShow1(0,"���ֶ����õ���Ϊ120A");				
			}
			APPEND_INFO(masgHandle,"BMS ��������Ѿ��ɹ�����Ϊ120A");
			
			if(FALSE==AlertDialogWithRet(0,"waring",tips120A,"����","��ȷ"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}			
			/*tBmsItem itemGet={
				0x120B,
				1,
			};
			
			if(BmsGetItem(eut.bmsConfig,&itemGet)==FALSE)
			{
					
			}
			APPEND_INFO_FORMAT(masgHandle,"��ǰ�������Ϊ%d",itemGet.value/10);							
			if(itemGet.value!=itemSet.value)
			{

			}else{
				APPEND_INFO(masgHandle,"BMS ��������Ѿ��ɹ�����Ϊ120A");
			}*/  
#else	
			WarnShow1(0,"���õ���Ϊ120A"); //����������485����������
			APPEND_INFO(masgHandle,"���õ���Ϊ120A"); 
			if(FALSE==AlertDialogWithRet(0,"waring",tips120A,"����","��ȷ"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}
#endif			
		}
		//WarnShow1(0,"�����̵�������,��ȷ����ʼ����");
		
		//WarnAlert(0,"���Ժ�,�ȴ��̵����պ�",20);
 	
		//char BI[10]={0};

		
		int reTryCnt=0;
		while(reTryCnt++<5)
		{
			memset(itemResult1.recvString,0,RESULT_RECEIVE_LEN);
			if(FALSE==ParamGetDepend(eut,"BI",itemResult1.recvString ))
			{
				APPEND_INFO_FORMAT(masgHandle,"%s ��ȡ�����ʷ��䷴��ʧ�� %s");
				goto ERROR;			
			}
			unsigned int standard= HexStrToUnsignedInt(item1.inputValue_);
			unsigned int bi = atoi(itemResult1.recvString);
			unsigned int result = bi & 0x1378;
			//unsigned int result = bi & standard;
			memset(itemResult1.recvString,0,RESULT_RECEIVE_LEN);
			sprintf(itemResult1.recvString,"0x%x",bi);
		

			if(result == standard)
			{
				itemResult1.pass=RESULT_PASS; 		
			}else{
				itemResult1.pass=RESULT_FAIL;
			}
			
			
			
			APPEND_INFO_FORMAT(masgHandle,"%s ��ȡ�����ʷ��䷴�� %s,���Ϊ%d ��ȡ����:%d",item1.itemName_,itemResult1.recvString,itemResult1.pass,reTryCnt);
			if(itemResult1.pass==RESULT_PASS)
				break;
			//WarnAlert(0,"��ȴ�",10);
			if(FALSE==AlertDialogWithRetAutoClose(0,"waring","���ȷ�������ȴ�������ȡ���˳����β���","ȡ��","����",10))
			{
				break;
			}
		}		
		//sprintf(itemResult1.recvString,"%s",BI);
		saveResult(hashTable,&itemResult1);
	}		
ERROR:	
	//100A����

	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"�޷�ֹͣ���,�밴��ͣ��ť");
	}
	WarnShow1(0,"��ȴ���ȷ����������Ѿ�ֹͣ");
	APPEND_INFO(masgHandle,"�˳�����");	
	return ret;
}


TPS registerPowerDistributeTestTps(void)
{
	TPS tps=newTps("PowerDistribute");
	tps.testFunction=PowerDistributeTest;
	//tps.protocolInit=BiboProtocolInit;
	return tps;			
}

//ǹ��ά�� "http://www.gdmcmc.cn/qrcode.html?qrcode=881011000851";
//����ַ strAddress="1711424989000065";

BOOL checkScanResult(int panel)
{
	char gun1[128]={0},gun2[128]={0},biao1[128]={0},biao2[128]={0};
	GetCtrlVal(panel,SCANPANEL_SCAN1,gun1);
	GetCtrlVal(panel,SCANPANEL_SCAN2,gun2);
	GetCtrlVal(panel,SCANPANEL_SCAN3,biao1); 
	GetCtrlVal(panel,SCANPANEL_SCAN4,biao2);
	if(strcmp(gun1,gun2)==0)
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"����:ǹ1ǹ2��ַһ��");
		return FALSE;
	}

	
	if((strlen(gun1)!=strlen(gun2)) | (strlen(gun1)!=12))
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"����:ǹ1ǹ2��ַ���ô���");
		return FALSE;		
	}
	
	if(strcmp(biao1,biao2)==0)
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"����:��1��2��ַһ��");
		return FALSE;
	}	
	
	if((strlen(biao1)!=strlen(biao2)) | (strlen(biao2)!=12)) 
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"����:��1��2��ַ���ô���");
		return FALSE;		
	}	
	return TRUE;
}

int CVICALLBACK ParamScanCtrlCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(control == SCANPANEL_COMMANDBUTTON)
			 {
				 if(checkScanResult(panel)==TRUE)
			 	   QuitUserInterface(1);
			 }
			 break;
			
		case EVENT_VAL_CHANGED:
			 //QuitUserInterface(1);
			 if(control == SCANPANEL_SCAN1)
			 {
			 	char temp[100]={0};
				GetCtrlVal(panel,control,temp);
				if(strstr(temp,"http")!=NULL)
				{
					if(strlen(temp)>=52)
					{
							char *strPtr=strstr(temp,"=");
							if(strPtr!=NULL)
							{
								strPtr=strPtr+1;
								SetCtrlVal(panel,control,strPtr);
								char *tempPtr=(char*)callbackData;
								memcpy(tempPtr,strPtr,strlen(strPtr)-1);
							}										
							SetActiveCtrl(panel,SCANPANEL_SCAN2);
						//printf("%s\n",temp);
					}
				}
			 
			 } else if(control == SCANPANEL_SCAN2)
			 {
			 	char temp[100]={0};
				GetCtrlVal(panel,control,temp);
				if(strstr(temp,"http")!=NULL)
				{
					if(strlen(temp)>=52)
					{
							char *strPtr=strstr(temp,"=");
							if(strPtr!=NULL)
							{
								strPtr=strPtr+1;
								SetCtrlVal(panel,control,strPtr);
								char *tempPtr=(char*)callbackData;
								memcpy(tempPtr,strPtr,strlen(strPtr)-1);
							}										
							SetActiveCtrl(panel,SCANPANEL_SCAN3);
						//printf("%s\n",temp);
					}
				}
			 
			 }else if(control == SCANPANEL_SCAN3)
			 {
			 	char temp[100]={0};
				GetCtrlVal(panel,control,temp);
				if(strlen(temp)>=16){
					SetCtrlVal(panel,control,temp+4);
					SetActiveCtrl(panel,SCANPANEL_SCAN4);
				}
				
			 
			 }else if(control == SCANPANEL_SCAN4){
				char temp[100]={0};
				GetCtrlVal(panel,control,temp);
				if(strlen(temp)>=16){
					SetCtrlVal(panel,control,temp+4);
				}				
			 }
		     break;
	}
	return 0;
}

int CVICALLBACK ParaPanelCallback (int panelHandle, int event, void *callbackData, int eventData1, int eventData2){
	
	switch (event)
	{
		case EVENT_CLOSE:
			//if(checkScanResult(panelHandle)==TRUE) 
			 BOOL *operateFlag = (BOOL *)callbackData;
			 *operateFlag = FALSE;
			 QuitUserInterface(1);
		     break;
	}
	return 0;
}

//#define DEBUG_PARAM_SCAN_TEST
METHODRET ParaScanTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandler)
{
	APPEND_INFO_FORMAT(masgHandler,"����%s����",group.groupName);
	METHODRET ret = TEST_RESULT_ALLPASS;
	BOOL operateFlag = TRUE;
	char stubName[20]={0};
#ifndef DEBUG_PARAM_SCAN_TEST	
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return ret;
	}
#endif	
	
	int panelHandle=LoadPanel(0,"ParamPanel.uir",SCANPANEL);
	
	InstallPanelCallback(panelHandle,ParaPanelCallback,&operateFlag);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN1,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN2,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN3,ParamScanCtrlCallback,stubName);
	InstallCtrlCallback(panelHandle,SCANPANEL_SCAN4,ParamScanCtrlCallback,stubName); 
	InstallCtrlCallback(panelHandle,SCANPANEL_COMMANDBUTTON,ParamScanCtrlCallback,NULL);
	SetActiveCtrl(panelHandle,SCANPANEL_SCAN1);
	DisplayPanel(panelHandle); 
	RunUserInterface();
	HidePanel(panelHandle);
	if(FALSE == operateFlag)
	{
		DiscardPanel(panelHandle);
		return ret;
	}
#ifdef DEBUG_PARAM_SCAN_TEST	
	goto DONE;
#else	
	int stubNameLen=strlen(stubName);
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		PARAMETER param={0};
		ListGetItem(group.subItems,&item,i);
		
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		itemResult.pass=RESULT_FAIL;
		if(stubNameLen>0)
			itemResult.pass=RESULT_PASS;
		
		APPEND_INFO_FORMAT(masgHandler,"----------%s ����------------------",item.itemName_);
		if(1==i)
		{
			GetCtrlVal(panelHandle,SCANPANEL_SCAN1,itemResult.recvString); 
		}else if(2==i){
			GetCtrlVal(panelHandle,SCANPANEL_SCAN2,itemResult.recvString);
		}else if(3==i)
		{
			/*if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"�޸ò�������");
				continue;
			}
			sprintf(itemResult.recvString,"00000%s",stubName);
			sprintf(param.value,"%s",itemResult.recvString);
			if(ConfigParameter(servicePtr,param)<0)
				continue;
			if(GetParameter(servicePtr,&param)<0){
				continue;
			}
			
			if(strcmp(itemResult.recvString,param.value)==0)
			{
				itemResult.pass=RESULT_PASS;
			}else{
				itemResult.pass=0;
			}
			sprintf(itemResult.recvString,"%s",param.value);*/
			char setValue[20]={0};
			sprintf(setValue,"00000%s",stubName);
			
			if(FALSE==ParamSet(servicePtr,item.itemName_,setValue))
			{
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s ���óɹ���ֵΪ%s",item.itemName_,setValue);
			}
			APPEND_INFO(masgHandler,"��ʱ2s"); 
			Delay(2);
			if(FALSE == ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3))
			{
				APPEND_INFO_FORMAT(masgHandler,"%s ��ȡʧ��",item.itemName_);
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s ��ȡ�ɹ���ֵΪ%s",item.itemName_,itemResult.recvString);	
			}
			if(strcmp(itemResult.recvString,setValue)==0)
			{
				itemResult.pass=RESULT_PASS;
			}else{
				itemResult.pass=RESULT_FAIL;
			}			
			
			
		}else if(i==4)
		{
			/*if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"�޸ò�������");
				continue;
			}
			GetCtrlVal(panelHandle,SCANPANEL_SCAN3,itemResult.recvString);
			sprintf(param.value,"%s",itemResult.recvString);
			if(ConfigParameter(servicePtr,param)<0)
				continue;
			if(GetParameter(servicePtr,&param)<0){
				continue;
			}
			
			if(strcmp(itemResult.recvString,param.value)==0)
			{
				itemResult.pass=1;
			}else{
				itemResult.pass=0;
			}
			sprintf(itemResult.recvString,"%s",param.value);*/
			
			char setValue[20]={0};
			GetCtrlVal(panelHandle,SCANPANEL_SCAN3,setValue);
			if(FALSE==ParamSet(servicePtr,item.itemName_,setValue))
			{
				APPEND_INFO_FORMAT(masgHandler,"%s ����ʧ��",item.itemName_); 
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s ���óɹ���ֵΪ%s",item.itemName_,setValue);
			}
			APPEND_INFO(masgHandler,"��ʱ2s"); 
			Delay(2);
			/*if(FALSE==ParamGet(servicePtr,item.itemName_,itemResult.recvString))
			{
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s ��ȡ�ɹ���ֵΪ%s",item.itemName_,itemResult.recvString);	
			}*/
			if(FALSE == ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3))
			{
				APPEND_INFO_FORMAT(masgHandler,"%s ��ȡʧ��",item.itemName_);
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s ��ȡ�ɹ���ֵΪ%s",item.itemName_,itemResult.recvString);	
			}
			if(strcmp(itemResult.recvString,setValue)==0)
			{
				itemResult.pass=RESULT_PASS;
			}else{
				itemResult.pass=RESULT_FAIL;
			}			
			
		}else if(i==5)
		{
			char setValue[20]={0};
			GetCtrlVal(panelHandle,SCANPANEL_SCAN4,setValue);
			if(FALSE==ParamSet(servicePtr,item.itemName_,setValue))
			{
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s ���óɹ���ֵΪ%s",item.itemName_,setValue);
			}
			APPEND_INFO(masgHandler,"��ʱ2s");
			Delay(2);
			if(FALSE == ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3))
			{
				APPEND_INFO_FORMAT(masgHandler,"%s ��ȡʧ��",item.itemName_);
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s ��ȡ�ɹ���ֵΪ%s",item.itemName_,itemResult.recvString);	
			}
			if(strcmp(itemResult.recvString,setValue)==0)
			{
				itemResult.pass=RESULT_PASS;
			}else{
				itemResult.pass=RESULT_FAIL;
			}			
		}
		saveResult(hashTable,&itemResult);
	}
	DiscardPanel(panelHandle); 
	APPEND_INFO_FORMAT(masgHandler,"�뿪%s����",group.groupName); 
	return ret;
#endif	
DONE:	
	DiscardPanel(panelHandle);
#ifndef DEBUG_PARAM_SCAN_TEST	
	onStubDisConnected(servicePtr);
#endif	
	APPEND_INFO_FORMAT(masgHandler,"�뿪%s����",group.groupName); 
	return ret;
}

TPS registerScanTestTps(void)
{
	TPS tps=newTps("scan");
	tps.testFunction=ParaScanTest;
	return tps;	
}

METHODRET InverseWarnTest(TestGroup group,EUT eut,HashTableType hashTable,int msgHandle)
{
	APPEND_INFO(msgHandle,"���뷴�Ӹ澯����");
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	int flag1=-1;
	int flag2=-1;
	int flag3=-1;
	int gunIndex=0;
	int plugined=0;
	
	if(strcmp(group.groupName,"ǹ1��ط��Ӹ澯")==0)
	{
		gunIndex=1;
	}else if(strcmp(group.groupName,"ǹ2��ط��Ӹ澯")==0)
	{
		gunIndex=2;	
	}else{
		return ret;
	}
	
	if(FALSE==CheckGunPlugined(eut,gunIndex,&plugined))
	{
		return  TEST_RESULT_ALLPASS;
	}
											 
	if(plugined==0)
	{
		APPEND_INFO(msgHandle,"��⵽δ����ǹ");
		WarnShow1(0,"δ����ǹ");
		//return TEST_RESULT_ALLPASS;
	}else{
		APPEND_INFO(msgHandle,"��⵽ǹ�Ѿ�����");
	}
	
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return TEST_RESULT_ERROR;
	}
	
	TestItem item={0};
	ListGetItem(group.subItems,&item,1); 
	char queryResult[20]={0};
	if(ParamGet(servicePtr,item.itemName_,queryResult)<0)
	{
		APPEND_INFO_FORMAT(msgHandle,"%s:��ȡʧ��",item.itemName_); 
		goto DONE;
	}else{
		APPEND_INFO_FORMAT(msgHandle,"%s:��ȡ�ɹ�:%s",item.itemName_,queryResult);
	}
	
	if(strcmp(queryResult,"false")==0)
	{
		flag1=1;
	}else{
		flag1=0;
	}
	APPEND_INFO_FORMAT(msgHandle,"flag1:%d",flag1);
	
	if(OperatDoSet(eut.relayConfig,RELAY(31)|RELAY(3)|RELAY(4)|RELAY(2),MASK32)==FALSE)
	{
		goto DONE;
	}else{
		APPEND_INFO(msgHandle,"�̵��������ɹ�,�պ�2��3��4��31�̵���");	
	}
	
	//WarnAlert(0,);
	WarnAlert(0,"��ʱ��",8);
	
	memset(queryResult,0,20);
	if(ParamGet(servicePtr,item.itemName_,queryResult)<0)
	{
		APPEND_INFO_FORMAT(msgHandle,"%s:��ȡʧ��",item.itemName_);
		goto DONE;
	}else{
		APPEND_INFO_FORMAT(msgHandle,"%s:��ȡ�ɹ�:%s",item.itemName_,queryResult); 		
	}

	if(strcmp(queryResult,"true")==0)
	{
		flag2=1;
	}else{
		flag2=0;
	}	
	
	if(AlertDialogWithRet(0,"ǹ���","���������ʾӦ���ǹ����ָʾ����ʾӦ��ΪX","����","��ȷ")==FALSE)
	{
		flag2=0;
	}
														 
	APPEND_INFO_FORMAT(msgHandle,"flag2:%d",flag2); 
	

	if(OperatDoSet(eut.relayConfig,RELAY(2),MASK32)==FALSE)
	{
		goto DONE;
	}else{
		APPEND_INFO(msgHandle,"�̵��������ɹ�,�պ�2�̵���");	
	}
	
	WarnAlert(0,"��ʱ��",8); 
	
	memset(queryResult,0,20);
	if(ParamGet(servicePtr,item.itemName_,queryResult)<0)
	{
		APPEND_INFO_FORMAT(msgHandle,"%s:��ȡʧ��",item.itemName_);
		goto DONE;
	}else{
		APPEND_INFO_FORMAT(msgHandle,"%s:��ȡ�ɹ�:%s",item.itemName_,queryResult); 		
	}
	
	if(strcmp(queryResult,"false")==0)
	{
		flag3=1;
	}else{
		flag3=0;
	}	
	
	if(AlertDialogWithRet(0,"ǹ���","���������ʾӦ���ǹ����ָʾ����ʾӦ����ʧ","����","��ȷ")==FALSE)
	{
		flag3=0;
	}
	
	APPEND_INFO_FORMAT(msgHandle,"flag3:%d",flag3); 	
	
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	itemResult.pass = flag1 & flag2 & flag3;
	sprintf(itemResult.recvString,"{%d,%d,%d}",flag1,flag2,flag3);
	saveResult(hashTable,&itemResult);
	
	if(OperatDoSet(eut.relayConfig,RELAY(2),MASK32)==TRUE)
	{
		 APPEND_INFO(msgHandle,"�̵��������ɹ�,�պ�2�̵���");
	}
	APPEND_INFO(msgHandle,"�˳�����");
	return ret;
DONE:
	if(OperatDoSet(eut.relayConfig,RELAY(2),MASK32)==TRUE)
	{
		 APPEND_INFO(msgHandle,"�̵��������ɹ�,�պ�2�̵���");
	}	
	onStubDisConnected(servicePtr);
	APPEND_INFO(msgHandle,"�˳�����"); 
	return ret;
}

TPS registerInverseWarnTestTps(void)
{
	TPS tps=newTps("inverseWarn");
	tps.testFunction=InverseWarnTest;
	return tps;	
}

METHODRET InsulationTest(TestGroup group,EUT eut,HashTableType hashTable,int msgHander)
{
	APPEND_INFO_FORMAT(msgHander,"���룺%s",group.groupName);
	BOOL flag=TRUE; 
	METHODRET ret = TEST_RESULT_ALLPASS;
	TestItem item;
	ListGetItem(group.subItems,&item,1);
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	char isolateCMD[20]={0};
	if(strcmp(item.itemName_,"ǹ1��Ե���")==0)
	{
		sprintf(startChargeCmd,"%s","1ǹ�����������");		
		sprintf(stopChargeCMD,"%s","1ǹ����ֹͣ���");
		sprintf(isolateCMD,"%s","1ǹ��Ե�����");
	}else if(strcmp(item.itemName_,"ǹ2��Ե���")==0){
		sprintf(startChargeCmd,"%s","2ǹ�����������");		
		sprintf(stopChargeCMD,"%s","2ǹ����ֹͣ���");
		sprintf(isolateCMD,"%s","2ǹ��Ե�����");
	}

	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(30)|RELAY(2),MASK32))
	{
		APPEND_INFO(msgHander,"�̵�������ʧ�ܣ���");
		goto DONE;
	}else{
	    APPEND_INFO(msgHander,"�̵��������ɹ����պ�2��30�̵�������");
	}

	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"�޷�������磡");
		APPEND_INFO(msgHander,"�޷�����������磡"); 
		flag=FALSE; 
		goto DONE;
	}else{
		APPEND_INFO(msgHander,"�ѳɹ������������");
	}
	
	if(AlertDialogWithRet(0,"ǹ���","��ȷ�ϳ�繦������","����","��ȷ")==FALSE)
	{
		WarnShow1(0,"��繦�ܲ�������"); 
		flag=FALSE;
		goto DONE;
	}
	APPEND_INFO(msgHander,"��繦��������");
	//��ȡ���
	char inSoResult[10]={0};
	if(FALSE==ParamGetDepend(eut,isolateCMD,inSoResult))
	{
		WarnShow1(0,"��ȡ��Ե�����ʧ��"); 
	}
	
	//printf("%s\n",inSoResult);
	

	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"�޷�ֹͣ��磡�밴��ֹͣͣ"); 
		APPEND_INFO(msgHander,"���ͳ��ֹͣ����ʧ��"); 
		goto DONE;
	}else{
		APPEND_INFO(msgHander,"���ͳ��ֹͣ����ɹ�"); 
	}	
	
	//WarnAlert(0,"��ʱ��",30);
	WarnShow1(0,"��һ������");
	
	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(29) | RELAY(30)| RELAY(2),MASK32))
	{
		APPEND_INFO(msgHander,"�̵�������ʧ�ܣ���");
		goto DONE;
	}else{
	    APPEND_INFO(msgHander,"�̵��������ɹ����պ�2��29,30�̵�������");
	}
	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"�޷�������磡");
		APPEND_INFO(msgHander,"�޷�����������磡"); 
		flag=FALSE; 
		goto DONE;
	}else{
		APPEND_INFO(msgHander,"�ѳɹ������������");
	}	

	if(AlertDialogWithRet(0,"ǹ���","��ȷ�ϳ�繦���쳣,Ӧ���Զ�ֹͣ���","����","��ȷ")==FALSE)
	{
		flag=FALSE;
	}
	
	char inSoResult1[10]={0};
	if(FALSE==ParamGetDepend(eut,isolateCMD,inSoResult1))
	{
		WarnShow1(0,"��ȡ��Ե�����ʧ��"); 
	}
	
	//printf("%s\n",inSoResult1);	
	
	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		APPEND_INFO(msgHander,"���ͳ��ֹͣ����ʧ��");
		WarnShow1(0,"�޷�ֹͣ��磡�밴��ֹͣͣ"); 
		goto DONE;
	}else{
		APPEND_INFO(msgHander,"���ͳ��ֹͣ����ɹ�"); 
	}	
DONE:	
	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(2),MASK32))
	{
	}
	
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	itemResult.pass=flag;
	saveResult(hashTable,&itemResult);
	WarnShow1(0,"��ȷ����������Ѿ�ֹͣ");
	APPEND_INFO(msgHander,"�˳�����");
	return ret;	
}


TPS registerInsulationTestTestTps(void)
{
	TPS tps=newTps("insulation");
	tps.testFunction=InsulationTest;
	return tps;	
}

METHODRET ChargingTest(TestGroup group,EUT eut,HashTableType hashTable,int msgHandler)
{
	APPEND_INFO_FORMAT(msgHandler,"����%s",group.groupName);
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return TEST_RESULT_ERROR;
	}	
	METHODRET ret = TEST_RESULT_ALLPASS;
	BOOL flag=TRUE;
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}*/
	//���ģ��ȷ��
	TestItem item1;
	ListGetItem(group.subItems,&item1,1);
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	if(strcmp(group.groupName,"ǹ1��繦��")==0)
	{
		sprintf(startChargeCmd,"%s","1ǹ�����������");		
		sprintf(stopChargeCMD,"%s","1ǹ����ֹͣ���");
	}else if(strcmp(group.groupName,"ǹ2��繦��")==0){
		sprintf(startChargeCmd,"%s","2ǹ�����������");		
		sprintf(stopChargeCMD,"%s","2ǹ����ֹͣ���");	
	}	
	
	RESULT itemResult1={0};
	itemResult1.index=item1.itemId;
	itemResult1.pass=RESULT_PASS;
	
	/*if(ParamSet(servicePtr,"1ǹ�����������","1")==FALSE)
	{
		goto DONE;
	}
	onStubDisConnected(servicePtr);*/
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"�޷��������");
		APPEND_INFO(msgHandler,"�޷��������");
		APPEND_INFO_FORMAT(msgHandler,"�뿪%s",group.groupName);
		return TEST_RESULT_ALLPASS;
	}
	
	if(FALSE==AlertDialogWithRet(0,"waring","������������̣����ҵ�ѹ�Ѿ��ȶ�","��","��"))
	{
			//getStubNetService(ip,port);
		//APPEND_INFO(msgHandler,"�޷��������");
		APPEND_INFO_FORMAT(msgHandler,"�뿪%s",group.groupName);			
		return TEST_RESULT_ALLPASS;
	}
	APPEND_INFO(msgHandler,"������������̣����ҵ�ѹ�Ѿ��ȶ�");
	
	if(AlertDialogWithRet(0,"ǹ���","��ȷ�ϳ������������,���ȷ�ϼӸ���","����","��ȷ")==FALSE)
	{
		flag=FALSE;
		itemResult1.pass=RESULT_FAIL;
		saveResult(hashTable,&itemResult1);
		//return TEST_RESULT_ALLPASS;
		ParamSetDepend(eut,stopChargeCMD,"1");	
		return TEST_RESULT_ALLPASS;
	}
	saveResult(hashTable,&itemResult1);
	
	//�Ӹ���
	if(OpenDo(eut.relayConfig,31)==FALSE)
	{
		return 	TEST_RESULT_ERROR;
	}else{
		APPEND_INFO(msgHandler,"�պϼ̵���31�����ؼ��سɹ�");
	}	
	
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}*/

	WarnShow1(0,"����ȷ����ʼ����");
	APPEND_INFO(msgHandler,"��ʼ����"); 

	ListType paramsToSet=ListCreate(sizeof(PARAMETER));
	for(int i=2;i<=4;i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);

		PARAMETER param={0};
		sprintf(param.paramName,"%s",item.itemName_);
		ListInsertItem(paramsToSet,&param,END_OF_LIST); 
		/*
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"�޸ò�������");
			goto DONE;
		}
		
		if(GetParameter(servicePtr,&param)<0)
		{	
			WarnShow1(0,"��ȡʧ��");
			goto DONE;
		}			
		//printf("----------------------------%s\n",param.value);
		ListInsertItem(paramsToGet,&param,END_OF_LIST);*/
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		itemResult.pass=RESULT_FAIL;
		//sprintf(itemResult.recvString,"%s",param.value);
		if(ParamGetDepend(eut,item.itemName_,itemResult.recvString)==FALSE)
		{
			 APPEND_INFO_FORMAT(msgHandler,"%s��ȡʧ�ܹ�",item.itemName_);
			 itemResult.pass=RESULT_FAIL;
			 saveResult(hashTable,&itemResult);
			 goto DONE;
		}else{
			APPEND_INFO_FORMAT(msgHandler,"%s��ȡ�ɹ���ֵΪ%s",item.itemName_,itemResult.recvString); 	
		}
		float strValue=atof(itemResult.recvString);
		if(i==3)
		{
			if(strValue<352 && strValue>348)
			{
				itemResult.pass=RESULT_PASS;
			}
		}else if(i==2)
		{
			if(strValue<2.87 && strValue>2.67)
			{
				itemResult.pass=RESULT_PASS;
			}
		}else{
			itemResult.pass=RESULT_PASS; 
		}
		
		APPEND_INFO_FORMAT(msgHandler,"%s ���Խ��Ϊ %d",item.itemName_,itemResult.pass);
		
		saveResult(hashTable,&itemResult);			
	}
		
	//ListType paramsToSet=ListCopy(paramsToGet);
	showParamSetPanel(paramsToSet);
	APPEND_INFO(msgHandler,"��ʼ���������ֵ");	
	for(int i=1;i<=ListNumItems(paramsToSet);i++)
	{
		TestItem item;
		PARAMETER param={0};
		ListGetItem(paramsToSet,&param,i);
		ListGetItem(group.subItems,&item,i+4);
		RESULT itemResult={0};
		itemResult.index=item.itemId;
		sprintf(itemResult.recvString,"%s",param.value);
		float strValue=atof(param.value);
		itemResult.pass=RESULT_PASS; 
		if(i==2)
		{
			if(strValue<352 && strValue>348)
			{
				itemResult.pass=RESULT_PASS;
			}else{
				itemResult.pass=RESULT_FAIL;
			}
		}else if(i==1)
		{
			if(strValue<2.87 && strValue>2.67)
			{
				itemResult.pass=RESULT_PASS;
			}else{
				itemResult.pass=RESULT_FAIL;
			}
		}
		APPEND_INFO_FORMAT(msgHandler,"%s ֵΪ %s�����Խ��Ϊ %d",item.itemName_,itemResult.recvString,itemResult.pass);
		saveResult(hashTable,&itemResult);			
	}
	
	
	
DONE:
	ListDispose(paramsToSet);
	//ListDispose(paramsToGet);
	
	if(ParamSetDepend(eut,stopChargeCMD,"1")==FALSE)
	{
		WarnShow1(0,"����ֹͣ���,�밴�¼�ͣ��ť");		
	}else{
		APPEND_INFO(msgHandler,"�Ѿ�ֹͣ�������");
	}
	
	WarnShow1(0,"��ȷ���Ѿ�ֹͣ���");
	
	if(CloseDo(eut.relayConfig,31)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}else{
		APPEND_INFO(msgHandler,"�Ͽ��̵���31�ɹ�");
	}
	APPEND_INFO(msgHandler,"�뿪����");
	return ret;
}

TPS registerChargingTestTestTps(void)
{
	TPS tps=newTps("charging");
	tps.testFunction=ChargingTest;
	return tps;	
}





METHODRET TimeSetTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"�������"); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	RESULT itemResult={0};
	TestItem item;
	ListGetItem(group.subItems,&item,1);
	itemResult.index = item.itemId;
	if(strcmp("NA",item.inputValue_)!=0){
		sprintf(itemResult.recvString,"%s",item.inputValue_);			
	}else{
		getSysTime(itemResult.recvString);
	}
	
	if(FALSE==ParamSetDepend(eut,item.itemName_,itemResult.recvString))
	{
		itemResult.pass =RESULT_FAIL; 
		APPEND_INFO_FORMAT(masgHandle,"%s:����ʧ��",item.itemName_); 
		return ret;
	}else{
		itemResult.pass =RESULT_PASS; 
		APPEND_INFO_FORMAT(masgHandle,"%s:���óɹ�:%s",item.itemName_,itemResult.recvString); 		
	}

	saveResult(hashTable,&itemResult);
	APPEND_INFO(masgHandle,"�����Ͽ�����"); 
	ReleaseStubNetService();
	if(CloseDo(eut.relayConfig,1)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	WarnAlert(0,"������",3);

	
	if(OpenDo(eut.relayConfig,1)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	
	
	WarnAlert(0,"��ȷ���豸�Ѿ�����",30);
	TestItem item2;
	RESULT itemResult2={0};
	ListGetItem(group.subItems,&item2,2);	
	itemResult2.index = item2.itemId;  
	if(FALSE==ParamGetDepend(eut,item.itemName_,itemResult2.recvString))
	{
		itemResult2.pass =RESULT_FAIL; 
		APPEND_INFO_FORMAT(masgHandle,"%s:��ȡʧ��",item.itemName_); 
		return ret;
	}else{
		itemResult2.pass =RESULT_PASS; 
		APPEND_INFO_FORMAT(masgHandle,"%s:��ȡ�ɹ�:%s",item.itemName_,itemResult2.recvString); 		
	}
	int compareResult=0;
	int tolerance = atoi(item2.standard_);
	APPEND_INFO_FORMAT(masgHandle,"ʱ����ΧΪ:%d",tolerance);
	char sysTime[30]={0};
	getSysTime(sysTime);
	APPEND_INFO_FORMAT(masgHandle,"��ǰϵͳʱ��:%s",sysTime);
	if(FALSE==compareTimeWithTolerance(sysTime,itemResult2.recvString,tolerance,&compareResult))
	{
		APPEND_INFO(masgHandle,"ʱ���ʽ����");
		itemResult2.pass=RESULT_FAIL;
	}else{
		if(compareResult==0)
		{
			APPEND_INFO(masgHandle,"ʱ������Χ��");
			itemResult2.pass=RESULT_PASS;
		}else{
			APPEND_INFO(masgHandle,"ʱ�䲻����Χ��"); 
			itemResult2.pass=RESULT_FAIL;
		}
	}
	itemResult2.index = item2.itemId;
	saveResult(hashTable,&itemResult2); 
	APPEND_INFO(masgHandle,"�˳�����");	
	return ret;
}

TPS registerTimeSetTestTps(void)
{
	TPS tps=newTps("timeSet");
	tps.testFunction=TimeSetTest;
	return tps;	
}

METHODRET StubCmdTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;
		itemResult.pass=RESULT_PASS;
		if(strcmp(item.itemName_,"����У��")==0){
			char voltage[10]={0};
			if(FALSE==ParamGet(servicePtr,"ǹ1��ǹ���ӵ�ѹ",voltage))
			{
																												
			}
			
			if(FALSE==ParamGet(servicePtr,"ǹ2��ǹ���ӵ�ѹ",voltage))
			{
																												
			}			
			
			if(startCommand(servicePtr,"prd coeff ins\r\n")<0)
			{
				WarnShow1(0,"����У��ʧ��");
				itemResult.pass=RESULT_FAIL;
			}
		}else if(strcmp(item.itemName_,"���Ź����")==0){ 
			if(startCommand(servicePtr,"product watch_dog\r\n")<0)
			{
				WarnShow1(0,"���Ź����");
				itemResult.pass=RESULT_FAIL;
			}
			onStubDisConnected(servicePtr);
			servicePtr=NULL;
			WarnAlert(0,"���Ժ󣡣���",25);
			servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
			if(servicePtr==NULL)
			{
				itemResult.pass=RESULT_FAIL;
				return 	TEST_RESULT_ERROR;
			}			
		}else if(strcmp(item.itemName_,"������λ")==0){
			PARAMETER param={0};
			if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"�޸ò�������");
			}
			sprintf(param.value,"%s","1");
			if(ConfigParameter(servicePtr,param)>=0)
			{
		 		itemResult.pass =RESULT_PASS;
			}else{
				itemResult.pass=RESULT_FAIL;
			}
			onStubDisConnected(servicePtr);
			servicePtr=NULL;
			WarnAlert(0,"���Ժ󣡣���",30);
			servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
			if(servicePtr==NULL)
			{
				itemResult.pass=RESULT_FAIL;
				return 	TEST_RESULT_ERROR;
			}			
		}
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s","OK");
		saveResult(hashTable,&itemResult);
		
	}
	return ret;
}

TPS registerStubCmdTestTps(void)
{
	TPS tps=newTps("StubCmd");
	tps.autoTestFunction=StubCmdTest;
	tps.createTpsPanel=NULL;
	return tps;	
}

METHODRET StubIoTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	if(strcmp(group.groupName,"ǹ1�ֶ���������")==0)
	{
		sprintf(startChargeCmd,"%s","1ǹ�����������");		
		sprintf(stopChargeCMD,"%s","1ǹ����ֹͣ���");
	}else if(strcmp(group.groupName,"ǹ2�ֶ���������")==0){
		sprintf(startChargeCmd,"%s","2ǹ�����������");		
		sprintf(stopChargeCMD,"%s","2ǹ����ֹͣ���");	
	}
	TestItem item={0};
	ListGetItem(group.subItems,&item,END_OF_LIST);
	RESULT result={0};
	result.index = item.itemId;
	result.pass = RESULT_FAIL;
	
	if(ParamSetDepend(eut,startChargeCmd,"1")==FALSE)
	{
		WarnShow1(0,"�޷��������");
		goto DONE;
	}
	
	if(AlertDialogWithRet(0,"��ѡ��","��ȷ���ܹ��ֶ�����,���ֹͣ","����","��ȷ")==TRUE)
	{
		result.pass=RESULT_PASS;	
	}else{
		if(ParamSetDepend(eut,stopChargeCMD,"1")==FALSE)
		{
			WarnShow1(0,"�޷�ֹͣ�������");
			goto DONE;
		}		
	}
	
	WarnShow1(0,"��ȷ��������̽��������û�У����¼�ͣ");
DONE:	
	saveResult(hashTable,&result); 		
	return ret;
	
}


TPS registerStubIoTestTps(void)
{
	TPS tps=newTps("unlock");
	tps.autoTestFunction=StubIoTest;
	tps.createTpsPanel=NULL;
	return tps;	
}

METHODRET ParamEutCheckTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS; 
	//���̵�����
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem testItem={0};
		RESULT testResult={0};
		ListGetItem(group.subItems,&testItem,i);
		testResult.index=testItem.itemId;
		
		if(strcmp(testItem.itemName_,"�̵���")==0)
		{
			if(FALSE==OperatDoSet(eut.relayConfig,0,MASK16))
			{
				//APPEND_INFO(msgHander,"�̵�������ʧ�ܣ���");
				WarnShow1(0,"�̵���ͨ��ʧ��");
				return TEST_RESULT_ERROR;
			}					
		}else if(strcmp(testItem.itemName_,"���������")==0)
		{
		
		
		}else if(strcmp(testItem.itemName_,"BMSģ����")==0)
		{
				tBmsItem item={
					0x120B,
					1,
				};			
				if(BmsGetItem(eut.bmsConfig,&item)==FALSE)
				{
					WarnShow1(0,"BMSģ����ͨ��ʧ��"); 
					return TEST_RESULT_ERROR; 
				}			
			
		}else if(strcmp(testItem.itemName_,"���׮����")==0)
		{
			
		}
	}
	return ret;
}

TPS registerParamEutCheckTestTps(void)
{
	TPS tps=newTps("CommunicationCheck");
	tps.autoTestFunction=ParamEutCheckTest;
	tps.createTpsPanel=NULL;
	return tps;	
}
#if 0
METHODRET BIBOTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"�������");
	METHODRET ret = TEST_RESULT_ALLPASS;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT result={0};
		tBIBO bibo={0};
		result.index = item.itemId;
		if(FALSE==getBibo(item.itemName_,&bibo))
		{
			goto DONE;
		}
		APPEND_INFO_FORMAT(masgHandle,"itemName:%s,BI:%x,BO:%x",item.itemName_,bibo.maskBi,bibo.maskBo); 
		char value[30]={0};
		sprintf(value,"%d",bibo.maskBo);
		if(FALSE==ParamSetDepend(eut,"BO",value))
		{
			APPEND_INFO(masgHandle,"�������ݳ�BOʧ��");
			goto DONE;
		}
		APPEND_INFO_FORMAT(masgHandle,"�������ݳ�BO�ɹ�:%s",value);
		memset(value,0,30);
		if(FALSE==ParamGetDepend(eut,"BI",result.recvString))
		{
			APPEND_INFO(masgHandle,"��ȡ���ݳ�BIʧ��"); 
			goto DONE;
		}
		APPEND_INFO_FORMAT(masgHandle,"��ȡ���ݳ�BO�ɹ�:%s",result.recvString); 
		unsigned int valueUi= HexStrToUnsignedInt(result.recvString);
		if(bibo.maskBi&valueUi == bibo.maskBi)
		{
			result.pass = RESULT_PASS;			
		}
		
		saveResult(hashTable,&result);
		
		
	}
	APPEND_INFO(masgHandle,"�˳�����");	
DONE:	
	return ret;
}
#else
METHODRET BIBOTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"�������");
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	if(FALSE==ParamSetDependWithRetry(eut,"DO��һ���Ʊ�־","1",3))
	{
		APPEND_INFO(masgHandle,"DO��һ���Ʊ�־ ʧ��");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(masgHandle,"DO��һ���Ʊ�־ �ɹ�"); 
	}	
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		char setVal[10]={0};
		ListGetItem(group.subItems,&item,i);
		if(strcmp(item.standard_,"true_")==0)
		{
			sprintf(setVal,"%s","1");
		}else{
			sprintf(setVal,"%s","0"); 
		}
		RESULT result={0};
		result.index = item.itemId;
		APPEND_INFO_FORMAT(masgHandle,"���Լ̵���%s",item.itemName_);
		if(strcmp(item.itemName_,"ǹ1K1K2")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"1ǹK1K2����",setVal,3))
			{
				APPEND_INFO(masgHandle,"�պϼ̵��� 1ǹK1K2 ʧ��");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"�պϼ̵��� 1ǹK1K2 �ɹ�"); 
			}
		}else if(strcmp(item.itemName_,"ǹ2K1K2")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"2ǹK1K2����",setVal,3))
			{
				APPEND_INFO(masgHandle,"�պϼ̵��� 2ǹK1K2 ʧ��");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"�պϼ̵��� 2ǹK1K2 �ɹ�"); 
			}
			
		}else if(strcmp(item.itemName_,"K5K6")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"1ǹK5K6����",setVal,3))
			{
				APPEND_INFO(masgHandle,"�պϼ̵��� 1ǹK5K6���� ʧ��");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"�պϼ̵��� 1ǹK5K6���� �ɹ�"); 
			}	
		}else if(strcmp(item.itemName_,"K7K8")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"K7K8����",setVal,3))
			{
				APPEND_INFO(masgHandle,"�պϼ̵��� K7K8���� ʧ��");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"�պϼ̵��� K7K8���� �ɹ�"); 
			}	
		}else if(strcmp(item.itemName_,"K9K10")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"2ǹK9K10����",setVal,3))
			{
				APPEND_INFO(masgHandle,"�պϼ̵��� 2ǹK9K10���� ʧ��");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"�պϼ̵��� 2ǹK9K10���� �ɹ�"); 
			}	
		}
		
		int reTryCnt=0;
		while(reTryCnt++<5)
		{
			memset(result.recvString,0,RESULT_RECEIVE_LEN);
			if(FALSE==ParamGetDepend(eut,"BI",result.recvString ))
			{
				APPEND_INFO_FORMAT(masgHandle,"%s ��ȡ���̵�������ʧ�� %s");
				goto DONE;			
			}
			unsigned int standard= HexStrToUnsignedInt(item.inputValue_);
			unsigned int bi = atoi(result.recvString);
			unsigned int resultUi = bi & 0x1378;
			resultUi = resultUi & standard;
			//unsigned int result = bi & standard;
			memset(result.recvString,0,RESULT_RECEIVE_LEN);
			sprintf(result.recvString,"0x%x",bi);
		
 			if(strcmp(item.standard_,"true_")==0)
			{
				if(resultUi == standard)
				{
					result.pass=RESULT_PASS; 		
				}else{
					result.pass=RESULT_FAIL;
				}
			}else{
				if(resultUi == 0)
				{
					result.pass=RESULT_PASS; 		
				}else{
					result.pass=RESULT_FAIL;
				}
			}

			APPEND_INFO_FORMAT(masgHandle,"%s ��ȡ���̵�������ʧ�� %s,���Ϊ%d ��ȡ����:%d",item.itemName_,result.recvString,result.pass,reTryCnt);
			if(result.pass==RESULT_PASS)
				break;
			//WarnAlert(0,"��ȴ�",10);
			if(FALSE==AlertDialogWithRetAutoClose(0,"waring","���ȷ�������ȴ�������ȡ���˳����β���","ȡ��","����",10))
			{
				break;
			}
		}			
		saveResult(hashTable,&result); 

	}
	APPEND_INFO(masgHandle,"�˳�����");	
DONE:	
	return ret;
}
#endif


TPS registerBIBOTestTps(void)
{
	TPS tps=newTps("BIBO");
	tps.testFunction=BIBOTest;
	tps.protocolInit=BiboProtocolInit;
	return tps;			
}

int CVICALLBACK CtrlBoxCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			if(control == BOXCODE_COMMANDBUTTON)
			 {
				char temp[30]={0};
				char *code = (char *)callbackData;
				GetCtrlVal(panel,BOXCODE_CODE,temp);
				if(strlen(temp)!=12)
				{
					SetCtrlVal(panel,BOXCODE_ERRORINFO,"��ʽ���ԣ�");				
				}else{
				
					sprintf(code,"%s",temp);
					QuitUserInterface(1);
				}
			 }
			 break;
	}
	return 0;
}


METHODRET CtrlBoxSetTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"�������"); 
	METHODRET ret = TEST_RESULT_ALLPASS; 
	char code[20]={0};
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}*/
	int panelHandle = LoadPanel(0,"ParamPanel.uir",BOXCODE);
	InstallCtrlCallback(panelHandle,BOXCODE_COMMANDBUTTON,CtrlBoxCallback,code);
	DisplayPanel(panelHandle);
	RunUserInterface();
	DiscardPanel(panelHandle);
	
	APPEND_INFO_FORMAT(masgHandle,"���ƺб���ǣ�%s",code);
	
	TestItem item={0};
	RESULT result={0};
	ListGetItem(group.subItems,&item,1);
	result.index = item.itemId;
	
	if(FALSE==ParamSetDepend(eut,item.itemName_,code))
	{
	
		APPEND_INFO(masgHandle,"���ݺв���ʧ��"); 
		goto DONE;
	}
	
	APPEND_INFO(masgHandle,"���ƺб�����óɹ�"); 	
	if(FALSE==ParamGetDepend(eut,item.itemName_,result.recvString))
	{
	
		APPEND_INFO(masgHandle,"���ݺв���ʧ��");
		goto DONE;
	}
	APPEND_INFO_FORMAT(masgHandle,"���ƺж��سɹ�����ǣ�%s",result.recvString);
	
	
	if(strcmp(code,result.recvString)==0)
	{
		result.pass=RESULT_PASS;
	}
	

	saveResult(hashTable,&result); 
		
	APPEND_INFO(masgHandle,"�˳�����");	
DONE:	
	return ret;
}


TPS registerCtrlBoxSetTestTps(void)
{
	TPS tps=newTps("CtrlBoxSet");
	tps.testFunction=CtrlBoxSetTest;
	return tps;			
}

#if 0
METHODRET PowerModuleTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"�������"); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		RESULT result={0};
		ListGetItem(group.subItems,&item,i);
		result.index=item.itemId;
		unsigned int input = HexStrToUnsignedInt(item.inputValue_);
		APPEND_INFO_FORMAT(masgHandle,"%s ����",item.itemName_); 
		tBIBO bibo={0};
		if(FALSE==getBibo(item.itemName_,&bibo))
		{
			APPEND_INFO_FORMAT(masgHandle,"%s �޴�����",item.itemName_);
			continue;
		}
		char setVal[20]={0}; 
		sprintf(setVal,"%d",bibo.maskBo);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(masgHandle,"����BOʧ��");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(masgHandle,"����BO�ɹ���%x",bibo.maskBo);
			
		}
		
		char getVal[20]={0};
		float getValF=0;
		float standardF=0;
		
		int retryCnt = 5;
		while(retryCnt-- >=0 )
		{
			if(strcmp(item.itemName_,"��Դģ��1")==0 || strcmp(item.itemName_,"��Դģ��2")==0 || strcmp(item.itemName_,"��Դģ��3")==0 || strcmp(item.itemName_,"��Դģ��6_1")==0)
			{
				if(FALSE == ParamGetDependWithRetry(eut,"ǹ1����ѹ",getVal,3))
				{
					APPEND_INFO(masgHandle,"��ȡǹ1��ѹʧ��"); 
					goto DONE;		
				}
			}else{
				if(FALSE == ParamGetDependWithRetry(eut,"ǹ2����ѹ",getVal,3))
				{
					APPEND_INFO(masgHandle,"��ȡǹ2��ѹʧ��");  
					goto DONE;		
				}				
			}
			
			getValF= atof(getVal);
		
			standardF=atof(item.standard_);
		
		
			
		
			if(getValF>standardF-5 && getValF<standardF+5)
			{
				result.pass=RESULT_PASS;
				//break;
			}else{
				result.pass=RESULT_FAIL;
			}
			
			APPEND_INFO_FORMAT(masgHandle,"��ѹֵΪ:%f,���Խ��Ϊ:%d",getValF,result.pass);
			if(result.pass == RESULT_PASS)
				break;
			
			if(FALSE==AlertDialogWithRetAutoClose(0,"waring","���ȷ�������ȴ�������ȡ���˳����β���","ȡ��","����",10))
			{
				break;
			}
			
		}
		
		//�Ͽ����м�����
		memset(setVal,0,20);
		sprintf(setVal,"%d",bibo.maskBi);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(masgHandle,"����BOʧ��");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(masgHandle,"����BO�ɹ���%x",bibo.maskBi);
		}		
		saveResult(hashTable,&result);				
	}
DONE:
	if(FALSE==ParamSetDependWithRetry(eut,"BO","1",3))
	{
		APPEND_INFO(masgHandle,"����BOʧ��");
		WarnShow1(0,"�밴�¼�ͣ");
	}else{
		APPEND_INFO_FORMAT(masgHandle,"����BO�ɹ���%s","1");
	}
	return ret;
}
#else
METHODRET PowerModuleTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"�������"); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	if(FALSE==ParamSetDependWithRetry(eut,"DO��һ���Ʊ�־","0",3))
	{
		APPEND_INFO(masgHandle,"DO��һ���Ʊ�־ ʧ��");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(masgHandle,"DO��һ���Ʊ�־ �ɹ�"); 
	}
	
	
	//�պϽ����Ӵ���
	if(FALSE==ParamSetDependWithRetry(eut,"KM����","1",3))
	{
		APPEND_INFO(masgHandle,"KM���� ʧ��");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(masgHandle,"KM���� �ɹ�"); 
	}
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		RESULT result={0};
		ListGetItem(group.subItems,&item,i);
		result.index=item.itemId;
		unsigned int input = HexStrToUnsignedInt(item.inputValue_);
		APPEND_INFO_FORMAT(masgHandle,"%s ����",item.itemName_); 
		tBIBO bibo={0};
		if(FALSE==getBibo(item.itemName_,&bibo))
		{
			APPEND_INFO_FORMAT(masgHandle,"%s �޴�����",item.itemName_);
			continue;
		}
		char setVal[20]={0}; 
		sprintf(setVal,"%d",bibo.maskBo);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(masgHandle,"����BOʧ��");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(masgHandle,"����BO�ɹ���%x",bibo.maskBo);
			
		}
		
		char getVal[20]={0};
		float getValF=0;
		float standardF=0;
		
		int retryCnt = 5;
		while(retryCnt-- >=0 )
		{
			if(strcmp(item.itemName_,"��Դģ��1")==0 || strcmp(item.itemName_,"��Դģ��2")==0 || strcmp(item.itemName_,"��Դģ��3")==0 || strcmp(item.itemName_,"��Դģ��6_1")==0)
			{
				if(FALSE == ParamGetDependWithRetry(eut,"ǹ1����ѹ",getVal,3))
				{
					APPEND_INFO(masgHandle,"��ȡǹ1��ѹʧ��"); 
					goto DONE;		
				}
			}else{
				if(FALSE == ParamGetDependWithRetry(eut,"ǹ2����ѹ",getVal,3))
				{
					APPEND_INFO(masgHandle,"��ȡǹ2��ѹʧ��");  
					goto DONE;		
				}				
			}
			
			getValF= atof(getVal);
		
			standardF=atof(item.standard_);
		
		
						
		
			if(getValF>standardF-5 && getValF<standardF+5)
			{
				result.pass=RESULT_PASS;
				//break;
			}else{
				result.pass=RESULT_FAIL;
			}
			
			memset(result.recvString,0,RESULT_RECEIVE_LEN);
			sprintf(result.recvString,"%f",getValF);
			
			APPEND_INFO_FORMAT(masgHandle,"��ѹֵΪ:%f,���Խ��Ϊ:%d",getValF,result.pass);
			if(result.pass == RESULT_PASS)
				break;
			
			if(FALSE==AlertDialogWithRetAutoClose(0,"waring","���ȷ�������ȴ�������ȡ���˳����β���","ȡ��","����",10))
			{
				break;
			}
			
		}
		
		//�Ͽ����м�����
		memset(setVal,0,20);
		sprintf(setVal,"%d",bibo.maskBi);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(masgHandle,"����BOʧ��");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(masgHandle,"����BO�ɹ���%x",bibo.maskBi);
		}		
		saveResult(hashTable,&result);				
	}
DONE:
	if(FALSE==ParamSetDependWithRetry(eut,"KM����","0",3))
	{
		APPEND_INFO(masgHandle,"KM���� ʧ��");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(masgHandle,"KM���� �ɹ�"); 
	}
	return ret;
}
#endif


TPS registerPowerModuleTestTps(void)
{
	TPS tps=newTps("PowerModule");
	tps.testFunction=PowerModuleTest;
	//tps.protocolInit=BiboProtocolInit;
	return tps;			
}

