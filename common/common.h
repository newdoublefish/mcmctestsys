/*---------------------------------------------------------------------------
 // 版权声明：本文件由广东万城万充电动车运营股份有限公司版权所有，未经授权，
 // 禁止第三方进行拷贝和使用。
 //
 // 文件名：
 // 文件功能描述: 
 //
 // 
 // 创建标识：曾衍仁 
 //
 // 修改标识：
 // 修改描述：
 //-------------------------------------------------------------------------*/
#ifndef __COMMON_H__
#define __COMMON_H__

#ifdef __cplusplus
    extern "C" {
#endif


void WarnShow1(int panel,char *meesage);


BOOL AlertDialogWithRet(int panel,char *title,char *lableMessage,char *lableNegative,char *lablePositive);

BOOL AlertDialogWithRetAutoClose(int panel,char *title,char *lableMessage,char *lableNegative,char *lablePositive,int closeTime);


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

BOOL showTips(int panel,char *title,char *tip,int *flag);
void WarnAlert(int panel,char *meesage,int closeTime);



#ifdef __cplusplus
    }
#endif

#endif
