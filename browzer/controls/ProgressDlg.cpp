// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "MfcPlayer.h"
#include "ProgressDlg.h"
#include "PlayerDlg.h"
#include "MyThread.h"

#include "MBGlobals.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define MB_TESTING_DELAY 00
#else
#define MB_TESTING_DELAY 0
#endif

/////////////////////////////////////////////////////////////////////////////
// ProgressDlg dialog

//int ProgressDlg::m_Abort = 0;

ProgressDlg::ProgressDlg(CDialog * pParent, int cflag, int abortflag)
	: CDialog(ProgressDlg::IDD, pParent), m_abortflag(abortflag),
	m_pThread(NULL)
{
	//{{AFX_DATA_PROGRESS(ProgressDlg)
	//}}AFX_DATA_PROGRESS

//	ProgressDlg::m_Abort = 0;
	m_Abort = 0;

//	m_InitStatus = "reading music database";
    
    if (cflag) {
	    Create(IDD_PROGRESS_DLG);
    }
	m_InitStatus.SetWordWrap(TRUE,TRUE);
	m_InitStatus2.SetWordWrap(TRUE,TRUE);
}


void ProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ProgressDlg)
	DDX_Control(pDX, IDC_PROGRESS_LABEL2, m_InitStatus2);
	DDX_Control(pDX, IDC_PROGRESS_ABORT, m_AbortButton);
	DDX_Control(pDX, IDC_PROGRESS_PROGRESS, m_InitProgress);
	DDX_Control(pDX, IDC_PROGRESS_STATUS, m_InitStatus);
	DDX_Control(pDX, IDC_PROGRESS_LABEL, m_InitLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ProgressDlg, CDialog)
	//{{AFX_MSG_MAP(ProgressDlg)
	ON_BN_CLICKED(IDC_PROGRESS_ABORT, OnInitAbort)
	//}}AFX_MSG_MAP
    
    ON_MESSAGE(MB_PROGRESS_DLG_MESSAGE, OnIDMessage)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ProgressDlg message handlers
BOOL ProgressDlg::PreTranslateMessage(MSG* pMsg)
{	// disable ESC & ENTER from killing the dialog
	// returning TRUE says msg has been processed, don't further process
    if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN) {
			return TRUE;
        }
	}
	return CDialog::PreTranslateMessage(pMsg);
}
void
ProgressDlg::SetTitle(const CString & label) {
	if (NULL == m_pThread) {
		SetWindowText(label);
	} else {
		SendUpdateStatus(5, label);
	}
}
void
ProgressDlg::SetLabel(const CString & label ) {
	if (NULL == m_pThread) {
		m_InitLabel.setText(label);
	} else {
		SendUpdateStatus(10, label);
	}
}
void
ProgressDlg::UpdateStatus(const CString & msg ) {
	if (NULL == m_pThread) {
		m_InitStatus.setText(msg);
	} else {
		SendUpdateStatus(20, msg);
	}
}
void
ProgressDlg::UpdateStatus2(const CString & msg ) {
	if (NULL == m_pThread) {
		m_InitStatus2.setText(msg);
	} else {
		SendUpdateStatus(30, msg);
	}
}
void
ProgressDlg::ProgressPos(int pos) {
	if (NULL == m_pThread) {
		m_InitProgress.SetPos(pos);
	} else {
		SendUpdateStatus(50, "", pos);
	}
}
void
ProgressDlg::ProgressRange(int beg, int end) {
	if (NULL == m_pThread) {
		m_InitProgress.SetRange(beg, end);
	} else {
		SendUpdateStatus(40, "", beg, end);
	}
}
void ProgressDlg::Abort() {
	SendUpdateStatus(0);
}
void ProgressDlg::OnInitAbort() 
{
	ProgressDlg::m_Abort = 1;
}
class MsgInts {
public:
    int mint1;
    int mint2;
};

