#include "toolbox.h"
#include "ListKeyValue.h"
ListType createMapList()
{
	return ListCreate(sizeof(Map));
}

void addToMapList(ListType list,Map map)
{
	ListInsertItem(list,&map,END_OF_LIST);
}

void addCharCharMap(ListType list,char *key,char *value)
{
	Map map={0};
	sprintf(map.key,"%s",key);
	sprintf(map.value,"%s",value);
	addToMapList(list,map);
	
}

void addCharIntMap(ListType list,char *key,int value)
{
	Map map={0};
	sprintf(map.key,"%s",key);
	sprintf(map.value,"%d",value);
	addToMapList(list,map);	
}

void disposeMapList(ListType list)
{
	ListDispose(list);
}

int getMap(ListType list,char *key,Map *mapPtr)
{
	int count=ListNumItems(list);
	for(int i=1;i<=count;i++)
	{
		ListGetItem(list,mapPtr,i);
		if(strcmp(mapPtr->key,key)==0)
		{
		  	return 1;
		}
	}
	return 0;
}
