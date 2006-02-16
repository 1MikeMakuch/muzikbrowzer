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
		BOOL log, BOOL enableCancel, BOOL nocheck, CWnd* pParent /*=NULL*/)
	: CDialog(MBMessageBoxImpl::IDD, pParent), m_title(title),m_info(info),
	m_log(log), m_EnableCancel(enableCancel), m_Parent(pParent),
	m_Control(new VirtualControl),m_MessageBoxPtr(0), m_NoParentCheck(nocheck)
{
	//{{AFX_DATA_INIT(MBMessageBoxImpl)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_sizeGrip.cx = GetSystemMetrics(SM_CXVSCROLL);
	m_sizeGrip.cy = GetSystemMetrics(SM_CYHSCROLL);
}
MBMessageBoxImpl::~MBMessageBoxImpl() {
	delete m_Control;
}


void MBMessageBoxImpl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(MBMessageBoxImpl)
	DDX_Control(pDX, IDC_MESSAGE_BOX2, m_MessageBox2);
	DDX_Control(pDX, IDC_MESSAGE_BOX, m_MessageBox);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(MBMessageBoxImpl, CDialog)
	//{{AFX_MSG_MAP(MBMessageBoxImpl)
	ON_EN_SETFOCUS(IDC_MESSAGE_BOX, OnSetfocusMessageBox)
	ON_EN_SETFOCUS(IDC_MESSAGEBOX_OK, OnSetfocusOk)
	ON_BN_CLICKED(IDC_MESSAGEBOX_OK, OnOK)
	ON_BN_CLICKED(IDC_MESSAGEBOX_CANCEL, OnCancel)
	ON_WM_SIZE()
	ON_WM_SIZING()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MBMessageBoxImpl message handlers

