

// About.cpp : implementation file
//


#include "stdafx.h"
#include "About.h"
#include "muzikbrowzerVersion.h"
#include "MBGlobals.h"
#include "MBmessageBox.h"
#include "MyString.h"
#include "GetBlackOutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// About dialog


About::About(CRect rect, CWnd* pParent /*=NULL*/)
	: CDialog(About::IDD, pParent), m_rect(rect)
{
	//{{AFX_DATA_INIT(About)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

}


void About::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(About)
	DDX_Control(pDX, IDC_EASTEREGG, m_EasterEgg);
	DDX_Control(pDX, IDC_PRODVERSION, m_ProdVersion);
	//}}AFX_DATA_MAP
}



BEGIN_MESSAGE_MAP(About, CDialog)
	//{{AFX_MSG_MAP(About)
	ON_BN_CLICKED(IDC_EASTEREGG, OnEasteregg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// About message handlers

BOOL About::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    m_hyperlink.SetURL(MBURL);
    m_hyperlink.SubclassDlgItem(IDC_HYPERLINK, this); 
    CString v = MUZIKBROWZER;
    v += " version ";
 
	CString version = String::extract(MUZIKBROWZER_VERSION,""," ");
	CString date = String::extract(MUZIKBROWZER_VERSION," ","");
	v += version + "\r\n" + " build " + date;
    m_ProdVersion.SetWindowText(v);


	
	CString cw = "Copyright (C) 2001-";

	CString mx;
	CTime t = CTime::GetCurrentTime();
	mx = t.Format("%Y");

	cw += mx + " Pecan Ventures, LLC";
	GetDlgItem(IDC_ABOUT_COPYRIGHT)->SetWindowText(cw);

	// hide the Easter Egg button
	m_EasterEgg.DrawTransparent(TRUE);
	CRect wrect;
	GetClientRect(wrect);
	CRect erect;
	m_EasterEgg.GetWindowRect(erect);
	m_EasterEgg.SetFlat(TRUE,TRUE);

	m_EasterEgg.DrawBorder(TRUE,TRUE);

	int xspan = wrect.right - (erect.Width() + 6);
	int yspan = wrect.bottom - (erect.Height() + 6);
	int x = MyUtil::random(xspan);
	int y = MyUtil::random(yspan);
	x = y = 5;

//	m_EasterEgg.ShowWindow(SW_HIDE);

	m_EasterEgg.MoveWindow(x,y,erect.Width(),erect.Height(),TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void About::OnEasteregg() 
{
	//MBMessageBox("", "Congratulations. You found the easter egg.", FALSE);
	CGetBlackOutDlg dlg(this);
	dlg.DoModal();

	
}
