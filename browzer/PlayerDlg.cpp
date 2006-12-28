
// PlayerDlg.cpp : implementation file


#include "stdafx.h"
#define THE_PLAYER
#include "PlayerDlg.h"

//#define LOGOWNER
#include "MyLog.h"

#include "About.h"
#include "ConfigFiles.h"
#include "RemoteReceiver.h"
#include "DialogData.h"
#include "FontCombo.h"
#include "FontTipWnd.h"
#include "HtmlHelp.h"
#include "id3/misc_support.h"
#include "IRCodes.h"
#include "Registry.h"
#include "LoadPlaylistDlg.h"
#include "MBConfig.h"
#include "MBGlobals.h"
#include "MBMessageBox.h"
#include "ModifyIDThree.h"
#include "MusicDb.h"
#include "MyID3LibMiscSupport.h"
#include "WmaTagger.h"
#include "PlayerApp.h"
#include "SavePlaylistDlg.h"
#include "TestHarness/TestHarness.h"
#include "VirtualControl.h"
#include "muzikbrowzerVersion.h"
#include "TransparentDialogDlg.h"
#include "util/Misc.h"
#include "FileUtils.h"
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "SkinDefs.h"
#include "DIBSectionLite.h"
#include "GetTextField.h"
#include "MusicPlayerWMP.h"
#include "ExportDlg.h"
#include "oggtagger/oggtagger.h"
#include "FileAndFolder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

MyLog CheckEmLog;

// This DBLOCKED global used to prevent access to the _selected*'s vars
// while the db is being updated and before the _selected*'s
// get updated. Introduced when On*Focus routines began calling
// OnSelectchange*'s routines. This to update the album art
// upon focus change between playlist and library.
BOOL DBLOCKED = FALSE;

void Redraw() {
	thePlayer->redraw();

}
CString MBDir() {
	return thePlayer->m_Config.mbdir();
}
void initDb() {
	thePlayer->init();
}
CString GetLibraryCounts() {
	return thePlayer->m_mlib.getLibraryCounts();
}
void PlayerStatusSet(CString text) {
	thePlayer->PlayerStatusSet(text);
}
void PlayerStatusTempSet(CString text) {
	thePlayer->PlayerStatusTempSet(text);
}
void SetSelected(const CString text, DWORD data)
{
	thePlayer->SetSelected(text, data);
}
void PlayerUpdateWindow() {
	thePlayer->UpdateWindow();
}
void SetDbLocation(CString path) {
	thePlayer->m_mlib.setDbLocation(path);
}

void Need2Erase(BOOL need) {
	thePlayer->Need2Erase(need);
}
MBConfig * mbconfig() {
	return thePlayer->pconfig();
}
MusicLib * musiclib() {
	return &thePlayer->m_mlib;
}
CPlayerDlg * getPlayer() {
	return thePlayer;
}
CDialog * getDialog() {
	return thePlayer;
}
BOOL UpdateArtists() {
	return thePlayer->UpdateArtists();
}
BOOL UpdateAlbums() {
	return thePlayer->UpdateAlbums();
}
BOOL UpdateSongs() {
	return thePlayer->UpdateSongs();
}
/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog

CPlayerDlg::CPlayerDlg(CPlayerApp * theApp, 
					   CWnd* pParent /*=NULL*/)
	: CDialogClassImpl(CPlayerDlg::IDD, pParent), m_PlayerApp(theApp), 
//	_initdialog(ip),
	m_mlib(), m_PlaylistCurrent(-1),
    m_MenuDlg(0), m_StopFlag(FALSE), m_PauseFlag(FALSE),m_LButtonDown(FALSE),
    m_Config("Configuration"), m_SavePlaylistFlag(TRUE),
	m_PlaylistDuration(0), m_timerid(0), m_StatusTimerId(0),
	m_Control(new VirtualControl), m_Dialog(new VirtualDialog),
	m_trialCounter(0),
	m_InitDone(FALSE),
	m_Genres(TRUE,"Genres",TRUE, &m_callbacksGenres),
	m_Artists(TRUE,"Artists",TRUE, &m_callbacksArtists),
	m_Albums(TRUE,"Albums",TRUE, &m_callbacksAlbums),
	m_Songs(TRUE,"songs",TRUE, &m_callbacksSongs),
	m_Playlist(TRUE, "Playlist",TRUE, &m_callbacks),
	m_Maximized(FALSE),
	m_AdjustLibrary(0),
	m_LibDrag(FALSE),
	m_Resizing(FALSE),
	m_FixedSize(FALSE),
	m_LastSized(0,0),
	m_QuickPlay(FALSE),
	m_ShowSearchPanel(FALSE),
	m_SearchCleared(TRUE),
	m_LastShowSearchPanel(FALSE),
	m_PlayLoopTimerId(0),
	m_LoadPlaylistDlg(0),
	m_WindowRect(0,0,640,480),
	m_Ready2Reset(FALSE),
	m_RebuildOnly(FALSE),
	m_ModifyDelete(FALSE),
	m_HandlingIRMessage(FALSE)
{
	//{{AFX_DATA_INIT(CPlayerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
		m_bPlayer=TRUE;
    m_Playlist.m_reorder = TRUE;
	// Changed this in April 2006, finding it now in Nov. Checking it in. We'll see!
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
//    m_hIcon = AfxGetApp()->LoadIcon(IDI_MBLOGO3232);
	thePlayer = this;
	
	m_callbacks.redraw = &::Redraw;
	m_callbacks.mbdir = &::MBDir;
	m_callbacks.initDb = &::initDb;
	m_callbacks.getLibraryCounts = &::GetLibraryCounts;
	m_callbacks.statusset = &::PlayerStatusSet;
	m_callbacks.statustempset = &::PlayerStatusTempSet;
	m_callbacks.UpdateWindow = &::PlayerUpdateWindow;
	m_callbacks.setDbLocation = &::SetDbLocation;
	m_callbacks.Need2Erase = &::Need2Erase;
	m_callbacks.mbconfig = &::mbconfig;
	m_callbacks.musiclib = &::musiclib;
	m_callbacks.playerdlg = &::getPlayer;
	m_callbacks.dlg = &::getDialog;

	memcpy(&m_callbacksGenres,&m_callbacks,sizeof(m_callbacks));
	memcpy(&m_callbacksArtists,&m_callbacks,sizeof(m_callbacks));
	memcpy(&m_callbacksAlbums,&m_callbacks,sizeof(m_callbacks));
	memcpy(&m_callbacksSongs,&m_callbacks,sizeof(m_callbacks));
	m_callbacksArtists.OnPaintCallback = &::UpdateArtists;
	m_callbacksAlbums.OnPaintCallback = &::UpdateAlbums;
	m_callbacksSongs.OnPaintCallback = &::UpdateSongs;
	m_callbacksSongs.SetSelected = &::SetSelected;
	m_callbacksAlbums.SetSelected = &::SetSelected;
	m_callbacksSongs.statustempset = NULL;
	m_callbacksAlbums.statustempset = NULL;

	m_HatchBrush.CreateHatchBrush(HS_CROSS,RGB(255,0,0));
//	m_LogoButton.logit = TRUE;

	m_UpdateNeeded[0] = FALSE;
	m_UpdateNeeded[1] = FALSE;
	m_UpdateNeeded[2] = FALSE;
	m_CheckingEm = FALSE;
}
CPlayerDlg::~CPlayerDlg() {
    //delete m_Config;

	m_HatchBrush.DeleteObject();
	delete m_Control;
	delete m_Dialog;
	delete m_Player;

    // When this DeleteObject is not done, I get one GDI leak (BoundsChecker)
	// from the allocation in OnCtlColor.
	// When this DeleteObject is done, I get one GDI error that the DC
	// still contains non default object(s)
	POSITION pos;
	HBRUSH hbr;
	CString key;
	for( pos = m_CtlColors.GetStartPosition(); pos != NULL; ) {
		m_CtlColors.GetNextAssoc( pos, key, (void*&)hbr);
		BOOL r = ::DeleteObject((HBRUSH)hbr);
	}
	RemoteReceiver::destroy();
}


void CPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogClassImpl::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPlayerDlg)
	DDX_Control(pDX, IDC_SEARCH_STATUS,				m_SearchStatus);
	DDX_Control(pDX, IDC_SEARCH_CLEAR,				m_SearchClear);
	DDX_Control(pDX, IDC_SEARCH_CANCEL,				m_SearchCancel);
	DDX_Control(pDX, IDC_SEARCH_GO,					m_SearchGo);
	DDX_Control(pDX, IDC_SEARCH_TEXT,				m_SearchEdit);
	DDX_Control(pDX, IDC_BUTTON_LOGO,				m_LogoButton);
	DDX_Control(pDX, IDC_BUTTON_SHUFFLE,			m_ButtonShuffle);
	DDX_Control(pDX, IDC_BUTTON_SAVE,				m_ButtonSave);
	DDX_Control(pDX, IDC_BUTTON_REVERSE,			m_ButtonReverse);
	DDX_Control(pDX, IDC_BUTTON_RANDOM,				m_ButtonRandom);
	DDX_Control(pDX, IDC_BUTTON_LOAD,				m_ButtonLoad);
	DDX_Control(pDX, IDC_BUTTON_FASTFORWARD,		m_ButtonFastForward);
	DDX_Control(pDX, IDC_BUTTON_CLEAR,				m_ButtonClear);
	DDX_Control(pDX, IDC_GENRESLABEL,               m_GenresLabel);
	DDX_Control(pDX, IDC_ARTISTSLABEL,              m_ArtistsLabel);
	DDX_Control(pDX, IDC_ALBUMSLABEL,               m_AlbumsLabel);
	DDX_Control(pDX, IDC_SONGSLABEL,                m_SongsLabel);
	DDX_Control(pDX, IDC_PLAYLISTLABEL,             m_PlaylistLabel);
	DDX_Control(pDX, IDC_POSITION_LABEL,            m_PositionLabel);
	DDX_Control(pDX, IDC_PLAYER_STATUS,             m_PlayerStatus);
	DDX_Control(pDX, IDC_GENRES,                    m_Genres);
	DDX_Control(pDX, IDC_ARTISTS,                   m_Artists);
	DDX_Control(pDX, IDC_ALBUMS,                    m_Albums);
	DDX_Control(pDX, IDC_SONGS,                     m_Songs);
	DDX_Control(pDX, IDC_PLAYLIST,                  m_Playlist);
	DDX_Control(pDX, IDC_OPTIONS_BUTTON,            m_OptionsButton);
	DDX_Control(pDX, IDC_MUSIC_BUTTON,				m_MusicButton);
	DDX_Control(pDX, IDC_PICTURES_BUTTON,           m_PicturesButton);
	DDX_Control(pDX, IDC_VIDEO_BUTTON,				m_VideoButton);
	DDX_Control(pDX, IDC_BUTTON_MINIMIZE,           m_ButtonMinimize);
	DDX_Control(pDX, IDC_BUTTON_MAXIMIZE,           m_ButtonMaximize);
	DDX_Control(pDX, IDC_BUTTON_EXIT,               m_ButtonExit);
	DDX_Control(pDX, IDC_BUTTON_RESTORE,            m_ButtonRestore);
	DDX_Control(pDX, IDC_BUTTON_STOP,               m_ButtonStop);
	DDX_Control(pDX, IDC_BUTTON_PLAY,               m_ButtonPlay);
	DDX_Control(pDX, IDC_BUTTON_PAUSE,              m_ButtonPause);
	DDX_Control(pDX, IDC_PICTURE_CTRL,              m_Picture);
	DDX_Control(pDX, IDC_VOLUME_SLIDER,             m_VolumeSlider);
	DDX_Control(pDX, IDC_POSITION_SLIDER,			m_PositionSlider);
	DDX_Control(pDX, IDC_WMP, m_WMP);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlayerDlg, CDialogClassImpl)
	//ON_COMMAND(ID_MENU_CHECKEM,			OnMenuCheckem)
	//ON_WM_NCPAINT()                        
	// Don't *think* these are used: 12/7/2006
	//ON_WM_COMPAREITEM()                      
	//ON_WM_RBUTTONDOWN()                      
	//ON_WM_RBUTTONUP()                        
	ON_MESSAGE(MB_DBLCLICK_GENRES,           OnDblclkGenres)
	ON_MESSAGE(MB_DBLCLICK_ARTISTS,          OnDblclkArtists)
	ON_MESSAGE(MB_DBLCLICK_ALBUMS,           OnDblclkAlbums)
	//{{AFX_MSG_MAP(CPlayerDlg)
	ON_WM_CAPTURECHANGED()                   
	ON_WM_CONTEXTMENU()                      
	ON_WM_CTLCOLOR()                         
	ON_WM_GETMINMAXINFO()                    
	ON_WM_HELPINFO()                         
	ON_WM_INITMENUPOPUP()                    
	ON_WM_LBUTTONUP()                        
	ON_WM_MOUSEMOVE()                        
	ON_WM_NCHITTEST()                        
	ON_WM_NCLBUTTONDOWN()                    
	ON_WM_NCLBUTTONUP()                      
	ON_WM_NCMOUSEMOVE()                      
	ON_WM_PAINT()                            
	ON_WM_QUERYDRAGICON()                    
	ON_WM_SETCURSOR()                        
	ON_WM_SIZE()                             
	ON_WM_SIZING()                           
	ON_WM_SYSCOMMAND()                       
	ON_WM_TIMER()                            
	ON_COMMAND(ID_MENU_HELP,                 HelpInfo)
	ON_BN_CLICKED(IDD_ABOUT,                 OnAbout)
	ON_COMMAND(ID_ADDTO_PLAYLIST,            OnAddToPlaylist)
	ON_LBN_SETFOCUS(IDC_ALBUMS,              OnAlbumsFocus)
	ON_LBN_SETFOCUS(IDC_ARTISTS,             OnArtistsFocus)
	ON_BN_CLICKED(IDC_BUTTON_MAXIMIZE,       OnButtonMaximize)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE,        OnButtonMaximize)
	ON_BN_CLICKED(IDC_OPTIONS_BUTTON,        OnButtonMenu)
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE,       OnButtonMinimize)
	ON_BN_CLICKED(IDC_BUTTON_EXIT,           OnCancel)
	ON_LBN_DBLCLK(IDC_ALBUMS,                OnDblclkAlbums)
	ON_LBN_DBLCLK(IDC_ARTISTS,               OnDblclkArtists)
	ON_LBN_DBLCLK(IDC_GENRES,                OnDblclkGenres)
	ON_LBN_DBLCLK(IDC_PLAYLIST,              OnDblclkPlaylist)
	ON_LBN_DBLCLK(IDC_SONGS,                 OnDblclkSongs)
	ON_COMMAND(ID_DELETE_FROM_PL,            OnDelete)
	ON_COMMAND(ID_DELETE_FROM_LIBRARY,       OnDelete)
	ON_COMMAND(ID__PLAYER_FASTFORWARD,       OnFastForward)
	ON_LBN_SETFOCUS(IDC_GENRES,              OnGenresFocus)
	ON_MESSAGE(WM_GRAPHNOTIFY,               OnGraphNotify)
	ON_MESSAGE(MB_HOVER_CANCEL_MSG,          OnHoverCancelMsg)
	ON_MESSAGE(MB_HOVER_CLEAR_MSG,           OnHoverMsg1)
	ON_MESSAGE(MB_HOVER_EXIT_MSG,            OnHoverMsg2)
	ON_MESSAGE(MB_HOVER_LOAD_MSG,            OnHoverMsg3)
	ON_MESSAGE(MB_HOVER_MAXIMIZE_MSG,        OnHoverMsg4)
	ON_MESSAGE(MB_HOVER_MENU_MSG,            OnHoverMsg5)
	ON_MESSAGE(MB_HOVER_MINIMIZE_MSG,        OnHoverMsg6)
	ON_MESSAGE(MB_HOVER_NEXT_MSG,            OnHoverMsg7)
	ON_MESSAGE(MB_HOVER_PAUSE_MSG,           OnHoverMsg8)
	ON_MESSAGE(MB_HOVER_PLAY_MSG,            OnHoverMsg9)
	ON_MESSAGE(MB_HOVER_POSITION_MSG,        OnHoverMsg10)
	ON_MESSAGE(MB_HOVER_PREVIOUS_MSG,        OnHoverMsg11)
	ON_MESSAGE(MB_HOVER_RANDOM_MSG,          OnHoverMsg12)
	ON_MESSAGE(MB_HOVER_RESTORE_MSG,         OnHoverMsg13)
	ON_MESSAGE(MB_HOVER_SAVE_MSG,            OnHoverMsg14)
	ON_MESSAGE(MB_HOVER_SHUFFLE_MSG,         OnHoverMsg15)
	ON_MESSAGE(MB_HOVER_STOP_MSG,            OnHoverMsg16)
	ON_MESSAGE(MB_HOVER_VOLUME_MSG,          OnHoverMsg17)
	ON_MESSAGE(MB_HOVER_WWW_MSG,             OnHoverMsg18)
	ON_MESSAGE(MB_HOVER_SEARCH_MSG,          OnHoverMsg19)
	ON_MESSAGE(MB_HOVER_SEARCHCLEAR_MSG,     OnHoverMsg20)
	ON_MESSAGE(MB_HOVER_SEARCHCLOSE_MSG,     OnHoverMsg21)
	ON_LBN_KILLFOCUS(IDC_ALBUMS,             OnKillfocusVirtuals)
	ON_LBN_KILLFOCUS(IDC_ARTISTS,            OnKillfocusVirtuals)
	ON_LBN_KILLFOCUS(IDC_GENRES,             OnKillfocusVirtuals)
	ON_LBN_KILLFOCUS(IDC_PLAYLIST,           OnKillfocusVirtuals)
	ON_LBN_KILLFOCUS(IDC_SONGS,              OnKillfocusVirtuals)
	ON_BN_CLICKED(IDC_BUTTON_LOGO,           OnLogoButton)
	ON_BN_CLICKED(IDC_MENU_BUTTON,           OnMenuButton)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR,          OnMenuClearplaylist)
	ON_COMMAND(ID_MENU_CLEARPLAYLIST,        OnMenuClearplaylist)
	ON_COMMAND(ID_PMENU_CLEAR,               OnMenuClearplaylist)
	ON_COMMAND(ID__PLAYER_CLEAR,             OnMenuClearplaylist)
	ON_COMMAND(ID_ALPHA_UP,					 OnAlphaUp)
	ON_COMMAND(ID_ALPHA_DOWN,				 OnAlphaDown)
	ON_COMMAND(ID_MENU_EDITPL,               OnMenuEditPlaylist)
	ON_COMMAND(ID_MENU_EXIT,                 OnMenuExit)
	ON_COMMAND(ID_PMENU_EXIT,                OnMenuExit)
	ON_COMMAND(ID_EXPORT_TAGS,               OnMenuExportLibrary)
	ON_COMMAND(ID_PMENU_HELP,                OnMenuHelp)
	ON_BN_CLICKED(IDC_BUTTON_LOAD,           OnMenuLoadplaylist)
	ON_COMMAND(ID_MENU_LOADPLAYLIST,         OnMenuLoadplaylist)
	ON_COMMAND(ID_PMENU_LOADPLAYLIST,        OnMenuLoadplaylist)
	ON_COMMAND(ID__PLAYER_LOAD,              OnMenuLoadplaylist)
	ON_COMMAND(ID_MENU_OPTIONS,              OnMenuOptions)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE,          OnMenuPause)
	ON_COMMAND(ID_MENU_PAUSE,                OnMenuPause)
	ON_COMMAND(ID_PMENU_PAUSE,               OnMenuPause)
	ON_COMMAND(ID__PLAYER_PAUSE,             OnMenuPause)
	ON_BN_CLICKED(IDC_BUTTON_PLAY,           OnMenuPlay)
	ON_COMMAND(ID_MENU_PLAY,                 OnMenuPlay)
	ON_COMMAND(ID_PMENU_PLAY,                OnMenuPlay)
	ON_COMMAND(ID__PLAYER_PLAY,              OnMenuPlay)
	ON_BN_CLICKED(IDC_BUTTON_RANDOM,         OnMenuRandomizePlaylist)
	ON_COMMAND(ID_MENU_RANDOMIZE_PLAYLIST,   OnMenuRandomizePlaylist)
	ON_COMMAND(ID__PLAYER_RANDOMIZE,         OnMenuRandomizePlaylist)
	ON_BN_CLICKED(IDC_BUTTON_SAVE,           OnMenuSaveplaylist)
	ON_COMMAND(ID_MENU_SAVEPLAYLIST,         OnMenuSaveplaylist)
	ON_COMMAND(ID__PLAYER_SAVE,              OnMenuSaveplaylist)
	ON_BN_CLICKED(IDC_BUTTON_SHUFFLE,        OnMenuShuffleplaylist)
	ON_COMMAND(ID_MENU_SHUFFLEPLAYLIST,      OnMenuShuffleplaylist)
	ON_COMMAND(ID_PMENU_SHUFFLE,             OnMenuShuffleplaylist)
	ON_COMMAND(ID__PLAYER_SHUFFLE,           OnMenuShuffleplaylist)
	ON_BN_CLICKED(IDC_BUTTON_STOP,           OnMenuStop)
	ON_COMMAND(ID_MENU_STOP,                 OnMenuStop)
	ON_COMMAND(ID_PMENU_STOP,                OnMenuStop)
	ON_COMMAND(ID__PLAYER_STOP,              OnMenuStop)
	ON_MESSAGE(MB_LISTMOVEDN,                OnMovePlaylistDn)
	ON_MESSAGE(MB_LISTMOVEUP,                OnMovePlaylistUp)
	ON_COMMAND(ID_MENU_ADD,                  OnMusicAdd)
	ON_BN_CLICKED(IDC_MUSIC_BUTTON,          OnMusicButton)
	ON_COMMAND(ID_MENU_SCAN,                 OnMusicScan)
	ON_COMMAND(ID_MENU_SCANNEW,              OnMusicScanNew)
	ON_BN_CLICKED(IDC_BUTTON_FASTFORWARD,    OnNextSong)
	ON_COMMAND(ID__PLAYER_NEXTSONG,          OnNextSong)
	ON_BN_CLICKED(IDC_SEARCH_CANCEL,         OnNoSearchDlg)
	ON_BN_CLICKED(ID_OPEN_FILE_BUTTON,       OnOpenFileButton)
	ON_BN_CLICKED(IDC_PICTURES_BUTTON,       OnPicturesButton)
	ON_BN_CLICKED(IDC_PLAY_BUTTON,           OnPlayButton)
	ON_LBN_SETFOCUS(IDC_PLAYLIST,            OnPlaylistFocus)
	ON_MESSAGE(WM_PLAYLOOP,                  OnPlayloop)
	ON_MESSAGE(MB_POST_MYIDLE_MESSAGE,       OnPostMyIdle)
	ON_BN_CLICKED(IDC_BUTTON_REVERSE,        OnPreviousSong)
	ON_COMMAND(ID__PLAYER_PREVIOUSSONG,      OnPreviousSong)
	ON_MESSAGE(MB_PROGRESS_MSG,              OnProgress)
	ON_COMMAND(ID_QUICK_PLAY,                OnQuickPlay)
	ON_COMMAND(ID__PLAYER_REVERSESKIP,       OnReverse)
	ON_BN_CLICKED(IDC_SEARCH_CLEAR,          OnSearchClear)
	ON_COMMAND(ID_SEARCH,                    OnSearchDlg)
	ON_BN_CLICKED(IDC_SEARCH_GO,             OnSearchGo)
	ON_LBN_SELCHANGE(IDC_ALBUMS,             OnSelchangeAlbums)
	ON_LBN_SELCHANGE(IDC_ARTISTS,            OnSelchangeArtists)
	ON_LBN_SELCHANGE(IDC_GENRES,             OnSelchangeGenres)
	ON_LBN_SELCHANGE(IDC_PLAYLIST,           OnSelchangePlaylist)
	ON_LBN_SELCHANGE(IDC_SONGS,              OnSelchangeSongs)
	ON_MESSAGE(MB_SERIAL_MESSAGE,            OnSerialMsg)
	ON_COMMAND_RANGE(MB_SKINPICS_MSGS_BEGIN,MB_SKINPICS_MSGS_END, OnSkinPic)
	ON_LBN_SETFOCUS(IDC_SONGS,               OnSongsFocus)
	ON_BN_CLICKED(IDC_STOP_BUTTON,           OnStopButton)
	ON_COMMAND(ID_MYTEST,                    OnTestMenu)
	ON_COMMAND(ID_EDIT_ID3TAG,               OnUserEditSong)
	ON_BN_CLICKED(IDC_VIDEO_BUTTON,          OnVideoButton)
	ON_COMMAND(ID__PLAYER_VOLUMEDOWN,        OnVolDown)
	ON_COMMAND(ID__PLAYER_VOLUMEUP,          OnVolUp)
	ON_MESSAGE(MB_VOLUME_MSG,                OnVolume)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg message handlers
