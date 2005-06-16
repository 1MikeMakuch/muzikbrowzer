// MessageBox.cpp : implementation file
//

#include "stdafx.h"
#include "MBGlobals.h"
#include "MBMessageBox.h"
#include "MyLog.h"
#include "VirtualControl.h"
#include "PlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// MessageBox dialog


MBMessageBoxImpl::MBMessageBoxImpl(CString & title, CString & info, 
		BOOL log, BOOL enableCancel, CWnd* pParent /*=NULL*/)
	: CDialog(MBMessageBoxImpl::IDD, pParent), m_title(title),m_info(info),
	m_log(log), m_EnableCancel(enableCancel),
	m_Control(new VirtualControl)
{
	//{{AFX_DATA_INIT(MBMessageBoxImpl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}
MBMessageBoxImpl::~MBMessageBoxImpl() {
	delete m_Control;
}


void MBMessageBoxImpl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MBMessageBoxImpl)
	DDX_Control(pDX, IDC_MESSAGE_BOX, m_MessageBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MBMessageBoxImpl, CDialog)
	//{{AFX_MSG_MAP(MBMessageBoxImpl)
	ON_EN_SETFOCUS(IDC_MESSAGE_BOX, OnSetfocusMessageBox)
	ON_EN_SETFOCUS(IDC_MESSAGEBOX_OK, OnSetfocusOk)
	ON_BN_CLICKED(IDC_MESSAGEBOX_OK, OnOK)
	ON_BN_CLICKED(IDC_MESSAGEBOX_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MBMessageBoxImpl message handlers

BOOL MBMessageBoxImpl::OnInitDialog() 
{
	CDialog::OnInitDialog();

    CString title = MUZIKBROWZER;
    title += " " + m_title;
    SetWindowText(title);
	
	CString msg;
	msg = m_info;
	if (m_log) {
		logger.log(m_info);
		msg += "\r\n\r\nThis message recorded in\r\n";
		msg += logger.location();
	}
//	m_MessageBox.SetFont();
	m_MessageBox.SetWindowText(msg);
	

    mfont.CreateFont(
        /* height */ 				15,
        /* width */ 				0,
        /* escapement */ 			0,
        /* orientation */ 			0,
        /* weight */ 				FW_NORMAL,
        /* italic */ 				FALSE,
        /* underline */ 			FALSE,
        /* strikeout */ 			0,
        /* charset */ 				DEFAULT_CHARSET,
        /* outprecision */ 			OUT_CHARACTER_PRECIS,
        /* clipprecision */ 		CLIP_CHARACTER_PRECIS,
        /* quality */ 				DEFAULT_QUALITY,
        /* pitch and family */ 		FIXED_PITCH,
        /* facename */ 				0
		);
	m_MessageBox.SetFont(&mfont);
	*m_Control = &m_MessageBox;

	CWnd * ctrl = GetDlgItem(IDC_MESSAGEBOX_CANCEL);
	ctrl->EnableWindow(m_EnableCancel);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int MBMessageBox(CString title, CString msg, BOOL log, BOOL enableCancel)
{
    MBMessageBoxImpl * mb = new MBMessageBoxImpl(title, msg, log, 
		enableCancel);
	if (thePlayer)
		*thePlayer->m_Dialog = mb;

    int r = mb->DoModal();
	if (thePlayer)
		*thePlayer->m_Dialog = thePlayer;
    delete mb;
	return r;
}

void MBMessageBoxImpl::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
void MBMessageBoxImpl::OnCancel() {
	EndDialog(0);
}
void MBMessageBoxImpl::OnExit() {
	EndDialog(0);
}

void MBMessageBoxImpl::OnSetfocusMessageBox() 
{
	*m_Control = (CEdit*)GetDlgItem(IDC_MESSAGE_BOX);
}
void MBMessageBoxImpl::OnSetfocusOk() 
{
	*m_Control = (CEdit*)GetDlgItem(IDC_MESSAGEBOX_OK);
}


