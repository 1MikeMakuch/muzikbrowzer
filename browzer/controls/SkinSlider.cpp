// MySliderCtl.cpp : implementation file
//

#include "stdafx.h"
#include "SkinSlider.h"
#include "FileUtils.h"
#include "MyString.h"
#include "MBGlobals.h"
#include "MyDC.h"
#include <stdlib.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSliderSK

BEGIN_MESSAGE_MAP(CMySliderCtrl, CStatic)
    //{{AFX_MSG_MAP(CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE() 
	ON_WM_NCHITTEST()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMySliderCtrl::CMySliderCtrl(CWnd * parent) : m_pParent(parent)
{
	m_Orientation = VERTICAL;
	m_Min = 0;
	m_Max = 0;
	m_Pos = 0;
	m_Dragging = FALSE;

	m_BgSet = FALSE;
	m_bmpBkCh.DeleteObject();

	m_csBitmaps[0].hBitmap = NULL;
	m_csBitmaps[1].hBitmap = NULL;
	m_csBitmaps[2].hBitmap = NULL;
	m_csBitmaps[3].hBitmap = NULL;
	m_csBitmaps[0].hMask = NULL;
	m_csBitmaps[1].hMask = NULL;
	m_csBitmaps[2].hMask = NULL;
	m_csBitmaps[3].hMask = NULL;
};
CMySliderCtrl::~CMySliderCtrl() {

	m_bmpBkCh.DeleteObject();

	FreeResources();
};
void CMySliderCtrl::SetOrientation(UINT or) {
	m_Orientation = or;
}

void CMySliderCtrl::SetRange(int min, int max, BOOL redraw)
{
	m_Min = min;
	m_Max = max;
	m_Span = m_Max - m_Min;
}

int CMySliderCtrl::GetPos() {
	return m_Pos;
}

void CMySliderCtrl::SetPos(CPoint & point) {
	int ipoint;
	if (m_Orientation == VERTICAL) {
		ipoint = point.y;
	} else {
		ipoint = point.x;
	}
	double ratio = (double)((double)((ipoint - m_ChannelMin)+1) 
		/ (double)(m_ChannelMax));
	if (m_Orientation == VERTICAL) {
		m_Pos = m_Span - int((double)(m_Span) * ratio);
	} else {
		m_Pos = int((double)(m_Span) * ratio);
	}

	SetPos(m_Pos, TRUE);
}
void CMySliderCtrl::SetPos(int pos, BOOL sendmsg) {
	if (pos < m_Min) pos = m_Min;
	if (pos > m_Max) pos = m_Max;
	m_Pos = pos;
	double ratio ;
	if (m_Orientation == VERTICAL) {
		ratio = (double)((double)((m_Span - m_Pos)+1) 
			/ (double)(m_Span));
	} else {
		ratio = (double)((double)((m_Pos)+1) 
			/ (double)(m_Span));
	}

	int y = int(((double)m_RectSpan * ratio)) + m_ChannelMin;
	int h = m_ThumbSpan;
	y -= h/2;
	if ((m_ChannelMax - y) < h) {
		y = m_ChannelMax - h;
	}
	if (y < (m_ChannelMin + 3))
		y = m_ChannelMin;

	if (m_Orientation == VERTICAL) {
		m_ThumbRect.top = y;
		m_ThumbRect.bottom = y + h;
	} else {
		m_ThumbRect.left = y;
		m_ThumbRect.right  = y + h;
	}

	Invalidate(TRUE);
	if (!sendmsg)
		return;
	if (m_Orientation == VERTICAL) {
		if (m_pParent) m_pParent->PostMessage(MB_VOLUME_MSG,0,0);
	} else {
		if (m_pParent) m_pParent->PostMessage(MB_PROGRESS_MSG,0,0);
	}
	return;
}
void CMySliderCtrl::OnLButtonDown(UINT nFlags, CPoint point) {
	if (m_ThumbRect.PtInRect(point)) {
		m_Dragging = TRUE;
//		SetCapture();
//		OutputDebugString("Slider Capture\r\n");
	} else {
		SetPos(point);
	}
	SetCapture();
	m_Dragging = TRUE;
}
void CMySliderCtrl::OnLButtonUp(UINT nFlags, CPoint point) {
	if (m_Dragging) {
		m_Dragging = FALSE;
		ReleaseCapture();
	}
}
void CMySliderCtrl::OnMouseMove(UINT nFlags, CPoint point) {
	if (m_Dragging)
		SetPos(point);
}
UINT
CMySliderCtrl::OnNcHitTest( CPoint point ) {
	CRect rect;
	GetWindowRect(rect);
	if (rect.PtInRect(point)) 
		return HTCLIENT;
	else {
		m_Dragging = FALSE;
	}
	return HTNOWHERE   ;
}

