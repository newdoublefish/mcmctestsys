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
#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
    extern "C" {
#endif


void WarnShow1(int panel,char *meesage);


BOOL AlertDialogWithRet(int panel,char *title,char *lableMessage,char *lableNegative,char *lablePositive);


int displaySlideProgress(char *title);  

void slideProgressShow(int panelHandle,int perc,char *message,double delayTime);

int displaySlideProgressWithTextBox(char *title);

void slideProgressShowWithTextBox(int panelHandle,int perc,char *description,double delayTime);

void disposeSlideProgress(int handle); 

void disposeSlideProgressWithButton(int handle);

void refreshSlideProgress(int panel,int perc);

BOOL GetConfigWarnPanelRetWithQuitFlag(int panel,char *title,char *lableMessage,char *lableNegative,char *lablePositive,int *flag);

BOOL getUserCustiomName(char *fileName);

void alignToParentPanel(int parent,int ctrl);

BOOL showTips(int panel,char *title,char *tip);



#ifdef __cplusplus
    }
#endif

#endif
