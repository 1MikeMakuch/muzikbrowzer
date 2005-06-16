// MySliderCtl.cpp : implementation file
//

#include "stdafx.h"
#include "SkinSlider.h"
#include "FileUtils.h"
#include "MyString.h"
#include "MBGlobals.h"
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
	m_dcBkCh.DeleteDC();
	m_dcBkU.DeleteDC();
	m_dcBkL.DeleteDC();
	m_dcThumbBk.DeleteDC();
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
	m_dcBkCh.DeleteDC();
	m_dcBkU.DeleteDC();
	m_dcBkL.DeleteDC();
	m_dcThumbBk.DeleteDC();
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
	if (m_Orientation == VERTICAL) {
		double ratio = (double)((double)((point.y - m_ChannelRect.top)+1) 
			/ (double)(m_ChannelRect.bottom));
		m_Pos = m_Span - int((double)(m_Span) * ratio);
	} else {
		double ratio = (double)((double)((point.x - m_ChannelRect.left)+1) 
			/ (double)(m_ChannelRect.right));
		m_Pos = /*m_Span + */int((double)(m_Span) * ratio);
	}
	SetPos(m_Pos, TRUE);
}
void CMySliderCtrl::SetPos(int pos, BOOL sendmsg) {
	if (pos < m_Min) pos = m_Min;
	if (pos > m_Max) pos = m_Max;
	m_Pos = pos;
	if (m_Orientation == VERTICAL) {
		double ratio = (double)((double)((m_Span - m_Pos)+1) 
			/ (double)(m_Span));
		int y = int(((double)m_RectSpan * ratio)) + m_ChannelRect.top;
		int h = m_ThumbRect.Height();
		y -= h/2;
		if ((m_ChannelRect.bottom - y) < h) {
			y = m_ChannelRect.bottom - h;
		}
		if (y < (m_ChannelRect.top + 3))
			y = m_ChannelRect.top;
		m_ThumbRect.top = y;
		m_ThumbRect.bottom = y + h;
	} else {
		double ratio = (double)((double)((m_Pos)+1) 
			/ (double)(m_Span));
		int x = int(((double)m_RectSpan * ratio)) + m_ChannelRect.left;
		int w = m_ThumbRect.Width();
		x -= w / 2;
		if ((m_ChannelRect.right - x) < w) {
			x = m_ChannelRect.right - (w+1);
		}
		if (x < (m_ChannelRect.left + 3))
			x = m_ChannelRect.left;
		m_ThumbRect.left = x;
		m_ThumbRect.right  = x + w;
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
//	DrawTheBitmap(&pDC, m_UpperRect, 1);
//	DrawTheBitmap(&pDC, m_LowerRect, 2);
	DrawTheBitmap(&pDC, m_ThumbRect, 3);
//	DrawTheBitmap(&pDC);
//	DrawTheThumb(&pDC);
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

	if (m_dcBkCh.m_hDC == NULL)
	{
		m_dcBkCh.CreateCompatibleDC(pDC);
		m_bmpBkCh.CreateCompatibleBitmap(pDC, m_ChannelRect.Width(), m_ChannelRect.Height());
		m_pbmpOldBkCh = m_dcBkCh.SelectObject(&m_bmpBkCh);
		m_dcBkCh.BitBlt(0, 0, m_ChannelRect.Width(), m_ChannelRect.Height(), pDC, 
			m_ChannelRect.left, m_ChannelRect.top, SRCCOPY);
		m_dcBkCh.SelectObject(m_pbmpOldBkCh);

	} // if

	m_pbmpOldBkCh = m_dcBkCh.SelectObject(&m_bmpBkCh);
	pDC->BitBlt(m_ChannelRect.left,m_ChannelRect.top, m_ChannelRect.Width(), m_ChannelRect.Height(), &m_dcBkCh, 0, 0, SRCCOPY);
	m_pbmpOldBkCh = m_dcBkCh.SelectObject(m_pbmpOldBkCh);


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
//								LPCTSTR sUpper, COLORREF crTransU, 
//								LPCTSTR sLower, COLORREF crTransL, 
							LPCTSTR sThumb, COLORREF crTransThumb)
{
    HBITMAP    hChannel		= NULL;
//	HBITMAP    hUpper		= NULL;
//	HBITMAP    hLower		= NULL;
	HBITMAP	   hThumb		= NULL;
    hChannel = (HBITMAP)::LoadImage(0, sChannel, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//	hUpper = (HBITMAP)::LoadImage(0, sUpper, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
//	hLower = (HBITMAP)::LoadImage(0, sLower, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hThumb = (HBITMAP)::LoadImage(0, sThumb, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return SetBitmaps(hChannel, crTransCh, 
//		hUpper, crTransU, 
//		hLower, crTransL, 
		hThumb, crTransThumb);
}
DWORD CMySliderCtrl::SetBitmaps(int nChannel, COLORREF crTransCh, 
//								int nUpper, COLORREF crTransU, 
//								int nLower, COLORREF crTransL, 
							int nThumb,	COLORREF crTransThumb)
{
	HBITMAP		hChannel		= NULL;
//	HBITMAP		hUpper			= NULL;
//	HBITMAP		hLower			= NULL;
	HBITMAP		hThumb			= NULL;
	HINSTANCE	hInstResource	= NULL;
	
	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmap), RT_BITMAP);

	// Load bitmap In
	hChannel = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nChannel), IMAGE_BITMAP, 0, 0, 0);
