


// PlayerDlg.cpp : implementation file

#include "stdafx.h"
#define THE_PLAYER
#include "PlayerDlg.h"
#define LOGOWNER
#include "MyLog.h"

#include "About.h"
#include "ConfigColors.h"
#include "ConfigFiles.h"
#include "ConfigFonts.h"
#include "ConfigIrman.h"
#include "DialogData.h"
#include "FontCombo.h"
#include "FontTipWnd.h"
#include "HtmlHelp.h"
#include "id3/misc_support.h"
#include "IRCodes.h"
#include "irman_registry.h"
#include "LoadPlaylistDlg.h"
#include "MBConfig.h"
#include "MBGlobals.h"
#include "MBMessageBox.h"
#include "ModifyIDThree.h"
#include "MusicDb.h"
#include "MyID3LibMiscSupport.h"
#include "PlayerApp.h"
#include "SavePlaylistDlg.h"
#include "SerialMFC.h"
#include "TestHarness/TestHarness.h"
#include "VirtualControl.h"
#include "muzikbrowzerVersion.h"
#include "TransparentDialogDlg.h"
#include "util/Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	//enum { IDD = IDD_ABOUTBOX };
    enum { IDD = IDD_DIALOG1 };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog

CPlayerDlg::CPlayerDlg(CPlayerApp * theApp, CTransparentDialogDlg *ip, CWnd* pParent /*=NULL*/)
	: CDialog(CPlayerDlg::IDD, pParent), m_PlayerApp(theApp), _initdialog(ip),
	m_mlib(NULL), m_PlaylistCurrent(-1),
    m_MenuDlg(0), m_StopFlag(FALSE), m_LButtonDown(FALSE),
    m_Config("Configuration"), m_SavePlaylistFlag(TRUE),
	m_PlaylistDuration(0), m_timerid(0), m_StatusTimerId(0),
	m_Control(new VirtualControl), m_Dialog(new VirtualDialog),
	m_trialCounter(0)
{
	//{{AFX_DATA_INIT(CPlayerDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_Playlist.m_reorder = TRUE;
	m_hIcon = AfxGetApp()->LoadIcon(IDI_MBLOGO);
//    m_hIcon = AfxGetApp()->LoadIcon(IDI_MBLOGO3232);
	thePlayer = this;
	irman().init(RegKeyIrman, IR_MESSAGE_NUMBER_OF, this);
//	m_brush.CreateSolidBrush(RGB( 255, 0, 0));
}
CPlayerDlg::~CPlayerDlg() {
    //delete m_Config;
    m_brush.DeleteObject();
	delete m_Control;
	delete m_Dialog;
	delete m_Player;
}

void CPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CPlayerDlg)
	DDX_Control(pDX, IDC_PICTURE_CTRL, m_Picture);
	DDX_Control(pDX, IDC_GENRESLABEL,		m_GenresLabel);
	DDX_Control(pDX, IDC_ARTISTSLABEL,		m_ArtistsLabel);
	DDX_Control(pDX, IDC_ALBUMSLABEL,		m_AlbumsLabel);
	DDX_Control(pDX, IDC_SONGSLABEL,		m_SongsLabel);	
	DDX_Control(pDX, IDC_PLAYLISTLABEL,		m_PlaylistLabel);
	DDX_Control(pDX, IDC_GENRES,			m_Genres);
	DDX_Control(pDX, IDC_ARTISTS,			m_Artists);
	DDX_Control(pDX, IDC_ALBUMS,			m_Albums);
	DDX_Control(pDX, IDC_SONGS,				m_Songs);
	DDX_Control(pDX, IDC_PLAYLIST,			m_Playlist);
	DDX_Control(pDX, IDC_OPTIONS_BUTTON,	m_OptionsButton);
	DDX_Control(pDX, IDC_POSITION_SLIDER,	m_PositionSlider);
	DDX_Control(pDX, IDC_VOLUME_SLIDER,		m_VolumeSlider);
	DDX_Control(pDX, IDC_VOLUME_LABEL,		m_VolumeLabel);
	DDX_Control(pDX, IDC_POSITION_LABEL,	m_PositionLabel);
	DDX_Control(pDX, IDC_PLAYER_STATUS,		m_PlayerStatus);
	DDX_Control(pDX, IDC_CURRENT_TITLE, m_CurrentTitle);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PLAY_BUTTON, OnPlayButton)
	ON_BN_CLICKED(IDC_OPEN_FILE_BUTTON, OnOpenFileButton)
	ON_BN_CLICKED(IDC_STOP_BUTTON, OnStopButton)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDD_ABOUT, OnAbout)
	ON_LBN_SELCHANGE(IDC_ARTISTS, OnSelchangeArtists)
	ON_LBN_SELCHANGE(IDC_ALBUMS, OnSelchangeAlbums)
	ON_LBN_DBLCLK(IDC_SONGS, OnDblclkSongs)
	ON_LBN_DBLCLK(IDC_ALBUMS, OnDblclkAlbums)
	ON_LBN_DBLCLK(IDC_ARTISTS, OnDblclkArtists)
	ON_LBN_DBLCLK(IDC_GENRES, OnDblclkGenres)
	ON_BN_CLICKED(IDC_MENU_BUTTON, OnMenuButton)
	ON_COMMAND(ID_MENU_OPTIONS, OnMenuOptions)
	ON_COMMAND(ID_MENU_CLEARPLAYLIST, OnMenuClearplaylist)
	ON_COMMAND(ID_MENU_EXIT, OnMenuExit)
	ON_COMMAND(ID_MENU_LOADPLAYLIST, OnMenuLoadplaylist)
	ON_COMMAND(ID_MENU_SAVEPLAYLIST, OnMenuSaveplaylist)
    ON_COMMAND(ID_MENU_EXPORT, OnMenuExportLibrary)
	ON_COMMAND(ID_MENU_SHUFFLEPLAYLIST, OnMenuShuffleplaylist)
	ON_COMMAND(ID_MENU_PAUSE, OnMenuPause)
	ON_COMMAND(ID_MENU_PLAY, OnMenuPlay)
	ON_COMMAND(ID_MENU_STOP, OnMenuStop)
	ON_COMMAND(ID_PMENU_HELP, OnMenuHelp)
	ON_LBN_SELCHANGE(IDC_GENRES, OnSelchangeGenres)
	ON_LBN_DBLCLK(IDC_PLAYLIST, OnDblclkPlaylist)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_ID3TAG, OnUserEditSong)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_LBN_SELCHANGE(IDC_SONGS, OnSelchangeSongs)
	ON_LBN_SELCHANGE(IDC_PLAYLIST, OnSelchangePlaylist)
	ON_COMMAND(ID_MENU_RANDOMIZE_PLAYLIST, OnMenuRandomizePlaylist)
	ON_BN_CLICKED(IDC_OPTIONS_BUTTON, OnButtonMenu)
	ON_BN_CLICKED(IDC_BUTTON_MINIMIZE, OnButtonMinimize)
	ON_BN_CLICKED(IDC_BUTTON_MAXIMIZE, OnButtonMaximize)
	ON_COMMAND(ID_MENU_HELP, HelpInfo)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, OnCancel)
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_PMENU_EXIT, OnMenuExit)
	ON_COMMAND(ID_PMENU_CLEAR, OnMenuClearplaylist)
	ON_COMMAND(ID_PMENU_LOADPLAYLIST, OnMenuLoadplaylist)
	ON_COMMAND(ID_PMENU_PAUSE, OnMenuPause)
	ON_COMMAND(ID_PMENU_PLAY, OnMenuPlay)
	ON_COMMAND(ID_PMENU_SHUFFLE, OnMenuShuffleplaylist)
	ON_COMMAND(ID_PMENU_STOP, OnMenuStop)
	ON_WM_COMPAREITEM()
//	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP

	ON_MESSAGE(WM_GRAPHNOTIFY, OnGraphNotify)
	ON_MESSAGE(WM_PLAYLOOP, OnPlayloop)
    ON_MESSAGE(MB_POST_MYIDLE_MESSAGE, OnPostMyIdle)
	ON_LBN_SETFOCUS(IDC_GENRES, OnGenresFocus)
	ON_LBN_SETFOCUS(IDC_ARTISTS, OnArtistsFocus)
	ON_LBN_SETFOCUS(IDC_ALBUMS, OnAlbumsFocus)
	ON_LBN_SETFOCUS(IDC_SONGS, OnSongsFocus)
	ON_LBN_SETFOCUS(IDC_PLAYLIST, OnPlaylistFocus)
    ON_WM_NCHITTEST()
    ON_WM_CTLCOLOR()
    ON_COMMAND(ID_MYTEST, OnTestMenu)
    ON_WM_HELPINFO()
	ON_WM_TIMER()
	ON_MESSAGE(MB_SERIAL_MESSAGE, OnSerialMsg)



