
// About.cpp : implementation file
//


#include "stdafx.h"
#include "About.h"
#include "muzikbrowzerVersion.h"
#include "MBGlobals.h"
#include "MBmessageBox.h"
#include "MyString.h"
#include "Fractal.h"

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
	MyUtil::seed();


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
    v += MUZIKBROWZER_VERSION;
    m_ProdVersion.SetWindowText(v);

	// hide the Easter Egg button
	m_EasterEgg.DrawTransparent(TRUE);
	CRect wrect;
	GetClientRect(wrect);
	CRect erect;
	m_EasterEgg.GetWindowRect(erect);
	m_EasterEgg.SetFlat(TRUE,TRUE);
	//m_EasterEgg.SetSound("c:\\DX90SDK\\Samples\\Media\\s1.wav");
	m_EasterEgg.DrawBorder(TRUE,TRUE);

	int xspan = wrect.right - (erect.Width() + 6);
	int yspan = wrect.bottom - (erect.Height() + 6);
	int x = MyUtil::random(xspan);
	int y = MyUtil::random(yspan);
	x = y = 5;
	// not working right. ASSERTs sometimes
	m_EasterEgg.ShowWindow(SW_HIDE);

	m_EasterEgg.MoveWindow(x,y,erect.Width(),erect.Height(),TRUE);
//	OutputDebugString("EE @ " + numToString(x)+" "+numToString(y)+
//		" "+numToString(xspan)+" "+numToString(yspan)+"\r\n");

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void About::OnEasteregg() 
{
//	MBMessageBox("", "Congratulations.", FALSE);
	FractalDlg dlg(m_rect);
	dlg.DoModal();
	
}