void initFont(CWnd *);
void changeFont(CWnd *, CFont &);
BOOL CPlayerDlg::OnInitDialog()
{

#ifdef _DEBUG
#ifndef _DEBUGBC
//	MBUtil::system(this,"rm /cygdrive/c/mkm/bmps/*");
//	Sleep(1000);
//	logger.ods("system done and slept");
#endif
#endif

    *m_Dialog = this;
	m_mlib.readDbLocation();

	m_Config.createit(this, &m_callbacks);

	if (m_RebuildOnly) {
		CStringArray dirs,excludes;
		m_Config.GetDirs(dirs,excludes,m_RebuildDir);
		CString saveDbLoc;
		if (m_RebuildDir.GetLength()) {
			saveDbLoc = m_mlib.getDbLocation();
			m_mlib.setDbLocation(m_RebuildDir); // writes to Registry
		}
		m_mlib.RebuildOnly(dirs,excludes);
		if (m_RebuildDir.GetLength()) {
			m_mlib.setDbLocation(saveDbLoc);	// writes to Registry
		}
		exit(0);
		return FALSE;
	}

	logger.ods("Begin InitDialog");
	CDialogClassImpl::OnInitDialog();
	CWaitCursor c;
	m_SearchClear.ShowWindow(SW_HIDE);
	m_SearchCancel.ShowWindow(SW_HIDE);
	m_SearchGo.ShowWindow(SW_HIDE);
	m_SearchEdit.ShowWindow(SW_HIDE);
	m_SearchStatus.ShowWindow(SW_HIDE);
	


	// this is to remain hard coded at 255,0,0 8/2/05
	_initdialog = new CTransparentDialogDlg(NULL, RGB(255,0,0));
    CString msg = MUZIKBROWZER;
    msg += " initializing";
	_initdialog->ShowWindow(SW_SHOWNORMAL /* CWinApp::m_nCmdShow */);
	_initdialog->UpdateWindow();

	CString skinname = m_Config.getCurrentSkin();
	if (!m_Config.verifySkin(skinname,FALSE)) {
		if (!m_Config.findGoodSkin()) {
			MBMessageBox("Alert","Muzikbrowzer is unable to locate it's skins and cannot proceed.\r\nYou may wish to reinstall Muzikbrowzer.",TRUE);
			exit(0);
		}
	}

	MBCONFIG_READ_SKIN_DEFS(m_Config,m_reg);

	MBCONFIG_READ_TRANS_COLORS(m_reg,m_TransMain,m_TransPanel);

	if (FileUtil::IsReadable("Muzikbrowzer.wav")) {
		::PlaySound("Muzikbrowzer.wav",NULL,
			SND_ASYNC | SND_FILENAME | SND_NODEFAULT | SND_NOWAIT);
	}
	MyUtil::seed(); // just do this once at startup for any/all rand calls

// App mutex
	CString amemsg,ammsg;
	
	// These mutex keys must match those in Inno/Release.iss
	// One user session and one global, from an Inno tip

#define MUZIKBROWZERAPPMUTEX "MuzikbrowzerAppMutex"
#define MUZIKBROWZERAPPMUTEXGLOBAL "Global\\MuzikbrowzerAppMutex"

	PSECURITY_DESCRIPTOR psd = malloc(sizeof(SECURITY_DESCRIPTOR));
	SECURITY_ATTRIBUTES sa ;

	BOOL isdresult = InitializeSecurityDescriptor(psd, 
		SECURITY_DESCRIPTOR_REVISION);
	DWORD e = ::GetLastError();
	if (isdresult == 0) {
		amemsg += "InitializeSecurityDescriptor error: ";
		amemsg += MBFormatError(e);
		logger.log(amemsg);
	}

	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.lpSecurityDescriptor = psd;
	sa.bInheritHandle = FALSE;

	// BoundsChecker says this is a resource leak
	// I tried closing it with CloseHandle and got an abort
	HANDLE m_hMutex = CreateMutex(&sa,FALSE, MUZIKBROWZERAPPMUTEXGLOBAL);
	e = GetLastError();
	if (NULL == m_hMutex || ERROR_ALREADY_EXISTS == e) {
		amemsg = "CreateMutex " + CString(MUZIKBROWZERAPPMUTEXGLOBAL);
		amemsg += "\r\n";
		amemsg += MBFormatError(e);
		amemsg += "\r\n";
		m_hMutex = CreateMutex(&sa,FALSE, MUZIKBROWZERAPPMUTEX);
		e = GetLastError();
		logger.log(amemsg);
	}

	if (NULL == m_hMutex || ERROR_ALREADY_EXISTS == e) {
		ammsg = ("It looks like ");
		ammsg += MUZIKBROWZER;
		ammsg += " is already running.\r\nYou should only run one instance at a time.\r\nContinue at your own risk.\r\n";
		amemsg = "CreateMutex " + CString(MUZIKBROWZERAPPMUTEX);
		amemsg += "\r\n";
		amemsg += MBFormatError(e);
		amemsg += "\r\n";
		int r = MBMessageBox(MUZIKBROWZER, ammsg, TRUE, TRUE);
		if (0 == r) {
			amemsg += "user aborted";
			logger.log(amemsg);
			_exit(0);
		} else {
			amemsg += "user continued";
		}
		logger.log(amemsg);
	}
	free(psd);
// end of AppMutex

	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	msg = "GetVersionEx=";
	msg += osvi.dwPlatformId;
	logger.log(msg);

	m_mlib.MovePlaylistsToDir();
    
	if (m_mlib.init()) {
		MBMessageBox("Error", "Database corrupted. Rebuild it by performing a\r\nComplete Rebuild.");
		PlayerStatusSet(CString(
			"Database corrupted. Do a Complete Rebuild."));
	}

    // set some of the control's properties
	// progress slider must be 0 to 100 in order for percentage
	// stuff to work - see updatePosition
    m_PositionSlider.SetRange(0,100);
	m_VolumeSlider.SetRange(0,50);

    // instanciate a Player
	m_Player = new MusicPlayerWMP(&m_WMP);
	int wmpver = m_Player->init();
	if (wmpver < 9) {
		MBMessageBox("Alert", 
			"Muzikbrowzer requires Windows Media Player 9 or later",TRUE);
	}


    // sets title in taskbar
    SetWindowText(MUZIKBROWZER); // needed if you used resource editor to remove title bar	
    ModifyStyle(WS_CAPTION,WS_SYSMENU,SWP_FRAMECHANGED);

	m_GenresLabel.setText("Genres");
	m_ArtistsLabel.setText("Artists");
	m_AlbumsLabel.setText("Albums");
	m_SongsLabel.setText("Songs");
	m_PlaylistLabel.setText("Playlist");

    CRect rect;
    int max;
    ReadWindowPos(max, rect);
    if (max) {
		m_Maximized = TRUE;
        ShowWindow(SW_SHOWMAXIMIZED);
		m_ButtonMaximize.ShowWindow(SW_HIDE);
		m_ButtonRestore.ShowWindow(SW_NORMAL);
    } else if (rect.TopLeft().x != 0) {
        MoveWindow(rect, TRUE );
		m_Maximized = FALSE;
		m_ButtonMaximize.ShowWindow(SW_NORMAL);
		m_ButtonRestore.ShowWindow(SW_HIDE);
    }
	m_MusicButton.ShowWindow(SW_HIDE);
	m_VideoButton.ShowWindow(SW_HIDE);
	m_PicturesButton.ShowWindow(SW_HIDE);
//	m_PositionLabel.ShowWindow(SW_HIDE);
//	m_CurrentTitle.ShowWindow(SW_HIDE);

// For CDialogSK
	EnableEasyMove();  // enable moving of the dialog 
    SetTransparentColor(m_TransMain, m_TransPanel); // set red as the 

	m_Ready2Reset = TRUE;
    resetControls();

	m_Genres.SetFocus();
    OnGenresFocus();


	initDb();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	// Removed this in April 2006, finding it now in Nov. Checking it in. We'll see!
//	SetIcon(m_hIcon, FALSE);		// Set small ic

//	ShowBetaWarning();
	StartStatusTimer();
	if (m_Config.trialMode() == 1) {
		PlayerStatusSet("Trial Mode. See Settings/License." );
		CString msg = "You have " + numToString(m_Config.trialLeft()) + " days left on your 60 day free trial";
		MBMessageBox("60 day Trial",msg,FALSE);
	} else if (m_Config.trialMode() == 2) {
		MBMessageBox("Trial Expired","Your 60 day trial has expired.",FALSE);
	}
	CString menuFunc,menuDesc;
	RemoteReceiver * rrcvr = RemoteReceiver::reset(this, MB_SERIAL_MESSAGE);
//	if (rrcvr)
//		rrcvr->getDescs(IR_MESSAGE_MENU, menuFunc, menuDesc);
//	if (menuDesc == "") menuDesc = "Menu";
//	m_HelpMsg = "Press "; m_HelpMsg += menuDesc; m_HelpMsg += " for options.";
	m_HelpMsg = "";
	if (m_Config.trialMode() == 1) {
		m_HelpMsg += " Trial Mode.";
	} else if (m_Config.trialMode() == 2) {
		m_HelpMsg = " Trial Expired.";
	}
	PlayerStatusSet(m_HelpMsg);

	UpdateWindow();
	m_Genres.RedrawWindow(FALSE);
	m_Artists.RedrawWindow(FALSE);

	readConfig();


	MBVersion mbv(m_Config.license());
	if (!mbv.goodLicense()) {
		MBMessageBox("Advisory","This Muzikbrowzer license is invalid.\r\nTrial Mode invoked.",TRUE);
		m_Config.resetTrial();
	}
	if (mbv.needUpdate()) {
		CString msg = "New version available at www.muzikbrowzer.com";
		PlayerStatusSet(msg);
		PlayerStatusTempSet(msg);
	}

	if (_initdialog) {
//		time_t now = CTime::GetCurrentTime().GetTime();
//		if (now - _initdialog->m_start.GetTime() < 2) {
//			// Just to show the banner for a sec
//			//Sleep(1000);
//		}
		_initdialog->DestroyWindow();
		delete _initdialog;

	}

	logger.ods("End of InitDialog");


	return FALSE;  // return TRUE  unless you set the focus to a control
}

void CPlayerDlg::ShowBetaWarning() {
	CTime t = CTime::GetCurrentTime();
	CString y = t.Format("%Y");
	int Y = atoi(y);

	CString mdy = t.Format("%d%m%y");
	RegistryKey reg(HKEY_LOCAL_MACHINE, RegKey);
	AutoBuf buf(500);
	reg.Read("lastrunmdy", buf.p, 49, "");
	CString lastrunmdy = buf.p;
	if (lastrunmdy != mdy) {
		reg.Write("lastrunmdy", (LPCTSTR) mdy);
		CString msg;
	msg << 
"Muzikbrowzer 2.0 is beta software. It may contain bugs\r\n" <<
"which could potentially damage data on your computer.\r\n" <<
"While every effort has been made to ensure against this\r\n" <<
"there are no guarantees. Pecan Ventures assumes no\r\n" <<
"liability what so ever for any damage caused by Muzikbrowzer.\r\n" <<
"Backup your data before performing tag editing with Muzikbrowzer.";

		if (Y >= 2007) {
			msg << 
"Muzikbrowzer 2.0 is beta software. The beta period for version 2.0 is\r\n" <<
"over. You must download the final release version from www.muzikbrowzer.com\r\n";
			MBMessageBox("Beta software", msg,FALSE);
			exit(0);
		}

		MBMessageBox("Beta software", msg,FALSE);
	}
}

void
CPlayerDlg::saveConfig() {
	int v = m_VolumeSlider.GetPos();
	m_Config.setRegistry("Volume", v);
	SaveWindowPos();
}
void
CPlayerDlg::readConfig() {
	int v=50;
	m_Config.getRegistry("Volume", v, 50);
	adjustVolume(v);
}

void 
initFont(CWnd * wnd) {
// Create a new font so we can change it later
     CFont * f = wnd->GetFont();
     CFont newfont;
     LOGFONT lf;
     if(f != NULL)
         { /* Set up duplicate font */
          f->GetObject(sizeof(LOGFONT), &lf);
          newfont.CreateFontIndirect(&lf);
         } /* Set up duplicate font */
     else
         { /* Use default font spec */
          newfont.CreateStockObject(ANSI_VAR_FONT);
         } /* Use default font spec */
     wnd->SetFont(&newfont);
     newfont.Detach();
}
void
changeFont(CWnd * wnd, CFont & f) {
     CFont * oldfont = wnd->GetFont();
     if(oldfont != NULL)
        oldfont->DeleteObject(); 

    wnd->SetFont(&f);
    f.Detach();
}
void CPlayerDlg::setFont() {
    static int first = 1;
    if (first) {
        first = 0;
        m_Genres.initFont();
        m_Artists.initFont();
        m_Albums.initFont();
        m_Songs.initFont();
        m_Playlist.initFont();
		m_GenresLabel.initFont();
		m_ArtistsLabel.initFont();
		m_AlbumsLabel.initFont();
		m_SongsLabel.initFont();
		m_PlaylistLabel.initFont();
		m_PlayerStatus.initFont();
		m_SearchStatus.initFont();
		m_PositionLabel.initFont();
//		m_CurrentTitle.initFont();
//		m_CurrentTitle.m_HCenter = 0;
		m_PlayerStatus.m_HCenter = 1;
    }
    LPLOGFONT lplf = m_Config.getTitlesFont();
    m_Genres.changeFont(lplf);
    m_Artists.changeFont(lplf);
    m_Albums.changeFont(lplf);
    m_Songs.changeFont(lplf);
    m_Playlist.changeFont(lplf);

	lplf = m_Config.getColHdrFont();
	m_GenresLabel.changeFont(lplf);
	m_ArtistsLabel.changeFont(lplf);
	m_AlbumsLabel.changeFont(lplf);
	m_SongsLabel.changeFont(lplf);
	m_PlaylistLabel.changeFont(lplf);
//	m_SearchStatus.changeFont(lplf);
	CFont font;
	font.CreateFontIndirect(lplf);
	m_SearchEdit.SetFont(&font);

//	lplf = m_Config.getCurPlayFont();
//	m_CurrentTitle.changeFont(lplf);
	lplf = m_Config.getStatusFont();
	m_PositionLabel.changeFont(lplf);
	m_PlayerStatus.changeFont(lplf);
	m_SearchStatus.changeFont(lplf);

}
void
CPlayerDlg::setColors() {
	
	COLORREF crColHdrInUL,crColHdrInLR,crColHdrOutUL,crColHdrOutLR;
	COLORREF crDataInUL,crDataInLR,crDataOutUL,crDataOutLR;
	COLORREF crStatusInUL,crStatusInLR,crStatusOutUL,crStatusOutLR;

	MBCONFIG_READ_COLOR_3D(m_reg,MB3DCOLHDRCOLOR,
		crColHdrInUL,crColHdrInLR,crColHdrOutUL,crColHdrOutLR);
	
	MBCONFIG_READ_COLOR_3D(m_reg,MB3DDATACOLOR,
		crDataInUL,crDataInLR,crDataOutUL,crDataOutLR);
	
	MBCONFIG_READ_COLOR_3D(m_reg,MB3DSTATUSCOLOR,
		crStatusInUL,crStatusInLR,crStatusOutUL,crStatusOutLR);

	COLORREF crOtherFg,crOtherBg,
		crButtonsFgOut,  crButtonsBgOut,
		crButtonsFgHover,crButtonsBgHover,
		crButtonsFgIn,crButtonsBgIn;
	BOOL OtherFlat,OtherBorder;

	MBCONFIG_READ_OTHER_COLORS(m_reg,
		crOtherFg,crOtherBg,
		crButtonsFgOut,  crButtonsBgOut,
		crButtonsFgHover,crButtonsBgHover,
		crButtonsFgIn,crButtonsBgIn,
		OtherFlat,OtherBorder);

	BOOL threeDDataWindows	= m_reg.Read(MB3DDATA,0);
	BOOL threeDColHdrs		= m_reg.Read(MB3DCOLHDRS,0);
	BOOL threeDStatus		= m_reg.Read(MB3DSTATUS,0);

	COLORREF crColHdrFg,crColHdrBg;
	crColHdrFg = m_Config.getColorTxColHdr();
	crColHdrBg = m_Config.getColorBkColHdr();
	
#define _LABEL_COLORS_ crColHdrFg,crColHdrBg,\
	crColHdrInUL,crColHdrInLR,crColHdrOutUL,crColHdrOutLR,\
	threeDColHdrs
	
	m_GenresLabel.SetColors(_LABEL_COLORS_);
	m_ArtistsLabel.SetColors(_LABEL_COLORS_);
	m_AlbumsLabel.SetColors(_LABEL_COLORS_);
	m_SongsLabel.SetColors(_LABEL_COLORS_);
	m_PlaylistLabel.SetColors(_LABEL_COLORS_);
//	m_SearchStatus.SetTransparent(m_TransPanel);

//	m_SearchStatus.SetColors(crColHdrFg,m_TransPanel,
//		m_TransPanel,m_TransPanel,m_TransPanel,m_TransPanel,FALSE);
//	m_SearchStatus.SetColors(_LABEL_COLORS_);

	COLORREF	crTxNormalFg,	crTxNormalBg, 
				crTxHighFg,		crTxHighBg,		
				crTxSelFg,		crTxSelBg,
				crStatusFg,		crStatusBg;
	crTxNormalFg = m_Config.getColorTxNormal();
	crTxNormalBg = m_Config.getColorBkNormal();
	crTxHighFg   = m_Config.getColorTxHigh();
	crTxHighBg   = m_Config.getColorBkHigh();
	crTxSelFg	 = m_Config.getColorTxSel();
	crTxSelBg	 = m_Config.getColorBkSel();
	crStatusFg	 = m_Config.getColorTxPanel();
	crStatusBg	 = m_Config.getColorBkPanel();

#define _DATA_COLORS_ crTxNormalBg,crTxHighBg,crTxSelBg,\
	crTxNormalFg,crTxHighFg,crTxSelFg,threeDDataWindows,\
	crDataInUL,crDataInLR,crDataOutUL,crDataOutLR

	m_Genres.SetColors(_DATA_COLORS_);
	m_Artists.SetColors(_DATA_COLORS_);
	m_Albums.SetColors(_DATA_COLORS_);
	m_Songs.SetColors(_DATA_COLORS_);
	m_Playlist.SetColors(_DATA_COLORS_);
	
#define _STATUS_COLORS_ crStatusFg,crStatusBg,\
	crStatusInUL,crStatusInLR,crStatusOutUL,crStatusOutLR,\
	threeDStatus

	m_PositionLabel.SetColors(_STATUS_COLORS_);
	m_PlayerStatus.SetColors(_STATUS_COLORS_);
	m_SearchStatus.SetColors(_STATUS_COLORS_);

	m_PlayerStatus.SetTicking(TRUE);

	m_GenresLabel.setDesc  ("genre ");
	m_ArtistsLabel.setDesc ("artist");
	m_AlbumsLabel.setDesc  ("album ");
	m_SongsLabel.setDesc   ("songs ");
	m_PlaylistLabel.setDesc("playl ");
	m_PlayerStatus.setDesc ("status");
	m_PositionLabel.setDesc("poslbl");
	m_SearchStatus.setDesc("searchstatus");
	m_SearchStatus.SetTicking(TRUE);


	m_SearchGo.SetFlat(OtherFlat);
	m_SearchCancel.SetFlat(OtherFlat);
	m_SearchClear.SetFlat(OtherFlat);
	m_SearchGo.DrawBorder(OtherBorder);
	m_SearchCancel.DrawBorder(OtherBorder);
	m_SearchClear.DrawBorder(OtherBorder);
	m_SearchGo.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);
	m_SearchCancel.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);
	m_SearchClear.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);
//	m_SearchEdit.SetBkColor

	m_OptionsButton.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONMENUIN),	m_TransPanel,
		m_Config.getSkin(MB_SKIN_BUTTONMENUHOVER),	m_TransPanel,
		m_Config.getSkin(MB_SKIN_BUTTONMENUOUT),	m_TransPanel
		);

	m_LogoButton.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONLOGOIN),	m_TransPanel,
		m_Config.getSkin(MB_SKIN_BUTTONLOGOHOVER),	m_TransPanel,
		m_Config.getSkin(MB_SKIN_BUTTONLOGOOUT),	m_TransPanel
		);

	m_ButtonMinimize.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONMINIMIZEIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONMINIMIZEHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONMINIMIZEOUT),		m_TransPanel);

	m_ButtonMaximize.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONMAXIMIZEIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONMAXIMIZEHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONMAXIMIZEOUT),		m_TransPanel);

	m_ButtonRestore.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONRESTOREIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONRESTOREHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONRESTOREOUT),		m_TransPanel);

	m_ButtonExit.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONEXITIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONEXITHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONEXITOUT),		m_TransPanel);

	m_MusicButton.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONMUSICIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONMUSICHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONMUSICOUT),		m_TransPanel);

	m_PicturesButton.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONPICTURESIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONPICTURESHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONPICTURESOUT),		m_TransPanel);

	m_VideoButton.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONVIDEOIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONVIDEOHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONVIDEOOUT),		m_TransPanel);

	m_ButtonStop.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONSTOPIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONSTOPHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONSTOPOUT),		m_TransPanel);

	m_ButtonPlay.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONPLAYIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONPLAYHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONPLAYOUT),		m_TransPanel);

	m_ButtonPause.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONPAUSEIN),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONPAUSEHOVER),	m_TransPanel,	
		m_Config.getSkin(MB_SKIN_BUTTONPAUSEOUT),		m_TransPanel);

	m_ButtonShuffle.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONSHUFFLEIN), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONSHUFFLEHOVER), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONSHUFFLEOUT), m_TransPanel);
//	m_ButtonSave.logit = TRUE;
	m_ButtonSave.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONSAVEIN), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONSAVEHOVER), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONSAVEOUT), m_TransPanel);

	m_ButtonReverse.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONREVERSEIN), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONREVERSEHOVER), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONREVERSEOUT), m_TransPanel);

	m_ButtonRandom.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONRANDOMIN), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONRANDOMHOVER), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONRANDOMOUT), m_TransPanel);

	m_ButtonLoad.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONLOADIN), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONLOADHOVER), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONLOADOUT), m_TransPanel);

	m_ButtonFastForward.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONFASTFORWARDIN), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONFASTFORWARDHOVER), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONFASTFORWARDOUT), m_TransPanel);

	m_ButtonClear.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONCLEARIN), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONCLEARHOVER), m_TransPanel, 
		m_Config.getSkin(MB_SKIN_BUTTONCLEAROUT), m_TransPanel);

//	m_OptionsButton.SetColor(CButtonST::BTNST_COLOR_FG_IN, m_Config.getColorTxPanel(), TRUE);
//	m_OptionsButton.SetColor(CButtonST::BTNST_COLOR_FG_OUT, m_Config.getColorTxPanel(), TRUE);
//	m_OptionsButton.SetColor(CButtonST::BTNST_COLOR_FG_FOCUS, m_Config.getColorTxPanel(), TRUE);
	
