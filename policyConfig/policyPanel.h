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

#define  COL_DETAIL                       1
#define  COL_DETAIL_NAME                  2
#define  COL_DETAIL_TESTTYPE              3
#define  COL_DETAIL_DERIVETYPE            4
#define  COL_DETAIL_MAXRUNDEVICE          5
#define  COL_DETAIL_NEWUUTCANCEL          6
#define  COL_DETAIL_NEWUUTSURE            7
#define  COL_DETAIL_DECORATION            8
#define  COL_DETAIL_RETRYSTRING           9

#define  PANEL                            2
#define  PANEL_TREE                       2
#define  PANEL_SURE                       3

#define  PLCYPANEL                        3
#define  PLCYPANEL_ITEMTREE               2       /* callback function: ONCLICKITEMTREE */
#define  PLCYPANEL_SAVESTRATEGY           3       /* callback function: OnSaveStrategy */
#define  PLCYPANEL_ADDCOLLECT             4       /* callback function: OnAddCollect */
#define  PLCYPANEL_COLLECTTREE            5       /* callback function: OnCollectClick */
#define  PLCYPANEL_ADDTOCOLLECT           6       /* callback function: OnAddTestItemToCollect */
#define  PLCYPANEL_DELETE                 7       /* callback function: OnDelete */


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
