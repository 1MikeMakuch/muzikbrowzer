// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColorStatic.h"
#include "MBGlobals.h"
#include "MyString.h"
#include "MyLog.h"
#include "FileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CColorStatic

CColorStatic::CColorStatic()
{
	first = 1;
	m_UseSkin = FALSE;
	// Set default parent window and notification message
	m_pParent = NULL;
	m_nMsg = WM_USER;

	// By default the control is not blinking
	m_bTextBlink = FALSE;
	m_nTextBlinkStep = 0;
	m_bBkBlink = FALSE;
	m_nBkBlinkStep = 0;
	m_nTimerId = 0;

	// Set default foreground text
	m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);

	// Set default foreground text (when blinking)
	m_crBlinkTextColors[0] = m_crTextColor;
	m_crBlinkTextColors[1] = m_crTextColor;

	// Set default background text
	m_crBkColor = ::GetSysColor(COLOR_BTNFACE);

	// Set default background text (when blinking)
	m_crBlinkBkColors[0] = m_crBkColor;
	m_crBlinkBkColors[1] = m_crBkColor;

	// Set default background brush
	m_brBkgnd.CreateSolidBrush(m_crBkColor);

	// Set default background brush (when blinking)
	m_brBlinkBkgnd[0].CreateSolidBrush(m_crBkColor);
	m_brBlinkBkgnd[1].CreateSolidBrush(m_crBkColor);
	m_border = FALSE;
	m_height = 0;
	m_width = 0;
	m_HCenter = 0;

	m_crCornerColor = 0;

	m_ClrShadowDark = RGB(0,0,0);
	m_ClrShadowLight = RGB(255,255,255);


} // End of CColorStatic


CColorStatic::~CColorStatic()
{
	m_font.DeleteObject();

} // End of ~CColorStatic


BEGIN_MESSAGE_MAP(CColorStatic, CStatic)
	//{{AFX_MSG_MAP(CColorStatic)
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
    ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CColorStatic message handlers


HBRUSH CColorStatic::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// Set foreground color
	// If control is blinking (text)
	if (m_bTextBlink == TRUE)
	{
		pDC->SetTextColor(m_crBlinkTextColors[m_nTextBlinkStep]);
	}
	else
	{
		pDC->SetTextColor(m_crTextColor);
	}

	// Set background color & brush
	// If control is blinking (background)
	if (m_bBkBlink == TRUE)
	{
		pDC->SetBkColor(m_crBlinkBkColors[m_nBkBlinkStep]);
		return (HBRUSH)m_brBlinkBkgnd[m_nBkBlinkStep];
	}
	// If control is not blinking (background)
	pDC->SetBkColor(m_crBkColor);

	// Return a non-NULL brush if the parent's handler should not be called
    return (HBRUSH)m_brBkgnd;
} // End of CtlColor


void CColorStatic::OnDestroy() 
{
	CStatic::OnDestroy();
	
	// Destroy timer (if any)
	if (m_nTimerId > 0) KillTimer(m_nTimerId);

	// Destroy resources
    m_brBkgnd.DeleteObject();
    m_brBlinkBkgnd[0].DeleteObject();
    m_brBlinkBkgnd[1].DeleteObject();
} // End of OnDestroy


void CColorStatic::SetTextColor(COLORREF crTextColor)
{
	// Set new foreground color
	if (crTextColor != 0xffffffff)
	{
		m_crTextColor = crTextColor;
	}
	else // Set default foreground color
	{
		m_crTextColor = ::GetSysColor(COLOR_BTNTEXT);
	}

	// Repaint control
	//Invalidate();
} // End of SetTextColor


COLORREF CColorStatic::GetTextColor()
{
	return m_crTextColor;
} // End of GetTextColor


void CColorStatic::SetBkColor(COLORREF crBkColor)
{
	// Set new background color
	if (crBkColor != 0xffffffff)
	{
		m_crBkColor = crBkColor;
	}
	else // Set default background color
	{
		m_crBkColor = ::GetSysColor(COLOR_BTNFACE);
	}

    m_brBkgnd.DeleteObject();
    m_brBkgnd.CreateSolidBrush(m_crBkColor);

	// Repaint control
	//Invalidate();
} // End of SetBkColor


