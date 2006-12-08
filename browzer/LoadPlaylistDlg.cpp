// LoadPlaylistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadPlaylistDlg.h"
#include "PlayerDlg.h"
#include "MBGlobals.h"
#include "VirtualControl.h"
#include "SkinDefs.h"
#include "MBMessageBox.h"
#include "GetTextField.h"
#include "MyLog.h"
#include "Misc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

LoadPlaylistDlg * theLoader;
void Need2EraseChooser(BOOL b) {
	theLoader->m_Need2Erase = b;
}
CDialog * getChooser() {
	return theLoader;
}
LoadPlaylistDlg * getLoader() {
	return theLoader;
}
/////////////////////////////////////////////////////////////////////////////
// LoadPlaylistDlg dialog


LoadPlaylistDlg::LoadPlaylistDlg(PlayerCallbacks * pcb,
				CWnd* pParent /*=NULL*/, BOOL editor)
	: CDialogSK(LoadPlaylistDlg::IDD, pParent), 
	m_parentcallbacks(pcb),
    m_PlaylistSongs(TRUE, "songs", FALSE, &m_callbacks), 
	m_PlaylistNames(TRUE, "names", FALSE, &m_callbacks),
	m_Control(new VirtualControl), 
	m_IsEditor(editor),
	m_reg(pcb->mbconfig()->getSkin(MB_SKIN_DEF))
{
	//{{AFX_DATA_INIT(LoadPlaylistDlg)
	//}}AFX_DATA_INIT
	m_bPlayer = FALSE;
	m_Resizing = FALSE;	
	m_AdjustLibrary = FALSE;
	m_LibraryDragging = FALSE;
	m_Modified = FALSE;
	theLoader = this;

	m_callbacks.Need2Erase = &::Need2EraseChooser;
	m_callbacks.mbconfig = m_parentcallbacks->mbconfig;
	m_callbacks.dlg = &::getChooser;
	m_callbacks.lpdlg = &::getLoader;

	MBCONFIG_READ_SKIN_DEFS_POBJ(m_parentcallbacks->mbconfig(),m_reg);
}
LoadPlaylistDlg::~LoadPlaylistDlg() {
	delete m_Control;

	POSITION pos;
	HBRUSH hbr;
	CString key;
	for( pos = m_CtlColors.GetStartPosition(); pos != NULL; ) {
		m_CtlColors.GetNextAssoc( pos, key, (void*&)hbr);
		BOOL r = ::DeleteObject((HBRUSH)hbr);
	}
}


void LoadPlaylistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogSK::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoadPlaylistDlg)
	DDX_Control(pDX, IDC_PLAYLIST_CLOSE, m_Close);
	DDX_Control(pDX, IDC_LPD_TITLE, m_TitleLabel);
	DDX_Control(pDX, IDC_PLAYLIST_DELETE, m_Delete);
	DDX_Control(pDX, IDC_PLAYLIST_LOAD, m_Load);
	DDX_Control(pDX, IDCANCEL, m_Exit);
	DDX_Control(pDX, IDC_PLAYLIST_UP, m_Up);
	DDX_Control(pDX, IDC_PLAYLIST_DOWN, m_Down);
	DDX_Control(pDX, IDC_PLAYLIST_SAVE, m_Save);
	DDX_Control(pDX, IDC_PLAYLIST_RENAME, m_Rename);
	DDX_Control(pDX, IDC_GAAS, m_GAAS);
	DDX_Control(pDX, IDC_LPD_NAMES_LABEL, m_NamesLabel);
	DDX_Control(pDX, IDC_LPD_SONGS_LABEL, m_SongsLabel);
	DDX_Control(pDX, IDC_PLAYLIST_SONGS, m_PlaylistSongs);
	DDX_Control(pDX, IDC_PLAYLIST_NAMES, m_PlaylistNames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LoadPlaylistDlg, CDialogSK)
	//{{AFX_MSG_MAP(LoadPlaylistDlg)
	ON_WM_CTLCOLOR()                     
	ON_WM_LBUTTONUP()                    
	ON_WM_MOUSEMOVE()                    
	ON_WM_NCHITTEST()                    
	ON_WM_NCLBUTTONDOWN()                
	ON_WM_NCLBUTTONUP()                  
	ON_WM_NCMOUSEMOVE()                  
	ON_WM_SETCURSOR()                    
	ON_WM_SIZE()                         
	ON_WM_SIZING()                       
	ON_BN_CLICKED(IDC_PLAYLIST_CLOSE,    OnCancel)
	ON_LBN_DBLCLK(IDC_PLAYLIST_NAMES,    OnDblclkPlaylistNames)
	ON_BN_CLICKED(IDC_PLAYLIST_DELETE,   OnDeletePlaylist)
	ON_LBN_KILLFOCUS(IDC_PLAYLIST_NAMES, OnKillfocusPlaylistNames)
	ON_LBN_KILLFOCUS(IDC_PLAYLIST_SONGS, OnKillfocusPlaylistSongs)
	ON_MESSAGE(MB_LISTMOVEDN,            OnMovePlaylistDn)
	ON_MESSAGE(MB_LISTMOVEUP,            OnMovePlaylistUp)
	ON_BN_CLICKED(IDC_PLAYLIST_DOWN,     OnPlaylistDown)
	ON_BN_CLICKED(IDC_PLAYLIST_LOAD,     OnPlaylistOk)
	ON_BN_CLICKED(IDC_PLAYLIST_SAVE,     OnPlaylistSave)
	ON_BN_CLICKED(IDC_PLAYLIST_UP,       OnPlaylistUp)
	ON_BN_CLICKED(IDC_PLAYLIST_RENAME,   OnRenamePlaylist)
	ON_LBN_SELCHANGE(IDC_PLAYLIST_NAMES, OnSelchangePlaylistNames)
	ON_LBN_SELCHANGE(IDC_PLAYLIST_SONGS, OnSelchangePlaylistSongs)
	ON_LBN_SETFOCUS(IDC_PLAYLIST_NAMES,  OnSetfocusPlaylistNames)
	ON_LBN_SETFOCUS(IDC_PLAYLIST_SONGS,  OnSetfocusPlaylistSongs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LoadPlaylistDlg message handlers
BOOL LoadPlaylistDlg::OnInitDialog() 
{

	m_Need2Erase = TRUE;
	CDialogSK::OnInitDialog();
	m_LastDragRect = CRect(0,0,0,0);

	if (m_IsEditor) { 
		m_TitleLabel.ShowWindow(SW_HIDE);
		m_Close.ShowWindow(SW_HIDE);
		ModifyStyle(0,WS_CAPTION | WS_SYSMENU,0);
	} else {
		ModifyStyle(WS_CAPTION,0,0); // delete the title bar
	}

//	InitGrip();
//	ShowSizeGrip(TRUE);

	int iwidth,iheight;
	m_parentcallbacks->mbconfig()->getRegistry("LoadPlaylistWidth", iwidth, MB_MIN_WIDTH);
	m_parentcallbacks->mbconfig()->getRegistry("LoadPlaylistHeight", iheight, MB_MIN_HEIGHT);

	MBCONFIG_READ_TRANS_COLORS(m_reg,m_crTransMain,m_crTransPanel);
	SetTransparentColor(m_crTransMain,m_crTransPanel); 

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
	crColHdrFg = m_parentcallbacks->mbconfig()->getColorTxColHdr();
	crColHdrBg = m_parentcallbacks->mbconfig()->getColorBkColHdr();

	m_PlaylistSongs.ResetContent();
	m_PlaylistNames.ResetContent();

	m_GAAS.setDesc("gaas");
//	m_GAAS.SetWordWrap(TRUE);
	m_GAAS.SetTicking(TRUE);

	m_Delete.SetDefaultColors(TRUE);
	m_Load.SetDefaultColors(TRUE);
	m_Exit.SetDefaultColors(TRUE);
	m_Up.SetDefaultColors(TRUE);
	m_Down.SetDefaultColors(TRUE);
	m_Save.SetDefaultColors(TRUE);
	m_Rename.SetDefaultColors(TRUE);

	m_Delete.SetFlat(FALSE);
	m_Load.SetFlat(FALSE);
	m_Exit.SetFlat(FALSE);
	m_Up.SetFlat(FALSE);
	m_Down.SetFlat(FALSE);
	m_Save.SetFlat(FALSE);
	m_Rename.SetFlat(FALSE);

	m_Delete.DrawBorder(TRUE);
	m_Load.DrawBorder(TRUE);
	m_Exit.DrawBorder(TRUE);
	m_Up.DrawBorder(TRUE);
	m_Down.DrawBorder(TRUE);
	m_Save.DrawBorder(TRUE);
	m_Rename.DrawBorder(TRUE);

	//m_Load.EnableBalloonTooltip();

	m_Delete.SetTooltipText("Delete selected playlist");
	m_Load.SetTooltipText("Load selected playlist and play");
	m_Exit.SetTooltipText("Exit");
	m_Up.SetTooltipText("Move selected song up in playlist");
	m_Down.SetTooltipText("Move selected song down in playlist");
	m_Save.SetTooltipText("Save playlist");
	m_Rename.SetTooltipText("Rename playlist");
	
	CDC * cdc = GetDC();

	m_TitleLabel.setText("Playlist Editor ");
	if (m_IsEditor) { 
		m_PlaylistNames.SetBitmaps(cdc);
		m_PlaylistSongs.SetBitmaps(cdc);
		m_PlaylistNames.Set3d(TRUE);
		m_PlaylistSongs.Set3d(TRUE);
		m_NamesLabel.initFont();
		m_SongsLabel.initFont();
	} else {
		m_TitleLabel.setText("Playlist Loader ");
		m_TitleLabel.SetColors(crOtherFg,crOtherBg);
//		m_TitleLabel.SetTransparent(cdc, m_crTransPanel);

		m_Delete.SetFlat(OtherFlat);
		m_Load.SetFlat(OtherFlat);
		m_Exit.SetFlat(OtherFlat);
		m_Up.SetFlat(OtherFlat);
		m_Down.SetFlat(OtherFlat);
		m_Save.SetFlat(OtherFlat);
		m_Rename.SetFlat(OtherFlat);

		m_Delete.DrawBorder(OtherBorder);
		m_Load.DrawBorder(OtherBorder);
		m_Exit.DrawBorder(OtherBorder);
		m_Up.DrawBorder(OtherBorder);
		m_Down.DrawBorder(OtherBorder);
		m_Save.DrawBorder(OtherBorder);
		m_Rename.DrawBorder(OtherBorder);

		m_Delete.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);
		m_Load.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);
		m_Exit.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);
		m_Up.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);
		m_Down.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);
		m_Save.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);
		m_Rename.SetColors(crButtonsFgHover,crButtonsBgHover,crButtonsFgOut,crButtonsBgOut,crButtonsFgIn,crButtonsBgIn);

		m_PlaylistSongs.initFont();
		m_PlaylistNames.initFont();
		m_NamesLabel.initFont();
		m_SongsLabel.initFont();
		LPLOGFONT lplf = m_parentcallbacks->mbconfig()->getTitlesFont();
		m_PlaylistSongs.changeFont(lplf);
		m_PlaylistNames.changeFont(lplf);

		lplf = m_parentcallbacks->mbconfig()->getColHdrFont();
		m_NamesLabel.changeFont(lplf);
		m_SongsLabel.changeFont(lplf);

		COLORREF crColHdrFg,crColHdrBg;
		crColHdrFg = m_parentcallbacks->mbconfig()->getColorTxColHdr();
		crColHdrBg = m_parentcallbacks->mbconfig()->getColorBkColHdr();

