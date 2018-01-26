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
#ifndef __EUTMANAGE_H__
#define __EUTMANAGE_H__

#ifdef __cplusplus
    extern "C" {
#endif
#include "cvixml.h" 
#include "ListKeyValue.h"
typedef struct
{
	char tagName[20];
	int configPtrHandle;
}ConfigItem;


typedef struct
{
	ListType configList;
}EEUT;


typedef struct
{
	char configName[20];
	void *onPanelChange;
	void *onCreate;
	void *onLoadConfig;
	void *onSaveConfig;
	void *onLoadPanel; 
}ConfigActivity;

typedef struct{
	char name[20];
	ConfigActivity activity;	
}tActivity;


typedef ConfigActivity (*ACTIVITY_FUNCTION)(void); 

int getActivity(char *name,tActivity *tActivityPtr);   

typedef int (*LOAD_EUT_SUB_CONFIG)(CVIXMLElement currElem,int handle);

typedef int (*LOAD_CONFIG_PANEL)(int);
typedef int (*SAVE_EUT_SUB_CONFIG)(CVIXMLElement currElem,int configHandle);

typedef void (*GET_CHANGE_CONFIG)(int panelHandle,int configHandle);

typedef int (*ON_CREATE_CONFIG)(void);
typedef void (*ON_LOAD_CONFIG)(ListType mapList,int handle);
typedef void (*ON_SAVE_CONFIG)(ListType mapList,int handle);  

void InitEutConfig(void);

ListType getEEutList(void);

ListType getGenerteEutList(void);

void addNewToEutList(void); 

void saveEutList(void);

#ifdef __cplusplus
    }
#endif

#endif
