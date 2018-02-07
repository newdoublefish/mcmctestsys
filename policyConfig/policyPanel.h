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

#define  COL_DETAIL                       1
#define  COL_DETAIL_NAME                  2       /* control type: string, callback function: (none) */
#define  COL_DETAIL_TESTTYPE              3       /* control type: ring, callback function: (none) */
#define  COL_DETAIL_DERIVETYPE            4       /* control type: ring, callback function: (none) */
#define  COL_DETAIL_MAXRUNDEVICE          5       /* control type: string, callback function: (none) */
#define  COL_DETAIL_NEWUUTCANCEL          6       /* control type: command, callback function: (none) */
#define  COL_DETAIL_NEWUUTSURE            7       /* control type: command, callback function: (none) */
#define  COL_DETAIL_DECORATION            8       /* control type: deco, callback function: (none) */
#define  COL_DETAIL_RETRYSTRING           9       /* control type: string, callback function: (none) */
#define  COL_DETAIL_TIP_MENTION           10      /* control type: radioButton, callback function: (none) */
#define  COL_DETAIL_TIP_CLOSE_TIME        11      /* control type: ring, callback function: (none) */
#define  COL_DETAIL_TEXTMSG               12      /* control type: textMsg, callback function: (none) */

#define  PANEL                            2
#define  PANEL_TREE                       2       /* control type: tree, callback function: (none) */
#define  PANEL_SURE                       3       /* control type: command, callback function: (none) */

#define  PLCYPANEL                        3
#define  PLCYPANEL_ITEMTREE               2       /* control type: tree, callback function: ONCLICKITEMTREE */
#define  PLCYPANEL_SAVESTRATEGY           3       /* control type: command, callback function: OnSaveStrategy */
#define  PLCYPANEL_COLLECT_MOVE_DOWN      4       /* control type: command, callback function: OnAddCollect */
#define  PLCYPANEL_COLLECT_MOVE_UP        5       /* control type: command, callback function: OnAddCollect */
#define  PLCYPANEL_ADDCOLLECT             6       /* control type: command, callback function: OnAddCollect */
#define  PLCYPANEL_COLLECTTREE            7       /* control type: tree, callback function: OnCollectClick */
#define  PLCYPANEL_ADDTOCOLLECT           8       /* control type: command, callback function: OnAddTestItemToCollect */
#define  PLCYPANEL_DELETE                 9       /* control type: command, callback function: OnDelete */
#define  PLCYPANEL_BTN_BELOW              10      /* control type: command, callback function: OnAddTestItemToCollect */
#define  PLCYPANEL_BTN_ABOVE              11      /* control type: command, callback function: OnAddTestItemToCollect */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK OnAddCollect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnAddTestItemToCollect(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ONCLICKITEMTREE(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnCollectClick(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnDelete(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK OnSaveStrategy(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
