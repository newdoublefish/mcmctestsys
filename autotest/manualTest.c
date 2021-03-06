#include "tpsHelper.h"
#include "resultSave.h"
#include "ManualTestPanel.h"

int CVICALLBACK onManualTestCallBack (int panel, int control, int event,
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

int CVICALLBACK onManualTestPanelCallBack (int panel, int event, void *callbackData,
		int eventData1, int eventData2)
{
	if (event == EVENT_CLOSE)
		QuitUserInterface (0);
	return 0;
}

METHODRET manualTest(TestGroup group,EUT eut,HashTableType hashTable)
{
	//printf("manualTest\n");
	int panelHandle=LoadPanel(0,"ManualTestPanel.uir",PANEL);
	DisplayPanel(panelHandle);
	METHODRET ret = TEST_RESULT_ALLPASS;
	InsertTableRows (panelHandle,PANEL_TABLE,-1,ListNumItems(group.subItems),VAL_USE_MASTER_CELL_TYPE);
	InsertTableCellRangeRingItem (panelHandle, PANEL_TABLE, MakeRect(1, 4, ListNumItems(group.subItems), 1), -1, "合格");
	InsertTableCellRangeRingItem (panelHandle, PANEL_TABLE, MakeRect(1, 4, ListNumItems(group.subItems), 1), -1, "不合格");
	SetActiveTableCell (panelHandle, PANEL_TABLE, MakePoint(3,1));
	FakeKeystroke(VAL_F2_VKEY); 
	SetActiveCtrl(panelHandle,PANEL_COMMANDBUTTON);
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		SetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(1,i),item.itemName_);
		SetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(2,i),item.standard_);  
	}
	InstallCtrlCallback(panelHandle,PANEL_COMMANDBUTTON,onManualTestCallBack,NULL);
	InstallPanelCallback(panelHandle,onManualTestPanelCallBack,NULL);
	RunUserInterface ();
	for(int i=1;i<=ListNumItems(group.subItems);i++)
	{
		TestItem item;
		ListGetItem(group.subItems,&item,i);
		RESULT res={0};
		res.index=item.itemId;
		GetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(3,i),res.recvString);
		char temp[50]={0};
		GetTableCellVal(panelHandle,PANEL_TABLE,MakePoint(4,i),temp);				
		if(strcmp("合格",temp)==0)
		{
			res.pass=RESULT_PASS;
		}else{
			res.pass=RESULT_FAIL;
			ret = TEST_RESULT_SOMEPASS;
		}
		//printf("%s,%d\n",res.recvString,res.pass);
		saveResult(hashTable,&res);
	}	
	DiscardPanel(panelHandle);
	
	return ret;
}
