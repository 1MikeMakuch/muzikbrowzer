#include "StdAfx.h"
#include "MBGlobals.h"
#include "ExtendedListBox.h"
#include "MyLog.h"
#include "PlayerDlg.h"
#include "IRCodes.h"
#include "PlayerCallbacks.h"
#include "Misc.h"
#include "FileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
    


#define SCROLL_BAR_WIDTH 10
#define SCROLL_BUTTON_HEIGHT 10
#define MB_BORDER_WIDTH 2
#define MB_BORDER_HEIGHT 2

/////////////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CExtendedListBox, CListBox)
	//{{AFX_MSG_MAP(CExtendedListBox)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SYSKEYDOWN()
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
	ON_WM_PAINT()
	ON_CONTROL_REFLECT_EX(LBN_SELCHANGE, OnLbnSelchange)
END_MESSAGE_MAP()

CExtendedListBox::CExtendedListBox(BOOL usecolors, CString desc, BOOL set
								   ,PlayerCallbacks * callbacks)
	: m_UseColors(usecolors), m_id(desc),
     m_reorder(FALSE), m_ScrollButtonRect(0,0,0,0), m_ScrollHitPos(0),
     m_Capture(FALSE), m_HaveScroll(FALSE), m_SetStatus(set),
	 m_parentcallbacks(callbacks)
{
	m_UseColors = TRUE;
    CListBox::CListBox();

//    m_ScrollUpArrowCDC.CreateCompatibleDC(NULL);
//    m_ScrollDownArrowCDC.CreateCompatibleDC(NULL);
//    m_ScrollButtonCDC.CreateCompatibleDC(NULL);
	//m_ScrollSBgCDC.CreateCompatibleDC(NULL);

//	m_ScrollUpArrowBMsave = NULL;
//	m_ScrollDownArrowBMsave = NULL;
//	m_ScrollButtonBMsave = NULL;
//	m_ScrollSBgBMsave = NULL;

	m_BkNormal = crbgNormal;
	m_TxNormal = crtextNormal;
	m_BkHigh = crbgHigh;
	m_TxHigh = crtextHigh;
	m_BkSel = crbgMedium;
	m_TxSel = crtextMedium;

	m_crInnerUpperLeft = cr3dDkShadow;
	m_crInnerLowerRight = cr3dLight;
	m_crOuterUpperLeft = crBtnShadow;
	m_crOuterLowerRight= crBtnHighLight;
	
	m_UseSkin = FALSE;

	m_ScrollWidth = SCROLL_BAR_WIDTH;
	m_ScrollButtonHeight = SCROLL_BUTTON_HEIGHT;
	m_3d = FALSE;
	m_bmpOldBg = NULL;
}
CExtendedListBox::~CExtendedListBox()
{
//	::SelectObject(m_ScrollUpArrowCDC.m_hDC, m_ScrollUpArrowBMsave);
//	::SelectObject(m_ScrollDownArrowCDC.m_hDC,m_ScrollDownArrowBMsave);
//	::SelectObject(m_ScrollButtonCDC.m_hDC, m_ScrollButtonBMsave);
	//::SelectObject(m_ScrollSBgCDC.m_hDC, m_ScrollSBgBMsave);
	m_font.DeleteObject();
	m_HatchBrush.DeleteObject();
//	m_BgDC.SelectObject(m_bmpOldBg);
//	m_BgMemDC.SelectObject(m_bmpOldBgMem);
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

void CExtendedListBox::PreSubclassWindow() 
{
	CListBox::PreSubclassWindow();

   ModifyStyle(0,SS_NOTIFY|LBS_NOSEL);

	return;

   CWnd *pParent = GetParent();
   if (pParent)
   {
      CRect Rect;
      GetClientRect(&Rect);
      ClientToScreen(&Rect);
      pParent->ScreenToClient(&Rect);
      CDC *pDC = pParent->GetDC();
      m_bmpBg.CreateCompatibleBitmap(pDC,Rect.Width(),Rect.Height());
   }
}
// CExtendedListBox message handlers
void CExtendedListBox::initBgDc() {
	CRect wrect,crect;
	GetWindowRect(&wrect);STC(wrect);
	GetClientRect(crect);

	CDC *pDC = GetDC();

	CDC m_BgDC;
	m_BgDC.CreateCompatibleDC(NULL);
	m_bmpBg.DeleteObject();

	// Fill the bg bmp with the bg color
	m_bmpBg.CreateCompatibleBitmap(pDC,wrect.Width(),wrect.Height());
	m_bmpOldBg = m_BgDC.SelectObject(&m_bmpBg);
	m_BgDC.FillSolidRect(wrect,m_BkNormal);
	m_BgDC.SelectObject(m_bmpOldBg );

	// create the mem bmp for OnPaint
	m_bmpMem.DeleteObject();
	m_bmpMem.CreateCompatibleBitmap(pDC,wrect.Width(),wrect.Height());

	ReleaseDC(pDC);
	POSITION pos;
	HBRUSH hbr;
	CString key;
	for( pos = m_CtlColors.GetStartPosition(); pos != NULL; ) {
		m_CtlColors.GetNextAssoc( pos, key, (void*&)hbr);
		BOOL r = ::DeleteObject((HBRUSH)hbr);
	}
	m_CtlColors.RemoveAll();
}
void CExtendedListBox::OnPaint() 
{
	// For some reason I found this CPaintDC needed to be declared
	// before the OnPaintCallback call
	CPaintDC dc(this); // device context for painting
//	logger.ods("ELB::OnPaint:"+m_id);
	if (m_parentcallbacks
			&& m_parentcallbacks->OnPaintCallback) {
		if ((*m_parentcallbacks->OnPaintCallback)()) {
//			logger.ods("ELB::NoPaint:"+m_id);
			// Don't paint yet. Update[Artists,Albums,Songs] just
			// called our invalidate so we're about to do the actual OnPaint
			// Next time in here the OnPaintCallback will return FALSE
			// so we'll do the paint.
			return;
		}
	}
	int size = GetCount();

	CRect crect,wrect;
	GetWindowRect(wrect);STC(wrect);
	GetClientRect(crect);

	CRect irect(crect);

	int Width = irect.Width();
	int Height = irect.Height();
	
	CDC m_BgMemDC;
	m_BgMemDC.CreateCompatibleDC(NULL);
	m_bmpOldBgMem = m_BgMemDC.SelectObject(&m_bmpMem);

	CDC bgDC;
	bgDC.CreateCompatibleDC(&dc);
	CBitmap *pOldbmp = bgDC.SelectObject(&m_bmpBg);
	m_BgMemDC.BitBlt(0,0,Width,Height,&bgDC,0,0,SRCCOPY);
	bgDC.SelectObject(pOldbmp);

	irect.top = 0;
	irect.left = 0;
	irect.bottom = irect.top + GetItemHeight(0);
	irect.right = Width;
	
	if (0 == size) {
		// If we're in focus call DrawItem to paint a cursor
		CWnd *f = GetFocus();	
		HWND thisH = this->m_hWnd;
		HWND fH = NULL;
		if (f)
			fH = f->m_hWnd;
		if (fH == thisH)
			DrawItem(m_BgMemDC,0,irect,TRUE,TRUE);
	} else {
		for (int i = GetTopIndex(); i < size && irect.top <= Height;++i)
		{
			DrawItem(m_BgMemDC,i,irect,GetSel(i), this == GetFocus());
			irect.OffsetRect(0,GetItemHeight(i));
		}
	}

	dc.BitBlt(0,0,Width,Height,&m_BgMemDC,0,0,SRCCOPY);

	m_BgMemDC.SelectObject(m_bmpOldBgMem);

}
void CExtendedListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	return;
}
void CExtendedListBox::DrawItem(CDC &pDC,int idx, const CRect &rect,
								BOOL selectedbit,BOOL focusbit)
{

    COLORREF crbg, crtext;

    crbg = m_BkNormal;
    crtext = m_TxNormal;

	if (selectedbit) {
		if (focusbit) {
			crbg = m_BkSel;
			crtext = m_TxSel;
		} else {
			crbg = m_BkHigh;
			crtext = m_TxHigh;
		}
	} else {
        crbg = m_BkNormal;
        crtext = m_TxNormal;
	}

    CString data; 
	int size = GetCount();
    if (size && 0 <= idx && idx < size) {
        GetText(idx, data); 
    }
	if (selectedbit && focusbit && m_SetStatus && m_parentcallbacks) {
		if (m_parentcallbacks->statustempset) {
			(*m_parentcallbacks->statustempset)(data);
		} else if (m_parentcallbacks->SetSelected) {
			(*m_parentcallbacks->SetSelected)(data,GetItemData(idx));
		}
	}

	CRect irect(rect);
	irect.right++; // fill blue all the way to the scroll bar
    if (GetCount() && 0 <= idx && idx < GetCount()) {
    
        CBrush br(crbg);
		pDC.FillRect(&irect, &br);

        pDC.SetBkColor(crbg);
        pDC.SetTextColor(crtext);

		irect.left++; // prevent from drawing right next to border
		irect.right--;
		CFont * oldfont = pDC.SelectObject(&m_font);
        pDC.DrawText(data,
            &irect,
            DT_NOPREFIX
            | DT_SINGLELINE
            | DT_LEFT
            | DT_END_ELLIPSIS
            );
		pDC.SelectObject(oldfont);
        
    } else {
        if (focusbit) {
            crbg = m_BkSel;
        } else {
            crbg = m_BkNormal;
        }
		pDC.FillSolidRect(&irect, crbg);
    }
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

	if (m_parentcallbacks && m_parentcallbacks->Need2Erase) {
		(*m_parentcallbacks->Need2Erase)(FALSE);
	}
	if (m_parentcallbacks && m_parentcallbacks->dlg) {
		if (nChar == VK_LEFT) {
			Invalidate();
			(*m_parentcallbacks->dlg)()->PrevDlgCtrl();
		} else if (nChar == VK_RIGHT) {
			Invalidate();
			(*m_parentcallbacks->dlg)()->NextDlgCtrl();
		} else {
			CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
		}
	} else
		CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CExtendedListBox::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
// Note: relying on the fact that CPlayerDlg::m_Playlist.m_reorder
// is the only CExtendedListBox setting to TRUE!!!!!!!!!!!!!!!!!!!!
// since it calls movePlaylist[Up|Down] in move below!!!!!!!!!!!!!!

#pragma hack
	int context ;
	if (m_reorder) {
		context = nFlags & 0x02000;
		if (context == 8192 && (nChar == VK_UP || nChar == VK_DOWN)) {
			move(nChar);
			return;
		}
	}

	CListBox::OnSysKeyDown(nChar, nRepCnt, nFlags);
}

