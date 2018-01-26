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

#define  PANEL                            1
#define  PANEL_LISTBOX                    2       /* callback function: onClickEutConfigCtrl */
#define  PANEL_saveBtn                    3       /* callback function: onClickEutConfigCtrl */
#define  PANEL_COMMANDBUTTON              4       /* callback function: onClickEutConfigCtrl */
#define  PANEL_COMMANDBUTTON_2            5       /* callback function: onClickEutConfigCtrl */

#define  PANEL_2                          2
#define  PANEL_2_LISTBOX                  2       /* callback function: onClickEutConfigCtrl */
#define  PANEL_2_TAB                      3
#define  PANEL_2_COMMANDBUTTON            4       /* callback function: onClickEutSubConfigCtrl */
#define  PANEL_2_COMMANDBUTTON_2          5       /* callback function: onClickEutSubConfigCtrl */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK onClickEutConfigCtrl(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onClickEutSubConfigCtrl(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
