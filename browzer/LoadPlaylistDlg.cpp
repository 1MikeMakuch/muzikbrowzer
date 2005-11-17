// LoadPlaylistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadPlaylistDlg.h"
//#include "ConfigSettings.h"
#include "MBGlobals.h"
#include "VirtualControl.h"
#include "SkinDefs.h"
#include "MBMessageBox.h"
#include "GetTextField.h"
#include "MyLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LoadPlaylistDlg dialog


LoadPlaylistDlg::LoadPlaylistDlg(CPlayerDlg *p, MusicLib *m, 
				CWnd* pParent /*=NULL*/, BOOL editor)
	: CDialog(LoadPlaylistDlg::IDD, pParent), m_pd(p), m_mdb(m),
    m_PlaylistSongs(TRUE, "songs", FALSE), m_PlaylistNames(TRUE, "names", FALSE),
	m_Control(new VirtualControl), m_IsEditor(editor)
{
	//{{AFX_DATA_INIT(LoadPlaylistDlg)
	//}}AFX_DATA_INIT
	m_Resizing = FALSE;	
	m_PlaylistSongs.SetPWnd(this);
	m_PlaylistNames.SetPWnd(this);
	m_AdjustLibrary = FALSE;
	m_LibraryDragging = FALSE;
	m_Modified = FALSE;
}


void LoadPlaylistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoadPlaylistDlg)
	DDX_Control(pDX, IDC_PLAYLIST_RENAME, m_Rename);
	DDX_Control(pDX, IDC_PLAYLIST_CANCEL, m_Exit);
	DDX_Control(pDX, IDC_PLAYLIST_DELETE, m_Delete);
	DDX_Control(pDX, IDC_PLAYLIST_LOAD, m_Load);
	DDX_Control(pDX, IDC_PLAYLIST_UP, m_Up);
	DDX_Control(pDX, IDC_PLAYLIST_DOWN, m_Down);
	DDX_Control(pDX, IDC_PLAYLIST_SAVE, m_Save);
	DDX_Control(pDX, IDC_GAAS, m_GAAS);
	DDX_Control(pDX, IDC_PLAYLIST_SONGS, m_PlaylistSongs);
	DDX_Control(pDX, IDC_PLAYLIST_NAMES, m_PlaylistNames);
	DDX_Control(pDX, IDC_LPD_NAMES_LABEL, m_NamesLabel);
	DDX_Control(pDX, IDC_LPD_SONGS_LABEL, m_SongsLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LoadPlaylistDlg, CDialog)
	//{{AFX_MSG_MAP(LoadPlaylistDlg)
	ON_LBN_DBLCLK(IDC_PLAYLIST_NAMES, OnDblclkPlaylistNames)
	ON_LBN_SELCHANGE(IDC_PLAYLIST_NAMES, OnSelchangePlaylistNames)
	ON_LBN_SETFOCUS(IDC_PLAYLIST_NAMES, OnSetfocusPlaylistNames)
	ON_LBN_SETFOCUS(IDC_PLAYLIST_SONGS, OnSetfocusPlaylistSongs)
	ON_BN_CLICKED(IDC_PLAYLIST_LOAD, OnPlaylistOk)
	ON_BN_CLICKED(IDC_PLAYLIST_UP, OnPlaylistUp)
	ON_BN_CLICKED(IDC_PLAYLIST_DOWN, OnPlaylistDown)
	ON_BN_CLICKED(IDC_PLAYLIST_SAVE, OnPlaylistSave)
	ON_BN_CLICKED(IDC_PLAYLIST_DELETE, OnDeletePlaylist)
	ON_BN_CLICKED(IDC_PLAYLIST_RENAME, OnRenamePlaylist)
	ON_LBN_KILLFOCUS(IDC_PLAYLIST_SONGS, OnKillfocusPlaylistSongs)
	ON_LBN_SELCHANGE(IDC_PLAYLIST_SONGS, OnSelchangePlaylistSongs)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_WM_SETCURSOR()
	ON_WM_NCMOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
	ON_WM_NCLBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_WM_NCHITTEST()
	ON_WM_CTLCOLOR()
	ON_WM_SETCURSOR()
	ON_MESSAGE(MB_LISTMOVEUP, OnMovePlaylistUp)
	ON_MESSAGE(MB_LISTMOVEDN, OnMovePlaylistDn)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LoadPlaylistDlg message handlers