void
CExtendedListBox::move(UINT nChar) {

    int sel = GetCurSel();
    if (sel < 0) return;

    CString name;
    GetText(sel, name);
	int origsel = sel;

    if (nChar == 38) { // uparrow
        if (sel < 1) return;
        DeleteString(sel);
        InsertString(sel-1, (LPCTSTR)name);
		sel -= 1;
		if (sel < 0) sel = 0;
        SetCurSel(sel);
		if (m_parentcallbacks && m_parentcallbacks->dlg)
			(*m_parentcallbacks->dlg)()->PostMessage(MB_LISTMOVEUP,origsel,origsel);

    } else if (nChar == 40) { // down arrow
        if (sel > GetCount()-2) return;
        DeleteString(sel);
        InsertString(sel+1, (LPCTSTR)name);
		if (m_parentcallbacks && m_parentcallbacks->dlg)
			(*m_parentcallbacks->dlg)()->PostMessage(MB_LISTMOVEDN,origsel,origsel);
		sel += 1;
        SetCurSel(sel);

    }
}


void CExtendedListBox::DrawBorders()
{
    if (m_UseColors == FALSE) return;
	//Gets the Controls device context used for drawing
	CDC *pDC=GetWindowDC();
	
	//Gets the size of the control's client area
#pragma hack // fix this to use wrect properly
	CRect wrect;
	GetWindowRect(wrect);STC(wrect);
	GetClientRect(m_ClientRect);
	wrect.OffsetRect(2,2);
//	if ("Genres" == m_id) {
//		logger.ods(CRect2String(wrect));
//		logger.ods(CRect2String(m_ClientRect));
//	}

	if (m_3d) {
		pDC->Draw3dRect(wrect, m_crOuterUpperLeft, m_crOuterLowerRight);
		wrect.DeflateRect(1,1);
		pDC->Draw3dRect(wrect, m_crInnerUpperLeft, m_crInnerLowerRight);
		wrect.DeflateRect(1,1);
	}

	ReleaseDC(pDC); //Frees the DC

    if (m_HaveScroll == TRUE) {
		//wrect.OffsetRect(-2,-2);
        m_ScrollBarBorderRect = wrect;
        m_ScrollBarBorderRect.left = m_ScrollBarBorderRect.right - m_ScrollWidth;

        CRect innerRect  = m_ScrollBarBorderRect;

        m_ScrollUpArrowRect = innerRect;
        m_ScrollUpArrowRect.bottom = m_ScrollUpArrowRect.top
            + m_ScrollButtonHeight;

        m_ScrollDownArrowRect = innerRect;
        m_ScrollDownArrowRect.top = m_ScrollDownArrowRect.bottom
            - m_ScrollButtonHeight;

        m_ScrollBgRect = innerRect;
        m_ScrollBgRect.top = m_ScrollUpArrowRect.bottom /*+ 1*/;
        m_ScrollBgRect.bottom = m_ScrollDownArrowRect.top /*-1*/;

        m_ScrollButtonRect = innerRect;
        m_ScrollButtonRect.bottom = m_ScrollButtonRect.top
            + m_ScrollButtonHeight;

        m_ScrollRange = (m_ScrollBgRect.bottom - m_ScrollButtonHeight)
            - m_ScrollBgRect.top;

        m_ScrollPageUpRect = m_ScrollUpArrowRect;
        m_ScrollPageUpRect.top = m_ScrollUpArrowRect.bottom;
        m_ScrollPageDownRect = m_ScrollDownArrowRect;
        m_ScrollPageDownRect.bottom = m_ScrollDownArrowRect.top;

		m_ScrollBarBorderRect.top += m_ScrollButtonHeight;
		m_ScrollBarBorderRect.bottom -= (m_ScrollButtonHeight);
    }
}

