// StatusDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mydiff.h"
#include "StatusDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// StatusDlg dialog


StatusDlg::StatusDlg(CWnd* pParent /*=NULL*/)
	: CDialog(StatusDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(StatusDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Create(IDD_DIALOG1);
	m_msg = "";
}

void StatusDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(StatusDlg)
	DDX_Control(pDX, IDC_SD_STATUS, m_Status);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(StatusDlg, CDialog)
	//{{AFX_MSG_MAP(StatusDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// StatusDlg message handlers
void
StatusDlg::showStatus(const CString & msg) {
	m_msg += "\r\n";
	m_msg += msg;

	m_Status.SetWindowText(m_msg);
	ShowWindow(SW_SHOW);
	UpdateData(FALSE);
}
void
StatusDlg::kill() {
	DestroyWindow();
}