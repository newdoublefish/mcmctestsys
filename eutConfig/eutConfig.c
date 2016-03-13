/******************************************************************************

                  ��Ȩ���� (C), 2014-2020, ���ݺ��µ�һ�����                  

 ******************************************************************************/
/** @file    systemSelect.c
 *  @version ����
 *  @author  ������
 *  @date    2015.1.22
 *  @brief   uut���ý�����������ļ��������������û�����Ĳ��������룬�����
 *  @note    �޶���¼:

 ******************************************************************************/

#include "cvixml.h"
#include <cvirte.h>		
#include <userint.h>
#include <stdlib.h>
#include "toolbox.h"
#include <utility.h>
#include <ansi_c.h>
#include <formatio.h>
#include <userint.h>
#include "eutEntity.h"
#include "EutPanel.h"
#include "MainPanel.h"
#include "debug.h"
//#define EUT_CONFIG_DEBUG

static int eutConfigPanelHandle;  //���panel���п������ⲿ����
static int currentEutIndex=0;	   //��ǰѡ�еĵ�EUT
static int isAddAction=0;		   //��ǰ�����Ƿ�Ϊ����UUT�Ĳ���

static ListType eutList;  //�豸����


#define EUT_SETTING_TAB_BASIC 0
#define EUT_SETTING_TAB_MATAIN 1
#define EUT_SETTING_TAB_RS422 2
#define EUT_SETTING_TAB_ARINC429 3
#define EUT_SETTING_TAB_RELAY 4   


static void ShowEutListByTree(ListType eutList)
{
	int eutIndex=0,eutCount=0;
	char temp[30]={0};
	EUT eut;
	eutCount=ListNumItems(eutList);
	for(eutIndex=1;eutIndex<=eutCount;eutIndex++)
	{
	    ListGetItem(eutList,&eut,eutIndex);
		sprintf(temp,"��ţ�%d",eut.index);
	    InsertTreeItem (eutConfigPanelHandle, EUTPANEL_EUT_TREE, VAL_SIBLING,0, VAL_LAST,
                    temp,0,0,0);
	    SetTreeCellAttribute(eutConfigPanelHandle,EUTPANEL_EUT_TREE,eutIndex-1,1, ATTR_LABEL_TEXT,eut.eutName);
	}
	//��ʾ��һ���豸
	if(eutCount>0)
	  SetActiveTreeItem(eutConfigPanelHandle,EUTPANEL_EUT_TREE,0,VAL_REPLACE_SELECTION_WITH_ITEM);  	
}


void DisplayEutPanelAsTabPanel(int panel)
{

	eutConfigPanelHandle=LoadPanel(panel, "EutPanel.uir", EUTPANEL);
	DisplayPanel(eutConfigPanelHandle);//��������ʾ���
	eutList = getEutListFromXmlFile(); //��xml�ļ��л�ȡeut����
	ClearListCtrl(eutConfigPanelHandle,EUTPANEL_EUT_TREE);//��������ʾ�ռ�   
	ShowEutListByTree(eutList);//��ʾeut����
}

static void ShowEutDetail(int panel,int index)
{
	 EUT device; 
     int comPort=0;
	 char temp[32]={0};
	 int i;
	 int tempPanel;

	 ListGetItem(eutList,&device,index);

	 //---------------------��������--------------------------
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_BASIC,&tempPanel);//��ȡ�����
   	 SetCtrlVal(tempPanel,BASIC_EUTNAME,device.eutName);  //�豸����
	 sprintf(temp,"%d",device.index);
	 SetCtrlVal(tempPanel,BASIC_EUTID,temp);	//�豸���
	 
	 SetCtrlVal(tempPanel,BASIC_ENABLECHECKBOX,device.enable);
	 