void CMySliderCtrl::OnPaint() {
	// without this first check it gets the background(s)
	// before they've been painted
	static BOOL first = TRUE;
	if (first) {
		first = FALSE;
		return;
	}

	CPaintDC pDC(this);
	PaintBk(&pDC);
	DrawTheBitmap(&pDC, m_ChannelRect, 0);
	DrawTheBitmap(&pDC, m_ThumbRect, 3);
}
void CMySliderCtrl::FreeResources(BOOL bCheckForNULL)
{
	if (bCheckForNULL)
	{
		// Destroy bitmaps
		if (m_csBitmaps[0].hBitmap)	::DeleteObject(m_csBitmaps[0].hBitmap);
		if (m_csBitmaps[1].hBitmap)	::DeleteObject(m_csBitmaps[1].hBitmap);
		if (m_csBitmaps[2].hBitmap)	::DeleteObject(m_csBitmaps[2].hBitmap);
		if (m_csBitmaps[3].hBitmap)	::DeleteObject(m_csBitmaps[3].hBitmap);

		// Destroy mask bitmaps
		if (m_csBitmaps[0].hMask)	::DeleteObject(m_csBitmaps[0].hMask);
		if (m_csBitmaps[1].hMask)	::DeleteObject(m_csBitmaps[1].hMask);
		if (m_csBitmaps[2].hMask)	::DeleteObject(m_csBitmaps[2].hMask);
		if (m_csBitmaps[3].hMask)	::DeleteObject(m_csBitmaps[3].hMask);
	} // if

	::ZeroMemory(&m_csBitmaps, sizeof(m_csBitmaps));
} // End of FreeResources

void CMySliderCtrl::PaintBk(CPaintDC* pDC)
{
	// without this first check it gets the background(s)
	// before they've been painted
	static BOOL first = TRUE;
	if (first) {
		first = FALSE;
		return;
	}

	if (!m_BgSet)
	{
		m_BgSet = TRUE;
		m_bmpBkCh.DeleteObject();
		m_bmpBkCh.CreateCompatibleBitmap(pDC, m_ChannelRect.Width(), m_ChannelRect.Height());
		CBmpDC BmpBgDC(m_bmpBkCh);
		BmpBgDC.BitBlt(0, 0, m_ChannelRect.Width(), m_ChannelRect.Height(), 
			pDC, m_ChannelRect.left, m_ChannelRect.top, SRCCOPY);
	} // if

	CBmpDC BmpBgDC(m_bmpBkCh);
	pDC->BitBlt(m_ChannelRect.left,m_ChannelRect.top, m_ChannelRect.Width(), m_ChannelRect.Height(),
		&BmpBgDC, 0, 0, SRCCOPY);

} // End of PaintBk

void CMySliderCtrl::DrawTheBitmap(CPaintDC* pDC, CRect & rect, BYTE byIndex)
{
	int width = m_csBitmaps[byIndex].dwWidth;
	int height = m_csBitmaps[byIndex].dwHeight;
	HBITMAP hMask = (HBITMAP)m_csBitmaps[byIndex].hMask;
	HBITMAP hBitmap = (HBITMAP)m_csBitmaps[byIndex].hBitmap;

	// AND the mask
	HDC hdcTmp = (HDC)::CreateCompatibleDC(NULL);
	HBITMAP hbmOld = (HBITMAP)::SelectObject(hdcTmp, hMask);
	::BitBlt(pDC->m_hDC, rect.left,rect.top, width, height, 
			hdcTmp, 0, 0, SRCAND); // mask

	// paint the image

	::SelectObject(hdcTmp, hBitmap);
	::BitBlt(pDC->m_hDC, rect.left,rect.top, width, height, 
			hdcTmp, 0, 0, SRCPAINT); // bmp

	::SelectObject(hdcTmp, (HBITMAP)hbmOld);
	::DeleteDC(hdcTmp);

} // End of DrawTheBitmap

DWORD CMySliderCtrl::SetBitmaps(LPCTSTR sChannel, COLORREF crTransCh, 
							LPCTSTR sThumb, COLORREF crTransThumb)
{
    HBITMAP    hChannel		= NULL;
	HBITMAP	   hThumb		= NULL;
    hChannel = (HBITMAP)::LoadImage(0, sChannel, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hThumb = (HBITMAP)::LoadImage(0, sThumb, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return SetBitmaps(hChannel, crTransCh, 
		hThumb, crTransThumb);
}
DWORD CMySliderCtrl::SetBitmaps(int nChannel, COLORREF crTransCh, 
							int nThumb,	COLORREF crTransThumb)
{
	HBITMAP		hChannel		= NULL;
	HBITMAP		hThumb			= NULL;
	HINSTANCE	hInstResource	= NULL;
	
	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmap), RT_BITMAP);

	// Load bitmap In
	hChannel = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nChannel), IMAGE_BITMAP, 0, 0, 0);
	hThumb = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nThumb), IMAGE_BITMAP, 0, 0, 0);

	return SetBitmaps(hChannel, crTransCh, 
		hThumb, crTransThumb);
} // End of SetBitmaps


