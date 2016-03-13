#ifndef __STATUSCASEENTITY_H__
#define __STATUSCASEENTITY_H__

#include "cvidef.h"


/*typedef struct
{
   double maxValue;          //���ֵ
   double minValue;			 //��Сֵ
}VALUE_STANDARD;			 //������׼ */



typedef struct
{			
     char *FamilyName;       //������������
	 int FamilyID;           //��������ID
	 int ItemSerial;         //���
	 char *ItemName;         //��������
	 int ParaByteLen;        //����λ��
	 int PostValue;			 //״̬����ֵ
}StatusItem;

typedef struct
{			
	 char *LRUName;         //��������
	 char *ItemName[32];	//״̬��Ŀ
}StatusLRU;

int StatusItemInit(char *filePath,StatusLRU *LRU );
 
#ifdef __cplusplus
    {
#endif
		
#endif