#define _LABEL_COLORS_ crColHdrFg,crColHdrBg,\
	crColHdrInUL,crColHdrInLR,crColHdrOutUL,crColHdrOutLR,\
	threeDColHdrs

		m_NamesLabel.SetColors(_LABEL_COLORS_);
		m_SongsLabel.SetColors(_LABEL_COLORS_);
		m_GAAS.SetColors(crColHdrFg,crColHdrBg,
			crColHdrInUL,crColHdrInLR,crColHdrOutUL,crColHdrOutLR,
			threeDStatus);

		m_PlaylistSongs.SetColors(m_parentcallbacks->mbconfig()->getColorBkNormal(),
			m_parentcallbacks->mbconfig()->getColorBkHigh(),
			m_parentcallbacks->mbconfig()->getColorBkSel(),
			m_parentcallbacks->mbconfig()->getColorTxNormal(),
			m_parentcallbacks->mbconfig()->getColorTxHigh(),
			m_parentcallbacks->mbconfig()->getColorTxSel(),
			threeDDataWindows,crDataInUL,crDataInLR,crDataOutUL,crDataOutLR);

		m_PlaylistNames.SetColors(m_parentcallbacks->mbconfig()->getColorBkNormal(),
			m_parentcallbacks->mbconfig()->getColorBkHigh(),
			m_parentcallbacks->mbconfig()->getColorBkSel(),
			m_parentcallbacks->mbconfig()->getColorTxNormal(),
			m_parentcallbacks->mbconfig()->getColorTxHigh(),
			m_parentcallbacks->mbconfig()->getColorTxSel(),
			threeDDataWindows,crDataInUL,crDataInLR,crDataOutUL,crDataOutLR);

		m_PlaylistNames.SetBitmaps(cdc, 
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_SCROLLUPARROW),
				m_crTransPanel,
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_SCROLLDOWNARROW),
				m_crTransPanel,
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_SCROLLBUTTON),
				m_crTransPanel,
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_SCROLLBACKGROUND),
				m_crTransPanel);
		
		m_PlaylistSongs.SetBitmaps(cdc, 
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_SCROLLUPARROW),
				m_crTransPanel,
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_SCROLLDOWNARROW),
				m_crTransPanel,
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_SCROLLBUTTON),
				m_crTransPanel,
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_SCROLLBACKGROUND),
				m_crTransPanel);

		m_Close.SetBitmaps(
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_BUTTONEXITIN),	
				m_crTransPanel,	
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_BUTTONEXITHOVER),	
				m_crTransPanel,	
			m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_BUTTONEXITOUT),		
				m_crTransPanel);

	}
	ReleaseDC(cdc);
	
	m_NamesLabel.setText("Playlists");
	m_SongsLabel.setText("Songs in playlist");


	if (!m_IsEditor) { // chooser only, no editing
		m_Delete.ShowWindow(SW_HIDE);
		m_Up.ShowWindow(SW_HIDE);
		m_Down.ShowWindow(SW_HIDE);
		m_Save.ShowWindow(SW_HIDE);
		m_Rename.ShowWindow(SW_HIDE);
		SetWindowText("Choose Playlist to Play");
	} else { // editor
		//m_Load.ShowWindow(SW_HIDE);
		SetWindowText("Playlist Editor");
	}

	m_HatchBrush.CreateHatchBrush(HS_CROSS,RGB(255,0,0));

	CRect rect;
	GetWindowRect(rect);
	rect.right = rect.left + iwidth;
	rect.bottom = rect.top + iheight;
	MoveWindow(rect);
	m_Save.EnableWindow(FALSE);
	resetControls();

    init();
	m_PlaylistNames.invalidate();

	m_PlaylistSongs.m_reorder = TRUE;
	m_PlaylistNames.SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void LoadPlaylistDlg::OnDblclkPlaylistNames() 
{
	CWaitCursor c;
    int sel = m_PlaylistNames.GetCurSel();
    if (sel < 0) return;
    CString name;
    m_PlaylistNames.GetText(sel, name);
    m_parentcallbacks->playerdlg()->LoadPlaylist(name);	
    EndDialog(0);
}

void LoadPlaylistDlg::OnSelchangePlaylistNames() 
{
    UpdateWindow();
	CWaitCursor c;
	m_PlaylistSongs.ResetContent();
    int sel = m_PlaylistNames.GetCurSel();
    if (sel < 0) return;
    CString name;
    m_PlaylistNames.GetText(sel, name);
	BOOL reload = TRUE;
	if (name != m_LastName) {
		if (m_Modified) {
			CString tmp = "You have made changes to your \"";
			tmp += m_LastName + "\" playlist.\r\nDo you wish to first save it?";
			int ok = MBMessageBox("Confirmation", tmp, FALSE, TRUE);
			if (0 != ok) {
				reload = FALSE;
			} else {
				// reset name so that they see that 
				// they discarded some changesto it
				name = m_LastName;
			}
		}
		if (TRUE == reload) { // Answer was no (Cancel)
			m_csaPlaylistDesc.RemoveAll();
			m_csaPlaylist.RemoveAll();
			m_cwaTlen.RemoveAll();
			m_parentcallbacks->musiclib()->getSongsInPlaylist(name,m_csaPlaylistDesc,m_csaPlaylist,
				m_cwaTlen);
			m_Modified = FALSE;
			m_Save.EnableWindow(FALSE);
			sel = m_PlaylistNames.SelectString(-1,name); // -1 for all
			m_PlaylistNames.SetCurSel(sel);
//			logger.ods(CString("LPD cursel " + numToString(sel)));
		} else {  // Answer was yes, (Ok)
			name = m_LastName;
			sel = m_PlaylistNames.SelectString(-1,name); // -1 for all
			m_PlaylistNames.SetCurSel(sel);
//			logger.ods(CString("LPD cursel " + numToString(sel)));
		}
	}
	int pos,size,total,duration;
	total = 0;
	size = m_csaPlaylistDesc.GetSize();
	for (pos = 0; pos < size; pos++ ) {
		m_PlaylistSongs.AddString(m_csaPlaylistDesc.GetAt(pos));
		int millis = m_cwaTlen.GetAt(pos);
		if (millis > 999)
			duration = millis / 1000;
		else duration = millis;
		total += duration;
	}
	m_PlaylistNames.invalidate();
	m_PlaylistSongs.invalidate();
//	if (m_IsEditor)
//		m_GAAS.SetWindowText(name);
//	else 
		m_GAAS.setText(name);
	UpdateWindow();
	m_LastName = name;

	AutoBuf buf(100);
	int h,m,s;
	MBUtil::SecsToHMS(total,h,m,s,buf.p);
	CString txt = "Songs";
	if (size) {
		txt += ": ";
		txt += numToString(size)
		+ ", duration: " + CS(buf.p);
	}

	m_SongsLabel.setText(txt);

}
void LoadPlaylistDlg::OnSelchangePlaylistSongs() 
{
    int sel = m_PlaylistSongs.GetCurSel();
	if (sel < 0) return;
    CString name;
    m_PlaylistSongs.GetText(sel,name);
// 	if (m_IsEditor)
//		m_GAAS.SetWindowText(name);
//	else 
		m_GAAS.setText(name);
	m_PlaylistSongs.invalidate();
}
void LoadPlaylistDlg::OnKillfocusPlaylistNames() 
{
	m_PlaylistNames.invalidate();
}
void LoadPlaylistDlg::OnKillfocusPlaylistSongs() 
{
	m_PlaylistSongs.invalidate();
}