DWORD CMySliderCtrl::SetBitmaps(HBITMAP hChannel, COLORREF crTransCh, 
							HBITMAP hThumb, COLORREF crTransThumb)
{
	int		nRetValue = 0;
	BITMAP	csBitmapSize;
	m_BgSet = FALSE;

	// Free any loaded resource
	FreeResources();

	if (hChannel)
	{
		m_csBitmaps[0].hBitmap = hChannel;
		m_csBitmaps[0].crTransparent = crTransCh;
		// Get bitmap size
		nRetValue = ::GetObject(hChannel, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // if
		m_csBitmaps[0].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[0].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// Create mask for bitmap In
		m_csBitmaps[0].hMask = CreateBitmapMask(hChannel, m_csBitmaps[0].dwWidth, m_csBitmaps[0].dwHeight, crTransCh);
		if (m_csBitmaps[0].hMask == NULL)
		{
			FreeResources();
			return BTNST_FAILEDMASK;
		} // if
	}
	if (hThumb)
	{
		m_csBitmaps[3].hBitmap = hThumb;
		m_csBitmaps[3].crTransparent = crTransThumb;
		// Get bitmap size
		nRetValue = ::GetObject(hThumb, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // if
		m_csBitmaps[3].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[3].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// Create mask for bitmap Out
		m_csBitmaps[3].hMask = CreateBitmapMask(hThumb, m_csBitmaps[3].dwWidth, m_csBitmaps[3].dwHeight, crTransThumb);
		if (m_csBitmaps[3].hMask == NULL)
		{
			FreeResources();
			return BTNST_FAILEDMASK;
		} // if
	} // if
	SizeToContent();

	Invalidate();

	return BTNST_OK;
} // End of SetBitmaps
HBITMAP CMySliderCtrl::CreateBitmapMask(HBITMAP hSourceBitmap, 
				DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor)
{
	HBITMAP		hMask		= NULL;
	HDC			hdcSrc		= NULL;
	HDC			hdcDest		= NULL;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	COLORREF	crSaveBk;
	COLORREF	crSaveDestText;

	HDC hdc = ::CreateCompatibleDC(NULL);
	hMask = ::CreateCompatibleBitmap(hdc, dwWidth, dwHeight);

	if (hMask == NULL)	return NULL;

	hdcSrc	= ::CreateCompatibleDC(NULL);
	hdcDest	= ::CreateCompatibleDC(NULL);

	hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hSourceBitmap);
	hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);

	crSaveBk = ::SetBkColor(hdcSrc, crTransColor);

	::BitBlt(hdcDest, 0, 0, dwWidth, dwHeight, hdcSrc, 0, 0, SRCCOPY);

	crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
	::SetBkColor(hdcSrc,RGB(0, 0, 0));

	::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCAND);

	SetTextColor(hdcDest, crSaveDestText);

	::SetBkColor(hdcSrc, crSaveBk);
	::SelectObject(hdcSrc, hbmSrcT);
	::SelectObject(hdcDest, hbmDestT);

	::DeleteDC(hdcSrc);
	::DeleteDC(hdcDest);
	::DeleteDC(hdc);

	return hMask;
} // End of CreateBitmapMask

void CMySliderCtrl::SizeToContent(CWnd * parent)
{
	if (parent)
		m_pParent = parent;

	int channelWidth = m_csBitmaps[0].dwWidth;
	int channelHeight = m_csBitmaps[0].dwHeight;
	int thumbWidth = m_csBitmaps[3].dwWidth;
	int thumbHeight = m_csBitmaps[3].dwHeight;
	if (m_csBitmaps[0].hBitmap)
	{
	    SetWindowPos(	NULL, -1, -1, channelWidth, channelHeight /*+ upperHeight + lowerHeight*/,
						SWP_NOMOVE | SWP_NOZORDER | SWP_NOREDRAW | SWP_NOACTIVATE);
	} // if
	int dw = channelWidth - thumbWidth;
	int dh = channelHeight - thumbHeight;
	int woffset = 0;
	int hoffset = 0;

	if (m_Orientation == VERTICAL) {
		if (dw < 0)
			thumbWidth = channelWidth;
		else
			woffset = dw / 2;

		if (dh < 5)
			thumbHeight = channelHeight / 4;

		m_ThumbRect = CRect(woffset,0,woffset+thumbWidth,thumbHeight);
	} else {
		if (dh < 0)
			thumbHeight = channelHeight;
		else
			hoffset = dh / 2;
		if (dw < 5)
			thumbWidth = channelWidth / 4;

		m_ThumbRect = CRect(0,hoffset,thumbWidth,thumbHeight+hoffset);
			
	}

	GetClientRect(m_ClientRect);
	
	m_ChannelRect = m_ClientRect;
	
	int half= m_ThumbRect.Height() / 2;

	if (m_Orientation == VERTICAL) {
		m_RectSpan = m_ChannelRect.Height();
		m_ChannelMin = m_ChannelRect.top;
		m_ChannelMax = m_ChannelRect.bottom;
		m_ThumbSpan = m_ThumbRect.Height();
	} else {
		m_RectSpan = m_ChannelRect.Width();
		m_ChannelMin = m_ChannelRect.left;
		m_ChannelMax = m_ChannelRect.right;
		m_ThumbSpan = m_ThumbRect.Width();
	}


} // End of SizeToContent
void CMySliderCtrl::getChannelRect(CRect & rect) {
	rect = m_ChannelRect;
}