BOOL LoadPlaylistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	InitGrip();
	ShowSizeGrip(TRUE);

	int iwidth,iheight;
	m_pd->config().getRegistry("LoadPlaylistWidth", iwidth, MB_MIN_WIDTH);
	m_pd->config().getRegistry("LoadPlaylistHeight", iheight, MB_MIN_HEIGHT);

	CString skindef = m_pd->config().getSkin(MB_SKIN_DEF);
	RegistryKey regSD(skindef);
	regSD.ReadFile();

	CString skincustom = m_pd->config().getSkin(MB_SKIN_DEF_CUSTOM);
	RegistryKey regSDCustom(skincustom);
	regSDCustom.ReadFile();
	regSD.Copy(regSDCustom);

	int r,g,b;
	r = regSD.Read("TransRedPanel",253);
	g = regSD.Read("TransGreenPanel",0);
	b = regSD.Read("TransBluePanel",0);
	COLORREF m_TransPanel = RGB(r,g,b);

	m_PlaylistSongs.ResetContent();
	m_PlaylistNames.ResetContent();
	
	CDC * cdc = GetDC();

	if (m_IsEditor) { 
		m_PlaylistNames.SetBitmaps(cdc);
		m_PlaylistSongs.SetBitmaps(cdc);
		m_NamesLabel.initFont();
		m_SongsLabel.initFont();
	} else {
		m_PlaylistSongs.initFont();
		m_PlaylistNames.initFont();
		m_NamesLabel.initFont();
		m_SongsLabel.initFont();
		LPLOGFONT lplf = m_pd->config().getTitlesFont();
		m_PlaylistSongs.changeFont(lplf);
		m_PlaylistNames.changeFont(lplf);

		lplf = m_pd->config().getColHdrFont();
		m_NamesLabel.changeFont(lplf);
		m_SongsLabel.changeFont(lplf);

		m_NamesLabel.SetTextColor(m_pd->config().getColorTxColHdr());
		m_SongsLabel.SetTextColor(m_pd->config().getColorTxColHdr());

		COLORREF rgb = m_pd->config().getColorBkColHdr();

		m_NamesLabel.SetBkColor(rgb);
		m_SongsLabel.SetBkColor(rgb);
		

		m_PlaylistSongs.SetColors(m_pd->config().getColorBkNormal(),
			m_pd->config().getColorBkHigh(),
			m_pd->config().getColorBkSel(),
			m_pd->config().getColorTxNormal(),
			m_pd->config().getColorTxHigh(),
			m_pd->config().getColorTxSel());

		m_PlaylistNames.SetColors(m_pd->config().getColorBkNormal(),
			m_pd->config().getColorBkHigh(),
			m_pd->config().getColorBkSel(),
			m_pd->config().getColorTxNormal(),
			m_pd->config().getColorTxHigh(),
			m_pd->config().getColorTxSel());

		m_PlaylistNames.SetBitmaps(cdc, 
			m_pd->config().getSkin(MB_SKIN_SCROLLUPARROW),m_TransPanel,
			m_pd->config().getSkin(MB_SKIN_SCROLLDOWNARROW),m_TransPanel,
			m_pd->config().getSkin(MB_SKIN_SCROLLBUTTON),m_TransPanel,
			m_pd->config().getSkin(MB_SKIN_SCROLLBACKGROUND),m_TransPanel);
		
		m_PlaylistSongs.SetBitmaps(cdc, 
			m_pd->config().getSkin(MB_SKIN_SCROLLUPARROW),m_TransPanel,
			m_pd->config().getSkin(MB_SKIN_SCROLLDOWNARROW),m_TransPanel,
			m_pd->config().getSkin(MB_SKIN_SCROLLBUTTON),m_TransPanel,
			m_pd->config().getSkin(MB_SKIN_SCROLLBACKGROUND),m_TransPanel);
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
    m_pd->LoadPlaylist(name);	
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
			tmp += name + "\" playlist.\r\nDo you wish to first save it?";
			int ok = MBMessageBox("Confirmation", tmp, TRUE, TRUE);
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
			m_mdb->getSongsInPlaylist(name,m_csaPlaylistDesc,m_csaPlaylist);
			m_Modified = FALSE;
			m_Save.EnableWindow(FALSE);
			sel = m_PlaylistNames.SelectString(-1,name); // -1 for all
			m_PlaylistNames.SetCurSel(sel);
			logger.ods(CString("LPD cursel " + numToString(sel)));
		} else {  // Answer was yes, (Ok)
			name = m_LastName;
			sel = m_PlaylistNames.SelectString(-1,name); // -1 for all
			m_PlaylistNames.SetCurSel(sel);
			logger.ods(CString("LPD cursel " + numToString(sel)));
		}
	}
	int pos,size;
	size = m_csaPlaylistDesc.GetSize();
	for (pos = 0; pos < size; pos++ ) {
		m_PlaylistSongs.AddString(m_csaPlaylistDesc.GetAt(pos));
	}
	m_PlaylistNames.invalidate();
	m_PlaylistSongs.invalidate();
	m_GAAS.SetWindowText(name);
	UpdateWindow();
	m_LastName = name;
