// PlayerDlg.h : header file
//
//{{AFX_INCLUDES()
#include "wmpplayer4.h"
//}}AFX_INCLUDES

#if !defined(AFX_PLAYERDLG_H__FCF45C46_7793_11D2_9317_0020AFF7E192__INCLUDED_)
#define AFX_PLAYERDLG_H__FCF45C46_7793_11D2_9317_0020AFF7E192__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

/////////////////////////////////////////////////////////////////////////////
// Includes

#include "PlayerApp.h"
#include "MusicDb.h"
#include "ExtendedListBox.h"
#include "MBConfig.h"
#include "ColorStatic.h"
#include "PictureStatic.h"
#include "MyString.h"
#include "Controls.h"
#include "ButtonSt.h"
#include "SkinSlider.h"
#include "PlayerCallbacks.h"
#include "Registry.h"

#define CDialogClassImpl CDialogSK

#include "CDialogSK.h"


class VirtualControl;
class VirtualDialog;
class RMenuDlg;
class CTransparentDialogDlg;
class MusicPlayerWMP;
class CPlayerDlg;
class LoadPlaylistDlg;

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

class CPlayerDlg : public CDialogClassImpl
{
// Construction
public:
    MusicPlayerWMP *m_Player;
//	MusicPlayer *m_Player;

//	CPlayerDlg(CPlayerApp *, CTransparentDialogDlg *, CWnd* pParent = NULL);	// standard constructor
	CPlayerDlg(CPlayerApp *, CWnd* pParent = NULL);	// standard constructor
    ~CPlayerDlg();
    CPlayerApp * m_PlayerApp;
	MusicLib m_mlib;
	CString _selectedGenre, 
		_lastSelectedGenre,
		_selectedArtist,
		_lastSelectedArtist,
		_lastSelectedAlbum,
		_selectedAlbum,
        _selectedSong, 
		_selectedPlaylistSong;

	void updatePlaylist(const BOOL save=TRUE);
	void updatePlaylistPostEdit();
	virtual BOOL PreTranslateMessage(MSG*);

    void PlayLoop();
    void initDb();
    void init();
    void SavePlaylist(CString);
    void LoadPlaylist(CString);
    void OnExit();
    void SetSavePlaylistFlag(BOOL flag) { m_SavePlaylistFlag = flag; }
//	void RunTestHarness();
	void calcDuration();

    VirtualControl * m_Control;
    VirtualDialog * m_Dialog;

    void OnControlSelChange();
    BOOL OnControlClick();
    void OnPageUp();
    void OnPageDown();
	void OnVerify();
    void setFont();
	void setColors();
    MBConfig & config() { return m_Config; }
	MBConfig * pconfig() { return &m_Config; }
	void SetSelected(const CString text, DWORD data);
    void PlayerStatusSet(CString &);
	void PlayerStatusClear();
	void PlayerStatusSet(LPCTSTR);
	void CurrentTitleSet(CString &);
	void CurrentTitleSet(LPCTSTR);
    void PlayerStatusTempSet(CString &);
	void PlayerStatusTempSet(LPCTSTR);
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
    void redraw();
	void Need2Erase(BOOL flag) {
		m_Need2Erase = flag;
	}
	void nextDlgCtrl();
	void prevDlgCtrl();
private:
	PlayerCallbacks m_callbacks;
	PlayerCallbacks m_callbacksGenres;
	PlayerCallbacks m_callbacksArtists;
	PlayerCallbacks m_callbacksAlbums;
	PlayerCallbacks m_callbacksSongs;
	Controls m_BtnControls;
	int m_PlaylistDuration;
	CTransparentDialogDlg *_initdialog;

	CString m_AlbumArt;
	UINT m_AdjustLibrary;
	BOOL m_LibDrag;
	CRect m_LibDragRect;
	CPoint m_LibDragPoint;
public:
    MBConfig m_Config;
	CRect getWindowRect() { return m_WindowRect; }
private:
	CRect m_GenresRect,m_GenresLabelRect,
		m_ArtistsRect,m_ArtistsLabelRect,
		m_AlbumsRect,m_AlbumsLabelRect,
		m_SongsRect,m_SongsLabelRect,
		m_GenreArtistAdjust,
		m_ArtistAlbumAdjust,
		m_AlbumSongAdjust,
		m_GenresLabelInt,
		m_ArtistsLabelInt,
		m_AlbumsLabelInt,
		m_SongsLabelInt,
		m_ColHdrsRect,
		m_SearchClearRect,
		m_SearchEditRect,
		m_SearchGoRect,
		m_SearchCancelRect,
		m_SearchStatusRect,
		m_LibraryRect,
		m_WindowRect;

