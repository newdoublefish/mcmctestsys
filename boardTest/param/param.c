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
		sprintf(gunVoltage,"%s","枪1插枪链接电压");
	}else if(gunIndex==2) {
		sprintf(gunVoltage,"%s","枪2插枪链接电压"); 
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
			WarnShow1(0,"无该参数配置");
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
			WarnShow1(0,"设置错误!");
		}
	}
																		 
	//showParamSetPanel(paramsToFetch); 
	
	
	//设置参数
	//TODO:读回参数并对比
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
	APPEND_INFO(masgHandle,"进入测试"); 
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}
	//发送自检命令
	if(startCommand(servicePtr,"product self_test\r\n")<0)
		goto DONE;  */
	
	if(strcmp("硬件自检",group.groupName)==0)
	{
		if(FALSE==CommandSend(eut,"product self_test\r\n"))
		{
			APPEND_INFO(masgHandle,"自检命令发送失败");
			return ret;				
		}else{
			APPEND_INFO(masgHandle,"自检命令发送成功");
		}
	}
	

	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;		
		itemResult.pass=RESULT_PASS;
		
		if(strcmp(item.itemName_,"门节点")==0 || strcmp(item.itemName_,"防雷器")==0 || strcmp(item.itemName_,"急停按钮")==0)
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
					APPEND_INFO(masgHandle,"获取结果失败");				
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
		
		if(strcmp(item.itemName_,"铭牌编号")==0)
		{
		
			if(FALSE==ParamGetDependWithRetry(eut,"装置编号",itemResult.recvString,3))
			{	
				APPEND_INFO_FORMAT(masgHandle,"%s:获取失败",item.itemName_); 
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandle,"%s,%s:获取成功",item.itemName_,itemResult.recvString); 
			}
			char minPai[20]={0};
			memcpy(minPai,itemResult.recvString+10,7);
			memset(itemResult.recvString,0,RESULT_RECEIVE_LEN); 
			sprintf(itemResult.recvString,"%s%s",item.inputValue_,minPai);
			APPEND_INFO_FORMAT(masgHandle,"铭牌编号位:%s",itemResult.recvString);
			saveResult(hashTable,&itemResult);  
			continue;
		}
		
		if(FALSE==ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3))
		{	
			APPEND_INFO_FORMAT(masgHandle,"%s:获取失败",item.itemName_); 
			goto DONE;
		}else{
			APPEND_INFO_FORMAT(masgHandle,"%s,%s:获取成功",item.itemName_,itemResult.recvString); 
		}
		itemResult.pass=RESULT_PASS;
		

		if(strcmp(group.groupName,"软件版本检查")==0){
			
			if(strcmp(item.standard_,"NA")!=0)
			{
				if(strcmp(item.standard_,itemResult.recvString)==0)
				{
					itemResult.pass=RESULT_PASS;
				}else{
					itemResult.pass=RESULT_FAIL;
				}
			}			
		}else if(strcmp(group.groupName,"电表时间")==0)
		{
			char sysTime[30]={0};
			getSysTime(sysTime);
			int compareResult=0;
			int tolerance=atoi(item.standard_);
			APPEND_INFO_FORMAT(masgHandle,"%s：时间为:%s",item.itemName_,itemResult.recvString);
			APPEND_INFO_FORMAT(masgHandle,"当前系统时间:%s",sysTime);
			if(FALSE==compareTimeWithTolerance(sysTime,itemResult.recvString,tolerance,&compareResult))
			{
				APPEND_INFO(masgHandle,"时间格式错误！");
				itemResult.pass=RESULT_FAIL;
			}else{
				if(compareResult==0)
				{
					APPEND_INFO(masgHandle,"时间在误差范围内");
					itemResult.pass=RESULT_PASS;
				}else{
					APPEND_INFO(masgHandle,"时间不在误差范围内"); 
					itemResult.pass=RESULT_FAIL;
				}
			}									
		}else if(strstr(item.itemName_,"插枪链接电压")!=NULL){
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
	APPEND_INFO(masgHandle,"退出测试");
	return ret;
}



TPS registerParamsCheckTps(void)
{
	TPS tps=newTps("paramCheck");
	tps.testFunction=ParamCheckTest;
	tps.protocolInit=ParamProtocolInit;
	return tps;			
}

METHODRET ParamSetTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	APPEND_INFO(masgHandle,"进入测试"); 
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT result={0};
		result.index = item.itemId;
		//char cmd[10]={0};
		//sprintf(cmd,"%d",(int)(atof(item.inputValue_)));
		if(FALSE==ParamSetDependWithRetry(eut,item.itemName_,item.inputValue_,3))
		{
			goto DONE;
		}else{
			APPEND_INFO_FORMAT(masgHandle,"%s 设置成功，值为%s",item.itemName_,item.inputValue_);
		}		
		if(FALSE==ParamGetDependWithRetry(eut,item.itemName_,result.recvString,3))
		{	
			APPEND_INFO_FORMAT(masgHandle,"%s:获取失败",item.itemName_); 
			goto DONE;
		}else{
			APPEND_INFO_FORMAT(masgHandle,"%s,%s:获取成功",item.itemName_,result.recvString); 
		}
		
		if(strstr(result.recvString,item.standard_)!=NULL)
		{
			result.pass = RESULT_PASS;
		}
		
		saveResult(hashTable,&result);		
	}

DONE:	
	//onStubDisConnected(servicePtr);
	APPEND_INFO(masgHandle,"退出测试");
	return ret;
}	

TPS registerParamsSetTps(void)
{
	TPS tps=newTps("paramSet");
	tps.testFunction=ParamSetTest;
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
		APPEND_INFO(msgPanel,"网络连接失败,退出测试");
		return 	TEST_RESULT_ERROR;
	}
	APPEND_INFO(msgPanel,"网络连接成功");
/*	ListType paramsToFetch=ListCreate(sizeof(PARAMETER));

	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		PARAMETER param={0};
		if(FALSE==getParameter(item.itemName_,&param))
		{
			WarnShow1(0,"无该参数配置");
		
			goto DONE;
		}else{
		
		}
		ListInsertItem(paramsToFetch,&param,END_OF_LIST);
	}*/
	//TODO:读回参数
	//showParamSetPanel(paramsToFetch);
	//设置参数

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
			WarnShow1(0,"获取失败");
			goto DONE;
		}*/
		
		if(FALSE==ParamGetDepend(eut,item.itemName_,itemResult.recvString))
		{
			APPEND_INFO_FORMAT(msgPanel,"%s:获取失败",msgPanel);		
		}else{
			APPEND_INFO_FORMAT(msgPanel,"%s:获取成功，值为%s",msgPanel,itemResult.recvString);			
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
	APPEND_INFO(masgHandle,"进入测试");
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		APPEND_INFO(masgHandle,"网络连接失败,退出测试");
		return 	TEST_RESULT_ERROR;
	}	
	METHODRET ret = TEST_RESULT_ALLPASS;
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	char tips60A[20]={0};
	char tips100A[20]={0};
	char tips120A[30]={0};
	char voltage[20]={0};
	if(strcmp(group.groupName,"枪1功率分配")==0)
	{
		sprintf(startChargeCmd,"%s","1枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","1枪调试停止充电");
		sprintf(tips60A,"%s","电源模块1，2启动");
		sprintf(tips100A,"%s","电源模块1，2，3启动");
		sprintf(tips120A,"%s","电源模块1，2，3，6 启动");
	}else if(strcmp(group.groupName,"枪2功率分配")==0){
		sprintf(startChargeCmd,"%s","2枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","2枪调试停止充电");
		sprintf(tips60A,"%s","电源模块4，5启动");
		sprintf(tips100A,"%s","电源模块4，5，6启动");
		sprintf(tips120A,"%s","电源模块3，4，5，6 启动");	
	}
	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		APPEND_INFO(masgHandle,"发送启动充电命令失败"); 
		WarnShow1(0,"发送启动充电命令失败！");
		return ret;
	}else{
		APPEND_INFO(masgHandle,"已成功发送启动充电命令");
	}
	
	if(FALSE==AlertDialogWithRet(0,"waring","已启动充电流程，并且电压已经稳定","否","是"))
	{
			//getStubNetService(ip,port);
		return ret;
	}	
	
	//60A需求
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
				APPEND_INFO(masgHandle,"BMS 需求电流设置失败"); 
				WarnShow1(0,"请手动设置电流为60A");				
			}
			APPEND_INFO(masgHandle,"BMS 需求电流已经成功设置为60A");
			if(FALSE==AlertDialogWithRet(0,"waring",tips60A,"错误","正确"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}			
			
#else
			WarnShow1(0,"设置电流为60A"); //后续可以用485控制来操作
			APPEND_INFO(masgHandle,"设置电流为60A");
			if(FALSE==AlertDialogWithRet(0,"waring",tips60A,"错误","正确"))
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
				APPEND_INFO(masgHandle,"BMS 需求电流设置失败"); 
				WarnShow1(0,"请手动设置电流为100A");				
			}
			APPEND_INFO(masgHandle,"BMS 需求电流已经成功设置为100A");
			if(FALSE==AlertDialogWithRet(0,"waring",tips100A,"错误","正确"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}			
			
#else			
			WarnShow1(0,"设置电流为100A"); //后续可以用485控制来操作
			APPEND_INFO(masgHandle,"设置电流为100A");
			if(FALSE==AlertDialogWithRet(0,"waring",tips100A,"错误","正确"))
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
				APPEND_INFO(masgHandle,"BMS 需求电流设置失败"); 
				WarnShow1(0,"请手动设置电流为120A");				
			}
			APPEND_INFO(masgHandle,"BMS 需求电流已经成功设置为120A");
			
			if(FALSE==AlertDialogWithRet(0,"waring",tips120A,"错误","正确"))
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
			APPEND_INFO_FORMAT(masgHandle,"当前需求电流为%d",itemGet.value/10);							
			if(itemGet.value!=itemSet.value)
			{

			}else{
				APPEND_INFO(masgHandle,"BMS 需求电流已经成功设置为120A");
			}*/  
