// GetTextFieldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetTextField.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GetTextFieldDlg dialog


GetTextFieldDlg::GetTextFieldDlg(CWnd* pParent /*=NULL*/)
	: CDialog(GetTextFieldDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(GetTextFieldDlg)
	m_sCaption = _T("");
	m_sTextField = _T("");
	//}}AFX_DATA_INIT
	m_GettingText = m_Confirming = FALSE;
}


void GetTextFieldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GetTextFieldDlg)
	DDX_Control(pDX, IDC_GETTEXTFIELD_DESC, m_cCaption);
	DDX_Text(pDX, IDC_GETTEXTFIELD, m_sTextField);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GetTextFieldDlg, CDialog)
	//{{AFX_MSG_MAP(GetTextFieldDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GetTextFieldDlg message handlers
BOOL GetTextFieldDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_Title);
	m_cCaption.SetWindowText(m_sCaption);
//	m_cTextField.SetWindowText(m_sTextField);
	if (m_GettingText) {
		m_cCaption.ShowWindow(SW_HIDE);
	} else if (m_Confirming) {
		CWnd * w = GetDlgItem(IDC_GETTEXTFIELD);
		w->ShowWindow(SW_HIDE);
	}
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void GetTextFieldDlg::OnOK() 
{
    
    UpdateData(TRUE);
	m_GettingText = m_Confirming = FALSE;
	m_sCaption = _T("");
	m_sTextField = _T("");
	CDialog::OnOK();
}
int
GetTextFieldDlg::Get(const CString & title, CString &text)
{
	m_Title = title;
	m_sTextField = text;
	m_GettingText = TRUE;
	int r = DoModal();
//	UpdateData(TRUE);
	text = m_sTextField;
	return r;
}

int
GetTextFieldDlg::Confirm(const CString &title, CString caption)
{
	m_Title = title;
	m_sCaption = caption;
	m_Confirming = TRUE;
	int r = DoModal();
	return r;
}

void GetTextFieldDlg::OnCancel() 
{
	m_GettingText = m_Confirming = FALSE;
	m_sCaption = _T("");
	m_sTextField = _T("");
	
	CDialog::OnCancel();
}