//	hUpper = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nUpper), IMAGE_BITMAP, 0, 0, 0);
//	hLower = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nLower), IMAGE_BITMAP, 0, 0, 0);
	hThumb = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nThumb), IMAGE_BITMAP, 0, 0, 0);

	return SetBitmaps(hChannel, crTransCh, 
//		hUpper, crTransU, 
//		hLower, crTransL, 
		hThumb, crTransThumb);
} // End of SetBitmaps


DWORD CMySliderCtrl::SetBitmaps(HBITMAP hChannel, COLORREF crTransCh, 
//								HBITMAP hUpper, COLORREF crTransU, 
//								HBITMAP hLower, COLORREF crTransL, 
							HBITMAP hThumb, COLORREF crTransThumb)
{
	int		nRetValue = 0;
	BITMAP	csBitmapSize;

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
#ifdef asdf
	if (hUpper)
	{
		m_csBitmaps[1].hBitmap = hUpper;
		m_csBitmaps[1].crTransparent = crTransU;
		// Get bitmap size
		nRetValue = ::GetObject(hUpper, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // if
		m_csBitmaps[1].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[1].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// Create mask for bitmap In
		m_csBitmaps[1].hMask = CreateBitmapMask(hUpper, m_csBitmaps[1].dwWidth, m_csBitmaps[1].dwHeight, crTransU);
		if (m_csBitmaps[1].hMask == NULL)
		{
			FreeResources();
			return BTNST_FAILEDMASK;
		} // if
	}

	if (hLower)
	{
		m_csBitmaps[2].hBitmap = hLower;
		m_csBitmaps[2].crTransparent = crTransL;
		// Get bitmap size
		nRetValue = ::GetObject(hLower, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return BTNST_INVALIDRESOURCE;
		} // if
		m_csBitmaps[2].dwWidth = (DWORD)csBitmapSize.bmWidth;
		m_csBitmaps[2].dwHeight = (DWORD)csBitmapSize.bmHeight;

		// Create mask for bitmap In
		m_csBitmaps[2].hMask = CreateBitmapMask(hLower, m_csBitmaps[2].dwWidth, m_csBitmaps[2].dwHeight, crTransL);
		if (m_csBitmaps[2].hMask == NULL)
		{
			FreeResources();
			return BTNST_FAILEDMASK;
		} // if
	}
#endif
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
//	FileUtil::BmpSave(hdcDest,"ssmask",dwWidth,dwHeight);

	crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
	::SetBkColor(hdcSrc,RGB(0, 0, 0));

	::BitBlt(hdcSrc, 0, 0, dwWidth, dwHeight, hdcDest, 0, 0, SRCAND);
//	FileUtil::BmpSave(hdcSrc,"ssmask",dwWidth,dwHeight);

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
//	int upperWidth = m_csBitmaps[1].dwWidth;
//	int upperHeight = m_csBitmaps[1].dwHeight;
//	int lowerWidth = m_csBitmaps[2].dwWidth;
//	int lowerHeight = m_csBitmaps[2].dwHeight;
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
	} else {
		m_RectSpan = m_ChannelRect.Width();
	}


} // End of SizeToContent
void CMySliderCtrl::getChannelRect(CRect & rect) {
	rect = m_ChannelRect;
}