void LoadPlaylistDlg::OnSetfocusPlaylistNames() 
{
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_PLAYLIST_NAMES);
	int sel = m_PlaylistNames.GetCurSel();
	CString name;
	if (sel > -1)
		m_PlaylistNames.GetText(sel,name);
// 	if (m_IsEditor)
//		m_GAAS.SetWindowText(name);
//	else 
		m_GAAS.setText(name);
    m_Load.EnableWindow(TRUE);
    m_Delete.EnableWindow(TRUE);
	m_Up.EnableWindow(FALSE);
	m_Down.EnableWindow(FALSE);
	m_Delete.SetTooltipText("Delete selected playlist");
	//OnSelchangePlaylistNames();
	m_PlaylistNames.invalidate();
	m_PlaylistNames.SetFocus();
}

void LoadPlaylistDlg::OnSetfocusPlaylistSongs() 
{
    m_Load.EnableWindow(TRUE);
    //m_Delete.EnableWindow(FALSE);
	m_Up.EnableWindow(TRUE);
	m_Down.EnableWindow(TRUE);

	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_PLAYLIST_SONGS);
//    m_PlaylistSongs.SetCurSel(0);
	OnSelchangePlaylistSongs();
	m_Delete.SetTooltipText("Delete selected song from playlist");
	m_PlaylistSongs.SetCurSel(0);
	m_PlaylistSongs.invalidate();
}


void
LoadPlaylistDlg::init() {
	CWaitCursor c;
	m_PlaylistNames.ResetContent();
    m_parentcallbacks->musiclib()->getPlaylistNames(m_PlaylistNames);
    m_PlaylistNames.SetCurSel(0);
    OnSelchangePlaylistNames();
	m_PlaylistSongs.SetCurSel(0);
}

void LoadPlaylistDlg::OnControlSelChange() {
    if (m_Control->ptr() == &m_PlaylistNames) {
        OnSelchangePlaylistNames();
    } else if (m_Control->ptr() == &m_PlaylistSongs) {
        OnSelchangePlaylistSongs();
    }
}
void LoadPlaylistDlg::OnControlClick() {
    if (m_Control->ptr() == &m_PlaylistNames) {
        OnDblclkPlaylistNames();
    }
}
void LoadPlaylistDlg::OnDelete() {}

void LoadPlaylistDlg::OnMenuButton() {}

void
LoadPlaylistDlg::OnPageUp() {
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
LoadPlaylistDlg::OnPageDown() {
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

void
LoadPlaylistDlg::OnExit() {
	if (m_Modified) {
		int sel = m_PlaylistNames.GetCurSel();
		if (sel < 0) return;
		CString name;
		m_PlaylistNames.GetText(sel, name);
		CString tmp = "You have made changes to your \"";
		tmp += name + "\" playlist.\r\nDo you wish to first save it?";
		int ok = MBMessageBox("Confirmation", tmp, FALSE, TRUE);
		if (0 != ok) { // OK, let them save it.
			return;
		}
	}
    EndDialog(0);
}

void LoadPlaylistDlg::OnCancel() 
{
	if (m_Modified) {
		int sel = m_PlaylistNames.GetCurSel();
		if (sel < 0) return;
		CString name;
		m_PlaylistNames.GetText(sel, name);
		CString tmp = "You have made changes to your \"";
		tmp += name + "\" playlist.\r\nDo you wish to first save it?";
		int ok = MBMessageBox("Confirmation", tmp, FALSE, TRUE);
		if (0 != ok) { // OK, let them save it.
			return;
		}
	}
    EndDialog(0);	
}

void LoadPlaylistDlg::OnPlaylistOk() 
{
	if (m_Modified) {
		int sel = m_PlaylistNames.GetCurSel();
		if (sel < 0) return;
		CString name;
		m_PlaylistNames.GetText(sel, name);
		CString tmp = "You have made changes to your \"";
		tmp += name + "\" playlist.\r\nDo you wish to first save it?";
		int ok = MBMessageBox("Confirmation", tmp, FALSE, TRUE);
		if (0 != ok) { // OK, let them save it.
			return;
		}
	}
	OnDblclkPlaylistNames();
}

void LoadPlaylistDlg::OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
    
    int sel = m_PlaylistSongs.GetCurSel();

    CString name;
    m_PlaylistSongs.GetText(sel, name);

    if (pNMUpDown->iDelta == -1) {
        if (sel < 1) return;
        m_PlaylistSongs.DeleteString(sel);
        m_PlaylistSongs.InsertString(sel-1, (LPCTSTR)name);
        m_PlaylistSongs.SetCurSel(sel-1);
    } else if (pNMUpDown->iDelta == 1) {
        if (sel > m_PlaylistSongs.GetCount()-2) return;
        m_PlaylistSongs.DeleteString(sel);
        m_PlaylistSongs.InsertString(sel+1, (LPCTSTR)name);
        m_PlaylistSongs.SetCurSel(sel+1);
    }

    UpdateWindow();
	
	*pResult = 0;
}



void LoadPlaylistDlg::OnSizing(UINT fwSide, LPRECT pRect) 
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

//	if (FALSE == m_Resizing) {
		CDialogSK::OnSizing(fwSide, pRect);
//	}
	m_Resizing = TRUE;	
//	UpdateGripPos();
}
void LoadPlaylistDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialogSK::OnSize(nType, cx, cy);
	m_Need2Erase = TRUE;
	resetControls();
	UpdateWindow();
