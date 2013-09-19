// SkinBmp.cpp : implementation file
//

#include "stdafx.h"
#include "SkinBmp.h"
#include "MBGlobals.h"
#include "FileUtils.h"
#include "MyString.h"
#include "MyLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SkinBmp

SkinBmp::SkinBmp()
{
	m_dcBk.DeleteDC();
	m_bmpBk.DeleteObject();

	m_hdcTmp = NULL;

	m_Bitmaps.hBitmap = NULL;
	m_Bitmaps.hMask = NULL;
	m_Bitmaps.hOldMask = NULL;
	m_Bitmaps.hOldBitmap = NULL;
	m_First = TRUE;
	m_UseSkin = FALSE;

} // End of SkinBmp


SkinBmp::~SkinBmp()
{
	m_dcBk.DeleteDC();
	m_bmpBk.DeleteObject();
	FreeResources();
	::DeleteDC(m_hdcTmp);
} // End of ~SkinBmp

void SkinBmp::FreeResources()
{
	// Destroy bitmaps
	if (m_Bitmaps.hBitmap)	::DeleteObject(m_Bitmaps.hBitmap);
	// Destroy mask bitmaps
	if (m_Bitmaps.hMask)	::DeleteObject(m_Bitmaps.hMask);

	::ZeroMemory(&m_Bitmaps, sizeof(m_Bitmaps));
} // End of FreeResources

void SkinBmp::PaintBk(CDC* pDC)
{
	if (m_First) {
		m_First = FALSE;
//		return;
	}
#pragma hack
	m_dcBk.DeleteDC();
	m_dcBk.m_hDC = NULL;
	if (m_dcBk.m_hDC == NULL)
	{
		m_dcBk.CreateCompatibleDC(pDC);
		if (m_bmpBk.m_hObject != NULL) {
			m_bmpBk.DeleteObject();
			m_bmpBk.m_hObject = NULL;
		}
		m_bmpBk.CreateCompatibleBitmap(pDC, m_ClientRect.Width(), m_ClientRect.Height());
		m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
		m_dcBk.BitBlt(0, 0, m_ClientRect.Width(), m_ClientRect.Height(), pDC, 
			m_ClientRect.left, m_ClientRect.top, SRCCOPY);
		m_dcBk.SelectObject(m_pbmpOldBk);

	} // if
	m_pbmpOldBk = m_dcBk.SelectObject(&m_bmpBk);
	pDC->BitBlt(0, 0, m_ClientRect.Width(), m_ClientRect.Height(), &m_dcBk, 0, 0, SRCCOPY);
	m_pbmpOldBk = m_dcBk.SelectObject(m_pbmpOldBk);
//	if (m_desc == "playlist"){

//		FileUtil::BmpLog((HBITMAP)m_bmpBk, "playlistBk");
//	}


} // End of PaintBk


void SkinBmp::DrawTheBitmap(CDC* pDC)
{
//	AutoLog al("DTB\r\n");
	int width = m_Bitmaps.dwWidth;
	int height = m_Bitmaps.dwHeight;

	// AND the mask
	HDC hdcTmp = (HDC)::CreateCompatibleDC(NULL);
	HBITMAP hOldMask = (HBITMAP)::SelectObject(hdcTmp, m_Bitmaps.hMask);
	::BitBlt(pDC->m_hDC, m_ClientRect.left,m_ClientRect.top, m_ClientRect.Width(), m_ClientRect.Height(), 
			hdcTmp, 0, 0, SRCAND); // mask
	
	// paint the image

	::SelectObject(hdcTmp, m_Bitmaps.hBitmap);
	::BitBlt(pDC->m_hDC, m_ClientRect.left,m_ClientRect.top, m_ClientRect.Width(), m_ClientRect.Height(), 
			hdcTmp, 0, 0, SRCPAINT); // bmp

	::SelectObject(hdcTmp, (HBITMAP)hOldMask);
	::DeleteDC(hdcTmp);

//	if (m_desc == "playlist"){

//		FileUtil::BmpLog((HBITMAP)m_Bitmaps.hBitmap, "playlistbmp");
//	}

} // End of DrawTheBitmap

DWORD SkinBmp::SetBitmaps(CWnd * cwnd, CDC * cdc, LPCTSTR sBitmap, COLORREF crTransBitmap)
{
    HBITMAP    hBitmap	    = NULL;
    hBitmap = (HBITMAP)::LoadImage(0, sBitmap, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return SetBitmaps(cwnd, cdc, hBitmap, crTransBitmap);
}
DWORD SkinBmp::SetBitmaps(CWnd * cwnd, CDC * cdc, int nBitmap, COLORREF crTransBitmap)
{
	HBITMAP		hBitmap		= NULL;
	HINSTANCE	hInstResource	= NULL;
	
	// Find correct resource handle
	hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmap), RT_BITMAP);

	// Load bitmap In
	hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmap), IMAGE_BITMAP, 0, 0, 0);

	return SetBitmaps(cwnd, cdc, hBitmap, crTransBitmap);
} // End of SetBitmaps


