// mfcplayerDlg.h : header file
//

#if !defined(AFX_PLAYERDLG_H__FCF45C46_7793_11D2_9317_0020AFF7E192__INCLUDED_)
#define AFX_PLAYERDLG_H__FCF45C46_7793_11D2_9317_0020AFF7E192__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Includes

#include "PlayerApp.h"
#include "InitDlg.h"
#include "MusicDb.h"
#include "irman.h"
//#include "VirtualControl.h"
#include "ExtendedListBox.h"
//#include "MenuDialog.h"
#include "MBConfig.h"
#include "ColorStatic.h"
#include "SliderCtrl.h"
#include "MusicPlayer.h"
#include "Mbutton.h"
#include "PictureStatic.h"

class VirtualControl;
class VirtualDialog;
class CMenuDialog;
class CTransparentDialogDlg;

class CPlayerDlg;

#ifdef THE_PLAYER
CPlayerDlg * thePlayer = 0;
#else
extern CPlayerDlg * thePlayer;
#endif


// class CPictureDlg;

//#include "LoadPlaylistDlg.h"
//class LoadPlaylistDlg;
/////////////////////////////////////////////////////////////////////////////
// CMfcplayerDlg dialog

//class CConfigSettings;

class CPlayerDlg : public CDialog
{
// Construction
public:
    MusicPlayer *m_Player;

	CPlayerDlg(CPlayerApp *, CTransparentDialogDlg *, CWnd* pParent = NULL);	// standard constructor
    ~CPlayerDlg();
    CPlayerApp * m_PlayerApp;
	MusicLib m_mlib;
	CString _selectedGenre, _selectedArtist, _selectedAlbum,
        _selectedSong, _selectedPlaylistSong;

	void updatePlaylist();
	virtual BOOL PreTranslateMessage(MSG*);
	Irman & irman() { return m_irman; }
    void PlayLoop();
    void initDb();
    void init();
    void SavePlaylist(CString);
    void LoadPlaylist(CString);
    void OnExit();
    void SetSavePlaylistFlag(BOOL flag) { m_SavePlaylistFlag = flag; }
//	void RunTestHarness();
	void setDuration(int duration);
	void addDuration(int duration);
	void calcDuration();
	afx_msg void OnTimer(UINT nIDEvent);

    VirtualControl * m_Control;
    VirtualDialog * m_Dialog;

    void OnControlSelChange();
    void OnControlClick();
    void OnDelete();
    void OnPageUp();
    void OnPageDown();
    void OnNextSong();
    void OnPreviousSong();
    void setFont();
    MBConfig & config() { return m_Config; }
    void PlayerStatusSet(CString );
	void CurrentTitleSet(CString);
    void PlayerStatusTempSet(CString );
    void setPlaylistCurrent(int sel) {
        m_PlaylistCurrent = sel; }
    void movePlaylistUp(int);
    void movePlaylistDown(int);
	void StartSeekTimer();
	void StopSeekTimer();
	void StartStatusTimer();
	void StopStatusTimer();
	void StopTimers();
	int HandleIRMessage(int key);
private:

	int m_PlaylistDuration;
	CTransparentDialogDlg *_initdialog;
	Irman m_irman;
    MBConfig m_Config;
    CString m_PlayerStatusSave;
    CTime m_PlayerStatusTime;
    CMenuDialog * m_MenuDlg;
    BOOL m_StopFlag;
    int m_PlaylistCurrent;
	CString m_LastThingQueuedUp;

    CString m_StatusArray[10];

    int mWindowFlag;
	int m_trialCounter;

    void getControlSettings();
    void resizeControls(BOOL withPic = FALSE, int init = 0);
    void SaveWindowPos();
    void ReadWindowPos(int &, CRect &);
    void redraw();
	void updateVolumeLabel();
	void adjustVolume(int level);
	void adjustVolume();
	void OnVolUp();
	void OnVolDown();
	void OnFastForward();
	void OnReverse();
	void updatePositionLabel();
	void adjustPosition();
	void adjustPosition(int pct);
	void resetPosition();
    void PlayerStatusRevert();

	void InputClose();
	void Pause();
	void Play();
	void Stop();
	void saveConfig();
	void readConfig();
	void ShowBetaWarning();
	BOOL HelpInfo();
	void recordTLEN();
 	void displayAlbumArt(const CString & file);
 	void killAlbumArt();

