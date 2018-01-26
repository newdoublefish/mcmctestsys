#include "EutManage.h"
#include "SerialConfigPanel.h"
#include "eutEntity.h"   

#define SERIAL_TAG "Serial"
#define	SERIAL_PORT "port"
#define SERIAL_RATE "rate"
#define SERIAL_PARITY "parity"
#define SERIAL_DATAB "data"
#define SERIAL_STOPB "stopb"

void LoadSerialConfig(ListType list,int handle)
{
	 RSCONFIG *config=(RSCONFIG *)handle;
	 Map map;
	 if(getMap(list,SERIAL_PORT,&map)>0)
	 {
	 	 config->portNum= atoi(map.value);
	 }
	 if(getMap(list,SERIAL_RATE,&map)>0)
	 {
	 	config->baudRate= atoi(map.value); 
	 }
	 if(getMap(list,SERIAL_PARITY,&map)>0)
	 {
	 	config->parity= atoi(map.value);
	 }
	 if(getMap(list,SERIAL_DATAB,&map)>0)
	 {
	 	config->dataBit= atoi(map.value);
	 }
	 if(getMap(list,SERIAL_STOPB,&map)>0)
	 {
	 	config->stopBit= atoi(map.value);
	 }	 
}  

int LoadSerialConfigPanel(int handle)
{
	int panelHandle=LoadPanel(0,"SerialConfigPanel.uir",PANEL);
	int i=0;
	if(handle!=0)
	{	
		RSCONFIG *config=(RSCONFIG *)(handle);
		if(config->portNum>0)
		{	
			GetIndexFromValue(panelHandle,PANEL_PORTNUM,&i,config->portNum);
			SetCtrlIndex(panelHandle,PANEL_PORTNUM,i);
			GetIndexFromValue(panelHandle,PANEL_RATE,&i,config->baudRate);
			SetCtrlIndex(panelHandle,PANEL_RATE,i);
			GetIndexFromValue(panelHandle,PANEL_PARITY,&i,config->parity);
			SetCtrlIndex(panelHandle,PANEL_PARITY,i);
			GetIndexFromValue(panelHandle,PANEL_DATAB,&i,config->dataBit);
			SetCtrlIndex(panelHandle,PANEL_DATAB,i);
			GetIndexFromValue(panelHandle,PANEL_STOPB,&i,config->stopBit);
			SetCtrlIndex(panelHandle,PANEL_STOPB,i);
		}
	}
	return panelHandle;
}

void GetChangedSerialConfig(int panel,int handle)
{
	int selectedIndex;
	RSCONFIG *config=(RSCONFIG *)(handle); 
	GetCtrlIndex(panel,PANEL_PORTNUM,&selectedIndex);
	GetValueFromIndex(panel,PANEL_PORTNUM,selectedIndex,&config->portNum);
	
	GetCtrlIndex(panel,PANEL_RATE,&selectedIndex);
	GetValueFromIndex(panel,PANEL_RATE,selectedIndex,&config->baudRate);
	
	GetCtrlIndex(panel,PANEL_PARITY,&selectedIndex);
	GetValueFromIndex(panel,PANEL_PARITY,selectedIndex,&config->parity);
	
	GetCtrlIndex(panel,PANEL_DATAB,&selectedIndex);
	GetValueFromIndex(panel,PANEL_DATAB,selectedIndex,&config->dataBit);
	
	GetCtrlIndex(panel,PANEL_STOPB,&selectedIndex);
	GetValueFromIndex(panel,PANEL_STOPB,selectedIndex,&config->stopBit);	
																			
}

int CreateSerialConfig()
{
	RSCONFIG *config=(RSCONFIG *)malloc(sizeof(RSCONFIG));
	memset(config,0,sizeof(RSCONFIG));
	return (int)config;  
}


void SaveSerialConfig(ListType list,int handle)  
{
	char temp[20]={0};
	RSCONFIG *config=(RSCONFIG *)handle; 
	addCharIntMap(list,SERIAL_PORT,config->portNum); 
	addCharIntMap(list,SERIAL_RATE,config->baudRate); 
	addCharIntMap(list,SERIAL_PARITY,config->parity); 
	addCharIntMap(list,SERIAL_DATAB,config->dataBit); 
	addCharIntMap(list,SERIAL_STOPB,config->stopBit); 
																
}

ConfigActivity RS422ConfigActivity(void)
{
	ConfigActivity activity={0};
	sprintf(activity.configName,"%s",SERIAL_TAG);
	activity.onCreate=(void *)CreateSerialConfig;
	activity.onLoadConfig=(void *)LoadSerialConfig;
	activity.onLoadPanel=(void *)LoadSerialConfigPanel;
	activity.onSaveConfig=(void *)SaveSerialConfig;
	activity.onPanelChange=(void *)GetChangedSerialConfig;
	return activity;
}



