DWORD SkinBmp::SetBitmaps(CWnd * cwnd, CDC * dc, HBITMAP hBitmap, COLORREF crTransBitmap)
{
	int		nRetValue = 0;
	BITMAP	csBitmapSize;

	// Free any loaded resource
	FreeResources();
	cwnd->GetClientRect(m_ClientRect);

	if (hBitmap)
	{
		// Get bitmap size
		nRetValue = ::GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
		if (nRetValue == 0)
		{
			FreeResources();
			return FALSE;
		} // if
		
		// resize bitmap into client area


		HDC hdcSrc = CreateCompatibleDC(NULL);
		HBITMAP hbmOldSrc = (HBITMAP)::SelectObject(hdcSrc, hBitmap);
		HBITMAP hbmResized = ::CreateCompatibleBitmap(dc->m_hDC,m_ClientRect.Width(),m_ClientRect.Height());
		HDC hdcTmp = CreateCompatibleDC(NULL);
		HBITMAP hbmTmp = (HBITMAP)::SelectObject(hdcTmp, hbmResized);
		
		::StretchBlt(hdcTmp,0,0,m_ClientRect.Width(),m_ClientRect.Height(),
			hdcSrc,0,0,csBitmapSize.bmWidth,csBitmapSize.bmHeight,SRCCOPY);

		::SelectObject(hdcSrc, hbmOldSrc);
		::DeleteDC(hdcSrc);
		::DeleteObject(hBitmap);
		::SelectObject(hdcTmp, hbmTmp);
		::DeleteDC(hdcTmp);
		
		hBitmap = hbmResized;

		m_Bitmaps.hBitmap = hBitmap;
		m_Bitmaps.crTransparent = crTransBitmap;

		m_Bitmaps.dwWidth = m_ClientRect.Width();
		m_Bitmaps.dwHeight = m_ClientRect.Height();

		// Create mask for bitmap In
		m_Bitmaps.hMask = CreateBitmapMask(hBitmap, m_Bitmaps.dwWidth, m_Bitmaps.dwHeight, crTransBitmap);
		if (m_Bitmaps.hMask == NULL)
		{
			FreeResources();
			return FALSE;
		} // if

	}
	m_UseSkin = TRUE;	
//	Invalidate();

	return TRUE;
} // End of SetBitmaps
HBITMAP SkinBmp::CreateBitmapMask(HBITMAP & hSourceBitmap, 
				DWORD & dwWidth, DWORD & dwHeight, COLORREF crTransColor)
{
	HBITMAP		hMask		= NULL;
	HDC			hdcSrc		= NULL;
	HDC			hdcDest		= NULL;
	HDC			hdcTmp		= NULL;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	HBITMAP		hbmNewSrc	= NULL;
	HBITMAP		hbmTmp		= NULL;
	COLORREF	crSaveBk;
	COLORREF	crSaveDestText;

	HDC hdc = ::CreateCompatibleDC(NULL);
	hMask = ::CreateCompatibleBitmap(hdc, m_ClientRect.Width(),m_ClientRect.Height());
	if (hMask == NULL)	return NULL;

	hdcSrc	= ::CreateCompatibleDC(NULL);
	hdcDest	= ::CreateCompatibleDC(NULL);
	
	hbmSrcT = (HBITMAP)::SelectObject(hdcSrc, hSourceBitmap);
	hbmDestT = (HBITMAP)::SelectObject(hdcDest, hMask);

	crSaveBk = ::SetBkColor(hdcSrc, crTransColor);


	::BitBlt(hdcDest, 0, 0, m_ClientRect.Width(),m_ClientRect.Height(),
		hdcSrc, 0, 0, SRCCOPY);

	crSaveDestText = ::SetTextColor(hdcSrc, RGB(255, 255, 255));
	::SetBkColor(hdcSrc,RGB(0, 0, 0));

	::BitBlt(hdcSrc, 0, 0, m_ClientRect.Width(),m_ClientRect.Height(),
		hdcDest, 0, 0, SRCAND);

	::SetTextColor(hdcDest, crSaveDestText);

	::SetBkColor(hdcSrc, crSaveBk);
	::SelectObject(hdcSrc, hbmSrcT);
	::SelectObject(hdcDest, hbmDestT);

	::DeleteDC(hdcSrc);
	::DeleteDC(hdcDest);
	::DeleteDC(hdc);

	return hMask;
} // End of CreateBitmapMask
