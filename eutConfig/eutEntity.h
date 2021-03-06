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
#ifndef __EUTENTITY_H__
#define __EUTENTITY_H__
#include"eutConfig.h"

#ifdef __cplusplus
   extern "C" {
#endif	 
	   
#define MAX_EUTNAME_LEN 30
#define MAX_NET_IP_LEN 50

typedef struct
{
    int portNum;
    int baudRate;
    int parity;
    int dataBit;
    int stopBit;
}RSCONFIG;	

typedef struct
{
	char ip[MAX_NET_IP_LEN];
	int port;
}NETCONFIG;

typedef struct
{
	char eutName[20];
	int id;
	int enable;
}EutBasicInfo;

typedef struct
{
    int index;
	char eutName[MAX_EUTNAME_LEN];
	RSCONFIG matainConfig;
	RSCONFIG relayConfig;
	RSCONFIG bmsConfig; 
	NETCONFIG chargingPile;
	NETCONFIG testInstrument;
	int enable;
	ListType configList;
} EUT;//Device under test	

	   
	   
ListType getEutListFromXmlFile(void);

void saveEutListToXmlFile(ListType list);  

#ifdef __cplusplus
   {
#endif	   

#endif