END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg message handlers
void initFont(CWnd *);
void changeFont(CWnd *, CFont &);
BOOL CPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_Genres.m_id = "genres";
	m_Artists.m_id = "artists";
	m_Albums.m_id = "albums";
	m_Songs.m_id = "songs";
	m_Playlist.m_id = "playlist";

    *m_Dialog = this;
	m_mlib.readDbLocation();
    CString lpath = m_mlib.getDbLocation();
    logger.open(lpath);
	logger.log(CS("muzikbrowzer version: ") + CS(MUZIKBROWZER_VERSION));
	m_Config.createit(this);

    if (m_mlib.init()) {
		MBMessageBox("Error", "Database corrupted. Rebuild it by performing a\r\nScan in Options/Configuration");
		PlayerStatusSet(CString(
			"Database corrupted. Do a scan in Configuration."));
	}

    // set some of the control's properties
    m_PositionSlider.SetRange(0,100);
	m_VolumeSlider.SetRange(0,50);

    // instanciate an Player
	m_Player = new MusicPlayer(this, lpath);
	m_Player->init();

    // sets title in taskbar
    SetWindowText(MUZIKBROWZER); // needed if you used resource editor to remove title bar	
    ModifyStyle(WS_CAPTION,WS_SYSMENU,SWP_FRAMECHANGED);

    setFont();




    m_HeaderFont.CreateFont(
        /* height */ 				20,
        /* width */ 				0,
        /* escapement */ 			0,
        /* orientation */ 			0,
        /* weight */ 				FW_BOLD,
        /* italic */ 				FALSE,
        /* underline */ 			FALSE,
        /* strikeout */ 			0,
        /* charset */ 				DEFAULT_CHARSET,
        /* outprecision */ 			OUT_CHARACTER_PRECIS,
        /* clipprecision */ 		CLIP_CHARACTER_PRECIS,
        /* quality */ 				DEFAULT_QUALITY,
        /* pitch and family */ 		DEFAULT_PITCH,
        /* facename */ 				"Arial"
		);

	m_OptionsButtonFont.CreateFont(
        /* height */ 				12,
        /* width */ 				0,
        /* escapement */ 			0,
        /* orientation */ 			0,
        /* weight */ 				FW_BOLD,
        /* italic */ 				FALSE,
        /* underline */ 			FALSE,
        /* strikeout */ 			0,
        /* charset */ 				DEFAULT_CHARSET,
        /* outprecision */ 			OUT_CHARACTER_PRECIS,
        /* clipprecision */ 		CLIP_CHARACTER_PRECIS,
        /* quality */ 				DEFAULT_QUALITY,
        /* pitch and family */ 		DEFAULT_PITCH,
        /* facename */ 				"Arial"
		);
	m_OptionsButton.SetFont(&m_OptionsButtonFont);
	m_OptionsButton.SetWindowText("&Menu");

	m_ButtonAppLabel.AutoLoad(IDC_APP_LABEL, this);
	m_ButtonMinimize.AutoLoad(IDC_BUTTON_MINIMIZE,this);
	m_ButtonMaximize.AutoLoad(IDC_BUTTON_MAXIMIZE, this);
//	m_ButtonRestore.AutoLoad(IDC_BUTTON_RESTORE, this);
	m_ButtonExit.AutoLoad(IDC_BUTTON_EXIT, this);

	//m_PlayerStatus.border(TRUE);
	m_GenresLabel.setText("Genres");
	m_ArtistsLabel.setText("Artists");
	m_AlbumsLabel.setText("Albums");
	m_SongsLabel.setText("Songs");
	m_PlaylistLabel.setText("Playlist");

	readConfig();
//	RunTestHarness();
	if (_initdialog) {
		time_t now = CTime::GetCurrentTime().GetTime();
		if (now - _initdialog->m_start.GetTime() < 2) {
			Sleep(1000);
		}
		_initdialog->DestroyWindow();
	}


    CRect rect;
    int max;
    ReadWindowPos(max, rect);
    if (max) {
        ShowWindow(SW_SHOWMAXIMIZED);
//		GetClientRect(rect);
//		MoveWindow(rect, TRUE );
    } else if (rect.TopLeft().x != 0) {
        MoveWindow(rect, TRUE );
    }

    resizeControls(1,1);
    m_Genres.SetFocus();
    OnGenresFocus();
	initDb();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small ic

//    PlayerStatusSet(m_mlib.getLibraryCounts());

	irman().Open();
	//	unsigned long tick = GetTickCount();
	//	do {
	//		if (irman().OK()) {
	//			break;
	//		}
	//	} while (GetTickCount() - tick < 1000);
	//	if (!irman().OK()) {
	//		irman().Close();
	//		MBMessageBox("Irman", "unable to initialize Irman");
	//	}


	//ShowBetaWarning();
	StartStatusTimer();
	//	if (m_Config.trialMode() == TRUE) {
	//		PlayerStatusTempSet("Trial Mode. Set Password in Configuration.");
	//	}
	CString menuFunc,menuDesc;
	irman().getDescs(IR_MESSAGE_MENU, menuFunc, menuDesc);
	if (menuDesc == "") menuDesc = "Menu";
	m_HelpMsg = "Press "; m_HelpMsg += menuDesc; m_HelpMsg += " for options.";
	if (m_Config.trialMode() == 1) {
		m_HelpMsg += " Trial Mode.";
	} else if (m_Config.trialMode() == 2) {
		m_HelpMsg = " Trial Expired.";
	}
	PlayerStatusSet(m_HelpMsg);

	UpdateWindow();

	return FALSE;  // return TRUE  unless you set the focus to a control
}
void CPlayerDlg::ShowBetaWarning() {
	CTime t = CTime::GetCurrentTime();
	CString mdy = t.Format("%d%m%y");
	RegistryKey reg(HKEY_LOCAL_MACHINE, RegKey);
	char buf[50];
	reg.Read("lastrunmdy", buf, 49, "");
	CString lastrunmdy = buf;
	if (lastrunmdy != mdy) {
		reg.Write("lastrunmdy", (LPCTSTR) mdy);
		CString msg;
	msg << 
"Warning: This is a beta version of muzikbrowzer. It\r\n" <<
"may contain bugs which could potentially damage\r\n" <<
"damage data on your computer. While every effort has\r\n" <<
"been made to ensure against this there are no\r\n" <<
"guarantees. Pecan Ventures assumes no liability what\r\n" <<
"so ever for any damage caused by muzikbrowzer\r\n";
		MBMessageBox("Beta software", msg);
	}
}

//void CPlayerDlg::RunTestHarness() {
//
//	TestResult tr;
//	TestRegistry::runAllTests(tr);
//#ifdef _DEBUG
//	if (tr.failures() > 0) {
//		MBMessageBox("muzikbrowzer Test Results", tr.results());
//	} else {
//		logger.log("Test Results");
//		logger.log(tr.results());
//	}
//#else
//	logger.log("Test Results");
//	logger.log(tr.results());
//#endif
//}
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
		m_PositionLabel.initFont();
		m_VolumeLabel.initFont();
		m_CurrentTitle.initFont();
		m_CurrentTitle.m_HCenter = 1;
    }
    LPLOGFONT lplfTitles = m_Config.getTitlesFont();
    m_Genres.changeFont(lplfTitles);
    m_Artists.changeFont(lplfTitles);
    m_Albums.changeFont(lplfTitles);
    m_Songs.changeFont(lplfTitles);
    m_Playlist.changeFont(lplfTitles);
	LPLOGFONT lplfPanel = m_Config.getPanelFont();
	m_GenresLabel.changeFont(lplfPanel);
	m_ArtistsLabel.changeFont(lplfPanel);
	m_AlbumsLabel.changeFont(lplfPanel);
	m_SongsLabel.changeFont(lplfPanel);
	m_PlaylistLabel.changeFont(lplfPanel);

	m_PlayerStatus.changeFont(lplfPanel);
	m_PositionLabel.changeFont(lplfPanel);
	m_VolumeLabel.changeFont(lplfPanel);
	m_CurrentTitle.changeFont(lplfTitles);

