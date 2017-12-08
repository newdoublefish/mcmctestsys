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

#define  EUTCONFIG                        1
#define  EUTCONFIG_CANCEL                 2       /* control type: command, callback function: EUTCONFIG_CALLBACK */
#define  EUTCONFIG_SURE                   3       /* control type: command, callback function: EUTCONFIG_CALLBACK */
#define  EUTCONFIG_TAB                    4       /* control type: tab, callback function: (none) */
#define  EUTCONFIG_TREE                   5       /* control type: tree, callback function: EUTCONFIG_CALLBACK */

#define  EUTPANEL                         2
#define  EUTPANEL_EUT_TREE                2       /* control type: tree, callback function: TREESELECTED */
#define  EUTPANEL_EUT_SAVE                3       /* control type: command, callback function: OnSaveEut */
#define  EUTPANEL_REMOVE_EUT              4       /* control type: command, callback function: OnEutRemove */
#define  EUTPANEL_EUT_ADD                 5       /* control type: command, callback function: OnAddEut */

     /* tab page panel controls */
#define  BASIC_EUTNAME                    2       /* control type: string, callback function: (none) */
#define  BASIC_EUTID                      3       /* control type: string, callback function: (none) */
#define  BASIC_ENABLECHECKBOX             4       /* control type: radioButton, callback function: (none) */

     /* tab page panel controls */
#define  MATAIN_STOPB                     2       /* control type: ring, callback function: (none) */
#define  MATAIN_DATAB                     3       /* control type: ring, callback function: (none) */
#define  MATAIN_PARITY                    4       /* control type: ring, callback function: (none) */
#define  MATAIN_RATE                      5       /* control type: ring, callback function: (none) */
#define  MATAIN_PORTNUM                   6       /* control type: ring, callback function: (none) */

     /* tab page panel controls */
#define  RS422_STOPB                      2       /* control type: ring, callback function: (none) */
#define  RS422_DATAB                      3       /* control type: ring, callback function: (none) */
#define  RS422_PARITY                     4       /* control type: ring, callback function: (none) */
#define  RS422_RATE                       5       /* control type: ring, callback function: (none) */
#define  RS422_PORTNUM                    6       /* control type: ring, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK EUTCONFIG_CALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnAddEut(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnEutRemove(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSaveEut(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK TREESELECTED(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
