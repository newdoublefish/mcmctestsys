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
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_V,"版本:v0.1");
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG,"正在初始化Excel组建...");
	
	//Delay(2);
	InitExcel();
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG,"正在初始化Excel组建...OK"); 
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_422,"正在初始化422板卡...");
	
	//Delay(2); 
	
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_422,"正在初始化422板卡...OK");
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_429,"正在初始化429板卡...");
	
	//Delay(2); 
	
	SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG_429,"正在初始化429板卡...OK");  
	
	initLogModule();
	
	//Delay(2); 
	
	
	DiscardPanel(panelLoadHandle);
	
	return 0;
}

HRESULT AppDeInit()
{
	quitLogModule();//是否有死锁可能
	DeInitExcel();
	return 0;
}

  

