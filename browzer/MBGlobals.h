#ifndef _MBGLOBALS_H_
#define _MBGLOBALS_H_

// #include <tchar.h>
// #include <afxwin.h>
//#include "StdAfx.h"


#define MBALL " all"
#define MBUNKNOWN " unknown"

#define MUZIKBROWZER "muzikbrowzer"
#define MBREGKEYIRMAN "Software\\muzikbrowzer\\Irman"
#define MBREGKEY "Software\\muzikbrowzer"

#define MBUNABLETOOPEN "Unable to open database location. Set in config options, then run Scan."
#define PLAYER_STATUS_DELAY 5


#define IR_MESSAGE				(WM_APP + 1)
#define WM_GRAPHNOTIFY			(WM_APP + 2)
#define WM_PLAYLOOP				(WM_APP + 3)
#define WM_FIRSTFILE			(WM_APP + 4)
#define WM_PLAYFILE				(WM_APP + 5)
#define WM_NEXTFILE				(WM_APP + 6)
#define WM_PREVIOUSFILE			(WM_APP + 7)
#define INIT_DLG_MESSAGE		(WM_APP + 8)
#define MB_POST_MYIDLE_MESSAGE  (WM_APP + 9)
#define MB_SCROLL_MESSAGE       (WM_APP + 10)
#define MB_SERIAL_MESSAGE		(WM_APP + 11)

#define MB_PLAYLOOP_TIMER_ID	1
#define MB_SEEK_TIMER_ID		2
#define MB_STATUS_TIMER_ID		3
#define MB_COLORSTATIC_TIMER_ID 4
#define MB_FONTCOMBO_TIMER_ID	5

#define MB_TRIAL_DAYS 60

#endif
