// PictureDlg.cpp : implementation file
//

#include "stdafx.h"

#include "PictureDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPictureDlg dialog


CPictureDlg::CPictureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPictureDlg::IDD, pParent), m_width(0), m_height(0)
{
	//{{AFX_DATA_INIT(CPictureDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Create(IDD_PICTUREDLG);
	m_UpperRight = CPoint(0,0);
}


void CPictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPictureDlg)
	DDX_Control(pDX, IDC_PICTURE, m_PictureCtl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPictureDlg, CDialog)
	//{{AFX_MSG_MAP(CPictureDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPictureDlg message handlers

BOOL
CPictureDlg::LoadFromBuffer(BYTE * pBuff, int nSize, const CRect & rect) {

	BOOL r = m_Picture.LoadFromBuffer(pBuff, nSize, m_width, m_height);
	if (r) {
		m_PictureCtl.setPicture(&m_Picture);
		CPoint p(rect.right, rect.top);
		m_UpperRight = p;
	}
	return r;
}
void
CPictureDlg::resizeControls() {
    CRect dialog, picture;
//    CWnd * ctrl;
	if (m_width == 0 || m_height == 0
		|| m_UpperRight.x == 0) {
		return;
	}

    GetClientRect(dialog);
	int x = m_UpperRight.x - m_width;
	int y = m_UpperRight.y;
	MoveWindow(x,y,m_width,m_height);

	//	// App label
	//	ctrl = GetDlgItem(IDC_APP_LABEL);
	//    ctrl->GetWindowRect(applabel);
	//	int appx = (dialog.Width() / 2) - (applabel.Width() / 2);
	//	ctrl->MoveWindow(appx,buttonRow_y+2,applabel.Width(), applabel.Height(), TRUE);
}
