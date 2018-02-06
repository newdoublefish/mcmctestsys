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

#ifndef __AUTOTEST_H__
#define __AUTOTEST_H__

#include <stdlib.h>  

#ifdef __cplusplus
    extern "C" {
#endif
typedef enum{
	ENUM_TEST_PANEL_AUTO=0,
	ENUM_TEST_PANEL_MANUAL=1,
	ENUM_TEST_PANEL_NONE
}ENUMETestPanel;
void DisplayAutoTestPanel(ListType itemList,ListType deviceList,ListType collectList,ENUMETestPanel type);
void DisplayAutoTestPanelWithTestData(ListType itemList,ListType deviceList,ListType collectList,ENUMETestPanel type,char *fileName); 
#ifdef __cplusplus
    }
#endif

#endif