//	m_OptionsButton.SetAlign(CButtonST::ST_ALIGN_OVERLAP,TRUE);
	m_ButtonMinimize.SetAlign(CButtonST::ST_ALIGN_OVERLAP,TRUE);
	m_ButtonMaximize.SetAlign(CButtonST::ST_ALIGN_OVERLAP,TRUE);
	m_ButtonExit.SetAlign(CButtonST::ST_ALIGN_OVERLAP,TRUE);
	m_ButtonRestore.SetAlign(CButtonST::ST_ALIGN_OVERLAP,TRUE);
	m_ButtonStop.SetAlign(CButtonST::ST_ALIGN_OVERLAP,TRUE);
	m_ButtonPlay.SetAlign(CButtonST::ST_ALIGN_OVERLAP,TRUE);
	m_ButtonPause.SetAlign(CButtonST::ST_ALIGN_OVERLAP,TRUE);

	m_OptionsButton.DrawTransparent(TRUE);
	m_LogoButton.DrawTransparent(TRUE);
	m_ButtonMinimize.DrawTransparent(TRUE);
	m_ButtonMaximize.DrawTransparent(TRUE);
	m_ButtonExit.DrawTransparent(TRUE);
	m_ButtonRestore.DrawTransparent(TRUE);
	m_ButtonStop.DrawTransparent(TRUE);
	m_ButtonPlay.DrawTransparent(TRUE);
	m_ButtonPause.DrawTransparent(TRUE);

	m_ButtonShuffle.DrawTransparent(TRUE);
	m_ButtonSave.DrawTransparent(TRUE);
	m_ButtonReverse.DrawTransparent(TRUE);
	m_ButtonRandom.DrawTransparent(TRUE);
	m_ButtonLoad.DrawTransparent(TRUE);
	m_ButtonFastForward.DrawTransparent(TRUE);
	m_ButtonClear.DrawTransparent(TRUE);

	m_VolumeSlider.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONVOLUMEBACKGROUND),
		m_TransPanel,
		m_Config.getSkin(MB_SKIN_BUTTONVOLUMEKNOB),
		m_TransPanel
		);
	m_PositionSlider.SetBitmaps(
		m_Config.getSkin(MB_SKIN_BUTTONPROGRESSBACKGROUND),
		m_TransPanel,
		m_Config.getSkin(MB_SKIN_BUTTONPROGRESSKNOB),
		m_TransPanel
		);
	m_PositionSlider.SetOrientation(CMySliderCtrl::HORIZONTAL);

	m_LogoButton.SetHoverMsg(this, MB_HOVER_WWW_MSG);
	m_ButtonShuffle.SetHoverMsg(this, MB_HOVER_SHUFFLE_MSG);
	m_ButtonSave.SetHoverMsg(this, MB_HOVER_SAVE_MSG);
	m_ButtonReverse.SetHoverMsg(this, MB_HOVER_PREVIOUS_MSG);
	m_ButtonRandom.SetHoverMsg(this, MB_HOVER_RANDOM_MSG);
	m_ButtonLoad.SetHoverMsg(this, MB_HOVER_LOAD_MSG);
	m_ButtonFastForward.SetHoverMsg(this, MB_HOVER_NEXT_MSG);
	m_ButtonClear.SetHoverMsg(this, MB_HOVER_CLEAR_MSG);
	m_ButtonStop.SetHoverMsg(this, MB_HOVER_STOP_MSG);
	m_ButtonPlay.SetHoverMsg(this, MB_HOVER_PLAY_MSG);
	m_ButtonPause.SetHoverMsg(this, MB_HOVER_PAUSE_MSG);
	m_VolumeSlider.SetHoverMsg(this, MB_HOVER_VOLUME_MSG);
	m_PositionSlider.SetHoverMsg(this, MB_HOVER_POSITION_MSG);
	m_OptionsButton.SetHoverMsg(this, MB_HOVER_MENU_MSG);
	m_ButtonMinimize.SetHoverMsg(this, MB_HOVER_MINIMIZE_MSG);
	m_ButtonMaximize.SetHoverMsg(this, MB_HOVER_MAXIMIZE_MSG);
	m_ButtonRestore.SetHoverMsg(this, MB_HOVER_RESTORE_MSG);
	m_ButtonExit.SetHoverMsg(this, MB_HOVER_EXIT_MSG);
	m_SearchGo.SetHoverMsg(this, MB_HOVER_SEARCH_MSG);
	m_SearchCancel.SetHoverMsg(this, MB_HOVER_SEARCHCLOSE_MSG);
	m_SearchClear.SetHoverMsg(this, MB_HOVER_SEARCHCLEAR_MSG);

	m_LogoButton.SetKeepFocused(FALSE);
	m_ButtonShuffle.SetKeepFocused(FALSE);
	m_ButtonSave.SetKeepFocused(FALSE);
	m_ButtonReverse.SetKeepFocused(FALSE);
	m_ButtonRandom.SetKeepFocused(FALSE);
	m_ButtonLoad.SetKeepFocused(FALSE);
	m_ButtonFastForward.SetKeepFocused(FALSE);
	m_ButtonClear.SetKeepFocused(FALSE);
	m_ButtonStop.SetKeepFocused(FALSE);
	m_ButtonPlay.SetKeepFocused(FALSE);
	m_ButtonPause.SetKeepFocused(FALSE);
	m_SearchGo.SetKeepFocused(FALSE);
	m_SearchCancel.SetKeepFocused(FALSE);
	m_SearchClear.SetKeepFocused(FALSE);


}

BOOL RecreateListBox(CExtendedListBox* pList, LPVOID lpParam/*=NULL*/)
{
	if (pList == NULL)
		return FALSE;
	if (pList->GetSafeHwnd() == NULL)
		return FALSE;

	CWnd* pParent = pList->GetParent();
	if (pParent == NULL)
		return FALSE;

	// get current attributes
	DWORD dwStyle = pList->GetStyle();
	DWORD dwStyleEx = pList->GetExStyle();
	CRect rc;
	pList->GetWindowRect(&rc);
	pParent->ScreenToClient(&rc);	// map to client co-ords
	UINT nID = pList->GetDlgCtrlID();
	CFont* pFont = pList->GetFont();
	CWnd* pWndAfter = pList->GetNextWindow(GW_HWNDPREV);

	// create the new list box and copy the old list box items 
	// into a new listbox along with each item's data, and selection state
	CExtendedListBox listNew;
	if (! listNew.CreateEx(dwStyleEx, _T("LISTBOX"), _T(""), dwStyle, 
                                rc, pParent, nID, lpParam))
	  return FALSE;
	listNew.SetFont(pFont);
	int nNumItems = pList->GetCount();
	BOOL bMultiSel = (dwStyle & LBS_MULTIPLESEL || dwStyle & LBS_EXTENDEDSEL);
	for (int n = 0; n < nNumItems; n++)
	{
		CString sText;
		pList->GetText(n, sText);
		int nNewIndex = listNew.AddString(sText);
		listNew.SetItemData(nNewIndex, pList->GetItemData(n));
		if (bMultiSel && pList->GetSel(n))
			listNew.SetSel(nNewIndex);
	}
	if (! bMultiSel)
	{
		int nCurSel = pList->GetCurSel();
		if (nCurSel != -1)
		{
			CString sSelText;
			// get the selection in the old list
			pList->GetText(nCurSel, sSelText);
			// now find and select it in the new list
			listNew.SetCurSel(listNew.FindStringExact(-1, sSelText));
		}
	}
	// destroy the existing window, then attach the new one
	pList->DestroyWindow();
	HWND hwnd = listNew.Detach();
	pList->Attach(hwnd);

	// position correctly in z-order
	pList->SetWindowPos(pWndAfter == NULL ? &CWnd::wndBottom
                                 : pWndAfter, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

	return TRUE;
}
void
CPlayerDlg::resetControls() {
	if (!m_Ready2Reset) return;

	if (m_Config.hideGenre()) {
		m_Genres.ShowWindow(SW_HIDE);
		m_GenresLabel.ShowWindow(SW_HIDE);
	} else {
		m_Genres.ShowWindow(SW_NORMAL);
		m_GenresLabel.ShowWindow(SW_NORMAL);
	}
	
	if (m_Config.AlbumSortAlpha()) {
		m_Albums.ModifyStyle(0,LBS_SORT,0);
	} else {
		m_Albums.ModifyStyle(LBS_SORT,0);
	}
	RecreateListBox(&m_Albums,NULL);

	CWaitCursor c;
	GetWindowRect(m_WindowRect);

#ifndef _DEBUGBC
	m_Config.getSkins(m_Skins);
	if (m_Skins.GetCount() < 1) {
		MBMessageBox("Alert","Muzikbrowzer is unable to locate it's skins and cannot proceed.\r\nYou may wish to reinstall Muzikbrowzer.",TRUE);
		exit(0);
	}
#endif

	// read skin def
	MBCONFIG_READ_SKIN_DEFS(m_Config,m_reg);

	// Now read into m_Config memory
	// Necessary cause I'm getting config info from 2 places
	// 1) via m_reg and 2) m_Config.
	m_Config.ReadReg(m_reg);
	m_Config.verifySkin();

	m_AlbumArt = m_Config.getSkin(MB_SKIN_ALBUMART);
	m_FixedSize = FALSE;

	LayOutStyle BackgroundMainType,BackgroundPanelType;
	MBCONFIG_READ_BACKGROUND_TYPES(m_reg,BackgroundMainType,BackgroundPanelType);

	int w = 0;
	int h = 0;

	if (LO_FIXED == BackgroundMainType) {
		CDIBSectionLite bmpBgMain;
		bmpBgMain.Load(m_Config.getSkin(MB_SKIN_BACKGROUNDMAIN));
		w = bmpBgMain.GetWidth();
		h = bmpBgMain.GetHeight();
	}
	if (w && h) {
		CRect rect;
		int max;
		ReadWindowPos(max, rect);
		rect.right = rect.left + w;
		rect.bottom = rect.top + h;
		MoveWindow(rect, TRUE );
	}
	if (LO_FIXED == BackgroundMainType) {
		m_FixedSize = TRUE;
	}

	
	m_BtnControls.init(this);

	GetClientRect(m_BtnControls.dialogrect);
//	CString msg = "dlg w,h=" + numToString(m_BtnControls.dialogrect.Width())
//		+ " " + numToString(m_BtnControls.dialogrect.Height()) + "\r\n";
//	OutputDebugString(msg);

	int ControlBoxWidth = m_reg.Read("ControlBoxWidth",0);
	int ControlBoxHeight = m_reg.Read("ControlBoxHeight",0);

//	int r,g,b;
//	r = regSD.Read("TransRedMain",254);
//	g = regSD.Read("TransGreenMain",0);
//	b = regSD.Read("TransBlueMain",0);
//	m_TransMain  = RGB(r,g,b);
//	r = regSD.Read("TransRedPanel",253);
//	g = regSD.Read("TransGreenPanel",0);
//	b = regSD.Read("TransBluePanel",0);
//	m_TransPanel = RGB(r,g,b);

	MBCONFIG_READ_TRANS_COLORS(m_reg,m_TransMain,m_TransPanel);

	SetTransparentColor(m_TransMain, m_TransPanel); // set red as the 
	setFont();
//	setColors();

	int border = m_Config.getDlgBorderWidth();
	int borderpanel = m_reg.Read("BorderPanel",5);
	int borderhorz = m_Config.getDlgBorderHorz();
	int bordervert = m_Config.getDlgBorderVert();


	m_BtnControls.FreeEm();

	m_BtnControls.add(0,0, "options button", IDC_OPTIONS_BUTTON);
	m_BtnControls.add(0,1, "app label",		IDC_BUTTON_LOGO); // logobutton
	m_BtnControls.add(0,2, "button min",		IDC_BUTTON_MINIMIZE);
	m_BtnControls.add(0,3, "button max",		IDC_BUTTON_MAXIMIZE);
	m_BtnControls.add(0,3, "button restore",	IDC_BUTTON_RESTORE);
	m_BtnControls.add(0,4, "button exit",	IDC_BUTTON_EXIT);
	m_BtnControls.add(1,0, "button reverse",	IDC_BUTTON_REVERSE);
//	m_BtnControls.add(1,1, "button music",	IDC_MUSIC_BUTTON);
//	m_BtnControls.add(1,1, "button pictures",	IDC_PICTURES_BUTTON);
//	m_BtnControls.add(1,1, "button video",	IDC_VIDEO_BUTTON);
	m_BtnControls.add(1,1, "button stop",	IDC_BUTTON_STOP);
	m_BtnControls.add(1,2, "button play",	IDC_BUTTON_PLAY);
	m_BtnControls.add(1,3, "button pause",	IDC_BUTTON_PAUSE);
	m_BtnControls.add(1,4,	"button fford", IDC_BUTTON_FASTFORWARD);
	m_BtnControls.add(1,5, "playlist",		IDC_PLAYLIST, IDC_PLAYLISTLABEL);
	m_BtnControls.add(1,6, "picture",		IDC_PICTURE_CTRL);
	m_BtnControls.add(2,0, "random",	IDC_BUTTON_RANDOM);
	m_BtnControls.add(2,1, "vol slider",		IDC_VOLUME_SLIDER);
	m_BtnControls.add(3,0, "shuffle",		IDC_BUTTON_SHUFFLE);
	m_BtnControls.add(4,0, "clear",		IDC_BUTTON_CLEAR);
	m_BtnControls.add(5,0, "load",			IDC_BUTTON_LOAD);
	m_BtnControls.add(6,0, "save",			IDC_BUTTON_SAVE);
//	m_BtnControls.add(7,1, "title",			IDC_CURRENT_TITLE);
	m_BtnControls.add(8,0, "genre",			IDC_GENRES, IDC_GENRESLABEL);
	m_BtnControls.add(8,1, "artist",			IDC_ARTISTS, IDC_ARTISTSLABEL);
	m_BtnControls.add(8,2, "album",			IDC_ALBUMS, IDC_ALBUMSLABEL);
	m_BtnControls.add(8,3, "songs",			IDC_SONGS, IDC_SONGSLABEL);
	m_BtnControls.add(9,0, "pos label",		IDC_POSITION_LABEL);
	m_BtnControls.add(9,1, "player status",	IDC_PLAYER_STATUS);

	m_BtnControls.add(2,0, "pos slider",		IDC_POSITION_SLIDER);

	int labelheight=0;
	int statusheight = 0;

	double GenresWidthPct;
	double ArtistsWidthPct ;
	double AlbumsWidthPct;
	double SongsWidthPct;

	CString tmp ;
	m_Config.getRegistry("LibGenresWidth",tmp,".25");

//	Now using the hard limits to enforce
	GenresWidthPct = atof(tmp);
	if (m_Config.hideGenre())
		GenresWidthPct = 0;

	m_Config.getRegistry("LibArtistsWidth",tmp,".25");
	ArtistsWidthPct = atof(tmp);

	m_Config.getRegistry("LibAlbumsWidth",tmp,".25");
	AlbumsWidthPct = atof(tmp);

	SongsWidthPct = 1 - (GenresWidthPct + ArtistsWidthPct + AlbumsWidthPct);

	int fixedy = bordervert * 3;
	int y = border;
	int x = border;
	int rowMaxY = 0;
	int maxX = 0;

	logger.ods("resizeControls 0");

	labelheight = m_ArtistsLabel.GetItemHeight() + 4;
	statusheight = m_PlayerStatus.GetItemHeight() + 6;

	Control * p ;
	p = m_BtnControls.getObj(IDC_OPTIONS_BUTTON);
	rowMaxY = __max(rowMaxY, y + p->height);
	m_BtnControls.move(p, x, y, p->row, p->col);

	CDIBSectionLite bmpaplabel;
	bmpaplabel.Load(m_Config.getSkin(MB_SKIN_BUTTONLOGOOUT));
	p = m_BtnControls.getObj(IDC_BUTTON_LOGO);
	x = (m_BtnControls.dialogrect.Width() / 2) - (bmpaplabel.GetWidth() / 2);
	y = border;
	rowMaxY = __max(rowMaxY, y + bmpaplabel.GetHeight());
	m_BtnControls.move(p, x, y, p->row, p->col);

	p = m_BtnControls.getObj(IDC_BUTTON_EXIT);
	x = m_BtnControls.dialogrect.Width()
		- (border + p->width);
	rowMaxY = __max(rowMaxY, y + p->height);
	m_BtnControls.move(p, x, y, p->row, p->col);

#define SYSCTRLSWIDTH 2

	p = m_BtnControls.getObj(IDC_BUTTON_MAXIMIZE);
	x -= (SYSCTRLSWIDTH + p->width);
	rowMaxY = __max(rowMaxY, y + p->height);
	m_BtnControls.move(p, x, y, p->row, p->col);

	p = m_BtnControls.getObj(IDC_BUTTON_RESTORE);
	m_BtnControls.move(p, x, y, p->row, p->col);

	p = m_BtnControls.getObj(IDC_BUTTON_MINIMIZE);
	x -= (SYSCTRLSWIDTH + p->width);
	rowMaxY = __max(rowMaxY, y + p->height);
	m_BtnControls.move(p, x, y, p->row, p->col);	
	int playpaneltop = rowMaxY;

// control box

	y = rowMaxY + borderpanel /*+ bordervert*/;
	x = border + borderpanel;
	int ControlBoxLeft = x;
	int ControlBoxTop  = y;
	int ControlBoxRight = x + ControlBoxWidth;
	int ControlBoxBottom = y + ControlBoxHeight;

	x = ControlBoxLeft + m_reg.Read("StopX",0);
	y = ControlBoxTop + m_reg.Read("StopY", 0);
	p = m_BtnControls.getObj(IDC_BUTTON_STOP);
	maxX = x + p->width;
//	rowMaxY = y + p->height;
	m_BtnControls.move(p, x, y, p->row, p->col);	

	int stopy = y;

	x = ControlBoxLeft + m_reg.Read("PlayX",0);
	y = ControlBoxTop + m_reg.Read("PlayY", 0);
	p = m_BtnControls.getObj(IDC_BUTTON_PLAY);
	maxX = __max(maxX, x + p->width);
//	rowMaxY = __max(rowMaxY, y + p->height);
	m_BtnControls.move(p, x, y, p->row, p->col);	

	x = ControlBoxLeft + m_reg.Read("PauseX",0);
	y = ControlBoxTop + m_reg.Read("PauseY", 0);
	p = m_BtnControls.getObj(IDC_BUTTON_PAUSE);
	maxX = __max(maxX, x + p->width);
//	rowMaxY = __max(rowMaxY, y + p->height);
	m_BtnControls.move(p, x, y, p->row, p->col);	
	int pauseright = x + p->width;
	
	y = rowMaxY;

	p = m_BtnControls.getObj(IDC_BUTTON_REVERSE);
	x = ControlBoxLeft + m_reg.Read("ReverseX",0);
	y = ControlBoxTop + m_reg.Read("ReverseY", 0);
//	rowMaxY = __max(rowMaxY, y + p->height);
	m_BtnControls.move(p, x, y, p->row, p->col);	

	x += p->width;
	p = m_BtnControls.getObj(IDC_BUTTON_FASTFORWARD);
	x = ControlBoxLeft + m_reg.Read("ForwardX",0);
	y = ControlBoxTop + m_reg.Read("ForwardY", 0);
	maxX = __max(maxX, x + p->width);
//	rowMaxY = __max(rowMaxY, y + p->height);
	m_BtnControls.move(p, x, y, p->row, p->col);	
	int ffright = x + p->width;

	y = rowMaxY;
	x = border + borderpanel;
	p = m_BtnControls.getObj(IDC_BUTTON_RANDOM);
	int randomy = y;
	x = ControlBoxLeft + m_reg.Read("RandomX",0);
	y = ControlBoxTop + m_reg.Read("RandomY", 0);
//	rowMaxY = __max(rowMaxY, y + p->height);
	maxX = __max(maxX, x + p->width);
	m_BtnControls.move(p, x, y, p->row, p->col);

	y += p->height;
	p = m_BtnControls.getObj(IDC_BUTTON_SHUFFLE);
	x = ControlBoxLeft + m_reg.Read("ShuffleX",0);
	y = ControlBoxTop + m_reg.Read("ShuffleY", 0);
	maxX = __max(maxX, x + p->width);
	m_BtnControls.move(p, x, y, p->row, p->col);
	
	y += p->height;
	p = m_BtnControls.getObj(IDC_BUTTON_CLEAR);
	x = ControlBoxLeft + m_reg.Read("ClearX",0);
	y = ControlBoxTop + m_reg.Read("ClearY", 0);
	maxX = __max(maxX, x + p->width);
	m_BtnControls.move(p, x, y, p->row, p->col);
	
	y += p->height;
	p = m_BtnControls.getObj(IDC_BUTTON_LOAD);
	x = ControlBoxLeft + m_reg.Read("LoadX",0);
	y = ControlBoxTop + m_reg.Read("LoadY", 0);
	maxX = __max(maxX, x + p->width);
	m_BtnControls.move(p, x, y, p->row, p->col);
	
	y += p->height;
	p = m_BtnControls.getObj(IDC_BUTTON_SAVE);
	x = ControlBoxLeft + m_reg.Read("SaveX",0);
	y = ControlBoxTop + m_reg.Read("SaveY", 0);
	maxX = __max(maxX, x + p->width);
	m_BtnControls.move(p, x, y, p->row, p->col);
	int savebottom = y + p->height;

	p = m_BtnControls.getObj(IDC_VOLUME_SLIDER);
	x = maxX ;//+ borderhorz;
	y = stopy;
	x = ControlBoxLeft + m_reg.Read("VolumeX",0);
	y = ControlBoxTop + m_reg.Read("VolumeY", 0);
	m_BtnControls.move(p, x, y, p->row, p->col);	
	int volright = x + p->width;
	int volbottom = y + p->height;

	p = m_BtnControls.getObj(IDC_POSITION_SLIDER);
	x = ControlBoxLeft + m_reg.Read("ProgressX",0);
	y = ControlBoxTop + m_reg.Read("ProgressY", 0);
	m_BtnControls.move(p, x, y, p->row, p->col);	
	
	y = ControlBoxTop;
	//y = rowMaxY + borderpanel /* + bordervert*/;
	p = m_BtnControls.getObj(IDC_PICTURE_CTRL);

	p->height = ControlBoxHeight;
	p->width = p->height;
	int pheight = p->height;
	x = m_BtnControls.dialogrect.Width() - (border + p->width + borderpanel);
	m_BtnControls.move(p,x,y,p->row,p->col);
	int picbottom = y + p->height;
	int picleft = x;

	int plmaxy = __max(ControlBoxBottom,volbottom);
	plmaxy = __max(plmaxy,picbottom);
	int plmaxx = __max(ffright,volright);
	plmaxx = __max(plmaxx,ControlBoxRight) + borderhorz;

	p = m_BtnControls.getObj(IDC_PLAYLIST);
	p->height = pheight - (labelheight );
	p->labelheight = labelheight;
	x = ControlBoxRight + borderhorz;
	p->width = (picleft - (( ( 2 * borderpanel) + borderhorz))) - x;
	m_BtnControls.move(p,x,y,p->row,p->col);
	int playlistright = x + p->width;
	int playlistleft = x;
	int playlistbottom = y + p->height + labelheight;
	
	y = playlistbottom + bordervert;
	int pwidth = (x + p->width) - (border + borderpanel);

	int posy = __max(ControlBoxBottom,volbottom);
	posy = __max(posy, playlistbottom) + bordervert;

	int genrey = ControlBoxTop + ControlBoxHeight + (2 * borderpanel) +
		bordervert;

	p = m_BtnControls.getObj(IDC_POSITION_LABEL);
	CSize s = m_PositionLabel.GetSize("00:00  /  00:00");
	p->width = s.cx;
	p->height = statusheight;
	x = border + borderpanel; 
	y = m_BtnControls.dialogrect.Height() 
		- (statusheight + border + borderpanel);
	m_BtnControls.move(p, x, y, p->row, p->col);	
	int poswidth = p->width;

	p = m_BtnControls.getObj(IDC_PLAYER_STATUS);
	p->height = statusheight;
	x = border + poswidth + borderhorz + borderpanel;
	p->width = m_BtnControls.dialogrect.Width()
		- ((2 * (border + borderpanel)) + poswidth + borderhorz );
	m_BtnControls.move(p,x,y,p->row,p->col);
	int statusbottom = y + p->height;

	int tw=0;
	p = m_BtnControls.getObj(IDC_GENRES);		
	m_LibRemainingWidth = (m_BtnControls.dialogrect.Width() - 
		((2 * (borderpanel + border)) + (3 * borderhorz)));
	p->width = m_LibRemainingWidth * GenresWidthPct;
	p->labelheight = labelheight;
	p->height = (y - bordervert) - (genrey + labelheight);
	y = genrey;
	x = border + borderpanel;
	m_BtnControls.move(p,x,y,p->row,p->col);
	m_SearchEdit.GetWindowRect(m_SearchEditRect);STC(m_SearchEditRect);
	CRectMove(m_SearchEditRect,x,y);
	m_SearchEdit.MoveWindow(m_SearchEditRect);
	
	pheight = p->height;
	if (!m_Config.hideGenre())
		x += p->width + borderhorz;

	p = m_BtnControls.getObj(IDC_ARTISTS);
	p->width = m_LibRemainingWidth * ArtistsWidthPct;
	p->labelheight = labelheight;
	p->height = pheight;
	m_BtnControls.move(p,x,y,p->row,p->col);

	pwidth = p->width;
	x += p->width + borderhorz;
	
	p = m_BtnControls.getObj(IDC_ALBUMS);
	p->width = m_LibRemainingWidth * AlbumsWidthPct;
	p->labelheight = labelheight;
	p->height = pheight;
	m_BtnControls.move(p,x,y,p->row,p->col);

	x += p->width + borderhorz;
	
	p = m_BtnControls.getObj(IDC_SONGS);
	p->width = m_BtnControls.dialogrect.Width() - (x + border + borderpanel);
	p->labelheight = labelheight;
	p->height = pheight;
	m_BtnControls.move(p,x,y,p->row,p->col);
	m_LibHeight = pheight;

	double pctSongs = 1 - (GenresWidthPct + ArtistsWidthPct + AlbumsWidthPct);
	m_Config.setRegistry("LibSongsWidth", numToString(pctSongs));

	CDC * cdc = GetDC();

	FreeResources();
	
	logger.ods("resizeControls 1");

///////////////////////////////
// Set all the CDialogSK bmps
///////////////////////////////
	
	// dialog
	CString bgmain = m_Config.getSkin(MB_SKIN_BACKGROUNDMAIN);
	if (bgmain.GetLength()) {
		CRect rect0,rect1;
		GetWindowRect(rect0);
		ScreenToClient(rect0);
		
		rect0.OffsetRect(3,3);
#pragma hack		
		// hack alert: rect0 needs 2 b Window instead of client
		// for the resizing border
		SetBitmap(bgmain, rect0, BackgroundMainType, CS("frame"));

	}

	// applabel

	CString bgpanel ;

	// l,t,r,b
	// playlist panel
	CRect plrect(ControlBoxLeft - borderpanel,
		ControlBoxTop - borderpanel,
		playlistright + borderpanel,
		ControlBoxBottom + borderpanel);
	plrect.left = ControlBoxLeft - borderpanel;
	plrect.top = ControlBoxTop - borderpanel;
	plrect.right = playlistright + borderpanel;
	plrect.bottom = ControlBoxBottom + borderpanel;
	bgpanel = m_Config.getSkin(MB_SKIN_BACKGROUNDPLAYLIST);
	if (bgpanel.GetLength()) {
		SetBitmap(bgpanel, plrect, BackgroundPanelType, CS("play panel"));
	}

	// lib panel
	CRect librect(plrect);
	librect.top = genrey-borderpanel;
	librect.bottom = statusbottom + borderpanel;
	librect.right = m_BtnControls.dialogrect.right - border;
	bgpanel = m_Config.getSkin(MB_SKIN_BACKGROUNDLIBRARY);
	if (bgpanel.GetLength()) {
		SetBitmap(bgpanel, librect, BackgroundPanelType, CS("lib panel"));
	}

	// album panel
	CRect alrect(plrect);
	alrect.left = plrect.right + borderhorz;
	alrect.right = librect.right;
	bgpanel = m_Config.getSkin(MB_SKIN_BACKGROUNDALBUMART);
	if (bgpanel.GetLength()) {
		SetBitmap(bgpanel, alrect, BackgroundPanelType, CS("album"));
	}

	// button background
	CRect buttonrect;
	buttonrect.left = ControlBoxLeft;
	buttonrect.top = ControlBoxTop;
	buttonrect.right = ControlBoxLeft + ControlBoxWidth;
	buttonrect.bottom = ControlBoxTop + ControlBoxHeight;
	bgpanel = m_Config.getSkin(MB_SKIN_BUTTONBACKGROUND);
	if (bgpanel.GetLength()) {
		SetBitmap(bgpanel, buttonrect, LO_FIXED, CS("controlbox"));
	}

	make(cdc); // the background

	logger.ods("resizeControls 2");
	m_Need2Erase = TRUE;

	OnNcPaint() ;
	OnEraseBkgnd(cdc); // force CDialogSK to paint background
	displayAlbumArt(""); // paint the AlbumArtDefault.bmp
	// This sends a WM_NCPAINT to repaint the resize frame
	RedrawWindow(NULL,NULL, RDW_FRAME|RDW_INVALIDATE);

	m_InitDone = TRUE;

	m_Genres.SetBitmaps(cdc, 
		m_Config.getSkin(MB_SKIN_SCROLLUPARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLDOWNARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBUTTON),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBACKGROUND),m_TransPanel);
	m_Artists.SetBitmaps(cdc,
		m_Config.getSkin(MB_SKIN_SCROLLUPARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLDOWNARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBUTTON),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBACKGROUND),m_TransPanel);
	m_Albums.SetBitmaps(cdc, 
		m_Config.getSkin(MB_SKIN_SCROLLUPARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLDOWNARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBUTTON),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBACKGROUND),m_TransPanel);
	m_Songs.SetBitmaps(cdc, 
		m_Config.getSkin(MB_SKIN_SCROLLUPARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLDOWNARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBUTTON),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBACKGROUND),m_TransPanel);
	m_Playlist.SetBitmaps(cdc, 
		
		m_Config.getSkin(MB_SKIN_SCROLLUPARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLDOWNARROW),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBUTTON),m_TransPanel,
		m_Config.getSkin(MB_SKIN_SCROLLBACKGROUND),m_TransPanel);
	setColors();
	

	m_Genres.invalidate();
	m_Artists.invalidate();
	m_Albums.invalidate();
	m_Songs.invalidate();
	m_Playlist.invalidate();

	m_Genres.RedrawWindow();
	m_Artists.RedrawWindow();
	m_Songs.RedrawWindow();
	m_Albums.RedrawWindow();
	m_Playlist.RedrawWindow();

	m_GenresLabel.RedrawWindow();
	m_ArtistsLabel.RedrawWindow();
	m_SongsLabel.RedrawWindow();
	m_AlbumsLabel.RedrawWindow();
	m_PlaylistLabel.RedrawWindow();

	m_PositionLabel.RedrawWindow();
	m_PlayerStatus.RedrawWindow();

	m_PositionSlider.SizeToContent(this);
	m_VolumeSlider.SizeToContent(this);

	m_LogoButton.SetBk(m_bmBackground);
	m_OptionsButton.SetBk(m_bmBackground);

	ReleaseDC(cdc);
	
	static int counter = 0;
	counter++;
	CString msg("resetControls ");
	msg += numToString(counter) + "\r\n";
	logger.logd(msg);
	
	UpdateRects();

	ShowSearchDlg();

	m_SearchStatus.GetWindowRect(m_SearchStatusRect);
	STC(m_SearchStatusRect);
	m_SearchStatus.SetTransparent(m_bmBackground,m_SearchStatusRect, m_TransPanel);
	m_SearchStatus.RedrawWindow();

	// Don't do this hear anymore, it's resetting volume back to
	// what's saved in registry instead of current setting
	//readConfig();
	// Do this instead cause the slider initiall paints thumb at top
	m_VolumeSlider.SetPos(m_VolumeSlider.GetPos());

	POSITION pos;
	HBRUSH hbr;
	CString key;
	for( pos = m_CtlColors.GetStartPosition(); pos != NULL; ) {
		m_CtlColors.GetNextAssoc( pos, key, (void*&)hbr);
		BOOL r = ::DeleteObject((HBRUSH)hbr);
	}
	m_CtlColors.RemoveAll();

	if (m_mlib.checkReReadDb()) {
		init();
	}
}
void
CPlayerDlg::ShowSearchDlg() {
	Control * p,*p1,*p2,*p3,*p4;
	int newy,newheight;
	if (TRUE == m_ShowSearchPanel) {
		newy = m_SearchEditRect.bottom + 5;
		newheight = m_LibHeight - (m_SearchEditRect.Height() + 5);
	} else {
		newy = m_SearchEditRect.top;
		m_SearchCancel.ShowWindow(SW_HIDE);
		m_SearchEdit.ShowWindow(SW_HIDE);
		m_SearchGo.ShowWindow(SW_HIDE);
		m_SearchClear.ShowWindow(SW_HIDE);
		m_SearchStatus.ShowWindow(SW_HIDE);
		newheight = m_LibHeight ;
	}
	p1 = m_BtnControls.getObj(IDC_GENRES);
	p2 = m_BtnControls.getObj(IDC_ARTISTS);
	p3 = m_BtnControls.getObj(IDC_ALBUMS);
	p4 = m_BtnControls.getObj(IDC_SONGS);
	if (m_LastShowSearchPanel != m_ShowSearchPanel) {
		p1->ctrl->ShowWindow(SW_HIDE);p1->label->ShowWindow(SW_HIDE);
		p2->ctrl->ShowWindow(SW_HIDE);p2->label->ShowWindow(SW_HIDE);
		p3->ctrl->ShowWindow(SW_HIDE);p3->label->ShowWindow(SW_HIDE);
		p4->ctrl->ShowWindow(SW_HIDE);p4->label->ShowWindow(SW_HIDE);
	}

	p = m_BtnControls.getObj(IDC_GENRES);
	p->y = newy; p->height = newheight; m_BtnControls.move(p,p->x,p->y,p->row,p->col);
	p = m_BtnControls.getObj(IDC_ARTISTS);
	p->y = newy; p->height = newheight; m_BtnControls.move(p,p->x,p->y,p->row,p->col);
	p = m_BtnControls.getObj(IDC_ALBUMS);
	p->y = newy; p->height = newheight; m_BtnControls.move(p,p->x,p->y,p->row,p->col);
	p = m_BtnControls.getObj(IDC_SONGS);
	p->y = newy; p->height = newheight; m_BtnControls.move(p,p->x,p->y,p->row,p->col);
	m_SearchCancel.MoveWindow(m_SearchCancelRect);
	m_SearchEdit.MoveWindow(m_SearchEditRect);
	m_SearchGo.MoveWindow(m_SearchGoRect);
	m_SearchClear.MoveWindow(m_SearchClearRect);
	m_SearchStatus.MoveWindow(m_SearchStatusRect);

	UpdateRects();

	m_Need2Erase = TRUE;
	//InvalidateRect(m_LibraryRect);
	UpdateWindow();
	if (TRUE == m_ShowSearchPanel && m_LastShowSearchPanel != m_ShowSearchPanel) {
		m_SearchCancel.ShowWindow(SW_NORMAL);
		m_SearchEdit.ShowWindow(SW_NORMAL);
		m_SearchGo.ShowWindow(SW_NORMAL);
		m_SearchClear.ShowWindow(SW_NORMAL);
		m_SearchStatus.ShowWindow(SW_NORMAL);
		m_SearchEdit.SetFocus();
	}
	if (m_LastShowSearchPanel != m_ShowSearchPanel) {
		p1->ctrl->ShowWindow(SW_NORMAL);p1->label->ShowWindow(SW_NORMAL);
		p2->ctrl->ShowWindow(SW_NORMAL);p2->label->ShowWindow(SW_NORMAL);
		p3->ctrl->ShowWindow(SW_NORMAL);p3->label->ShowWindow(SW_NORMAL);
		p4->ctrl->ShowWindow(SW_NORMAL);p4->label->ShowWindow(SW_NORMAL);
	}
	m_LastShowSearchPanel = m_ShowSearchPanel;
}

