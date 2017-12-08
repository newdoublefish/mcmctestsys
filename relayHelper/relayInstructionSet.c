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
#include <userint.h>
#include "excelHelper.h" 
#include "sutCommon.h"
#include "relayHelper.h"
#include "convertTool.h"
#include "relayPanel.h"
#include "eutConfig.h"

typedef struct
{
    char name[30];
	RELAY_OPERATION operation;
}INSTRUCTION;

static ListType istList=0; 

#define SHEET_RANGE_INSTRUCTION "A2:D2"


static INSTRUCTION tempInstruction;

static int relayPanel=0;

static EUT currentEut;
static int currentTabHandle;

static HRESULT onCellListenerGetInstruction(VARIANT *MyVariant,int row,int column)    
{
	char *temp; 
	if(column==0)
	{
		 memset(&tempInstruction,0,sizeof(INSTRUCTION));
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
			memcpy(tempInstruction.name,temp,strlen(temp)+1);
			CA_FreeMemory(temp);
		 }else
		 {
			return EXCEL_TASK_QUIT;
		 }
	}else if(column==1)
	{
		 if(CA_VariantHasCString(MyVariant))
	     {
		    CA_VariantGetCString(MyVariant, &temp); 
		    tempInstruction.operation.testType=(unsigned char)String2UnsignedLong(temp);   
		    CA_FreeMemory(temp);
		 }else
		 {
			tempInstruction.operation.testType=0;//无效
		 }	
		
	}else if(column==2)
	{
		     if(CA_VariantHasCString(MyVariant))
	         {
				 CA_VariantGetCString(MyVariant, &temp); 
				 tempInstruction.operation.testValue=(unsigned char)String2UnsignedLong(temp);
				 CA_FreeMemory(temp);
			 }else
			 {
			     tempInstruction.operation.testValue=0;
			 }	   
	}else if(column==3)
	{
		     if(CA_VariantHasCString(MyVariant))
	         {
				 CA_VariantGetCString(MyVariant, &temp);
				 
				 tempInstruction.operation.data=(unsigned short)String2UnsignedLong(temp); 
				 CA_FreeMemory(temp);
			 }else
			 {
			     tempInstruction.operation.data=0; 
			 }
			 ListInsertItem(istList,&tempInstruction,END_OF_LIST);
	}	
	return 0;
}


static HRESULT onStartGetInstruction(VARIANT *MyVariant,int row,int column)				
{
	istList=ListCreate(sizeof(INSTRUCTION));
    return 0;	
}

static HRESULT getInstructionSetFromExcel(char *sheetName)   
{
	
	SUT sut=GetSeletedSut();
	EXCELTask task=createExcelTask(sut.configPath,sheetName,SHEET_RANGE_INSTRUCTION,4);
	task.onExcelTaskStartListener=(void *)onStartGetInstruction;
	task.onCellListener=(void *)onCellListenerGetInstruction;
	return runExcelTask(task);

}

void DisplayInstructionPanel()
{
   
   if((relayPanel = LoadPanel (0, "relayPanel.uir", PANEL)) <= 0)		   //状态监视面板
   {
	  return ; 
   } 
   
   //ATTR_TITLE
   //SetPanelAttribute(relayPanel,ARRT_TITLE,"继电器测试");
   
   if(istList==0)
       getInstructionSetFromExcel("继电器指令集");
   //InsertPanelAsTabPage(relayPanel,PANEL_TAB,0,panelHandle);
   //SetTabPageAttribute (panelStateMonitor,PANEL_STAT_TAB_STAT,i-1 ,ATTR_LABEL_TEXT, eut.eutName);
   ListType eutList=getEutList();
   for(int i=0;i<ListNumItems(eutList);i++)
   {
       EUT eut;
	   ListGetItem(eutList,&eut,i+1);
	   int panelHandle=LoadPanel(0,"relayPanel.uir",PANEL_2);
	   char temp[100]={0};
	   sprintf(temp,"%d",eut.index);
	   SetCtrlVal(panelHandle,PANEL_2_TEXTMSG_ID,temp);
	   
	   InsertTableRows (panelHandle,PANEL_2_TABLE,-1,ListNumItems(istList),VAL_USE_MASTER_CELL_TYPE); 
	   
	   for(int j=1;j<=ListNumItems(istList);j++)
	   {
		   
		   Point p;
		   INSTRUCTION instruction;
		   ListGetItem(istList,&instruction,j);
		   p=MakePoint(1,j);
		   SetTableCellVal(panelHandle,PANEL_2_TABLE,p,instruction.name);
		   
		   p=MakePoint(2,j);
		   memset(temp,0,100);
		   sprintf(temp,"0x%x",instruction.operation.testType);
		   SetTableCellVal(panelHandle,PANEL_2_TABLE,p,temp);
		   
		   p=MakePoint(3,j);
		   memset(temp,0,100);
		   sprintf(temp,"0x%x",instruction.operation.testValue);
		   SetTableCellVal(panelHandle,PANEL_2_TABLE,p,temp);
		   
		   p=MakePoint(4,j);
		   memset(temp,0,100);
		   sprintf(temp,"0x%x",instruction.operation.data);
		   SetTableCellVal(panelHandle,PANEL_2_TABLE,p,temp);	
		   
		   p=MakePoint(5,j);
		   SetTableCellVal(panelHandle,PANEL_2_TABLE,p,"测试");
           	   
	   }
	   
	   InsertPanelAsTabPage(relayPanel,PANEL_TAB,i,panelHandle);
	   sprintf(temp,"设备%d,%s",eut.index,eut.eutName);
	   SetTabPageAttribute (relayPanel,PANEL_TAB,i ,ATTR_LABEL_TEXT, temp);
   }	   
   
   DisplayPanel(relayPanel);
}

