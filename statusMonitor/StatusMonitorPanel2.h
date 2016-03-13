/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/* Copyright (c) National Instruments 2007. All Rights Reserved.          */
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
#define  PANEL_QUIT                       9       /* callback function: MONITORQUITCALLBACK */

#define  PANEL_STAT                       2
#define  PANEL_STAT_BACK                  2       /* callback function: QUITSTATUSMONITOR2 */
#define  PANEL_STAT_PICTURE               3
#define  PANEL_STAT_TEXTMSG               4
#define  PANEL_STAT_TREE                  5       /* callback function: MONITOREUTSELECTCALLBACK */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK MONITOREUTSELECTCALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK MONITORQUITCALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QUITSTATUSMONITOR2(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
