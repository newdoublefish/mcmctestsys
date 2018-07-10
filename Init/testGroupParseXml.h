typedef struct{
	char *itemName;
	HashTableType attributeHash;
}tProtocolItem;

typedef struct{
	char *protocolName;
	ListType protocolItems;
}tProtocol;
BOOL LoadTestGroupFromXml(ListType groupList,ListType itemList,ListType protocolList,char *filePath);