#else	
			WarnShow1(0,"设置电流为120A"); //后续可以用485控制来操作
			APPEND_INFO(masgHandle,"设置电流为120A"); 
			if(FALSE==AlertDialogWithRet(0,"waring",tips120A,"错误","正确"))
			{
			//getStubNetService(ip,port);
				goto ERROR;
			}
#endif			
		}
		//WarnShow1(0,"听到继电器跳后,点确定开始读书");
		
		//WarnAlert(0,"请稍后,等待继电器闭合",20);
 	
		//char BI[10]={0};

		
		int reTryCnt=0;
		while(reTryCnt++<5)
		{
			memset(itemResult1.recvString,0,RESULT_RECEIVE_LEN);
			if(FALSE==ParamGetDepend(eut,"BI",itemResult1.recvString ))
			{
				APPEND_INFO_FORMAT(masgHandle,"%s 获取到功率分配反馈失败 %s");
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
			
			
			
			APPEND_INFO_FORMAT(masgHandle,"%s 获取到功率分配反馈 %s,结果为%d 获取次数:%d",item1.itemName_,itemResult1.recvString,itemResult1.pass,reTryCnt);
			if(itemResult1.pass==RESULT_PASS)
				break;
			//WarnAlert(0,"请等待",10);
			if(FALSE==AlertDialogWithRetAutoClose(0,"waring","点击确定跳过等待，按下取消退出本次测试","取消","跳过",10))
			{
				break;
			}
		}		
		//sprintf(itemResult1.recvString,"%s",BI);
		saveResult(hashTable,&itemResult1);
	}		
ERROR:	
	//100A需求

	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"无法停止充电,请按急停按钮");
	}
	WarnShow1(0,"请等待并确保充电流程已经停止");
	APPEND_INFO(masgHandle,"退出测试");	
	return ret;
}


TPS registerPowerDistributeTestTps(void)
{
	TPS tps=newTps("PowerDistribute");
	tps.testFunction=PowerDistributeTest;
	//tps.protocolInit=BiboProtocolInit;
	return tps;			
}

//枪二维码 "http://www.gdmcmc.cn/qrcode.html?qrcode=881011000851";
//电表地址 strAddress="1711424989000065";

