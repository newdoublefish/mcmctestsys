#include "excel2000.h"    
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "AppInitSplash.h" 
#include "excelHelper.h"   
#include "log.h"



HRESULT EnterAppInit()
{
	int panelLoadHandle;
	if ((panelLoadHandle = LoadPanel (0, "AppInitSplash.uir", LOAD_P)) < 0)
		return -1;
	DisplayPanel(panelLoadHandle);
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_V,"�汾:v0.1");
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG,"���ڳ�ʼ��Excel�齨...");
	
	//Delay(2);
	InitExcel();
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG,"���ڳ�ʼ��Excel�齨...OK"); 
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_422,"���ڳ�ʼ��422�忨...");
	
	//Delay(2); 
	
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_422,"���ڳ�ʼ��422�忨...OK");
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_429,"���ڳ�ʼ��429�忨...");
	
	//Delay(2); 
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_429,"���ڳ�ʼ��429�忨...OK");  
	
	initLogModule();
	
	//Delay(2); 
	
	
	DiscardPanel(panelLoadHandle);
	
	return 0;
}

HRESULT AppDeInit()
{
	quitLogModule();//�Ƿ�����������
	DeInitExcel();
	return 0;
}

  