COLORREF CColorStatic::GetBkColor()
{
	return m_crBkColor;
} // End of GetBkColor


void CColorStatic::SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2)
{
	// Set new blink text colors
	m_crBlinkTextColors[0] = crBlinkTextColor1;
	m_crBlinkTextColors[1] = crBlinkTextColor2;
} // End of SetBlinkTextColors


void CColorStatic::StartTextBlink(BOOL bStart, UINT nElapse)
{
	UINT nCount;

	// Destroy any previous timer
	if (m_nTimerId > 0)	
	{
		KillTimer(m_nTimerId);
		m_nTimerId = 0;
	}

	m_bTextBlink = bStart;
	m_nTextBlinkStep = 0;

	// Start timer
	if (m_bTextBlink == TRUE) 
	{
		switch (nElapse)
		{
			case ST_FLS_SLOW:
				nCount = 2000;
				break;
			case ST_FLS_NORMAL:
				nCount = 1000;
				break;
			case ST_FLS_FAST:
				nCount = 500;
				break;
			default:
				nCount = nElapse;
				break;
		}
		m_nTimerId = SetTimer(MB_COLORSTATIC_TIMER_ID, nCount, NULL); 
	}
} // End of StartTextBlink


void CColorStatic::SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2)
{
	// Set new blink background colors
	m_crBlinkBkColors[0] = crBlinkBkColor1;
	m_crBlinkBkColors[1] = crBlinkBkColor2;

    m_brBlinkBkgnd[0].DeleteObject();
    m_brBlinkBkgnd[0].CreateSolidBrush(m_crBlinkBkColors[0]);
    m_brBlinkBkgnd[1].DeleteObject();
    m_brBlinkBkgnd[1].CreateSolidBrush(m_crBlinkBkColors[1]);

	// Repaint control
	//Invalidate();
} // End of SetBlinkBkColor


void CColorStatic::StartBkBlink(BOOL bStart, UINT nElapse)
{
	UINT nCount;

	// Destroy any previous timer
	if (m_nTimerId > 0)	
	{
		KillTimer(m_nTimerId);
		m_nTimerId = 0;
	}

	m_bBkBlink = bStart;
	m_nBkBlinkStep = 0;

	// Start timer
	if (m_bBkBlink == TRUE) 
	{
		switch (nElapse)
		{
			case ST_FLS_SLOW:
				nCount = 2000;
				break;
			case ST_FLS_NORMAL:
				nCount = 1000;
				break;
			case ST_FLS_FAST:
				nCount = 500;
				break;
			default:
				nCount = nElapse;
				break;
		}
		m_nTimerId = SetTimer(MB_COLORSTATIC_TIMER_ID, nCount, NULL);
	}
} // End of StartBkBlink


void CColorStatic::EnableNotify(CWnd* pParent, UINT nMsg)
{
	m_pParent = pParent;
	m_nMsg = nMsg;
} // End of EnableNotify


const short CColorStatic::GetVersionI()
{
	return 10; // Divide by 10 to get actual version
} // End of GetVersionI


const char* CColorStatic::GetVersionC()
{
	return "1.0";
} // End of GetVersionC


void CColorStatic::OnTimer(UINT nIDEvent) 
{
	if (nIDEvent == m_nTimerId)	
	{
		// If control is blinking (text) switch its color
		if (m_bTextBlink == TRUE) m_nTextBlinkStep = !m_nTextBlinkStep;

		// If control is blinking (background) switch its color
		if (m_bBkBlink == TRUE) m_nBkBlinkStep = !m_nBkBlinkStep;

		// If there is any blinking in action then repaint the control
		// and send the notification message (if any)
		if (m_bBkBlink == TRUE || m_bTextBlink == TRUE) 
		{
			Invalidate();
			// Send notification message only on rising blink
			if (m_pParent != NULL && (m_nBkBlinkStep == 1 || m_nTextBlinkStep == 1)) 
				m_pParent->PostMessage(m_nMsg, GetDlgCtrlID(), 0);
		}
	}
	else
	CStatic::OnTimer(nIDEvent);
} // End of OnTimer

