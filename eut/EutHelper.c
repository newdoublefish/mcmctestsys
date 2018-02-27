#include "EutManage.h"
#include "EutHelper.h"   
#include "toolbox.h"
#include <ansi_c.h>

BOOL getSerialConfig(ListType list,char *tagName,RSCONFIG *config)
{
	for(int i=1;i<=ListNumItems(list);i++)
	{
		ConfigItem configItem={0};
		ListGetItem(list,&configItem,i);
		if(strcmp(configItem.tagName,tagName)==0)
		{
			RSCONFIG *rsconfig=(RSCONFIG *)configItem.configPtrHandle;
			memcpy(config,rsconfig,sizeof(RSCONFIG));
			return TRUE;
		}
	}
	return FALSE;
}

BOOL getNetConfig(ListType list,char *tagName,NETCONFIG *config)
{
	for(int i=1;i<=ListNumItems(list);i++)
	{
		ConfigItem configItem={0};
		ListGetItem(list,&configItem,i);
		if(strcmp(configItem.tagName,tagName)==0)
		{
			NETCONFIG *rsconfig=(NETCONFIG *)configItem.configPtrHandle;
			memcpy(config,rsconfig,sizeof(NETCONFIG));
			return TRUE;
		}
	}
	return FALSE;
}
