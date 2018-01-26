#define MAX_KEY_LEN 20
#define MAX_VALUE_LEN 20

typedef struct
{
	char key[MAX_KEY_LEN];
	char value[MAX_VALUE_LEN];
}Map;

ListType createMapList(void);
void addToMapList(ListType list,Map map);
void disposeMapList(ListType list);
int getMap(ListType list,char *key,Map *mapPtr);
void addCharCharMap(ListType list,char *key,char *value);
void addCharIntMap(ListType list,char *key,int value);