//    m_PlayerStatus.SetTextColor(config().getColorTxPanel());
//    m_PlayerStatus.SetBkColor(m_Config.getColorBkPanel());

    m_PositionSlider.SetThumbColors(m_Config.getColorTxPanel(),
        m_Config.getColorTxPanel());
}
void
displayControlInfo(CString desc, CRect & rect, CRect & dialog) {
    CPoint topleft = rect.TopLeft();
    CPoint bottomright = rect.BottomRight();
    int width = rect.Width();
    int height = rect.Height();
    int dialogwidth = dialog.Width();
    int dialogheight = dialog.Height();
    float widthPct = float(width) / dialogwidth;
    float heightPct = float(height) / dialogheight;

    int rel_x = topleft.x - dialog.TopLeft().x;
    int rel_y = topleft.y - dialog.TopLeft().y;

    float rel_x_Pct = float(rel_x) / dialogwidth;
    float rel_y_Pct = float(rel_y) / dialogheight;

    AutoBuf buf(10000);
//    sprintf(buf, "x:%d y:%d x:%d y:%d rx:%d ry:%d rxPct:%d ryPct:%d w:%d h:%d wpct:%f hpct:%f %s\n",
//        topleft.x, topleft.y, bottomright.x, bottomright.y,
//        rel_x, rel_y, rel_x_Pct, rel_y_Pct, width, height, widthPct, heightPct, desc.GetBuffer(100));
    CString msg;

    sprintf(buf.p, "%s_%s_%s %d", "#define MBCD", desc.GetBuffer(100), "TLX", topleft.x);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %d", "#define MBCD", desc.GetBuffer(100), "TLY", topleft.y);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %d", "#define MBCD", desc.GetBuffer(100), "BRX", bottomright.x);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %d", "#define MBCD", desc.GetBuffer(100), "BRY", bottomright.y);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %d", "#define MBCD", desc.GetBuffer(100), "RX", rel_x);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %d", "#define MBCD", desc.GetBuffer(100), "RY", rel_y);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %f", "#define MBCD", desc.GetBuffer(100), "RXPCT", rel_x_Pct);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %f", "#define MBCD", desc.GetBuffer(100), "RYPCT", rel_y_Pct);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %d", "#define MBCD", desc.GetBuffer(100), "WIDTH", width);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %d", "#define MBCD", desc.GetBuffer(100), "HEIGHT", height);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %f", "#define MBCD", desc.GetBuffer(100), "WIDTHPCT", widthPct);
    msg = buf.p; logger.log(msg);
    sprintf(buf.p, "%s_%s_%s %f", "#define MBCD", desc.GetBuffer(100), "HEIGHTPCT", heightPct);
    msg = buf.p; logger.log(msg);



    
}
#define CONTROL_HSPACE 5
#define CONTROL_VSPACE 1

class Control {
public:
	Control(const CString & desc, CWnd *p=NULL, CWnd*l=NULL) : row(0),
		col(0), hpos(-1),
		ctrl(p), label(l),
		height(0), width(0),
		widthpct(0), heightpct(0),
		labelwidth(0), labelheight(0), m_desc(desc) {}

	void dump(CString txt) {
		CString msg;
		char buf[1000];
		sprintf(buf,"%s %15s r=%d c=%d w=%04d h=%04d\r\n",
			txt, m_desc, row, col, width, height);
		OutputDebugString(buf);
	}


	CString m_desc;
	int row;
	int col;
	int hpos; // -1=left just, 0=center, 1=right just
	CWnd * ctrl;
	CWnd * label;
	int width;
	int height;
	double widthpct;
	double heightpct;
	int labelwidth;
	int labelheight;
};
class Controls {
public:
	Controls(CWnd * w) : m_cwnd(w) {}
	~Controls() {
		POSITION pos;
		WORD dlgid;
		Control *cp;
		for (pos = m_controls.GetStartPosition() ; pos != NULL ; ) {
			m_controls.GetNextAssoc(pos, dlgid, (void*&)cp);
			if (cp) {
				delete cp;
			}
		}
	}
	CMapWordToPtr m_controls;
	void add(const CString & desc, int id, int label=0) {
		CWnd * cw = m_cwnd->GetDlgItem(id);
		CWnd * cl = NULL;
		Control * cp;
		if (label) {
			cl = m_cwnd->GetDlgItem(label);
		}
		cp = new Control(desc, cw, cl); 
		m_controls.SetAt((WORD)id, cp);
	}
	Control * getObj(int id) {
		void * p;
		m_controls.Lookup(id, p);
		return (Control*)p;
	}
	Control * getObj(int row, int col) {
		Control * cp;
		WORD dlgid;
		POSITION pos;
		int rowMaxHeight = 0;
		int found = 0;
		for (pos = m_controls.GetStartPosition();
				pos != NULL && found == 0;) {
			cp = NULL;
			m_controls.GetNextAssoc(pos, dlgid, (void*&)cp);
			if (cp->row == row && cp->col == col) {
				found = 1;
			}
		}
		if (found) {
			return cp;
		} else {
			return NULL;
		}
			
	}
private:
	CWnd * m_cwnd;
};

