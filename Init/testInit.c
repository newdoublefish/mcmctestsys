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
#include <cvirte.h>		
#include <userint.h>
#include <pathctrl.h>
#include "cvixml.h"        
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "testGroupInit.h"  
#include "sutCommon.h"
#include "protocolHelper.h"
#include "log.h"
#include "common.h"
#include "tpsHelper.h"
#include "tipsParse.h"
#include "relayProtocol.h"



static HRESULT showProgressValidity(int panel,char *itemName,int perc,char *status)
{
	char description[100]={0};
  
	if(status!=NULL)
	{	
	   Fmt(description,"%s      %s\n",itemName,status);
	   slideProgressShowWithTextBox(panel,perc,description,0.01);
	}else
	{
	   slideProgressShowWithTextBox(panel,perc,NULL,0.01);
	}	
	
	return 0;
}



HRESULT testInit(SUT selectedsut)
{
	//printf("%s",selectedsut.systemName);
	if(strcmp(selectedsut.systemName,"1305")==0)
	{	
#if 0	
	if(collectParamInit(selectedsut.configPath)<0)
	{
	   LOG_EVENT(LOG_ERROR,"collectParam analyse error");   
	   return -1;
	}
	//return -1; 
	initFrame();
	
	//CollectParam param = getCollectParamByHashTable("模拟电压量6(桨叶动载荷值)");
	//printf("id:%d\n",param.id);
#endif
	}
	
	
	
    if(testGroupInit(selectedsut.configPath)<0)   //解析测试用例表
	{
		
		LOG_EVENT(LOG_ERROR,"testCase analyse error");   
	    return -1;
	}
	
	
	
	initTps();
	
	getTipsFromExcel();
	
	parseRelayProtocol();					//继电器协议解析	
	
#if 1	
	if(protocolInit()<0)                   //解析协议
	{
		LOG_EVENT(LOG_ERROR,"protocol analyse error"); 
	    return -1;
	}	
#endif
	//ManualItemLen = ManualItemInit(project.configPath,ManuItem);	
	//StatusPanelInit (LRUstatus); 	
	
	LOG_EVENT(LOG_INFO,"TEST init success!");
	
	return TRUE;
}

HRESULT testDeinit()
{
	//todo:释放空间
	testGroupDeinit();
    return TRUE;
}
