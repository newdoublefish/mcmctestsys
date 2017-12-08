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

static int eutConfigPanelHandle;  //面板panel，有可能是外部传入
static int currentEutIndex=0;	   //当前选中的的EUT
static int isAddAction=0;		   //当前操作是否为增加UUT的操作

static ListType eutList;  //设备链表


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
		sprintf(temp,"编号：%d",eut.index);
	    InsertTreeItem (eutConfigPanelHandle, EUTPANEL_EUT_TREE, VAL_SIBLING,0, VAL_LAST,
                    temp,0,0,0);
	    SetTreeCellAttribute(eutConfigPanelHandle,EUTPANEL_EUT_TREE,eutIndex-1,1, ATTR_LABEL_TEXT,eut.eutName);
	}
	//显示第一个设备
	if(eutCount>0)
	  SetActiveTreeItem(eutConfigPanelHandle,EUTPANEL_EUT_TREE,0,VAL_REPLACE_SELECTION_WITH_ITEM);  	
}


void DisplayEutPanelAsTabPanel(int panel)
{

	eutConfigPanelHandle=LoadPanel(panel, "EutPanel.uir", EUTPANEL);

	DisplayPanel(eutConfigPanelHandle);//创建并显示面板
	eutList = getEutListFromXmlFile(); //从xml文件中获取eut链表
	ClearListCtrl(eutConfigPanelHandle,EUTPANEL_EUT_TREE);//清空面板显示空间   
	ShowEutListByTree(eutList);//显示eut链表
}

static void ShowEutDetail(int panel,int index)
{
	 EUT device; 
     int comPort=0;
	 char temp[32]={0};
	 int i;
	 int tempPanel;

	 ListGetItem(eutList,&device,index);

	 //---------------------基本配置--------------------------
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_BASIC,&tempPanel);//获取面板句柄
   	 SetCtrlVal(tempPanel,BASIC_EUTNAME,device.eutName);  //设备名称
	 sprintf(temp,"%d",device.index);
	 SetCtrlVal(tempPanel,BASIC_EUTID,temp);	//设备序号
	 
	 SetCtrlVal(tempPanel,BASIC_ENABLECHECKBOX,device.enable);
	 
#ifdef EUT_CONFIG_DEBUG
	  PRINT("MATAIN CONFIG port:%d,baudRate:%d,parity:%d,dataBit:%d,stopBit:%d\n",device.matainConfig.portNum,device.matainConfig.baudRate,device.matainConfig.parity,device.matainConfig.dataBit,device.matainConfig.stopBit);
	  PRINT("RS422 CONFIG port:%d,baudRate:%d,parity:%d,dataBit:%d,stopBit:%d\n",device.RS422Config.portNum,device.RS422Config.baudRate,device.RS422Config.parity,device.RS422Config.dataBit,device.RS422Config.stopBit);  
	  PRINT("Relay CONFIG port:%d,baudRate:%d,parity:%d,dataBit:%d,stopBit:%d\n",device.relayConfig.portNum,device.relayConfig.baudRate,device.relayConfig.parity,device.relayConfig.dataBit,device.relayConfig.stopBit); 
