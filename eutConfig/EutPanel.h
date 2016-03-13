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

#define  EUTCONFIG                        1
#define  EUTCONFIG_CANCEL                 2       /* callback function: EUTCONFIG_CALLBACK */
#define  EUTCONFIG_SURE                   3       /* callback function: EUTCONFIG_CALLBACK */
#define  EUTCONFIG_TAB                    4
#define  EUTCONFIG_TREE                   5       /* callback function: EUTCONFIG_CALLBACK */

#define  EUTPANEL                         2
#define  EUTPANEL_EUT_TREE                2       /* callback function: TREESELECTED */
#define  EUTPANEL_EUT_SAVE                3       /* callback function: OnSaveEut */
#define  EUTPANEL_REMOVE_EUT              4       /* callback function: OnEutRemove */
#define  EUTPANEL_EUT_ADD                 5       /* callback function: OnAddEut */

     /* tab page panel controls */
#define  ARINC429_RING429CARD             2
#define  ARINC429_RING429                 3
#define  ARINC429_TEXTMSG                 4
#define  ARINC429_TEXTMSG_2               5
#define  ARINC429_TEXTMSG_3               6

     /* tab page panel controls */
#define  BASIC_EUTNAME                    2
#define  BASIC_EUTID                      3
#define  BASIC_ENABLECHECKBOX             4

     /* tab page panel controls */
#define  MATAIN_STOPB                     2
#define  MATAIN_DATAB                     3
#define  MATAIN_PARITY                    4
#define  MATAIN_RATE                      5
#define  MATAIN_PORTNUM                   6

     /* tab page panel controls */
#define  RELAY_STOPB                      2
#define  RELAY_DATAB                      3
#define  RELAY_PARITY                     4
#define  RELAY_RATE                       5
#define  RELAY_PORTNUM                    6

     /* tab page panel controls */
#define  RS422_STOPB                      2
#define  RS422_DATAB                      3
#define  RS422_PARITY                     4
#define  RS422_RATE                       5
#define  RS422_PORTNUM                    6


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
