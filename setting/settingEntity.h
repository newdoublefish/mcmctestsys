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
#ifndef __SETTINGENTITY_H__
#define __SETTINGENTITY_H__
#include "userint.h"
#ifdef __cplusplus
   extern "C" {
#endif
	   
typedef struct
{
    int showTipsWhenError; //�����Ƿ������ʾ
	int voice;		  //�Ƿ����������ʾ
	int saveLog;			 //�Ƿ񱣴�log
	char saveDir[MAX_DIRNAME_LEN];  //������·��
	int maxComunPerRowInAutoTestPanel; //ÿ����ʾ������������
	// methodToGetdirFile;  //��ȡĿ¼�ļ��ķ�ʽ 0���� 1 ����
	int collectTestMention; //�����Ƿ������ʾ 
	int mentionAutoCloseTime; //��ʾ�Զ��ر�ʱ��
	int autoSave;//�Զ�����
	//int relayEnable;//�̵���ʹ��
	int reTestCnt; //�ز����
	int reportInfoCustom;//�Ƿ��Զ����ļ���Ϣ
	int simuTest;//ģ�����
	int frontSize;
}SETTING;
 

SETTING getSetting(void);
HRESULT saveSetting(SETTING s);

#ifdef __cplusplus
   }
#endif

#endif