BOOL checkScanResult(int panel)
{
	char gun1[128]={0},gun2[128]={0},biao1[128]={0},biao2[128]={0};
	GetCtrlVal(panel,SCANPANEL_SCAN1,gun1);
	GetCtrlVal(panel,SCANPANEL_SCAN2,gun2);
	GetCtrlVal(panel,SCANPANEL_SCAN3,biao1); 
	GetCtrlVal(panel,SCANPANEL_SCAN4,biao2);
	if(strcmp(gun1,gun2)==0)
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"错误:枪1枪2地址一致");
		return FALSE;
	}

	
	if((strlen(gun1)!=strlen(gun2)) || (strlen(gun1)!=12))
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"错误:枪1枪2地址设置错误");
		return FALSE;		
	}
	
	if(gun1[11]!='1' || gun2[11]!='2')
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"错误:枪1枪2地址末尾数字错误");
		return FALSE;		
	}
	
	if(strcmp(biao1,biao2)==0)
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"错误:表1表2地址一致");
		return FALSE;
	}	
	
	if((strlen(biao1)!=strlen(biao2)) | (strlen(biao2)!=12)) 
	{
		SetCtrlVal(panel,SCANPANEL_TEXTMSG_2,"错误:表1表2地址设置错误");
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
	APPEND_INFO_FORMAT(masgHandler,"进入%s测试",group.groupName);
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
		
		APPEND_INFO_FORMAT(masgHandler,"----------%s 设置------------------",item.itemName_);
		if(1==i)
		{
			GetCtrlVal(panelHandle,SCANPANEL_SCAN1,itemResult.recvString); 
		}else if(2==i){
			GetCtrlVal(panelHandle,SCANPANEL_SCAN2,itemResult.recvString);
		}else if(3==i)
		{
			/*if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"无该参数配置");
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
			
			if(FALSE==ParamSetDependWithRetry(eut,item.itemName_,setValue,3))
			{
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s 设置成功，值为%s",item.itemName_,setValue);
			}
			APPEND_INFO(masgHandler,"延时2s"); 
			Delay(2);
			if(FALSE == ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3))
			{
				APPEND_INFO_FORMAT(masgHandler,"%s 获取失败",item.itemName_);
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s 获取成功，值为%s",item.itemName_,itemResult.recvString);	
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
				WarnShow1(0,"无该参数配置");
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
			if(FALSE==ParamSetDependWithRetry(eut,item.itemName_,setValue,3))
			{
				APPEND_INFO_FORMAT(masgHandler,"%s 设置失败",item.itemName_); 
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s 设置成功，值为%s",item.itemName_,setValue);
			}
			APPEND_INFO(masgHandler,"延时2s"); 
			Delay(2);
			/*if(FALSE==ParamGet(servicePtr,item.itemName_,itemResult.recvString))
			{
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s 获取成功，值为%s",item.itemName_,itemResult.recvString);	
			}*/
			if(FALSE == ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3))
			{
				APPEND_INFO_FORMAT(masgHandler,"%s 获取失败",item.itemName_);
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s 获取成功，值为%s",item.itemName_,itemResult.recvString);	
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
			if(FALSE==ParamSetDependWithRetry(eut,item.itemName_,setValue,3))
			{
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s 设置成功，值为%s",item.itemName_,setValue);
			}
			APPEND_INFO(masgHandler,"延时2s");
			Delay(2);
			if(FALSE == ParamGetDependWithRetry(eut,item.itemName_,itemResult.recvString,3))
			{
				APPEND_INFO_FORMAT(masgHandler,"%s 获取失败",item.itemName_);
				goto DONE;
			}else{
				APPEND_INFO_FORMAT(masgHandler,"%s 获取成功，值为%s",item.itemName_,itemResult.recvString);	
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
	APPEND_INFO_FORMAT(masgHandler,"离开%s测试",group.groupName); 
	return ret;
#endif	
DONE:	
	DiscardPanel(panelHandle);
#ifndef DEBUG_PARAM_SCAN_TEST	
	onStubDisConnected(servicePtr);
#endif	
	APPEND_INFO_FORMAT(masgHandler,"离开%s测试",group.groupName); 
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
	APPEND_INFO(msgHandle,"进入反接告警测试");
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	int flag1=-1;
	int flag2=-1;
	int flag3=-1;
	int gunIndex=0;
	int plugined=0;
	
	if(strcmp(group.groupName,"枪1电池反接告警")==0)
	{
		gunIndex=1;
	}else if(strcmp(group.groupName,"枪2电池反接告警")==0)
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
		APPEND_INFO(msgHandle,"检测到未插入枪");
		WarnShow1(0,"未插入枪");
		//return TEST_RESULT_ALLPASS;
	}else{
		APPEND_INFO(msgHandle,"检测到枪已经插入");
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
		APPEND_INFO_FORMAT(msgHandle,"%s:获取失败",item.itemName_); 
		goto DONE;
	}else{
		APPEND_INFO_FORMAT(msgHandle,"%s:获取成功:%s",item.itemName_,queryResult);
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
		APPEND_INFO(msgHandle,"继电器操作成功,闭合2，3，4，31继电器");	
	}
	
	//WarnAlert(0,);
	WarnAlert(0,"延时中",8);
	
	memset(queryResult,0,20);
	if(ParamGet(servicePtr,item.itemName_,queryResult)<0)
	{
		APPEND_INFO_FORMAT(msgHandle,"%s:获取失败",item.itemName_);
		goto DONE;
	}else{
		APPEND_INFO_FORMAT(msgHandle,"%s:获取成功:%s",item.itemName_,queryResult); 		
	}

	if(strcmp(queryResult,"true")==0)
	{
		flag2=1;
	}else{
		flag2=0;
	}	
	
	if(AlertDialogWithRet(0,"枪检查","请检查面板显示应充电枪故障指示灯显示应该为X","错误","正确")==FALSE)
	{
		flag2=0;
	}
														 
	APPEND_INFO_FORMAT(msgHandle,"flag2:%d",flag2); 
	

	if(OperatDoSet(eut.relayConfig,RELAY(2),MASK32)==FALSE)
	{
		goto DONE;
	}else{
		APPEND_INFO(msgHandle,"继电器操作成功,闭合2继电器");	
	}
	
	WarnAlert(0,"延时中",8); 
	
	memset(queryResult,0,20);
	if(ParamGet(servicePtr,item.itemName_,queryResult)<0)
	{
		APPEND_INFO_FORMAT(msgHandle,"%s:获取失败",item.itemName_);
		goto DONE;
	}else{
		APPEND_INFO_FORMAT(msgHandle,"%s:获取成功:%s",item.itemName_,queryResult); 		
	}
	
	if(strcmp(queryResult,"false")==0)
	{
		flag3=1;
	}else{
		flag3=0;
	}	
	
	if(AlertDialogWithRet(0,"枪检查","请检查面板显示应充电枪故障指示灯显示应该消失","错误","正确")==FALSE)
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
		 APPEND_INFO(msgHandle,"继电器操作成功,闭合2继电器");
	}
	APPEND_INFO(msgHandle,"退出测试");
	return ret;
DONE:
	if(OperatDoSet(eut.relayConfig,RELAY(2),MASK32)==TRUE)
	{
		 APPEND_INFO(msgHandle,"继电器操作成功,闭合2继电器");
	}	
	onStubDisConnected(servicePtr);
	APPEND_INFO(msgHandle,"退出测试"); 
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
	APPEND_INFO_FORMAT(msgHander,"进入：%s",group.groupName);
	BOOL flag=TRUE; 
	METHODRET ret = TEST_RESULT_ALLPASS;
	TestItem item;
	ListGetItem(group.subItems,&item,1);
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	char isolateCMD[20]={0};
	if(strcmp(item.itemName_,"枪1绝缘检测")==0)
	{
		sprintf(startChargeCmd,"%s","1枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","1枪调试停止充电");
		sprintf(isolateCMD,"%s","1枪绝缘检测结果");
	}else if(strcmp(item.itemName_,"枪2绝缘检测")==0){
		sprintf(startChargeCmd,"%s","2枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","2枪调试停止充电");
		sprintf(isolateCMD,"%s","2枪绝缘检测结果");
	}

	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(30)|RELAY(2),MASK32))
	{
		APPEND_INFO(msgHander,"继电器操作失败！！");
		goto DONE;
	}else{
	    APPEND_INFO(msgHander,"继电器操作成功，闭合2，30继电器！！");
	}

	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"无法启动充电！");
		APPEND_INFO(msgHander,"无法发送启动充电！"); 
		flag=FALSE; 
		goto DONE;
	}else{
		APPEND_INFO(msgHander,"已成功发送启动命令！");
	}
	
	if(AlertDialogWithRet(0,"枪检查","请确认充电功能正常","错误","正确")==FALSE)
	{
		WarnShow1(0,"充电功能不正常！"); 
		flag=FALSE;
		goto DONE;
	}
	APPEND_INFO(msgHander,"充电功能正常！");
	//读取结果
	char inSoResult[10]={0};
	if(FALSE==ParamGetDepend(eut,isolateCMD,inSoResult))
	{
		WarnShow1(0,"获取绝缘检测结果失败"); 
	}
	
	//printf("%s\n",inSoResult);
	

	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		WarnShow1(0,"无法停止充电！请按急停停止"); 
		APPEND_INFO(msgHander,"发送充电停止命令失败"); 
		goto DONE;
	}else{
		APPEND_INFO(msgHander,"发送充电停止命令成功"); 
	}	
	
	//WarnAlert(0,"延时中",30);
	WarnShow1(0,"请确认充电已经停止，充电停止后进入下一步测试");
	
	WarnShow1(0,"下一步测试");
	
	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(29) | RELAY(30)| RELAY(2),MASK32))
	{
		APPEND_INFO(msgHander,"继电器操作失败！！");
		goto DONE;
	}else{
	    APPEND_INFO(msgHander,"继电器操作成功，闭合2，29,30继电器！！");
	}
	
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"无法启动充电！");
		APPEND_INFO(msgHander,"无法发送启动充电！"); 
		flag=FALSE; 
		goto DONE;
	}else{
		APPEND_INFO(msgHander,"已成功发送启动命令！");
	}	

	if(AlertDialogWithRet(0,"枪检查","请确认充电功能异常,应该自动停止充电","错误","正确")==FALSE)
	{
		flag=FALSE;
	}
	
	char inSoResult1[10]={0};
	if(FALSE==ParamGetDepend(eut,isolateCMD,inSoResult1))
	{
		WarnShow1(0,"获取绝缘检测结果失败"); 
	}
	
	//printf("%s\n",inSoResult1);	
	
	if(FALSE==ParamSetDepend(eut,stopChargeCMD,"1"))
	{
		APPEND_INFO(msgHander,"发送充电停止命令失败");
		WarnShow1(0,"无法停止充电！请按急停停止"); 
		goto DONE;
	}else{
		APPEND_INFO(msgHander,"发送充电停止命令成功"); 
	}	
DONE:	
	if(FALSE==OperatDoSet(eut.relayConfig,RELAY(2),MASK32))
	{
	}
	
	RESULT itemResult={0};
	itemResult.index=item.itemId;
	itemResult.pass=flag;
	saveResult(hashTable,&itemResult);
	WarnShow1(0,"请确保充电流程已经停止");
	APPEND_INFO(msgHander,"退出测试");
	return ret;	
}

