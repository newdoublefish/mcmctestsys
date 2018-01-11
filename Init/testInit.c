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
	
	//CollectParam param = getCollectParamByHashTable("ģ���ѹ��6(��Ҷ���غ�ֵ)");
	//printf("id:%d\n",param.id);
#endif
	}
	
	
	
    if(testGroupInit(selectedsut.configPath)<0)   //��������������
	{
		
		LOG_EVENT(LOG_ERROR,"testCase analyse error");   
	    return -1;
	}
	
	
	
	initTps();
	
	getTipsFromExcel();
	
	parseRelayProtocol();					//�̵���Э�����	
	
#if 1	
	if(protocolInit()<0)                   //����Э��
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
	//todo:�ͷſռ�
	testGroupDeinit();
    return TRUE;
}
