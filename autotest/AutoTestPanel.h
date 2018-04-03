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

#define  P_ITEMSHOW                       1
#define  P_ITEMSHOW_TREE                  2       /* control type: tree, callback function: (none) */
#define  P_ITEMSHOW_TEXTMSG_2             3       /* control type: textMsg, callback function: (none) */
#define  P_ITEMSHOW_ACCURACY              4       /* control type: textMsg, callback function: (none) */
#define  P_ITEMSHOW_TEXTMSG               5       /* control type: textMsg, callback function: (none) */
#define  P_ITEMSHOW_PERCSTR               6       /* control type: textMsg, callback function: (none) */

#define  PANEL_AUTO                       2       /* callback function: onPanelAutoCall */
#define  PANEL_AUTO_PICTURE_5             2       /* control type: picture, callback function: (none) */
#define  PANEL_AUTO_TEXTMSG_3             3       /* control type: textMsg, callback function: (none) */
#define  PANEL_AUTO_TEST                  4       /* control type: pictButton, callback function: STARTTEST */
#define  PANEL_AUTO_RESTBUTTON            5       /* control type: pictButton, callback function: RESET */
#define  PANEL_AUTO_BACK                  6       /* control type: pictButton, callback function: QUITAUTOTEST */
#define  PANEL_AUTO_TIMER                 7       /* control type: timer, callback function: TESTTIMECALLBACK */
#define  PANEL_AUTO_TAB_AUTO              8       /* control type: tab, callback function: (none) */
#define  PANEL_AUTO_BACKGROUD             9       /* control type: textMsg, callback function: (none) */
#define  PANEL_AUTO_TEXTMSG               10      /* control type: textMsg, callback function: (none) */
#define  PANEL_AUTO_GENREPORT             11      /* control type: pictButton, callback function: genReport */
#define  PANEL_AUTO_TEST_PROJECT          12      /* control type: textMsg, callback function: (none) */
#define  PANEL_AUTO_CURRENT_SUT           13      /* control type: textMsg, callback function: (none) */

#define  PANEL_WA_2                       3
#define  PANEL_WA_2_TEXTMSG               2       /* control type: textMsg, callback function: (none) */
#define  PANEL_WA_2_TEXTMSG_2             3       /* control type: textMsg, callback function: (none) */
#define  PANEL_WA_2_TEXTMSG_3             4       /* control type: textMsg, callback function: (none) */

#define  PANEL_WARN                       4
#define  PANEL_WARN_COMMANDBUTTON         2       /* control type: command, callback function: (none) */
#define  PANEL_WARN_TEXTMSG               3       /* control type: textMsg, callback function: (none) */
#define  PANEL_WARN_TEXTMSG_2             4       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK genReport(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onPanelAutoCall(int panel, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK QUITAUTOTEST(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK RESET(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK STARTTEST(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TESTTIMECALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
