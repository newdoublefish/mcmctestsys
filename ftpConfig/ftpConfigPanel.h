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

#define  PANEL                            1
#define  PANEL_REMOTEDIR                  2       /* control type: string, callback function: (none) */
#define  PANEL_SERVER                     3       /* control type: string, callback function: (none) */
#define  PANEL_PASSWORD                   4       /* control type: string, callback function: (none) */
#define  PANEL_USERNAME                   5       /* control type: string, callback function: (none) */
#define  PANEL_CHECKBOX_MODE              6       /* control type: radioButton, callback function: (none) */
#define  PANEL_BUTTON                     7       /* control type: command, callback function: onFtpConfigSave */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK onFtpConfigSave(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