void
CPlayerDlg::UpdateRects() {
	m_SearchCancel.GetWindowRect(m_SearchCancelRect);STC(m_SearchCancelRect);
	m_SearchClear.GetWindowRect(m_SearchClearRect);STC(m_SearchClearRect);
	m_SearchEdit.GetWindowRect(m_SearchEditRect);STC(m_SearchEditRect);
	m_SearchGo.GetWindowRect(m_SearchGoRect);STC(m_SearchGoRect);
	m_SearchStatus.GetWindowRect(m_SearchStatusRect);STC(m_SearchStatusRect);
	m_Genres.GetWindowRect(m_GenresRect);STC(m_GenresRect);
	m_GenresLabel.GetWindowRect(m_GenresLabelRect);STC(m_GenresLabelRect);
	m_SongsLabel.GetWindowRect(m_SongsLabelRect);STC(m_SongsLabelRect);

	if (!m_ShowSearchPanel) {
		CRectMove(m_SearchEditRect,m_GenresLabelRect.left,
			m_GenresLabelRect.top);
	}

	CSize cs = m_SearchStatus.GetSize("A");
	CRectHeight(m_SearchEditRect,cs.cy+4); // +4 so the cursor is still inside vertically
	CRectHeight(m_SearchGoRect,m_SearchEditRect.Height());
	CRectHeight(m_SearchClearRect,m_SearchEditRect.Height());
	CRectHeight(m_SearchCancelRect,m_SearchEditRect.Height());
	CRectHeight(m_SearchStatusRect,m_SearchEditRect.Height()+4);

//	CRectMove(m_SearchEditRect,m_GenresLabelRect.left,m_GenresLabelRect.top);
	CRectMove(m_SearchGoRect,m_SearchEditRect.right + 5,m_SearchEditRect.top);
	CRectMove(m_SearchClearRect,m_SearchGoRect.right + 5, m_SearchEditRect.top);
	CRectMove(m_SearchCancelRect,m_SearchClearRect.right + 5,m_SearchEditRect.top);

	CRectWidth(m_SearchStatusRect,
		(m_SongsLabelRect.right - m_SearchCancelRect.right) -4);
	CRectMove(m_SearchStatusRect,m_SearchCancelRect.right + 5,
		m_SearchEditRect.top); // +2 centers the text nicely with the buttons

	m_Artists.GetWindowRect(m_ArtistsRect);STC(m_ArtistsRect);
	m_ArtistsLabel.GetWindowRect(m_ArtistsLabelRect);STC(m_ArtistsLabelRect);
	m_Albums.GetWindowRect(m_AlbumsRect);STC(m_AlbumsRect);
	m_AlbumsLabel.GetWindowRect(m_AlbumsLabelRect);STC(m_AlbumsLabelRect);
	m_Songs.GetWindowRect(m_SongsRect);STC(m_SongsRect);
	m_SongsLabel.GetWindowRect(m_SongsLabelRect);STC(m_SongsLabelRect);
	m_GenreArtistAdjust = CRect(crtopright(m_GenresLabelRect),crbottomleft(m_ArtistsLabelRect));
	m_ArtistAlbumAdjust = CRect(crtopright(m_ArtistsLabelRect),crbottomleft(m_AlbumsLabelRect));
	m_AlbumSongAdjust = CRect(crtopright(m_AlbumsLabelRect),crbottomleft(m_SongsLabelRect));
	m_GenreArtistAdjust.InflateRect(2,0);
	m_ArtistAlbumAdjust.InflateRect(2,0);
	m_AlbumSongAdjust.InflateRect(2,0);

	m_GenresLabelInt.UnionRect(m_GenresRect,m_GenresLabelRect);
	m_ArtistsLabelInt.UnionRect(m_ArtistsRect,m_ArtistsLabelRect);
	m_AlbumsLabelInt.UnionRect(m_AlbumsRect,m_AlbumsLabelRect);
	m_SongsLabelInt.UnionRect(m_SongsRect,m_SongsLabelRect);

//	m_GenresLabelInt.InflateRect(1,1,1,5);
//	m_ArtistsLabelInt.InflateRect(1,1,1,1);
//	m_AlbumsLabelInt.InflateRect(1,1,1,1);
//	m_SongsLabelInt.InflateRect(1,1,1,1);
	m_ColHdrsRect.UnionRect(m_GenresLabelRect,m_SongsLabelRect);
	m_LibraryRect.UnionRect(m_SearchEditRect,m_SongsRect);
	//GetClientRect(m_LibraryRect);
}
void CPlayerDlg::OnSearchDlg() {
	if (m_ShowSearchPanel) {
		OnNoSearchDlg();
		return;
	}
	m_SearchEdit.SetWindowText("");
	m_SearchGo.SetWindowText("Search");
	m_LastShowSearchPanel = m_ShowSearchPanel;
	m_ShowSearchPanel = TRUE;
	m_mlib.SearchSetup();
	m_Need2Erase = TRUE;
	EraseBkgndNC();
	ShowSearchDlg();
	PlayerStatusSet("");
	//InvalidateRect(m_WindowRect);
	RedrawWindow();

}
void CPlayerDlg::OnNoSearchDlg() {
	CWaitCursor c;
	m_LastShowSearchPanel = m_ShowSearchPanel;
	m_ShowSearchPanel = FALSE;
	m_mlib.SearchCancel();
	m_Genres.SetCurSel(0);
	ShowSearchDlg();
	_lastSelectedGenre = "xxx";
	_lastSelectedArtist = "xxx";
	_selectedGenre = "";
	_selectedArtist = "";
	_selectedAlbum = "";
	_selectedSong = "";
	initDb();
	PlayerStatusSet("");
	m_SearchStatus.setText("",DT_RIGHT);
	m_Genres.SetFocus();
}

void CPlayerDlg::OnSearchGo() {

	if (!m_ShowSearchPanel) return;
	CString txt;
	m_SearchEdit.GetWindowText(txt);
	CWaitCursor c;
	int found = 0;
	m_SearchGo.SetWindowText("Searching");
	if (0 < (found = m_mlib.Search(txt))) {
		m_Genres.SetCurSel(0);
//		ShowSearchDlg();
		_lastSelectedGenre = "xxx";
		_lastSelectedArtist = "xxx";
		_selectedGenre = "";
		_selectedArtist = "";
		initDb();
		m_SearchCleared = FALSE;
	}
	m_SearchGo.SetWindowText("Search");
	if (txt.GetLength()) {
		CString msg = numToString(found);
		if (1 == found)
			msg += " song";
		else 
			msg += " songs";
		msg += " found with '" + txt + "' ";
		PlayerStatusSet("");
		m_SearchStatus.setText(msg,DT_RIGHT);
	}
	m_SearchEdit.SetFocus();
}
void CPlayerDlg::OnSearchClear() 
{
	m_SearchStatus.setText("",DT_RIGHT);
	m_SearchEdit.SetWindowText("");
	if (!m_ShowSearchPanel) return;
	if (m_SearchCleared) return;
	m_SearchCleared = TRUE;
	CWaitCursor c;
	m_mlib.SearchClear();
	m_Genres.SetCurSel(0);
	ShowSearchDlg();
	_lastSelectedGenre = "xxx";
	_lastSelectedArtist = "xxx";
	_selectedGenre = "";
	_selectedArtist = "";
	initDb();
	m_SearchEdit.SetFocus();
}
void
CPlayerDlg::init() {
	// Not to be called by CPlayerDlg::InitDialog
	m_mlib.readDbLocation();
	logger.close();
	CString lpath = m_mlib.getDbLocation();
	lpath += "\\";
	lpath += MUZIKBROWZER;
	lpath += ".log";
    logger.open(lpath);
	logger.log(CS("Muzikbrowzer version: ") + CS(MUZIKBROWZER_VERSION));

	m_mlib.MovePlaylistsToDir();

    if (m_mlib.init()) {
		MBMessageBox("Error", "Database corrupted. Rebuild it by performing a\r\nComplete Rebuild.");
		PlayerStatusSet(CString(
			"Database corrupted. Do a Complete Rebuild."));
	}
	_selectedGenre = "";
	_selectedArtist = "";
    initDb();
}

void
CPlayerDlg::initDb() {
	//	IRReaderStop();
    m_Genres.ResetContent();
    m_Songs.ResetContent();
    m_Albums.ResetContent();
    m_Artists.ResetContent();

	m_mlib.getGenres(m_Genres);
	if (m_Genres.GetCount() == 0) {
		_selectedGenre = "";
		_selectedArtist = "";
		_selectedAlbum = "";
		_selectedSong = "";
		PlayerStatusSet(m_mlib.getLibraryCounts());
		PlayerStatusTempSet("");
	}

	if (_selectedGenre == ""
			|| m_Genres.SelectString(0, _selectedGenre) == LB_ERR) {
		m_Genres.SetCurSel(0);
	}
	m_Artists.SetCurSel(0);
	m_Albums.SetCurSel(0);
	OnSelchangeGenres() ;
	m_Genres.invalidate();


	//	IRReaderStart();

}

void CPlayerDlg::OnSysCommand(UINT nID, LONG lParam)
{
//	logger.ods("OnSysCommand:" + numToString(nID) + " " + numToString(lParam));

	if ((nID & 0xFFF0) == IDM_ABOUTBOX)	{
		CRect rect;
		GetWindowRect(rect);
		About dlgAbout(rect);
		dlgAbout.DoModal();
	} else if (nID == SC_RESTORE) {
		m_Need2Erase = TRUE;
		CDialogClassImpl::OnSysCommand(nID, lParam);
		RedrawWindow();
	} else if (SC_KEYMENU == nID) {
		switch(lParam) {
		case 'm': case 'M': 
			OnButtonMenu(); break;
		case 'a': case 'A': Pause();			break;
		case 'c': case 'C': OnMenuClearplaylist();		break;
		case 'd': case 'D': OnMenuRandomizePlaylist();	break;
		case 'e': case 'E': OnReverse();		break;
		case 'f': case 'F': OnFastForward();	break;
		case 'h': case 'H': OnMenuShuffleplaylist();	break;
		case 'i': case 'I': OnPreviousSong();	break;
		case 'j': case 'J': HandleIRMessage(IR_MESSAGE_ALPHAUP); break;
		case 'k': case 'K': HandleIRMessage(IR_MESSAGE_ALPHADOWN); break;
		case 'l': case 'L': OnMenuLoadplaylist();		break;
		case 'n': case 'N': OnNextSong();		break;
		case 'o': case 'O': OnVolDown();		break;
		case 'p': case 'P': Play();				break;
		case 'r': case 'R': OnMenuButton();	break;
		case 's': case 'S': Stop();				break;
		case 'u': case 'U': OnVolUp();			break;
		case 'v': case 'V': OnMenuSaveplaylist();		break;

//		case 'z': case 'Z': m_mlib.Scan(); break;
//		case 'b': case 'B': {
//				m_mlib.garbageCollectTest();
//				OnSelchangeGenres();
//			}
			break;
//		case 'z': case 'Z': OnMusicScan();		break;
		}
    } else {
		CDialogClassImpl::OnSysCommand(nID, lParam);
	}
}
#pragma hack
// get rid of the 'z' above before release.

void CPlayerDlg::OnVerify() {
	CString msg = m_mlib.JustVerify();
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPlayerDlg::OnPaint() 
{
//	logger.ods("OnPaint");
	if (m_Resizing) return;
//	CString msg="CPlayerDlg OnPaint\r\n"; OutputDebugString(msg);
	CPaintDC dc(this); // device context for painting


	CDialogClassImpl::OnPaint();
//	msg="CPlayerDlg OnPaint done\r\n"; OutputDebugString(msg);
}




// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPlayerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CPlayerDlg::DestroyWindow() 
{
	SavePlaylist("Last");
	// delete the Xaudio player
    if (m_Player) {
        Stop();
    }

	//	IRReaderStop();
	return CDialogClassImpl::DestroyWindow();
}

void CPlayerDlg::OnOpenFileButton() 
{
    // open a file
	CFileDialog *dialog;

	dialog = new CFileDialog(TRUE, 
							"mp3",
							"",
							OFN_FILEMUSTEXIST    | 
							OFN_HIDEREADONLY     |
							OFN_EXPLORER,
"mp3,wma,wav,ogg files{*.mp3;*.wma;*.wav;*.ogg;*.mpg;*.mp1;*.mp2}|*.mp3;*.wma;*.wav;*.ogg;*.mpg;*.mp1;*.mp2|All Files {*.*}|*.*||");


	int ret;
	ret = dialog->DoModal();
	if (ret == IDOK) {
        // a file was selected
		m_mlib.addFileToPlaylist(dialog->GetPathName());
		updatePlaylist();	
	}

	delete dialog;
	
}

#ifdef asdf
void CPlayerDlg::OnOpenUrlButton() 
{
	CUrlDlg *dialog;

	dialog = new CUrlDlg();

	int ret;
	ret = dialog->DoModal();
	if (ret == IDOK) {
		// an URL was entered
		m_Player->InputOpen(dialog->m_URL);
	}
	delete dialog;
}
#endif
#ifdef asdf
void CPlayerDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
OnHScroll(nSBCode, nPos, pScrollBar) ;
}
void CPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	return;
	if (pScrollBar->m_hWnd == m_PositionSlider.m_hWnd) {
		switch (nSBCode) {
			case SB_ENDSCROLL: 
				adjustPosition();
				break;
			case SB_THUMBPOSITION: 
				adjustPosition(nPos);
				break;
		}
		CDialogClassImpl::OnHScroll(nSBCode, nPos, pScrollBar);
	} else if (pScrollBar->m_hWnd == m_VolumeSlider.m_hWnd) {
		switch (nSBCode) {
			case SB_ENDSCROLL: 
				adjustVolume();
				break;
			case SB_THUMBPOSITION: 
			case SB_THUMBTRACK:
				adjustVolume(nPos);
//				OutputDebugString("vol nPos " + numToString(nPos) + "\r\n");
				break;
		}
		CDialogClassImpl::OnVScroll(nSBCode, nPos, pScrollBar);
	}
}
#endif
void CPlayerDlg::OnAbout() 
{
		CRect rect;
		GetWindowRect(rect);
		About dlgAbout(rect);
		dlgAbout.DoModal();
}