//	m_SongsLabel.setText("Songs in \"" + name + "\"");
}
void LoadPlaylistDlg::OnSelchangePlaylistSongs() 
{
    int sel = m_PlaylistSongs.GetCurSel();
    CString name;
    m_PlaylistSongs.GetText(sel,name);
    m_GAAS.SetWindowText(name);
	m_PlaylistSongs.invalidate();
}

void LoadPlaylistDlg::OnSetfocusPlaylistNames() 
{
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_PLAYLIST_NAMES);
    m_GAAS.SetWindowText("");
    m_Load.EnableWindow(TRUE);
    m_Delete.EnableWindow(TRUE);
	m_Up.EnableWindow(FALSE);
	m_Down.EnableWindow(FALSE);
	//OnSelchangePlaylistNames();
}

void LoadPlaylistDlg::OnSetfocusPlaylistSongs() 
{
    m_Load.EnableWindow(TRUE);
    //m_Delete.EnableWindow(FALSE);
	m_Up.EnableWindow(TRUE);
	m_Down.EnableWindow(TRUE);

	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_PLAYLIST_SONGS);
    m_PlaylistSongs.SetCurSel(0);
	OnSelchangePlaylistSongs();
}


void
LoadPlaylistDlg::init() {
	CWaitCursor c;
	m_PlaylistNames.ResetContent();
    m_mdb->getPlaylistNames(m_PlaylistNames);
    m_PlaylistNames.SetCurSel(0);
    OnSelchangePlaylistNames();
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
		int ok = MBMessageBox("Confirmation", tmp, TRUE, TRUE);
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
		int ok = MBMessageBox("Confirmation", tmp, TRUE, TRUE);
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
		int ok = MBMessageBox("Confirmation", tmp, TRUE, TRUE);
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

void LoadPlaylistDlg::OnKillfocusPlaylistSongs() 
{


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
		CDialog::OnSizing(fwSide, pRect);
//	}
	m_Resizing = TRUE;	
	UpdateGripPos();
}
void LoadPlaylistDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	resetControls();
	UpdateGripPos();
}
void LoadPlaylistDlg::resetControls() {
	if (!IsWindow(m_hWnd)) return;
	if (!IsWindow(m_PlaylistNames.m_hWnd)) return;

	CRect clientrect,namesrect,songsrect,
		deleterect,loadrect,gaasrect,exitrect,
		nameslabelrect,songslabelrect,
		uprect,downrect,saverect,wrect,renamerect;
	GetClientRect(clientrect);
	GetWindowRect(wrect);
	m_Delete.GetClientRect(deleterect);
	m_Load.GetClientRect(loadrect);
	m_GAAS.GetClientRect(gaasrect);
	m_Exit.GetClientRect(exitrect);
	m_Up.GetClientRect(uprect);
	m_Down.GetClientRect(downrect);
	m_Save.GetClientRect(saverect);
	m_Rename.GetClientRect(renamerect);

	int pct;// = int((double)100 * ((double)width / (double)dw));
	int iwidth,iheight;
	iwidth = wrect.Width();
	iheight = wrect.Height();
	m_pd->config().getRegistry("LoadPlaylistNamesWidthPct", pct, 25);
	m_pd->config().setRegistry("LoadPlaylistWidth", iwidth);
	m_pd->config().setRegistry("LoadPlaylistHeight", iheight);
	double lpct = (double) pct / (double)100;

	int buttonheight = loadrect.Height();
	int buttonwidth = loadrect.Width();
	loadrect.top = clientrect.bottom - (10 + buttonheight);
	loadrect.bottom = loadrect.top + buttonheight;
	loadrect.left = 10;
	loadrect.right = loadrect.left + buttonwidth;

	renamerect = loadrect;
	renamerect.left = loadrect.right + 5;
	renamerect.right = renamerect.left + buttonwidth;

	exitrect = renamerect;
	exitrect.left = renamerect.right + 5;
	exitrect.right = exitrect.left + buttonwidth;
	
	deleterect = exitrect;
	deleterect.left = exitrect.right + 5;
	deleterect.right = deleterect.left + buttonwidth;

	uprect = loadrect;
	uprect.left = deleterect.right + 5;
	uprect.right = uprect.left + buttonwidth;

	downrect = loadrect;
	downrect.left = uprect.right + 5;
	downrect.right = downrect.left + buttonwidth;

	saverect = loadrect;
	saverect.left = downrect.right + 5;
	saverect.right = saverect.left + buttonwidth;

	m_MinNamesWidth = loadrect.Width() + 5 + exitrect.Width();
	m_MaxNamesWidth = clientrect.Width() - 50;

	m_Load.MoveWindow(loadrect);
	m_Delete.MoveWindow(deleterect);
	m_Exit.MoveWindow(exitrect);
	m_Up.MoveWindow(uprect);
	m_Down.MoveWindow(downrect);
	m_Save.MoveWindow(saverect);
	m_Rename.MoveWindow(renamerect);

	int gaasheight = gaasrect.Height();
	//gaasheight = 30;
	gaasrect.left = clientrect.left + 10;
	gaasrect.right = clientrect.right - 10;
	gaasrect.bottom = loadrect.top - 5;
	gaasrect.top = gaasrect.bottom - gaasheight;
	m_GAAS.MoveWindow(gaasrect);

	CSize s = m_NamesLabel.GetSize("Playlists");

	nameslabelrect.top = 10;;
	nameslabelrect.bottom = nameslabelrect.top + s.cy;
	nameslabelrect.left = 10;
	nameslabelrect.right = (int)( (double)clientrect.Width() * lpct);
	if (nameslabelrect.Width() < m_MinNamesWidth) 
		nameslabelrect.right = nameslabelrect.left + m_MinNamesWidth;
	if (nameslabelrect.Width() > m_MaxNamesWidth) 
		nameslabelrect.right = nameslabelrect.left + m_MaxNamesWidth;

	songslabelrect = nameslabelrect;
	songslabelrect.left = nameslabelrect.right + 5;
	songslabelrect.right = clientrect.right - 10;

	m_NamesLabel.MoveWindow(nameslabelrect);
	m_SongsLabel.MoveWindow(songslabelrect);

	namesrect.top = nameslabelrect.bottom;
	namesrect.bottom = gaasrect.top - 5;
	namesrect.left = 10;
	namesrect.right = nameslabelrect.right;

	songsrect = namesrect;
	songsrect.left = namesrect.right + 5;
	songsrect.right = clientrect.right - 10;

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

	RedrawWindow();

}

HBRUSH LoadPlaylistDlg::OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor ) {

	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if (m_IsEditor)
		return (HBRUSH)hbr;

    COLORREF bkcolor, txcolor;
    switch (nCtlColor) {
    case CTLCOLOR_BTN: //   3 Button control
		return hbr;

        bkcolor = m_pd->config().getColorBkPanel();
        txcolor = m_pd->config().getColorTxPanel();

        break;
    case CTLCOLOR_DLG: //   4 Dialog box

		bkcolor = m_pd->config().getColorBkColHdr();
		txcolor = m_pd->config().getColorTxColHdr();
        break;
    case CTLCOLOR_EDIT: //   1 Edit control
        bkcolor = m_pd->config().getColorBkPanel();
        txcolor = m_pd->config().getColorTxPanel();
        break;
    case CTLCOLOR_LISTBOX: //  2 List-box control
        bkcolor = m_pd->config().getColorBkNormal();
        txcolor = m_pd->config().getColorTxPanel();
        break;
    case CTLCOLOR_MSGBOX: //   0 Message box
        bkcolor = m_pd->config().getColorBkPanel();
        txcolor = m_pd->config().getColorTxPanel();
        break;
    case CTLCOLOR_SCROLLBAR: //  5 Scroll-bar control
        bkcolor = m_pd->config().getColorBkPanel();
        txcolor = m_pd->config().getColorTxPanel();
        break;
    case CTLCOLOR_STATIC: //  6 Static control
        bkcolor = m_pd->config().getColorBkPanel();
        txcolor = m_pd->config().getColorTxPanel();
		break;
    default:
        bkcolor = m_pd->config().getColorBkPanel();
        txcolor = m_pd->config().getColorTxPanel();
        break;
    }

    pDC->SetBkColor(bkcolor);
    pDC->SetTextColor(txcolor);

	m_brush.DeleteObject();
    m_brush.CreateSolidBrush(bkcolor);


    return (HBRUSH)m_brush;


}





