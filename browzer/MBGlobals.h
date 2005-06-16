#ifndef _MBGLOBALS_H_
#define _MBGLOBALS_H_

#include <tchar.h>
// #include <afxwin.h>
//#include "StdAfx.h"


#define MBALL " all"
#define MBUNKNOWN " unknown"
#define MBURL "www.muzikbrowzer.com"

#define MUZIKBROWZER "muzikbrowzer"
#define MBREGKEYIRMAN "Software\\muzikbrowzer\\Irman"
#define MBREGKEY "Software\\muzikbrowzer"
#define MBREGKEYLAST "Software\\muzikbrowzer\\PreviousValues"

#define MBUNABLETOOPEN "Unable to open database location. Set in config options, then run Scan."
#define PLAYER_STATUS_DELAY 10
#define MBPLAYLIST "_Playlist_"
#define MBPLAYLISTEXT ".mbp"
#define MBTHEMEEXT ".mbtheme"

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
#define MB_CONFIG_CBUTTON		(WM_APP + 12)
#define MB_FRAC_MESSAGE			(WM_APP + 13)
#define MB_FRAC_PIX_MSG			(WM_APP + 14)
#define WM_IDLE					(WM_APP + 14)
#define MB_VOLUME_MSG			(WM_APP + 15)
#define MB_PROGRESS_MSG			(WM_APP + 16)
#define MB_BITMAP_CUTTER_MSG	(WM_APP + 17)

#define MB_PLAYLOOP_TIMER_ID	1
#define MB_SEEK_TIMER_ID		2
#define MB_STATUS_TIMER_ID		3
#define MB_COLORSTATIC_TIMER_ID 4
#define MB_FONTCOMBO_TIMER_ID	5
#define MB_FRAC_TIMER_ID		6
#define MB_VOLUME_TIMER_ID		7

#define MB_TRIAL_DAYS 60

#undef MB_USING_TRIAL_MODE

#define Stringize( L ) #L 
#define MakeString( M, L ) M(L) 
#define $Line \
	MakeString( Stringize, __LINE__ ) 
#define Reminder \
	 __FILE__ "(" $Line ") : Reminder: "
#define hack message(Reminder "Fix this hack!") 


#endif
