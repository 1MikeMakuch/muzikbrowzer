// InitDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "MfcPlayer.h"
#include "InitDlg.h"
#include "PlayerDlg.h"

#include "MBGlobals.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#ifdef _DEBUG
#define MB_TESTING_DELAY 0
#else
#define MB_TESTING_DELAY 0
#endif

/////////////////////////////////////////////////////////////////////////////
// InitDlg dialog

int InitDlg::m_Abort = 0;

InitDlg::InitDlg(int cflag, int abortflag, 
				 CWinThread *pthread, CWnd* pParent /*=NULL*/)
	: CDialog(InitDlg::IDD, pParent), m_abortflag(abortflag),
	m_pThread(pthread)
{
	//{{AFX_DATA_INIT(InitDlg)
	//}}AFX_DATA_INIT

	InitDlg::m_Abort = 0;

//	m_InitStatus = "reading music database";
    
    if (cflag) {
	    Create(IDD_INIT_DIALOG);
    }
	m_InitStatus.SetWordWrap(TRUE,TRUE);
	m_InitStatus2.SetWordWrap(TRUE,TRUE);


}


void InitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(InitDlg)
	DDX_Control(pDX, IDC_INIT_LABEL2, m_InitStatus2);
	DDX_Control(pDX, IDC_INIT_ABORT, m_AbortButton);
	DDX_Control(pDX, IDC_INIT_PROGRESS, m_InitProgress);
	DDX_Control(pDX, IDC_INIT_STATUS, m_InitStatus);
	DDX_Control(pDX, IDC_INIT_LABEL, m_InitLabel);
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
InitDlg::SetLabel(const CString & label ) {
	m_InitLabel.setText(label);
	if (MB_TESTING_DELAY)
		Sleep(MB_TESTING_DELAY);
}
void
InitDlg::UpdateStatus(const CString & msg ) {
	m_InitStatus.setText(msg);
	if (MB_TESTING_DELAY)
		Sleep(MB_TESTING_DELAY);
}
void
InitDlg::UpdateStatus2(const CString & msg ) {
	m_InitStatus2.setText(msg);
	if (MB_TESTING_DELAY)
		Sleep(MB_TESTING_DELAY);
}
void InitDlg::OnInitAbort() 
{
	InitDlg::m_Abort = 1;
	
}
class MsgInts {
public:
    int mint1;
    int mint2;
};

void
InitDlg::SendUpdateStatus(int flag, const CString & msg, int int1, int int2) {
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
	if (MB_TESTING_DELAY)
		Sleep(MB_TESTING_DELAY);
	PostMessage(INIT_DLG_MESSAGE, (WPARAM)flag, (LPARAM)param);
}

void
InitDlg::ProgressPos(int pos) {
	m_InitProgress.SetPos(pos);
	if (MB_TESTING_DELAY)
		Sleep(MB_TESTING_DELAY);
}
void
InitDlg::ProgressRange(int beg, int end) {
	m_InitProgress.SetRange(beg, end);
}

LRESULT
InitDlg::OnIDMessage(UINT wParam, LONG lParam) {
    int flag = wParam;
    if (flag == 0) {
        char * msg = (char *) lParam;
        CString Msg = msg;
        free(msg);
        m_InitLabel.setText(Msg);
    } else if (flag == 1) {
        char * msg = (char *) lParam;
        CString Msg = msg;
        free(msg);
        //m_InitStatus = Msg;
		m_InitStatus.setText(Msg);
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

BOOL InitDlg::OnInitDialog() 
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
	if (m_pThread)
		m_pThread->ResumeThread();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
