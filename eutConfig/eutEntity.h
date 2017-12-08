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
#ifndef __EUTENTITY_H__
#define __EUTENTITY_H__
#include"eutConfig.h"

#ifdef __cplusplus
   extern "C" {
#endif	 
	   
#define MAX_EUTNAME_LEN 30	   

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
   int card;
   int ch;
}ARINC429CONFIG;


typedef struct
{
    int index;
	char eutName[MAX_EUTNAME_LEN];
	RSCONFIG matainConfig;
	RSCONFIG RS422Config; 
	//RSCONFIG relayConfig;
	//ARINC429CONFIG arinc429Config;
	//int arinc429Port;
	int enable;
} EUT;//Device under test	

	   
	   
ListType getEutListFromXmlFile(void);

void saveEutListToXmlFile(ListType list);  

#ifdef __cplusplus
   {
#endif	   

#endif