int CVICALLBACK relayPanelQuitCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	switch (event)
	{
		case EVENT_COMMIT:
			DiscardPanel(relayPanel);
			relayPanel=0;
			break;
	}
	return 0;
}

void testInstruction(int panelHandle,INSTRUCTION instruction,int row)
{
	char temp[100]={0};
	BOOL ret;
	RelayBuf sndBuf;
	RelayBuf recvBuf;
	BOOL isValue=TRUE;
	if(instruction.operation.testType==1 || instruction.operation.testType==2 )
	{
	    isValue=TRUE;
	}else
	{
	    isValue=FALSE;
	}
	ret=SendRelayOperationWithRelayBuf(currentEut,instruction.operation,isValue,&sndBuf,&recvBuf);
	//SendRelayOperationWithRelayBuf(EUT eut,RELAY_OPERATION operation,BOOL isValue,RelayBuf *sndBuf,RelayBuf *rcvBuf);  
	sprintf(temp,"正在测试%s:\n",instruction.name);
	SetCtrlVal(relayPanel,PANEL_TEXTBOX,temp);
	
	//SetCtrlVal("发送:");
	SetCtrlVal(relayPanel,PANEL_TEXTBOX,"发送:");
	for(int i=0;i<sndBuf.len;i++)
	{
	     //SetCtrlVal("%2x ",sndBuf.data[i]);
		char sndStr[10]={0};
		sprintf(sndStr," 0x%x",sndBuf.data[i]);
		SetCtrlVal(relayPanel,PANEL_TEXTBOX,sndStr);
	}
	SetCtrlVal(relayPanel,PANEL_TEXTBOX,"\n");
	
	SetCtrlVal(relayPanel,PANEL_TEXTBOX,"接收:");
	for(int i=0;i<recvBuf.len;i++)
	{
		char recvStr[10]={0};
		sprintf(recvStr," 0x%x",recvBuf.data[i]);
		SetCtrlVal(relayPanel,PANEL_TEXTBOX,recvStr);
	}
	SetCtrlVal(relayPanel,PANEL_TEXTBOX,"\n"); 
	Point p;
	p=MakePoint(6,row); 
	if(ret==TRUE)
	{
	    
		SetTableCellVal(panelHandle,PANEL_2_TABLE,p,"通过");
		SetCtrlVal(relayPanel,PANEL_TEXTBOX,"测试成功\n");
		
	}else
	{
		SetTableCellVal(panelHandle,PANEL_2_TABLE,p,"未通过"); 
		SetCtrlVal(relayPanel,PANEL_TEXTBOX,"测试失败\n"); 
	}	
	
	
	
}

int CVICALLBACK TB_CB (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
    Point focus;
    
    switch (event)
    {
        case EVENT_COMMIT:
            
            GetActiveTableCell(panel, control, &focus);
            
            if (focus.x == 5)
			{	
				INSTRUCTION instruction;
				//printf("%d,%d\n",focus.x,focus.y);
				ListGetItem(istList,&instruction,focus.y);
				//printf("%x,%x,%x\n",instruction.operation.testType,instruction.operation.testValue,instruction.operation.data); 
				
				testInstruction(panel,instruction,focus.y);
                //TestChannel(panel, control, focus.y);
			}
			
            
            break;
    }
    return 0;
}

int CVICALLBACK TAB_CALLBACK (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	int tabIndex=0;  
	switch (event)
	{
		case EVENT_ACTIVE_TAB_CHANGE:
			
			GetActiveTabPage(panel,PANEL_TAB,&tabIndex);
			//printf("tablIndex:%d\n",tabIndex);
			ListType eutList=getEutList(); 
			ListGetItem(eutList,&currentEut,tabIndex+1);
			GetPanelHandleFromTabPage(panel,PANEL_TAB,tabIndex,&currentTabHandle) ;
			break;
	}
	return 0;
}

int testFlag=0;

static int testAllInstruction(void *unused) 
{
   for(int i=1;i<=ListNumItems(istList);i++)
   {
	    INSTRUCTION instruction;
		ListGetItem(istList,&instruction,i);
		testInstruction(currentTabHandle,instruction,i);
		Delay(0.2);
		if(testFlag!=1)
		{
			break;
		}
   }
   MessagePopup("继电器测试","测试完成");
   return 0;
}


int CVICALLBACK testAllCallback (int panel, int control, int event,
		void *callbackData, int eventData1, int eventData2)
{
	char temp[30]={0}; 
	switch (event)
	{
		case EVENT_COMMIT:
			
			GetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,temp); 
			if(strcmp(temp,"全部测试")==0)
			{
			   int funcId;
			   testFlag=1;
			   CmtScheduleThreadPoolFunctionAdv (DEFAULT_THREAD_POOL_HANDLE,
                                              testAllInstruction,
                                              NULL, THREAD_PRIORITY_NORMAL,
                                              NULL,
                                              EVENT_TP_THREAD_FUNCTION_END, NULL,
                                              CmtGetCurrentThreadID(),&funcId);  
			   SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"停止");
			}else
			{
			   testFlag=0;
			   SetCtrlAttribute(panel,control,ATTR_LABEL_TEXT,"全部测试"); 
			}

			break;
	}
	return 0;
}
