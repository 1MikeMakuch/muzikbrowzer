// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "ColorStatic.h"
#include "MBGlobals.h"
#include "MyString.h"
#include "MyLog.h"
#include "FileUtils.h"
#include "MyDC.h"
#include "MyString.h"

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
//	m_UseSkin = FALSE;
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
	m_brFg.CreateSolidBrush(m_crTextColor);

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

#define TICKERLEFTBORDER 4
	m_TickerX = TICKERLEFTBORDER;
	m_TickerY = 0;
	m_TickerXStep = 3;
	m_TickerTime = 100;
	m_NeedTicker = FALSE;
	m_TickerIt = FALSE;
	m_Ticking = FALSE;
	m_TickerDelay = 3;

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

	//CFont *oldfont = pDC->SelectObject(&m_font);

	pDC->SetTextColor(m_crTextColor);
	pDC->SetBkColor(m_crBkColor);

	m_brush.DeleteObject();
    m_brush.CreateSolidBrush(m_crBkColor);
	return (HBRUSH) m_brush;


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

    m_brFg.DeleteObject();
    m_brFg.CreateSolidBrush(m_crTextColor);

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
    m_brBkgnd.CreateSolidBrush(crBkColor);

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
	} else 	if (nIDEvent == MB_TICKER_TIMER_ID) {
		if (m_TickerStart.GetTime()) {
			m_TimeTmp = CTime::GetCurrentTime();
			if (m_TimeTmp.GetTime() > (m_TickerStart.GetTime() + m_TickerDelay)) {
				m_TickerX -= m_TickerXStep ;
				Invalidate ();
			}
		} else {
			m_TickerX -= m_TickerXStep ;
			Invalidate ();
		}
	} else {
		CStatic::OnTimer(nIDEvent);
	}
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

#define TICKSPACES "                              "
void CColorStatic::OnPaint() {
	GetClientRect(m_Rect);
	CPaintDC pDC(this);
    CMemDC memdc(&pDC, &m_Rect);

	memdc.FillSolidRect(&m_Rect, m_crBkColor  );
//	memdc.DrawEdge(m_Rect, EDGE_BUMP, BF_RECT);

    if (m_text.GetLength()) {
		CFont * oldfont = memdc.SelectObject(&m_font);
		CSize cs = memdc.GetTextExtent(m_text);

		if (cs.cx > m_Rect.Width()) {
			m_NeedTicker = TRUE;
		} else {
			m_NeedTicker = FALSE;
		}

		if (m_Ticking) {
			if (m_NeedTicker) {
				if (m_TickerX <= - cs.cx ){
					CSize cs2 = memdc.GetTextExtent(TICKSPACES);
					m_TickerX = cs2.cx;
				}
			} else {
				m_Ticking = FALSE;
				m_TickerX = TICKERLEFTBORDER;
				KillTimer (MB_TICKER_TIMER_ID);
			}
		} else {
			if (m_NeedTicker && m_TickerIt) {
				StartTicker();
			}
		}
		memdc.SetTextColor(m_crTextColor);
		memdc.SetBkMode(TRANSPARENT);
		if (m_Ticking) {
			m_text2 = m_text + TICKSPACES + m_text;
		} else {
			m_text2 = m_text;
			if (m_center && !m_NeedTicker) {
				m_TickerX = (m_Rect.Width() - cs.cx) / 2;
			}
		}
		memdc.TextOut (m_TickerX,m_TickerY,m_text2);
		memdc.SelectObject(oldfont);
	} else {
		if (m_Ticking) {
			m_Ticking = FALSE;
			KillTimer(MB_TICKER_TIMER_ID);
		}
	}
}
void
CColorStatic::setText(CString text, const BOOL center) {
	m_center = center;
    m_text = text;
	//SetItemWidth();
	// Don't do this cause it allows the control to somehow
	// paint the text occaisionally outside of OnPaint
	// which causes confusion over centered vs non-centered
	//SetWindowText(m_text);
//    OnPaint();

	if (m_TickerIt && !m_Ticking) {
		StartTicker();
	}
	RedrawWindow();
}
void
CColorStatic::StartTicker() {
	m_TickerX = TICKERLEFTBORDER;
	KillTimer (MB_TICKER_TIMER_ID);
	SetTimer(MB_TICKER_TIMER_ID,m_TickerTime,NULL);
	m_Ticking = TRUE;
	m_TickerStart = CTime::GetCurrentTime();
}
//		memdc.DrawText(m_text, m_Rect, 
//			DT_VCENTER
//			|DT_NOPREFIX
//			|centerFlag
//			|DT_LEFT
//			|DT_SINGLELINE /* |DT_NOCLIP */);

void
CColorStatic::SetItemWidth() {
	return;

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
	SetFont(&m_font);
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
//	GetClientRect(m_ClientRect);

} // End of SizeToContent
//DWORD CColorStatic::SetBitmaps(CDC * cdc, LPCTSTR nBitmap, 
//							   COLORREF crTransColor) {
//	return SkinBmp::SetBitmaps(this,cdc,nBitmap,crTransColor);
//}

#ifdef asdf
BOOL CColorStatic::OnEraseBkgnd(CDC* pDC) 
{
//	GetClientRect(m_Rect);
//	pDC->FillSolidRect(m_Rect, m_crBkColor);
//	CRect rect(m_Rect);
//	rect.DeflateRect(2,2);
//	pDC->FrameRect(rect, &m_brBkgnd);

	pDC->SetTextColor(m_crTextColor);
	pDC->SetBkColor(m_crBkColor);

	
//	return CStatic::OnEraseBkgnd(pDC);
	return TRUE;

}

#endif