METHODRET InsulationTest2(TestGroup group,EUT eut,HashTableType hashTable,int msgHander)
{
	APPEND_INFO_FORMAT(msgHander,"进入：%s",group.groupName);
	METHODRET ret = TEST_RESULT_ALLPASS;
	char insulationResultCmd[20]={0};
	char insulationResultPos[20]={0};
	char insulationResultNag[20]={0};
	char ammeter[20]={0};
	

	
	if(strcmp(group.groupName,"枪1绝缘检测功能")==0)
	{
		sprintf(insulationResultCmd,"%s","1枪绝缘检测结果");
		sprintf(insulationResultPos,"%s","1枪绝缘电阻正");
		sprintf(insulationResultNag,"%s","1枪绝缘电阻负");
		sprintf(ammeter,"%s","枪1电表电压");
	
	}else if(strcmp(group.groupName,"枪2绝缘检测功能")==0) {
	
		sprintf(insulationResultCmd,"%s","2枪绝缘检测结果");
		sprintf(insulationResultPos,"%s","2枪绝缘电阻正");
		sprintf(insulationResultNag,"%s","2枪绝缘电阻负");
		sprintf(ammeter,"%s","枪2电表电压"); 
	
	}else{
	
		return ret;
	}
	if(FALSE==ParamSetDependWithRetry(eut,"DO单一控制标志","0",3))
	{
		APPEND_INFO(msgHander,"DO单一控制标志 失败");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(msgHander,"DO单一控制标志 成功"); 
	}
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,i);
		RESULT result={0};
		result.index=item.itemId;
		
		APPEND_INFO_FORMAT(msgHander,"%s 测试",item.itemName_); 
		tBIBO bibo={0};
		if(FALSE==getBibo(item.itemName_,&bibo))
		{
			APPEND_INFO_FORMAT(msgHander,"%s 无此配置",item.itemName_);
			continue;
		}
		char setVal[20]={0}; 
		sprintf(setVal,"%d",bibo.maskBo);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(msgHander,"操作BO失败");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(msgHander,"操作BO成功：%x",bibo.maskBo);
			
		}	
		int retryCnt =40;
		result.pass = RESULT_FAIL;
		double outTime = Timer();
		while(result.pass == RESULT_FAIL)
		{
			//memset(result.recvString,0,RESULT_RECEIVE_LEN);
			double currentTime = Timer();	
			if(currentTime-outTime > 30)
			{
				break;
			}
			char voltage[20]={0};
			if(FALSE == ParamGetDependWithRetry(eut,ammeter,voltage,3))
			{
				APPEND_INFO_FORMAT(msgHander,"获取%s失败",ammeter); 
				goto DONE;		
			}
			APPEND_INFO_FORMAT(msgHander,"%s ：%s",ammeter,voltage);
			char getVal[20]={0};
			if(FALSE == ParamGetDependWithRetry(eut,insulationResultCmd,getVal,3))
			{
				APPEND_INFO_FORMAT(msgHander,"获取%s失败",insulationResultCmd); 
				goto DONE;		
			}
			APPEND_INFO_FORMAT(msgHander,"获取%s成功,值为:%s",insulationResultCmd,getVal); 
			if(atoi(getVal) == 0)
			{
				result.pass = RESULT_PASS;
				char valz[20]={0},valf[20]={0};
				if(FALSE == ParamGetDependWithRetry(eut,insulationResultPos,valz,3))
				{
					APPEND_INFO_FORMAT(msgHander,"获取%s失败",insulationResultPos); 
					goto DONE;		
				}
				APPEND_INFO_FORMAT(msgHander,"获取%s成功,值为:%s",insulationResultPos,valz); 	
				if(FALSE == ParamGetDependWithRetry(eut,insulationResultNag,valf,3))
				{
					APPEND_INFO_FORMAT(msgHander,"获取%s失败",insulationResultNag); 
					goto DONE;		
				}
				APPEND_INFO_FORMAT(msgHander,"获取%s成功,值为:%s",insulationResultNag,valf);
				sprintf(result.recvString,"%s,%s",valz,valf);
				break;
			}
			
		}
		saveResult(hashTable,&result);
		memset(setVal,0,20);
		sprintf(setVal,"%d",bibo.maskBi);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(msgHander,"操作BO失败");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(msgHander,"操作BO成功：%x",bibo.maskBi);
			
		}		
	}
DONE:
	if(FALSE==ParamSetDependWithRetry(eut,"BO","0",3))
	{
		APPEND_INFO(msgHander,"操作BO失败");
	}	
	APPEND_INFO(msgHander,"退出测试");
	return ret;		
}	


TPS registerInsulationTestTestTps(void)
{
	TPS tps=newTps("insulation");
	//tps.testFunction=InsulationTest;
	tps.testFunction=InsulationTest;
	return tps;	
}

METHODRET ChargingTest(TestGroup group,EUT eut,HashTableType hashTable,int msgHandler)
{
	APPEND_INFO_FORMAT(msgHandler,"进入%s",group.groupName);
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
	//充电模块确认
	TestItem item1;
	ListGetItem(group.subItems,&item1,1);
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	if(strcmp(group.groupName,"枪1充电功能")==0)
	{
		sprintf(startChargeCmd,"%s","1枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","1枪调试停止充电");
	}else if(strcmp(group.groupName,"枪2充电功能")==0){
		sprintf(startChargeCmd,"%s","2枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","2枪调试停止充电");	
	}	
	
	RESULT itemResult1={0};
	itemResult1.index=item1.itemId;
	itemResult1.pass=RESULT_PASS;
	
	/*if(ParamSet(servicePtr,"1枪调试启动充电","1")==FALSE)
	{
		goto DONE;
	}
	onStubDisConnected(servicePtr);*/
	if(FALSE==ParamSetDepend(eut,startChargeCmd,"1"))
	{
		WarnShow1(0,"无法启动充电");
		APPEND_INFO(msgHandler,"无法启动充电");
		APPEND_INFO_FORMAT(msgHandler,"离开%s",group.groupName);
		return TEST_RESULT_ALLPASS;
	}
	
	if(FALSE==AlertDialogWithRet(0,"waring","已启动充电流程，并且电压已经稳定","否","是"))
	{
			//getStubNetService(ip,port);
		//APPEND_INFO(msgHandler,"无法启动充电");
		APPEND_INFO_FORMAT(msgHandler,"离开%s",group.groupName);			
		return TEST_RESULT_ALLPASS;
	}
	APPEND_INFO(msgHandler,"已启动充电流程，并且电压已经稳定");
	
	if(AlertDialogWithRet(0,"枪检查","请确认充电流程已启动,点击确认加负载","错误","正确")==FALSE)
	{
		flag=FALSE;
		itemResult1.pass=RESULT_FAIL;
		saveResult(hashTable,&itemResult1);
		//return TEST_RESULT_ALLPASS;
		ParamSetDepend(eut,stopChargeCMD,"1");	
		return TEST_RESULT_ALLPASS;
	}
	saveResult(hashTable,&itemResult1);
	
	//加负载
	if(OpenDo(eut.relayConfig,31)==FALSE)
	{
		return 	TEST_RESULT_ERROR;
	}else{
		APPEND_INFO(msgHandler,"闭合继电器31，负载加载成功");
	}	
	
	/*tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		return 	TEST_RESULT_ERROR;
	}*/

	WarnShow1(0,"按下确定开始读数");
	APPEND_INFO(msgHandler,"开始读数"); 

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
			WarnShow1(0,"无该参数配置");
			goto DONE;
		}
		
		if(GetParameter(servicePtr,&param)<0)
		{	
			WarnShow1(0,"获取失败");
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
			 APPEND_INFO_FORMAT(msgHandler,"%s获取失败功",item.itemName_);
			 itemResult.pass=RESULT_FAIL;
			 saveResult(hashTable,&itemResult);
			 goto DONE;
		}else{
			APPEND_INFO_FORMAT(msgHandler,"%s获取成功，值为%s",item.itemName_,itemResult.recvString); 	
		}
		float strValue=atof(itemResult.recvString);
		if(i==3)
		{
			if(strstr(item.inputValue_,"NA")==NULL && strstr(item.inputValue_,"NA")==NULL)
			{
				float tolenrent = atof(item.inputValue_);
				float standard = atof(item.standard_);
				if(strValue<=(standard+tolenrent) && strValue>=(standard-tolenrent))
				{
					itemResult.pass=RESULT_PASS;
				}
			}else{
				if(strValue<353 && strValue>347)
				{
					itemResult.pass=RESULT_PASS;
				}
			}
		}else if(i==2)
		{
			if(strstr(item.inputValue_,"NA")==NULL && strstr(item.standard_,"NA")==NULL)
			{
				float tolenrent = atof(item.inputValue_);
				float standard = atof(item.standard_);
				if(strValue<=(standard+tolenrent) && strValue>=(standard-tolenrent))
				{
					itemResult.pass=RESULT_PASS;
				}
			}else{
				if(strValue<2.87 && strValue>2.67)
				{
					itemResult.pass=RESULT_PASS;
				}
			}
		}else{
			itemResult.pass=RESULT_PASS; 
		}
		
		APPEND_INFO_FORMAT(msgHandler,"%s 测试结果为 %d",item.itemName_,itemResult.pass);
		
		saveResult(hashTable,&itemResult);			
	}
		
	//ListType paramsToSet=ListCopy(paramsToGet);
	showParamSetPanel(paramsToSet);
	APPEND_INFO(msgHandler,"开始读数面板数值");	
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
			if(strstr(item.inputValue_,"NA")==NULL && strstr(item.standard_,"NA")==NULL)
			{
				int tolenrent = atof(item.inputValue_);
				int standard = atof(item.standard_);
				if(strValue<=(standard+tolenrent) && strValue>=(standard-tolenrent))
				{
					itemResult.pass=RESULT_PASS;
				}
			}else{
				if(strValue<353 && strValue>347)
				{
					itemResult.pass=RESULT_PASS;
				}
			}
		}else if(i==1)
		{
			if(strstr(item.inputValue_,"NA")==NULL && strstr(item.standard_,"NA")==NULL)
			{
				float tolenrent = atof(item.inputValue_);
				float standard = atof(item.standard_);
				if(strValue<=(standard+tolenrent) && strValue>=(standard-tolenrent))
				{
					itemResult.pass=RESULT_PASS;
				}
			}else{
				if(strValue<2.87 && strValue>2.67)
				{
					itemResult.pass=RESULT_PASS;
				}
			}
		}
		APPEND_INFO_FORMAT(msgHandler,"%s 值为 %s，测试结果为 %d",item.itemName_,itemResult.recvString,itemResult.pass);
		saveResult(hashTable,&itemResult);			
	}
	
	WarnShow1(0,"手动解锁测试,请按下手动解锁按钮！！");
	
	if(AlertDialogWithRet(0,"手动解锁测试","手动解锁能是否正常","否","正常")==TRUE)
	{
		itemResult1.pass = RESULT_PASS;	
		saveResult(hashTable,&itemResult1); 
	}else{
		itemResult1.pass = RESULT_FAIL;	
		saveResult(hashTable,&itemResult1); 		
	}
	
	
	