void
ProgressDlg::SendUpdateStatus(int flag, const CString & msg, int int1, int int2) {
    LPARAM param;
	if (0 == flag) {
		param = 0;
    } else if (5 == flag || flag == 10 || flag == 20 || flag == 30) {
        param = (LPARAM)strdup((LPCTSTR)msg);
        // LEAK_SCOPE here is ok since it's freed in OnIDMessage
    } else if (40 == flag || 50 == flag) {
        MsgInts * m = new MsgInts;
        m->mint1 = int1;
        m->mint2 = int2;
        param = (LPARAM)m;
    }
	if (MB_TESTING_DELAY)
		Sleep(MB_TESTING_DELAY);
	PostMessage(MB_PROGRESS_DLG_MESSAGE, (WPARAM)flag, (LPARAM)param);
}
LRESULT
ProgressDlg::OnIDMessage(UINT wParam, LONG lParam) {
    int flag = wParam;
	if (5 == flag) {
        char * msg = (char *) lParam;
        CString Msg = msg;
        free(msg);
        SetWindowText(Msg);
    } else if (flag == 10) {
        char * msg = (char *) lParam;
        CString Msg = msg;
        free(msg);
        m_InitLabel.setText(Msg);
    } else if (flag == 20) {
        char * msg = (char *) lParam;
        CString Msg = msg;
        free(msg);
		m_InitStatus.setText(Msg);
    } else if (flag == 30) {
        char * msg = (char *) lParam;
        CString Msg = msg;
        free(msg);
		m_InitStatus2.setText(Msg);
    } else if (flag == 40) {
        MsgInts * m = (MsgInts*)lParam;
		m_lower = m->mint1;
		m_upper = m->mint2;
        m_InitProgress.SetRange32(m_lower,m_upper);

		m_lastupdate = 0;
		time(&m_starttime);
        delete m;
    } else if (flag == 50) {
        MsgInts * m = (MsgInts*)lParam;
        m_InitProgress.SetPos(m->mint1);

		if (m->mint1 > (m_upper/10)) {
			time(&m_now);
			m_elapsed = m_now - m_starttime;
			m_timeper = (float)m_elapsed / (float)m->mint1;
			m_eta = (m_timeper * (m_upper - m->mint1));
			m_etaS = "Estimated time remaining: ";
			if (m_eta >= 0)
				m_etaS += String::secs2HMS(m_eta);
			if (m_lastupdate < m_now) {
				SetLabel(m_etaS);
				m_lastupdate = m_now;
			}
		}
        delete m;
    } else if (flag == 0) {
        EndDialog(0);
    }
    UpdateData(FALSE);
	UpdateWindow();
    return 0;
}

#define CCFONTFMT "%08X %08X %08X %08X %08X %02X %02X %02X %02X %02X %02X %02X %02X "
#define CCFONTFACEPOS 69

static unsigned int italic;
static unsigned int underline;
static unsigned int strikeout;
static unsigned int charset;
static unsigned int outprecision;
static unsigned int clipprecision;
static unsigned int quality;
static unsigned int pitchandfamily;

BOOL ProgressDlg::OnInitDialog() 
{
	char deffont[] = "FFFFFFF4 00000000 00000000 00000000 00000190 00 00 00 00 00 00 00 01 Arial";
	char deffontfacename[] = "Arial";
	CDialog::OnInitDialog();

	m_InitStatus.initFont();
	m_InitLabel.initFont();

	if (thePlayer && thePlayer->pconfig()) {
		LPLOGFONT lf = thePlayer->config().getTitlesFont();
		m_InitStatus.changeFont(lf);
		m_InitStatus2.changeFont(lf);
		m_InitLabel.changeFont(lf);
	} else {
	
		LOGFONT lf;
		sscanf(deffont, CCFONTFMT, &lf.lfHeight, &lf.lfWidth, 
		&lf.lfEscapement, &lf.lfOrientation,
		&lf.lfWeight, &italic, &underline, &strikeout, &charset,
		&outprecision, &clipprecision, &quality, &pitchandfamily);

		lf.lfItalic = italic; 
		lf.lfUnderline = underline;
		lf.lfStrikeOut = strikeout; 
		lf.lfCharSet = charset;
		lf.lfOutPrecision = outprecision;
		lf.lfClipPrecision = clipprecision;
		lf.lfQuality = quality;
		lf.lfPitchAndFamily = pitchandfamily;
		strcpy(lf.lfFaceName, deffontfacename);
		m_InitStatus.changeFont(&lf);
		m_InitStatus2.changeFont(&lf);
		m_InitLabel.changeFont(&lf);
	}

	if (m_abortflag) {
		m_AbortButton.EnableWindow(TRUE);
	} else {
		m_AbortButton.EnableWindow(FALSE);
	}
	if (m_pThread) {
		m_pThread->CreateNewThread();
		m_pThread->Resume();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