void CPlayerDlg::OnControlSelChange() {
    if (m_Control->ptr() == &m_Genres) {
        OnSelchangeGenres();
		m_Genres.invalidate();
    } else if (m_Control->ptr() == &m_Artists) {
        OnSelchangeArtists();
		m_Artists.invalidate();
    } else if (m_Control->ptr() == &m_Albums) {
        OnSelchangeAlbums();
		m_Albums.invalidate();
    } else if (m_Control->ptr() == &m_Songs) {
        OnSelchangeSongs();
		m_Songs.invalidate();
    } else if (m_Control->ptr() == &m_Playlist) {
        OnSelchangePlaylist();
		m_Playlist.invalidate();
    }
}

BOOL CPlayerDlg::OnControlClick() {
    if (m_Control->ptr() == &m_Genres) {
        OnDblclkGenres() ; return TRUE;
    } else if (m_Control->ptr() == &m_Artists) {
        OnDblclkArtists() ;return TRUE;
    } else if (m_Control->ptr() == &m_Albums) {
        OnDblclkAlbums() ;return TRUE;
    } else if (m_Control->ptr() == &m_Songs) {
        OnDblclkSongs();return TRUE;
    } else if (m_Control->ptr() == &m_Playlist) {
        OnDblclkPlaylist();return TRUE;

//    } else if (m_Control->ptr() == m_MenuDlg) {
//        m_MenuDlg->EndDialog(m_MenuDlg->currentbutton());
    } else {
		return FALSE;
	}
}
static void
rememberSelections(CMapStringToString & map, CString sel,
                   CExtendedListBox & box) {
    CString val;
    if (map.Lookup(sel, val) == 0 || box.SelectString(-1, val) == LB_ERR) {
        box.SetCurSel(0);
    }
#ifdef asdf
    if (map.Lookup(sel, val) == 0) {
        box.SetCurSel(0);
    } else {
        int sel = box.GetCurSel();
        if (box.SelectString(0, val) == LB_ERR) {
            box.SetCurSel(sel);
        }
    }
#endif
}

void CPlayerDlg::OnSelchangeGenres() 
{
	m_UpdateNeeded[0] = TRUE;
	m_UpdateNeeded[1] = TRUE;
	m_UpdateNeeded[2] = TRUE;
	m_Artists.invalidate();
}
void CPlayerDlg::OnSelchangeArtists() 
{
	m_UpdateNeeded[1] = TRUE;
	m_UpdateNeeded[2] = TRUE;
	m_Albums.invalidate();
}
void CPlayerDlg::OnSelchangeAlbums() 
{
	m_UpdateNeeded[2] = TRUE;
	m_Songs.invalidate();
}
void CPlayerDlg::OnSelchangeSongs() 
{
}
BOOL CPlayerDlg::UpdateArtists() // called by m_Artists::OnPaint
{
	BOOL updated = FALSE;
	static int lastsel = -1;
	if (DBLOCKED) return updated;
	int sel = m_Genres.GetCurSel();
	_lastSelectedGenre = _selectedGenre;
	if (sel > -1)
		m_Genres.GetText(sel, _selectedGenre);
	if (_selectedGenre == MBALL) {
		PlayerStatusTempSet(m_mlib.getLibraryCounts());
	}
	if (TRUE == m_UpdateNeeded[0] || _lastSelectedGenre != _selectedGenre) {
		m_UpdateNeeded[0] = FALSE;
//		logger.ods("	UpdateArtists");
		CWaitCursor c;
		m_Artists.ResetContent();	
		m_mlib.getArtists(_selectedGenre, m_Artists);
		rememberSelections(m_GenreArtist, _selectedGenre, m_Artists);
		m_Albums.invalidate();
		m_Artists.invalidate();
		updated = TRUE; // TRUE so ELB won't finish OnPainting cause it's
						// about to do the actual OnPaint due to the invalidate()
	}
	if (lastsel != sel)	m_LastThingQueuedUp = ""; lastsel = sel;
	return updated;
}
BOOL CPlayerDlg::UpdateAlbums() // called by m_Albums::OnPaint
{
	BOOL updated = FALSE;
	static int lastsel = -1;
	if (DBLOCKED) return updated;
	int sel = m_Artists.GetCurSel();
	_lastSelectedArtist = _selectedArtist;
	if (sel > -1)
		m_Artists.GetText(sel,_selectedArtist);
	m_GenreArtist.SetAt(_selectedGenre, _selectedArtist);
	if (TRUE == m_UpdateNeeded[1] || _lastSelectedGenre != _selectedGenre
			|| _lastSelectedArtist != _selectedArtist) {
		m_UpdateNeeded[1] = FALSE;
//		logger.ods("	UpdateAlbums");
		CWaitCursor c;
		m_Albums.ResetContent();	
		m_mlib.getAlbums(_selectedGenre,
			_selectedArtist, m_Albums, m_Config.AlbumSortAlpha());
		rememberSelections(m_ArtistAlbum, _selectedArtist, m_Albums);
		m_Songs.invalidate();
		m_Albums.invalidate();
		updated = TRUE;
	}
	if (lastsel != sel)	m_LastThingQueuedUp = ""; lastsel = sel;
	return updated;
}
BOOL CPlayerDlg::UpdateSongs()
{
	BOOL updated = FALSE;
	static int lastsel = -1;
	if (DBLOCKED) return updated;
	int sel = m_Albums.GetCurSel();
	_lastSelectedAlbum = _selectedAlbum;
	if (sel > -1)
		m_Albums.GetText(sel,_selectedAlbum);
    m_ArtistAlbum.SetAt(_selectedArtist, _selectedAlbum);
	if (TRUE == m_UpdateNeeded[2] || _lastSelectedGenre != _selectedGenre
			|| _lastSelectedArtist != _selectedArtist
			|| _lastSelectedAlbum != _selectedAlbum) {
		m_UpdateNeeded[2] = FALSE;
//		logger.ods("	UpdateSongs");
		m_Songs.ResetContent();	
		m_mlib.getSongs(_selectedGenre,
			_selectedArtist,
			_selectedAlbum,m_Songs);
		rememberSelections(m_AlbumSong, _selectedAlbum, m_Songs);
		m_Songs.invalidate();
		updated = TRUE;
		return updated; // No need to throw the album art up yet
	}

	sel = m_Songs.GetCurSel();
	if (sel > -1)
		m_Songs.GetText(sel,_selectedSong);
	m_AlbumSong.SetAt(_selectedAlbum, _selectedSong);

#pragma hack
	if (_selectedSong.GetLength()
		&& _selectedAlbum.GetLength()
		&& _selectedArtist.GetLength()
		&& _selectedGenre.GetLength()
		) { // hack! not doing this check
		// adds a song to the db!
		Song song = m_mlib.getSong(_selectedGenre, _selectedArtist,
			_selectedAlbum, _selectedSong);
		CString file = song->getId3("FILE");
		displayAlbumArt(file,song->getId3("TALB"));
	}
	if (lastsel != sel)	m_LastThingQueuedUp = ""; lastsel = sel;
	return updated;
}

void CPlayerDlg::OnSelchangePlaylist() 
{
	if (DBLOCKED) return;
	int sel = m_Playlist.GetCurSel();
	if (sel > -1) {
		m_Playlist.GetText(sel,_selectedPlaylistSong);
	}
    if (sel < 0) return;
    m_Playlist.invalidate();

	CString file = m_mlib._playlist[sel]->getId3("FILE");
	displayAlbumArt(file,m_mlib._playlist[sel]->getId3("TALB"));
}
void CPlayerDlg::OnKillfocusVirtuals() 
{
	// Now that we have non virtual controls, need this
	// to prevent misdirected actions from going to the
	// most recent active virtual control.

	// but first repaint w losing focus
	((CWnd*)m_Control->ptr())->InvalidateRgn(NULL);

	m_Control->zero();
	
}
void
CPlayerDlg::OnGenresFocus( ) {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_GENRES);
    PlayerStatusTempSet(_selectedGenre);
	//OnSelchangeGenres();
	m_Genres.InvalidateRgn(NULL);
}
void
CPlayerDlg::OnArtistsFocus() {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_ARTISTS);
    PlayerStatusTempSet(_selectedArtist);
	//OnSelchangeArtists();
	m_Artists.InvalidateRgn(NULL);
}
void
CPlayerDlg::OnAlbumsFocus() {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_ALBUMS);
    PlayerStatusTempSet(_selectedAlbum);
	//OnSelchangeAlbums();
	m_Albums.InvalidateRgn(NULL);
}
void
CPlayerDlg::OnSongsFocus() {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_SONGS);
    PlayerStatusTempSet(_selectedSong);
	//OnSelchangeSongs();
	m_Songs.InvalidateRgn(NULL);
}
void
CPlayerDlg::OnPlaylistFocus() {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_PLAYLIST);
	int sel = m_Playlist.GetCurSel();
	if (sel > -1) {
		m_Playlist.GetText(sel,_selectedPlaylistSong);
	}
    PlayerStatusTempSet(_selectedPlaylistSong);
	OnSelchangePlaylist() ;
	if (m_Playlist.GetCount() == 0)
		m_Playlist.InvalidateRgn(NULL);;
}
void CPlayerDlg::updatePlaylistPostEdit() {
	int sel = m_Playlist.GetCurSel();
	m_Playlist.ResetContent();
	m_mlib.getPlaylist(m_Playlist);
	m_Playlist.SetCurSel(sel);
	m_Playlist.invalidate();
}
void CPlayerDlg::updatePlaylist(const BOOL save) {
	m_Playlist.ResetContent();
	m_mlib.getPlaylist(m_Playlist);
	m_Playlist.SetCurSel(m_Playlist.GetCount()-1);

	calcDuration();

	UpdateWindow();
    PlayLoop();

    m_Playlist.invalidate();
	if (save)
		SavePlaylist("Last");
}

void CPlayerDlg::OnDblclkSongs() 
{
	CString last = _selectedGenre + _selectedArtist + _selectedAlbum +
		_selectedSong;
	if (last != m_LastThingQueuedUp) {
		Song song = new CSong;
	    int sel = m_Songs.GetCurSel();
		DWORD data = m_Songs.GetItemData(sel);
		song = m_mlib.m_SongLib.getSong(data);
		m_mlib.addSongToPlaylist(song);
		updatePlaylist();
		m_LastThingQueuedUp = last;
	} else if (last.GetLength()) {
		PlayerStatusTempSet("You just added that!");
	}
}

LRESULT CPlayerDlg::OnDblclkAlbums(WPARAM, LPARAM) {
	OnDblclkAlbums() ;
	return 0;
}
void CPlayerDlg::OnDblclkAlbums() 
{
	static justclicked = TRUE;
	CString last = _selectedGenre + _selectedArtist + _selectedAlbum;
	if (last != m_LastThingQueuedUp) {
		if (justclicked) {
			PostMessage(MB_DBLCLICK_ALBUMS, (WPARAM)0, (LPARAM)0);
			justclicked = FALSE;
		} else {
			justclicked = TRUE;
			m_mlib.addAlbumToPlaylist(_selectedGenre, _selectedArtist, _selectedAlbum);
			updatePlaylist();
			m_LastThingQueuedUp = last;
		}
	} else if (last.GetLength()) {
		PlayerStatusTempSet("You just added that!");
	}
}
LRESULT CPlayerDlg::OnDblclkArtists(WPARAM, LPARAM) {
	OnDblclkArtists();
	return 0;
}
void CPlayerDlg::OnDblclkArtists()
{
	static justclicked = TRUE;
	if (_selectedArtist == MBALL) return;
	CString last = _selectedGenre + _selectedArtist;
	if (last != m_LastThingQueuedUp) {
		if (justclicked) {
			PostMessage(MB_DBLCLICK_ARTISTS, (WPARAM)0, (LPARAM)0);
			justclicked = FALSE;
		} else {
			justclicked = TRUE;
			m_mlib.addArtistToPlaylist(_selectedGenre, _selectedArtist);
			updatePlaylist();
			m_LastThingQueuedUp = last;
		}
	} else if (last.GetLength()) {
		PlayerStatusTempSet("You just added that!");
	}
}
LRESULT CPlayerDlg::OnDblclkGenres(WPARAM, LPARAM) {
	OnDblclkGenres();
	return 0;
}
void CPlayerDlg::OnDblclkGenres() 
{
	static justclicked = TRUE;
	if (_selectedGenre == MBALL) return;
	CString last = _selectedGenre;
	if (last != m_LastThingQueuedUp) {
		if (justclicked) {
			PostMessage(MB_DBLCLICK_GENRES, (WPARAM)0, (LPARAM)0);
			justclicked = FALSE;
		} else {
			justclicked = TRUE;
			m_mlib.addGenreToPlaylist(_selectedGenre);	
			updatePlaylist();
			m_LastThingQueuedUp = last;
		}
	} else if (last.GetLength()) {
		PlayerStatusTempSet("You just added that!");
	}
}

void CPlayerDlg::OnDblclkPlaylist() 
{
    int sel = m_Playlist.GetCurSel();
    m_PlaylistCurrent = sel -1;
    Stop();
	m_StopFlag = FALSE;

    PlayLoop();

}

BOOL CPlayerDlg::PreTranslateMessage(MSG* pMsg)
{	// disable ESC & ENTER from killing the dialog
	// returning TRUE says msg has been processed, don't further process
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_RETURN) {
			if (OnControlClick()) // if one of VirtualControls
				return TRUE;
		} else if (pMsg->wParam == VK_ESCAPE) {
			return TRUE;
        } else if (pMsg->wParam == VK_DELETE) {
            //if (OnDelete()) // changed OnDelete to void per proper sig.
			OnDelete();
			return TRUE;
		} else if (pMsg->wParam == VK_F5) {
			resetControls();
			return TRUE;
        }
    }
	// Since the SearchGo button is the only default button, hitting
	// return presses that button. There's code in OnSearchGo
	// to prevent a search if it's not in view.
	// If there's no default button, a return will exit the dialog.
	return CDialogClassImpl::PreTranslateMessage(pMsg);
}

void CPlayerDlg::OnMenuButton() 
{
    int r = -1;
    if (m_MenuDlg == 0) {
        m_MenuDlg = new RMenuDlg (this, &m_callbacks);
        *m_Dialog = m_MenuDlg;
	    r = m_MenuDlg->DoModal();
	    delete m_MenuDlg;
	    m_MenuDlg = 0;
		*m_Dialog = this;
    }

    switch (r) {
    case 10:     // exit
//        OnMenuExit();
        break;
    case 11:     // shuffle
        OnMenuShuffleplaylist();
        break;
    case 12:     // randomize
        OnMenuRandomizePlaylist();
        break;
    case 13:     // clear
        OnMenuClearplaylist();
        break;
    case 14:
        SetSavePlaylistFlag(FALSE);
        OnMenuSaveplaylist();
        break;
    case 15:      // load
        OnMenuLoadplaylist();
        break;
    case 16:     // pause
        OnMenuPause();
        break;
    case 17:     // play
        OnMenuPlay();
        break;
    case 18:     // stop
        OnMenuStop();
        break;
    case 19:     // help
        OnMenuHelp();
    }
}

void CPlayerDlg::OnDelete() {

	mWindowFlag = 0;
    if (m_Control->ptr() == &m_Playlist) {
	    int sel = m_Playlist.GetCurSel();
        if (sel < 0) { return ; }
        if (sel == m_PlaylistCurrent) {
            Stop();
            m_PlaylistCurrent = sel-1;
			PostMessage(WM_PLAYLOOP, (WPARAM)0, (LPARAM)0);
        }
        void  * song = m_Playlist.GetItemDataPtr(sel);
        m_Playlist.DeleteString(sel);
        m_mlib.deleteSongFromPlaylist((PlaylistNode*)song);
        if (m_Playlist.GetCount()) {
            if (sel+1 > m_Playlist.GetCount()) {
                sel--;
                if (sel < 0) sel = 0;
            }
            m_Playlist.SetCurSel(sel);
            m_Playlist.GetText(sel,_selectedPlaylistSong);
            PlayerStatusTempSet(_selectedPlaylistSong);
            UpdateData(TRUE);
//            m_mlib.dumpPL(m_PlaylistCurrent);
        } else {
			PlayerStatusClear();
			_selectedPlaylistSong = "";
			m_StopFlag = FALSE;
//            PlayerStatusSet(m_mlib.getLibraryCounts());
        }
		calcDuration();
		m_LastThingQueuedUp = "";
		OnSelchangePlaylist() ;
		return ;
    } else if (!m_HandlingIRMessage && (
		(m_Control->ptr() == &m_Genres && (mWindowFlag = 1))
			|| (m_Control->ptr() == &m_Artists && (mWindowFlag = 2))
			|| (m_Control->ptr() == &m_Albums && (mWindowFlag = 3))
			|| (m_Control->ptr() == &m_Songs && (mWindowFlag = 4))) )
	{
		m_ModifyDelete = TRUE;
		OnUserEditSong();
	}
	m_ModifyDelete = FALSE;
	return ;
}

void
CPlayerDlg::OnPageUp() {
    if (!m_Control->isList()) {
        return;
    }

    CRect wrect;
    ((CExtendedListBox*)m_Control->ptr())->GetWindowRect( wrect ) ;
    int iheight = ((CExtendedListBox*)m_Control->ptr())->GetItemHeight(0);

    int pagelen = wrect.Height() / iheight;
    m_Control->up(pagelen-1);
    OnControlSelChange();
}

void
CPlayerDlg::OnPageDown() {
    if (!m_Control->isList()) {
        return;
    }

    CRect wrect;
    ((CExtendedListBox*)m_Control->ptr())->GetWindowRect( wrect ) ;
    int iheight = ((CExtendedListBox*)m_Control->ptr())->GetItemHeight(0);

    int pagelen = wrect.Height() / iheight;
    m_Control->down(pagelen-1);
    OnControlSelChange();
}

// WinNT, not 98, not XP
BOOL CALLBACK KillScreenSaverFunc (HWND hWnd, LPARAM lParam){
    PostMessage(hWnd,WM_CLOSE,0,0);

    return TRUE;
}

// Need to try GetVersionEx() to determine version of windows running
// also try keybd_event and SendInput
// PostMessage (GetForegroundWindow(), WM_CLOSE, 0, 0L);

BOOL WakeUp (){
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	BOOL bIsWindowsNT =
		(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);

//    if(bIsWindowsNT) {
        HDESK hDesktop;

//        hDesktop=OpenDesktop("screen-saver",0,FALSE,MAXIMUM_ALLOWED);
		hDesktop=OpenDesktop("screen-saver",0,FALSE,DESKTOP_WRITEOBJECTS);
        if (hDesktop!=NULL) {
            EnumDesktopWindows(hDesktop,(WNDENUMPROC)KillScreenSaverFunc,0);
            CloseDesktop(hDesktop);
        }
//    } else {
        keybd_event(VK_SHIFT,0,0,0);
	    keybd_event(VK_SHIFT,0,KEYEVENTF_KEYUP,0);
//    }

//    PostMessage (GetActiveWindow(), WM_KEYDOWN, VK_SHIFT, 0L);
//    PostMessage (GetActiveWindow(), WM_KEYUP, VK_SHIFT, 0L);
	return true;
}

#ifdef asdf
BOOL CALLBACK KillScreenSaverFunc(HWND hwnd, LPARAM lParam)
{
  if(IsWindowVisible(hwnd))
      PostMessage(hwnd, WM_CLOSE, 0, 0);
  return TRUE;
}

BOOL EndScreenSaver()
{
	OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	BOOL bIsWindowsNT =
		(osvi.dwPlatformId == VER_PLATFORM_WIN32_NT);

	if(!bIsWindowsNT)
	{
		HWND hWnd = FindWindow("WindowsScreenSaverClass",
NULL); 
 
		if (hWnd) {
			PostMessage(hWnd, WM_CLOSE, 0, 0);
		}
	}
	else
	{
		HDESK hdesk;

		hdesk = OpenDesktop(TEXT("Screen-saver"),
						   0,
						   FALSE,
						   DESKTOP_READOBJECTS
| DESKTOP_WRITEOBJECTS);
		if (hdesk)
		{
		  EnumDesktopWindows(hdesk, KillScreenSaverFunc, 0);
		  CloseDesktop(hdesk);
		} 
	}



    return TRUE;
}
#endif

LRESULT
CPlayerDlg::OnGraphNotify(WPARAM wParam, LPARAM lParam) {
	m_Player->HandleGraphEvent();
	return 0;
}

LRESULT
CPlayerDlg::OnPlayloop(WPARAM wParam, LPARAM lParam) {
	PlayLoop();
	return 0;
}
afx_msg LRESULT
CPlayerDlg::OnSerialMsg (WPARAM wParam, LPARAM lParam) {

	int key = 0;
	BOOL more = TRUE;
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) {
		rrcvr->HandleSerialMsg(wParam, lParam, key);
		HandleIRMessage(key);
	}
	return 0;
}
int
CPlayerDlg::HandleIRMessage(int key) {
	m_HandlingIRMessage = TRUE;
	int wakeitup = 1;
    switch (key) {
	case IR_MESSAGE_UP: {
        m_Dialog->control().up(1);
		m_Dialog->OnControlSelChange();
		break;}
	case IR_MESSAGE_DOWN: {
        m_Dialog->control().down(1);
		m_Dialog->OnControlSelChange();
		break;}
	case IR_MESSAGE_LEFT: {
//		Tab to previous control
		m_Dialog->PrevDlgCtrl();
		break; }
	case IR_MESSAGE_RIGHT: {
//		Tab to next control
		m_Dialog->NextDlgCtrl();
		break; }
    case IR_MESSAGE_SELECT: {
        m_Dialog->OnControlClick();
        break;}
    case IR_MESSAGE_MENU: {
        m_Dialog->OnMenuButton();
        break;}
    case IR_MESSAGE_EXIT: {
        m_Dialog->OnExit();
        break;}
    case IR_MESSAGE_DELETE: {
        m_Dialog->OnDelete();
        break; }
	case IR_MESSAGE_VOLUP:
		OnVolUp();
		break;
	case IR_MESSAGE_VOLDOWN:
		OnVolDown();
		break;
    case IR_MESSAGE_PAGEUP: {
        m_Dialog->OnPageUp();
        break; }
    case IR_MESSAGE_PAGEDOWN: {
        m_Dialog->OnPageDown();
        break; }
    case IR_MESSAGE_PLAY: {
        OnMenuPlay();
        break;}
	case IR_MESSAGE_PAUSE: {
        OnMenuPause();
        break;}
	case IR_MESSAGE_STOP: {
        OnMenuStop();
        break;}
	case IR_MESSAGE_NEXT: {
        OnNextSong();
        break;}
	case IR_MESSAGE_PREVIOUS: {
        OnPreviousSong();
        break;}
	case IR_MESSAGE_FASTFORWARD: {
		OnFastForward();
		break;}
	case IR_MESSAGE_REVERSE: {
		OnReverse();
		break;}
	case IR_MESSAGE_ALPHAUP: {
        m_Dialog->control().alphaUp();
		m_Dialog->OnControlSelChange();
		break;}
	case IR_MESSAGE_ALPHADOWN: {
        m_Dialog->control().alphaDown();
		m_Dialog->OnControlSelChange();
		break;}
	case IR_MESSAGE_RANDOM: {
		OnMenuRandomizePlaylist();
		break;}
	case IR_MESSAGE_SHUFFLE: {
		OnMenuShuffleplaylist();
		break;}
	case IR_MESSAGE_CLEAR: {
		OnMenuClearplaylist();
		break;}
	case IR_MESSAGE_LOAD: {
		OnMenuLoadplaylist();
		break;}
	case IR_MESSAGE_SAVE: {
		SetSavePlaylistFlag(FALSE);
		OnMenuSaveplaylist();
		break;}
	default:
		wakeitup = 0;
	}
	m_HandlingIRMessage = FALSE;

	if (wakeitup) {
	    WakeUp();
	    if (IsIconic()) {
			ShowWindow(SW_RESTORE);
			return 0;
		}
	}
    return 0;
}
void
CPlayerDlg::nextDlgCtrl() {
	CWnd * f = GetFocus();
	if ((CWnd * )&m_Playlist == f
		|| &m_SearchClear == f
		|| &m_SearchCancel == f
		|| &m_SearchGo == f
		|| &m_SearchEdit == f
		) {
		if (!m_Config.hideGenre())
			GotoDlgCtrl(&m_Genres);
		else
			GotoDlgCtrl(&m_Artists);
	} else
		NextDlgCtrl();
}
void
CPlayerDlg::prevDlgCtrl() {
	CWnd * f = GetFocus();
	if ((&m_Genres== f
		|| &m_SearchClear == f
		|| &m_SearchCancel == f
		|| &m_SearchGo == f
		|| &m_SearchEdit == f
		)
		|| (m_Config.hideGenre() && f == &m_Artists)
		) 
	{
		GotoDlgCtrl(&m_Playlist);
	} else
		PrevDlgCtrl();
}