DONE:
	ListDispose(paramsToSet);
	//ListDispose(paramsToGet);
	
	/*if(ParamSetDepend(eut,stopChargeCMD,"1")==FALSE)
	{
		WarnShow1(0,"不能停止充电,请按下急停按钮");		
	}else{
		APPEND_INFO(msgHandler,"已经停止充电流程");
	}*/
	
	//WarnShow1(0,"请按下急停按钮")
	
	WarnShow1(0,"请确保已经停止充电,如果没有请按急停按钮");
	
	
	if(CloseDo(eut.relayConfig,31)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}else{
		APPEND_INFO(msgHandler,"断开继电器31成功");
	}
	APPEND_INFO(msgHandler,"离开测试");
	return ret;
}

TPS registerChargingTestTestTps(void)
{
	TPS tps=newTps("charging");
	tps.testFunction=ChargingTest;
	return tps;	
}


static void BuildSetTime(char *timeBuffer)
{
	unsigned int year, month, day, hour, min, sec, weekDay;
	CVIAbsoluteTime absTime;
	GetCurrentCVIAbsoluteTime(&absTime);
    CVIAbsoluteTimeToLocalCalendar(absTime, &year, &month, &day, &hour, 
                &min, &sec, 0, &weekDay);
	//sprintf(timeBuffer,"%02d-%02d-%02d %02d:%02d:%02d",year-2000,month,day,hour,min,sec);
	sprintf(timeBuffer,"%02d-%02d-%02d-%02d:%02d:%02d:0000",year-2000,month,day,hour,min,sec);

}


METHODRET TimeSetTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"进入测试"); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	RESULT itemResult={0};
	TestItem item;
	ListGetItem(group.subItems,&item,1);
	itemResult.index = item.itemId;
	if(strcmp("NA",item.inputValue_)!=0){
		sprintf(itemResult.recvString,"%s",item.inputValue_);			
	}else{
		BuildSetTime(itemResult.recvString);
	}
	
	if(FALSE==ParamSetDependWithRetry(eut,item.itemName_,itemResult.recvString,3))
	{
		itemResult.pass =RESULT_FAIL; 
		APPEND_INFO_FORMAT(masgHandle,"%s:设置失败",item.itemName_); 
		return ret;
	}else{
		itemResult.pass =RESULT_PASS; 
		APPEND_INFO_FORMAT(masgHandle,"%s:设置成功:%s",item.itemName_,itemResult.recvString); 		
	}

	saveResult(hashTable,&itemResult);
	APPEND_INFO(masgHandle,"主动断开网络"); 
	ReleaseStubNetService();
	if(CloseDo(eut.relayConfig,1)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	WarnAlert(0,"重启中",3);

	
	if(OpenDo(eut.relayConfig,1)==FALSE)
	{
		return TEST_RESULT_ERROR;
	}
	
	
	
	WarnAlert(0,"请确认设备已经器动",30);
	TestItem item2;
	RESULT itemResult2={0};
	ListGetItem(group.subItems,&item2,2);	
	itemResult2.index = item2.itemId;  
	if(FALSE==ParamGetDependWithRetry(eut,item.itemName_,itemResult2.recvString,3))
	{
		itemResult2.pass =RESULT_FAIL; 
		APPEND_INFO_FORMAT(masgHandle,"%s:获取失败",item.itemName_); 
		return ret;
	}else{
		itemResult2.pass =RESULT_PASS; 
		APPEND_INFO_FORMAT(masgHandle,"%s:获取成功:%s",item.itemName_,itemResult2.recvString); 		
	}
	int compareResult=0;
	int tolerance = atoi(item2.standard_);
	APPEND_INFO_FORMAT(masgHandle,"时间误差范围为:%d",tolerance);
	char sysTime[30]={0};
	getSysTime(sysTime);
	APPEND_INFO_FORMAT(masgHandle,"当前系统时间:%s",sysTime);
	if(FALSE==compareTimeWithTolerance(sysTime,itemResult2.recvString,tolerance,&compareResult))
	{
		APPEND_INFO(masgHandle,"时间格式错误！");
		itemResult2.pass=RESULT_FAIL;
	}else{
		if(compareResult==0)
		{
			APPEND_INFO(masgHandle,"时间在误差范围内");
			itemResult2.pass=RESULT_PASS;
		}else{
			APPEND_INFO(masgHandle,"时间不在误差范围内"); 
			itemResult2.pass=RESULT_FAIL;
		}
	}
	itemResult2.index = item2.itemId;
	saveResult(hashTable,&itemResult2); 
	APPEND_INFO(masgHandle,"退出测试");	
	return ret;
}

TPS registerTimeSetTestTps(void)
{
	TPS tps=newTps("timeSet");
	tps.testFunction=TimeSetTest;
	return tps;	
}

