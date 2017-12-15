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
#include <formatio.h>
#include "toolbox.h"
#include <ansi_c.h>
#include <cvirte.h>		
#include <userint.h>
#include "testGroupInit.h"
#include "common.h"
#include "tpsHelper.h"

static BOOL parseProtocol(TestGroup group)
{
   	
   BOOL ret=FALSE;
   TPS tps;
   if(getTps(group.type,&tps))
   {
       if(tps.protocolInit!=NULL)
	   {
	   		ret=(*(PROTOCOL_INIT)(tps.protocolInit))(group.groupName);
	   }
   }
   
   return ret;
}


int protocolInit(void)
{
	int ret=0;
	ListType list=getItemList();
	char string[100]={0};
	char description[100]={0};

	int slideHandle=displaySlideProgressWithTextBox("协议解析");
	int itemCount=ListNumItems(list);
	for(int i=1;i<=itemCount;i++)
	{
		TestGroup group; 
		ListGetItem(list,&group,i);
		 if(parseProtocol(group)<0)
		 {
			  ret=-1;
			  memset(string,0,100);
			  Fmt(string,"协议%s分析失败",group.groupName);
			  WarnShow(string);
		      break;
		 } 	 
		 int perc;
         perc=i*100/itemCount;
		 memset(description,0,100);
		 Fmt(description,"%s     %s\n",group.groupName,"ok");
		 slideProgressShowWithTextBox(slideHandle,perc,description,0.01);  
		 //Delay(0.1);
	}
	
	disposeSlideProgress(slideHandle); 
	return ret;
}

void protocolDInit(void)
{
	
}