void CExtendedListBox::DrawScrollBg(CDC * pDC)
{
    if (m_HaveScroll == FALSE) return;

	if (m_UseSkin) {
		MBUtil::BmpToDC(pDC,
			m_BmpSBg,
			0,0,
			m_ScrollBgRect.Width(),
			m_ScrollBgRect.Height(),
			m_BmpSBg.GetWidth(),
			m_BmpSBg.GetHeight(),
			LO_TILED0,
			FALSE,
			0,0);
	} else {

		CRect rect;
		rect.left = 0;
		rect.top = 0;
		rect.right = m_ScrollBgRect.Width();
		rect.bottom = m_ScrollBgRect.Height();
		pDC->FillRect(rect, &m_HatchBrush);
	}
}
void CExtendedListBox::DrawScrollArrows()
{
    if (m_HaveScroll == FALSE) return;
	CDC *pDC=GetWindowDC();
	CRect rect;
	GetClientRect(rect); //Gets the dimensions

	CBmpDC UpDC(m_BmpUp);
	CBmpDC DnDC(m_BmpDown);

    // BitBlt converts src white to foreground and src black to background color of the cdc.
    pDC->BitBlt(m_ScrollUpArrowRect.left,m_ScrollUpArrowRect.top, 
		m_BmpUp.GetWidth(), m_BmpUp.GetHeight(), &UpDC, 0,0, SRCCOPY);

    // Draw the scroll down button

    pDC->BitBlt(m_ScrollDownArrowRect.left, m_ScrollDownArrowRect.top, 
		m_BmpDown.GetWidth(), m_BmpDown.GetHeight(), &DnDC, 0,0, SRCCOPY);
    ReleaseDC(pDC);
}

