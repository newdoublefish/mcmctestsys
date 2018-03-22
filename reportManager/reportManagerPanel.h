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

#define  FTP                              1
#define  FTP_TEXTBOX                      2       /* control type: textBox, callback function: (none) */
#define  FTP_BACKGROUD                    3       /* control type: textMsg, callback function: (none) */
#define  FTP_COMMANDBUTTON                4       /* control type: command, callback function: onFtpFinishClick */
#define  FTP_TEXTMSG                      5       /* control type: textMsg, callback function: (none) */

#define  PANEL                            2       /* callback function: ReportPanelCallbak */
#define  PANEL_RECORDTREE                 2       /* control type: tree, callback function: (none) */
#define  PANEL_TITLE                      3       /* control type: textMsg, callback function: (none) */
#define  PANEL_BACKGROUD                  4       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK onFtpFinishClick(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);
int  CVICALLBACK ReportPanelCallbak(int panel, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