	CString m_CurrentTitleDesc;
	COLORREF m_TransMain;
	COLORREF m_TransPanel;
	CStringList m_Skins;
    CString m_PlayerStatusSave;
    CTime m_PlayerStatusTime;
    RMenuDlg * m_MenuDlg;
	LoadPlaylistDlg * m_LoadPlaylistDlg;
    BOOL m_StopFlag;
	BOOL m_PauseFlag;
    int m_PlaylistCurrent;
	CString m_LastThingQueuedUp;
	BOOL m_Resizing;

    CString m_StatusArray[10];

    int mWindowFlag;
	int m_trialCounter;

    void resetControls();
	void reloadDB();
	void AdjustLibraryWidths(CPoint & point);
    void SaveWindowPos();
    void ReadWindowPos(int &, CRect &);

//	void updateVolumeLabel();
	void adjustVolume(int level);
	void adjustVolume();
	void VolumeGainAdjust(const double rg);
	void updatePositionLabel();
	void adjustPosition();
	void adjustPosition(int pct);
	void resetPosition();
    void PlayerStatusRevert();

	void InputClose();
	void Pause();
	BOOL Play();
	void Stop();
	void saveConfig();
	void readConfig();
	void ShowBetaWarning();
	void HelpInfo(BOOL userormain);
	void recordTLEN();
 	void displayAlbumArt(const CString & file = "", const CString & album="");
 	void killAlbumArt();
	void ShowSearchDlg();
	void UpdateRects();
	void CheckScreenSaver();

    BOOL m_LButtonDown;
    CPoint m_deltaPoint;
    CBrush m_HatchBrush;
	int m_LibHeight;


    CMapStringToString m_GenreArtist;
    CMapStringToString m_ArtistAlbum;
    CMapStringToString m_AlbumSong;
    BOOL m_SavePlaylistFlag;
	UINT m_timerid;
	UINT m_StatusTimerId;
	UINT m_VolumeTimerId;
	UINT m_VolumeGainBase;
	UINT m_PlayLoopTimerId;

	CString m_HelpMsg;
	BOOL m_InitDone;

	BOOL m_Maximized;
	CString m_LastAlbumArtFile;
	CString m_LastGAA;
	BOOL m_FixedSize;
	CPoint m_LastSized;
	HANDLE m_hMutex;
	BOOL m_QuickPlay;
	BOOL m_ShowSearchPanel;
	BOOL m_LastShowSearchPanel;
	BOOL m_SearchCleared;
	RegistryKey m_reg;
	CMapStringToPtr m_CtlColors;
	BOOL m_UpdateNeeded[3];
// Dialog Data
	//{{AFX_DATA(CPlayerDlg)
	enum { IDD = IDD_PLAYER_DIALOG };
	CColorStatic	m_SearchStatus;
	CButtonST	m_SearchClear;
	CButtonST	m_SearchCancel;
	CButtonST	m_SearchGo;
	CEdit		m_SearchEdit;
	CButtonST	m_LogoButton;
	CButtonST	m_ButtonShuffle;
	CButtonST	m_ButtonSave;
	CButtonST	m_ButtonReverse;
	CButtonST	m_ButtonRandom;
	CButtonST	m_ButtonLoad;
	CButtonST	m_ButtonFastForward;
	CButtonST	m_ButtonClear;
	CColorStatic	m_GenresLabel;
	CColorStatic	m_ArtistsLabel;
	CColorStatic	m_AlbumsLabel;
	CColorStatic	m_SongsLabel;	
	CColorStatic	m_PlaylistLabel;
	CColorStatic	m_PositionLabel;
	CColorStatic	m_PlayerStatus;
	CExtendedListBox    m_Genres;
	CExtendedListBox	m_Artists;
	CExtendedListBox	m_Albums;
	CExtendedListBox	m_Songs;	
	CExtendedListBox	m_Playlist;    
	CButtonST		m_OptionsButton;
	CButtonST		m_MusicButton;
	CButtonST		m_PicturesButton;
	CButtonST		m_VideoButton;
	CButtonST		m_ButtonMinimize;
	CButtonST		m_ButtonMaximize;
	CButtonST		m_ButtonExit;
	CButtonST		m_ButtonRestore;
	CButtonST		m_ButtonStop;
	CButtonST		m_ButtonPlay;
	CButtonST		m_ButtonPause;
 	PictureStatic	m_Picture;
	CMySliderCtrl	m_VolumeSlider;
	CMySliderCtrl	m_PositionSlider;
	CWMPPlayer4	m_WMP;
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
	//afx_msg LRESULT OnMusicAdd(UINT wParam, LONG lParam);
	//afx_msg LRESULT OnMusicScan(UINT wParam, LONG lParam);
	//afx_msg LRESULT OnMusicScanNew(UINT wParam, LONG lParam);
	//afx_msg LRESULT OnSkinPic(UINT wParam, LONG lParam);
	//afx_msg void    OnNcPaint( );

