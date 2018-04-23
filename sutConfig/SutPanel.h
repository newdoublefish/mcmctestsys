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

#define  PANEL_PART                       1
#define  PANEL_PART_LISTBOX_PARTS_SELECT  2       /* control type: listBox, callback function: (none) */
#define  PANEL_PART_COMMANDBUTTON_QUIT    3       /* control type: command, callback function: oNsutQuit */
#define  PANEL_PART_TEXTMSG               4       /* control type: textMsg, callback function: (none) */
#define  PANEL_PART_TEXTMSG_2             5       /* control type: textMsg, callback function: (none) */
#define  PANEL_PART_TEXTMSG_3             6       /* control type: textMsg, callback function: (none) */

#define  PROGRESS                         2
#define  PROGRESS_RINGSLIDE               2       /* control type: slide, callback function: (none) */
#define  PROGRESS_TEXTMSG                 3       /* control type: textMsg, callback function: (none) */


     /* Control Arrays: */

          /* (no control arrays in the resource file) */


     /* Menu Bars, Menus, and Menu Items: */

          /* (no menu bars in the resource file) */


     /* Callback Prototypes: */

int  CVICALLBACK oNsutQuit(int panel, int control, int event, void *callbackData, int eventData1, int eventData2);


#ifdef __cplusplus
    }
#endif
