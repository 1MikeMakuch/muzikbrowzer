// CheckResult.cpp : implementation file
//

#include "stdafx.h"
#include "id3tagger.h"
#include "CheckResult.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCheckResult dialog


CCheckResult::CCheckResult(CString buttonLabel, CString & data, CWnd* pParent /*=NULL*/)
	: CDialog(CCheckResult::IDD, pParent), m_data(data), m_buttonlabel(buttonLabel)
{
	//{{AFX_DATA_INIT(CCheckResult)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCheckResult::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCheckResult)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OkButton);
	DDX_Control(pDX, IDC_EDIT_CHECK, m_Check);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCheckResult, CDialog)
	//{{AFX_MSG_MAP(CCheckResult)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCheckResult message handlers

BOOL CCheckResult::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CFont font;
	font.CreateStockObject(ANSI_FIXED_FONT);
	m_Check.SetFont(&font);
	
	m_Check.SetWindowText(m_data);

	m_OkButton.SetWindowText(m_buttonlabel);
	if (m_buttonlabel == "Ok") {
		m_Cancel.EnableWindow(FALSE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
