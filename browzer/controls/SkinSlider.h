
#ifndef __SKIN_SLIDER_H__
#define __SKIN_SLIDER_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


// Return values
#ifndef	BTNST_OK
#define	BTNST_OK						0
#endif
#ifndef	BTNST_INVALIDRESOURCE
#define	BTNST_INVALIDRESOURCE			1
#endif
#ifndef	BTNST_FAILEDMASK
#define	BTNST_FAILEDMASK				2
#endif
#ifndef	BTNST_INVALIDINDEX
#define	BTNST_INVALIDINDEX				3
#endif
#ifndef	BTNST_INVALIDALIGN
#define	BTNST_INVALIDALIGN				4
#endif
#ifndef	BTNST_BADPARAM
#define	BTNST_BADPARAM					5
#endif
#ifndef	BTNST_INVALIDPRESSEDSTYLE
#define	BTNST_INVALIDPRESSEDSTYLE		6
#endif

// Dummy identifier for grayscale icon
#ifndef	BTNST_AUTO_GRAY
#define	BTNST_AUTO_GRAY					(HICON)(0xffffffff - 1L)
#endif
// Dummy identifier for 15% darker icon
#ifndef	BTNST_AUTO_DARKER
#define	BTNST_AUTO_DARKER				(HICON)(0xffffffff - 2L)
#endif

class CMySliderCtrl : public CStatic
{
public:
	CMySliderCtrl(CWnd * parent=NULL);
	~CMySliderCtrl();

	void SetRange(int nMin, int nMax, BOOL bRedraw = FALSE);
	int GetPos() ;
	void SetPos(int nPos, BOOL sendMsg = FALSE);
	void SetPos(CPoint &point);
	void OnDraw();
	DWORD SetBitmaps(LPCTSTR nChannel, COLORREF crTransCh, 
//		LPCTSTR nUpper, COLORREF crTransU, 
//		LPCTSTR nLower, COLORREF crTransL, 
		LPCTSTR nThumb, COLORREF crTransThumb);
	DWORD SetBitmaps(int nBitmap, COLORREF crTransCh, 
//		int nUpper, COLORREF crTransU,
//		int nLower, COLORREF crTransL,
		int nThumb, COLORREF crTransThumb);
	DWORD SetBitmaps(HBITMAP hBitmap, COLORREF crTransCh, 
//		HBITMAP hUpper, COLORREF crTransU, 
//		HBITMAP hLower, COLORREF crTransL, 
		HBITMAP hThumb, COLORREF crTransThumb);
	void SizeToContent(CWnd * parent = NULL);
	BOOL m_Dragging;
	void getChannelRect(CRect & rect);

	enum { VERTICAL, HORIZONTAL } ;
	void SetOrientation(UINT);

protected:
    //{{AFX_MSG(CMySliderSK)
	afx_msg void OnLButtonDown( UINT nFlags, CPoint point );
	afx_msg void OnLButtonUp( UINT nFlags, CPoint point );
	afx_msg void OnMouseMove( UINT nFlags, CPoint point );
	afx_msg void OnPaint();
	afx_msg UINT OnNcHitTest( CPoint point );
	//}}AFX_MSG

	void FreeResources(BOOL bCheckForNULL = TRUE);
	HBITMAP CreateBitmapMask(HBITMAP hSourceBitmap, DWORD dwWidth, DWORD dwHeight, COLORREF crTransColor);
	void DrawTheBitmap(CPaintDC* pDC, CRect & rect, BYTE index);
	void PaintBk(CPaintDC* pDC);
private:
	UINT m_Orientation;
	int m_Min;
	int m_Max;
	int m_Span;
	int m_Pos;
	int m_RectSpan;
	CRect m_ThumbRect;
	CRect m_ClientRect;
	CRect m_ChannelRect;
	CRect m_UpperRect;
	CRect m_LowerRect;

	CDC			m_dcBkCh;
	CDC			m_dcBkU;
	CDC			m_dcBkL;
	CDC			m_dcThumbBk;
	CBitmap		m_bmpBkCh;
	CBitmap		m_bmpBkU;
	CBitmap		m_bmpBkL;
	CBitmap*	m_pbmpOldBkCh;
	CBitmap*	m_pbmpOldBkU;
	CBitmap*	m_pbmpOldBkL;
	CBitmap*	m_pbmpOldThumbBk;
	CWnd *		m_pParent;

	typedef struct _STRUCT_BITMAPS
	{
		HBITMAP		hBitmap;		// Handle to bitmap
		DWORD		dwWidth;		// Width of bitmap
		DWORD		dwHeight;		// Height of bitmap
		HBITMAP		hMask;			// Handle to mask bitmap
		COLORREF	crTransparent;	// Transparent color
	} STRUCT_BITMAPS;

	STRUCT_BITMAPS	m_csBitmaps[4];

	DECLARE_MESSAGE_MAP()
};

#endif