#endif	 

 	 //---------------------维护-------------------------- 
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_MATAIN,&tempPanel);//获取面板句柄  
	 
	 GetIndexFromValue(tempPanel,MATAIN_PORTNUM,&i,device.matainConfig.portNum);
	 SetCtrlIndex(tempPanel,MATAIN_PORTNUM,i);//串口号
 	 
	 GetIndexFromValue(tempPanel,MATAIN_RATE,&i,device.matainConfig.baudRate);
	 SetCtrlIndex(tempPanel,MATAIN_RATE,i);//波特率 
 	 
	 GetIndexFromValue(tempPanel,MATAIN_PARITY,&i,device.matainConfig.parity);
	 SetCtrlIndex(tempPanel,MATAIN_PARITY,i); //奇偶校验   
 	 
	 GetIndexFromValue(tempPanel,MATAIN_DATAB,&i,device.matainConfig.dataBit);
	 SetCtrlIndex(tempPanel,MATAIN_DATAB,i);//数据位 
 	 
	 GetIndexFromValue(tempPanel,MATAIN_STOPB,&i,device.matainConfig.stopBit);
	 SetCtrlIndex(tempPanel,MATAIN_STOPB,i);//停止位 
	 
	 
	 //---------------------422--------------------------  
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_RS422,&tempPanel); //获取面板句柄  
	 GetIndexFromValue(tempPanel,RS422_PORTNUM,&i,device.RS422Config.portNum);
	 SetCtrlIndex(tempPanel,RS422_PORTNUM,i);//串口号  
 	 
	 GetIndexFromValue(tempPanel,RS422_RATE,&i,device.RS422Config.baudRate);
	 SetCtrlIndex(tempPanel,RS422_RATE,i);//波特率  
 	 
	 GetIndexFromValue(tempPanel,RS422_PARITY,&i,device.RS422Config.parity);
	 SetCtrlIndex(tempPanel,RS422_PARITY,i); //奇偶校验 
 	 
	 GetIndexFromValue(tempPanel,RS422_DATAB,&i,device.RS422Config.dataBit);
	 SetCtrlIndex(tempPanel,RS422_DATAB,i); //数据位
 	 
	 GetIndexFromValue(tempPanel,RS422_STOPB,&i,device.RS422Config.stopBit);
	 SetCtrlIndex(tempPanel,RS422_STOPB,i);//停止位 

	 
	  //---------------------429--------------------------
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_ARINC429,&tempPanel);   //获取面板句柄  
	 GetIndexFromValue(tempPanel,ARINC429_RING429CARD,&i,device.arinc429Config.card);
	 SetCtrlIndex(tempPanel,ARINC429_RING429CARD,i); //通道号 
	 GetIndexFromValue(tempPanel,ARINC429_RING429,&i,device.arinc429Config.ch);
	 SetCtrlIndex(tempPanel,ARINC429_RING429,i); //通道号 
	 
	 
	 //---------------------Relay--------------------------  
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_RELAY,&tempPanel); //获取面板句柄  
	 GetIndexFromValue(tempPanel,RELAY_PORTNUM,&i,device.relayConfig.portNum);
	 SetCtrlIndex(tempPanel,RELAY_PORTNUM,i);//串口号  
 	 
	 GetIndexFromValue(tempPanel,RELAY_RATE,&i,device.relayConfig.baudRate);
	 SetCtrlIndex(tempPanel,RELAY_RATE,i);//波特率  
 	 
	 GetIndexFromValue(tempPanel,RELAY_PARITY,&i,device.relayConfig.parity);
	 SetCtrlIndex(tempPanel,RELAY_PARITY,i); //奇偶校验 
 	 
	 GetIndexFromValue(tempPanel,RELAY_DATAB,&i,device.relayConfig.dataBit);
	 SetCtrlIndex(tempPanel,RELAY_DATAB,i); //数据位
 	 
	 GetIndexFromValue(tempPanel,RELAY_STOPB,&i,device.relayConfig.stopBit);
	 SetCtrlIndex(tempPanel,RELAY_STOPB,i);//停止位
	 
}


static void RefreshEutConfigPanel()
{
	ClearListCtrl(eutConfigPanelHandle,EUTPANEL_EUT_TREE); //清除控件内容
	ShowEutListByTree(eutList); //填充控件
}