METHODRET StubCmdTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"进入测试");
	METHODRET ret = TEST_RESULT_ALLPASS;
	tNET_SERVICE *servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
	if(servicePtr==NULL)
	{
		//return 	TEST_RESULT_ERROR;
		goto DONE;
	}	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT itemResult;
		itemResult.index=item.itemId;
		itemResult.pass=RESULT_PASS;
		if(strcmp(item.itemName_,"参数校正")==0){
			char voltage[10]={0};
			if(FALSE==ParamGet(servicePtr,"枪1插枪链接电压",voltage))
			{
																												
			}
			
			if(FALSE==ParamGet(servicePtr,"枪2插枪链接电压",voltage))
			{
																												
			}			
			
			if(startCommand(servicePtr,"prd coeff ins\r\n")<0)
			{
				WarnShow1(0,"参数校正失败");
				itemResult.pass=RESULT_FAIL;
			}
		}else if(strcmp(item.itemName_,"看门狗检查")==0){ 
			if(startCommand(servicePtr,"product watch_dog\r\n")<0)
			{
				WarnShow1(0,"看门狗检查");
				itemResult.pass=RESULT_FAIL;
			}
			onStubDisConnected(servicePtr);
			servicePtr=NULL;
			WarnAlert(0,"请稍后！！！",25);
			servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
			if(servicePtr==NULL)
			{
				itemResult.pass=RESULT_FAIL;
				//return 	TEST_RESULT_ERROR;
				goto DONE;
			}			
		}else if(strcmp(item.itemName_,"交流复位")==0){
			PARAMETER param={0};
			if(FALSE==getParameter(item.itemName_,&param))
			{
				WarnShow1(0,"无该参数配置");
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
			WarnAlert(0,"请稍后！！！",30);
			servicePtr = getStubNetService(eut.chargingPile.ip,eut.chargingPile.port);
			if(servicePtr==NULL)
			{
				itemResult.pass=RESULT_FAIL;
				//return 	TEST_RESULT_ERROR;
				goto DONE;
			}			
		}
		memset(itemResult.recvString,0,sizeof(itemResult.recvString));
		sprintf(itemResult.recvString,"%s","OK");
		saveResult(hashTable,&itemResult);
		
	}
DONE:	
	APPEND_INFO(masgHandle,"退出测试"); 
	return ret;
}

TPS registerStubCmdTestTps(void)
{
	TPS tps=newTps("StubCmd");
	tps.testFunction=StubCmdTest;
	//tps.createTpsPanel=NULL;
	return tps;	
}

METHODRET StubIoTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	char startChargeCmd[20]={0};
	char stopChargeCMD[20]={0};
	if(strcmp(group.groupName,"枪1手动解锁功能")==0)
	{
		sprintf(startChargeCmd,"%s","1枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","1枪调试停止充电");
	}else if(strcmp(group.groupName,"枪2手动解锁功能")==0){
		sprintf(startChargeCmd,"%s","2枪调试启动充电");		
		sprintf(stopChargeCMD,"%s","2枪调试停止充电");	
	}
	TestItem item={0};
	ListGetItem(group.subItems,&item,END_OF_LIST);
	RESULT result={0};
	result.index = item.itemId;
	result.pass = RESULT_FAIL;
	
	if(ParamSetDependWithRetry(eut,startChargeCmd,"1",3)==FALSE)
	{
		WarnShow1(0,"无法启动充电");
		goto DONE;
	}
	
	if(AlertDialogWithRet(0,"请选择","请确认能够手动解锁,充电停止","错误","正确")==TRUE)
	{
		result.pass=RESULT_PASS;	
	}else{
		if(ParamSetDepend(eut,stopChargeCMD,"1")==FALSE)
		{
			WarnShow1(0,"无法停止充电流程");
			goto DONE;
		}		
	}
	
	WarnShow1(0,"请确保充电流程结束，如果没有，按下急停");
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
	//检查继电器口
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem testItem={0};
		RESULT testResult={0};
		ListGetItem(group.subItems,&testItem,i);
		testResult.index=testItem.itemId;
		
		if(strcmp(testItem.itemName_,"继电器")==0)
		{
			if(FALSE==OperatDoSet(eut.relayConfig,0,MASK16))
			{
				//APPEND_INFO(msgHander,"继电器操作失败！！");
				WarnShow1(0,"继电器通信失败");
				return TEST_RESULT_ERROR;
			}					
		}else if(strcmp(testItem.itemName_,"安规测试仪")==0)
		{
		
		
		}else if(strcmp(testItem.itemName_,"BMS模拟器")==0)
		{
				tBmsItem item={
					0x120B,
					1,
				};			
				if(BmsGetItem(eut.bmsConfig,&item)==FALSE)
				{
					WarnShow1(0,"BMS模拟器通信失败"); 
					return TEST_RESULT_ERROR; 
				}			
			
		}else if(strcmp(testItem.itemName_,"充电桩网口")==0)
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
#if 1
METHODRET BIBOTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"进入测试");
	METHODRET ret = TEST_RESULT_ALLPASS;
	if(FALSE==ParamSetDependWithRetry(eut,"DO单一控制标志","0",3))
	{
		APPEND_INFO(masgHandle,"DO单一控制标志 失败");
		goto DONE;
	}else{
		APPEND_INFO(masgHandle,"DO单一控制标志 成功"); 
	}
	
	if(strcmp(group.groupName,"断开所有继电器")==0)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,1);
		RESULT result={0};
		result.index = item.itemId;
		result.pass = FALSE;
		
		tBIBO bibo={0};
		char value[30]={0};
		if(FALSE==getBibo(group.groupName,&bibo))
		{
			saveResult(hashTable,&result);
			goto DONE;
		}
		sprintf(value,"%d",bibo.maskBo);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",value,3))
		{
			APPEND_INFO(masgHandle,"设置数据池BO失败");
			saveResult(hashTable,&result);
			goto DONE;
		}
		result.pass = TRUE;
		APPEND_INFO_FORMAT(masgHandle,"设置数据池BO成功:%s",value);
		saveResult(hashTable,&result);
		goto DONE;
	}
	
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
		int retryCnt=5;
		while(retryCnt-->0)
		{
			char value[30]={0};
			memset(result.recvString,0,RESULT_RECEIVE_LEN);
			sprintf(value,"%d",bibo.maskBo);
			if(FALSE==ParamSetDependWithRetry(eut,"BO",value,3))
			{
				APPEND_INFO(masgHandle,"设置数据池BO失败");
				goto DONE;
			}
			APPEND_INFO_FORMAT(masgHandle,"设置数据池BO成功:%s",value);
			if(FALSE==ParamGetDependWithRetry(eut,"BI",result.recvString,3))
			{
				APPEND_INFO(masgHandle,"获取数据池BI失败"); 
				goto DONE;
			}
			 
			unsigned int valueUi= atoi(result.recvString);
			//valueUi = 24;
			//bibo.maskBi = 8;
			unsigned int standard = bibo.maskBi&valueUi;
			APPEND_INFO_FORMAT(masgHandle,"获取数据池BI成功:%x,%x",valueUi,standard);
			if(standard == bibo.maskBi)
			{
				result.pass = RESULT_PASS;
				break;
			} 
		}
		saveResult(hashTable,&result);
	}
	