//	UpdateGripPos();
}
void LoadPlaylistDlg::resetControls() {
//	int counter=0;
//	logger.ods("resetControls "+numToString(counter++));
	if (!IsWindow(m_hWnd)) return;
	if (!IsWindow(m_PlaylistNames.m_hWnd)) return;

	CRect titlerect,closerect,clientrect,namesrect,songsrect,
		deleterect,loadrect,gaasrect,exitrect,
		nameslabelrect,songslabelrect,
		uprect,downrect,saverect,wrect,renamerect;
	GetClientRect(clientrect);
	GetWindowRect(wrect);
	m_SysBarHeight = wrect.Height() - clientrect.Height();
	ScreenToClient(wrect);
	m_Delete.GetClientRect(deleterect);
	m_Load.GetClientRect(loadrect);
	m_GAAS.GetClientRect(gaasrect);
	m_Exit.GetClientRect(exitrect);
	m_Up.GetClientRect(uprect);
	m_Down.GetClientRect(downrect);
	m_Save.GetClientRect(saverect);
	m_Rename.GetClientRect(renamerect);
	m_TitleLabel.GetClientRect(titlerect);
	m_Close.GetClientRect(closerect);
	m_Close.SetAlign(CButtonST::ST_ALIGN_OVERLAP,TRUE);
	m_Close.DrawTransparent(TRUE);

	int border = m_parentcallbacks->mbconfig()->getDlgBorderWidth();
	int borderpanel = m_reg.Read("BorderPanel",5);
	int borderhorz = m_parentcallbacks->mbconfig()->getDlgBorderHorz();
	int bordervert = m_parentcallbacks->mbconfig()->getDlgBorderVert();
	
	CRectMove(closerect,clientrect.Width() - (closerect.Width() + border),
		border);
	
	CString txt; m_TitleLabel.getText(txt);
	CSize ts = m_TitleLabel.GetSize(txt);txt += "xx";
	titlerect.bottom = titlerect.top + ts.cy + 2;
	titlerect.right = titlerect.left + ts.cx + 4;

	if (m_IsEditor) { 
//		CPoint tp((wrect.Width() / 2) - (titlerect.Width() / 2),wrect.top+borderpanel);
//		titlerect.OffsetRect(tp);
		clientrect.top += bordervert;
	} else {
		clientrect.DeflateRect(border,border,border,border);
		CPoint tp((wrect.Width() / 2) - (titlerect.Width() / 2),wrect.top+borderpanel);
		titlerect.OffsetRect(tp);
		clientrect.top = titlerect.bottom + bordervert;

		LayOutStyle BackgroundMainType,BackgroundPanelType;
		MBCONFIG_READ_BACKGROUND_TYPES(m_reg,BackgroundMainType,BackgroundPanelType);
	
		wrect.OffsetRect(3,3);
	#pragma hack		
		// hack alert: rect0 needs 2 b Window instead of client
		// for the resizing border
		FreeResources();
		SetBitmap(m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_BACKGROUNDMAIN), 
			wrect, BackgroundMainType, CS("frame"));
		SetBitmap(m_parentcallbacks->mbconfig()->getSkin(MB_SKIN_BACKGROUNDPLAYLIST),
			clientrect, BackgroundPanelType, CS("play panel"));
		CDC * cdc = GetDC();
		make(cdc);
		ReleaseDC(cdc);
		OnNcPaint() ;
		EraseBkgndNC();//(cdc); // force CDialogSK to paint background
		// This sends a WM_NCPAINT to repaint the resize frame
		RedrawWindow(NULL,NULL, RDW_FRAME|RDW_INVALIDATE);
		
		// reset trans, this tells the ColorStatic to grab a new bg
		//cdc = GetDC();
		CRect trect;
		m_TitleLabel.GetWindowRect(trect);
		STC(trect);
		m_TitleLabel.SetTransparent(m_bmBackground, trect, m_crTransPanel);

		clientrect.DeflateRect(borderpanel,borderpanel,borderpanel,borderpanel);
	}
