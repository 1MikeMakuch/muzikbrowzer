
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
	int GetFileDuration(void);
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
	void Play();
	void ShowState();
	void Stop();
	void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	BOOL isPlaying();
	BOOL isStopped();
	int Seek(long pos, BOOL endpos = FALSE);
	HRESULT SetVolume(long level);
	HRESULT GetVolume(long & level);

	void EnumFilters (IFilterGraph *pGraph) ;
	void FilterProps(IBaseFilter *pGraph) ;

protected:
    REFERENCE_TIME g_rtTotalTime;
    UINT_PTR g_wTimerID;
	CWnd * m_PlayerDlg;
	BOOL m_SongStarted;
	CString m_logpath;
	CFile m_logcfile;
	UINT m_logfile;

//    void InitMediaDirectory(void);
//    void HandleTrackbar(WPARAM wReq);
//    BOOL CanStep(void);
//    HRESULT StepFrame(void);
//    HRESULT EnumFilters(void);
//    HRESULT EnumPins(IBaseFilter *pFilter, PIN_DIRECTION PinDir, CListBox& Listbox);
//    IBaseFilter * FindFilterFromName(LPTSTR szName);
//    BOOL SupportsPropertyPage(IBaseFilter *pFilter);
//    BOOL DisplayImageInfo(void);
//    BOOL DisplayFileInfo(LPTSTR szFile);
//    LONG GetDXMediaPath(TCHAR *strPath);
//    LONG GetGraphEditPath(TCHAR *szPath);
//    void CenterVideo(void);
//    void PlayNextFile(void);
//    void PlayPreviousFile(void);
//    void PlaySelectedFile(void);
//    void ShowState(void);
//    void ConfigureSeekbar(void);

	//	
	//	Dialog Data
	//	{{AFX_DATA(MusicPlayerDS)
	//	enum { IDD = IDD_JUKEBOX_DIALOG };
	//	CStatic	m_StrPosition;
	//	CSliderCtrl	m_Seekbar;
	//	CStatic	m_StrImageSize;
	//	CStatic	m_StrDuration;
	//	CEdit	m_EditMediaDir;
	//	CSpinButtonCtrl	m_SpinFiles;
	//	CButton	m_ButtonFrameStep;
	//	CListBox	m_ListEvents;
	//	CButton	m_CheckEvents;
	//	CButton	m_ButtonProperties;
	//	CStatic	m_StrMediaPath;
	//	CButton	m_CheckMute;
	//	CButton	m_ButtonStop;
	//	CButton	m_ButtonPlay;
	//	CButton	m_ButtonPause;
	//	CButton	m_CheckPlaythrough;
	//	CButton	m_CheckLoop;
	//	CStatic	m_StrFileDate;
	//	CStatic	m_StrFileSize;
	//	CListBox	m_ListPinsOutput;
	//	CListBox	m_ListPinsInput;
	//	CStatic	m_StrFileList;
	//	CStatic	m_Status;
	//	CStatic	m_Screen;
	//	CListBox	m_ListInfo;
	//	CListBox	m_ListFilters;
	//	CListBox	m_ListFiles;
	//	}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MusicPlayerDS)
	//protected:
	//	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation

//protected:
	//	HICON m_hIcon;
	//    int   m_nCurrentFileSelection;
//    TCHAR m_szCurrentDir[MAX_PATH];

	//	// Generated message map functions
	//	//{{AFX_MSG(MusicPlayerDS)
	//	virtual BOOL OnInitDialog();
	//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//	afx_msg void OnPaint();
	//	afx_msg HCURSOR OnQueryDragIcon();
	//	afx_msg void OnClose();
	//	afx_msg void OnSelectFile();
	//	afx_msg void OnPause();
	//	afx_msg void OnPlay();
	//	afx_msg void OnStop();
	//	afx_msg void OnCheckMute();
	//	afx_msg void OnCheckLoop();
	//	afx_msg void OnCheckPlaythrough();
	//	afx_msg void OnSelchangeListFilters();
	//	afx_msg void OnDblclkListFilters();
	//	afx_msg void OnButtonProppage();
	//	afx_msg void OnCheckEvents();
	//	afx_msg void OnButtonFramestep();
	//	afx_msg void OnButtonClearEvents();
	//	afx_msg void OnDblclkListFiles();
	//	afx_msg void OnDeltaposSpinFiles(NMHDR* pNMHDR, LRESULT* pResult);
	//	afx_msg void OnButtonSetMediadir();

	//	afx_msg void OnButtonGraphedit();
	//	afx_msg void OnDestroy();
	//	afx_msg BOOL OnEraseBkgnd(CDC *);
	//	//}}AFX_MSG
	//	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MusicPlayerDS_H__04AD8433_DF22_4491_9611_260EDAE17B96__INCLUDED_)
