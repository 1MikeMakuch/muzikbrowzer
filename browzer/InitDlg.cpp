// InitDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "MfcPlayer.h"
#include "InitDlg.h"
#include "IRCodes.h"
#include "MBGlobals.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// InitDlg dialog


InitDlg::InitDlg(int cflag, int abortflag, CWnd* pParent /*=NULL*/)
	: CDialog(InitDlg::IDD, pParent), m_Abort(0), m_abortflag(abortflag)
{
	//{{AFX_DATA_INIT(InitDlg)
	m_InitStatus = _T("");
	m_InitLabel = _T("");
	//}}AFX_DATA_INIT

//	m_InitStatus = "reading music database";
    
    if (cflag) {
	    Create(IDD_INIT_DIALOG);
    }

}


void InitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(InitDlg)
	DDX_Control(pDX, IDC_INIT_ABORT, m_AbortButton);
	DDX_Control(pDX, IDC_INIT_PROGRESS, m_InitProgress);
	DDX_Text(pDX, IDC_INIT_STATUS, m_InitStatus);
	DDX_Text(pDX, IDC_INIT_LABEL, m_InitLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(InitDlg, CDialog)
	//{{AFX_MSG_MAP(InitDlg)
	ON_BN_CLICKED(IDC_INIT_ABORT, OnInitAbort)
	//}}AFX_MSG_MAP
    
    ON_MESSAGE(INIT_DLG_MESSAGE, OnIDMessage)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// InitDlg message handlers

void
InitDlg::SetLabel(CString & label) {
	m_InitLabel = label;
	UpdateData(FALSE);
}
void
InitDlg::UpdateStatus(CString & msg) {
	m_InitStatus = msg;
	UpdateData(FALSE);
}
void InitDlg::OnInitAbort() 
{
	m_Abort = 1;
	
}
class MsgInts {
public:
    int mint1;
    int mint2;
};

void
InitDlg::SendUpdateStatus(int flag, CString msg, int int1, int int2) {
    LPARAM param;
    if (flag == 0 || flag == 1) {
        param = (LPARAM)strdup((LPCTSTR)msg);
        // LEAK_SCOPE here is ok since it's freed in OnIDMessage
    } else {
        MsgInts * m = new MsgInts;
        m->mint1 = int1;
        m->mint2 = int2;
        param = (LPARAM)m;
    }

//    HWND hwnd = /*AfxGetMainWnd()->*/ m_hWnd;
//    ::PostMessage(hwnd, INIT_DLG_MESSAGE, (WPARAM)flag, (LPARAM)param);
	PostMessage(INIT_DLG_MESSAGE, (WPARAM)flag, (LPARAM)param);

}

void
InitDlg::ProgressPos(int pos) {
	m_InitProgress.SetPos(pos);
	UpdateData(FALSE);
}
void
InitDlg::ProgressRange(int beg, int end) {
	m_InitProgress.SetRange(beg, end);
	UpdateData(FALSE);
}

LRESULT
InitDlg::OnIDMessage(UINT wParam, LONG lParam) {
    int flag = wParam;
    if (flag == 0) {
        char * msg = (char *) lParam;
        CString Msg = msg;
        free(msg);
        m_InitLabel = Msg;
    } else if (flag == 1) {
        char * msg = (char *) lParam;
        CString Msg = msg;
        free(msg);
        m_InitStatus = Msg;
    } else if (flag == 2) {
        MsgInts * m = (MsgInts*)lParam;
        m_InitProgress.SetRange(m->mint1, m->mint2);
        delete m;
    } else if (flag == 3) {
        MsgInts * m = (MsgInts*)lParam;
        m_InitProgress.SetPos(m->mint1);
        delete m;
    } else if (flag == 4) {
        MsgInts * m = (MsgInts*)lParam;
        delete m;
        EndDialog(0);
    }
    UpdateData(FALSE);
	UpdateWindow();
    return 0;
}

BOOL InitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if (m_abortflag) {
		m_AbortButton.EnableWindow(TRUE);
	} else {
		m_AbortButton.EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
