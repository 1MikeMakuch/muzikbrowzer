// TipWnd.cpp : implementation file
//

#include "stdafx.h"
#include "FontTipWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define FONT_HEIGHT -18

//////////////////////////////////////////////////////////////////////////
// � Paramax Technology Limited                                         // 
// ----------------------------                                         //
//                                                                      //
// The author accepts no liablility for injury or loss of profits       // 
// if this software is used. You willingness to use this software       //
// indicates you accept total liability                                 //
//                                                                      // 
//////////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CTipWnd

CTipWnd::CTipWnd()
{
}

CTipWnd::~CTipWnd()
{
	m_font.DeleteObject();
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CTipWnd::Create
//
// DESCRIPTION:	Creates tip window based on Parent window
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
BOOL CTipWnd::Create(CWnd* pParent)
{

	return 	CWnd::CreateEx(0,
		AfxRegisterWndClass(0),
		NULL,
		WS_BORDER|WS_POPUP,
		0,
		0,0,0,
		NULL,
		(HMENU)0);

}


BEGIN_MESSAGE_MAP(CTipWnd, CWnd)
	//{{AFX_MSG_MAP(CTipWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTipWnd message handlers

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CTipWnd::OnEraseBkgnd
//
// DESCRIPTION:	Called to paint background in infowindow color
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
BOOL CTipWnd::OnEraseBkgnd(CDC* pDC) 
{
	CBrush br(GetSysColor(COLOR_INFOBK));
	CRect rc;
	pDC->GetClipBox(rc);
	CBrush* pOldBrush = pDC->SelectObject(&br);
	pDC->PatBlt(rc.left,rc.top,rc.Width(),rc.Height(),PATCOPY);

	pDC->SelectObject(pOldBrush);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CTipWnd::ShowTips
//
// DESCRIPTION:	Shows tip window 
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CTipWnd::ShowTips(CPoint pt,const CString& str)
{


	// Create new font if the selection has changed
	if (m_strFont != str)
	{
		CSize sz;
		CDC* pDC = GetDC();
		m_strFont = str;

		LOGFONT lf;
		ZeroMemory(&lf,sizeof(lf));

		lf.lfHeight = FONT_HEIGHT;
		strcpy(lf.lfFaceName,m_strFont);
		
		// Delete old font
		m_font.DeleteObject();
		m_font.CreateFontIndirect(&lf);
		

		CFont* pFont = pDC->SelectObject(&m_font);

		// String demensions of font on screen 
		sz = pDC->GetTextExtent(m_strFont);

		// Give some space round the font
		sz.cx += 8;
		sz.cy += 8;

		pDC->SelectObject(pFont);
		ReleaseDC(pDC);
//		OutputDebugString("ShowTips\r\n");

		SetWindowPos(0,pt.x,pt.y,sz.cx,sz.cy,SWP_SHOWWINDOW|SWP_NOACTIVATE);
		RedrawWindow(); // Force immediate redraw
	}
}



void CTipWnd::OnPaint() 
{
//	OutputDebugString("CTipWnd::OnPaint\r\n");
	CPaintDC dc(this); // device context for painting
	
	CFont * oldfont = dc.SelectObject(&m_font);
	CRect rc;
	GetClientRect(rc);
	dc.DrawText(m_strFont,rc,DT_SINGLELINE|DT_VCENTER|DT_CENTER);
	dc.SelectObject(oldfont);
	// Do not call CWnd::OnPaint() for painting messages
}
