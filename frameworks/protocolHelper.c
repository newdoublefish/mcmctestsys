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
   if(strcmp(group.type,"422�ź�")==0)
   {
	   ret=add422Protocol(group.groupName);
   }else if(strcmp(group.type,"429�ź�")==0)
   {
	   ret=add429Protocol(group.groupName);
   }else if(strcmp(group.type,"���ź�")==0)
   {
	   ret=addFileProtocol(group.groupName);
   }	   
   else if(strcmp(group.type,"ģ���ź�")==0) 
   {
	   ret=addSimuProtocol(group.groupName);
   }else if(strcmp(group.type,"��ɢ�ź�")==0)
   {
       ret=addDispProtocol(group.groupName);
   }else if(strcmp(group.type,"�ɲμ�¼")==0)
   {
       ret = addFlyParamProtocol(group.groupName);
   }else if(strcmp(group.type,"8·�������")==0)
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

	int slideHandle=displaySlideProgressWithTextBox("Э�����");
	int itemCount=ListNumItems(list);
	for(int i=1;i<=itemCount;i++)
	{
		TestGroup group; 
		ListGetItem(list,&group,i);
		 if(parseProtocol(group)<0)
		 {
			  ret=-1;
			  memset(string,0,100);
			  Fmt(string,"Э��%s����ʧ��",group.groupName);
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
