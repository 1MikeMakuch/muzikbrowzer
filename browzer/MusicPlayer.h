
#ifndef _MUSICPLAYER_H_
#define _MUSICPLAYER_H_

//class CPlayerDlg;

/////////////////////////////////////////////////////
// DirectShow
////////////////////////////////////////////////////

#include "MusicPlayerDS.h"

class MusicPlayer
{
public:
	MusicPlayer(CWnd *, const CString & logpath);
	~MusicPlayer();
	int init();
	int InputOpen(const char *name);
	int InputClose();
	int Pause();
	int Play();
	int Seek(long pos, BOOL endpos = FALSE);
	int Stop();
	int SetVolume(int level);
	int GetVolume(int &);
	void setScrolling(BOOL b) {m_Scrolling = b;};

	BOOL isScrolling() { return m_Scrolling; }
	BOOL isPlaying();
	BOOL isStopped();
	HRESULT HandleGraphEvent(void) { return m_MP->HandleGraphEvent();}
	int GetFileDuration();
	long ReadMediaPosition(void);
	void ReadMediaPosition(long &, long &, long &);
private:
	MusicPlayerDS * m_MP;

	int m_State;
	HINSTANCE m_Instance;
//	CPlayerDlg * m_PlayerDlg;
	CWnd * m_PlayerDlg;
	BOOL m_Scrolling;
};

#endif // ifndef _MUSICPLAYER_H_