void
CPlayerDlg::resizeControls(BOOL withPic, int init) {
	withPic = TRUE;
	static Controls controls(this);
	CRect rect;
	const int numRows = 6;

	if (init) {
		controls.add("options button", IDC_OPTIONS_BUTTON);
		controls.add("app label", IDC_APP_LABEL);
		controls.add("button min", IDC_BUTTON_MINIMIZE);
		controls.add("button max", IDC_BUTTON_MAXIMIZE);
		controls.add("button exit", IDC_BUTTON_EXIT);
		controls.add("genre", IDC_GENRES, IDC_GENRESLABEL);
		controls.add("artist", IDC_ARTISTS, IDC_ARTISTSLABEL);
		controls.add("album", IDC_ALBUMS, IDC_ALBUMSLABEL);
		controls.add("picture", IDC_PICTURE_CTRL);
		controls.add("songs", IDC_SONGS, IDC_SONGSLABEL);
		controls.add("playlist", IDC_PLAYLIST, IDC_PLAYLISTLABEL);
		controls.add("title", IDC_CURRENT_TITLE);
		controls.add("pos slider", IDC_POSITION_SLIDER);
		controls.add("pos label", IDC_POSITION_LABEL);
		controls.add("vol slider", IDC_VOLUME_SLIDER);
		controls.add("vol label", IDC_VOLUME_LABEL);
		controls.add("player status", IDC_PLAYER_STATUS);

		int labelheight = m_GenresLabel.GetItemHeight();
		int textheight = m_Genres.GetItemHeight(0);
	
		Control * p = controls.getObj(IDC_OPTIONS_BUTTON);
		p->ctrl->GetWindowRect(rect);
		p->width = rect.Width(); p->height = rect.Height();
		p->row = 0;	p->col = 0;

		p = controls.getObj(IDC_APP_LABEL);
		p->ctrl->GetWindowRect(rect);
		p->width = rect.Width(); p->height = rect.Height();
		p->row = 0;	p->col = 1; p->hpos = 0;
		
		p = controls.getObj(IDC_BUTTON_MINIMIZE);
		p->ctrl->GetWindowRect(rect);
		p->width = rect.Width(); p->height = rect.Height();
		p->row = 0;	p->col = 2; p->hpos = 1;
		
		p = controls.getObj(IDC_BUTTON_MAXIMIZE);
		p->width = rect.Width(); p->height = rect.Height();
		p->row = 0;	p->col = 3; p->hpos = 1;
		
		p = controls.getObj(IDC_BUTTON_EXIT);
		p->width = rect.Width(); p->height = rect.Height();
		p->row = 0;	p->col = 4; p->hpos = 1;
		
		p = controls.getObj(IDC_GENRES);
		p->row = 1;	p->col = 0; 
		p->widthpct = 0.30; p->heightpct = 0.333;
		p->labelheight = labelheight;

		p = controls.getObj(IDC_ARTISTS);
		p->row = 1;	p->col = 1; 
		p->widthpct = 0.50; p->heightpct = 0.333;
		p->labelheight = labelheight;
		
		p = controls.getObj(IDC_PICTURE_CTRL);
		p->row = 1;	p->col = 2;
		p->widthpct = 0.20; p->heightpct = 0.333;
		p->labelheight = labelheight;
		
		p = controls.getObj(IDC_ALBUMS);
		p->row = 2;	p->col = 0; 
		p->widthpct = 0.50; p->heightpct = 0.333;
		p->labelheight = labelheight;
		
		p = controls.getObj(IDC_SONGS);
		p->row = 2;	p->col = 1;
		p->widthpct = 0.50; p->heightpct = 0.333;
		p->labelheight = labelheight;
		
		p = controls.getObj(IDC_PLAYLIST);
		p->row = 3;	p->col = 0;
		p->widthpct = 1.0; p->heightpct = 0.333;
		p->labelheight = labelheight;

		p = controls.getObj(IDC_CURRENT_TITLE);
		p->row = 4;	p->col = 0;
		p->widthpct = 1.0;
		p->height = textheight;

		p = controls.getObj(IDC_POSITION_SLIDER);
		p->row = 5;	p->col = 0;
		p->ctrl->GetWindowRect(rect);
		p->width = rect.Width(); p->height = rect.Height();
		
		p = controls.getObj(IDC_POSITION_LABEL);
		p->row = 5;	p->col = 1;
		p->height = labelheight;
		
		p = controls.getObj(IDC_VOLUME_SLIDER);
		p->row = 5;	p->col = 2;
		p->width = rect.Width(); p->height = rect.Height();
		
		p = controls.getObj(IDC_VOLUME_LABEL);
		p->row = 5;	p->col = 3;
		p->height = labelheight;
		
		p = controls.getObj(IDC_PLAYER_STATUS);
		p->row = 5;	p->col = 4;
		p->height = labelheight;
		p->widthpct = 1.0;
	}

	int labelheight = m_GenresLabel.GetItemHeight();
	int textheight = m_Genres.GetItemHeight(0);

	Control * p;

	p = controls.getObj(IDC_GENRES);
	p->labelheight = labelheight;

	p = controls.getObj(IDC_ARTISTS);
	p->labelheight = labelheight;

	p = controls.getObj(IDC_PICTURE_CTRL);
	p->labelheight = labelheight;

	p = controls.getObj(IDC_ALBUMS);
	p->labelheight = labelheight;
	
	p = controls.getObj(IDC_SONGS);
	p->labelheight = labelheight;
	
	p = controls.getObj(IDC_PLAYLIST);
	p->labelheight = labelheight;

	p = controls.getObj(IDC_CURRENT_TITLE);
	p->height = textheight;

	p = controls.getObj(IDC_POSITION_LABEL);
	CSize s = m_PositionLabel.GetSize("00000000000");
	p->width = s.cx;
	p->height = labelheight;
	
	p = controls.getObj(IDC_VOLUME_LABEL);
	s = m_VolumeLabel.GetSize("0000");
	p->width = s.cx;
	p->height = labelheight;
	
	p = controls.getObj(IDC_PLAYER_STATUS);
	p->height = labelheight;

	Control * cp;
	CRect dialog;
    GetClientRect(dialog);
	int row = 0;

	int bottom = 0 ; int right = 0;
	int moreCols;
	int controly = 0;
	CList<Control *, Control *> rjust;

//	OutputDebugString("dialog.Height()=" + numToString(dialog.Height())+
//		"\r\n");

	// Determine fixedy
	int fixedy = CONTROL_VSPACE;
	for(row = 0 ; row < numRows ; ++row ) {
		int col = 0;
		int rowMaxHeight = 0;
		int labelMaxHeight = 0;
		moreCols = 1;
		while (moreCols) {
			cp = controls.getObj(row, col);
			if (cp == NULL) {
				moreCols = 0;
			} else {
				if (cp->heightpct == 0.0) {
					rowMaxHeight =
						cp->height > rowMaxHeight 
							? cp->height : rowMaxHeight;
				}
				labelMaxHeight = cp->labelheight
					? cp->labelheight : labelMaxHeight;
//				cp->dump("fixedy calc");
//				OutputDebugString(numToString(rowMaxHeight) + "\r\n");
			}
			++col;
		}
		if (rowMaxHeight)
			fixedy += rowMaxHeight + CONTROL_VSPACE;
		if (labelMaxHeight)
			fixedy += labelMaxHeight + CONTROL_VSPACE;
	}
	fixedy += CONTROL_VSPACE;
	int yremains = dialog.Height() - fixedy;
//	OutputDebugString("fixedy=" + numToString(fixedy) + " yremains=" +
//		numToString(yremains) + "\r\n");
	
	for(row = 0 ; row < numRows ; ++row ) {
		// Determine fixedx per row
		int fixedx = CONTROL_HSPACE;
		int col = 0;
		moreCols = 1;
		while (moreCols) {
			cp = controls.getObj(row, col);
			if (cp == NULL) {
				moreCols = 0;
			} else {
				if (cp->widthpct == 0.0)
					fixedx += cp->width + CONTROL_HSPACE;
			}
			++col;
		}
		fixedx += CONTROL_HSPACE;
		int xremains = dialog.Width() - fixedx;
		CString msg = "xremains=" + numToString(xremains);
		msg += " yremains=" + numToString(yremains) + "\r\n";
//		OutputDebugString(msg);

		// Now have fixedx && fixedy so calculate %'s
		col = 0;
		moreCols = 1;
		int rowwidth = CONTROL_HSPACE;
		Control * lastcp;
		while (moreCols) {
			lastcp = cp;
			cp = controls.getObj(row, col);
			if (cp == NULL) {
				moreCols = 0;
				if (lastcp->hpos == -1) {
					rowwidth -= (lastcp->width + CONTROL_HSPACE);
					lastcp->width = (dialog.Width() - rowwidth) - CONTROL_HSPACE;
				}
			} else {
				if (cp->widthpct > 0) {
					cp->width = xremains * cp->widthpct;
				}
				if (cp->heightpct > 0) {
					cp->height = yremains * cp->heightpct;
				}
				rowwidth += cp->width + CONTROL_HSPACE;

//				cp->dump("1");
			}

			++col;
		}
	}

	// Now just paint 'em
	int y = CONTROL_VSPACE;
	for(row = 0 ; row < numRows ; ++row ) {
		int x = 0;
		int col = 0;
		int rowMaxHeight = 0;
		rjust.RemoveAll();
		moreCols = 1;
		while (moreCols) {
			cp = controls.getObj(row, col);
			if (cp == NULL) { // last col in row
				moreCols = 0;
				y += rowMaxHeight + CONTROL_VSPACE;
			} else {
				if (cp->hpos == 1) {
					rjust.AddHead(cp);
				} else {
					if (cp->hpos == -1) {
						x += CONTROL_HSPACE;
					} else if (cp->hpos == 0) {
						x = (dialog.Width() / 2) - (cp->width / 2);
					}
					controly = y;
					if (cp->label) {
						cp->label->MoveWindow(x, y, cp->width,
							cp->labelheight, TRUE);
					}
					controly = y + cp->labelheight + CONTROL_VSPACE;
					cp->ctrl->MoveWindow(x, controly, cp->width,
						cp->height, TRUE);
					rowMaxHeight = cp->height+cp->labelheight > rowMaxHeight 
						? cp->height+cp->labelheight : rowMaxHeight;
					x += cp->width;
//					cp->dump("2");
				}

				++col;
			}
		}
		x = dialog.Width();
		while (!rjust.IsEmpty()) {
			cp = rjust.RemoveHead();
			x -= (CONTROL_HSPACE + cp->width);
			cp->ctrl->MoveWindow(x, controly, cp->width, cp->height, TRUE);
			rowMaxHeight = cp->height > rowMaxHeight 
				? cp->height : rowMaxHeight;
		}
	}
	RedrawWindow();
}

void
CPlayerDlg::getControlSettings() {

    CRect dialog, genres, artists, albums, songs, playlist, positionslider, xstatus,
        playerstatus;
    CRect genresLabel, artistsLabel, albumsLabel, songsLabel, playlistLabel;
    
    CWnd * ctrl;

//    GetWindowRect(dialog);
    GetClientRect(dialog);
    ClientToScreen(dialog);
    logger.log(CString("#ifndef _DIALOG_DATA_"));
    logger.log(CString("#define _DIALOG_DATA_"));
    displayControlInfo("DIALOG", dialog, dialog);
    
    ctrl = GetDlgItem(IDC_GENRESLABEL);
    ctrl->GetWindowRect(genresLabel);
    displayControlInfo("GENRESLABEL", genresLabel, dialog);
    ctrl = GetDlgItem(IDC_GENRES);
    ctrl->GetWindowRect(genres);
    displayControlInfo("GENRES", genres, dialog);

    ctrl = GetDlgItem(IDC_ARTISTSLABEL);
    ctrl->GetWindowRect(artistsLabel);
    displayControlInfo("ARTISTSLABEL", artistsLabel, dialog);    
    ctrl = GetDlgItem(IDC_ARTISTS);
    ctrl->GetWindowRect(artists);
    displayControlInfo("ARTISTS", artists, dialog);

    ctrl = GetDlgItem(IDC_ALBUMSLABEL);
    ctrl->GetWindowRect(albumsLabel);
    displayControlInfo("ALBUMSLABEL", albumsLabel, dialog);    
    ctrl = GetDlgItem(IDC_ALBUMS);
    ctrl->GetWindowRect(albums);
    displayControlInfo("ALBUMS", albums, dialog);
    
    ctrl = GetDlgItem(IDC_SONGSLABEL);
    ctrl->GetWindowRect(songsLabel);
    displayControlInfo("SONGSLABEL", songsLabel, dialog);    
    ctrl = GetDlgItem(IDC_SONGS);
    ctrl->GetWindowRect(songs);
    displayControlInfo("SONGS", songs, dialog);

    ctrl = GetDlgItem(IDC_PLAYLISTLABEL);
    ctrl->GetWindowRect(playlistLabel);
    displayControlInfo("PLAYLISTLABEL", playlistLabel, dialog);    
    ctrl = GetDlgItem(IDC_PLAYLIST);
    ctrl->GetWindowRect(playlist);
    displayControlInfo("PLAYLIST", playlist, dialog);

    ctrl = GetDlgItem(IDC_POSITION_SLIDER);
    ctrl->GetWindowRect(positionslider);
    displayControlInfo("POSITIONSLIDER", positionslider, dialog);

    ctrl = GetDlgItem(IDC_VOLUME_SLIDER);
    ctrl->GetWindowRect(xstatus);
    displayControlInfo("XSTATUS", xstatus, dialog);

    ctrl = GetDlgItem(IDC_PLAYER_STATUS);
    ctrl->GetWindowRect(playerstatus);
    displayControlInfo("PLAYERSTATUS", playerstatus, dialog);
    logger.log(CString("#endif"));
}

