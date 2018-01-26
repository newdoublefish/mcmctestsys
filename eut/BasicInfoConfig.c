
#include "BasicInfoConfigPanel.h"
#include "EutManage.h"
#include "eutEntity.h"


#define BASIC 				"Basic"               
#define BASICINFO_ID 		"Id"
#define BASICINFO_NAME 		"name"
#define BASICINFO_ENABLE 	"enable"

void loadBasicInfoConfig(ListType list,int handle)
{
	 EutBasicInfo *config=(EutBasicInfo *)handle;
	 Map map;
	 if(getMap(list,BASICINFO_ID,&map)>0)
	 {
	 	 config->id= atoi(map.value);
	 }
	 
	 if(getMap(list,BASICINFO_NAME,&map)>0)
	 {
	 	sprintf(config->eutName,"%s",map.value);
	 }
	 
	 if(getMap(list,BASICINFO_ENABLE,&map)>0)
	 {
	 	config->enable= atoi(map.value);
	 }
}

void saveBasicInfoConfig(ListType list,int handle)  
{
	char temp[20]={0};
	EutBasicInfo *config=(EutBasicInfo *)handle; 
	addCharIntMap(list,BASICINFO_ID,config->id); 
	addCharCharMap(list,BASICINFO_NAME,config->eutName);
	addCharIntMap(list,BASICINFO_ENABLE,config->enable); 

}


int LoadBasicConfigPanel(int handle)
{
	int panelHandle=LoadPanel(0,"BasicInfoConfigPanel.uir",PANEL);
	if(handle!=0)
	{
		EutBasicInfo *config=(EutBasicInfo *)(handle); 
		char temp[20]={0};
		sprintf(temp,"%s",config->eutName);
		SetCtrlVal(panelHandle,PANEL_NAMESTR,temp);
		memset(temp,0,20);
		sprintf(temp,"%d",config->id);
		SetCtrlVal(panelHandle,PANEL_IDSTR,temp);
		memset(temp,0,20);
		sprintf(temp,"%d",config->enable);
		SetCtrlVal(panelHandle,PANEL_CHECKBOX,config->enable);
	}
	return panelHandle;
}

void GetChangedBasicInfoConfig(int panel,int handle)
{
	EutBasicInfo *config=(EutBasicInfo *)handle;  
	char temp[100]={0};
	GetCtrlVal(panel,PANEL_IDSTR,temp);
	config->id=atoi(temp);
	GetCtrlVal(panel,PANEL_NAMESTR,config->eutName);
	GetCtrlVal(panel,PANEL_CHECKBOX,&config->enable);
}


int createBasicInfoConfig()
{
	EutBasicInfo *config=(EutBasicInfo *)malloc(sizeof(EutBasicInfo));
	memset(config,0,sizeof(EutBasicInfo));
	return (int)config;  
}

ConfigActivity BasicConfigActivity()
{
	ConfigActivity activity={0};
	sprintf(activity.configName,"%s",BASIC);
	activity.onCreate=(void *)createBasicInfoConfig;
	activity.onLoadConfig=(void *)loadBasicInfoConfig;
	activity.onSaveConfig=(void *)saveBasicInfoConfig;
	activity.onLoadPanel=(void *)LoadBasicConfigPanel;
	activity.onPanelChange=(void *)GetChangedBasicInfoConfig;
	return activity;
}