//	logger.ods("resetControls "+numToString(counter++));



	int pct;// = int((double)100 * ((double)width / (double)dw));
	int iwidth,iheight;
	iwidth = wrect.Width();
	iheight = wrect.Height();
	m_parentcallbacks->mbconfig()->getRegistry("LoadPlaylistNamesWidthPct", pct, 25);
	m_parentcallbacks->mbconfig()->setRegistry("LoadPlaylistWidth", iwidth);
	m_parentcallbacks->mbconfig()->setRegistry("LoadPlaylistHeight", iheight);
	double lpct = (double) pct / (double)100;

	int buttonheight = loadrect.Height();
	int buttonwidth = loadrect.Width();
	loadrect.top = clientrect.bottom - buttonheight;
	loadrect.bottom = loadrect.top + buttonheight;
	loadrect.left = clientrect.left;
	loadrect.right = loadrect.left + buttonwidth;

	exitrect = loadrect;
	exitrect.left = loadrect.right + borderhorz;
	exitrect.right = exitrect.left + buttonwidth;

	renamerect = exitrect;
	renamerect.left = exitrect.right + borderhorz;
	renamerect.right = renamerect.left + buttonwidth;
	
	deleterect = renamerect;
	deleterect.left = renamerect.right + borderhorz;
	deleterect.right = deleterect.left + buttonwidth;

	uprect = loadrect;
	uprect.left = deleterect.right + borderhorz;
	uprect.right = uprect.left + buttonwidth;

	downrect = loadrect;
	downrect.left = uprect.right + borderhorz;
	downrect.right = downrect.left + buttonwidth;

	saverect = loadrect;
	saverect.left = downrect.right + borderhorz;
	saverect.right = saverect.left + buttonwidth;

	m_MinNamesWidth = loadrect.Width() + borderhorz + exitrect.Width();
	m_MaxNamesWidth = clientrect.Width() - 50;

	m_Load.MoveWindow(loadrect);
	m_Delete.MoveWindow(deleterect);
	m_Exit.MoveWindow(exitrect);
	m_Up.MoveWindow(uprect);
	m_Down.MoveWindow(downrect);
	m_Save.MoveWindow(saverect);
	m_Rename.MoveWindow(renamerect);
	m_TitleLabel.MoveWindow(titlerect);
	m_Close.MoveWindow(closerect);

	int gaasheight = gaasrect.Height();
	//gaasheight = 30;
	gaasrect.left = clientrect.left ;
	gaasrect.right = clientrect.right;
	gaasrect.bottom = loadrect.top - bordervert;
	gaasrect.top = gaasrect.bottom - gaasheight;
	m_GAAS.MoveWindow(gaasrect);

	CSize s = m_NamesLabel.GetSize("Playlists");
	nameslabelrect.top = clientrect.top;
	nameslabelrect.bottom = nameslabelrect.top + s.cy + 4;
	nameslabelrect.left = clientrect.left;
	nameslabelrect.right = (int)( (double)clientrect.Width() * lpct);
	if (nameslabelrect.Width() < m_MinNamesWidth) 
		nameslabelrect.right = nameslabelrect.left + m_MinNamesWidth;
	if (nameslabelrect.Width() > m_MaxNamesWidth) 
		nameslabelrect.right = nameslabelrect.left + m_MaxNamesWidth;

	songslabelrect = nameslabelrect;
	songslabelrect.left = nameslabelrect.right + borderhorz;
	songslabelrect.right = clientrect.right;// - 10;

	m_NamesLabel.MoveWindow(nameslabelrect);
	m_SongsLabel.MoveWindow(songslabelrect);

	namesrect.top = nameslabelrect.bottom;
	namesrect.bottom = gaasrect.top - bordervert;
	namesrect.left = nameslabelrect.left;
	namesrect.right = nameslabelrect.right;

	songsrect = namesrect;
	songsrect.left = namesrect.right + borderhorz;
	songsrect.right = clientrect.right;// - 10;

	if (IsWindow(m_PlaylistNames.m_hWnd)) {
		m_PlaylistNames.MoveWindow(namesrect);
		m_PlaylistNames.RedrawWindow();
	}
	if (IsWindow(m_PlaylistSongs.m_hWnd)) {
		m_PlaylistSongs.MoveWindow(songsrect);
		m_PlaylistSongs.RedrawWindow();
	}

	m_AdjustRect = nameslabelrect;
	m_AdjustRect.left = nameslabelrect.right;
	m_AdjustRect.right = songslabelrect.left;
	m_AdjustRect.bottom += 20;
	m_AdjustInt.UnionRect(nameslabelrect,namesrect);

	m_Need2Erase = TRUE;
	m_PlaylistNames.initBgDc();	
	m_PlaylistSongs.initBgDc();	

	m_PlaylistNames.invalidate();	
	m_PlaylistSongs.invalidate();	

	m_PlaylistNames.RedrawWindow();	
	m_PlaylistSongs.RedrawWindow();	
	UpdateWindow();
	POSITION pos;
	HBRUSH hbr;
	CString key;
	for( pos = m_CtlColors.GetStartPosition(); pos != NULL; ) {
		m_CtlColors.GetNextAssoc( pos, key, (void*&)hbr);
		BOOL r = ::DeleteObject((HBRUSH)hbr);
	}
	m_CtlColors.RemoveAll();
}

HBRUSH LoadPlaylistDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor ) {

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (m_IsEditor)
		return (HBRUSH)hbr;

    COLORREF bkcolor, txcolor;
    switch (nCtlColor) {
    case CTLCOLOR_BTN: //   3 Button control
		return hbr;

        bkcolor = m_parentcallbacks->mbconfig()->getColorBkPanel();
        txcolor = m_parentcallbacks->mbconfig()->getColorTxPanel();

        break;
    case CTLCOLOR_DLG: //   4 Dialog box

		bkcolor = m_parentcallbacks->mbconfig()->getColorBkColHdr();
		txcolor = m_parentcallbacks->mbconfig()->getColorTxColHdr();
        break;
    case CTLCOLOR_EDIT: //   1 Edit control
        bkcolor = m_parentcallbacks->mbconfig()->getColorBkPanel();
        txcolor = m_parentcallbacks->mbconfig()->getColorTxPanel();
        break;
    case CTLCOLOR_LISTBOX: //  2 List-box control
        bkcolor = m_parentcallbacks->mbconfig()->getColorBkNormal();
        txcolor = m_parentcallbacks->mbconfig()->getColorTxPanel();
        break;
    case CTLCOLOR_MSGBOX: //   0 Message box
        bkcolor = m_parentcallbacks->mbconfig()->getColorBkPanel();
        txcolor = m_parentcallbacks->mbconfig()->getColorTxPanel();
        break;
    case CTLCOLOR_SCROLLBAR: //  5 Scroll-bar control
        bkcolor = m_parentcallbacks->mbconfig()->getColorBkPanel();
        txcolor = m_parentcallbacks->mbconfig()->getColorTxPanel();
        break;
    case CTLCOLOR_STATIC: //  6 Static control
        bkcolor = m_parentcallbacks->mbconfig()->getColorBkPanel();
        txcolor = m_parentcallbacks->mbconfig()->getColorTxPanel();
		break;
    default:
        bkcolor = m_parentcallbacks->mbconfig()->getColorBkPanel();
        txcolor = m_parentcallbacks->mbconfig()->getColorTxPanel();
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





UINT
LoadPlaylistDlg::OnNcHitTest( CPoint point ) {

    CRect r;//,applabelr; //,positionr,volumer;
	GetClientRect(&r);
	CRect br = r;
	br.TopLeft() = CPoint(br.BottomRight().x-20, br.BottomRight().y-20);
	CPoint cpoint(point);
	ScreenToClient(&cpoint);
	CRect ur = br;
	CRectMove(ur,r.BottomRight().x-20,r.TopLeft().y+20);
	
	
	ClientToScreen(&r);
	ClientToScreen(&br);
	ClientToScreen(&ur);

	UINT rv = CDialogSK::OnNcHitTest(point);
	
    if(br.PtInRect(point)) {
		return HTBOTTOMRIGHT;
	} else if (m_AdjustRect.PtInRect(cpoint)) {
		return HTSIZE;
	} else {
		return rv;
	} //else if (ur.PtInRect(point)) {
//		return HTCLOSE;
//    } else {
//		return HTCAPTION;
//    } 

	return rv;
}
BOOL LoadPlaylistDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTSIZE) {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
	} else {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return CDialogSK::OnSetCursor(pWnd, nHitTest, message);
	}
	return TRUE;
}
void LoadPlaylistDlg::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	CPoint cpoint(point);
	ScreenToClient(&cpoint);
	m_AdjustLibrary = FALSE;
	m_LibraryDragging = FALSE;
	if (m_AdjustRect.PtInRect(cpoint)) {
		m_AdjustLibrary = TRUE;
	}
	if (m_AdjustLibrary) {
		m_LibraryDragging = TRUE;
		SetCapture();
	} else {
		CDialogSK::OnNcLButtonDown(nHitTest, point);
	}
}
void LoadPlaylistDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	OnNcLButtonUp(nFlags,point);
}