BOOL MBMessageBoxImpl::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_MessageBox.ShowWindow(SW_HIDE);
	m_MessageBox2.ShowWindow(SW_HIDE);

    CString title = MUZIKBROWZER;
    title += " " + m_title;
    SetWindowText(title);
	
	CString msg;
	msg = m_info;
	if (m_log) {
		logger.log(m_info);
//		msg += "\r\n\r\nThis message recorded in\r\n";
//		msg += logger.location();
	}
	m_MessageBox.SetWindowText(msg);
	m_MessageBox2.SetWindowText(msg);

	if (thePlayer) {
		LPLOGFONT lplf = thePlayer->config().getTitlesFont();
		lplf->lfPitchAndFamily = FIXED_PITCH;
		strcpy(lplf->lfFaceName,"");
		mfont.CreateFontIndirect(lplf);
	} else {
		mfont.CreateFont(
        /* height */ 				16,
        /* width */ 				0,
        /* escapement */ 			0,
        /* orientation */ 			0,
        /* weight */ 				FW_BOLD,
        /* italic */ 				FALSE,
        /* underline */ 			FALSE,
        /* strikeout */ 			0,
        /* charset */ 				ANSI_CHARSET,
        /* outprecision */ 			OUT_CHARACTER_PRECIS,
        /* clipprecision */ 		CLIP_CHARACTER_PRECIS,
        /* quality */ 				DEFAULT_QUALITY,
        /* pitch and family */ 		FIXED_PITCH,
        /* facename */ 				0
		);
	}
	m_MessageBox.SetFont(&mfont);
	m_MessageBox2.SetFont(&mfont);

	CRect wrect,crect;
	GetClientRect(crect);
	GetWindowRect(wrect);
	m_MinWidth = wrect.Width();
	m_MinHeight = wrect.Height();	
	resizeControls();

	CRect prect,cmrect,mbrect;
	CalcMsgRect(cmrect);
	if (GetParent())
		GetParent()->GetWindowRect(prect);
	else 
		prect = CRect(0,0,640,480);

	if (m_MessageBoxPtr) {
		m_MessageBoxPtr->GetClientRect(mbrect);
	}

	int dw,dh;
	dw = wrect.Width() - mbrect.Width();
	dh = wrect.Height() - mbrect.Height();
	dw += 10;
	dh += 10;

	wrect.right = wrect.left + cmrect.Width() + dw;
	wrect.bottom = wrect.top + cmrect.Height() + dh;
	
	if (FALSE == m_NoParentCheck) {
		if (wrect.Width() > prect.Width()) 
			wrect.right = wrect.left + (prect.Width() - 100);

		if (wrect.Height() > prect.Height())
			wrect.bottom = wrect.top + (prect.Height() - 100);
	}

	if (wrect.Width() < m_MinWidth ) {
		wrect.right = wrect.left + m_MinWidth ;
	}
	if (wrect.Height() < m_MinHeight) {
		wrect.bottom = wrect.top + m_MinHeight;
	}
	MoveWindow(wrect);

	resizeControls();

	*m_Control = m_MessageBoxPtr;

	CWnd * ctrl = GetDlgItem(IDC_MESSAGEBOX_CANCEL);
	ctrl->EnableWindow(m_EnableCancel);
	if (!m_EnableCancel)
		ctrl->ShowWindow(SW_HIDE);

	InitGrip();
	ShowSizeGrip(TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

int MBMessageBox(CString title, CString msg, BOOL log, BOOL enableCancel,
				 BOOL NoParentCheck)
{
    MBMessageBoxImpl * mb = new MBMessageBoxImpl(title, msg, log, 
		enableCancel,NoParentCheck);
	if (thePlayer)
		*thePlayer->m_Dialog = mb;

    int r = mb->DoModal(); // 1 for OK, 0 for Cancel
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
	*m_Control = m_MessageBoxPtr;
}
void MBMessageBoxImpl::OnSetfocusOk() 
{
	*m_Control = (CEdit*)GetDlgItem(IDC_MESSAGEBOX_OK);
}



void MBMessageBoxImpl::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	if (!::IsWindow(m_hWnd)) { return; }
	resizeControls();
	UpdateGripPos();
}
void MBMessageBoxImpl::OnSizing(UINT fwSide, LPRECT pRect) 
{
	if (!::IsWindow(m_hWnd)) { return; }
	int left,right,top,bottom;
	left = pRect->left;
	right = pRect->right;
	top = pRect->top;
	bottom = pRect->bottom;

	if ((fwSide == 7 || fwSide == 1 || fwSide == 4)
		&& (left > right - m_MinWidth))
			left = right - m_MinWidth;

	if ((fwSide == 4 || fwSide == 3 || fwSide == 5)
		&& (top > bottom - m_MinHeight))
			top = bottom - m_MinHeight;

	if ((fwSide == 5 || fwSide == 2 || fwSide == 8)
		&& (right < left + m_MinWidth))
		right = left + m_MinWidth;
	
	if ((fwSide == 7 || fwSide == 6 || fwSide == 8)
		&& (bottom < top + m_MinHeight))
		bottom = top + m_MinHeight;

	pRect->left = left;
	pRect->right = right;
	pRect->top = top;
	pRect->bottom = bottom;
	CDialog::OnSizing(fwSide, pRect);
	UpdateGripPos();

}
void MBMessageBoxImpl::resizeControls() {

	CRect okrect,cancelrect,crect,mrect;
	if (!::IsWindow(m_hWnd)) { return; }
	GetClientRect(crect);
	CWnd * wok = GetDlgItem(IDC_MESSAGEBOX_OK);
	if (!wok) { return; }
	CWnd * wc = GetDlgItem(IDC_MESSAGEBOX_CANCEL);
	if (!wc) { return; }
	if (!::IsWindow(wok->m_hWnd)) { return; }
	if (!::IsWindow(wc->m_hWnd)) { return; }
	wok->GetClientRect(okrect);

	wc->GetClientRect(cancelrect);
	int x,y;
	x = (crect.Width() / 2) - (okrect.Width() + 20);
	y = crect.Height() - (okrect.Height() + 10);

	wok->MoveWindow(x,y,okrect.Width(),okrect.Height());

	x = (crect.Width() / 2) + (20);
	wc->MoveWindow(x,y,okrect.Width(),okrect.Height());
	wok->GetWindowRect(okrect);
	ScreenToClient(okrect);

	mrect.left = 10;
	mrect.right = crect.Width() - 10;
	mrect.top = 20;
	mrect.bottom = crect.bottom - (okrect.Height() + 10);
	mrect.bottom = okrect.top - 20;
	m_MessageBox.MoveWindow(mrect);
	m_MessageBox2.MoveWindow(mrect);

	int numlines = m_MessageBox.GetLineCount();

	CRect calcRect;
	CalcMsgRect(calcRect);

	if (calcRect.Height() > mrect.Height()
			|| calcRect.Width() > mrect.Width()) {
		m_MessageBoxPtr = &m_MessageBox;
		m_MessageBox.ShowWindow(SW_NORMAL);
		m_MessageBox2.ShowWindow(SW_HIDE);
	} else {
		m_MessageBoxPtr = &m_MessageBox2;
		m_MessageBox.ShowWindow(SW_HIDE);
		m_MessageBox2.ShowWindow(SW_NORMAL);
	}

}
void MBMessageBoxImpl::CalcMsgRect(CRect & calcRect) {
	CDC memDC;
	memDC.CreateCompatibleDC(NULL);
	CFont * oldfont = memDC.SelectObject(&mfont);
	calcRect = CRect(0,0,0,0);
	memDC.DrawText(m_info, calcRect, DT_CALCRECT | DT_LEFT | DT_NOPREFIX);
	memDC.SelectObject(oldfont);
}

