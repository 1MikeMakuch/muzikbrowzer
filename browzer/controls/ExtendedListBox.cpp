
#include "StdAfx.h"
#include "MBGlobals.h"
#include "ExtendedListBox.h"
#include "MyLog.h"
#include "PlayerDlg.h"
#include "IRCodes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CExtendedListBox, CListBox)
	//{{AFX_MSG_MAP(CExtendedListBox)
//	ON_WM_PAINT()
	//}}AFX_MSG_MAP
    ON_WM_KEYDOWN()
    ON_WM_NCCALCSIZE()
    ON_WM_NCPAINT()
    ON_WM_NCHITTEST()
    ON_WM_NCLBUTTONDOWN()
    ON_WM_TIMER()
    ON_WM_VSCROLL()
    ON_WM_MOUSEMOVE()
    ON_WM_NCLBUTTONUP()
    ON_WM_NCLBUTTONDBLCLK()
END_MESSAGE_MAP()



CExtendedListBox::CExtendedListBox(BOOL usecolors) : m_UseColors(usecolors),
     m_reorder(FALSE), m_ScrollButtonRect(0,0,0,0), m_ScrollHitPos(0),
     m_Capture(FALSE), m_HaveScroll(FALSE)
{
    CListBox::CListBox();
    m_ScrollUpArrowBM.LoadBitmap(IDB_SCROLLUPARROW);
    m_ScrollDownArrowBM.LoadBitmap(IDB_SCROLLDOWNARROW);
    m_ScrollButtonBM.LoadBitmap(IDB_SCROLLBUTTON);
    
    m_ScrollUpArrowCDC.CreateCompatibleDC(NULL);
    m_ScrollDownArrowCDC.CreateCompatibleDC(NULL);
    m_ScrollButtonCDC.CreateCompatibleDC(NULL);

    m_ScrollUpArrowBMsave = m_ScrollUpArrowCDC.SelectObject(&m_ScrollUpArrowBM);
    m_ScrollDownArrowBMsave = m_ScrollDownArrowCDC.SelectObject(&m_ScrollDownArrowBM);
    m_ScrollButtonBMsave = m_ScrollButtonCDC.SelectObject(&m_ScrollButtonBM);
}
CExtendedListBox::~CExtendedListBox()
{
    if (m_ScrollUpArrowBMsave)
		m_ScrollUpArrowCDC.SelectObject(m_ScrollUpArrowBMsave);
	if (m_ScrollDownArrowBMsave)
		m_ScrollDownArrowCDC.SelectObject(m_ScrollDownArrowBMsave);
    if (m_ScrollButtonBMsave)
		m_ScrollButtonCDC.SelectObject(m_ScrollButtonBMsave);
	m_font.DeleteObject();
}

class CUseBrush {
	CGdiObject* m_OldBrush;
	CDC* m_pDC;
public:
	CUseBrush(CDC* pDC, CGdiObject * br) {
		m_pDC=pDC; 
		m_OldBrush=pDC->SelectObject(br);
	}
	~CUseBrush() {
		m_pDC->SelectObject(m_OldBrush);
	}
};

/////////////////////////////////////////////////////////////////////////////
    static COLORREF crbgNormal = RGB(255,255,255);
    static COLORREF crtextNormal = RGB(0,0,0);
    static COLORREF crbgMedium = RGB(225,225,225);
    static COLORREF crtextMedium = RGB(0,0,255);
    static COLORREF crbgHigh = GetSysColor(COLOR_HIGHLIGHT);
    static COLORREF crtextHigh = GetSysColor(COLOR_HIGHLIGHTTEXT);