void
CPlayerDlg::init() {

    if (m_mlib.init()) {
		MBMessageBox("Error", "Database corrupted. Rebuild it by performing a\r\nScan in Options/Configuration");
		PlayerStatusSet(CString(
			"Database corrupted. Do a scan in Configuration."));
	}
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
	if (_selectedGenre == ""
			|| m_Genres.SelectString(0, _selectedGenre) == LB_ERR) {
		m_Genres.SetCurSel(0);
	}
	m_Artists.SetCurSel(0);
	m_Albums.SetCurSel(0);
	OnSelchangeGenres() ;



	//	IRReaderStart();

}

void CPlayerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)	{
		About dlgAbout;
		dlgAbout.DoModal();
    } else {
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CPlayerDlg::OnPaint() 
{
//	OutputDebugString("CPlayerDlg::OnPaint\r\n");
	if (IsIconic())	{
		CPaintDC dc(this); // device context for painting

//		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
//		int cxIcon = GetSystemMetrics(SM_CXICON);
//		int cyIcon = GetSystemMetrics(SM_CYICON);
//		CRect rect;
//		GetClientRect(&rect);
//		int x = (rect.Width() - cxIcon + 1) / 2;
//		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
//		dc.DrawIcon(x, y, m_hIcon);
    } else {
		//CPaintDC dc(this); // device context for painting

		//CRect rect;
        //GetClientRect(&rect);
        //ScreenToClient(rect);
        //rect.InflateRect(2,2);
        //CBrush br(config().getColorBkNormal());
        //dc.FillRect(rect, &br);
        //OnSelchangeGenres();


		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CPlayerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CPlayerDlg::DestroyWindow() 
{
	// delete the Xaudio player
    if (m_Player) {
        Stop();
    }

	//	IRReaderStop();
	return CDialog::DestroyWindow();
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
							"MPEG Audio Files {*.mpg;*.mp1;*.mp2;*.mp3}|*.mpg;*.mp1;*.mp2;*.mp3|All Files {*.*}|*.*||");


	int ret;
	ret = dialog->DoModal();
	if (ret == IDOK) {
        // a file was selected
		m_Player->InputOpen(dialog->GetPathName());
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


#ifdef asdfold
void CPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{

    if (pScrollBar->m_hWnd == m_PositionSlider.m_hWnd) {
        switch (nSBCode) {
          case SB_LINELEFT:
          case SB_LINERIGHT:
          case SB_PAGELEFT:
          case SB_PAGERIGHT:
			  //            if (m_Player->isScrolling() == FALSE) {
			  //                m_Player->setScrolling(TRUE);
			  //                Pause();
			  //            }
            break;

          case SB_ENDSCROLL: 
            m_Player->Seek(m_PositionSlider.GetPos(), TRUE);
			//            if (m_Player->isScrolling() == TRUE) {
			//                m_Player->setScrolling(FALSE);
			//                if (m_Player->isPlaying()) {
			//                    Play();
			//                }
			//            }
            break;

          case SB_THUMBTRACK:
			  //            if (m_Player->isScrolling() == FALSE) {
			  //                m_Player->setScrolling(TRUE);
			  //                Pause();
			  //            }
            break;

          case SB_THUMBPOSITION: 
            m_Player->Seek(nPos);
            break;
          }

        CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
    }
}
#endif

void CPlayerDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if (pScrollBar->m_hWnd == m_PositionSlider.m_hWnd) {
		switch (nSBCode) {
			case SB_ENDSCROLL: 
				adjustPosition();
				break;
			case SB_THUMBPOSITION: 
				adjustPosition(nPos);
				break;
		}
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	} else if (pScrollBar->m_hWnd == m_VolumeSlider.m_hWnd) {
		switch (nSBCode) {
			case SB_ENDSCROLL: 
				adjustVolume();
				break;
			case SB_THUMBPOSITION: 
			case SB_THUMBTRACK:
				adjustVolume(nPos);
				break;
		}
		CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	}
}

void CPlayerDlg::OnAbout() 
{
    About dialog;
    dialog.DoModal();
}


void CPlayerDlg::OnControlSelChange() {
    if (m_Control->ptr() == &m_Genres) {
        OnSelchangeGenres();
    } else if (m_Control->ptr() == &m_Artists) {
        OnSelchangeArtists();
    } else if (m_Control->ptr() == &m_Albums) {
        OnSelchangeAlbums();
    } else if (m_Control->ptr() == &m_Songs) {
        OnSelchangeSongs();
    } else if (m_Control->ptr() == &m_Playlist) {
        OnSelchangePlaylist();
    }
}

void CPlayerDlg::OnControlClick() {
    if (m_Control->ptr() == &m_Genres) {
        OnDblclkGenres() ;
    } else if (m_Control->ptr() == &m_Artists) {
        OnDblclkArtists() ;
    } else if (m_Control->ptr() == &m_Albums) {
        OnDblclkAlbums() ;
    } else if (m_Control->ptr() == &m_Songs) {
        OnDblclkSongs();
    } else if (m_Control->ptr() == &m_Playlist) {
        OnDblclkPlaylist();

//    } else if (m_Control->ptr() == m_MenuDlg) {
//        m_MenuDlg->EndDialog(m_MenuDlg->currentbutton());
    }
}
static void
rememberSelections(CMapStringToString & map, CString sel,
                   CExtendedListBox & box) {
    CString val;
    if (map.Lookup(sel, val) == 0 || box.SelectString(0, val) == LB_ERR) {
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
	static int lastsel = -1;
	UpdateWindow();

	int sel = m_Genres.GetCurSel();
    if (sel < 0) return;
	if (lastsel != sel) {
		m_LastThingQueuedUp = "";
	}
	lastsel = sel;
	m_Genres.GetText(sel, _selectedGenre);
	m_Artists.ResetContent();	
	m_mlib.getArtists(_selectedGenre, m_Artists);
//	m_Artists.SetCurSel(0);	
    rememberSelections(m_GenreArtist, _selectedGenre, m_Artists);
	OnSelchangeArtists();
	if (_selectedGenre == MBALL) {
		PlayerStatusTempSet(m_mlib.getLibraryCounts());
	} else {
		PlayerStatusTempSet(_selectedGenre);
	}

    m_Genres.invalidate();
}

void CPlayerDlg::OnSelchangeArtists() 
{
	static int lastsel = -1;
	int sel = m_Artists.GetCurSel();
    if (sel < 0) return;
	if (lastsel != sel) {
		m_LastThingQueuedUp = "";
	}
	lastsel = sel;
	m_Artists.GetText(sel,_selectedArtist);
    m_GenreArtist.SetAt(_selectedGenre, _selectedArtist);

	m_Albums.ResetContent();	
	m_mlib.getAlbums(_selectedGenre,
		_selectedArtist, m_Albums);
//	m_Albums.SetCurSel(0);	
    rememberSelections(m_ArtistAlbum, _selectedArtist, m_Albums);
	OnSelchangeAlbums();
    PlayerStatusTempSet(_selectedArtist);
    m_Artists.invalidate();
}

void CPlayerDlg::OnSelchangeAlbums() 
{
	static int lastsel = -1;
	int sel = m_Albums.GetCurSel();
    if (sel < 0) return;
	if (lastsel != sel) {
		m_LastThingQueuedUp = "";
	}
	lastsel = sel;
	m_Albums.GetText(sel,_selectedAlbum);
    m_ArtistAlbum.SetAt(_selectedArtist, _selectedAlbum);

	m_Songs.ResetContent();	
	m_mlib.getSongs(_selectedGenre,
		_selectedArtist,
		_selectedAlbum,m_Songs);
//	m_Songs.SetCurSel(0);	
    rememberSelections(m_AlbumSong, _selectedAlbum, m_Songs);
    OnSelchangeSongs();
    PlayerStatusTempSet(_selectedAlbum);
    m_Albums.invalidate();
}

void CPlayerDlg::OnSelchangeSongs() 
{
	static int lastsel = -1;
	static CString lastalbum;
	int sel = m_Songs.GetCurSel();
    if (sel < 0) return;
	if (lastsel != sel) {
		m_LastThingQueuedUp = "";
	}
	lastsel = sel;
	m_Songs.GetText(sel,_selectedSong);
    m_AlbumSong.SetAt(_selectedAlbum, _selectedSong);

    OnSelchangePlaylist();
    PlayerStatusTempSet(_selectedSong);	
    m_Songs.invalidate();

	if (lastalbum != _selectedAlbum) {
		Song song = m_mlib.getSong(_selectedGenre, _selectedArtist,
			_selectedAlbum, _selectedSong);
		CString file = song->getId3("FILE");
		displayAlbumArt(file);
	}
	lastalbum = _selectedAlbum;
}

void CPlayerDlg::OnSelchangePlaylist() 
{
	int sel = m_Playlist.GetCurSel();
    if (sel < 0) return;
	m_Playlist.GetText(sel,_selectedPlaylistSong);
    PlayerStatusTempSet(_selectedPlaylistSong);
    m_Playlist.invalidate();
}

void CPlayerDlg::updatePlaylist() {
	m_Playlist.ResetContent();
	m_mlib.getPlaylist(m_Playlist);
	m_Playlist.SetCurSel(m_Playlist.GetCount()-1);

	calcDuration();

	UpdateWindow();
    PlayLoop();

    m_Playlist.invalidate();
}

void CPlayerDlg::OnDblclkSongs() 
{
	CString last = _selectedGenre + _selectedArtist + _selectedAlbum +
		_selectedSong;
	if (last != m_LastThingQueuedUp) {
//		m_mlib.addSongToPlaylist(_selectedGenre, _selectedArtist, _selectedAlbum,
//			_selectedSong);

		Song song = new CSong;
	    int sel = m_Songs.GetCurSel();
		DWORD data = m_Songs.GetItemData(sel);
		song = m_mlib.m_SongLib.getSong(data);
		m_mlib.addSongToPlaylist(song);

		updatePlaylist();
		m_LastThingQueuedUp = last;
	} else {
		PlayerStatusTempSet("You just added that!");
	}
}

void CPlayerDlg::OnDblclkAlbums() 
{
	CString last = _selectedGenre + _selectedArtist + _selectedAlbum;
	if (last != m_LastThingQueuedUp) {
		m_mlib.addAlbumToPlaylist(_selectedGenre, _selectedArtist, _selectedAlbum);
		updatePlaylist();
		m_LastThingQueuedUp = last;
	} else {
		PlayerStatusTempSet("You just added that!");
	}
}

void CPlayerDlg::OnDblclkArtists() 
{
	CString last = _selectedGenre + _selectedArtist;
	if (last != m_LastThingQueuedUp) {
		m_mlib.addArtistToPlaylist(_selectedGenre, _selectedArtist);
		updatePlaylist();
		m_LastThingQueuedUp = last;
	} else {
		PlayerStatusTempSet("You just added that!");
	}
}

void CPlayerDlg::OnDblclkGenres() 
{
	if (_selectedGenre == " all") return;
	CString last = _selectedGenre;
	if (last != m_LastThingQueuedUp) {
		m_mlib.addGenreToPlaylist(_selectedGenre);	
		updatePlaylist();
		m_LastThingQueuedUp = last;
	} else {
		PlayerStatusTempSet("You just added that!");
	}
}
void CPlayerDlg::OnDblclkPlaylist() 
{
    int sel = m_Playlist.GetCurSel();
    m_PlaylistCurrent = sel -1;
    Stop();

    PlayLoop();

}

BOOL CPlayerDlg::PreTranslateMessage(MSG* pMsg)
{	// disable ESC & ENTER from killing the dialog
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) {
            return TRUE;
        } else if (pMsg->wParam == VK_DELETE) {
            OnDelete();
            return TRUE;
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

void CPlayerDlg::OnMenuButton() 
{
    int r = -1;
    if (m_MenuDlg == 0) {
        m_MenuDlg = new CMenuDialog (this);
        *m_Dialog = m_MenuDlg;
	    r = m_MenuDlg->DoModal();
	    delete m_MenuDlg;
	    m_MenuDlg = 0;
		*m_Dialog = this;
    }

    switch (r) {
    case 0:     // exit
//        OnMenuExit();
        break;
    case 1:     // shuffle
        OnMenuShuffleplaylist();
        break;
    case 2:     // randomize
        OnMenuRandomizePlaylist();
        break;
    case 3:     // clear
        OnMenuClearplaylist();
        break;
    case 4:
        SetSavePlaylistFlag(FALSE);
        OnMenuSaveplaylist();
        break;
    case 5:      // load
        OnMenuLoadplaylist();
        break;
    case 6:     // pause
        OnMenuPause();
        break;
    case 7:     // play
        OnMenuPlay();
        break;
    case 8:     // stop
        OnMenuStop();
        break;
    case 9:     // help
        OnMenuHelp();
    }
}

void
CPlayerDlg::OnGenresFocus( ) {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_GENRES);
    PlayerStatusTempSet(_selectedGenre);
}
void
CPlayerDlg::OnArtistsFocus() {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_ARTISTS);
    PlayerStatusTempSet(_selectedArtist);
}
void
CPlayerDlg::OnAlbumsFocus() {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_ALBUMS);
    PlayerStatusTempSet(_selectedAlbum);
}
void
CPlayerDlg::OnSongsFocus() {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_SONGS);
    PlayerStatusTempSet(_selectedSong);
}
void
CPlayerDlg::OnPlaylistFocus() {
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_PLAYLIST);
    PlayerStatusTempSet(_selectedPlaylistSong);
}
void
CPlayerDlg::OnDelete() {
    if (m_Control->ptr() == &m_Playlist) {
	    int sel = m_Playlist.GetCurSel();
        if (sel < 0) { return; }
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
            m_mlib.dumpPL(m_PlaylistCurrent);
        } else {
//            PlayerStatusSet(m_mlib.getLibraryCounts());
        }
		calcDuration();
    }
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

