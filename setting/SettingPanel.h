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

#define  SET_PANEL                        1
#define  SET_PANEL_CBVOICE                2       /* control type: radioButton, callback function: (none) */
#define  SET_PANEL_SETTINGSAVE            3       /* control type: command, callback function: SETTINGSAVECALLBACK */
#define  SET_PANEL_CBTIPS                 4       /* control type: radioButton, callback function: (none) */
#define  SET_PANEL_SAVEDIR                5       /* control type: string, callback function: (none) */
#define  SET_PANEL_COMMANDBUTTON          6       /* control type: command, callback function: DIRSELECTED */
#define  SET_PANEL_CBLOG                  7       /* control type: radioButton, callback function: (none) */
#define  SET_PANEL_RETESTNUM              8       /* control type: numeric, callback function: (none) */
#define  SET_PANEL_NUMERIC                9       /* control type: numeric, callback function: (none) */
#define  SET_PANEL_Operater               10      /* control type: string, callback function: (none) */
#define  SET_PANEL_AUTOSAVE               11      /* control type: radioButton, callback function: (none) */
#define  SET_PANEL_RELAY_ENABLE           12      /* control type: radioButton, callback function: (none) */
#define  SET_PANEL_CBCOLLECTMENTION       13      /* control type: radioButton, callback function: (none) */
#define  SET_PANEL_RING_CLOSETIME         14      /* control type: ring, callback function: (none) */
#define  SET_PANEL_SAVETODB               15      /* control type: radioButton, callback function: (none) */
#define  SET_PANEL_RPINFOCB               16      /* control type: radioButton, callback function: (none) */
#define  SET_PANEL_SIMUTESTCB             17      /* control type: radioButton, callback function: (none) */
#define  SET_PANEL_FRONT_SIZE             18      /* control type: ring, callback function: (none) */
#define  SET_PANEL_SHOWPROCESS            19      /* control type: radioButton, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK DIRSELECTED(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK SETTINGSAVECALLBACK(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