// CExtendedListBox message handlers
void CExtendedListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{

    CSize size;
	CDC dc;
	dc.Attach(lpDIS->hDC);

    if (m_UseColors == TRUE) {
        m_BkNormal = thePlayer->config().getColorBkNormal();
        m_BkHigh = thePlayer->config().getColorBkHigh();
        m_BkSel = thePlayer->config().getColorBkSel();
        m_TxNormal = thePlayer->config().getColorTxNormal();
        m_TxHigh = thePlayer->config().getColorTxHigh();
        m_TxSel = thePlayer->config().getColorTxSel();
    } else {
        m_BkNormal = crbgNormal;
        m_BkHigh = crbgMedium;
        m_BkSel = crbgHigh;
        m_TxNormal = crtextNormal;
        m_TxHigh = crtextMedium;
        m_TxSel = crtextHigh;
    }
    COLORREF crbg, crtext;

    crbg = m_BkNormal;
    crtext = m_TxNormal;

    BOOL selectChanged,focusChanged,drawentire, selectedbit,
        focusbit;
    drawentire = (lpDIS->itemAction & ODA_DRAWENTIRE);
    focusChanged = (lpDIS->itemAction & ODA_FOCUS);
    focusbit = (lpDIS->itemState & ODS_FOCUS);
    selectChanged = (lpDIS->itemAction & ODA_SELECT);
    selectedbit = (lpDIS->itemState & ODS_SELECTED);

    if (!selectedbit) {
        crbg = m_BkNormal;
        crtext = m_TxNormal;
    } else if (selectedbit && !focusbit) {
        crbg = m_BkHigh;
        crtext = m_TxHigh;
    } else if (selectedbit && focusbit) {
        crbg = m_BkSel;
        crtext = m_TxSel;
    }

    CString data; 
    int idx = lpDIS->itemID; 
    if (0 <= idx && idx <= GetCount()) {
        GetText(idx, data); 
    } else {
        data = "none";
    }

    if (0 <= idx && idx <= GetCount()) {
    
        CBrush br(crbg);
        dc.FillRect(&lpDIS->rcItem, &br);
        dc.SetBkColor(crbg);
        dc.SetTextColor(crtext);

		CFont * oldfont = dc.SelectObject(&m_font);
        dc.DrawText(data,
            &lpDIS->rcItem,
            DT_NOPREFIX
            | DT_SINGLELINE
            | DT_LEFT
            | DT_END_ELLIPSIS
            );
		dc.SelectObject(oldfont);
        
    } else {
        if (focusbit) {
            crbg = m_BkSel;
        } else {
            crbg = m_BkNormal;
        }
        CBrush br(crbg);
		dc.FrameRect(&lpDIS->rcItem, &br);
    }
    dc.Detach();
}


int CExtendedListBox::GetSelectedItemFromPoint(CPoint &ScreenPoint) {
    int sel;
    sel = GetTopIndex();

    int height = GetItemHeight(0);
    CPoint point (ScreenPoint);
    ScreenToClient(&point);

    int pos = point.y / height;
    sel += pos;

    
    return sel;
}
void CExtendedListBox::initFont() {
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
	 SetItemHeight(0, abs(lf2.lfHeight)+3);

}

int CExtendedListBox::CompareItem( LPCOMPAREITEMSTRUCT lpdata ) {
    DWORD id1 = lpdata->itemData1;
    DWORD id2 = lpdata->itemData2;
    if (id1 < id2) return -1;
    if (id1 == id2) return 0;
    if (id1 > id2) return 1;
    return 0;
}

void CExtendedListBox::changeFont(LPLOGFONT lplf) {
    if (lplf == 0) { return; }
	m_font.DeleteObject();
	m_font.CreateFontIndirect(lplf);
    SetItemHeight(0, abs(lplf->lfHeight)+3);
}

