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
#define  PANEL_TAB                        2       /* callback function: TAB_CALLBACK */
#define  PANEL_TEXTBOX                    3
#define  PANEL_QUIT                       4       /* callback function: relayPanelQuitCallback */

#define  PANEL_2                          2
#define  PANEL_2_TEXTMSG_2                2
#define  PANEL_2_TABLE                    3       /* callback function: TB_CB */
#define  PANEL_2_COMMANDBUTTON            4       /* callback function: testAllCallback */
#define  PANEL_2_TEXTMSG_ID               5


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK relayPanelQuitCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TAB_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TB_CB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK testAllCallback(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