#ifdef EUT_CONFIG_DEBUG
	  PRINT("MATAIN CONFIG port:%d,baudRate:%d,parity:%d,dataBit:%d,stopBit:%d\n",device.matainConfig.portNum,device.matainConfig.baudRate,device.matainConfig.parity,device.matainConfig.dataBit,device.matainConfig.stopBit);
	  PRINT("RS422 CONFIG port:%d,baudRate:%d,parity:%d,dataBit:%d,stopBit:%d\n",device.RS422Config.portNum,device.RS422Config.baudRate,device.RS422Config.parity,device.RS422Config.dataBit,device.RS422Config.stopBit);  
	  PRINT("Relay CONFIG port:%d,baudRate:%d,parity:%d,dataBit:%d,stopBit:%d\n",device.relayConfig.portNum,device.relayConfig.baudRate,device.relayConfig.parity,device.relayConfig.dataBit,device.relayConfig.stopBit); 
#endif	 

 	 //---------------------ά��-------------------------- 
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_MATAIN,&tempPanel);//��ȡ�����  
	 
	 GetIndexFromValue(tempPanel,MATAIN_PORTNUM,&i,device.matainConfig.portNum);
	 SetCtrlIndex(tempPanel,MATAIN_PORTNUM,i);//���ں�
 	 
	 GetIndexFromValue(tempPanel,MATAIN_RATE,&i,device.matainConfig.baudRate);
	 SetCtrlIndex(tempPanel,MATAIN_RATE,i);//������ 
 	 
	 GetIndexFromValue(tempPanel,MATAIN_PARITY,&i,device.matainConfig.parity);
	 SetCtrlIndex(tempPanel,MATAIN_PARITY,i); //��żУ��   
 	 
	 GetIndexFromValue(tempPanel,MATAIN_DATAB,&i,device.matainConfig.dataBit);
	 SetCtrlIndex(tempPanel,MATAIN_DATAB,i);//����λ 
 	 
	 GetIndexFromValue(tempPanel,MATAIN_STOPB,&i,device.matainConfig.stopBit);
	 SetCtrlIndex(tempPanel,MATAIN_STOPB,i);//ֹͣλ 
	 
	 
	 //---------------------422--------------------------  
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_RS422,&tempPanel); //��ȡ�����  
	 GetIndexFromValue(tempPanel,RS422_PORTNUM,&i,device.RS422Config.portNum);
	 SetCtrlIndex(tempPanel,RS422_PORTNUM,i);//���ں�  
 	 
	 GetIndexFromValue(tempPanel,RS422_RATE,&i,device.RS422Config.baudRate);
	 SetCtrlIndex(tempPanel,RS422_RATE,i);//������  
 	 
	 GetIndexFromValue(tempPanel,RS422_PARITY,&i,device.RS422Config.parity);
	 SetCtrlIndex(tempPanel,RS422_PARITY,i); //��żУ�� 
 	 
	 GetIndexFromValue(tempPanel,RS422_DATAB,&i,device.RS422Config.dataBit);
	 SetCtrlIndex(tempPanel,RS422_DATAB,i); //����λ
 	 
	 GetIndexFromValue(tempPanel,RS422_STOPB,&i,device.RS422Config.stopBit);
	 SetCtrlIndex(tempPanel,RS422_STOPB,i);//ֹͣλ 

	 
	  //---------------------429--------------------------
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_ARINC429,&tempPanel);   //��ȡ�����  
	 GetIndexFromValue(tempPanel,ARINC429_RING429CARD,&i,device.arinc429Config.card);
	 SetCtrlIndex(tempPanel,ARINC429_RING429CARD,i); //ͨ���� 
	 GetIndexFromValue(tempPanel,ARINC429_RING429,&i,device.arinc429Config.ch);
	 SetCtrlIndex(tempPanel,ARINC429_RING429,i); //ͨ���� 
	 
	 
	 //---------------------Relay--------------------------  
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_RELAY,&tempPanel); //��ȡ�����  
	 GetIndexFromValue(tempPanel,RELAY_PORTNUM,&i,device.relayConfig.portNum);
	 SetCtrlIndex(tempPanel,RELAY_PORTNUM,i);//���ں�  
 	 
	 GetIndexFromValue(tempPanel,RELAY_RATE,&i,device.relayConfig.baudRate);
	 SetCtrlIndex(tempPanel,RELAY_RATE,i);//������  
 	 
	 GetIndexFromValue(tempPanel,RELAY_PARITY,&i,device.relayConfig.parity);
	 SetCtrlIndex(tempPanel,RELAY_PARITY,i); //��żУ�� 
 	 
	 GetIndexFromValue(tempPanel,RELAY_DATAB,&i,device.relayConfig.dataBit);
	 SetCtrlIndex(tempPanel,RELAY_DATAB,i); //����λ
 	 
	 GetIndexFromValue(tempPanel,RELAY_STOPB,&i,device.relayConfig.stopBit);
	 SetCtrlIndex(tempPanel,RELAY_STOPB,i);//ֹͣλ
	 
}


