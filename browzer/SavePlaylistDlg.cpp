// SavePlaylistDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SavePlaylistDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SavePlaylistDlg dialog


SavePlaylistDlg::SavePlaylistDlg(CPlayerDlg *p, CWnd* pParent /*=NULL*/)
	: CDialog(SavePlaylistDlg::IDD, pParent), m_pd(p)
{
	//{{AFX_DATA_INIT(SavePlaylistDlg)
	m_Name = _T("");
	//}}AFX_DATA_INIT
}


void SavePlaylistDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SavePlaylistDlg)
	DDX_Text(pDX, IDC_SAVEPLAYLIST_BOX, m_Name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SavePlaylistDlg, CDialog)
	//{{AFX_MSG_MAP(SavePlaylistDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SavePlaylistDlg message handlers

void SavePlaylistDlg::OnOK() 
{
    
    UpdateData(TRUE);
	m_pd->SavePlaylist(m_Name);
	
	CDialog::OnOK();
}
