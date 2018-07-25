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
#ifndef __SETTINGENTITY_H__
#define __SETTINGENTITY_H__
#include "userint.h"
#ifdef __cplusplus
   extern "C" {
#endif
	   
typedef struct
{
    int showTipsWhenError; //出错是否进行提示
	int voice;		  //是否进行声音提示
	int saveLog;			 //是否保存log
	char saveDir[MAX_DIRNAME_LEN];  //报表保存路径
	int maxComunPerRowInAutoTestPanel; //每行显示的最多面板数量
	// methodToGetdirFile;  //获取目录文件的方式 0串口 1 网口
	int collectTestMention; //集合是否进行提示 
	int mentionAutoCloseTime; //提示自动关闭时间
	int autoSave;//自动保存
	int relayEnable;//继电器使能
	int reTestCnt; //重测次数
	int reportInfoCustom;//是否自定义文件信息
	int simuTest;//模拟测试
	int frontSize;
	int saveToDb; //保存到数据库
	int showProcess;//显示测试过程信息
	int uploadXml; //上传xml
}SETTING;
 

SETTING getSetting(void);
HRESULT saveSetting(SETTING s);

#ifdef __cplusplus
   }
#endif

#endif

