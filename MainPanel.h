/**************************************************************************/
/* LabWindows/CVI User Interface Resource (UIR) Include File              */
/*                                                                        */
/* WARNING: Do not add to, delete from, or otherwise modify the contents  */
/*          of this include file.                                         */
/**************************************************************************/

#include <userint.h>

#ifdef __cplusplus
    extern "C" {
#endif

     /* Panels and Controls: */

#define  NEW_TEST                         1
#define  NEW_TEST_STRING                  2       /* control type: string, callback function: (none) */
#define  NEW_TEST_SURE                    3       /* control type: command, callback function: (none) */
#define  NEW_TEST_TEXTMSG                 4       /* control type: textMsg, callback function: (none) */

#define  PANEL_MAIN                       2
#define  PANEL_MAIN_PICTURE               2       /* control type: picture, callback function: (none) */
#define  PANEL_MAIN_TAB_MAIN              3       /* control type: tab, callback function: CLICKTAB */
#define  PANEL_MAIN_QUIT                  4       /* control type: pictButton, callback function: QUITAPP */
#define  PANEL_MAIN_BACK                  5       /* control type: pictButton, callback function: BackMain */
#define  PANEL_MAIN_CONF                  6       /* control type: pictButton, callback function: EnableConf */
#define  PANEL_MAIN_TST                   7       /* control type: pictButton, callback function: EnableTst */
#define  PANEL_MAIN_VERSION_VALUE         8       /* control type: textMsg, callback function: (none) */
#define  PANEL_MAIN_DECROTE_HEADER        9       /* control type: textMsg, callback function: (none) */
#define  PANEL_MAIN_TEXTMSG_2             10      /* control type: textMsg, callback function: (none) */
#define  PANEL_MAIN_COPYRIGHT             11      /* control type: textMsg, callback function: (none) */

     /* tab page panel controls */
#define  TABPANEL_PICTUREBUTTON_MANUAL    2       /* control type: pictButton, callback function: PICTUREBUTTON_STATE */
#define  TABPANEL_PICTUREBUTTON_INSTR     3       /* control type: pictButton, callback function: PICTUREBUTTON_INSTRUCTIONCALLBACK */
#define  TABPANEL_PICTUREBUTTON_AUTO      4       /* control type: pictButton, callback function: PICTUREBUTTON_AUTO */
#define  TABPANEL_PICTUREBUTTON_STATE     5       /* control type: pictButton, callback function: PICTUREBUTTON_STATE */

     /* tab page panel controls */
#define  TABPANEL_2_TAB                   2       /* control type: tab, callback function: CLICKTAB */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


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
