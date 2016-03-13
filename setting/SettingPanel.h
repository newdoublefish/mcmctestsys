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

#define  SET_PANEL                        1
#define  SET_PANEL_CBVOICE                2
#define  SET_PANEL_SETTINGSAVE            3       /* callback function: SETTINGSAVECALLBACK */
#define  SET_PANEL_CBTIPS                 4
#define  SET_PANEL_SAVEDIR                5
#define  SET_PANEL_COMMANDBUTTON          6       /* callback function: DIRSELECTED */
#define  SET_PANEL_CBLOG                  7
#define  SET_PANEL_RETESTNUM              8
#define  SET_PANEL_NUMERIC                9
#define  SET_PANEL_RING                   10
#define  SET_PANEL_Operater               11
#define  SET_PANEL_RELAY_ENABLE           12
#define  SET_PANEL_AUTOSAVE               13
#define  SET_PANEL_CBCOLLECTMENTION       14
#define  SET_PANEL_RING_CLOSETIME         15
#define  SET_PANEL_RPINFOCB               16
#define  SET_PANEL_SIMUTESTCB             17


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK DIRSELECTED(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETTINGSAVECALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
