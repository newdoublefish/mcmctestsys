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
#include "excel2000.h"    
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "AppInitSplash.h" 
#include "excelHelper.h"   
#include "log.h"
#include "reportDb.h"
#include "common.h"
#include "application.h"


#define DEBUG

HRESULT EnterAppInit()
{
	int panelLoadHandle;
	if ((panelLoadHandle = LoadPanel (0, "AppInitSplash.uir", LOAD_P)) < 0)
		return -1;
	DisplayPanel(panelLoadHandle);
	
	ApplicationInit();
	
	tApplication ta = getApplication();
	
	SetCtrlVal(panelLoadHandle,LOAD_P_LOGO,ta.basic.logo);
	SetCtrlVal(panelLoadHandle,LOAD_P_APPNAME,ta.basic.applicationName);
	SetCtrlVal(panelLoadHandle,LOAD_P_VERSION,ta.basic.version);
	SetCtrlVal(panelLoadHandle,LOAD_P_COPYRIGHT,ta.basic.copyRight);
	
	//SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG,"正在初始化Excel组建...");
	
	
	//注意加入以下这行代码，程序不会中断，调试时候打开
#ifndef DEBUG	
	DisableBreakOnLibraryErrors();
#endif	
	
	//Delay(2);
	InitExcel();
	
	//SetCtrlVal(panelLoadHandle,LOAD_P_TEXTMSG,"正在初始化Excel组建...OK"); 
	
	/*if(initDb()<0)
	{
		WarnShow1(0,"数据库连接失败");
		//DeInitExcel();
	}*/
	
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

  

