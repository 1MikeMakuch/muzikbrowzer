// ID3Display.cpp : implementation file
//



#include "stdafx.h"
#include "id3/tag.h"
#include "ID3Display.h"
//#include "ace/SString.h"
#include <id3/utils.h>
#include <id3/misc_support.h>
#include <id3/readers.h>
#include <id3/io_helpers.h>
#include "MyID3LibMiscSupport.h"
#include "Mp3Header.h"
#include "MyString.h"
#include "FExtension.h"
#include "oggtagger/oggtagger.h"

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
	if (fext.ext() == "mp3") {
		ID3_Tag id3;
		id3.Link(file, ID3TT_ALL);
		CString msg = ::displayTag(&id3, FALSE, file);
		msg += "\r\n";
		msg += file;
		return msg;
	} else if (fext.ext() == "ogg") {
		OggTag ogg(file);
		return ogg.getInfo();
	}
	return "";
}