DONE:
	APPEND_INFO(masgHandle,"退出测试");
	return ret;
}
#else
METHODRET BIBOTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"进入测试");
	METHODRET ret = TEST_RESULT_ALLPASS;
	
	if(FALSE==ParamSetDependWithRetry(eut,"DO单一控制标志","1",3))
	{
		APPEND_INFO(masgHandle,"DO单一控制标志 失败");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(masgHandle,"DO单一控制标志 成功"); 
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
		APPEND_INFO_FORMAT(masgHandle,"测试继电器%s",item.itemName_);
		if(strcmp(item.itemName_,"枪1K1K2")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"1枪K1K2控制",setVal,3))
			{
				APPEND_INFO(masgHandle,"闭合继电器 1枪K1K2 失败");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"闭合继电器 1枪K1K2 成功"); 
			}
		}else if(strcmp(item.itemName_,"枪2K1K2")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"2枪K1K2控制",setVal,3))
			{
				APPEND_INFO(masgHandle,"闭合继电器 2枪K1K2 失败");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"闭合继电器 2枪K1K2 成功"); 
			}
			
		}else if(strcmp(item.itemName_,"K5K6")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"1枪K5K6控制",setVal,3))
			{
				APPEND_INFO(masgHandle,"闭合继电器 1枪K5K6控制 失败");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"闭合继电器 1枪K5K6控制 成功"); 
			}	
		}else if(strcmp(item.itemName_,"K7K8")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"K7K8控制",setVal,3))
			{
				APPEND_INFO(masgHandle,"闭合继电器 K7K8控制 失败");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"闭合继电器 K7K8控制 成功"); 
			}	
		}else if(strcmp(item.itemName_,"K9K10")==0)
		{
			if(FALSE==ParamSetDependWithRetry(eut,"2枪K9K10控制",setVal,3))
			{
				APPEND_INFO(masgHandle,"闭合继电器 2枪K9K10控制 失败");
				return TEST_RESULT_SOMEPASS;
			}else{
				APPEND_INFO(masgHandle,"闭合继电器 2枪K9K10控制 成功"); 
			}	
		}
		
		int reTryCnt=0;
		while(reTryCnt++<5)
		{
			memset(result.recvString,0,RESULT_RECEIVE_LEN);
			if(FALSE==ParamGetDepend(eut,"BI",result.recvString ))
			{
				APPEND_INFO_FORMAT(masgHandle,"%s 获取到继电器反馈失败 %s");
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

			APPEND_INFO_FORMAT(masgHandle,"%s 获取到继电器反馈失败 %s,结果为%d 获取次数:%d",item.itemName_,result.recvString,result.pass,reTryCnt);
			if(result.pass==RESULT_PASS)
				break;
			//WarnAlert(0,"请等待",10);
			if(FALSE==AlertDialogWithRetAutoClose(0,"waring","点击确定跳过等待，按下取消退出本次测试","取消","跳过",10))
			{
				break;
			}
		}			
		saveResult(hashTable,&result); 

	}
	APPEND_INFO(masgHandle,"退出测试");	
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
					SetCtrlVal(panel,BOXCODE_ERRORINFO,"格式不对！");				
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
	APPEND_INFO(masgHandle,"进入测试"); 
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
	
	APPEND_INFO_FORMAT(masgHandle,"控制盒编号是：%s",code);
	
	TestItem item={0};
	RESULT result={0};
	ListGetItem(group.subItems,&item,1);
	result.index = item.itemId;
	
	if(FALSE==ParamSetDepend(eut,item.itemName_,code))
	{
	
		APPEND_INFO(masgHandle,"数据盒操作失败"); 
		goto DONE;
	}
	
	APPEND_INFO(masgHandle,"控制盒编号设置成功"); 	
	if(FALSE==ParamGetDepend(eut,item.itemName_,result.recvString))
	{
	
		APPEND_INFO(masgHandle,"数据盒操作失败");
		goto DONE;
	}
	APPEND_INFO_FORMAT(masgHandle,"控制盒读回成功编号是：%s",result.recvString);
	
	
	if(strcmp(code,result.recvString)==0)
	{
		result.pass=RESULT_PASS;
	}
	

	saveResult(hashTable,&result); 
		
	APPEND_INFO(masgHandle,"退出测试");	
DONE:	
	return ret;
}


TPS registerCtrlBoxSetTestTps(void)
{
	TPS tps=newTps("CtrlBoxSet");
	tps.testFunction=CtrlBoxSetTest;
	return tps;			
}


METHODRET PowerModuleTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"进入测试"); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	if(FALSE==ParamSetDependWithRetry(eut,"DO单一控制标志","0",3))
	{
		APPEND_INFO(masgHandle,"DO单一控制标志 失败");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(masgHandle,"DO单一控制标志 成功"); 
	}
	
	if(strcmp(group.groupName,"断开交流接触器")==0 || strcmp(group.groupName,"枪1泄放")==0 || strcmp(group.groupName,"枪2泄放")==0)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,1);
		RESULT result={0};
		result.index = item.itemId;
		result.pass = FALSE;
		tBIBO bibo={0};
		if(FALSE==getBibo(group.groupName,&bibo))
		{
			APPEND_INFO_FORMAT(masgHandle,"%s 无此配置",group.groupName);
			saveResult(hashTable,&result);
			goto DONE;
		}
		char setVal[20]={0}; 
		sprintf(setVal,"%d",bibo.maskBo);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO_FORMAT(masgHandle,"%s,操作BO失败",group.groupName);
			saveResult(hashTable,&result);
			goto DONE; 
		}else{
			result.pass = TRUE; 
			APPEND_INFO_FORMAT(masgHandle,"%s,操作BO成功：%x",group.groupName,bibo.maskBo);
			
		}
		saveResult(hashTable,&result);
		return ret;
	}	
	
	
	//闭合交流接触器
	/*if(FALSE==ParamSetDependWithRetry(eut,"KM控制","1",3))
	{
		APPEND_INFO(masgHandle,"KM控制 失败");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(masgHandle,"KM控制 成功"); 
	}*/
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		RESULT result={0};
		ListGetItem(group.subItems,&item,i);
		result.index=item.itemId;
		unsigned int input = HexStrToUnsignedInt(item.inputValue_);
		APPEND_INFO_FORMAT(masgHandle,"%s 测试",item.itemName_); 
		tBIBO bibo={0};
		if(FALSE==getBibo(item.itemName_,&bibo))
		{
			APPEND_INFO_FORMAT(masgHandle,"%s 无此配置",item.itemName_);
			continue;
		}
		char setVal[20]={0}; 
		sprintf(setVal,"%d",bibo.maskBo);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(masgHandle,"操作BO失败");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(masgHandle,"操作BO成功：%x",bibo.maskBo);
		}
		
		char getVal[20]={0};
		float getValF=0;
		float standardF=0;
		
		int retryCnt = 10;
		while(retryCnt-- >=0 )
		{
			if(strcmp(item.itemName_,"电源模块1")==0 || strcmp(item.itemName_,"电源模块2")==0 || strcmp(item.itemName_,"电源模块3")==0 || strcmp(item.itemName_,"电源模块6_1")==0)
			{
				if(FALSE == ParamGetDependWithRetry(eut,"枪1电表电压",getVal,3))
				{
					APPEND_INFO(masgHandle,"获取枪1电压失败"); 
					goto DONE;		
				}
			}else{
				if(FALSE == ParamGetDependWithRetry(eut,"枪2电表电压",getVal,3))
				{
					APPEND_INFO(masgHandle,"获取枪2电压失败");  
					goto DONE;		
				}				
			}
			
			getValF= atof(getVal);
		
			standardF=atof(item.standard_);
		
		
						
		
			if(getValF>standardF-3 && getValF<standardF+3)
			{
				result.pass=RESULT_PASS;
				//break;
			}else{
				result.pass=RESULT_FAIL;
			}
			
			memset(result.recvString,0,RESULT_RECEIVE_LEN);
			sprintf(result.recvString,"%f",getValF);
			
			APPEND_INFO_FORMAT(masgHandle,"电压值为:%f,测试结果为:%d",getValF,result.pass);
			if(result.pass == RESULT_PASS)
				break;
			
			if(FALSE==AlertDialogWithRetAutoClose(0,"waring","点击确定跳过等待，按下取消退出本次测试","取消","跳过",5))
			{
				break;
			}
			
		}
		
		//断开所有及短期
		memset(setVal,0,20);
		sprintf(setVal,"%d",bibo.maskBi);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(masgHandle,"操作BO失败");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(masgHandle,"操作BO成功：%x",bibo.maskBi);
		}		
		saveResult(hashTable,&result);				
	}
	APPEND_INFO(masgHandle,"退出测试");
	return ret;
DONE:	
	APPEND_INFO(masgHandle,"退出测试");
	return ret;
}

TPS registerPowerModuleTestTps(void)
{
	TPS tps=newTps("PowerModule");
	tps.testFunction=PowerModuleTest;
	//tps.protocolInit=BiboProtocolInit;
	return tps;			
}


