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
#include "radioGroup.h"
#include <userint.h>  
#include <stdlib.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include "SettingPanel.h"
#include "settingEntity.h"
#include "HardwarePanel.h"
#include "hardwareConfig.h"

static int hdsPanel;
static HARDWARE gHardware;
void DisplayHardwarePanelAsTabPanel(int panel)
{
	hdsPanel=LoadPanel (panel, "HardwarePanel.uir", HDS_PANEL);
	gHardware=getHardware();
	Radio_SetMarkedOption(hdsPanel,HDS_PANEL_RADIOGROUP,gHardware.arinc429card);
	DisplayPanel(hdsPanel);  
}

int CVICALLBACK HDSCALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int selectedIndex;
	switch (event)
	{
		case EVENT_COMMIT:
			
			Radio_GetMarkedOption(hdsPanel,HDS_PANEL_RADIOGROUP,&(gHardware.arinc429card));
			saveHardware(gHardware);
			break;
	}
	return 0;
}

HARDWARE GetHardware()
{
    return gHardware;
}


