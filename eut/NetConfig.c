#include "NetConfigPanel.h"
#include "EutManage.h"   
#include "eutEntity.h"

#define NET_TAG "Net"
#define	NET_IP "ip"
#define NET_PORT "port"


void LoadNetConfig(ListType list,int handle)
{
	 NETCONFIG *config=(NETCONFIG *)handle;
	 Map map;
	 if(getMap(list,NET_IP,&map)>0)
	 {
	 	 //config->ip= atoi(map.value);
		 sprintf(config->ip,"%s",map.value);
	 }
	 if(getMap(list,NET_PORT,&map)>0)
	 {
	 	config->port= atoi(map.value); 
	 }
}  

int LoadNetConfigPanel(int handle)
{
	int panelHandle=LoadPanel(0,"NetConfigPanel.uir",PANEL);
	int i=0;
	if(handle!=0)
	{	
		NETCONFIG *config=(NETCONFIG *)(handle);
		SetCtrlVal(panelHandle,PANEL_IP,config->ip);
		char temp[10]={0};
		sprintf(temp,"%d",config->port);
		SetCtrlVal(panelHandle,PANEL_PORT,temp);
	}
	return panelHandle;
}

void GetChangedNetConfig(int panel,int handle)
{
	NETCONFIG *config=(NETCONFIG *)(handle); 
	memset(config->ip,0,MAX_NET_IP_LEN);
	GetCtrlVal(panel,PANEL_IP,config->ip);
	char temp[30]={0};
	GetCtrlVal(panel,PANEL_PORT,temp);
	config->port=atoi(temp);
}

int CreateNetConfig()
{
	NETCONFIG *config=(NETCONFIG *)malloc(sizeof(NETCONFIG));
	memset(config,0,sizeof(NETCONFIG));
	return (int)config;  
}


void SaveNetConfig(ListType list,int handle)  
{
	NETCONFIG *config=(NETCONFIG *)handle; 
	addCharCharMap(list,NET_IP,config->ip); 
	addCharIntMap(list,NET_PORT,config->port);
}

ConfigActivity NetConfigActivity(void)
{
	ConfigActivity activity={0};
	sprintf(activity.configName,"%s",NET_TAG);
	activity.onCreate=(void *)CreateNetConfig;
	activity.onLoadConfig=(void *)LoadNetConfig;
	activity.onLoadPanel=(void *)LoadNetConfigPanel;
	activity.onSaveConfig=(void *)SaveNetConfig;
	activity.onPanelChange=(void *)GetChangedNetConfig;
	return activity;
}