static void RefreshEutConfigPanel()
{
	ClearListCtrl(eutConfigPanelHandle,EUTPANEL_EUT_TREE); //����ؼ�����
	ShowEutListByTree(eutList); //���ؼ�
}



static void DimEutConfigPanelControl(int panel,int dim)
{

	 int tempPanel;
	 //---------------------��������--------------------------
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_BASIC,&tempPanel);
	 SetCtrlAttribute(tempPanel,BASIC_EUTNAME,ATTR_DIMMED,dim); 
	 SetCtrlAttribute(tempPanel,BASIC_EUTID,ATTR_DIMMED,dim);
	 SetCtrlAttribute(tempPanel,BASIC_ENABLECHECKBOX,ATTR_DIMMED,dim); 
	 
	 
	 //---------------------ά��--------------------------   
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_MATAIN,&tempPanel); 
	 SetCtrlAttribute(tempPanel,MATAIN_PORTNUM,ATTR_DIMMED,dim);
	 SetCtrlAttribute(tempPanel,MATAIN_RATE,ATTR_DIMMED,dim);  
	 SetCtrlAttribute(tempPanel,MATAIN_PARITY,ATTR_DIMMED,dim);  
	 SetCtrlAttribute(tempPanel,MATAIN_DATAB,ATTR_DIMMED,dim);  
     SetCtrlAttribute(tempPanel,MATAIN_STOPB,ATTR_DIMMED,dim); 
	 
	 //---------------------422--------------------------   
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_RS422,&tempPanel);
	 SetCtrlAttribute(tempPanel,RS422_PORTNUM,ATTR_DIMMED,dim);
	 SetCtrlAttribute(tempPanel,RS422_RATE,ATTR_DIMMED,dim);  
	 SetCtrlAttribute(tempPanel,RS422_PARITY,ATTR_DIMMED,dim);  
	 SetCtrlAttribute(tempPanel,RS422_DATAB,ATTR_DIMMED,dim);  
     SetCtrlAttribute(tempPanel,RS422_STOPB,ATTR_DIMMED,dim);
	 
	  //---------------------429-------------------------- 
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_ARINC429,&tempPanel);
	 SetCtrlAttribute(tempPanel,ARINC429_RING429CARD,ATTR_DIMMED,dim); 
	 SetCtrlAttribute(tempPanel,ARINC429_RING429,ATTR_DIMMED,dim); 
	 
	 //---------------------Relay--------------------------   
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_RELAY,&tempPanel);
	 SetCtrlAttribute(tempPanel,RELAY_PORTNUM,ATTR_DIMMED,dim);
	 SetCtrlAttribute(tempPanel,RELAY_RATE,ATTR_DIMMED,dim);  
	 SetCtrlAttribute(tempPanel,RELAY_PARITY,ATTR_DIMMED,dim);  
	 SetCtrlAttribute(tempPanel,RELAY_DATAB,ATTR_DIMMED,dim);  
     SetCtrlAttribute(tempPanel,RELAY_STOPB,ATTR_DIMMED,dim);
	 
}

int CVICALLBACK OnAddEut(int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
	        isAddAction=1;
	        int testPanel;
        	testPanel=LoadPanel(0,"EutPanel.uir",EUTCONFIG);
			SetCtrlAttribute(testPanel,EUTCONFIG_TAB,ATTR_TABS_VISIBLE,0); 
	        DimEutConfigPanelControl(testPanel,0);
	        DisplayPanel(testPanel); 
			break;
	}
	return 0;
}