	// class wizard pukes on these handlers;
	afx_msg LRESULT   OnDblclkGenres(WPARAM, LPARAM);
	afx_msg LRESULT   OnDblclkArtists(WPARAM, LPARAM);
	afx_msg LRESULT   OnDblclkAlbums(WPARAM, LPARAM);

	//{{AFX_MSG(CPlayerDlg)
	afx_msg void      HelpInfo();
	afx_msg void      OnAbout();
	afx_msg void      OnAddToPlaylist();
	afx_msg void      OnAlbumsFocus();
	afx_msg void      OnArtistsFocus();
	afx_msg void      OnButtonMaximize();
	afx_msg void      OnButtonMenu();
	afx_msg void      OnButtonMinimize();
	afx_msg void      OnCancel();
	afx_msg void      OnCaptureChanged(CWnd *pWnd);
	afx_msg void      OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg HBRUSH    OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	afx_msg void      OnDblclkAlbums();
	afx_msg void      OnDblclkArtists();
	afx_msg void      OnDblclkGenres();
	afx_msg void      OnDblclkPlaylist();
	afx_msg void      OnDblclkSongs();
	afx_msg void      OnDelete();
	afx_msg void      OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct );
	afx_msg void      OnErrorWmp();
	afx_msg void      OnFastForward();
	afx_msg void      OnGenresFocus();
	afx_msg void      OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI );
	afx_msg LRESULT   OnGraphNotify(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL      OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg LRESULT   OnHoverCancelMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg1(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg2(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg3(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg4(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg5(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg6(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg7(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg8(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg9(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg10(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg11(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg12(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg13(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg14(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg15(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg16(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg17(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg18(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg19(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg20(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnHoverMsg21(WPARAM wParam, LPARAM lParam);
	virtual BOOL      OnInitDialog();
	afx_msg void      OnInitMenuPopup(CMenu *pPopupMenu, UINT nIndex,BOOL bSysMenu);
	afx_msg void      OnKillfocusVirtuals();
	afx_msg void      OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void      OnLogoButton();
	afx_msg void      OnMediaErrorWmp(LPDISPATCH pMediaObject);
	afx_msg void      OnMenuButton();
	afx_msg void      OnMenuCheckem();
	afx_msg void      OnMenuClearplaylist();
	afx_msg void      OnAlphaUp();
	afx_msg void      OnAlphaDown();
	afx_msg void      OnMenuEditPlaylist();
	afx_msg void      OnMenuExit();
	afx_msg void      OnMenuExportLibrary();
	afx_msg void      OnMenuHelp();
	afx_msg void      OnMenuLoadplaylist();
	afx_msg void      OnMenuOptions();
	afx_msg void      OnMenuPause();
	afx_msg void      OnMenuPlay();
	afx_msg void      OnMenuRandomizePlaylist();
	afx_msg void      OnMenuSaveplaylist();
	afx_msg void      OnMenuShuffleplaylist();
	afx_msg void      OnMenuStop();
	afx_msg void      OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT   OnMovePlaylistDn(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT   OnMovePlaylistUp(WPARAM wParam, LPARAM lParam);
	afx_msg void      OnMusicAdd();
	afx_msg void      OnMusicButton();
	afx_msg void      OnMusicScan();
	afx_msg void      OnMusicScanNew();
	afx_msg UINT      OnNcHitTest( CPoint point );
	afx_msg void      OnNcLButtonDown(UINT, CPoint);
	afx_msg void      OnNcLButtonUp(UINT, CPoint);
	afx_msg void      OnNcMouseMove(UINT nFlags, CPoint point);
	afx_msg void      OnNoSearchDlg();
    afx_msg void      OnNextSong();
	afx_msg void      OnOpenFileButton();
	afx_msg void      OnPaint();
	afx_msg void      OnPicturesButton();
	afx_msg void      OnPlayButton();
	afx_msg void      OnPlaylistFocus();
	afx_msg LRESULT   OnPlayloop(WPARAM wParam, LPARAM lParam);
	afx_msg void      OnPlayStateChangeWmp(long NewState);
	afx_msg void      OnPositionChangeWmp(double oldPosition, double newPosition);
	afx_msg LRESULT   OnPostMyIdle(WPARAM wParam, LPARAM lParam);
    afx_msg void      OnPreviousSong();
	afx_msg LRESULT   OnProgress(WPARAM wParam, LPARAM lParam);
	afx_msg HCURSOR   OnQueryDragIcon();
	afx_msg void      OnQuickPlay();
	afx_msg void      OnReverse();
	afx_msg void      OnSearchClear();
	afx_msg void      OnSearchDlg();
	afx_msg void      OnSearchGo();
	afx_msg void      OnSelchangeAlbums();
	afx_msg void      OnSelchangeArtists();
	afx_msg void      OnSelchangeGenres();
	afx_msg void      OnSelchangePlaylist();
	afx_msg void      OnSelchangeSongs();
	afx_msg LRESULT   OnSerialMsg (WPARAM wParam, LPARAM lParam);
	afx_msg BOOL      OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void      OnSize(UINT nType, int cx, int cy);
	afx_msg void      OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void      OnSkinPic(UINT wParam);
	afx_msg void      OnSongsFocus();
	afx_msg void      OnStopButton();
	afx_msg void      OnSysCommand(UINT nID, LONG lParam);
	afx_msg void      OnTestMenu();
	afx_msg void      OnTimer(UINT nIDEvent);
	afx_msg void      OnUserEditSong();
	afx_msg void      OnVideoButton();
	afx_msg void      OnVolUp();
	afx_msg void      OnVolDown();
	afx_msg LRESULT   OnVolume(WPARAM wParam, LPARAM lParam);
	afx_msg void      OnWarningWmp(long WarningType, long Param, LPCTSTR Description);
	afx_msg void OnDeviceSyncErrorWmp(LPUNKNOWN pDevice, LPDISPATCH pMedia);
	afx_msg void OnStatusChangeWmp();
	afx_msg void OnSwitchedToControlWmp();
	afx_msg void OnSwitchedToPlayerApplicationWmp();
	afx_msg void OnEndOfStreamWmp(long Result);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
public:
	BOOL UpdateGenres();
	BOOL UpdateArtists();
	BOOL UpdateAlbums();
	BOOL UpdateSongs();
	void RebuildOnly(const CString &dir) { 
		m_RebuildOnly = TRUE;
		m_RebuildDir = dir;
	}
protected:
	BOOL m_RebuildOnly;
	CString m_RebuildDir;
	BOOL m_CheckingEm;
	int m_CheckSongCount;
	int m_CheckSongCounter;
	int m_CheckGoodCount;
	int m_CheckBadCount;
	BOOL m_Ready2Reset;
	int m_LibRemainingWidth;
	BOOL m_ModifyDelete;
	BOOL m_HandlingIRMessage;

	DECLARE_MESSAGE_MAP()

//	AFX_THREADPROC  IRReader( LPVOID pParam );
};



#endif // !defined(AFX_PLAYERDLG_H__FCF45C46_7793_11D2_9317_0020AFF7E192__INCLUDED_)


