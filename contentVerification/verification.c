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