bool EndScreenSaver()
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
CPlayerDlg::OnGraphNotify(UINT wParam, LONG lParam) {
	m_Player->HandleGraphEvent();
	return 0;
}

LRESULT
CPlayerDlg::OnPlayloop(UINT wParam, LONG lParam) {
	PlayLoop();
	return 0;
}

int
CPlayerDlg::HandleIRMessage(int key) {

//    EndScreenSaver();

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
	default:
		wakeitup = 0;
	}

	if (wakeitup) {
	    WakeUp();
	    if (IsIconic()) {
			ShowWindow(SW_RESTORE);
			return 0;
		}
	}
    return 0;
}


afx_msg LRESULT
CPlayerDlg::OnSerialMsg (WPARAM wParam, LPARAM lParam) {
	int key;
	BOOL more = TRUE;
	while (more) {
		more = irman().HandleSerialMsg(wParam, lParam, key);
		HandleIRMessage(key);
	}
	return 0;
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
CPlayerDlg::OnPostMyIdle(UINT wParam, LONG lParam) {
//	OutputDebugString("OnPostMyIdle");
    PlayerStatusRevert();
    return 0;
}

void CPlayerDlg::OnMenuOptions() {
	//    IRReaderStop();
	irman().Close();
	m_Config.DoModal();
    setFont();
    PlayerStatusTempSet(m_mlib.getLibraryCounts());
	resizeControls();
    redraw();
	//    IRReaderStart();
	irman().Close();
	irman().init(RegKeyIrman, IR_MESSAGE_NUMBER_OF, this);
	irman().Open();

	CString menuFunc,menuDesc;
	irman().getDescs(IR_MESSAGE_MENU, menuFunc, menuDesc);
	if (menuDesc == "") menuDesc = "Menu";
	m_HelpMsg = "Press "; m_HelpMsg += menuDesc; m_HelpMsg += " for options.";
	if (m_Config.trialMode() == 1) {
		m_HelpMsg += " Trial Mode.";
	} else if (m_Config.trialMode() == 2) {
		m_HelpMsg = " Trial Expired.";
	}
	PlayerStatusSet(m_HelpMsg);
}
void
CPlayerDlg::redraw() {
    m_PositionSlider.ClearSel(TRUE);
    RedrawWindow();
}

void CPlayerDlg::OnMenuClearplaylist() 
{
    m_mlib._playlist.reset();
    m_PlaylistCurrent = -1;
    updatePlaylist();
    Stop();
    InputClose();
    CString libcounts = m_mlib.getLibraryCounts();
	resetPosition();
	
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
void CPlayerDlg::OnMenuLoadplaylist() 
{

    LoadPlaylistDlg * m_LoadPlaylistDlg 
        = new LoadPlaylistDlg (this, &m_mlib, this);
    *m_Dialog = m_LoadPlaylistDlg;
	int r = m_LoadPlaylistDlg->DoModal();
	delete m_LoadPlaylistDlg;
	m_LoadPlaylistDlg = 0;
    *m_Dialog = this;
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
		m_mlib.savePlaylist(name);
}

void CPlayerDlg::OnMenuShuffleplaylist() 
{
	m_mlib.shufflePlaylist();
    m_Playlist.ResetContent();
	m_mlib.getPlaylist(m_Playlist);
	m_Playlist.SetCurSel(0);
	UpdateWindow();
    Stop();
    m_PlaylistCurrent = -1;
	PlayLoop();	
}

void CPlayerDlg::OnMenuRandomizePlaylist() 
{
	m_mlib.RandomizePlaylist();
    m_Playlist.ResetContent();
	m_mlib.getPlaylist(m_Playlist);
	m_Playlist.SetCurSel(0);
	UpdateWindow();
    Stop();
    m_PlaylistCurrent = -1;
	PlayLoop();	
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
	msg = "Use Up,Down,Left,Right to navigate,\r\nSelect to queue up music.\r\n";
	n = irman().numOfKeys();
	AutoBuf buf(200);
	const char * HFORMAT = "%-12s %-12s | %-12s %-12s\r\n";
	sprintf(buf.p, HFORMAT,
		"Function","Remote Key", "Function", "Remote Key");
	msg += buf.p;
	msg += "-------------------------------------------------\r\n";
	int first = 1;
	for(i = 0 ; i < n ; i += 2) {
		irman().getDescs(i, d1, r1);
		irman().getDescs(i+1, d2, r2);
		sprintf(buf.p, HFORMAT,
			d1,r1,d2,r2);
		msg += buf.p;
	}
	MBMessageBox("Help", msg, FALSE);
}

void CPlayerDlg::OnNextSong() {
    Stop();
	PlayLoop();
}
void CPlayerDlg::OnPreviousSong() {
    if (m_PlaylistCurrent <= 0) {
        m_PlaylistCurrent = m_Playlist.GetCount() -2;
    } else {
        m_PlaylistCurrent -= 2;
    }
    OnNextSong();
}
void CPlayerDlg::Play() {
	StartSeekTimer();
	m_Player->Play();
}
void CPlayerDlg::Stop() {
	m_Player->Stop();
	StopSeekTimer();
	CurrentTitleSet("");
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
	}
}


void CPlayerDlg::OnStopButton() 
{
	Stop();
    m_StopFlag = TRUE;
	PlayerStatusTempSet("Stopped");
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
    static int first = 1;
//	static int trialCounter = 0;
    int good = 0;
    do {
        if (m_Player->isStopped()
                && m_mlib._playlist.size() && m_StopFlag == FALSE) {
            if (first) {
                first = 0;
            } else {
				recordTLEN();
				resetPosition();
				InputClose();
            }
            m_PlaylistCurrent++;
            if (m_PlaylistCurrent+1 > m_mlib._playlist.size()) {

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
            CString file = m_mlib._playlist[m_PlaylistCurrent]->getId3("FILE");

			if (fileIsReadable(file)) {
				good = 1;
			} else {
				good = 0;
			}
        
			CString msg;
            if (good && m_Player->InputOpen(file)) {
				Play();
				adjustVolume();
				good = 1;
				displayAlbumArt(file);
			} else {
				good = 0;
				msg = "Unable to play ";
				msg += m_mlib._playlist[m_PlaylistCurrent]->getId3("FILE");
				CurrentTitleSet(msg);
				UpdateWindow();
				logger.log(msg);
				SetTimer(MB_PLAYLOOP_TIMER_ID, 1000, NULL);
				break;
			}

            msg = m_mlib._playlist[m_PlaylistCurrent]->getId3("TPE1");
            msg += " / ";
            msg += m_mlib._playlist[m_PlaylistCurrent]->getId3("TALB");
            msg += " / ";
            msg += m_mlib._playlist[m_PlaylistCurrent]->getId3("TIT2");
            CurrentTitleSet(msg);
			if (m_Config.trialMode() == 1) {
				PlayerStatusTempSet("Trial Mode. Set Password in Configuration.");
			}
            UpdateWindow();


        } else {
            good = 1;
        }
    } while (!good);
}

void
CPlayerDlg::setDuration(int duration) {
	m_PlaylistDuration = duration;
	CString msg = "Playlist ";
//	msg += "  duration = ";
	AutoBuf buf(500);
	int hour = duration / 3600;
	int sec = duration % 3600;
	int min = sec / 60;
	sec = sec % 60;
	sprintf(buf.p, "%02d:%02d:%02d", hour, min, sec);
	msg += buf.p;
	m_PlaylistLabel.setText(msg);
}
void
CPlayerDlg::addDuration(int duration) {
	m_PlaylistDuration += duration;
	setDuration(m_PlaylistDuration);
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
		if (sDuration != "") {
			// TLEN is in milliseconds
			int millis = atoi((LPCTSTR)sDuration);
			if (millis > 999)
				duration = millis / 1000;
		}
		//		if (duration == 0) {
		//			CString file = m_mlib._playlist[i]->getId3("FILE");
		//			duration = m_Player->GetFileDuration(file);
		//		}
		totduration += duration;
	}
	setDuration(totduration);
}

void CPlayerDlg::LoadPlaylist(CString name) {
    m_Playlist.ResetContent();
    CString errormsg;
    int r = m_mlib.loadPlaylist(name,errormsg);
	m_mlib.getPlaylist(m_Playlist);
	m_Playlist.SetCurSel(m_Playlist.GetCount()-1);
	UpdateWindow();
    if (r != 0) {
        errormsg = "There are errors in your playlist\r\n\r\n" + errormsg;
        MBMessageBox("alert", errormsg);
    }
    PlayLoop();
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
        int sel = m_Genres.GetSelectedItemFromPoint(point);
        if (sel >= 0) {
            m_Genres.SetCurSel(sel);
            OnSelchangeGenres();
            mWindowFlag = 1;
        }
    } else if (artistRect.PtInRect(point)) {
        m_Artists.SetFocus();
        int sel = m_Artists.GetSelectedItemFromPoint(point);
        if (sel >= 0) {
            m_Artists.SetCurSel(sel);
            OnSelchangeArtists();
            mWindowFlag = 2;
        }
    } else if (albumRect.PtInRect(point)) {
        m_Albums.SetFocus();
        int sel = m_Albums.GetSelectedItemFromPoint(point);
        if (sel >= 0) {
            m_Albums.SetCurSel(sel);
            OnSelchangeAlbums();
            mWindowFlag = 3;
        }
    } else if (songRect.PtInRect(point)) {
        m_Songs.SetFocus();
        int sel = m_Songs.GetSelectedItemFromPoint(point);
        if (sel >= 0) m_Songs.SetCurSel(sel);
        mWindowFlag = 4;
    } else if (playlistRect.PtInRect(point)) {
        m_Playlist.SetFocus();
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
	ModifyIDThree *dialog;
    CString genre,artist,album,title,year,track;
    CStringList genreList;
    m_mlib.getGenres(genreList);

	if (m_mlib._playlist.size() > 0) {
		MBMessageBox("Error", "You cannot edit/view song tags with a populated Playlist");
		return;
	}
	if (mWindowFlag > 4) {
		return;
	}

	Song song = new CSong;
    if (mWindowFlag == 4) {
	    int sel = m_Songs.GetCurSel();
		DWORD data = m_Songs.GetItemData(sel);
		song = m_mlib.m_SongLib.getSong(data);

//	    CString songname;
//	    m_Songs.GetText(sel, songname);
//		CString filename = m_mlib.getSongVal("FILE", _selectedGenre,
//			_selectedArtist, _selectedAlbum, songname);

		CString filename = song->getId3("FILE");
		if (!fileIsReadable(filename)) {
//			song = m_mlib.getSong(_selectedGenre, _selectedArtist,
//				_selectedAlbum, songname);
//		else {
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
//        if (_selectedGenre != MBALL) {
//            Genre_addGenre(id3, _selectedGenre);
			song->setId3(CS("TCON"), _selectedGenre);
//      }
    }

	dialog = new ModifyIDThree(&genreList, song, mWindowFlag);

	int ret;
	ret = dialog->DoModal();
	if (ret == IDOK) {
        m_mlib.modifyID3(song, dialog->m_newSong);
        initDb();
	}
	delete dialog;    	
}

#define MB_MIN_WIDTH 500
#define MB_MIN_HEIGHT 400

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

    CDialog::OnSizing(fwSide, pRect);
}

void CPlayerDlg::OnSize(UINT nType, int cx, int cy) 
{
    static int first = 0;
	CDialog::OnSize(nType, cx, cy);
    if (first < 2) {
        first++;
        return;
    }
    resizeControls();
}
void
CPlayerDlg::CurrentTitleSet(CString msg) {
	m_CurrentTitle.setText(msg);
}
void
CPlayerDlg::PlayerStatusSet(CString msg) {
    m_PlayerStatus.setText(msg);
    m_PlayerStatusSave = msg;
}
void
CPlayerDlg::PlayerStatusTempSet(CString msg) {
    m_PlayerStatus.setText(msg);
    m_PlayerStatusTime = CTime::GetCurrentTime();
	StartStatusTimer();
}
void
CPlayerDlg::PlayerStatusRevert() {

    CTime t = CTime::GetCurrentTime();
    if (t.GetTime() > (m_PlayerStatusTime.GetTime() + PLAYER_STATUS_DELAY)) {
		m_PlayerStatus.setText(m_PlayerStatusSave);
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
    CRect r;
	GetClientRect(&r);
    //GetWindowRect(&r);
	ClientToScreen(&r);

    if(r.PtInRect(point)) {
//        CRect ir;
//        ir.TopLeft().x = r.TopLeft().x+5;
//        ir.TopLeft().y = r.TopLeft().y+5;
//        ir.BottomRight().x = r.BottomRight().x-5;
//        ir.BottomRight().y = r.BottomRight().y-5;
//        if (ir.PtInRect(point))
            return HTCAPTION;
//        else
//            return HTBOTTOMRIGHT;
    }

	return CDialog::OnNcHitTest(point);

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
        popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,button.TopLeft().x,
            button.TopLeft().y, AfxGetMainWnd());
    }
}

