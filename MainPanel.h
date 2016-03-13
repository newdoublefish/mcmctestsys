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

#define  PANEL_MAIN                       1
#define  PANEL_MAIN_PICTURE_2             2
#define  PANEL_MAIN_PICTURE               3
#define  PANEL_MAIN_TAB_MAIN              4       /* callback function: CLICKTAB */
#define  PANEL_MAIN_QUIT                  5       /* callback function: QUITAPP */
#define  PANEL_MAIN_BACK                  6       /* callback function: BackMain */
#define  PANEL_MAIN_CONF                  7       /* callback function: EnableConf */
#define  PANEL_MAIN_TST                   8       /* callback function: EnableTst */
#define  PANEL_MAIN_VERSION_VALUE         9
#define  PANEL_MAIN_VERSION               10

     /* tab page panel controls */
#define  TABPANEL_PICTUREBUTTON_MANUAL    2
#define  TABPANEL_PICTUREBUTTON_INSTR     3       /* callback function: PICTUREBUTTON_INSTRUCTIONCALLBACK */
#define  TABPANEL_PICTUREBUTTON_AUTO      4       /* callback function: PICTUREBUTTON_AUTO */
#define  TABPANEL_PICTUREBUTTON_STATE     5       /* callback function: PICTUREBUTTON_STATE */

     /* tab page panel controls */
#define  TABPANEL_2_TAB                   2       /* callback function: CLICKTAB */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK BackMain(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK CLICKTAB(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK EnableConf(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK EnableTst(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PICTUREBUTTON_AUTO(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PICTUREBUTTON_INSTRUCTIONCALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK PICTUREBUTTON_STATE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QUITAPP(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
