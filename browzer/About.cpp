
// About.cpp : implementation file
//

#include "stdafx.h"
#include "About.h"
#include "muzikbrowzerVersion.h"
#include "MBGlobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// About dialog


About::About(CWnd* pParent /*=NULL*/)
	: CDialog(About::IDD, pParent)
{
	//{{AFX_DATA_INIT(About)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void About::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(About)
	DDX_Control(pDX, IDC_PRODVERSION, m_ProdVersion);
//	DDX_Control(pDX, IDC_PRODVERSION, m_ProdVersion);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(About, CDialog)
	//{{AFX_MSG_MAP(About)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// About message handlers

BOOL About::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_hyperlink.SetURL("http://www.muzikbrowzer.com");
    m_hyperlink.SubclassDlgItem(IDC_HYPERLINK, this); 
    CString v = MUZIKBROWZER;
    v += " version ";
    v += MUZIKBROWZER_VERSION;
    m_ProdVersion.SetWindowText(v);


//    m_hyperlink.SetWindowText("www.muzikbrowzer.com");
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
