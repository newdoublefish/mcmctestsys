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
#include "toolbox.h"
#include <ansi_c.h>
#include "verification.h"
BOOL verificateProtocolItem(TestGroup group,char *paramName)
{
	 BOOL ret=FALSE;
	 int num=ListNumItems(group.subItems);
     for(int i=0;i<=num;i++)
	 {
	    TestItem item;
		ListGetItem(group.subItems,&item,i);
		if(strcmp(item.itemName_,paramName)==0)
		{
			 //printf("itemName:%s\n",item.itemName_);
		     ret=TRUE;
			 break;
		}	
	 }	
	 return ret;
}