int CVICALLBACK OnEutRemove (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int selectedIndex;  
	switch (event)
	{
		case EVENT_COMMIT:
			EUT device;  
			GetActiveTreeItem(eutConfigPanelHandle,EUTPANEL_EUT_TREE,&selectedIndex);
	        //UUTListRemoveItem(selectedIndex);
	        ListRemoveItem(eutList,&device,selectedIndex+1);
			RefreshEutConfigPanel();
			break;
	}
	return 0;
}

int CVICALLBACK TREESELECTED (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int selectedIndex;
	char temp[30]={0};
	switch(event)
	{
		case EVENT_LEFT_CLICK:
			GetActiveTreeItem(eutConfigPanelHandle,EUTPANEL_EUT_TREE,&selectedIndex); 
			break;
	    case EVENT_VAL_CHANGED:
			GetActiveTreeItem(eutConfigPanelHandle,EUTPANEL_EUT_TREE,&selectedIndex);  
			break;
		case EVENT_LEFT_DOUBLE_CLICK:
			GetActiveTreeItem(eutConfigPanelHandle,EUTPANEL_EUT_TREE,&selectedIndex);
			currentEutIndex=selectedIndex+1;
	        int testPanel;
	        testPanel=LoadPanel(0,"EutPanel.uir",EUTCONFIG);
	        DimEutConfigPanelControl(testPanel,1);
			SetCtrlAttribute(testPanel,EUTCONFIG_TAB,ATTR_TABS_VISIBLE,0);  
	        ShowEutDetail(testPanel,currentEutIndex);
        	Fmt(temp,"EUT%d����",currentEutIndex);
	        //SetCtrlAttribute(testPanel,EUTCONFIG,ATTR_TITLE,temp);
			SetPanelAttribute (testPanel,ATTR_TITLE,temp);
	        DisplayPanel(testPanel);
	        SetCtrlAttribute (testPanel, EUTCONFIG_SURE, ATTR_LABEL_TEXT ,"�޸�"); 
			break;
	}	
    return 0;
}