METHODRET Power120KWModuleTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	APPEND_INFO(masgHandle,"进入测试"); 
	METHODRET ret = TEST_RESULT_ALLPASS;
	if(FALSE==ParamSetDependWithRetry(eut,"DO单一控制标志","0",3))
	{
		APPEND_INFO(masgHandle,"DO单一控制标志 失败");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(masgHandle,"DO单一控制标志 成功"); 
	}
	
	if(strcmp(group.groupName,"断开交流接触器")==0 || strcmp(group.groupName,"枪1泄放")==0 || strcmp(group.groupName,"枪2泄放")==0)
	{
		TestItem item={0};
		ListGetItem(group.subItems,&item,1);
		RESULT result={0};
		result.index = item.itemId;
		result.pass = FALSE;
		tBIBO bibo={0};
		if(FALSE==getBibo(group.groupName,&bibo))
		{
			APPEND_INFO_FORMAT(masgHandle,"%s 无此配置",group.groupName);
			saveResult(hashTable,&result);
			goto DONE;
		}
		char setVal[20]={0}; 
		sprintf(setVal,"%d",bibo.maskBo);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO_FORMAT(masgHandle,"%s,操作BO失败",group.groupName);
			saveResult(hashTable,&result);
			goto DONE; 
		}else{
			result.pass = TRUE; 
			APPEND_INFO_FORMAT(masgHandle,"%s,操作BO成功：%x",group.groupName,bibo.maskBo);
			
		}
		saveResult(hashTable,&result);
		return ret;
	}	
	
	
	//闭合交流接触器
	/*if(FALSE==ParamSetDependWithRetry(eut,"KM控制","1",3))
	{
		APPEND_INFO(masgHandle,"KM控制 失败");
		return TEST_RESULT_SOMEPASS;
	}else{
		APPEND_INFO(masgHandle,"KM控制 成功"); 
	}*/
	
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item={0};
		RESULT result={0};
		ListGetItem(group.subItems,&item,i);
		result.index=item.itemId;
		unsigned int input = HexStrToUnsignedInt(item.inputValue_);
		APPEND_INFO_FORMAT(masgHandle,"%s 测试",item.itemName_); 
		tBIBO bibo={0};
		if(FALSE==getBibo(item.itemName_,&bibo))
		{
			APPEND_INFO_FORMAT(masgHandle,"%s 无此配置",item.itemName_);
			continue;
		}
		char setVal[20]={0}; 
		sprintf(setVal,"%d",bibo.maskBo);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(masgHandle,"操作BO失败");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(masgHandle,"操作BO成功：%x",bibo.maskBo);
		}
		
		char getVal[20]={0};
		float getValF=0;
		float standardF=0;
		
		int retryCnt = 10;
		while(retryCnt-- >=0 )
		{
			/*if(strcmp(item.itemName_,"120KW_电源模块1")==0 || strcmp(item.itemName_,"120KW_电源模块2")==0 || strcmp(item.itemName_,"120KW_电源模块3")==0 || strcmp(item.itemName_,"120KW_电源模块4")==0)
			{
				if(FALSE == ParamGetDependWithRetry(eut,"枪1电表电压",getVal,3))
				{
					APPEND_INFO(masgHandle,"获取枪1电压失败"); 
					goto DONE;		
				}
			}else{
				if(FALSE == ParamGetDependWithRetry(eut,"枪2电表电压",getVal,3))
				{
					APPEND_INFO(masgHandle,"获取枪2电压失败");  
					goto DONE;		
				}				
			}*/
			
			if(strcmp(item.inputValue_,"电表1")==0)
			{
				if(FALSE == ParamGetDependWithRetry(eut,"枪1电表电压",getVal,3))
				{
					APPEND_INFO(masgHandle,"获取枪1电压失败"); 
					goto DONE;		
				}
			}else  if(strcmp(item.inputValue_,"电表2")==0){
				if(FALSE == ParamGetDependWithRetry(eut,"枪2电表电压",getVal,3))
				{
					APPEND_INFO(masgHandle,"获取枪1电压失败"); 
					goto DONE;		
				}
			}
			
			getValF= atof(getVal);
		
			standardF=atof(item.standard_);
		
		
						
		
			if(getValF>standardF-3 && getValF<standardF+3)
			{
				result.pass=RESULT_PASS;
				//break;
			}else{
				result.pass=RESULT_FAIL;
			}
			
			memset(result.recvString,0,RESULT_RECEIVE_LEN);
			sprintf(result.recvString,"%f",getValF);
			
			APPEND_INFO_FORMAT(masgHandle,"电压值为:%f,测试结果为:%d",getValF,result.pass);
			if(result.pass == RESULT_PASS)
			{
			
				if(FALSE==AlertDialogWithRetAutoClose(0,"waring","请观察风扇转动情况","取消","跳过",15))
				{
					break;
				}
				
				if(AlertDialogWithRet(0,"warning","风扇是否正常","不正常","正常")==FALSE)
				{
					result.pass =  RESULT_FAIL;
				}
				
				

				break;
			}

			
			if(FALSE==AlertDialogWithRetAutoClose(0,"waring","点击确定跳过等待，按下取消退出本次测试","取消","跳过",5))
			{
				break;
			}
			
		}
		
		//断开所有及短期
		memset(setVal,0,20);
		sprintf(setVal,"%d",bibo.maskBi);
		if(FALSE==ParamSetDependWithRetry(eut,"BO",setVal,3))
		{
			APPEND_INFO(masgHandle,"操作BO失败");
			goto DONE; 
		}else{
			APPEND_INFO_FORMAT(masgHandle,"操作BO成功：%x",bibo.maskBi);
		}		
		saveResult(hashTable,&result);				
	}
	APPEND_INFO(masgHandle,"退出测试");
	return ret;
DONE:	
	APPEND_INFO(masgHandle,"退出测试");
	return ret;
}


TPS register120KWPowerModuleTestTps(void)
{
	TPS tps=newTps("PowerModule_120KW");
	tps.testFunction=Power120KWModuleTest;
	//tps.protocolInit=BiboProtocolInit;
	return tps;			
}

int CVICALLBACK GunSelectPanelCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	switch (event)
	{
		case EVENT_COMMIT:
			QuitUserInterface(0);
			break;
	}
	return 0;
}

METHODRET GunSelectedTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	APPEND_INFO(masgHandle,"进入测试");
	int panelHandle = LoadPanel(0,"ParamPanel.uir",GUNPANEL);
	InstallCtrlCallback(panelHandle,GUNPANEL_COMMANDBUTTON,GunSelectPanelCallback,NULL);
	DisplayPanel(panelHandle);
	RunUserInterface();
	TestItem item={0};
	ListGetItem(group.subItems,&item,1);
	RESULT result ={0};
	result.index = item.itemId;
	result.pass= RESULT_PASS;
	int ringIdx=0;
	GetCtrlIndex (panelHandle,GUNPANEL_RING,&ringIdx);
	GetLabelFromIndex (panelHandle, GUNPANEL_RING, ringIdx, result.recvString);
	saveResult(hashTable,&result);
	DiscardPanel(panelHandle);
	APPEND_INFO(masgHandle,"退出测试"); 
	return ret;
}

TPS registerGunSelectedTestTps(void)
{
	TPS tps=newTps("gunSelect");
	tps.testFunction=GunSelectedTest;
	tps.createTpsPanel=NULL;
	//tps.protocolInit=BiboProtocolInit;
	return tps;			
}

METHODRET NetWorkCheckTest(TestGroup group,EUT eut,HashTableType hashTable,int masgHandle)
{
	METHODRET ret = TEST_RESULT_ALLPASS;
	APPEND_INFO(masgHandle,"主动断开网络"); 
	ReleaseStubNetService();
	TestItem item={0};
	ListGetItem(group.subItems,&item,1);
	RESULT result={0};
	result.index=item.itemId;
	result.pass = RESULT_PASS;
	saveResult(hashTable,&result);
	return ret;
}

TPS registerNetWorkCheckTestTps(void)
{
	TPS tps=newTps("NetWorkCheck");
	tps.testFunction=NetWorkCheckTest;
	tps.createTpsPanel=NULL;
	//tps.protocolInit=BiboProtocolInit;
	return tps;			
}