void
CExtendedListBox::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ) {

    if (nChar == VK_LEFT) {
		thePlayer->PrevDlgCtrl();
    } else if (nChar == VK_RIGHT) {
		thePlayer->NextDlgCtrl();
    } else if (m_reorder && (nChar == 85 || nChar == 117
					|| nChar == 68 || nChar == 100)) {
        move(nChar);
	} else {
		CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void
CExtendedListBox::move(UINT nChar) {
    int sel = GetCurSel();
    if (sel < 0) return;

    CString name;
    GetText(sel, name);

    if (nChar == 85 || nChar == 117) { // u or U
        if (sel < 1) return;
        DeleteString(sel);
        InsertString(sel-1, (LPCTSTR)name);
		sel -= 1;
		if (sel < 0) sel = 0;
        SetCurSel(sel);
        thePlayer->movePlaylistUp(sel);
    } else if (nChar == 68 || nChar == 100) { // d or D
        if (sel > GetCount()-2) return;
        DeleteString(sel);
        InsertString(sel+1, (LPCTSTR)name);
		sel += 1;
        SetCurSel(sel);
        thePlayer->movePlaylistDown(sel);
    }
}

#define SCROLL_BAR_WIDTH 10
#define SCROLL_BUTTON_HEIGHT 10
#define MB_BORDER_WIDTH 2
#define MB_BORDER_HEIGHT 2

void CExtendedListBox::OnNcCalcSize(BOOL bCalcValidRects,
                                    NCCALCSIZE_PARAMS FAR* lpncsp) 
{
    if (m_UseColors == FALSE) {
        CListBox::OnNcCalcSize(bCalcValidRects, lpncsp);
        return;
    }
	CRect rect;
	GetClientRect(rect); //Gets the dimensions
 
    int iheight = GetItemHeight(0);
    int count = GetCount();
    int height = rect.bottom - rect.top;
    int fit = height / iheight;

    if (count > fit && m_UseColors == TRUE) {
        m_HaveScroll = TRUE;
        lpncsp->rgrc[0].right -= (SCROLL_BAR_WIDTH + 1);
    } else {
        m_HaveScroll = FALSE;
        //lpncsp->rgrc[0].right += SCROLL_BAR_WIDTH;
    }
//    DrawBorders();
//    DrawScroll();
    
//	lpncsp->rgrc[0].bottom -= 16; //Bottom
	
	CListBox::OnNcCalcSize(bCalcValidRects, lpncsp);
//	OutputDebugString(m_id + " OnNcCalcSize()\r\n");
}
void CExtendedListBox::OnNcPaint() 
{
//	OutputDebugString("CExtendedListBox::OnNcPaint\r\n");
    if (m_UseColors == FALSE) {
        CListBox::OnNcPaint();
        return;
    }

	static BOOL before=FALSE;
	if (!before) { //If first time, the OnNcCalcSize function will be called
			SetWindowPos(NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE
                |SWP_NOZORDER); 
			before=TRUE;
	}
	DrawBorders();
    DrawScroll();
//	OutputDebugString(m_id + " OnNcPaint()\r\n");

	// Do not call CListBox::OnNcPaint() for painting messages
}

void
CExtendedListBox::invalidate() {
	
//    if (m_UseColors == TRUE)
		// Causes OnNCPaint to get called.
        SetWindowPos(NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER); 

	
//	DrawScroll();

	// Trying calling DrawScroll directly instead of calling
	// SetWindowPos to get scroll bars drawn.

}

void CExtendedListBox::DrawBorders()
{
    if (m_UseColors == FALSE) return;
	//Gets the Controls device context used for drawing
	CDC *pDC=GetDC();
	
	//Gets the size of the control's client area
	CRect rect;
	GetClientRect(rect);
    CRect srect(rect);
    rect.InflateRect(MB_BORDER_WIDTH, MB_BORDER_HEIGHT);

    CBrush br(thePlayer->config().getColorTxPanel());
	
	CBrush* pOldBrush = pDC->SelectObject(&br);
    
	pDC->FrameRect(rect, &br);
	
	pDC->SelectObject(pOldBrush);
	
	ReleaseDC(pDC); //Frees the DC

    if (m_HaveScroll == TRUE) {
        m_ScrollBarBorderRect = rect;

        m_ScrollBarBorderRect.left = m_ScrollBarBorderRect.right-1;
        m_ScrollBarBorderRect.right = (m_ScrollBarBorderRect.left +
            SCROLL_BAR_WIDTH + MB_BORDER_WIDTH);

        CRect innerRect  = m_ScrollBarBorderRect;
        innerRect.DeflateRect(1,1);

        m_ScrollUpArrowRect = innerRect;
        m_ScrollUpArrowRect.bottom = m_ScrollUpArrowRect.top
            + SCROLL_BUTTON_HEIGHT;

        m_ScrollDownArrowRect = innerRect;
        m_ScrollDownArrowRect.top = m_ScrollDownArrowRect.bottom
            - SCROLL_BUTTON_HEIGHT;

        m_ScrollBgRect = innerRect;
        m_ScrollBgRect.top = m_ScrollUpArrowRect.bottom + 1;
        m_ScrollBgRect.bottom = m_ScrollDownArrowRect.top -1;

        m_ScrollButtonRect = innerRect;
        m_ScrollButtonRect.bottom = m_ScrollButtonRect.top
            + SCROLL_BUTTON_HEIGHT;

        m_ScrollRange = (m_ScrollBgRect.bottom - SCROLL_BUTTON_HEIGHT)
            - m_ScrollBgRect.top;

        m_ScrollPageUpRect = m_ScrollUpArrowRect;
        m_ScrollPageUpRect.top = m_ScrollUpArrowRect.bottom;
        m_ScrollPageDownRect = m_ScrollDownArrowRect;
        m_ScrollPageDownRect.bottom = m_ScrollDownArrowRect.top;
    }

//OutputDebugString("DrawBorders\r\n");
}

void CExtendedListBox::DrawScrollBg()
{
    if (m_HaveScroll == FALSE) return;
	CDC *pDC=GetDC();
	CRect rect;
	GetClientRect(rect); //Gets the dimensions
 
    // Fill entire scrollbar area with hatch pattern

    CBrush brbg(thePlayer->config().getColorBkPanel());
	CBrush* pOldBrush = pDC->SelectObject(&brbg);
    pDC->FillRect(m_ScrollBgRect, &brbg);
    CBrush br(thePlayer->config().getColorTxPanel());
	
    pDC->FrameRect(m_ScrollBarBorderRect, &br);

	pDC->SelectObject(pOldBrush);

    ReleaseDC(pDC);
}
void CExtendedListBox::DrawScrollArrows()
{
    if (m_HaveScroll == FALSE) return;
	CDC *pDC=GetDC();
	CRect rect;
	GetClientRect(rect); //Gets the dimensions
//    CRect saverect(rect);

    //Draws the scroll up button
    pDC->SetBkColor(thePlayer->config().getColorBkPanel());
    pDC->SetTextColor(thePlayer->config().getColorTxPanel());


    // BitBlt converts src white to foreground and src black to background color of the cdc.
    pDC->BitBlt(m_ScrollUpArrowRect.left,m_ScrollUpArrowRect.top, rect.Width(), rect.Height(), &m_ScrollUpArrowCDC, 0,0, SRCCOPY);

    // Draw the scroll down button

    pDC->BitBlt(m_ScrollDownArrowRect.left, m_ScrollDownArrowRect.top, rect.Width(), rect.Height(), &m_ScrollDownArrowCDC, 0,0, SRCCOPY);
    ReleaseDC(pDC);
}

void CExtendedListBox::DrawScrollButton()
{
    if (m_HaveScroll == FALSE) return;
	CDC *pDC=GetDC();
	CRect rect;
	GetClientRect(rect); //Gets the dimensions
    CRect saverect(rect);

    // Draw the elevator button
    UINT cursel = GetTopIndex();
    float relpos;
    if (cursel > 0) {
        UINT count = GetCount();
        int iheight = GetItemHeight(0);
        int itemsPerPage = saverect.Height() / iheight;
        if (cursel >= (count - itemsPerPage)) {
            relpos = (float)count;
        } else {
            relpos = (float)cursel / (float)count;
        }
    } else {
        relpos = 0;
    }

    m_ScrollButtonRect.top = m_ScrollBgRect.top
        + (UINT) (m_ScrollRange * relpos);
    if (m_ScrollButtonRect.top
            > m_ScrollBgRect.bottom - SCROLL_BUTTON_HEIGHT) {
        m_ScrollButtonRect.top = m_ScrollBgRect.bottom - SCROLL_BUTTON_HEIGHT;
    }
    m_ScrollButtonRect.bottom = m_ScrollButtonRect.top + SCROLL_BUTTON_HEIGHT;
    DrawScrollBg();

    pDC->SetBkColor(thePlayer->config().getColorBkPanel());
    pDC->SetTextColor(thePlayer->config().getColorTxPanel());
    pDC->BitBlt(m_ScrollButtonRect.left, m_ScrollButtonRect.top, rect.Width(), rect.Height(), &m_ScrollButtonCDC, 0,0, SRCCOPY);

	ReleaseDC(pDC);
}

void CExtendedListBox::DrawScroll()
{
    if (m_HaveScroll == FALSE) return;

    // Fill entire scrollbar area with hatch pattern
    DrawScrollBg();
    DrawScrollArrows();
    DrawScrollButton();

}
#define SCROLL_HIT_NONE -1

UINT CExtendedListBox::OnNcHitTest(CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT where = CListBox::OnNcHitTest(point); //Obtains where the mouse is
    if (m_HaveScroll == FALSE) return where;
    
    m_ScrollPageUpRect.bottom = m_ScrollButtonRect.top;
    m_ScrollPageDownRect.top = m_ScrollButtonRect.bottom;

    m_ScrollHitPos = 0x0401;
	
	ScreenToClient(&point); // Converts the point so its relative to the client area
	
	if (where == HTNOWHERE) // If mouse is not in a place it recognizes
        if (m_ScrollUpArrowRect.PtInRect(point)) { // Check to see if the mouse is on the top
			where = HTVSCROLL;
            m_ScrollHitPos = SB_LINEUP;
        } else if (m_ScrollDownArrowRect.PtInRect(point)) { // Check to see if its on the bottom
            where=HTVSCROLL; 
            m_ScrollHitPos = SB_LINEDOWN;
        } else if (m_ScrollPageUpRect.PtInRect(point)) {
            where=HTVSCROLL;
            m_ScrollHitPos = SB_PAGEUP;
        } else if (m_ScrollButtonRect.PtInRect(point)) {
            where = HTVSCROLL;
            m_ScrollHitPos = SB_THUMBTRACK;
        } else if (m_ScrollPageDownRect.PtInRect(point)) {
            where = HTVSCROLL;
            m_ScrollHitPos = SB_PAGEDOWN;
        } else if (m_Capture == TRUE) {
            ReleaseCapture();
            m_Capture = FALSE;
        }
		
	return where; // Returns where it is
}
void
CExtendedListBox::OnNcLButtonDblClk(UINT nHitTest, CPoint point) {
    OnNcLButtonDown(nHitTest, point);
}

void CExtendedListBox::OnNcLButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
    if (m_HaveScroll == FALSE) {
        CListBox::OnNcLButtonDown(nHitTest, point);
        return;
    }
	if (nHitTest==HTVSCROLL) //Up scroll Pressed
	{
        if (SB_LINEUP <= m_ScrollHitPos && m_ScrollHitPos <= SB_ENDSCROLL) {
            if (m_ScrollHitPos == SB_THUMBPOSITION
                    || m_ScrollHitPos == SB_THUMBTRACK) {
                SetCapture();
                m_Capture = TRUE;
                SendMessage(WM_VSCROLL,MAKEWPARAM(m_ScrollHitPos,point.y),0);
            } else {
		        SendMessage(WM_VSCROLL,MAKEWPARAM(m_ScrollHitPos,0),0);

                DrawScrollButton(/*SC_UP,SC_PRESSED*/);
                SetTimer(m_ScrollHitPos,100,NULL); //Sets the timer ID 1
            }
        }
    } else if (nHitTest) {
        //Sleep(0);
    }
	
	CListBox::OnNcLButtonDown(nHitTest, point);
}
void CExtendedListBox::OnNcLButtonUp(UINT nHitTest, CPoint point) {
    if (m_Capture == TRUE)
        ReleaseCapture();
    CListBox::OnNcLButtonUp(nHitTest, point);
}
void CExtendedListBox::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
    if (m_HaveScroll == FALSE) return;	
	//Gets the state of the left button to see if it is pressed
	short result=GetKeyState(VK_LBUTTON);
	
	if ((SB_LINEUP <= nIDEvent && nIDEvent <= SB_ENDSCROLL)
            || (nIDEvent == SB_THUMBPOSITION + MB_SCROLL_MESSAGE)
            || (nIDEvent == SB_THUMBTRACK + MB_SCROLL_MESSAGE)){
		
		//If it returns negative then it is pressed
		if (result<0){
            DrawScrollButton(/*SC_DOWN,SC_NORMAL*/);
			SendMessage(WM_VSCROLL,MAKEWPARAM(nIDEvent,0),0);	
		}
		else { //No longer pressed
			
			KillTimer(nIDEvent);
			DrawScrollButton(/*SC_UP,SC_NORMAL*/);
		}
    }
    CListBox::OnTimer(nIDEvent);
} 
void
CExtendedListBox::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) {
    if (m_UseColors == FALSE) {
        CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
        return;
    }
    if (m_HaveScroll == FALSE) return;
    if (SB_LINEUP <= nSBCode && nSBCode <= SB_ENDSCROLL
            && nSBCode != 4 && nSBCode != 5 ) {
        CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
    } 
}