void CPlayerDlg::OnButtonMinimize() 
{
	ShowWindow(SW_MINIMIZE);
	
}

void CPlayerDlg::OnButtonMaximize() 
{
    WINDOWPLACEMENT lpwndpl;
    BOOL r = GetWindowPlacement(&lpwndpl);
    UINT newstate;
    if (lpwndpl.showCmd == SW_SHOWMAXIMIZED) {
        newstate = SW_SHOWNORMAL;
    } else if (lpwndpl.showCmd == SW_SHOWMINIMIZED
        || lpwndpl.showCmd == SW_SHOWNORMAL) {
        newstate = SW_SHOWMAXIMIZED;
    } else {
        newstate = SW_SHOWNORMAL;
    }

	ShowWindow(newstate);
	resizeControls();
	
}

HBRUSH CPlayerDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor ) {
//    char buf[1000];
//    sprintf(buf, "nCtlColor: %u\n", nCtlColor);
//    logger.log(buf);
//    int id = GetDlgCtrlID();
//    char buf[10];
//    sprintf(buf,"getdlgid: %d", id);
//    logger.log(buf);

    COLORREF bkcolor, txcolor;
//    CBrush * br;

    switch (nCtlColor) {
    case CTLCOLOR_BTN: //   3 Button control
        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();
        break;
    case CTLCOLOR_DLG: //   4 Dialog box
        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();
        break;
    case CTLCOLOR_EDIT: //   1 Edit control
        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();
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
        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();
        break;
    default:
        bkcolor = m_Config.getColorBkPanel();
        txcolor = m_Config.getColorTxPanel();
        break;
    }

//    color = m_Config.getColorBkPanel();
    pDC->SetBkColor(bkcolor);
    pDC->SetTextColor(txcolor);
//    br = new CBrush(bkcolor);
    m_brush.DeleteObject();
    m_brush.CreateSolidBrush(bkcolor);
    return (HBRUSH)m_brush;
//    return (HBRUSH) *br;

#ifdef thisworks
    color = m_Config.getColorBkPanel();
    pDC->SetBkColor(color);
    br = new CBrush(color);
    return (HBRUSH) *br;;
#endif
}

