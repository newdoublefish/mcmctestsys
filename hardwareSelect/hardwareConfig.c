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


