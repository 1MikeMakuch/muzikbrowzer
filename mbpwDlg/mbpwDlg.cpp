// mbpwDlg.cpp : implementation file
//

#include "stdafx.h"
#include "mbpwDlg.h"
#include "mbpw.h"
#include "MyString.h"
#define LOGOWNER
#include "MyLog.h"

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
	enum { IDD = IDD_ABOUTBOX };
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
// CMbpwDlg dialog

CMbpwDlg::CMbpwDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMbpwDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMbpwDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMbpwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMbpwDlg)
	DDX_Control(pDX, IDC_MBPW_OUTPUT, m_Output);
	DDX_Control(pDX, IDC_MBPW_INPUT, m_Input);
	DDX_Control(pDX, IDC_MBPW_GEN_BUTTON, m_Generate);
	DDX_Control(pDX, IDC_MBPW_EMAIL, m_Email);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMbpwDlg, CDialog)
	//{{AFX_MSG_MAP(CMbpwDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_MBPW_QUIT, OnMbpwQuit)
	ON_BN_CLICKED(IDC_MBPW_GEN_BUTTON, OnMbpwGenButton)
	ON_BN_CLICKED(IDC_MBPW_EMAIL, OnMbpwEmail)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMbpwDlg message handlers

BOOL CMbpwDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	//	CString msg = "Host:M0Jym-FgF-1Be-8iT-000ACff9O\r\n";
	//	msg += "Firstname:Mark\r\n";
	//	msg += "Lastname:O'Connell\r\n";
	//	msg += "Email:m_oconnell1\r\n";
	//	m_Input.SetWindowText(msg);
	//UpdateData(FALSE);

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CMbpwDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMbpwDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMbpwDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMbpwDlg::OnMbpwQuit() 
{
	EndDialog(0);
}

void CMbpwDlg::OnMbpwGenButton() 
{
	UpdateData(TRUE);
	CString input;
	m_Input.GetWindowText(input);
	if (input.GetLength() == 0) return;

	CString host,fname,lname,email,passwd;
	parseInput(host,fname,lname,email);

	char * md5 = mbpwcreate(host,fname,lname,email);
	CString password = md5;
	free(md5);

	char buf[10000];
	sprintf(buf,"%15s %s\r\n%15s %s\r\n%15s %s\r\n%15s %s\r\n%15s %s\r\n",
		MBPW_HOSTID, host, MBPW_FNAME, fname, MBPW_LNAME, lname,
		MBPW_EMAIL, email, MBPW_PW, password);

	CString output;
	output = "Each of these values must exactly match to those in your ";
	output += "Configuration/Password\r\nscreen:\r\n\r\n";
	output += buf;
	output += "\r\n\r\nIf your ";
	output += MBPW_HOSTID;
	output += " does not match ";
	output += "contact support.\r\n\r\nThe muzikbrowzer team.\r\n";

	if (host.GetLength() && fname.GetLength() && lname.GetLength()
		&& email.GetLength() && password.GetLength()) {
		m_Output.SetWindowText(output);
	}
	UpdateData(FALSE);
}

void CMbpwDlg::OnMbpwEmail() 
{
	
}
void CMbpwDlg::parseInput(CString & host, CString & fname, CString & lname,
						  CString & email) {

	CString input;
	m_Input.GetWindowText(input);
	int nlines = String::delCount(input, "\r\n");
	int i;
	CString arg;
	host = "" ; fname = "" ; lname = "" ; email = "";
	CString ohost,ofname,olname,oemail;
	for (i = 1 ; i <= nlines ; ++i) {
		arg = String::field(input, "\r\n", i);
		ohost = String::field(arg, MBPW_HOSTID, 2);
		ofname = String::field(arg, MBPW_FNAME, 2);
		olname = String::field(arg, MBPW_LNAME, 2);
		oemail = String::field(arg, MBPW_EMAIL, 2);
		if (ohost != "" && host == "") {
			host = ohost;
		} else if (ofname != "" && fname == "") {
			fname = ofname;
		} else if (olname != "" && lname == "") {
			lname = olname;
		} else if (oemail != "" && email == "") {
			email = oemail;
		}
	}

}








