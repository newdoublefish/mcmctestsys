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

#define  PANEL_PART                       1
#define  PANEL_PART_PICTURE_2             2
#define  PANEL_PART_PICTURE               3
#define  PANEL_PART_LISTBOX_PARTS_SELECT  4
#define  PANEL_PART_COMMANDBUTTON_QUIT    5       /* callback function: oNsutQuit */
#define  PANEL_PART_COMMANDBUTTON_BACK    6       /* callback function: oNsutQuit */
#define  PANEL_PART_COMMANDBUTTON_NEXT    7       /* callback function: SYSTEM_SELECTE_NEXT */
#define  PANEL_PART_RING                  8

#define  PROGRESS                         2
#define  PROGRESS_RINGSLIDE               2
#define  PROGRESS_TEXTMSG                 3


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK oNsutQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SYSTEM_SELECTE_NEXT(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