static EUT GetEutInfo(int panelHandle)
{
    EUT device;
	char temp[100]={0};
	int selectedIndex=0;
	
	int tempPanel=0;
	//---------------------��������--------------------------    
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_BASIC,&tempPanel);
	GetCtrlVal(tempPanel,BASIC_EUTNAME,temp);
	memset(&device,0,sizeof(EUT));
	//device.deviceName=(char *)malloc(strlen(temp)+1);
	sprintf(device.eutName,"%s",temp); //����
	
	memset(temp,0,100);
	GetCtrlVal(tempPanel,BASIC_EUTID,temp);		//�豸Id
	StrToInt(temp,&device.index);
	
    GetCtrlVal(tempPanel,BASIC_ENABLECHECKBOX,&selectedIndex);   //�豸�Ƿ�������
	device.enable=selectedIndex;
	
	//---------------------ά��-------------------------------
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_MATAIN,&tempPanel);   
	GetCtrlIndex(tempPanel,MATAIN_PORTNUM,&selectedIndex); //���ں� 
	GetValueFromIndex(tempPanel,MATAIN_PORTNUM,selectedIndex,&device.matainConfig.portNum);
	GetCtrlIndex(tempPanel,MATAIN_RATE,&selectedIndex); //������
	GetValueFromIndex(tempPanel,MATAIN_RATE,selectedIndex,&device.matainConfig.baudRate);   
	GetCtrlIndex(tempPanel,MATAIN_PARITY,&selectedIndex);//У�� 
	GetValueFromIndex(tempPanel,MATAIN_PARITY,selectedIndex,&device.matainConfig.parity);   
	GetCtrlIndex(tempPanel,MATAIN_DATAB,&selectedIndex);//����λ 
	GetValueFromIndex(tempPanel,MATAIN_DATAB,selectedIndex,&device.matainConfig.dataBit);   
	GetCtrlIndex(tempPanel,MATAIN_STOPB,&selectedIndex); //ֹͣλ 
	GetValueFromIndex(tempPanel,MATAIN_STOPB,selectedIndex,&device.matainConfig.stopBit); 
	//printf("%d,%d,%d,%d,%d\n",device.matainConfig.portNum,device.matainConfig.baudRate,device.matainConfig.parity,device.matainConfig.dataBit,device.matainConfig.stopBit);
	
	//---------------------422------------------------------- 
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_RS422,&tempPanel);
	GetCtrlIndex(tempPanel,RS422_PORTNUM,&selectedIndex);//���ں�
	GetValueFromIndex(tempPanel,RS422_PORTNUM,selectedIndex,&device.RS422Config.portNum);
	GetCtrlIndex(tempPanel,RS422_RATE,&selectedIndex);//������ 
	GetValueFromIndex(tempPanel,RS422_RATE,selectedIndex,&device.RS422Config.baudRate);   
	GetCtrlIndex(tempPanel,RS422_PARITY,&selectedIndex); //У�� 
	GetValueFromIndex(tempPanel,RS422_PARITY,selectedIndex,&device.RS422Config.parity);   
	GetCtrlIndex(tempPanel,RS422_DATAB,&selectedIndex);//����λ  
	GetValueFromIndex(tempPanel,RS422_DATAB,selectedIndex,&device.RS422Config.dataBit);   
	GetCtrlIndex(tempPanel,RS422_STOPB,&selectedIndex);//ֹͣλ
	GetValueFromIndex(tempPanel,RS422_STOPB,selectedIndex,&device.RS422Config.stopBit); 
	//printf("%d,%d,%d,%d,%d\n",device.RS422Config.portNum,device.RS422Config.baudRate,device.RS422Config.parity,device.RS422Config.dataBit,device.RS422Config.stopBit); 
	//---------------------429--------------------------------
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_ARINC429,&tempPanel);
	GetCtrlIndex(tempPanel,ARINC429_RING429CARD,&selectedIndex); //�忨��
	GetValueFromIndex(tempPanel,ARINC429_RING429CARD,selectedIndex,&device.arinc429Config.card);
	GetCtrlIndex(tempPanel,ARINC429_RING429,&selectedIndex); //ͨ����
	GetValueFromIndex(tempPanel,ARINC429_RING429,selectedIndex,&device.arinc429Config.ch);
	
	//---------------------RELAY------------------------------- 
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_RELAY,&tempPanel);
	GetCtrlIndex(tempPanel,RELAY_PORTNUM,&selectedIndex);//���ں�
	GetValueFromIndex(tempPanel,RELAY_PORTNUM,selectedIndex,&device.relayConfig.portNum);
	GetCtrlIndex(tempPanel,RELAY_RATE,&selectedIndex);//������ 
	GetValueFromIndex(tempPanel,RELAY_RATE,selectedIndex,&device.relayConfig.baudRate);   
	GetCtrlIndex(tempPanel,RELAY_PARITY,&selectedIndex); //У�� 
	GetValueFromIndex(tempPanel,RELAY_PARITY,selectedIndex,&device.relayConfig.parity);   
	GetCtrlIndex(tempPanel,RELAY_DATAB,&selectedIndex);//����λ  
	GetValueFromIndex(tempPanel,RELAY_DATAB,selectedIndex,&device.relayConfig.dataBit);   
	GetCtrlIndex(tempPanel,RELAY_STOPB,&selectedIndex);//ֹͣλ
	GetValueFromIndex(tempPanel,RELAY_STOPB,selectedIndex,&device.relayConfig.stopBit); 
	//printf("%d,%d,%d,%d,%d\n",device.RS422Config.portNum,device.RS422Config.baudRate,device.RS422Config.parity,device.RS422Config.dataBit,device.RS422Config.stopBit); 	
#ifdef EUT_CONFIG_DEBUG
	  PRINT("BASIC ID:%d,name:%s\n",device.index,device.eutName);
	  PRINT("MATAIN CONFIG port:%d,baudRate:%d,parity:%d,dataBit:%d,stopBit:%d\n",device.matainConfig.portNum,device.matainConfig.baudRate,device.matainConfig.parity,device.matainConfig.dataBit,device.matainConfig.stopBit);
	  PRINT("RS422 CONFIG port:%d,baudRate:%d,parity:%d,dataBit:%d,stopBit:%d\n",device.RS422Config.portNum,device.RS422Config.baudRate,device.RS422Config.parity,device.RS422Config.dataBit,device.RS422Config.stopBit);  
	  PRINT("Relay CONFIG port:%d,baudRate:%d,parity:%d,dataBit:%d,stopBit:%d\n",device.relayConfig.portNum,device.relayConfig.baudRate,device.relayConfig.parity,device.relayConfig.dataBit,device.relayConfig.stopBit); 
