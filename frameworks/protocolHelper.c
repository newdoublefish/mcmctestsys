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
	slideProgressShowWithTextBox(slideHandle,0,"���ڷ���Э��",0.01);
#if 0 //TODO:�Ժ�Ľ�	
	/*int itemCount=ListNumItems(list);
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
	}*/
#endif	
	TpsInitProtocol();
	slideProgressShowWithTextBox(slideHandle,100,"Э��������",0.01);	
	disposeSlideProgress(slideHandle); 
	return ret;
}

void protocolDInit(void)
{
	
}