void LoadPlaylistDlg::OnNcLButtonUp(UINT nHitTest, CPoint point) 
{
	if (m_LibraryDragging) {
		ReleaseCapture();
		m_LibraryDragging = FALSE;
		AdjustLibraryWidths(point);
	} else {
		CDialogSK::OnNcLButtonUp(nHitTest, point);
	}
	m_AdjustLibrary = FALSE;
	UpdateWindow();	
}
void LoadPlaylistDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	OnNcMouseMove(nFlags,point);
}

void LoadPlaylistDlg::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	static const SIZE rbs = {1,1};	
	if (m_LibraryDragging) {
		CPoint cpoint(point);
		ScreenToClient(&cpoint);
		CRect rect;
		UpdateWindow();

		BOOL doit = TRUE;
		if (m_AdjustLibrary) {
			rect = m_AdjustInt;
			rect.right = cpoint.x;
			if (rect.right == m_AdjustInt.right)
				rect.right--;
		} else {
			doit = FALSE;
		}
		if (doit) {
			CDC * cdc = GetWindowDC();
			if (m_IsEditor)
				CRectMove(rect,0,m_SysBarHeight+1);
			if (rect.Width() < m_MinNamesWidth)
				rect.right = rect.left + m_MinNamesWidth;
			if (rect.Width() > m_MaxNamesWidth)
				rect.right = rect.left + m_MaxNamesWidth;
			rect.top += 4;
			rect.left = rect.right - 1;
			cdc->DrawDragRect(rect,rbs,m_LastDragRect,rbs);
			ReleaseDC(cdc);
			m_LastDragRect = rect;
		}
	} else {
		CDialogSK::OnNcMouseMove(nHitTest, point);
	}
}

void
LoadPlaylistDlg::AdjustLibraryWidths(CPoint &apoint) {

	CPoint point(apoint);
	ScreenToClient(&point);
	int sw,dx;

	CRect crect,namesrect,nameslabelrect,songsrect,songslabelrect;
	GetClientRect(crect);
	m_PlaylistNames.GetWindowRect(namesrect);
	ScreenToClient(namesrect);

	int dw = crect.Width() - 20;
	int pct;

	sw = namesrect.Width();
	int width = point.x - namesrect.left;

	dx = sw - width;
	namesrect.right -= dx;
	if (namesrect.Width() < m_MinNamesWidth) {
		namesrect.right = namesrect.left + m_MinNamesWidth;
	}
	if (namesrect.Width() > m_MaxNamesWidth) {
		namesrect.right = namesrect.left + m_MaxNamesWidth;
	}
	m_NamesLabel.GetWindowRect(nameslabelrect);
	ScreenToClient(nameslabelrect);
	nameslabelrect.right = namesrect.right;
	
	m_PlaylistSongs.GetWindowRect(songsrect);
	ScreenToClient(songsrect);
	m_SongsLabel.GetWindowRect(songslabelrect);
	ScreenToClient(songslabelrect);
	songsrect.left = namesrect.right + 5;
	songslabelrect.left = songsrect.left;

	m_PlaylistNames.MoveWindow(namesrect);
	m_NamesLabel.MoveWindow(nameslabelrect);
	m_PlaylistSongs.MoveWindow(songsrect);
	m_SongsLabel.MoveWindow(songslabelrect);

	m_AdjustRect = nameslabelrect;
	m_AdjustRect.left = nameslabelrect.right;
	m_AdjustRect.right = songslabelrect.left;
	m_AdjustRect.bottom += 20;
	m_AdjustInt.UnionRect(nameslabelrect,namesrect);

	pct = int((double)100 * ((double)width / (double)dw));
	m_parentcallbacks->mbconfig()->setRegistry("LoadPlaylistNamesWidthPct", pct);

	if (!m_IsEditor)
		OnNcPaint();
	m_Need2Erase = TRUE;
	UpdateWindow();
	resetControls();
}
// These 2 up/dn functions process msg from ExtendedListBox that
// a move has occured. Since ELB handled moving the display
// strings we only do our backend data structure here.
LRESULT
LoadPlaylistDlg::OnMovePlaylistUp(WPARAM sel, LPARAM lParam) {
	CString name,file;
	name = m_csaPlaylistDesc.GetAt(sel);
	file = m_csaPlaylist.GetAt(sel);
	int tlen = m_cwaTlen.GetAt(sel);
	m_csaPlaylistDesc.RemoveAt(sel);
	m_csaPlaylist.RemoveAt(sel);
	m_cwaTlen.RemoveAt(sel);
	sel--;
	if ((int)sel < 0) sel = 0;
	m_csaPlaylistDesc.InsertAt(sel,name);
	m_csaPlaylist.InsertAt(sel,file);
	m_cwaTlen.InsertAt(sel,tlen);
	m_Modified = TRUE;
	m_Save.EnableWindow(TRUE);
	return 0;
}
LRESULT
LoadPlaylistDlg::OnMovePlaylistDn(WPARAM sel, LPARAM lParam) {
	CString name,file;
	name = m_csaPlaylistDesc.GetAt(sel);
	file = m_csaPlaylist.GetAt(sel);
	int tlen = m_cwaTlen.GetAt(sel);
	m_csaPlaylistDesc.RemoveAt(sel);
	m_csaPlaylist.RemoveAt(sel);
	m_cwaTlen.RemoveAt(sel);
	sel++;
	if ((int)sel > m_PlaylistSongs.GetCount()) sel = m_PlaylistSongs.GetCount();
	m_csaPlaylistDesc.InsertAt(sel,name);
	m_csaPlaylist.InsertAt(sel,file);
	m_cwaTlen.InsertAt(sel,tlen);
	m_Modified = TRUE;
	m_Save.EnableWindow(TRUE);
	return 0;
}
// These 2 handle the UP/DOwn buttons being clicked
// Since ELB didn't initiate the move we do both moves here, i.e.
// the ELB array and our backend data structures
void LoadPlaylistDlg::OnPlaylistUp() 
{
	if (m_Control->ptr() == &m_PlaylistSongs) {
		int sel = m_PlaylistSongs.GetCurSel();
		if (0 <= sel && sel <= m_PlaylistSongs.GetCount()) {
			CString name;
			m_PlaylistSongs.GetText(sel, name);
			m_PlaylistSongs.DeleteString(sel);
			OnMovePlaylistUp(sel,0);
			sel--;
			if (sel < 0) sel = 0;
			m_PlaylistSongs.InsertString(sel,name);
			m_Modified = TRUE;
			m_Save.EnableWindow(TRUE);
		}
//		m_PlaylistSongs.SetFocus();
		m_PlaylistSongs.SetCurSel(sel);
	}
}