#endif		
    return device;
}


static void ModifyEut(int panelHandle,int index) 
{
	EUT device; 
	device =GetEutInfo(panelHandle);
	ListReplaceItem(eutList,&device,index);
	RefreshEutConfigPanel();
}


static void AddEut(int panelHandle) 
{
	EUT device; 
	device =GetEutInfo(panelHandle);  
	ListInsertItem(eutList,&device,END_OF_LIST);
	RefreshEutConfigPanel();
}




int CVICALLBACK EUTCONFIG_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	
	char temp[100]={0}; 
	switch(event)
	{
	     case EVENT_COMMIT:
			 if(control==EUTCONFIG_SURE)
			 {
			   GetCtrlAttribute (panel, EUTCONFIG_SURE, ATTR_LABEL_TEXT ,temp); 
			   if(strcmp(temp,"ȷ��")==0)
			   {
				    
				   if(isAddAction==0)
				   {	   
			          ModifyEut(panel,currentEutIndex);
					  DiscardPanel(panel);
				   }else
				   {
				      AddEut(panel);
					  DiscardPanel(panel); 
				   }	   
			   }else if(strcmp(temp,"�޸�")==0)
			   {
				   DimEutConfigPanelControl(panel,0); 
			   	   SetCtrlAttribute (panel, EUTCONFIG_SURE, ATTR_LABEL_TEXT ,"ȷ��");
				   isAddAction=0;
			   }			 
			 }else if(control==EUTCONFIG_CANCEL)
			 {
			    DiscardPanel(panel); 
			 }	 
			 break;
		 case EVENT_VAL_CHANGED:
			 if(control==EUTCONFIG_TREE)
			 {
			     int selectedIndex=0;
		         GetActiveTreeItem(panel,EUTCONFIG_TREE,&selectedIndex); 
		         SetActiveTabPage (panel, EUTCONFIG_TAB,selectedIndex);
			 }	 
			 break;
	}
	return 0;
/*	if(control==EUTCONFIG_TREE)
	{	
	   switch (event)
	   {
		  case EVENT_COMMIT:

			 break;
		  case EVENT_VAL_CHANGED:
		     int selectedIndex=0;
		     GetActiveTreeItem(panel,EUTCONFIG_TREE,&selectedIndex); 
		     SetActiveTabPage (panel, EUTCONFIG_TAB,selectedIndex);
			
			break;
	   }
	   return 0;
	}else if(control==EUTCONFIG_SURE){
		switch(event)
		{
		   case EVENT_COMMIT:
			   
			   GetCtrlAttribute (panel, EUTCONFIG_SURE, ATTR_LABEL_TEXT ,temp); 
			   if(strcmp(temp,"ȷ��")==0)
			   {
				    
				   if(isAddAction==0)
				   {	   
			          ModifyEut(panel,currentEutIndex);
					  DiscardPanel(panel);
				   }else
				   {
				      AddEut(panel);
					  DiscardPanel(panel); 
				   }	   
			   }else if(strcmp(temp,"�޸�")==0)
			   {
				   DimEutConfigPanelControl(panel,0); 
			   	   SetCtrlAttribute (panel, EUTCONFIG_SURE, ATTR_LABEL_TEXT ,"ȷ��");
				   isAddAction=0;
			   }	   
			   break;
		
		}	
		return 0;
	}else if(control==EUTCONFIG_CANCEL){

		switch(event)
		{
		   case EVENT_COMMIT:
			   DiscardPanel(panel);  
			   break;
		
		}	
		return 0;
	} 
	return 0;*/
}

ListType getEutList(void)
{
     return eutList;
}

int CVICALLBACK OnSaveEut (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			saveEutListToXmlFile(eutList);
			break;
	}
	return 0;
}