void
CExtendedListBox::OnMouseMove( UINT nFlags, CPoint p) {
    if (m_UseColors == FALSE) {
        CListBox::OnMouseMove(nFlags, p);
        return;
    }
    if (m_HaveScroll == FALSE) return;
    if (nFlags & MK_LBUTTON) {
        if (m_ScrollHitPos == SB_THUMBPOSITION
                || m_ScrollHitPos == SB_THUMBTRACK) {
            UINT count = GetCount();
            CRect rect;
            GetClientRect(rect);
            //p.y = p.y - ((SCROLL_BUTTON_HEIGHT * 3)
              //  + GetSystemMetrics(SM_CYEDGE));
            p.y = p.y - (SCROLL_BUTTON_HEIGHT + GetSystemMetrics(SM_CYEDGE));
            UINT range = rect.bottom - ((SCROLL_BUTTON_HEIGHT * 3)
                + GetSystemMetrics(SM_CYEDGE));
            float pct = (float) p.y / (float) range;
            UINT pos = (UINT)( pct * (float)count);
            SetTopIndex(pos);
            DrawScrollButton();            
        }
    } else if (m_Capture == TRUE) {
        ReleaseCapture();
        m_Capture = FALSE;
    }
}
void
CExtendedListBox::alphaUp() {
	int cur = GetCurSel();
	CString text;
	GetText(cur, text);
	CString ch1 = text.GetAt(0);
	int i;
	for(i = cur - 1 ; i >= 0 ; --i) {
		GetText(i, text);
		CString ch = text.GetAt(0);
		if (ch.CompareNoCase(ch1) != 0) {
			SetCurSel(i);
			return;
		}
	}
}
void
CExtendedListBox::alphaDown() {
	int cur = GetCurSel();
	CString text;
	GetText(cur, text);
	CString ch1 = text.GetAt(0);
	int i;
	int n = GetCount();
	for(i = cur + 1 ; i <  n; ++i) {
		GetText(i, text);
		CString ch = text.GetAt(0);
		if (ch.CompareNoCase(ch1) != 0) {
			SetCurSel(i);
			return;
		}
	}
}
