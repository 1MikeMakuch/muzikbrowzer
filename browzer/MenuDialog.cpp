// MenuDialog.cpp : implementation file
//

#include "stdafx.h"

#include "PlayerDlg.h"
#include "MenuDialog.h"
#include "VirtualControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuDialog dialog


CMenuDialog::CMenuDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuDialog::IDD, pParent),
    m_maxbutton(9),
    m_currentbutton(0), m_Control(new VirtualControl)
{
	//{{AFX_DATA_INIT(CMenuDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
CMenuDialog::~CMenuDialog() {
	delete m_Control;
}


void CMenuDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMenuDialog, CDialog)
	//{{AFX_MSG_MAP(CMenuDialog)
	ON_BN_CLICKED(IDC_PMENUEXIT, OnMenuExit)
    ON_BN_CLICKED(IDC_PMENUSHUFFLEPLAYLIST, OnMenuShuffleplaylist)
    ON_BN_CLICKED(IDC_PMENURANDOMIZEPLAYLIST, OnMenuRandomizePlaylist)
    ON_BN_CLICKED(IDC_PMENUCLEARPLAYLIST, OnMenuClearplaylist)
	ON_BN_CLICKED(IDC_PMENUSAVEPLAYLIST, OnMenuSavePlaylist)
    ON_BN_CLICKED(IDC_PMENULOADPLAYLIST, OnMenuLoadplaylist)
	ON_BN_CLICKED(IDC_PMENUPAUSE, OnMenuPause)
	ON_BN_CLICKED(IDC_PMENUPLAY, OnMenuPlay)
	ON_BN_CLICKED(IDC_PMENUSTOP, OnMenuStop)
	ON_BN_CLICKED(IDC_PMENUHELP, OnMenuHelp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuDialog message handlers

void CMenuDialog::OnMenuExit() 
{
//	((CPlayerDlg*)m_pParentWnd)->OnMenuExit();
	EndDialog(0);
}
void CMenuDialog::OnMenuShuffleplaylist() 
{
	((CPlayerDlg*)m_pParentWnd)->OnMenuShuffleplaylist();
	EndDialog(0);
}
void CMenuDialog::OnMenuRandomizePlaylist() 
{
	// TODO: Add your control notification handler code here
	
}
void CMenuDialog::OnMenuClearplaylist() 
{
    ((CPlayerDlg*)m_pParentWnd)->OnMenuClearplaylist();
    EndDialog(0);
}
void CMenuDialog::OnMenuSavePlaylist() 
{
    ((CPlayerDlg*)m_pParentWnd)->SetSavePlaylistFlag(FALSE);
	((CPlayerDlg*)m_pParentWnd)->OnMenuSaveplaylist();
	EndDialog(0);	
}
void CMenuDialog::OnMenuLoadplaylist() 
{
	((CPlayerDlg*)m_pParentWnd)->OnMenuLoadplaylist();
	EndDialog(0);
}
void CMenuDialog::OnMenuPause() 
{
	((CPlayerDlg*)m_pParentWnd)->OnMenuPause();
	EndDialog(0);
}
void CMenuDialog::OnMenuPlay() 
{
	((CPlayerDlg*)m_pParentWnd)->OnMenuPlay();
	EndDialog(0);
}
void CMenuDialog::OnMenuStop() 
{
	((CPlayerDlg*)m_pParentWnd)->OnMenuStop();
	EndDialog(0);
}
void CMenuDialog::OnMenuHelp() 
{
	((CPlayerDlg*)m_pParentWnd)->OnMenuHelp();
	EndDialog(0);
}
void CMenuDialog::OnControlClick() {
	EndDialog(m_currentbutton);
}
void CMenuDialog::OnExit() {
	EndDialog(0);
}