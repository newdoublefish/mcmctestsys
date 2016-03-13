/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2016. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  P_ITEMSHOW                       1
#define  P_ITEMSHOW_TREE                  2
#define  P_ITEMSHOW_TEXTMSG               3
#define  P_ITEMSHOW_PERCSTR               4

#define  PANEL_AUTO                       2       /* callback function: onPanelAutoCall */
#define  PANEL_AUTO_PICTURE_5             2
#define  PANEL_AUTO_PICTURE_3             3
#define  PANEL_AUTO_TEXTMSG_3             4
#define  PANEL_AUTO_TEST                  5       /* callback function: STARTTEST */
#define  PANEL_AUTO_RESTBUTTON            6       /* callback function: RESET */
#define  PANEL_AUTO_BACK                  7       /* callback function: QUITAUTOTEST */
#define  PANEL_AUTO_TEXTMSG               8
#define  PANEL_AUTO_TIMER                 9       /* callback function: TESTTIMECALLBACK */
#define  PANEL_AUTO_TAB_AUTO              10

#define  PANEL_WA_2                       3
#define  PANEL_WA_2_PICTURE               2
#define  PANEL_WA_2_TEXTMSG               3
#define  PANEL_WA_2_TEXTMSG_2             4

#define  PANEL_WARN                       4
#define  PANEL_WARN_PICTURE               2
#define  PANEL_WARN_COMMANDBUTTON         3
#define  PANEL_WARN_TEXTMSG               4


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK onPanelAutoCall(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QUITAUTOTEST(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RESET(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK STARTTEST(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TESTTIMECALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