    BOOL m_LButtonDown;
    CPoint m_deltaPoint;
    CBrush m_brush;

    CMapStringToString m_GenreArtist;
    CMapStringToString m_ArtistAlbum;
    CMapStringToString m_AlbumSong;
    BOOL m_SavePlaylistFlag;
	REFERENCE_TIME m_timerid;
	REFERENCE_TIME m_StatusTimerId;
	CFont m_HeaderFont;
	CFont m_OptionsButtonFont;
	CString m_HelpMsg;
// Dialog Data
	//{{AFX_DATA(CPlayerDlg)
	enum { IDD = IDD_PLAYER_DIALOG };
 	PictureStatic	m_Picture;
	CColorStatic	m_GenresLabel;
	CColorStatic	m_ArtistsLabel;
	CColorStatic	m_AlbumsLabel;
	CColorStatic	m_SongsLabel;	
	CColorStatic	m_PlaylistLabel;
	CExtendedListBox    m_Genres;
	CExtendedListBox	m_Artists;
	CExtendedListBox	m_Albums;
	CExtendedListBox	m_Songs;	
	CExtendedListBox	m_Playlist;    
	CMbutton		m_OptionsButton;
	CBitmapButton	m_ButtonAppLabel;
	CBitmapButton	m_ButtonMinimize;
	CBitmapButton	m_ButtonMaximize;
	CBitmapButton	m_ButtonExit;
	CBitmapButton	m_ButtonRestore;
	SliderCtrl		m_PositionSlider;	
	SliderCtrl		m_VolumeSlider;
	CColorStatic	m_VolumeLabel;
	CColorStatic	m_PositionLabel;
	CColorStatic	m_PlayerStatus;
	CColorStatic	m_CurrentTitle;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerDlg)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation

public:
	HICON m_hIcon;
//protected:
	// Generated message map functions
	//{{AFX_MSG(CPlayerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnPlayButton();
	afx_msg void OnOpenFileButton();
	afx_msg void OnStopButton();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnAbout();
	afx_msg void OnSelchangeArtists();
	afx_msg void OnSelchangeAlbums();
	afx_msg void OnDblclkSongs();
	afx_msg void OnDblclkAlbums();
	afx_msg void OnDblclkArtists();
	afx_msg void OnDblclkGenres();
	afx_msg void OnMenuButton();
	afx_msg void OnMenuOptions();
	afx_msg void OnMenuClearplaylist();
	afx_msg void OnMenuExit();
	afx_msg void OnMenuLoadplaylist();
	afx_msg void OnMenuSaveplaylist();
    afx_msg void OnMenuExportLibrary();
	afx_msg void OnMenuShuffleplaylist();
	afx_msg void OnMenuPause();
	afx_msg void OnMenuPlay();
	afx_msg void OnMenuStop();
	afx_msg void OnMenuHelp();
	afx_msg void OnSelchangeGenres();
	afx_msg void OnDblclkPlaylist();
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnUserEditSong();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSelchangeSongs();
	afx_msg void OnSelchangePlaylist();
	afx_msg void OnMenuRandomizePlaylist();
	afx_msg void OnButtonMenu();
	afx_msg void OnButtonMinimize();
	afx_msg void OnButtonMaximize();
	afx_msg void OnCancel();
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	afx_msg LRESULT OnGraphNotify(UINT wParam, LONG lParam);
	afx_msg LRESULT OnPlayloop(UINT wParam, LONG lParam);
    afx_msg LRESULT OnPostMyIdle(UINT wParam, LONG lParam);
	afx_msg void OnGenresFocus();
	afx_msg void OnArtistsFocus();
	afx_msg void OnAlbumsFocus();
	afx_msg void OnSongsFocus();
	afx_msg void OnPlaylistFocus();
    afx_msg UINT OnNcHitTest( CPoint point );
    afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
    afx_msg void OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct );
    afx_msg void OnTestMenu();
    afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg LRESULT OnSerialMsg (WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

//	AFX_THREADPROC  IRReader( LPVOID pParam );
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERDLG_H__FCF45C46_7793_11D2_9317_0020AFF7E192__INCLUDED_)
