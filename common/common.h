/*---------------------------------------------------------------------------
 // ��Ȩ���������ļ��ɹ��ݺ��º��տƼ��ɷ����޹�˾��Ȩ���У�δ����Ȩ����ֹ����
 // �����п�����ʹ�á�
 //
 // �ļ�����common.h
 // �ļ���������:���Թ��õ���� 
 //
 // 
 // ������ʶ�������� 20141114
 //
 // �޸ı�ʶ��������  20150814
 // �޸�������
 //-------------------------------------------------------------------------*/
#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
    extern "C" {
#endif

void WarnShow(char *meesage);

void WarnShow1(int panel,char *meesage);

BOOL GetWarnPanelRet(char *meesage);

BOOL GetConfigWarnPanelRet(int panel,char *title,char *lableMessage,char *lableNegative,char *lablePositive);


int displaySlideProgress(char *title);  

void slideProgressShow(int panelHandle,int perc,char *message,double delayTime);

int displaySlideProgressWithTextBox(char *title);

void slideProgressShowWithTextBox(int panelHandle,int perc,char *description,double delayTime);

void disposeSlideProgress(int handle); 

void disposeSlideProgressWithButton(int handle);

void refreshSlideProgress(int panel,int perc);

void ShowManualTip(int panel,char *title,char *tip);

BOOL GetConfigWarnPanelRetWithQuitFlag(int panel,char *title,char *lableMessage,char *lableNegative,char *lablePositive,int *flag);

BOOL getUserCustiomName(char *fileName);

void alignToParentPanel(int parent,int ctrl);



#ifdef __cplusplus
    }
#endif

#endif
