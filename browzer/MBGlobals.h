#ifndef _MBGLOBALS_H_
#define _MBGLOBALS_H_

#include <tchar.h>
// #include <afxwin.h>
//#include "StdAfx.h"


#define MBALL " all"
#define MBUNKNOWN " unknown"
#define MBURL "www.muzikbrowzer.com"

#define MUZIKBROWZER "Muzikbrowzer"
#define MBREGKEYIRMAN "Software\\Muzikbrowzer\\Irman"
#define MBREGKEY "Software\\Muzikbrowzer"
#define MBREGKEYLAST "Software\\Muzikbrowzer\\PreviousValues"

#define MBUNABLETOOPEN "Unable to open database location. Set in config options, then run Scan."
#define PLAYER_STATUS_DELAY 2
#define MBPLAYLIST "_Playlist_"
#define MBPLAYLISTEXT ".mbp"
#define MBPLAYLISTDIR "playlists"
#define MBPLAYLISTM3U "m3u"

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
#define MB_TV_MSG				(WM_APP + 18)
#define MB_LISTMOVEUP			(WM_APP + 19)
#define MB_LISTMOVEDN			(WM_APP + 20)

#define MB_HOVER_CANCEL_MSG		(WM_APP + 30)

#define MB_HOVER_BEGIN_MSG      (WM_APP + 31)
#define MB_HOVER_WWW_MSG		(WM_APP + 31)
#define MB_HOVER_SHUFFLE_MSG	(WM_APP + 32)
#define MB_HOVER_LOAD_MSG		(WM_APP + 33)
#define MB_HOVER_CLEAR_MSG		(WM_APP + 34)
#define MB_HOVER_SAVE_MSG		(WM_APP + 35)
#define MB_HOVER_RANDOM_MSG		(WM_APP + 36)
#define MB_HOVER_PLAY_MSG		(WM_APP + 37)
#define MB_HOVER_STOP_MSG		(WM_APP + 38)
#define MB_HOVER_PAUSE_MSG		(WM_APP + 39)
#define MB_HOVER_PREVIOUS_MSG	(WM_APP + 40)
#define MB_HOVER_NEXT_MSG		(WM_APP + 41)
#define MB_HOVER_VOLUME_MSG		(WM_APP + 42)
#define MB_HOVER_POSITION_MSG	(WM_APP + 43)
#define MB_HOVER_MENU_MSG		(WM_APP + 44)
#define MB_HOVER_MINIMIZE_MSG	(WM_APP + 45)
#define MB_HOVER_MAXIMIZE_MSG	(WM_APP + 46)
#define MB_HOVER_RESTORE_MSG	(WM_APP + 47)
#define MB_HOVER_EXIT_MSG		(WM_APP + 48)
#define MB_HOVER_END_MSG		(WM_APP + 48)

#define MB_DBLCLICK_GENRES		(WM_APP + 60)
#define MB_DBLCLICK_ARTISTS		(WM_APP + 61)
#define MB_DBLCLICK_ALBUMS		(WM_APP + 62)


#define MB_SKINPICS_MSGS_BEGIN	(WM_APP + 1000)
#define MB_SKINPICS_MSGS_END    (WM_APP + 1999)

#define MB_PLAYLOOP_TIMER_ID	1
#define MB_SEEK_TIMER_ID		2
#define MB_STATUS_TIMER_ID		3
#define MB_COLORSTATIC_TIMER_ID 4
#define MB_FONTCOMBO_TIMER_ID	5
#define MB_FRAC_TIMER_ID		6
#define MB_VOLUME_TIMER_ID		7
#define MB_TICKER_TIMER_ID		8

#define MB_TRIAL_DAYS 60

#undef MB_USING_TRIAL_MODE
//#define MB_USING_TRIAL_MODE

#define Stringize( L ) #L 
#define MakeString( M, L ) M(L) 

#ifdef MBUsePragmaHack
#define $Line \
	MakeString( Stringize, __LINE__ ) 
#define Reminder \
	 __FILE__ "(" $Line ") : Reminder: "
#define hack message(Reminder "Fix this hack!") 
#else
#define hack 
#endif

#define MB_MIN_WIDTH 500
#define MB_MIN_HEIGHT 400

#endif