void MBMessageBoxImpl::UpdateGripPos()
{
	// size-grip goes bottom right in the client area
	// (any right-to-left adjustment should go here)
	if (!::IsWindow(m_wndGrip.m_hWnd)) { return; }

	RECT rect;
	GetClientRect(&rect);

	rect.left = rect.right - m_sizeGrip.cx;
	rect.top = rect.bottom - m_sizeGrip.cy;

	// must stay below other children
	m_wndGrip.SetWindowPos(&CWnd::wndBottom, rect.left, rect.top, 0, 0,
		SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOREPOSITION);

	// maximized windows cannot be resized
	if (IsZoomed())
		m_wndGrip.EnableWindow(FALSE);
	else
		m_wndGrip.EnableWindow(TRUE);
}

void MBMessageBoxImpl::ShowSizeGrip(BOOL bShow)
{
	m_wndGrip.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

#define RSZ_GRIP_OBJ	_T("ResizableGrip")

BOOL MBMessageBoxImpl::InitGrip()
{
	CRect rect(0 , 0, m_sizeGrip.cx, m_sizeGrip.cy);

	BOOL bRet = m_wndGrip.Create(WS_CHILD | WS_CLIPSIBLINGS | SBS_SIZEGRIP,
		rect, this, 0);

	if (bRet)
	{
		// set a triangular window region
		CRgn rgnGrip, rgn;
		rgn.CreateRectRgn(0,0,1,1);
		rgnGrip.CreateRectRgnIndirect(&rect);
	
		for (int y=0; y<m_sizeGrip.cy; y++)
		{
			rgn.SetRectRgn(0, y, m_sizeGrip.cx-y, y+1);
			rgnGrip.CombineRgn(&rgnGrip, &rgn, RGN_DIFF);
		}
		m_wndGrip.SetWindowRgn((HRGN)rgnGrip.Detach(), FALSE);

		// subclass control
		::SetProp(m_wndGrip, RSZ_GRIP_OBJ,
			(HANDLE)::GetWindowLong(m_wndGrip, GWL_WNDPROC));
		::SetWindowLong(m_wndGrip, GWL_WNDPROC, (LONG)GripWindowProc);

		// update pos
		UpdateGripPos();
		ShowSizeGrip();
	}

	return bRet;
}

LRESULT MBMessageBoxImpl::GripWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC oldWndProc = (WNDPROC)::GetProp(hwnd, RSZ_GRIP_OBJ);

	switch (msg)
	{
	case WM_NCHITTEST:

		// choose proper cursor shape
		if (IsRTL(hwnd))
			return HTBOTTOMLEFT;
		else
			return HTBOTTOMRIGHT;

	case WM_DESTROY:
		
		// unsubclass
		::RemoveProp(hwnd, RSZ_GRIP_OBJ);
		::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)oldWndProc);

		break;
	}

	return ::CallWindowProc(oldWndProc, hwnd, msg, wParam, lParam);
}