void
PostMyIdleMessage() {
    // Post it once a second
//	OutputDebugString("PostMyIdleMessage");
    CTime t = CTime::GetCurrentTime();
    static int last = t.GetSecond();
    if (t.GetSecond() != last) {
        last = t.GetSecond();
        HWND hwnd = AfxGetMainWnd()->m_hWnd;
//		OutputDebugString("PostMessage PostMyIdleMessage");
//	    ::PostMessage(hwnd, MB_POST_MYIDLE_MESSAGE, (WPARAM)0, (LPARAM)0);
    }
}
LRESULT
CPlayerDlg::OnPostMyIdle(WPARAM wParam, LPARAM lParam) {
//	OutputDebugString("OnPostMyIdle");
    PlayerStatusRevert();
    return 0;
}
void CPlayerDlg::OnMenuCheckem() {
	OnMenuClearplaylist();
	m_CheckSongCount = m_mlib.getSongCount();
	m_CheckGoodCount = 0;
	m_CheckBadCount = 0;
	m_CheckSongCounter = 0;
	if (0 == m_CheckSongCount) {
		m_CheckingEm = -1;
		return;
	}
	m_mlib.readDbLocation();
    CString lpath = m_mlib.getDbLocation();
	lpath += "\\";
	lpath += MUZIKBROWZER;
	lpath += "-Check";
	lpath += ".log";
    CheckEmLog.open(lpath);
	CheckEmLog.log(CS("Muzikbrowzer version: ") + CS(MUZIKBROWZER_VERSION));
	CheckEmLog.log("Checking all files for playability");
	logger.log("Checking all files for playability");
	logger.log("see "+lpath);
	m_CheckingEm = TRUE;
	CString first = m_mlib.getSongFileName(m_CheckSongCounter);
	m_mlib.addFileToPlaylist(first);
	updatePlaylist();	
}
// Configuration dlg
void CPlayerDlg::OnMenuOptions() {
	
	CString skinnameOrig = m_Config.getCurrentSkin();
	CWaitCursor cw;
	m_Config.DoModal();
	cw.Restore();
	m_Config.initSkins();
	m_Config.getSkins(m_Skins);
	CString skinname = m_Config.getCurrentSkin();
	if (!m_Config.verifySkin(skinname,FALSE)) {
		if (!m_Config.findGoodSkin()) {
			MBMessageBox("Alert","No skins folder!",TRUE);
			skinname = skinnameOrig;
		}
	}
	if (skinnameOrig != skinname) {
		resetControls();
	}

	PlayerStatusClear();
	m_AlbumArt = m_Config.getSkin(MB_SKIN_ALBUMART);

//	CString menuFunc,menuDesc;
	RemoteReceiver * rrcvr = RemoteReceiver::reset(this, MB_SERIAL_MESSAGE);	
//	if (rrcvr)
//		rrcvr->getDescs(IR_MESSAGE_MENU, menuFunc, menuDesc);
//	if (menuDesc == "") menuDesc = "Menu";
//	m_HelpMsg = "Press "; m_HelpMsg += menuDesc; m_HelpMsg += " for options.";
	m_HelpMsg = "";
	if (m_Config.trialMode() == 1) {
		m_HelpMsg += " Trial Mode.";
	} else if (m_Config.trialMode() == 2) {
		m_HelpMsg = " Trial Expired.";
	}
	if (m_Config.resetNeeded()) {
		resetControls();
		OnSelchangeAlbums();
	}

}
void
CPlayerDlg::redraw() {
	resetControls();
}
void CPlayerDlg::OnAlphaUp() {
	HandleIRMessage(IR_MESSAGE_ALPHAUP);
}
void CPlayerDlg::OnAlphaDown() {
	HandleIRMessage(IR_MESSAGE_ALPHADOWN);
}
void CPlayerDlg::OnMenuClearplaylist() 
{
    m_mlib._playlist.reset();
    m_PlaylistCurrent = -1;
    updatePlaylist(FALSE);
    Stop();
    InputClose();
    //CString libcounts = m_mlib.getLibraryCounts();
	resetPosition();
	PlayerStatusClear();
	_selectedPlaylistSong = "";
	m_StopFlag = FALSE;
	m_LastThingQueuedUp = "";
	
}
void CPlayerDlg::OnCancel() {
	StopTimers();
    saveConfig();
    EndDialog(0);
}
void
CPlayerDlg::ReadWindowPos(int & max, CRect & rect) {
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
    int x1,x2,y1,y2;

    max = reg.Read(RegWindowMaximized, 0);
    x1 = reg.Read(RegWindowX1, 0);
    y1 = reg.Read(RegWindowY1, 0);
    x2 = reg.Read(RegWindowX2, 0);
    y2 = reg.Read(RegWindowY2, 0);
    CPoint tl(x1,y1);
    CPoint br(x2,y2);
    rect.TopLeft() = tl;
    rect.BottomRight() = br;
//	MoveWindow(100,100,800,600,TRUE);
//	GetClientRect(rect);

}
void
CPlayerDlg::SaveWindowPos() {
    CRect dialog;
    GetWindowRect(dialog);
    CPoint tl = dialog.TopLeft();
    CPoint br = dialog.BottomRight();

    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
    reg.Write(RegWindowMaximized, int(0));
//    WINDOWPLACEMENT lpwndpl;
//    BOOL s = GetWindowPlacement(&lpwndpl );
//    if (s) {
//        if (lpwndpl.showCmd == SW_SHOWMAXIMIZED) {
//            reg.Write(RegWindowMaximized, 1);
//        }
//    }
    if (IsZoomed() || IsIconic()) {
        reg.Write(RegWindowMaximized, 1);
    }

    reg.Write(RegWindowX1, tl.x);
    reg.Write(RegWindowY1, tl.y);
    reg.Write(RegWindowX2, br.x);
    reg.Write(RegWindowY2, br.y);

}

void CPlayerDlg::OnMenuExit() 
{
    if (m_MenuDlg) {
	    m_MenuDlg->EndDialog(0);
    } else {
        saveConfig();
        EndDialog(0);
    }
}
void CPlayerDlg::OnExit() 
{
    if (m_MenuDlg) {
	    m_MenuDlg->EndDialog(0);
    } else {
        saveConfig();
    }
}
void CPlayerDlg::OnMenuEditPlaylist() {
	if (0 == m_LoadPlaylistDlg) {
		m_LoadPlaylistDlg = new LoadPlaylistDlg (&m_callbacks, this, TRUE);
		*m_Dialog = m_LoadPlaylistDlg;
		int r = m_LoadPlaylistDlg->DoModal();
		delete m_LoadPlaylistDlg;
		m_LoadPlaylistDlg = 0;
		*m_Dialog = this;
	}
}
void CPlayerDlg::OnMenuLoadplaylist() 
{
	if (0 == m_LoadPlaylistDlg) {
		m_LoadPlaylistDlg = new LoadPlaylistDlg (&m_callbacks, this, FALSE);
		*m_Dialog = m_LoadPlaylistDlg;
		int r = m_LoadPlaylistDlg->DoModal();
		delete m_LoadPlaylistDlg;
		m_LoadPlaylistDlg = 0;
		*m_Dialog = this;
	}
}

void CPlayerDlg::OnMenuSaveplaylist() 
{

    if (m_Playlist.GetCount() < 1) {
        PlayerStatusTempSet("No Playlist to save!");
        return;
    }
    if (m_SavePlaylistFlag == TRUE) {
	    SavePlaylistDlg *dialog;

	    dialog = new SavePlaylistDlg(this);

	    int ret;
	    ret = dialog->DoModal();
	    if (ret == IDOK) {
            // a file was selected
    //        CString file;
    //        dialog->GetWindowText(file);
    //		m_mlib.savePlaylist(file);
	    }

	    delete dialog;
    } else {
        AutoBuf buf(1000);
        CString name;
        CTime t = CTime::GetCurrentTime();
        name = t.Format("%y%m%d-%H%M%S");
        SavePlaylist(name);
    }
    m_SavePlaylistFlag = TRUE;

	
}

void CPlayerDlg::SavePlaylist(CString name) {
	if (!::IsWindow(m_Playlist.m_hWnd)) {
		return;
	}
	if (m_Playlist.GetCount() == 0) {
		return;
	}
	m_mlib.savePlaylist(name);
}

void CPlayerDlg::OnMenuShuffleplaylist() 
{
	m_mlib.shufflePlaylist();
    m_Playlist.ResetContent();
	m_mlib.getPlaylist(m_Playlist);
	m_Playlist.SetCurSel(0);
	UpdateWindow();
	SavePlaylist("Last");
    Stop();
    m_PlaylistCurrent = -1;
	PlayLoop();	
}

void CPlayerDlg::OnMenuRandomizePlaylist() 
{
	CWaitCursor cw;
	if (m_mlib._playlist.size()) {
		m_mlib.RandomizePlaylist();

	} else {
		m_mlib.getRandomPlaylist(_selectedGenre);
	}
	m_Playlist.ResetContent();
	m_mlib.getPlaylist(m_Playlist);
	m_Playlist.SetCurSel(0);
	UpdateWindow();
    Stop();
    m_PlaylistCurrent = -1;
	SavePlaylist("Last");
	PlayLoop();	
	calcDuration();
}

void CPlayerDlg::OnMenuPause() 
{
	Pause();
	
}

void CPlayerDlg::OnMenuPlay() 
{
    OnPlayButton();
	
}

void CPlayerDlg::OnMenuStop() 
{
    OnStopButton();
	
}

void CPlayerDlg::OnMenuHelp() 
{
	CString msg,d1,d2,d3,r1,r2,r3;
	int i,n;
	BOOL AnyDefined = FALSE;
	msg = "Use Up,Down,Left,Right to navigate,\r\nSelect to queue up music.\r\n\r\n";
	RemoteReceiver * rrcvr = RemoteReceiver::rrcvr();	
	if (rrcvr) {
		n = rrcvr->numOfKeys();
		AutoBuf buf(200);
		const char * HFORMAT = "%s\t%s\t| %s\t%s\r\n";
		sprintf(buf.p, HFORMAT,
			    "Function","On Remote", "Function", "On Remote");
		msg += buf.p;
		msg += "_______________________________________\r\n";
		int first = 1;

		for(i = 0 ; i < n ; i += 2) {
			rrcvr->getDescs(i, d1, r1);
			rrcvr->getDescs(i+1, d2, r2);
			if (r1.GetLength() || r2.GetLength())
				AnyDefined = TRUE;
			sprintf(buf.p, HFORMAT,
				d1,r1,d2,r2);
			msg += buf.p;
		}
	}
	if (!AnyDefined) 
		msg = "No Remote Control descriptions defined. See Settings/Remote.";
	MBMessageBox("Remote Control Help", msg, FALSE, FALSE,60);
}

void CPlayerDlg::OnNextSong() {
    Stop();
        m_StopFlag = FALSE;
		m_PauseFlag = FALSE;
	PlayLoop();
}
void CPlayerDlg::OnPreviousSong() {
    if (m_PlaylistCurrent <= 0) {
        m_PlaylistCurrent = m_Playlist.GetCount() -2;
    } else {
        m_PlaylistCurrent -= 2;
    }
        m_StopFlag = FALSE;
		m_PauseFlag = FALSE;
    OnNextSong();
}
BOOL CPlayerDlg::Play() {
	StartSeekTimer();
	adjustVolume() ;
	BOOL r = m_Player->Play();
	m_PositionLabel.setText ( "");
	return r;

}
void CPlayerDlg::Stop() {
	StopSeekTimer();
	resetPosition();
	m_Player->Stop();
	m_PositionLabel.setText ( "Stop", DT_CENTER);
//	CurrentTitleSet("");
}
void CPlayerDlg::Pause() {
	if (m_PauseFlag == FALSE) {
		m_PauseFlag = TRUE;
		StopSeekTimer();
	} else {
		m_PauseFlag = FALSE;
		StartSeekTimer();
	}
	adjustVolume() ;
	m_Player->Pause();
	m_StopFlag = FALSE;
	m_PositionLabel.setText ( "Pause", DT_CENTER);
}
void CPlayerDlg::OnPlayButton() 
{
	if (m_Config.trialMode() == 1) {
		--m_PlaylistCurrent;
		PlayLoop();
		return;
	} else if (m_Config.trialMode() == 2) {
		return;
	}
	if (m_Player->isPlaying() == FALSE) {
        Play();
        m_StopFlag = FALSE;
		m_PauseFlag = FALSE;
	}
}

void CPlayerDlg::OnStopButton() 
{
	Stop();
    m_StopFlag = TRUE;
	m_PauseFlag = FALSE;
	//PlayerStatusClear();
}

void
CPlayerDlg::recordTLEN() {
	long pos_secs, dur_secs, pos_pct;
	m_Player->ReadMediaPosition(pos_secs, dur_secs, pos_pct);
	CString tlen = m_mlib._playlist[m_PlaylistCurrent]->getId3("TLEN");
	if (tlen == "") {
		int duration = dur_secs * 1000;
		tlen = numToString(duration);
		m_mlib.setSongVal("TLEN", tlen, 
			m_mlib._playlist[m_PlaylistCurrent]->getId3("TCON"),
			m_mlib._playlist[m_PlaylistCurrent]->getId3("TPE1"),
			m_mlib._playlist[m_PlaylistCurrent]->getId3("TALB"),
			m_mlib._playlist[m_PlaylistCurrent]->getId3("TIT2"));
	}
}

void CPlayerDlg::PlayLoop() {
	AutoLog alog("PlayLoop()##################################################");
    static int first = 1;
	static int trialCounter = 0;
    int good = 0;
    do {
        if (m_Player->isStopped()
                && m_mlib._playlist.size() && m_StopFlag == FALSE) {
            if (first) {
                first = 0;
            } else {
				CurrentTitleSet("");
				// No longer needed, WmaTag guarantees it's in the db.
				//recordTLEN();
				resetPosition();
				InputClose();
// this prevents repeat!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// this prevents repeat!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// this prevents repeat!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// this prevents repeat!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//				return;
            }
            m_PlaylistCurrent++;
            if (m_PlaylistCurrent+1 > m_mlib._playlist.size() 
					|| m_PlaylistCurrent < 0) {
                m_PlaylistCurrent = 0;
            }
			if (m_Config.trialMode() == 1) {
				++m_trialCounter;
				if (m_trialCounter > 3) {
					m_trialCounter = 0;
					PlayerStatusSet("Trial Mode Stopped. Press Play to continue.");
					return;
				}
			} else if (m_Config.trialMode() == 2) {
				PlayerStatusSet("Trial Expired.");
				return;
			}
            m_Playlist.SetCurSel(m_PlaylistCurrent);
			m_Playlist.centerCurSel();
			m_Playlist.invalidate();
            CString file = m_mlib._playlist[m_PlaylistCurrent]->getId3("FILE");

			if (FileUtil::IsReadable(file)) {
				good = 1;
			} else {
				good = 0;
			}
        
			CString msg;
            if (good && m_Player->InputOpen(file) && Play()) {
//				Play();
//				adjustVolume();
				good = 1;
				displayAlbumArt(file,m_mlib._playlist[m_PlaylistCurrent]->getId3("TALB"));
			} else {
				good = 0;
				if (!FileUtil::IsReadable(file)) {
					msg = "File is unreadable ";
					msg += file;
				} else {
					msg = "Unable to play ";
					msg += file;
				}
				CurrentTitleSet(msg);
				UpdateWindow();
				logger.log(msg);
				if (!m_CheckingEm) {
					m_PlayLoopTimerId = SetTimer(MB_PLAYLOOP_TIMER_ID, 1000, NULL);
					break;
				}
			}
			if (m_CheckingEm) {
				if (good) {
					m_CheckGoodCount++;
				} else {
					m_CheckBadCount++;
					CheckEmLog.log("Unable to play:"+file);
				}
				m_Playlist.ResetContent();
				//m_mlib.getPlaylist(m_Playlist);
				CString msg = "Checking:" +numToString(1+m_CheckSongCounter)
					+"/"+numToString(m_CheckSongCount);
				m_Playlist.AddString(msg);
				msg = "Good:" + numToString(m_CheckGoodCount);
				m_Playlist.AddString(msg);
				msg = "Bad:" + numToString(m_CheckBadCount);
				m_Playlist.AddString(msg);
				msg = file;
				m_Playlist.AddString(msg);
				m_Playlist.invalidate();
				m_CheckSongCounter++;
				if (m_CheckSongCounter < m_CheckSongCount) {
					CString file = m_mlib.getSongFileName(m_CheckSongCounter);
					PlayerStatusSet(file);
					m_mlib._playlist.reset();
					m_PlaylistCurrent = -1;
					Stop();
					InputClose();
					m_mlib.addFileToPlaylist(file);
					m_PlayLoopTimerId = SetTimer(MB_PLAYLOOP_TIMER_ID, 1000, NULL);
					//good = 0;
				} else {
					m_mlib._playlist.reset();
					m_PlaylistCurrent = -1;
					Stop();
					InputClose();
					PlayerStatusSet("Check Em done, see "+CheckEmLog._pathfile);
					m_CheckingEm = FALSE;
					CString msg = 
						"Total:" + numToString(m_CheckSongCount) + ", "
						+ "Good:" + numToString(m_CheckGoodCount) + ", "
						+ "Bad:" + numToString(m_CheckBadCount) + " "
						+ "Check Em Done";
					CheckEmLog.log(msg);
					CheckEmLog.close();
					logger.log(msg);
					msg = FileUtil::FileToString(CheckEmLog._pathfile);
					MBMessageBox("Check Em results",msg,TRUE,FALSE);
				}
			} else if (good) {
				msg = m_mlib._playlist[m_PlaylistCurrent]->getId3("TIT2");
				msg += " by ";
				msg += m_mlib._playlist[m_PlaylistCurrent]->getId3("TPE1");
				msg += " on ";
				msg += m_mlib._playlist[m_PlaylistCurrent]->getId3("TALB");
				msg += " in ";
				msg += m_mlib._playlist[m_PlaylistCurrent]->getId3("TCON");

				CString comments = m_mlib.getComments(file);
				if (comments.GetLength()) {
					msg += ". ";
					msg += comments;
				}
				CurrentTitleSet(msg);
				if (m_Config.trialMode() == 1) {
					PlayerStatusTempSet("Trial Mode. See Settings/License.");
				}
				UpdateWindow();
			}
        } else {
			if (m_Player->isStopped() && m_QuickPlay) {
				m_QuickPlay = FALSE;
				CurrentTitleSet("");
				resetPosition();
				InputClose();
			}
            good = 1;
        }
    } while (!good);
}

void
CPlayerDlg::calcDuration() {
	int size = m_mlib._playlist.size() ;
	int i;
	int totduration = 0;
	int duration;
	for(i = 0 ; i < size ; ++i) {
		duration = 0;
		CString sDuration = m_mlib._playlist[i]->getId3("TLEN");
		logger.ods("Duration "+sDuration);
		if (sDuration != "") {
			// TLEN is in milliseconds but we're counting up seconds
			int millis = atoi((LPCTSTR)sDuration);
			if (millis > 999)
				duration = millis / 1000;
			else duration = millis;
		}
		totduration += duration;
	}
	m_PlaylistDuration = totduration;
	CString msg = "Playlist";
	if (size) {
		msg += ": ";
		AutoBuf buf(100);
		int h,m,s;
		MBUtil::SecsToHMS(totduration,h,m,s,buf.p);
		msg += numToString(size);
		msg += ", ";
		msg += buf.p;
	}
	m_PlaylistLabel.setText(msg);
}

void CPlayerDlg::LoadPlaylist(CString name) {
    m_Playlist.ResetContent();
    CString errormsg;
    int r = m_mlib.loadPlaylist(name,errormsg);
	{
		CWaitCursor c;
		m_mlib.getPlaylist(m_Playlist);
	}
	m_Playlist.SetCurSel(m_Playlist.GetCount()-1);
	
	UpdateWindow();
    if (r != 0) {
        errormsg = "Muzikbrowzer encountered problems with your playlist\r\n\r\n" + errormsg;
        MBMessageBox("alert", errormsg);
    }
    PlayLoop();
	calcDuration();
}

void CPlayerDlg::OnContextMenu(CWnd* pWnd, CPoint ScreenPnt) 
{


    CPoint  point; // point inside the list ctrl where right-click occurred
    int  nIndex = 0; // index of the client that was right-clicked
    UINT uFlags = 0;

    // make sure window is active
// GetParentFrame()->ActivateFrame();

 // convert the point to the list control coordinates
    point = ScreenPnt;
    
//    m_Genres.ScreenToClient (&ClntPnt);

    CRect genreRect, artistRect, albumRect, songRect, playlistRect;
	m_Genres.GetWindowRect(genreRect);
    m_Artists.GetWindowRect(artistRect);
    m_Albums.GetWindowRect(albumRect);
    m_Songs.GetWindowRect(songRect);
    m_Playlist.GetWindowRect(playlistRect);

    mWindowFlag = 0;

    if (genreRect.PtInRect(point)) {
        m_Genres.SetFocus();
		OnGenresFocus( ); // added this to update selection upon right click
        int sel = m_Genres.GetSelectedItemFromPoint(point);
        if (sel >= 0) {
            m_Genres.SetCurSel(sel);
            OnSelchangeGenres();
            mWindowFlag = 1;
        }
    } else if (artistRect.PtInRect(point)) {
        m_Artists.SetFocus();
		OnArtistsFocus( );
        int sel = m_Artists.GetSelectedItemFromPoint(point);
        if (sel >= 0) {
            m_Artists.SetCurSel(sel);
            OnSelchangeArtists();
            mWindowFlag = 2;
        }
    } else if (albumRect.PtInRect(point)) {
        m_Albums.SetFocus();
		OnAlbumsFocus( );
        int sel = m_Albums.GetSelectedItemFromPoint(point);
        if (sel >= 0) {
            m_Albums.SetCurSel(sel);
            OnSelchangeAlbums();
            mWindowFlag = 3;
        }
    } else if (songRect.PtInRect(point)) {
        m_Songs.SetFocus();
		OnSongsFocus( );
        int sel = m_Songs.GetSelectedItemFromPoint(point);
        if (sel >= 0) {
			m_Songs.SetCurSel(sel);
			OnSelchangeSongs();
	        mWindowFlag = 4;
		}
    } else if (playlistRect.PtInRect(point)) {
        m_Playlist.SetFocus();
		OnPlaylistFocus( );
        int sel = m_Playlist.GetSelectedItemFromPoint(point);
        if (sel >= 0) m_Playlist.SetCurSel(sel);
        mWindowFlag = 5;
    }

    if (mWindowFlag) {
        CMenu menu;
        if (menu.LoadMenu(IDR_MENU1)) {
            CMenu * popup;
            popup = menu.GetSubMenu(1);
            ASSERT(popup != NULL);
			if (4 == mWindowFlag || 5 == mWindowFlag) {
				popup->EnableMenuItem(ID_QUICK_PLAY, 
					MF_ENABLED | MF_BYCOMMAND);
			} else {
				popup->EnableMenuItem(ID_QUICK_PLAY, 
					MF_DISABLED |MF_GRAYED| MF_BYCOMMAND);
			}
			if (1 <= mWindowFlag && mWindowFlag <= 4) {
				popup->EnableMenuItem(ID_ADDTO_PLAYLIST, 
					MF_ENABLED | MF_BYCOMMAND);
				popup->EnableMenuItem(ID_DELETE_FROM_LIBRARY, 
					MF_ENABLED | MF_BYCOMMAND);
				popup->EnableMenuItem(ID_DELETE_FROM_PL, 
					MF_DISABLED |MF_GRAYED| MF_BYCOMMAND);
			} else {
				popup->EnableMenuItem(ID_ADDTO_PLAYLIST, 
					MF_DISABLED |MF_GRAYED| MF_BYCOMMAND);
				popup->EnableMenuItem(ID_DELETE_FROM_PL, 
					MF_ENABLED | MF_BYCOMMAND);
				popup->EnableMenuItem(ID_EDIT_ID3TAG, 
					MF_DISABLED |MF_GRAYED| MF_BYCOMMAND);
				popup->EnableMenuItem(ID_DELETE_FROM_LIBRARY, 
					MF_DISABLED |MF_GRAYED| MF_BYCOMMAND);
			}
            popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,ScreenPnt.x,
                ScreenPnt.y, AfxGetMainWnd());
        }
    }
}