void LoadPlaylistDlg::OnPlaylistDown() 
{
	if (m_Control->ptr() == &m_PlaylistSongs) {
		int sel = m_PlaylistSongs.GetCurSel();
		if (0 <= sel && sel <= m_PlaylistSongs.GetCount()) {
			CString name;
			m_PlaylistSongs.GetText(sel, name);
			m_PlaylistSongs.DeleteString(sel);
			OnMovePlaylistDn(sel,0);
			sel++;
			if (sel > m_PlaylistSongs.GetCount()) sel = m_PlaylistSongs.GetCount();
			m_PlaylistSongs.InsertString(sel,name);
			m_Modified = TRUE;
			m_Save.EnableWindow(TRUE);
		}
//		m_PlaylistSongs.SetFocus();
		m_PlaylistSongs.SetCurSel(sel);
	}
}
void LoadPlaylistDlg::OnPlaylistSave() 
{
	int sel = m_PlaylistNames.GetCurSel();
	if (sel == LB_ERR) return;
	CString name;
	m_PlaylistNames.GetText(sel, name);
	GetTextFieldDlg text;
	if (text.Get("Playlist Name", name) == IDOK) {
		m_parentcallbacks->musiclib()->savePlaylist(m_csaPlaylist,name);
		m_Modified = FALSE;
		m_Save.EnableWindow(FALSE);
	}
}
void LoadPlaylistDlg::OnRenamePlaylist() 
{
	int sel = m_PlaylistNames.GetCurSel();
	if (sel == LB_ERR) return;
	CString name,newname;
	m_PlaylistNames.GetText(sel, name);
	newname = name;
	GetTextFieldDlg text;
	if (text.Get("New name for Playlist \""+CS(name)+"\"",newname) == IDOK) {
		if (name == newname) return;

		if (!m_parentcallbacks->musiclib()->renamePlaylist(name, newname, FALSE)) {
			if (text.Confirm("Rename playlist "+name+" to "+newname,
				"Playlist "+CS(newname)+" exists. Overwrite?") == IDOK) {
				m_parentcallbacks->musiclib()->renamePlaylist(name, newname, TRUE);
			} else
				newname = name;
		}
		m_Modified = FALSE;
		m_Save.EnableWindow(FALSE);
		init();
		sel = m_PlaylistNames.SelectString(-1,newname);
		m_PlaylistNames.SetCurSel(sel);
	}
}
void LoadPlaylistDlg::OnDeletePlaylist() 
{
    if (m_Control->ptr() == &m_PlaylistNames) {
		int sel = m_PlaylistNames.GetCurSel();
		if (sel == LB_ERR) return;
		CString name;
		m_PlaylistNames.GetText(sel, name);
		if (name.GetLength()) {
			GetTextFieldDlg text;
			if (IDOK == text.Confirm("Confirmation","Delete playlist "+name+"?")) {
				m_PlaylistNames.DeleteString(sel);
				if (sel > m_PlaylistNames.GetCount()-1) {
					sel = m_PlaylistNames.GetCount()-1;
				}
				m_parentcallbacks->musiclib()->deletePlaylist(name);
				init();
			}
		}
		m_PlaylistNames.SetCurSel(sel);
		m_PlaylistNames.SetFocus();
		OnSelchangePlaylistNames();
	} else if (m_Control->ptr() == &m_PlaylistSongs) {
		int sel = m_PlaylistSongs.GetCurSel();
		if (sel == LB_ERR) return;

		CString name;
		m_PlaylistSongs.GetText(sel, name);
		m_PlaylistSongs.DeleteString(sel);
		m_csaPlaylistDesc.RemoveAt(sel);
		m_csaPlaylist.RemoveAt(sel);
		m_cwaTlen.RemoveAt(sel);
		if (sel > m_PlaylistSongs.GetCount()-1) {
			sel = m_PlaylistSongs.GetCount()-1;
		}

		m_PlaylistSongs.SetFocus();
		m_PlaylistSongs.SetCurSel(sel);
		m_Modified = TRUE;
		m_Save.EnableWindow(TRUE);
	}
    UpdateWindow();	
}
CWnd *
LoadPlaylistDlg::GetResizableWnd() {
	return this;
}
BOOL LoadPlaylistDlg::PreTranslateMessage(MSG* pMsg)
{	// disable ESC & ENTER from killing the dialog
    if (pMsg->message == WM_KEYDOWN) {
//		logger.ods("PreTMsg");
//        if (pMsg->wParam == VK_RETURN) {
//			OnControlClick();
//            return TRUE;
//		} else if (pMsg->wParam == VK_ESCAPE) {
//			return TRUE;
        if (pMsg->wParam == VK_DELETE) {
            OnDeletePlaylist();
            return TRUE;
		}
//		} else if (pMsg->wParam == VK_F5) {
//			resetControls();
//			return TRUE;
//      }
    }
    return CDialog::PreTranslateMessage(pMsg);
}

