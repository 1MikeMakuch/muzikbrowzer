
#ifndef _EXTENDEDLISTBOX_H_
#define _EXTENDEDLISTBOX_H_
// custlist.cpp : custom listbox
//
// This is a part of the Microsoft Foundation Classes C++ library.
// Copyright (C) 1992-1997 Microsoft Corporation
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Microsoft Foundation Classes Reference and related
// electronic documentation provided with the library.
// See these sources for detailed information regarding the
// Microsoft Foundation Classes product.

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// Custom Listbox - containing colors

class CExtendedListBox : public CListBox
{
public:
    CExtendedListBox(BOOL usecolors=TRUE);
	~CExtendedListBox();
// Operations
//	void AddColorItem(COLORREF color);

// Implementation
//	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);

//	virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCIS);
    int GetSelectedItemFromPoint(CPoint &);
    virtual int CompareItem( LPCOMPAREITEMSTRUCT lpCompareItemStruct );
    void changeFont(LPLOGFONT);
    void initFont();

    afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
    afx_msg void OnNcCalcSize(BOOL bCalcValidRects,
        NCCALCSIZE_PARAMS FAR* lpncsp) ;
    afx_msg void OnNcPaint();
    afx_msg UINT OnNcHitTest(CPoint);
    afx_msg void OnNcLButtonDown(UINT, CPoint);
    afx_msg void OnNcLButtonUp(UINT, CPoint);
    afx_msg void OnNcLButtonDblClk( UINT nHitTest, CPoint point );
    afx_msg void OnTimer(UINT);
    afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
    afx_msg void OnMouseMove( UINT nFlags, CPoint point );

    BOOL m_reorder;
    void invalidate();
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CExtendedListBox)
	//}}AFX_VIRTUAL
	CString m_id;

protected:
    void move(UINT);
    void DrawBorders();
    void DrawScroll();
    void DrawScrollBg();
    void DrawScrollArrows();
    void DrawScrollButton();

	//{{AFX_MSG(CExtendedListBox)
//	afx_msg void OnPaint();
	//}}AFX_MSG
    DECLARE_MESSAGE_MAP();
private:
    COLORREF m_BkNormal;
    COLORREF m_BkHigh;
    COLORREF m_BkSel;
    COLORREF m_TxNormal;
    COLORREF m_TxHigh;
    COLORREF m_TxSel;
    UINT m_ScrollHitPos;
    BOOL m_Capture;
    BOOL m_HaveScroll;
    CBitmap m_ScrollUpArrowBM;
    CBitmap m_ScrollDownArrowBM;
    CBitmap m_ScrollButtonBM;

    CBitmap * m_ScrollUpArrowBMsave;
    CBitmap * m_ScrollDownArrowBMsave;
    CBitmap * m_ScrollButtonBMsave;

    CDC m_ScrollUpArrowCDC;
    CDC m_ScrollDownArrowCDC;
    CDC m_ScrollButtonCDC;

    CRect m_ScrollButtonRect;
    CRect m_ScrollBarBorderRect;
    CRect m_ScrollUpArrowRect;
    CRect m_ScrollDownArrowRect;
    CRect m_ScrollBgRect;
    CRect m_ScrollPageUpRect;
    CRect m_ScrollPageDownRect;

    UINT m_ScrollRange;
    BOOL m_UseColors;
	CFont m_font;


    void TransparentBlt( HDC hdcDest, int nXDest, int nYDest, int nWidth, 
			int nHeight, HBITMAP hBitmap, int nXSrc, int nYSrc,
			COLORREF colorTransparent, HPALETTE hPal );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif
