// LoadPlaylistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "LoadPlaylistDlg.h"
//#include "ConfigSettings.h"
#include "MBGlobals.h"
#include "VirtualControl.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LoadPlaylistDlg dialog


LoadPlaylistDlg::LoadPlaylistDlg(CPlayerDlg *p, MusicLib *m, CWnd* pParent /*=NULL*/)
	: CDialog(LoadPlaylistDlg::IDD, pParent), m_pd(p), m_mdb(m),
    m_PlaylistSongs(FALSE), m_PlaylistNames(FALSE),
	m_Control(new VirtualControl)
{
	//{{AFX_DATA_INIT(LoadPlaylistDlg)
	//}}AFX_DATA_INIT
}


void LoadPlaylistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LoadPlaylistDlg)
	DDX_Control(pDX, IDC_PLAYLIST_DELETE, m_Delete);
	DDX_Control(pDX, IDC_PLAYLIST_LOAD, m_Load);
	DDX_Control(pDX, IDC_GAAS, m_GAAS);
	DDX_Control(pDX, IDC_PLAYLIST_SONGS, m_PlaylistSongs);
	DDX_Control(pDX, IDC_PLAYLIST_NAMES, m_PlaylistNames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LoadPlaylistDlg, CDialog)
	//{{AFX_MSG_MAP(LoadPlaylistDlg)
	ON_LBN_DBLCLK(IDC_PLAYLIST_NAMES, OnDblclkPlaylistNames)
	ON_LBN_SELCHANGE(IDC_PLAYLIST_NAMES, OnSelchangePlaylistNames)
	ON_LBN_SETFOCUS(IDC_PLAYLIST_NAMES, OnSetfocusPlaylistNames)
	ON_LBN_SETFOCUS(IDC_PLAYLIST_SONGS, OnSetfocusPlaylistSongs)
	ON_BN_CLICKED(IDC_PLAYLIST_LOAD, OnPlaylistOk)
	ON_BN_CLICKED(IDC_PLAYLIST_DELETE, OnDeletePlaylist)

	ON_LBN_KILLFOCUS(IDC_PLAYLIST_SONGS, OnKillfocusPlaylistSongs)
	ON_LBN_SELCHANGE(IDC_PLAYLIST_SONGS, OnSelchangePlaylistSongs)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
//	ON_BN_CLICKED(IDC_PLAYLIST_SAVE, OnPlaylistSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LoadPlaylistDlg message handlers

void LoadPlaylistDlg::OnDblclkPlaylistNames() 
{
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
	m_PlaylistSongs.ResetContent();
    int sel = m_PlaylistNames.GetCurSel();
    if (sel < 0) return;
    CString name;
    m_PlaylistNames.GetText(sel, name);
    m_mdb->getSongsInPlaylist(name,m_PlaylistSongs);
	UpdateWindow();
}

void LoadPlaylistDlg::OnSetfocusPlaylistNames() 
{
	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_PLAYLIST_NAMES);
    m_GAAS.SetWindowText("");
    m_Load.EnableWindow(TRUE);
    m_Delete.EnableWindow(TRUE);

}


void LoadPlaylistDlg::OnSetfocusPlaylistSongs() 
{
    m_Load.EnableWindow(FALSE);
    m_Delete.EnableWindow(FALSE);

	*m_Control = (CExtendedListBox*)GetDlgItem(IDC_PLAYLIST_SONGS);
    m_PlaylistSongs.SetCurSel(0);
}


BOOL LoadPlaylistDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_PlaylistSongs.ResetContent();
	m_PlaylistNames.ResetContent();
    m_PlaylistSongs.initFont();
    m_PlaylistNames.initFont();
    LPLOGFONT lplf = m_pd->config().getTitlesFont();
    m_PlaylistSongs.changeFont(lplf);
    m_PlaylistNames.changeFont(lplf);

    init();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void
LoadPlaylistDlg::init() {
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
    EndDialog(0);
}

void LoadPlaylistDlg::OnCancel() 
{
    EndDialog(0);	
}

void LoadPlaylistDlg::OnPlaylistOk() 
{
	OnDblclkPlaylistNames();
}

void LoadPlaylistDlg::OnDeletePlaylist() 
{
    if (m_Control->ptr() != &m_PlaylistNames) {
        return;
    }
    int sel = m_PlaylistNames.GetCurSel();
    if (sel == LB_ERR) return;
    CString name;
    m_PlaylistNames.GetText(sel, name);
    m_PlaylistNames.DeleteString(sel);
    if (sel > m_PlaylistNames.GetCount()-1) {
        sel = m_PlaylistNames.GetCount()-1;
    }
    m_mdb->deletePlaylist(name);
    init();
    m_PlaylistNames.SetCurSel(sel);
    UpdateWindow();	
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

void LoadPlaylistDlg::OnSelchangePlaylistSongs() 
{
    int sel = m_PlaylistSongs.GetCurSel();
    CString name;
    m_PlaylistSongs.GetText(sel,name);
    m_GAAS.SetWindowText(name);
}

void LoadPlaylistDlg::OnPlaylistSave() 
{
	// TODO: Add your control notification handler code here
	
}
