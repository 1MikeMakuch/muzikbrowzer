
#if !defined(_MUSICPLAYERDS_H_)
#define _MUSICPLAYERDS_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dshow.h>

//
// Constants
//
const int TICKLEN=100, TIMERID=55;


/////////////////////////////////////////////////////////////////////////////
// MusicPlayerDS

class MusicPlayerDS
{
// Construction
public:
	MusicPlayerDS(CWnd * pParent, const CString & logpath);	// standard constructor
	~MusicPlayerDS();

    HRESULT PrepareMedia(LPTSTR lpszMovie);
    HRESULT DisplayFileDuration(void);
	long GetFileDuration(void);
    void Say(LPTSTR szText);


    HRESULT InitDirectShow(void);
    HRESULT FreeDirectShow(void);
    HRESULT HandleGraphEvent(void);

    void ResetDirectShow(void);
    void DisplayECEvent(long lEventCode, long lParam1, long lParam2);

    void StartSeekTimer(void);
    void StopSeekTimer(void);
    void UpdatePosition(REFERENCE_TIME rtNow);
    long ReadMediaPosition(void);
	void ReadMediaPosition(long &, long &, long &);

    void CALLBACK MediaTimer(UINT wTimerID, UINT msg, ULONG dwUser, ULONG dw1, ULONG dw2);

	BOOL init();
	void OnDestroy();
	void Pause();
	BOOL Play();
	void ShowState();
	void Stop();
	void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	BOOL isPlaying();
	BOOL isStopped();
	int Seek(long pos, BOOL endpos = FALSE);
	HRESULT SetVolume(long level);
	HRESULT GetVolume(long & level);
	CString GetVersion();

	void EnumFilters (IFilterGraph *pGraph) ;
	void FilterProps(IBaseFilter *pGraph) ;

	void LogError(HRESULT hr, CString msg = "");

protected:
    REFERENCE_TIME g_rtTotalTime;
    UINT_PTR g_wTimerID;
	CWnd * m_PlayerDlg;
	BOOL m_SongStarted;
	CString m_logpath;
	CFile m_logcfile;
	UINT m_logfile;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MusicPlayerDS_H__04AD8433_DF22_4491_9611_260EDAE17B96__INCLUDED_)
