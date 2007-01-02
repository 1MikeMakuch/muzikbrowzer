// ID3Display.cpp : implementation file
//



#include "stdafx.h"
#include "ID3Display.h"
#include "MyID3LibMiscSupport.h"
#include "MyString.h"
#include "FExtension.h"
#include "WmaTagger.h"
#include "MBTag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace dami;

/////////////////////////////////////////////////////////////////////////////
// ID3Display dialog

ID3Display::ID3Display(Song song, CWnd* pParent /*=NULL*/)
	: CDialog(ID3Display::IDD, pParent), m_Song(song)
{
	//{{AFX_DATA_INIT(ID3Display)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void ID3Display::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ID3Display)
	DDX_Control(pDX, IDC_ID3_DISPLAY, m_ID3Display);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(ID3Display, CDialog)
	//{{AFX_MSG_MAP(ID3Display)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ID3Display message handlers

BOOL ID3Display::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_ID3Display.SetWindowText(displayTag(m_Song));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

CString
ID3Display::displayTag(Song song) {
	CString file = song->getId3(CS("FILE"));
	FExtension fext(file);
	if (fext == "mp3"
		|| fext == "mpg"
		|| fext == "mp1"
		|| fext == "mp2"
		|| fext == "flac"
		|| fext == "ogg") {
		MBTag tag;
		return tag.getInfo(file);
	} else if (fext.ext() == "wma") {
		WmaTag wma(file);
		return wma.getInfo();
	}
	return "";
}

