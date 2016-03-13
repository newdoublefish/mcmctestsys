/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：protocolHelper.c
 // 文件功能描述:协议解析
 //
 // 
 // 创建标识：曾衍仁 20150803
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
/*
   if(strcmp(group.type,"422信号")==0)
   {
	   ret=add422Protocol(group.groupName);
   }else if(strcmp(group.type,"429信号")==0)
   {
	   ret=add429Protocol(group.groupName);
   }else if(strcmp(group.type,"振动信号")==0)
   {
	   ret=addFileProtocol(group.groupName);
   }	   
   else if(strcmp(group.type,"模拟信号")==0) 
   {
	   ret=addSimuProtocol(group.groupName);
   }else if(strcmp(group.type,"离散信号")==0)
   {
       ret=addDispProtocol(group.groupName);
   }else if(strcmp(group.type,"飞参记录")==0)
   {
       ret = addFlyParamProtocol(group.groupName);
   }else if(strcmp(group.type,"8路输入输出")==0)
   {
       ret=add8InputOutputProtocol(group.groupName);
   }
*/   
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