class CUseDialogFont {
	CGdiObject* OldFont;
	CDC* pDC;
public:
	CUseDialogFont(CDC* _pDC) {
		pDC=_pDC; 
		OldFont=pDC->SelectStockObject(ANSI_VAR_FONT);
	}
	~CUseDialogFont() {
		pDC->SelectObject(OldFont);
	}
};


void CColorStatic::OnPaint() {
//	OutputDebugString("CColorStatic::OnPaint\r\n");

    CPaintDC paintdc(this);

    CRect rect;
    GetClientRect(rect);

    if (!m_UseSkin) { 
		paintdc.FillSolidRect(&rect, m_crBkColor  );
//		OutputDebugString("CS " + numToString(m_crBkColor) + "\r\n");
	}
 
	if (m_border) {
		CBrush br;
		br.CreateSolidBrush(m_crTextColor);
	//    rect.InflateRect(1,1,1,1);
		CBrush * oldbrush = paintdc.SelectObject(&br);
		paintdc.FrameRect(&rect, &br);
		paintdc.SelectObject(oldbrush);
	}

	if (m_UseSkin) {
		PaintBk(&paintdc);
		DrawTheBitmap(&paintdc);
	}

    CString S;
    getText(S);
    if (S.GetLength()) {
		int centerFlag = 0;
		if (m_HCenter) {
			centerFlag = DT_CENTER;
		}
		CFont * oldfont = paintdc.SelectObject(&m_font);
        paintdc.SetTextColor(m_crTextColor);
        rect.DeflateRect(2,0);
		paintdc.SetBkMode(TRANSPARENT);
        paintdc.DrawText(S, rect, 
			DT_VCENTER
			|centerFlag
			|DT_LEFT
			|DT_SINGLELINE /* |DT_NOCLIP */);
		paintdc.SelectObject(oldfont);
		paintdc.SetBkMode(OPAQUE);

	}


}
void
CColorStatic::SetItemWidth() {
	int length = m_text.GetLength();
	if (length == 0)
		return;

	CFont * font = GetFont();
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	font->GetLogFont(&lf);
	m_width = (lf.lfWidth + 2) * length;
}
void
CColorStatic::setText(CString text) {
    m_text = text;
	SetItemWidth();
	SetWindowText(m_text);
//    OnPaint();
	RedrawWindow();
}
void
CColorStatic::getText(CString & text) {
    text = m_text;
}
void CColorStatic::changeFont(CFont * f) {
	LOGFONT lf;
	memset(&lf, 0, sizeof(LOGFONT));
	f->GetLogFont(&lf);
	changeFont(&lf);
}
void CColorStatic::changeFont(LPLOGFONT lplf) {
    if (lplf == 0) { return; }

	m_font.DeleteObject();
    m_font.CreateFontIndirect(lplf);

     CFont * oldfont = GetFont();
     if(oldfont != NULL)
        oldfont->DeleteObject(); 

    SetItemHeight(abs(lplf->lfHeight)+3);
	SetItemWidth();
}
void CColorStatic::initFont() {
// Create a new font so we can change it later
     CFont * f = GetFont();
     LOGFONT lf;
     if(f != NULL)
         { /* Set up duplicate font */
          f->GetObject(sizeof(LOGFONT), &lf);
          m_font.CreateFontIndirect(&lf);
         } /* Set up duplicate font */
     else
         { /* Use default font spec */
          m_font.CreateStockObject(ANSI_VAR_FONT);
         } /* Use default font spec */

	 LOGFONT lf2;
	 m_font.GetLogFont(&lf2);
	 SetItemHeight(abs(lf2.lfHeight)+3);
	 SetItemWidth();
}
void CColorStatic::border(BOOL flag) {
	m_border = flag;
}

CSize CColorStatic::GetSize(CString string) {
	CDC * cdc = GetDC();
	CFont * oldfont = cdc->SelectObject(&m_font);
	CSize s = cdc->GetTextExtent(string);
	cdc->SelectObject(oldfont);
	ReleaseDC(cdc);
	return s;
}


void CColorStatic::SizeToContent()
{
	GetClientRect(m_ClientRect);

} // End of SizeToContent
DWORD CColorStatic::SetBitmaps(CDC * cdc, LPCTSTR nBitmap, 
							   COLORREF crTransColor) {
	return SkinBmp::SetBitmaps(this,cdc,nBitmap,crTransColor);
}