void CPlayerDlg::OnUserEditSong() 
{
	if (m_Config.trialMode() == 2) {
		MBMessageBox("Notice", "Trial Expired.");
		return;
	}
	ModifyIDThree *dialog = NULL;
    CString genre,artist,album,title,year,track;
    CStringArray genreList;
    m_mlib.getGenres(genreList);

	if (mWindowFlag > 4) {
		PlayerStatusTempSet("not allowed");
		return;
	}
	Song song = new CSong;
    if (mWindowFlag == 4) {
	    int sel = m_Songs.GetCurSel();
		DWORD data = m_Songs.GetItemData(sel);
		song = m_mlib.m_SongLib.getSong(data);

		CString filename = song->getId3("FILE");
		if (!FileUtil::IsReadable(filename)) {
			CString msg = "unable to open ";
			msg += filename;
			MBMessageBox(CString("Error"), msg);
			return;
		}
    } else {
        if (mWindowFlag >= 3) {
			song->setId3("TALB", _selectedAlbum);
	        int sel = m_Songs.GetCurSel();
	        CString songname;
	        m_Songs.GetText(sel, songname);
			Song tmp = m_mlib.getSong(_selectedGenre, _selectedArtist,
				_selectedAlbum, songname);
			song->setId3("TYER", tmp->getId3("TYER"));
        }
        if (mWindowFlag >= 2) {
			song->setId3(CS("TPE1"), _selectedArtist);
        }
		song->setId3(CS("TCON"), _selectedGenre);
    }
	if (!m_ModifyDelete && mWindowFlag  < 3 && MBALL == _selectedGenre && MBALL == _selectedArtist) {
		PlayerStatusTempSet("not allowed");
		return;
	}
	int ret=IDOK;
	if (!m_ModifyDelete) {
		dialog = new ModifyIDThree(&genreList, song, mWindowFlag);
		ret = dialog->DoModal();
	}

	if (ret == IDOK) {
		DBLOCKED = TRUE;
		logger.ods("Deleting, DBLOCKED=TRUE");
		OnSearchClear();
		CStringArray deletes;
		CWaitCursor c;
		if (m_ModifyDelete) {
			ret = m_mlib.preDeleteSong(song,deletes);
		} else {
			ret = m_mlib.preModifyID3(song, dialog->m_newSong);
		}
		if (ret) {
			CWaitCursor c;
			logger.ods("pre initDb");
			initDb();
			logger.ods("post initDb");
//			DBLOCKED = FALSE;
//			_selectedGenre = "";
			OnSelchangeGenres();
			logger.ods("post 0");
			updatePlaylistPostEdit();
			logger.ods("post 1");
			if (m_ModifyDelete) {
				m_Config.AddDeletedFiles(deletes);
			}
			logger.ods("post 2");

		}
		DBLOCKED = FALSE;
		logger.ods("Deleting, DBLOCKED=FALSE");
	}

	if (dialog)
		delete dialog;    	

}

void CPlayerDlg::OnGetMinMaxInfo( MINMAXINFO FAR* lpMMI ) {
   RECT r;
   GetWindowRect(&r);
   if ((r.right - r.left) > 0 && TRUE == m_FixedSize )
   {
      lpMMI->ptMaxTrackSize.x = r.right - r.left;
      lpMMI->ptMinTrackSize.x = r.right - r.left;
      lpMMI->ptMaxTrackSize.y = r.bottom - r.top;
      lpMMI->ptMinTrackSize.y = r.bottom - r.top;
   }
   else
      CDialog::OnGetMinMaxInfo(lpMMI); 
}

void CPlayerDlg::OnSizing(UINT fwSide, LPRECT pRect) 
{
	int left,right,top,bottom;
	left = pRect->left;
	right = pRect->right;
	top = pRect->top;
	bottom = pRect->bottom;

	if ((fwSide == 7 || fwSide == 1 || fwSide == 4)
		&& (left > right - MB_MIN_WIDTH))
			left = right - MB_MIN_WIDTH;

	if ((fwSide == 4 || fwSide == 3 || fwSide == 5)
		&& (top > bottom - MB_MIN_HEIGHT))
			top = bottom - MB_MIN_HEIGHT;

	if ((fwSide == 5 || fwSide == 2 || fwSide == 8)
		&& (right < left + MB_MIN_WIDTH))
		right = left + MB_MIN_WIDTH;
	
	if ((fwSide == 7 || fwSide == 6 || fwSide == 8)
		&& (bottom < top + MB_MIN_HEIGHT))
		bottom = top + MB_MIN_HEIGHT;

	pRect->left = left;
	pRect->right = right;
	pRect->top = top;
	pRect->bottom = bottom;

	if (FALSE == m_Resizing) {
		CDialogClassImpl::OnSizing(fwSide, pRect);
	}
	m_Resizing = TRUE;	
//	logger.ods("OnSizing");
}
void CPlayerDlg::OnCaptureChanged(CWnd *pWnd) {
	// This forces rubber band style resizing
//	logger.ods("OnCaptureChanged");
	if (m_Resizing)
	{
		m_Resizing = FALSE;

		if (FALSE == m_FixedSize ) {
			CRect wr;
			GetWindowRect(wr);
			if (wr.Size() != m_WindowRect.Size())
				resetControls();
		} else {
			PlayerStatusTempSet("This skin is not resizeable");
		}
	}
	
	CDialog::OnCaptureChanged(pWnd);
}

void CPlayerDlg::OnSize(UINT nType, int cx, int cy) 
{
    static int first = 0;
    if (first < 2) {
        first++;
        return;
    }
//	logger.ods(CS("OnSize ") + numToString(nType));
//	logger.ods("last x,y:"+numToString(m_LastSized.x)+","+numToString(m_LastSized.y)+" x,y:"
//		+numToString(cx)+","+numToString(cy));


	if (!m_Resizing) {
		if (SIZE_MAXIMIZED == nType && TRUE == m_FixedSize) {
			PlayerStatusTempSet("This skin is not resizeable");
		} else {
			CDialogClassImpl::OnSize(nType, cx, cy);
			switch(nType) {
			case SIZE_RESTORED:
//				switch(m_LastSized) {
//				case SIZE_MAXIMIZED:
//					resetControls();
//					break;
//				}
				if (m_LastSized.x != cx || m_LastSized.y != cy)
					resetControls();
				break;
			case SIZE_MINIMIZED:
				break;
			case SIZE_MAXIMIZED:
			case SIZE_MAXSHOW:
			case SIZE_MAXHIDE:
				resetControls();
				break;
			}
		}
	}
	if (SIZE_MAXIMIZED == nType) {
		m_ButtonMaximize.ShowWindow(SW_HIDE);
		m_ButtonRestore.ShowWindow(SW_NORMAL);
	} else {
		m_ButtonMaximize.ShowWindow(SW_NORMAL);
		m_ButtonRestore.ShowWindow(SW_HIDE);
	}
	if (SIZE_MINIMIZED != nType) {
		m_LastSized.x = cx;
		m_LastSized.y = cy;
	}
}
void
CPlayerDlg::CurrentTitleSet(LPCTSTR lpmsg) {
	CString msg(lpmsg);
	CurrentTitleSet(msg);
}
void
CPlayerDlg::CurrentTitleSet(CString & msg) {
	//m_CurrentTitle.setText(msg);
	m_CurrentTitleDesc = msg;
	PlayerStatusSet(msg);

}
void
CPlayerDlg::PlayerStatusClear() {
	PlayerStatusSet("");
}
void
CPlayerDlg::SetSelected(const CString text, DWORD itemData) {
	CString msg(text);
	if (itemData && MBALL == _selectedArtist) {
		Song song = new CSong;
		song = m_mlib.m_SongLib.getSong(itemData);
		msg += " by ";
		msg += song->getId3("TPE1");
		msg += " in ";
		msg += song->getId3("TCON");
	}
	PlayerStatusTempSet(msg);
}
void
CPlayerDlg::PlayerStatusSet(LPCTSTR lpmsg) {
	CString msg(lpmsg);
	PlayerStatusSet(msg);
}

void
CPlayerDlg::PlayerStatusSet(CString & msg) {
	if (m_StatusTimerId == 0) {
		m_PlayerStatus.setText(msg);
	}
    m_PlayerStatusSave = msg;
	PlayerStatusTempSet(msg);
}
void
CPlayerDlg::PlayerStatusTempSet(LPCTSTR lpmsg) {
	CString msg(lpmsg);
	PlayerStatusTempSet(msg);
}
void
CPlayerDlg::PlayerStatusTempSet(CString & cmsg) {
	CString msg(cmsg);
//	if (MBALL == msg)
//		msg = m_mlib.getLibraryCounts();		
	m_PlayerStatus.setText(msg/*,TRUE*/);
    m_PlayerStatusTime = CTime::GetCurrentTime();
	StartStatusTimer();
//	m_PositionLabel.setText ( "" );
}
void
CPlayerDlg::PlayerStatusRevert() {

    CTime t = CTime::GetCurrentTime();
    if (t.GetTime() > (m_PlayerStatusTime.GetTime() + PLAYER_STATUS_DELAY)) {
		if (m_PlayerStatusSave.GetLength()) {
			m_PlayerStatus.setText(m_PlayerStatusSave);
		}
		StopStatusTimer();
	}
	return;
	{
        CString current;
        m_PlayerStatus.GetWindowText(current);
        if (current == m_PlayerStatusSave) {
			m_PlayerStatus.setText(m_HelpMsg);
		} else if (current == m_HelpMsg) {
            m_PlayerStatus.setText(m_PlayerStatusSave);
        } else {
			m_PlayerStatus.setText(m_PlayerStatusSave);
		}
        m_PlayerStatusTime = CTime::GetCurrentTime();
    }
}
LRESULT
CPlayerDlg::OnMovePlaylistUp(WPARAM wParam, LPARAM lParam) {
	movePlaylistUp(wParam);
	return 0;
}
LRESULT
CPlayerDlg::OnMovePlaylistDn(WPARAM wParam, LPARAM lParam) {
	movePlaylistUp(wParam);
	return 0;
}
void
CPlayerDlg::movePlaylistUp(int sel) {
    m_mlib.movePlaylistUp(m_PlaylistCurrent, sel);
    if (m_PlaylistCurrent == sel) {
        m_PlaylistCurrent--;
    } else if (m_PlaylistCurrent == (sel -1)) {
        m_PlaylistCurrent++;
    }
}
void
CPlayerDlg::movePlaylistDown(int sel) {
    m_mlib.movePlaylistDown(m_PlaylistCurrent, sel);
    if (m_PlaylistCurrent == sel) {
        m_PlaylistCurrent++;
    } else if (m_PlaylistCurrent == (sel +1)) {
        m_PlaylistCurrent--;
    }
}

UINT
CPlayerDlg::OnNcHitTest( CPoint point ) {

    CRect r;//,applabelr; //,positionr,volumer;
	GetClientRect(&r);
	CRect br = r;
	br.TopLeft() = CPoint(br.BottomRight().x-20, br.BottomRight().y-20);
	CPoint cpoint(point);
	ScreenToClient(&cpoint);
	
	ClientToScreen(&r);
	ClientToScreen(&br);

	UINT rv = CDialogClassImpl::OnNcHitTest(point);
	
    if(br.PtInRect(point)) {
		return HTBOTTOMRIGHT;
	} else if (m_GenreArtistAdjust.PtInRect(cpoint)) {
		return HTSIZE;
	} else if (m_ArtistAlbumAdjust.PtInRect(cpoint)) {
		return HTSIZE;
	} else if (m_AlbumSongAdjust.PtInRect(cpoint)) {
		return HTSIZE;
    } else if (r.PtInRect(point)) {
		return HTCAPTION;
    } 

	return rv;
}
BOOL CPlayerDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTSIZE) {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
	} else {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return CDialogClassImpl::OnSetCursor(pWnd, nHitTest, message);
	}
	return TRUE;
}

void CPlayerDlg::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CPoint cpoint(point);
	ScreenToClient(&cpoint);
	m_AdjustLibrary = 0;
	m_LibDrag = FALSE;
	if (m_GenreArtistAdjust.PtInRect(cpoint)) {
		m_AdjustLibrary = 1;
	} else if (m_ArtistAlbumAdjust.PtInRect(cpoint)) {
		m_AdjustLibrary = 2;
	} else if (m_AlbumSongAdjust.PtInRect(cpoint)) {
		m_AdjustLibrary = 3;
	}
	if (m_AdjustLibrary) {
		m_LibDrag = TRUE;
		m_LibDragRect = CRect(0,0,0,0);
		SetCapture();
	} else {
		CDialogClassImpl::OnNcLButtonDown(nHitTest, point);
	}
}
void CPlayerDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	OnNcLButtonUp(nFlags,point);
}

void CPlayerDlg::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	if (m_LibDrag) {
		ReleaseCapture();
		m_LibDrag = FALSE;
		m_LibDragRect = CRect(0,0,0,0);
		if (m_LibDragPoint == CPoint(0,0))
			m_LibDragPoint = point;
		AdjustLibraryWidths(point);
		
	} else {
		CDialogClassImpl::OnNcLButtonUp(nHitTest, point);
	}
	m_AdjustLibrary = 0;
	
}
void CPlayerDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	OnNcMouseMove(nFlags,point);
}


void CPlayerDlg::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	static const SIZE rbs = {1,1};
	
	if (m_LibDrag) {

		CPoint cpoint(point);
		ScreenToClient(&cpoint);
		CRect rect,lrect;
		RedrawWindow();
		int hardright = 0;
		int hardleft = 0;

		BOOL doit = TRUE;
		switch(m_AdjustLibrary) {
		case 1: {
				rect = m_GenresLabelInt;
				rect.right = cpoint.x;
				if (rect.right == m_GenresLabelInt.right)
					rect.right--;
				} 
				hardleft = m_GenresLabelRect.left + 50;
				hardright = m_ArtistsLabelRect.right-50;
				lrect = m_GenresLabelRect;
			break;
		case 2: {
				rect = m_ArtistsLabelInt;
				rect.right = cpoint.x;
				if (rect.right == m_ArtistsLabelInt.right)
					rect.right--;
				} 
				hardleft = m_ArtistsLabelRect.left + 50;
				hardright = m_AlbumsLabelRect.right-50;
				lrect = m_ArtistsLabelRect;
				break;
		case 3: {
				rect = m_AlbumsLabelInt;
				rect.right = cpoint.x;
				if (rect.right == m_AlbumsLabelInt.right)
					rect.right--;
				} 
				hardleft = m_AlbumsLabelRect.left + 50;
				hardright = m_SongsLabelRect.right - 50;
				lrect = m_AlbumsLabelRect;
				break;
		default: {
				doit = FALSE;
				 }break;
		}
		if (rect.right < hardleft)
			rect.right = hardleft;
		if (rect.right > hardright) {
			rect.right = hardright;
		}
		if (rect.right > m_LibraryRect.right-50)
			doit = FALSE;
		if (doit) {
			CDC * cdc = GetWindowDC();

			rect.top += 4;
			rect.left = rect.right - 1;
			cdc->DrawDragRect(rect,rbs,m_LibDragRect,rbs);
			ReleaseDC(cdc);
			m_LibDragRect = rect;
			m_LibDragPoint = rect.BottomRight();

			double x = ((double)(rect.right - lrect.left) / (double)m_LibRemainingWidth)*100;
			PlayerStatusTempSet(String::substring(numToString(x),0,5) + "%");

		}
	} else {
		CDialogClassImpl::OnNcMouseMove(nHitTest, point);
	}
}

void
CPlayerDlg::AdjustLibraryWidths(CPoint &apoint) {
	Control * p1,*p2, *p3, *p4 ;
	CPoint point = m_LibDragPoint;

	int sw,dx;
	int bpanel = m_reg.Read("BorderPanel",5);

	int dw = m_BtnControls.dialogrect.Width() 
		- (((bpanel + m_Config.getDlgBorderWidth())* 2) + (m_Config.getDlgBorderHorz()*3));
	double pctGenre,pctArtist,pctAlbum;
	p1 = m_BtnControls.getObj(IDC_GENRES);
	p2 = m_BtnControls.getObj(IDC_ARTISTS);
	p3 = m_BtnControls.getObj(IDC_ALBUMS);
	p4 = m_BtnControls.getObj(IDC_SONGS);
	int tw = p1->width + p2->width + p3->width + p4->width;

	int neww =0;

	switch(m_AdjustLibrary) {
		case 1: 
			sw = p1->width;
			p1->width = point.x - p1->x;
			if (p1->width < 50) p1->width = 50;
			if (p1->width + p1->x > p2->x + p2->width)
				p1->width = ((p2->x + p2->width) - 50) - p1->x;

			dx = sw - p1->width;
			m_BtnControls.move(p1,p1->x,p1->y,p1->row,p1->col);

			pctGenre = ((double)p1->width / (double)dw);
			m_Config.setRegistry("LibGenresWidth", numToString(pctGenre));

			neww = tw - (p1->width + p3->width + p4->width);
			p2->width = neww;
			p2->x -= neww;
			m_BtnControls.move(p2,p2->x,p2->y,p2->row,p2->col);
			
			pctArtist = ((double)p2->width / (double)(tw));

			m_Config.setRegistry("LibArtistsWidth", numToString(pctArtist));
			m_Genres.initBgDc();
			m_Artists.initBgDc();
			break;
		case 2:
			sw = p2->width;
			p2->width = point.x - p2->x;
			if (p2->width < 50) p2->width = 50;
			if (p2->width + p2->x > p3->x + p3->width)
				p2->width = ((p3->x + p3->width) - 50) - p2->x;

			dx = sw - p2->width;
			m_BtnControls.move(p2,p2->x,p2->y,p2->row,p2->col);

			pctArtist = ((double)p2->width / (double)dw);
			m_Config.setRegistry("LibArtistsWidth", numToString(pctArtist));

			neww = tw - (p1->width + p2->width + p4->width);
			p3->width = neww;
			p3->x -= neww;
			m_BtnControls.move(p3,p3->x,p3->y,p3->row,p3->col);
			
			pctAlbum = ((double)p3->width / (double)(tw));

			m_Config.setRegistry("LibAlbumsWidth", numToString(pctAlbum));
			m_Artists.initBgDc();
			m_Albums.initBgDc();
			break;
		case 3:
			sw = p3->width;
			p3->width = point.x - p3->x;
			if (p3->width < 50) p3->width = 50;
			if (p3->width + p3->x > p4->x + p4->width)
				p3->width = ((p4->x + p4->width) - 50) - p3->x;
			dx = sw - p3->width;
			m_BtnControls.move(p3,p3->x,p3->y,p3->row,p3->col);
			pctAlbum = ((double)p3->width / (double)dw);
			m_Config.setRegistry("LibAlbumsWidth", numToString(pctAlbum));

			neww = tw - (p1->width + p2->width + p3->width);
			p4->width = neww;
			p4->x -= neww;

			m_BtnControls.move(p4,p4->x,p4->y,p4->row,p4->col);
			m_Albums.initBgDc();
			m_Songs.initBgDc();
			break;
		default:
			break;
	}

	OnNcPaint();
//	Invalidate();
//	RedrawWindow();
	resetControls();

	UpdateRects();

}



void CPlayerDlg::OnButtonMenu() 
{
    CMenu menu;
    if (menu.LoadMenu(IDR_MENU2)) {
        CMenu * popup = menu.GetSubMenu(0);
        ASSERT(popup != NULL);
        CWnd * ctrl;
        ctrl = GetDlgItem(IDC_OPTIONS_BUTTON);
        CRect button;
        ctrl->GetWindowRect(button);

		if (TRUE == m_ShowSearchPanel) {
			popup->CheckMenuItem(ID_SEARCH, MF_CHECKED | MF_BYCOMMAND);
		} else {
			popup->CheckMenuItem(ID_SEARCH, MF_UNCHECKED | MF_BYCOMMAND);
		}

		CMenu * skinmenu = popup->GetSubMenu(3);
		ASSERT(skinmenu != NULL);
		CString currentskin = m_Config.getCurrentSkin();


		UINT n = skinmenu->GetMenuItemCount();
		for(int m = n-1; m >= 0; --m) {
			skinmenu->RemoveMenu(m, MF_BYPOSITION);
		}

		POSITION pos = m_Skins.GetHeadPosition();
		UINT i=0;
		UINT needscheck = 0;
		for (pos = m_Skins.GetHeadPosition(); pos != NULL 
				&& (MB_SKINPICS_MSGS_BEGIN + i) < MB_SKINPICS_MSGS_END; ) {
			CString skin = m_Skins.GetAt(pos);
			if (currentskin == skin) {
				needscheck = MB_SKINPICS_MSGS_BEGIN + i;
			}
			skinmenu->AppendMenu(MF_STRING, MB_SKINPICS_MSGS_BEGIN + i++, skin);
			m_Skins.GetNext(pos);
		}
		if (needscheck) {
			skinmenu->CheckMenuItem(needscheck, MF_CHECKED | MF_BYCOMMAND);
		}

//		CRect rect; GetWindowRect(rect);
//		CMenu * sizemenu = popup->GetSubMenu(5);
//		ASSERT(sizemenu != NULL);
//		if (rect.Width() == 640 && rect.Height() == 480) {
//			sizemenu->CheckMenuItem(ID_640X480, MF_CHECKED | MF_BYCOMMAND);
//		} else if (rect.Width() == 800 && rect.Height() == 600) {
//			sizemenu->CheckMenuItem(ID_800X600, MF_CHECKED | MF_BYCOMMAND);
//		} else if (rect.Width() == 1024 && rect.Height() == 768) {
//			sizemenu->CheckMenuItem(ID_1024X768, MF_CHECKED | MF_BYCOMMAND);
//		}

        popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			crbottomleft(button).x,
            crbottomleft(button).y, 
			AfxGetMainWnd());
    }
}
void
CPlayerDlg::OnSkinPic(UINT wParam) {
	int skinp = wParam - MB_SKINPICS_MSGS_BEGIN;
	CString skin;
	POSITION pos = m_Skins.GetHeadPosition();
	int i=0;
	for (pos = m_Skins.GetHeadPosition(); pos != NULL; ) {
		if (i == skinp) {
			skin = m_Skins.GetAt(pos);
			break;
		}
		if (i > skinp) {
			break;
		}
		i++;
		m_Skins.GetNext(pos);
	}
	
	if (skin != "") {
		if (m_Config.ChooseSkin(skin))
			redraw();
	}

//	return 0;
}
void
CPlayerDlg::OnMusicAdd() {
	OnSearchClear();
	CStringArray dirs,excludes;
	CString dflt;
	m_Config.GetDirs(dirs,excludes);
	if (dirs.GetSize()) {
		dflt = dirs.GetAt(dirs.GetSize()-1);
		dflt = String::upDir(dflt);
	}
	CFileAndFolder dialog(this, dflt);
	dialog.setTitle("Add music to Muzikbrowzer library");
	dialog.setMsg("Select file(s)/folder(s) to be added.");
	int ret;
	ret = dialog.DoModal();
	if (ret == IDOK) {
		CStringList list;
		CStringArray array;
		dialog.GetPaths(list);
		String::CStringList2CStringArray(array,list);
		m_Config.AddFolders(array);

		if (!m_mlib.Scan(array,excludes,FALSE,TRUE)) {
			init(); // re-read db
			m_Config.DelFolders(array);
		} else {
			initDb(); // just reinit the Player dlg
		}
		PlayerStatusClear();
		PlayerStatusSet(m_mlib.getLibraryCounts());
		UpdateWindow();
	}
}
void 
CPlayerDlg::OnMusicScan() {
	OnSearchClear();
	CStringArray dirs,excludes;
	m_Config.GetDirs(dirs,excludes);
	if (!m_mlib.Scan(dirs,excludes,FALSE,FALSE)) {
		init();
	} else {
		initDb();
	}
	PlayerStatusClear();
	PlayerStatusSet(m_mlib.getLibraryCounts());
	UpdateWindow();
}
void
CPlayerDlg::OnMusicScanNew() {
	OnSearchClear();
	CStringArray dirs,excludes;
	m_Config.GetDirs(dirs,excludes);
	if (!m_mlib.Scan(dirs,excludes,TRUE,FALSE)) {
		init();
	} else {
		initDb();
	}
	PlayerStatusClear();
	PlayerStatusSet(m_mlib.getLibraryCounts());
	UpdateWindow();
}
void CPlayerDlg::OnButtonMinimize() 
{
	ShowWindow(SW_MINIMIZE);
}

