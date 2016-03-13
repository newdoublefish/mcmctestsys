/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ��ݺ��º��տƼ��ɷ����޹�˾��Ȩ���У�δ����Ȩ����ֹ����
 // �����п�����ʹ�á�
 //
 // �ļ�����settingEntity.h
 // �ļ���������: ϵͳ���ô�ȡ
 //
 // 
 // ������ʶ�������� 20150721
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
	int methodToGetdirFile;  //��ȡĿ¼�ļ��ķ�ʽ 0���� 1 ����
	int collectTestMention; //�����Ƿ������ʾ 
	int mentionAutoCloseTime; //��ʾ�Զ��ر�ʱ��
	int autoSave;//�Զ�����
	int relayEnable;//�̵���ʹ��
	int reTestCnt; //�ز����
	int reportInfoCustom;//�Ƿ��Զ����ļ���Ϣ
	int simuTest;//ģ�����
}SETTING;
 

SETTING getSetting(void);
HRESULT saveSetting(SETTING s);

#ifdef __cplusplus
   }
#endif

#endif