#ifdef asdf
BOOL CTestDlg::OnInitDialog() 
{
 m_redcolor=RGB(255,0,0); // red
 m_bluecolor=RGB(0,0,255); // blue
 m_textcolor=RGB(255,255,255); // white text
 m_redbrush.CreateSolidBrush(m_redcolor);   // red background
 m_bluebrush.CreateSolidBrush(m_bluecolor); // blue background
} 

Finally do this on the ID_CTLCOLOR handle:
HBRUSH CTestDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
    HBRUSH hbr; 
    
    switch (nCtlColor) 
    { 
    case CTLCOLOR_EDIT:   
    case CTLCOLOR_MSGBOX:  
        switch (pWnd->GetDlgCtrlID())  
        {     
        case IDC_MYCONTROLNAME1: // first CEdit control ID
            // put your own CONTROL ID here    
            pDC->SetBkColor(bluecolor); // change the background color
            pDC->SetTextColor(textcolor); // change the text color
            hbr = (HBRUSH) m_bluebrush; //  apply the brush
            break;   
            
        case IDC_MYCONTROLNAME2: // second CEdit control ID
            // put your own CONTROL ID here    
            pDC->SetBkColor(redcolor); // change the background color
            pDC->SetTextColor(textcolor); // change the text color
            hbr = (HBRUSH) m_redbrush; // apply the brush
            break;   
            
            // otherwise do default handling of OnCtlColor
        default:    
            hbr=CDialog::OnCtlColor(pDC,pWnd,nCtlColor); 
            break;  
        }  
        break; 
        
        // otherwise do default handling of OnCtlColor
        default:  
            hbr=CDialog::OnCtlColor(pDC,pWnd,nCtlColor); 
    }
    
    return hbr; // return brush
}

#endif

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
        CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
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

BOOL CPlayerDlg::HelpInfo() {
    TCHAR szPath[_MAX_PATH],
      szFname[_MAX_FNAME],
      szDir[_MAX_DIR],
      szDrive[_MAX_DRIVE];

	if (0 == GetModuleFileName(NULL, szPath, sizeof(szPath))) {
		DWORD dwError = GetLastError();
		CString msg;
		msg << "HelpInfo: Error " << dwError;
		logger.log(msg);
		return FALSE;
	}

    _splitpath(szPath, szDrive, 
               szDir, szFname, NULL);
    _makepath(szPath, szDrive, szDir, 
                      MUZIKBROWZER, "chm");

//	HtmlHelp(GetDesktopWindow()->m_hWnd, 
//                 szPath,HH_DISPLAY_TOPIC, NULL);

//	HINSTANCE h = ShellExecute(this->m_hWnd, "open", "KeyHH.exe", szPath,
	HINSTANCE h = ShellExecute(this->m_hWnd, "open",  szPath, NULL,
		szDir, SW_SHOW);
	if ((int)h <= 32) {
		CString msg;
		msg << "ShellExecute on help failed. ShellExecute returned "
			<< (int)h << " on " << szPath << " " << szDir;
		logger.log(msg);
	}

    return(TRUE);
}
BOOL CPlayerDlg::OnHelpInfo(HELPINFO* pHelpInfo) {
	return HelpInfo();

}

void CPlayerDlg::OnMenuExportLibrary() {
//    m_mlib.exportLibrary();
    return;
}

void CPlayerDlg::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent) {
	case MB_PLAYLOOP_TIMER_ID:
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
void CPlayerDlg::updatePositionLabel()
{
	long pos_secs, dur_secs, pos_pct;
	m_Player->ReadMediaPosition(pos_secs, dur_secs, pos_pct);
	m_PositionSlider.SetPos(pos_pct);
	char buf[50];
	int mins = pos_secs / 60;
	int secs = pos_secs % 60;
	int dmins,dsecs;
	dmins = dur_secs / 60;
	dsecs = dur_secs % 60;
	sprintf(buf, "%d:%02d %d:%02d", mins, secs, dmins, dsecs);
	m_PositionLabel.setText ( buf );
}
void CPlayerDlg::adjustPosition() {
	m_Player->Seek(m_PositionSlider.GetPos(), TRUE);
	updatePositionLabel();
}
void CPlayerDlg::adjustPosition(int pct) {
	m_Player->Seek(pct, TRUE);
	updatePositionLabel();
}
void CPlayerDlg::OnFastForward() {
	adjustPosition(m_PositionSlider.GetPos() + 5);
}
void CPlayerDlg::OnReverse() {
	adjustPosition(m_PositionSlider.GetPos() - 5);
}

void CPlayerDlg::updateVolumeLabel() {
	char buf[10];
	sprintf(buf,"%d", m_VolumeSlider.GetPos());
	m_VolumeLabel.setText(buf);
}
void CPlayerDlg::adjustVolume() {
	if (m_Player->SetVolume(m_VolumeSlider.GetPos())) {
		updateVolumeLabel();
	} else {
		MBMessageBox("Error", "Unable to SetVolume");
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
void CPlayerDlg::Pause() {
	m_Player->Pause();
	StopSeekTimer();
}

//# define M_E        2.7182818284590452354 
//#include <math.h>
//TEST(LOGT,LOGT)
//{
//    double volume;
//    double dcb;
//	char buf[100];
//	CString msg;
//    for (volume = 0 ; volume < 51 ; ++volume) {
//        if (volume > 51.0) {
//			dcb = 0;
//		} else if (volume <= 1) {
//			dcb = -10000;
//		} else {
//			dcb = ((log(volume) / log(50)) * 10000) - 10000;
//		}
//
//		double v2;
//		double tmp = log(50) * ((10000 + dcb) / 10000);
//		v2 = pow(M_E, tmp);
//        sprintf(buf,"%.3f %.3f %.3f", volume, dcb,v2);
//		msg += buf;
//		msg += "\r\n";
//    }
//	MBMessageBox("log", msg);
//}

void CPlayerDlg::killAlbumArt() {
	m_Picture.unload();
}
void CPlayerDlg::displayAlbumArt(const CString & file) {

	killAlbumArt();
	size_t size = 0;
	uchar * data = NULL;
	BOOL pic = m_mlib.apic(file, data, size);


	if (pic) {
		m_Picture.load(data,size);
	} else {
//		m_Picture.blank();
	}

}



