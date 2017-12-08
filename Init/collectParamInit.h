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
#ifndef __COLLECTPARAMINIT_H__
#define __COLLECTPARAMINIT_H__
#include "toolbox.h"
#ifdef __cplusplus
   extern "C" {
#endif
#define MAX_COLLECT_PARAM_NAME_LEN 50
typedef struct
{
      char type[MAX_COLLECT_PARAM_NAME_LEN];
	  int id;
	  char name[MAX_COLLECT_PARAM_NAME_LEN];
	  int paramLen;
	  int storeLen;
	  int rate;
	  int dataType;
}CollectParam;

HRESULT collectParamInit(char *filePath);

ListType getCollectParamList(void);

HashTableType getCollcetParamHashType(void);

CollectParam getCollectParamByHashTable(char *name);

#ifdef __cplusplus
   {
#endif	   

#endif
