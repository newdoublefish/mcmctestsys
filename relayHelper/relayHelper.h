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
#ifndef __RELAYHELPER_H__
#define __RELAYHELPER_H__
#include "eutConfig.h"   
typedef struct
{
   unsigned char testType;	   //��������
   unsigned char testValue;	   //����ֵ
   unsigned short data;		  //��������
}RELAY_OPERATION;

typedef struct
{
   unsigned short synHeader;  //ͬ��ͷ   0xAA55
   unsigned char sndDevice;   //�����豸
   unsigned char recvDevice;  //�����豸
   unsigned char testType;	   //��������
   unsigned char testValue;	   //����ֵ
   unsigned short data;		  //��������
   unsigned char checksum;	  //У����   ǰ�������ֽ�֮��ȡ256�Ĳ���
   unsigned char tail;		 //ͬ��β 0XA5
}RELAY_REQUEST;

typedef struct
{
   unsigned short synHeader;  //ͬ��ͷ   0xAA55
   unsigned char testType;	   //��������
   unsigned char testValue;	   //����ֵ
   unsigned long data;		  //��������
   unsigned char checksum;	  //У����   ���У��� 
   unsigned char tail;		 //ͬ��β 0XA5
}RELAY_REPONSE;

typedef struct
{
    int len;
	unsigned char data[1024];
}RelayBuf;


BOOL SendRelayOperation(EUT eut,RELAY_OPERATION operation,BOOL isValue);

BOOL SendRelayOperationWithRelayBuf(EUT eut,RELAY_OPERATION operation,BOOL isValue,RelayBuf *sndBuf,RelayBuf *rcvBuf);  
#endif  