void CExtendedListBox::DrawScrollButton(BOOL calc, CPoint p)
{
//	MBAutoTimer("ELB::DrawScrollButton");
	// Draw the elevator button

    if (m_HaveScroll == FALSE) return;
	CRect rect;
	GetClientRect(rect); //Gets the dimensions
    CRect saverect(rect);

	m_ItemsPerPage = 0;
    m_Count = GetCount();
	m_ItemsPerPage = 0;
	int iheight = GetItemHeight(0);
	m_Count = GetCount();
	iheight = GetItemHeight(0);
	m_ItemsPerPage = saverect.Height() / iheight;

	if (calc) {
		int topindex = GetTopIndex();
		float relpos = 0;
		if (topindex > 0) {
			int bottomindex = 0;
			int cursel = GetCurSel();
			relpos = (float) topindex / (float) (m_Count - m_ItemsPerPage);
		}
		m_ScrollButtonRect.top = m_ScrollBgRect.top
			+ (UINT) (m_ScrollRange * relpos);
	} else {
		m_ScrollButtonRect.top = p.y - m_ScrollButtonDelta;
	}

    if (m_ScrollButtonRect.top
            > m_ScrollBgRect.bottom - m_ScrollButtonHeight) {
        m_ScrollButtonRect.top = m_ScrollBgRect.bottom - m_ScrollButtonHeight;
    }
    if (m_ScrollButtonRect.top < m_ScrollBgRect.top) {
        m_ScrollButtonRect.top = m_ScrollBgRect.top;
    }
    m_ScrollButtonRect.bottom = m_ScrollButtonRect.top + m_ScrollButtonHeight;

	CBmpDC BtnDC(m_BmpButton);

	CDC *pDC=GetWindowDC();	
	CDC memdc;
	memdc.CreateCompatibleDC(pDC);
	CBitmap *oldBmp,bmp;
	bmp.CreateCompatibleBitmap(pDC,m_ScrollBarBorderRect.Width(),
		m_ScrollBarBorderRect.Height());
	oldBmp = memdc.SelectObject(&bmp);
	rect.top = 0;
	rect.left = 0;
	rect.right = m_ScrollBarBorderRect.Width();
	rect.bottom = m_ScrollBarBorderRect.Height();
	
	DrawScrollBg(&memdc);
	
	memdc.BitBlt(m_ScrollButtonRect.left-m_ScrollBarBorderRect.left, 
		m_ScrollButtonRect.top-m_ScrollBarBorderRect.top,
		m_ScrollButtonRect.Width(), m_ScrollButtonRect.Height(), 
		&BtnDC, 0,0, SRCCOPY);

	pDC->BitBlt(m_ScrollBarBorderRect.left,m_ScrollBarBorderRect.top,
		m_ScrollBarBorderRect.Width(), m_ScrollBarBorderRect.Height(), 
		&memdc,0,0,SRCCOPY);
	memdc.SelectObject(oldBmp);

	ReleaseDC(pDC);
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
            m_Count = GetCount();
            float pct = (float) p.y / (float) m_ScrollRange;
            UINT pos = (UINT)( pct * (float)(m_Count - m_ItemsPerPage));
            SetTopIndex(pos);
            DrawScrollButton(FALSE, p);
        }
    } else if (m_Capture == TRUE) {
        ReleaseCapture();
        m_Capture = FALSE;
    }
}
void CExtendedListBox::DrawScroll()
{
    if (m_HaveScroll == FALSE) return;

    DrawScrollButton();
	DrawScrollArrows();

}
#define SCROLL_HIT_NONE -1

