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

#define  EUTPANEL                         1
#define  EUTPANEL_LISTBOX                 2       /* control type: listBox, callback function: onClickEutSubConfigCtrl */
#define  EUTPANEL_TAB                     3       /* control type: tab, callback function: (none) */
#define  EUTPANEL_SURE                    4       /* control type: command, callback function: onClickEutSubConfigCtrl */
#define  EUTPANEL_COMMANDBUTTON_2         5       /* control type: command, callback function: onClickEutSubConfigCtrl */
#define  EUTPANEL_TEXTMSG                 6       /* control type: textMsg, callback function: (none) */

#define  PANEL                            2
#define  PANEL_SAVE                       2       /* control type: command, callback function: onClickEutConfigCtrl */
#define  PANEL_COMMANDBUTTON              3       /* control type: command, callback function: onClickEutConfigCtrl */
#define  PANEL_ADD                        4       /* control type: command, callback function: onClickEutConfigCtrl */
#define  PANEL_DELETE                     5       /* control type: command, callback function: OnEutDelete */
#define  PANEL_TREE                       6       /* control type: tree, callback function: onClickEutConfigCtrl */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK onClickEutConfigCtrl(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK onClickEutSubConfigCtrl(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnEutDelete(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
