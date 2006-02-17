// ColorStatic.cpp : implementation file
//

#include "stdafx.h"
#include "GColorStatic.h"
#include "GetBlackOutDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GColorStatic

GColorStatic::GColorStatic(): mWon(FALSE)
{
	// Set default parent window and notification message
	m_pParent = NULL;

} // End of GColorStatic


GColorStatic::~GColorStatic()
{

} // End of ~GColorStatic

BEGIN_MESSAGE_MAP(GColorStatic, CStatic)
	//{{AFX_MSG_MAP(GColorStatic)
	//}}AFX_MSG_MAP
    ON_WM_PAINT()
	ON_WM_LBUTTONDBLCLK( )
	ON_WM_LBUTTONDOWN( )
	ON_WM_LBUTTONUP( )
	ON_WM_MOUSEMOVE( )
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GColorStatic message handlers


void GColorStatic::OnPaint() {

    CPaintDC paintdc(this);

    CDC * cdc = GetDC();
    if (cdc == NULL) {
//        logger.log("cdc null in GColorStatic::OnPaint()");
        return;
    }
    CRect rect;
    GetClientRect(rect);

	COLORREF clr(RGB(212,208,200));
	clr=RGB(170,170,170);



	COLORREF crblack = RGB(0,0,0);
	CBrush brush(crblack);
//	CGdiObject * oldbrush = cdc->SelectObject(brush);
	cdc->FrameRect(rect, &brush);
	rect.DeflateRect(1,1,1,1);
	cdc->FrameRect(rect, &brush);
	rect.DeflateRect(1,1,1,1);
	cdc->FrameRect(rect, &brush);
//	cdc->SelectObject(oldbrush);


	cdc->FillSolidRect( mRect.TopLeft().x, mRect.TopLeft().y,
		mRect.Width(), mRect.Height(), clr);


	ReleaseDC(cdc);



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

}

void GColorStatic::draw(CDC * cdc, int x, int y, int cx, int cy, COLORREF clr) {
//	CDC * cdc = GetDC();
	cdc->FillSolidRect( x, y, cx,cy, clr );
//	ReleaseDC(cdc);
//	RedrawWindow();
//	UpdateWindow();
}
void GColorStatic::OnLButtonDblClk( UINT nflags, CPoint point)
{
	mdialog->OnMouseDown(nflags, point);
}
void GColorStatic::OnLButtonDown( UINT nflags, CPoint point)
{
	mdialog->OnMouseDown(nflags, point);
}
void GColorStatic::OnLButtonUp( UINT nflags, CPoint point)
{
	mdialog->OnMouseUp(nflags, point);
}
void GColorStatic::OnMouseMove( UINT nflags, CPoint point)
{
	mdialog->OnMouseMove(nflags, point);
}