void CPlayerDlg::OnButtonMaximize() 
{
	if (TRUE != m_FixedSize) {
		WINDOWPLACEMENT lpwndpl;
		BOOL r = GetWindowPlacement(&lpwndpl);
		UINT newstate;
		m_Maximized = FALSE;
		if (lpwndpl.showCmd == SW_SHOWMAXIMIZED) {
			newstate = SW_SHOWNORMAL;
			m_ButtonMaximize.ShowWindow(SW_NORMAL);
			m_ButtonRestore.ShowWindow(SW_HIDE);
		} else if (lpwndpl.showCmd == SW_SHOWMINIMIZED
			|| lpwndpl.showCmd == SW_SHOWNORMAL) {
			newstate = SW_SHOWMAXIMIZED;
			m_Maximized = TRUE;
			m_ButtonMaximize.ShowWindow(SW_HIDE);
			m_ButtonRestore.ShowWindow(SW_NORMAL);
		} else {
			newstate = SW_SHOWNORMAL;
			m_ButtonMaximize.ShowWindow(SW_NORMAL);
			m_ButtonRestore.ShowWindow(SW_HIDE);
		}

		ShowWindow(newstate);
	} else {
		PlayerStatusTempSet("This skin is not resizeable");
	}
	
}

HBRUSH CPlayerDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor ) {

	HBRUSH hbr = CDialogClassImpl::OnCtlColor(pDC, pWnd, nCtlColor);

    COLORREF bkcolor, txcolor;
    switch (nCtlColor) {
    case CTLCOLOR_BTN: //   3 Button control
		return hbr;

        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();

        break;
    case CTLCOLOR_DLG: //   4 Dialog box
        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();
        break;
    case CTLCOLOR_EDIT: //   1 Edit control
		bkcolor = m_Config.getColorBkColHdr();
		txcolor = m_Config.getColorTxColHdr();
        break;
    case CTLCOLOR_LISTBOX: //  2 List-box control
        bkcolor = m_Config.getColorBkNormal();
        txcolor = m_Config.getColorTxPanel();
        break;
    case CTLCOLOR_MSGBOX: //   0 Message box
        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();
        break;
    case CTLCOLOR_SCROLLBAR: //  5 Scroll-bar control
        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();
        break;
    case CTLCOLOR_STATIC: //  6 Static control
#pragma hack
// Panel for status all others ColHdr
		if (pWnd == &m_PlayerStatus) {
			bkcolor = m_Config.getColorBkPanel();
			txcolor = m_Config.getColorTxPanel();
		} else if (pWnd == &m_SearchStatus) {
			bkcolor = m_TransPanel;
			txcolor = m_Config.getColorTxPanel();
		} else {
			bkcolor = m_Config.getColorBkColHdr();
			txcolor = m_Config.getColorTxColHdr();
		}
		break;
    default:
        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();
        break;
    }

    pDC->SetBkColor(bkcolor);
    pDC->SetTextColor(txcolor);

	if (m_CtlColors.Lookup(numToString(bkcolor),(void *&)hbr) == 0) {
		LOGBRUSH brush;
		brush.lbColor = bkcolor;
		brush.lbHatch = 0;
		brush.lbStyle = 0;
		hbr = CreateBrushIndirect(&brush);
		m_CtlColors.SetAt(numToString(bkcolor),(void*)hbr);
	}

    return (HBRUSH)hbr;


}


void CPlayerDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) {

    switch (nIDCtl)	{
//    case IDC_BUTTON_MINIMIZE:
//		break;
//    case IDC_BUTTON_MAXIMIZE:
//    case IDC_BUTTON_EXIT: 
    case IDC_OPTIONS_BUTTON: {
        CBrush br;
        CString sBtnText;
        CWnd * c = GetDlgItem(nIDCtl);

        c->GetWindowText(sBtnText);
        CDC * cdc = c->GetDC();

        br.CreateSolidBrush(config().getColorBkPanel());

        cdc->SetTextColor(config().getColorTxPanel());
        cdc->SetBkColor(m_Config.getColorBkPanel());
        cdc->FillRect(&lpDrawItemStruct->rcItem, &br);
        cdc->DrawText(sBtnText, &lpDrawItemStruct->rcItem, 
            DT_SINGLELINE | DT_VCENTER | DT_CENTER);
        br.DeleteObject();
        }
        break;
		//    case IDC_BUTTON_MINIMIZE:
		//    case IDC_BUTTON_MAXIMIZE:
		//    case IDC_BUTTON_EXIT: 
		//		{
		//        CBrush br;
		//        CString sBtnText;
		//        CWnd * c = GetDlgItem(nIDCtl);
		//        c->GetWindowText(sBtnText);
		//        CDC * cdc = c->GetDC();
		//        br.CreateSolidBrush(config().getColorBkPanel());
		//        //cdc->SetBkMode(lpDrawItemStruct->hDC, TRANSPARENT);
		//        cdc->SetTextColor(config().getColorTxPanel());
		//        cdc->SetBkColor(m_Config.getColorBkPanel());
		//        cdc->FillRect(&lpDrawItemStruct->rcItem, &br);
		//        cdc->DrawText(sBtnText, &lpDrawItemStruct->rcItem, 
		//            DT_SINGLELINE | DT_VCENTER | DT_CENTER);
		//        br.DeleteObject();
		////        br.CreateSolidBrush(config().getColorTxPanel());
		////        cdc->FrameRect(&lpDrawItemStruct->rcItem, &br);
		//        }
        //break;
    default:
        CDialogClassImpl::OnDrawItem(nIDCtl, lpDrawItemStruct);
        break;	
    }
}
void CPlayerDlg::OnQuickPlay() {
	CString file;
	Song song = new CSong;
	int sel = -1 ;
	DWORD data = 0;
	switch (mWindowFlag) {
	case 4:
	    sel = m_Songs.GetCurSel();
		if (sel < 0) return;
		data = m_Songs.GetItemData(sel);
		song = m_mlib.m_SongLib.getSong(data);
		file = song->getId3("FILE");
		break;
	case 5:
		sel = m_Playlist.GetCurSel();
		if (sel < 0) return;
		file = m_mlib._playlist[sel]->getId3("FILE");
		song = m_mlib.createSongFromFile(file);
		break;
	}

	if (file.GetLength() < 1 || !FileUtil::IsReadable(file))
		return;

	CString msg;
	Stop();
	InputClose();
    m_Player->InputOpen(file) ;
	Play();
	adjustVolume();

    msg = song->getId3("TIT2");
	msg += " by ";
	msg += song->getId3("TPE1");
	msg += " on ";
	msg += song->getId3("TALB");
	msg += " in ";
	msg += song->getId3("TCON");
    CurrentTitleSet(msg);
	m_QuickPlay = TRUE;
	m_PlaylistCurrent--;

}
void CPlayerDlg::OnAddToPlaylist() {
	switch (mWindowFlag) {
	case 1:
		OnDblclkGenres() ;
		break;
	case 2:
		OnDblclkArtists() ;
		break;
	case 3:
		OnDblclkAlbums();
		break;
	case 4:
		OnDblclkSongs();
		break;
	}
}
void CPlayerDlg::OnTestMenu() {
//    CScrollBar *s = new CScrollBar();
//    LPSCROLLINFO lpScrollInfo;
//    UINT nMask;
//    s->GetScrollInfo(  lpScrollInfo,  nMask );
//    TestScrollBar d;
//    d.DoModal();

}

void CPlayerDlg::HelpInfo(BOOL user) {
    TCHAR szPath[_MAX_PATH],
      szFname[_MAX_FNAME],
      szDir[_MAX_DIR],
      szDrive[_MAX_DRIVE];

	if (0 == GetModuleFileName(NULL, szPath, sizeof(szPath))) {
		DWORD dwError = GetLastError();
		CString msg;
		msg << "HelpInfo: Error " << dwError;
		logger.log(msg);
		return ;
	}

	CString helppage,anchor ;

    _splitpath(szPath, szDrive, 
               szDir, szFname, NULL);
    _makepath(szPath, szDrive, szDir, 
                      MUZIKBROWZER, "chm");

	helppage = "ms-its:" + CString(szPath);
	if (user) {
		anchor = "::/Html/UserGuide.htm";
	}
	helppage += anchor;

	HINSTANCE h = ShellExecute(this->m_hWnd, "open",  "hh.exe", helppage, 
		szDir, SW_SHOW);
	if ((int)h <= 32) {
		CString msg;
		msg << "ShellExecute on help failed. ShellExecute returned "
			<< (int)h << " on " << szPath << " " << szDir;
		logger.log(msg);
	}


}
// This one's called from the Menu
void CPlayerDlg::HelpInfo() {
	HelpInfo(FALSE);
}
// While this one's called via F1
BOOL CPlayerDlg::OnHelpInfo(HELPINFO* pHelpInfo) {
	HelpInfo(TRUE);
	return TRUE;
}

void CPlayerDlg::OnMenuExportLibrary() {
	ExportDlg dlg;
	BOOL again = TRUE;
	while (again) {
		dlg.DoModal();
		again = dlg.m_Again;
	}

	if (dlg.m_Doit) {
		CWaitCursor c;
		m_mlib.preExport(&dlg);
	}
    return;
}

void CPlayerDlg::StopTimers() {
	StopSeekTimer();
	StopStatusTimer();
}
void CPlayerDlg::StartStatusTimer()
{
	StopStatusTimer();
	m_StatusTimerId = SetTimer(MB_STATUS_TIMER_ID, 3000, NULL);
}
void CPlayerDlg::StopStatusTimer()
{
	if (m_StatusTimerId) {
		KillTimer(m_StatusTimerId);
		m_StatusTimerId = 0;
	}
}
void CPlayerDlg::StartSeekTimer() 
{
    // Cancel any pending timer event
    StopSeekTimer();
	m_PositionSlider.SetRange(0,100);	
    // Create a new timer
	m_timerid = SetTimer(MB_SEEK_TIMER_ID, 1000, NULL);
	m_Player->GetFileDuration();

}

void CPlayerDlg::StopSeekTimer() 
{
    // Cancel the timer
    if(m_timerid)        
    {                
		KillTimer(m_timerid);
        m_timerid = 0;
    }
}
void CPlayerDlg::resetPosition() {
	m_PositionLabel.setText("");
	m_PositionSlider.SetPos(0);
}
void CPlayerDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent) {
	case MB_PLAYLOOP_TIMER_ID:
		KillTimer(m_PlayLoopTimerId);
		PostMessage(WM_PLAYLOOP, (WPARAM)0, (LPARAM)0);
		break;
	case MB_SEEK_TIMER_ID:
		updatePositionLabel();
		break;
	case MB_STATUS_TIMER_ID:
		PlayerStatusRevert();
		break;
	}
}
void CPlayerDlg::updatePositionLabel()
{
	long pos_secs, dur_secs, pos_pct;
	pos_secs = dur_secs = pos_pct = 0;
	m_Player->ReadMediaPosition(pos_secs, dur_secs, pos_pct);
	// handing % to SetPos works cause the Pos slider has a range of
	// 0 - 100
	m_PositionSlider.SetPos(pos_pct);
	char buf[50];
	int mins = pos_secs / 60;
	int secs = pos_secs % 60;
	int dmins,dsecs;
	dmins = dur_secs / 60;
	dsecs = dur_secs % 60;
	sprintf(buf, "%d:%02d / %d:%02d", mins, secs, dmins, dsecs);

	// This leaves the pos label blank while Temp Msg in
	// status line
	if (m_StatusTimerId == 0) {
		m_PositionLabel.setText ( buf, DT_CENTER);
	}

//	CString msg;//("Now playing ");
//	msg += buf;
//	msg += " ";
//	msg += m_CurrentTitleDesc;
//	PlayerStatusSet(msg);

}
void CPlayerDlg::adjustPosition() {
	m_Player->Seek(m_PositionSlider.GetPos(), TRUE);
	updatePositionLabel();
}
void CPlayerDlg::adjustPosition(int pct) {
	m_Player->Seek(pct, TRUE);
	updatePositionLabel();
}
LRESULT
CPlayerDlg::OnProgress(WPARAM wParam, LPARAM lParam) {
	adjustPosition();
	return 0;
}
void CPlayerDlg::OnFastForward() {
	adjustPosition(m_PositionSlider.GetPos() + 5);
}
void CPlayerDlg::OnReverse() {
	adjustPosition(m_PositionSlider.GetPos() - 5);
}

//void CPlayerDlg::updateVolumeLabel() {
//	AutoBuf buf(10);
//	sprintf(buf.p,"%d", m_VolumeSlider.GetPos());
//	m_VolumeLabel.setText(buf.p);
//}
LRESULT
CPlayerDlg::OnVolume(WPARAM wParam, LPARAM lParam) {
	adjustVolume();
	return 0;
}
void CPlayerDlg::adjustVolume() {
	if (m_Player->SetVolume(m_VolumeSlider.GetPos())) {
//		updateVolumeLabel();
	} else {
//		logger.log("Unable to set volume");
//		PlayerStatusTempSet("Unable to set volume");
	}
}
void CPlayerDlg::adjustVolume(int level) {
	m_VolumeSlider.SetPos(level);
	adjustVolume();
}
void CPlayerDlg::OnVolUp() {
	adjustVolume(m_VolumeSlider.GetPos() + 1);
}
void CPlayerDlg::OnVolDown() {
	adjustVolume(m_VolumeSlider.GetPos() - 1);
}

void CPlayerDlg::InputClose() {
	m_Player->InputClose();
	StopSeekTimer();
}

void CPlayerDlg::killAlbumArt() {
	m_Picture.unload();
}
void CPlayerDlg::displayAlbumArt(const CString & file, const CString & album) {
//	return;

	if (""== file) {
		killAlbumArt();
		m_Picture.load(m_AlbumArt);
		return;
	}

	if (file == m_LastAlbumArtFile) return;

	CString tmp = _selectedGenre + _selectedArtist + album;
	if (m_LastGAA == tmp && _selectedAlbum != MBALL)
		return;

	static BOOL first = TRUE;
	// first time up show the dflt skin art

	killAlbumArt();
	size_t size = 0;
	uchar * data = NULL;
	BOOL pic = m_mlib.apic(file, data, size,album);

	if (!first && pic && size > 50) {
		m_Picture.load(data,size);
	} else {
		first = FALSE;
		if (m_AlbumArt != "") {
			m_Picture.load(m_AlbumArt);
		} else {
			m_Picture.blank();
		}
	}
	if (data)
		delete [] data;

	m_LastAlbumArtFile = file;
	m_LastGAA = tmp;

//	RedrawWindow();

}

//void
//CPlayerDlg::OnNcPaint() {
//	CDC* pdc= GetWindowDC();
//	EraseBkgndNC(pdc);//, BitmapToCRect * bmcr)
//	ReleaseDC(pdc);	
//	return;
//}

TEST(WmaTag2, read2)
{
	CString file = "..\\testdata\\x.wma";
	WmaTag wma(file);
//	CString info = wma.getInfo();
	file = "..\\testdata\\01.ogg";
	OggTag ogg(file);
//	info = ogg.getInfo();
	file = "..\\testdata\\311AllMixedUp.mp3";
	ID3_Tag id3;
	id3.Link(file, ID3TT_ALL);
//	info = ::displayTag2(&id3, FALSE, file);
//	MBMessageBox("test",info);
}

void CPlayerDlg::OnMusicButton() {
}
void CPlayerDlg::OnPicturesButton() {
}
void CPlayerDlg::OnVideoButton() {
}
LONG myGetRegKey(HKEY key, LPCTSTR subkey, LPTSTR retdata)
{
    HKEY hkey;
    LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

    if (retval == ERROR_SUCCESS) {
        long datasize = MAX_PATH;
        TCHAR data[MAX_PATH];
        RegQueryValue(hkey, NULL, data, &datasize);
        lstrcpy(retdata,data);
        RegCloseKey(hkey);
    }

    return retval;
}
void CPlayerDlg::OnLogoButton() 
{
    TCHAR key[MAX_PATH + MAX_PATH];
	const char * url = MBURL;


    // First try ShellExecute()
    HINSTANCE result = ShellExecute(NULL, _T("open"), url, NULL,NULL, SW_SHOW);
//HKEY_CLASSES_ROOT\mailto\shell\open\command
    // If it failed, get the .htm regkey and lookup the program
    if ((UINT)result <= HINSTANCE_ERROR) {

        if (myGetRegKey(HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS) {
            lstrcat(key, _T("\\shell\\open\\command"));

            if (myGetRegKey(HKEY_CLASSES_ROOT,key,key) == ERROR_SUCCESS) {
                TCHAR *pos;
                pos = _tcsstr(key, _T("\"%1\""));
                if (pos == NULL) {                     // No quotes found
                    pos = strstr(key, _T("%1"));       // Check for %1, without quotes 
                    if (pos == NULL)                   // No parameter at all...
                        pos = key+lstrlen(key)-1;
                    else
                        *pos = '\0';                   // Remove the parameter
                }
                else
                    *pos = '\0';                       // Remove the parameter

                lstrcat(pos, _T(" "));
                lstrcat(pos, url);
                result = (HINSTANCE) WinExec(key,SW_SHOW);
            }
        }
    }

}
LRESULT
CPlayerDlg::OnHoverCancelMsg(WPARAM wParam, LPARAM lParam) {
	PlayerStatusTempSet("");
	return 0;
}

LRESULT
CPlayerDlg::OnHoverMsg1(WPARAM wParam, LPARAM lParam) {
	CString msg;
	switch(wParam) {

	case MB_HOVER_WWW_MSG:
		msg = "www.muzikbrowzer.com";break;
	case MB_HOVER_SHUFFLE_MSG:
		msg = "Shuffle playlist"; break;
	case MB_HOVER_LOAD_MSG:
		msg = "Choose playlist to play";break;
	case MB_HOVER_CLEAR_MSG:
		msg = "Clear playlist";break;
	case MB_HOVER_SAVE_MSG:
		msg = "Save playlist";break;
	case MB_HOVER_RANDOM_MSG: {
			if (m_mlib._playlist.size()) {
				msg = "Randomize playlist";
			} else {
				msg = "Add " + numToString(MB_RANDOM_PICKS) + " random songs to playlist from selected Genre";
			}
		}
		break;
	case MB_HOVER_PLAY_MSG:
		msg = "Play";break;
	case MB_HOVER_STOP_MSG:
		msg = "Stop"; break;
	case MB_HOVER_PAUSE_MSG:
		msg = "Pause"; break;
	case MB_HOVER_PREVIOUS_MSG:
		msg = "Play previous song in playlist";break;
	case MB_HOVER_NEXT_MSG:
		msg = "Play next song in playlist";break;
	case MB_HOVER_VOLUME_MSG:
		msg = "Volume"; break;
	case MB_HOVER_POSITION_MSG:
		msg = "Progress"; break;
	case MB_HOVER_MENU_MSG:
		msg = "Menu: Settings, Options, Help"; break;
	case MB_HOVER_MINIMIZE_MSG:
		msg = "Minimize"; break;
	case MB_HOVER_MAXIMIZE_MSG:
		msg = "Maximize"; break;
	case MB_HOVER_RESTORE_MSG:
		msg = "Restore"; break;
	case MB_HOVER_EXIT_MSG:
		msg = "Exit"; break;
	case MB_HOVER_SEARCH_MSG:
		msg = "Search"; break;
	case MB_HOVER_SEARCHCLEAR_MSG:
		msg = "Clear search results"; break;
	case MB_HOVER_SEARCHCLOSE_MSG:
		msg = "Close Search"; break;
	}
	if (msg.GetLength())
		PlayerStatusTempSet(msg);
	return 0;
}
// This bit of nonsense is to allow the class wizard to parse the message map,
// i.e. it fails to parse when multiple messages use the same handler!
LRESULT CPlayerDlg::OnHoverMsg2(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg3(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg4(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg5(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg6(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg7(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg8(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg9(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg10(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg11(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg12(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg13(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg14(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg15(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg16(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg17(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg18(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg19(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg20(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}
LRESULT CPlayerDlg::OnHoverMsg21(WPARAM wParam, LPARAM lParam) { return OnHoverMsg1(wParam,lParam);}


void CPlayerDlg::OnInitMenuPopup(CMenu *pSysMenu, 
								 UINT nIndex,
								 BOOL bSysMenu)
{
	// Disable maximize button for fixed skins
	if (pSysMenu != NULL)
	{
		int count = pSysMenu->GetMenuItemCount();
		CString tmp;
		for(int i=0 ; i < count ; i++) {
			pSysMenu->GetMenuString(i, tmp, MF_BYPOSITION);
			if (TRUE == m_FixedSize && "Ma&ximize" == tmp) {
				pSysMenu->EnableMenuItem(i, 
					MF_BYPOSITION|MF_DISABLED|MF_GRAYED);
			}
		}
	}

}


BEGIN_EVENTSINK_MAP(CPlayerDlg, CDialogClassImpl)
    //{{AFX_EVENTSINK_MAP(CPlayerDlg)
	ON_EVENT(CPlayerDlg, IDC_WMP, 5101 /* PlayStateChange */, OnPlayStateChangeWmp, VTS_I4)
	ON_EVENT(CPlayerDlg, IDC_WMP, 5202 /* PositionChange */, OnPositionChangeWmp, VTS_R8 VTS_R8)
	ON_EVENT(CPlayerDlg, IDC_WMP, 5821 /* MediaError */, OnMediaErrorWmp, VTS_DISPATCH)
	ON_EVENT(CPlayerDlg, IDC_WMP, 5501 /* Error */, OnErrorWmp, VTS_NONE)
	ON_EVENT(CPlayerDlg, IDC_WMP, 5601 /* Warning */, OnWarningWmp, VTS_I4 VTS_I4 VTS_BSTR)
	//}}AFX_EVENTSINK_MAP
END_EVENTSINK_MAP()

void CPlayerDlg::OnPlayStateChangeWmp(long NewState) 
{
	logger.ods("OnPlayStateChangeWmp:"+numToString(NewState));
	if (8 == NewState) {
		KillTimer(m_PlayLoopTimerId);
		m_PlayLoopTimerId = SetTimer(MB_PLAYLOOP_TIMER_ID, 1, NULL);
	}
}

void CPlayerDlg::OnPositionChangeWmp(double oldPosition, double newPosition) 
{
	updatePositionLabel();
}


// In event of these errors, stop MB from continuing so the user
// can see which file has a problem. 

void CPlayerDlg::OnMediaErrorWmp(LPDISPATCH pMediaObject) 
{
	// TODO: Add your control notification handler code here
	logger.log("Media Error Wmp");
	//OnPlayStateChangeWmp(0);
	Stop();
	InputClose();
	//PlayLoop();
	
}

void CPlayerDlg::OnErrorWmp() 
{
	// TODO: Add your control notification handler code here
	logger.log("Error Wmp");
	//OnPlayStateChangeWmp(0);
	Stop();
	InputClose();
	//PlayLoop();
	
}

void CPlayerDlg::OnWarningWmp(long WarningType, long Param, LPCTSTR Description) 
{
	// TODO: Add your control notification handler code here
	logger.log("Warning Wmp");
	//OnPlayStateChangeWmp(0);
	Stop();
	InputClose();
	//PlayLoop();
	
}


