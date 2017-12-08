/*---------------------------------------------------------------------------
 // 版权声明：本文件由广州航新航空科技股份有限公司版权所有，未经授权，禁止第三
 // 方进行拷贝和使用。
 //
 // 文件名：common.h
 // 文件功能描述:可以共用的面板 
 //
 // 
 // 创建标识：曾衍仁 20141114
 //
 // 修改标识：曾衍仁  20150814
 // 修改描述：
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