UINT
LoadPlaylistDlg::OnNcHitTest( CPoint point ) {

    CRect r;//,applabelr; //,positionr,volumer;
	GetClientRect(&r);
	CRect br = r;
	br.TopLeft() = CPoint(br.BottomRight().x-20, br.BottomRight().y-20);
	CPoint cpoint(point);
	ScreenToClient(&cpoint);
	
	ClientToScreen(&r);
	ClientToScreen(&br);

	UINT rv = CDialog::OnNcHitTest(point);
	
    if(br.PtInRect(point)) {
		return HTBOTTOMRIGHT;
	} else if (m_AdjustRect.PtInRect(cpoint)) {
		return HTSIZE;
    } else {
		return HTCAPTION;
    } 

	return rv;
}
BOOL LoadPlaylistDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (nHitTest == HTSIZE) {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_SIZEWE));
	} else {
		::SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
		return CDialog::OnSetCursor(pWnd, nHitTest, message);
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
		CDialog::OnNcLButtonDown(nHitTest, point);
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
		CDialog::OnNcLButtonUp(nHitTest, point);
	}
	m_AdjustLibrary = FALSE;
	
}
void LoadPlaylistDlg::OnMouseMove(UINT nFlags, CPoint point) 
{
	ClientToScreen(&point);
	OnNcMouseMove(nFlags,point);
}

