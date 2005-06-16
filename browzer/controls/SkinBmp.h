#ifndef _SKIN_BMP_H
#define _SKIN_BMP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class SkinBmp
{
// Construction
public:
	SkinBmp();
	CString m_desc;

	virtual ~SkinBmp();

	DWORD SetBitmaps(CWnd *, CDC * cdc, LPCTSTR nBitmap, COLORREF crTransColor);
	DWORD SetBitmaps(CWnd *, CDC * cdc, int nBitmap, COLORREF crTransColor);
	DWORD SetBitmaps(CWnd *, CDC * cdc, HBITMAP hBitmap, COLORREF crTransColor);

	void FreeResources();
	HBITMAP CreateBitmapMask(HBITMAP & hSourceBitmap, DWORD & dwWidth, DWORD & dwHeight, COLORREF crTransColor);
	void DrawTheBitmap(CDC* pDC);
	void PaintBk(CDC* pDC);
	void SetClientRect(CRect rect) { m_ClientRect = rect;}
private:

	UINT m_nMsg;

	typedef struct _STRUCT_BITMAPS
	{
		HBITMAP		hBitmap;		// Handle to bitmap
		DWORD		dwWidth;		// Width of bitmap
		DWORD		dwHeight;		// Height of bitmap
		HBITMAP		hMask;			// Handle to mask bitmap
		HBITMAP		hOldMask;
		HBITMAP		hOldBitmap;
		COLORREF	crTransparent;	// Transparent color
	} STRUCT_BITMAPS;

	STRUCT_BITMAPS	m_Bitmaps;
	CDC			m_dcBk;
	HDC			m_hdcTmp;
	CBitmap		m_bmpBk;
	CBitmap*	m_pbmpOldBk;

	BOOL m_First;
protected:
	CRect m_ClientRect;
	BOOL m_UseSkin;

};



#endif 