UINT CExtendedListBox::OnNcHitTest(CPoint point) 
{
	if (m_parentcallbacks && m_parentcallbacks->Need2Erase) {
		(*m_parentcallbacks->Need2Erase)(FALSE);
	}
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
	CPoint p(0,nPos);
	ScreenToClient(&p);
	m_ScrollButtonDelta = p.y - m_ScrollButtonRect.top;
    if (m_UseColors == FALSE) {
        CListBox::OnVScroll(nSBCode, nPos, pScrollBar);
        return;
    }
    if (m_HaveScroll == FALSE) return;
    if (SB_LINEUP <= nSBCode && nSBCode <= SB_ENDSCROLL
            && nSBCode != 4 && nSBCode != 5 ) {
		SetRedraw(FALSE);
		CListBox::OnVScroll(nSBCode,nPos,pScrollBar);
		SetRedraw(TRUE);

		RedrawWindow(0,0,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
    } 
}
BOOL CExtendedListBox::OnLbnSelchange()
{
//	logger.ods("elb::OnLbnSelchange:"+m_id);
   RedrawWindow(0,0,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
   return FALSE;
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

void CExtendedListBox::Set3d(BOOL threeD) {
	m_3d = threeD;
	BOOL r;
	if (m_3d) {  // the "Border" checkbox on the ms resource editor
				// maps to WS_EX_CLIENTEDGE, a 2 pixel border
		r = ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	} else {
		r = ModifyStyleEx(WS_EX_CLIENTEDGE,0);
	}
}
void CExtendedListBox::SetColors(COLORREF bknormal, COLORREF bkhigh, COLORREF bksel,
		COLORREF txnormal, COLORREF txhigh, COLORREF txsel,
		BOOL threeD,
		COLORREF InnerUpperLeft,
		COLORREF InnerLowerRight,
		COLORREF OuterUpperLeft,
		COLORREF OuterLowerRight)
{
	m_BkNormal = bknormal;
	m_BkHigh = bkhigh;
	m_BkSel = bksel;
	m_TxNormal = txnormal;
	m_TxHigh = txhigh;
	m_TxSel = txsel;

	m_crInnerUpperLeft = InnerUpperLeft;
	m_crInnerLowerRight = InnerLowerRight;
	m_crOuterUpperLeft = OuterUpperLeft;
	m_crOuterLowerRight = OuterLowerRight;
	Set3d(threeD);
	initBgDc();

}

HBRUSH CExtendedListBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CListBox::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (nCtlColor == CTLCOLOR_LISTBOX) {
		pDC->SetBkColor(m_BkNormal);
		if (m_CtlColors.Lookup(numToString(m_BkNormal),(void *&)hbr) == 0) {
			LOGBRUSH brush;
			brush.lbColor = m_BkNormal;
			brush.lbHatch = 0;
			brush.lbStyle = 0;
			hbr = CreateBrushIndirect(&brush);
			m_CtlColors.SetAt(numToString(m_BkNormal),(void*)hbr);
		}

		return (HBRUSH)hbr;
	}
	return hbr;
}
DWORD CExtendedListBox::SetBitmaps(CDC * cdc, 
		LPCTSTR sBitmapUp, COLORREF crTransUp,
		LPCTSTR sBitmapDown, COLORREF crTransDown,
		LPCTSTR sBitmapButton, COLORREF crTransButton,
		LPCTSTR sBitmapSBg, COLORREF crTransSBg
		) {
	if (NULL == sBitmapUp) { // use stock windows like buttons
		CBitmap bmp1,bmp2,bmp3,bmp4;
		bmp1.LoadBitmap(IDB_OBM_UPARROW);
		bmp2.LoadBitmap(IDB_OBM_DNARROW);
		bmp3.LoadBitmap(IDB_OBM_SCRLBTN);
		bmp4.LoadBitmap(IDB_OBM_SCRLBG);
		m_BmpUp.SetBitmap((HBITMAP)bmp1);
		m_BmpDown.SetBitmap((HBITMAP)bmp2);
		m_BmpButton.SetBitmap((HBITMAP)bmp3);
		m_BmpSBg.SetBitmap(sBitmapSBg); // needed for getting max width below
		m_HatchBrush.DeleteObject();
		m_HatchBrush.CreatePatternBrush(&bmp4);
		m_UseSkin = FALSE;
	} else {
		m_BmpUp.Load(sBitmapUp);
		m_BmpDown.Load(sBitmapDown);
		m_BmpButton.Load(sBitmapButton);
		m_BmpSBg.Load(sBitmapSBg);
		m_UseSkin = TRUE;
	}

	m_ScrollWidth = __max(m_ScrollWidth,m_BmpUp.GetWidth());
	m_ScrollWidth = __max(m_ScrollWidth,m_BmpDown.GetWidth());
	m_ScrollWidth = __max(m_ScrollWidth,m_BmpButton.GetWidth());
	m_ScrollWidth = __max(m_ScrollWidth,m_BmpSBg.GetWidth());
	m_ScrollButtonHeight = __max(m_ScrollButtonHeight,m_BmpUp.GetHeight());
	m_ScrollButtonHeight = __max(m_ScrollButtonHeight,m_BmpDown.GetHeight());
	m_ScrollButtonHeight = __max(m_ScrollButtonHeight,m_BmpButton.GetHeight());
	
	initBgDc();	
	return 0;
}

void CExtendedListBox::OnNcCalcSize(BOOL bCalcValidRects,
                                    NCCALCSIZE_PARAMS FAR* lpncsp) 
{

    if (m_UseColors == FALSE) {
        CListBox::OnNcCalcSize(bCalcValidRects, lpncsp);
        return;
    }
	CRect rect,wrect;
	GetClientRect(rect); //Gets the dimensions
 
    UINT iheight = GetItemHeight(0);
    m_Count = GetCount();
    UINT height = rect.bottom - rect.top;
    int fit = height / iheight;

    if (m_Count > fit && m_UseColors == TRUE) {
        m_HaveScroll = TRUE;
        lpncsp->rgrc[0].right -= (m_ScrollWidth);
    } else {
        m_HaveScroll = FALSE;
        //lpncsp->rgrc[0].right += m_ScrollWidth;
    }
	
	CListBox::OnNcCalcSize(bCalcValidRects, lpncsp);

}
void CExtendedListBox::OnNcPaint() 
{
    if (m_UseColors == FALSE) {
        CListBox::OnNcPaint();
        return;
    }

	static BOOL before=FALSE;
	if (!before) { 
//If first time, the OnNcCalcSize function will be called
			SetWindowPos(NULL,0,0,0,0,SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE
                |SWP_NOZORDER); 
			before=TRUE;
	}

	DrawBorders();
    DrawScroll();


	// Do not call CListBox::OnNcPaint() for painting messages
}
BOOL CExtendedListBox::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}
void
CExtendedListBox::invalidate() {
	SetWindowPos(NULL,0,0,0,0,
		SWP_FRAMECHANGED|SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER); 	
	InvalidateRgn(NULL);


//	RedrawWindow(0,0,RDW_FRAME|RDW_INVALIDATE|RDW_UPDATENOW);
	// this forces a complete redraw. Don't think I need it.
	// I do need it, this is what makes DrawScroll get called.
//    if (m_UseColors == TRUE)
		// Causes OnNCPaint to get called.
	// Trying calling DrawScroll directly instead of calling
	// SetWindowPos to get scroll bars drawn.
}



