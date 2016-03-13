/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2015. All Rights Reserved.          */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  PANEL                            1
#define  PANEL_PICTURE_2                  2
#define  PANEL_TEXTMSG_MAINTAN            3
#define  PANEL_TREE                       4
#define  PANEL_TEXTMSG                    5
#define  PANEL_TEXTMSG_3                  6
#define  PANEL_SENDCOUNT                  7
#define  PANEL_RECVCOUNT                  8

#define  PANEL_STAT                       2
#define  PANEL_STAT_TAB_STAT              2       /* callback function: TAB_STAT_CLICK */
#define  PANEL_STAT_BACK                  3       /* callback function: QUITSTATUSMONITOR */
#define  PANEL_STAT_PICTURE               4
#define  PANEL_STAT_TEXTMSG               5


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK QUITSTATUSMONITOR(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TAB_STAT_CLICK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
