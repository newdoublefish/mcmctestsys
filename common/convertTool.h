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
#ifndef __CONVERTTOOL_H__
#define __CONVERTTOOL_H__
#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
   extern "C" {
#endif	   
	   
unsigned long String2UnsignedLong(char *str);


unsigned long ByteArray2UnsignedLong(unsigned char *str,int num);

int String2ByteArray(char *str,unsigned char *array);

double String2Double(char *str);

void HexString2UpperCase(char *str);

int OctToHex(int a);

#ifdef __cplusplus
   }
#endif	   

#endif