static void DimEutConfigPanelControl(int panel,int dim)
{

	 int tempPanel;
	 //---------------------基本配置--------------------------
	 GetPanelHandleFromTabPage(panel,EUTCONFIG_TAB,EUT_SETTING_TAB_BASIC,&tempPanel);
	 SetCtrlAttribute(tempPanel,BASIC_EUTNAME,ATTR_DIMMED,dim); 
	 SetCtrlAttribute(tempPanel,BASIC_EUTID,ATTR_DIMMED,dim);
	 SetCtrlAttribute(tempPanel,BASIC_ENABLECHECKBOX,ATTR_DIMMED,dim); 
	 
	 
	 //---------------------维护--------------------------   
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
        	Fmt(temp,"EUT%d设置",currentEutIndex);
	        //SetCtrlAttribute(testPanel,EUTCONFIG,ATTR_TITLE,temp);
			SetPanelAttribute (testPanel,ATTR_TITLE,temp);
	        DisplayPanel(testPanel);
	        SetCtrlAttribute (testPanel, EUTCONFIG_SURE, ATTR_LABEL_TEXT ,"修改"); 
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
	//---------------------基本配置--------------------------    
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_BASIC,&tempPanel);
	GetCtrlVal(tempPanel,BASIC_EUTNAME,temp);
	memset(&device,0,sizeof(EUT));
	//device.deviceName=(char *)malloc(strlen(temp)+1);
	sprintf(device.eutName,"%s",temp); //名字
	
	memset(temp,0,100);
	GetCtrlVal(tempPanel,BASIC_EUTID,temp);		//设备Id
	StrToInt(temp,&device.index);
	
    GetCtrlVal(tempPanel,BASIC_ENABLECHECKBOX,&selectedIndex);   //设备是否参与测试
	device.enable=selectedIndex;
	
	//---------------------维护-------------------------------
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_MATAIN,&tempPanel);   
	GetCtrlIndex(tempPanel,MATAIN_PORTNUM,&selectedIndex); //串口号 
	GetValueFromIndex(tempPanel,MATAIN_PORTNUM,selectedIndex,&device.matainConfig.portNum);
	GetCtrlIndex(tempPanel,MATAIN_RATE,&selectedIndex); //波特率
	GetValueFromIndex(tempPanel,MATAIN_RATE,selectedIndex,&device.matainConfig.baudRate);   
	GetCtrlIndex(tempPanel,MATAIN_PARITY,&selectedIndex);//校验 
	GetValueFromIndex(tempPanel,MATAIN_PARITY,selectedIndex,&device.matainConfig.parity);   
	GetCtrlIndex(tempPanel,MATAIN_DATAB,&selectedIndex);//数据位 
	GetValueFromIndex(tempPanel,MATAIN_DATAB,selectedIndex,&device.matainConfig.dataBit);   
	GetCtrlIndex(tempPanel,MATAIN_STOPB,&selectedIndex); //停止位 
	GetValueFromIndex(tempPanel,MATAIN_STOPB,selectedIndex,&device.matainConfig.stopBit); 
	//printf("%d,%d,%d,%d,%d\n",device.matainConfig.portNum,device.matainConfig.baudRate,device.matainConfig.parity,device.matainConfig.dataBit,device.matainConfig.stopBit);
	
	//---------------------422------------------------------- 
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_RS422,&tempPanel);
	GetCtrlIndex(tempPanel,RS422_PORTNUM,&selectedIndex);//串口号
	GetValueFromIndex(tempPanel,RS422_PORTNUM,selectedIndex,&device.RS422Config.portNum);
	GetCtrlIndex(tempPanel,RS422_RATE,&selectedIndex);//波特率 
	GetValueFromIndex(tempPanel,RS422_RATE,selectedIndex,&device.RS422Config.baudRate);   
	GetCtrlIndex(tempPanel,RS422_PARITY,&selectedIndex); //校验 
	GetValueFromIndex(tempPanel,RS422_PARITY,selectedIndex,&device.RS422Config.parity);   
	GetCtrlIndex(tempPanel,RS422_DATAB,&selectedIndex);//数据位  
	GetValueFromIndex(tempPanel,RS422_DATAB,selectedIndex,&device.RS422Config.dataBit);   
	GetCtrlIndex(tempPanel,RS422_STOPB,&selectedIndex);//停止位
	GetValueFromIndex(tempPanel,RS422_STOPB,selectedIndex,&device.RS422Config.stopBit); 
	//printf("%d,%d,%d,%d,%d\n",device.RS422Config.portNum,device.RS422Config.baudRate,device.RS422Config.parity,device.RS422Config.dataBit,device.RS422Config.stopBit); 
	//---------------------429--------------------------------
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_ARINC429,&tempPanel);
	GetCtrlIndex(tempPanel,ARINC429_RING429CARD,&selectedIndex); //板卡号
	GetValueFromIndex(tempPanel,ARINC429_RING429CARD,selectedIndex,&device.arinc429Config.card);
	GetCtrlIndex(tempPanel,ARINC429_RING429,&selectedIndex); //通道号
	GetValueFromIndex(tempPanel,ARINC429_RING429,selectedIndex,&device.arinc429Config.ch);
	
	//---------------------RELAY------------------------------- 
	GetPanelHandleFromTabPage(panelHandle,EUTCONFIG_TAB,EUT_SETTING_TAB_RELAY,&tempPanel);
	GetCtrlIndex(tempPanel,RELAY_PORTNUM,&selectedIndex);//串口号
	GetValueFromIndex(tempPanel,RELAY_PORTNUM,selectedIndex,&device.relayConfig.portNum);
	GetCtrlIndex(tempPanel,RELAY_RATE,&selectedIndex);//波特率 
	GetValueFromIndex(tempPanel,RELAY_RATE,selectedIndex,&device.relayConfig.baudRate);   
	GetCtrlIndex(tempPanel,RELAY_PARITY,&selectedIndex); //校验 
	GetValueFromIndex(tempPanel,RELAY_PARITY,selectedIndex,&device.relayConfig.parity);   
	GetCtrlIndex(tempPanel,RELAY_DATAB,&selectedIndex);//数据位  
	GetValueFromIndex(tempPanel,RELAY_DATAB,selectedIndex,&device.relayConfig.dataBit);   
	GetCtrlIndex(tempPanel,RELAY_STOPB,&selectedIndex);//停止位
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
			   if(strcmp(temp,"确定")==0)
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
			   }else if(strcmp(temp,"修改")==0)
			   {
				   DimEutConfigPanelControl(panel,0); 
			   	   SetCtrlAttribute (panel, EUTCONFIG_SURE, ATTR_LABEL_TEXT ,"确定");
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
			   if(strcmp(temp,"确定")==0)
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
			   }else if(strcmp(temp,"修改")==0)
			   {
				   DimEutConfigPanelControl(panel,0); 
			   	   SetCtrlAttribute (panel, EUTCONFIG_SURE, ATTR_LABEL_TEXT ,"确定");
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