void LoadPlaylistDlg::OnNcMouseMove(UINT nHitTest, CPoint point) 
{
	
	if (m_LibraryDragging) {
//		CString msg = " mAL=" + numToString(m_AdjustLibrary);
//		msg += "\r\n";
		CPoint cpoint(point);
		ScreenToClient(&cpoint);
		CRect rect;
		RedrawWindow();

		BOOL doit = TRUE;
		if (m_AdjustLibrary) {
			rect = m_AdjustInt;
			rect.right = cpoint.x;
			if (rect.right == m_AdjustInt.right)
				rect.right--;
		} 
		if (doit) {
			CDC * cdc = GetDC();
			if (rect.Width() < m_MinNamesWidth)
				rect.right = rect.left + m_MinNamesWidth;
			if (rect.Width() > m_MaxNamesWidth)
				rect.right = rect.left + m_MaxNamesWidth;
			cdc->FrameRect(rect, &m_HatchBrush);
			ReleaseDC(cdc);
		}
	} else {
		CDialog::OnNcMouseMove(nHitTest, point);
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

//	ClientToScreen(namesrect);
//	ClientToScreen(nameslabelrect);
//	ClientToScreen(songsrect);
//	ClientToScreen(songslabelrect);
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
	m_pd->config().setRegistry("LoadPlaylistNamesWidthPct", pct);


	OnNcPaint();
	Invalidate();
	RedrawWindow();
}
// These 2 up/dn functions process msg from ExtendedListBox that
// a move has occured. Since ELB handled moving the display
// strings we only do our backend data structure here.
LRESULT
LoadPlaylistDlg::OnMovePlaylistUp(UINT sel, LONG lParam) {
	CString name,file;
	name = m_csaPlaylistDesc.GetAt(sel);
	file = m_csaPlaylist.GetAt(sel);
	m_csaPlaylistDesc.RemoveAt(sel);
	m_csaPlaylist.RemoveAt(sel);
	sel--;
	if ((int)sel < 0) sel = 0;
	m_csaPlaylistDesc.InsertAt(sel,name);
	m_csaPlaylist.InsertAt(sel,file);
	m_Modified = TRUE;
	m_Save.EnableWindow(TRUE);
	return 0;
}
LRESULT
LoadPlaylistDlg::OnMovePlaylistDn(UINT sel, LONG lParam) {
	CString name,file;
	name = m_csaPlaylistDesc.GetAt(sel);
	file = m_csaPlaylist.GetAt(sel);
	m_csaPlaylistDesc.RemoveAt(sel);
	m_csaPlaylist.RemoveAt(sel);
	sel++;
	if ((int)sel > m_PlaylistSongs.GetCount()) sel = m_PlaylistSongs.GetCount();
	m_csaPlaylistDesc.InsertAt(sel,name);
	m_csaPlaylist.InsertAt(sel,file);
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
			OnMovePlaylistUp(sel);
			sel--;
			if (sel < 0) sel = 0;
			m_PlaylistSongs.InsertString(sel,name);
			m_Modified = TRUE;
			m_Save.EnableWindow(TRUE);
		}
		m_PlaylistSongs.SetFocus();
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
			OnMovePlaylistDn(sel);
			sel++;
			if (sel > m_PlaylistSongs.GetCount()) sel = m_PlaylistSongs.GetCount();
			m_PlaylistSongs.InsertString(sel,name);
			m_Modified = TRUE;
			m_Save.EnableWindow(TRUE);
		}
		m_PlaylistSongs.SetFocus();
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
		m_mdb->savePlaylist(m_csaPlaylist,name);
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

		if (!m_mdb->renamePlaylist(name, newname, FALSE)) {
			if (text.Confirm("Rename playlist "+name+" to "+newname,
				"Playlist "+CS(newname)+" exists. Overwrite?") == IDOK) {
				m_mdb->renamePlaylist(name, newname, TRUE);
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
				m_mdb->deletePlaylist(name);
				init();
			}
		}
		m_PlaylistNames.SetCurSel(sel);
		m_PlaylistNames.SetFocus();
	} else if (m_Control->ptr() == &m_PlaylistSongs) {
		int sel = m_PlaylistSongs.GetCurSel();
		if (sel == LB_ERR) return;
		CString name;
		m_PlaylistSongs.GetText(sel, name);
		m_PlaylistSongs.DeleteString(sel);
		m_csaPlaylistDesc.RemoveAt(sel);
		m_csaPlaylist.RemoveAt(sel);
		if (sel > m_PlaylistSongs.GetCount()-1) {
			sel = m_PlaylistSongs.GetCount()-1;
		}
//		m_mdb->deletePlaylistSong(name);
		init();
		m_PlaylistSongs.SetCurSel(sel);
		m_PlaylistSongs.SetFocus();
		m_Modified = TRUE;
		m_Save.EnableWindow(TRUE);
	}
    UpdateWindow();	
}
CWnd *
LoadPlaylistDlg::GetResizableWnd() {
	return this;
}
