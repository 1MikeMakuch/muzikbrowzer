#ifndef _COLORSTATIC_H
#define _COLORSTATIC_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

// ColorStaticST.h : header file
//
#include "GetSysColors.h"

class CColorStatic : public CStatic //, public SkinBmp
{
// Construction
public:
	CColorStatic();
    enum {	ST_FLS_SLOW,
			ST_FLS_NORMAL,
			ST_FLS_FAST};

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CColorStatic();
	void SizeToContent();
	void setDesc(CString desc) { m_desc = desc; }
//	virtual DWORD SetBitmaps(CDC * cdc, LPCTSTR nBitmap, COLORREF crTransColor);

	void SetColors(
		COLORREF fg, 
		COLORREF bg,
		COLORREF inUL=0,
		COLORREF inLR=0,
		COLORREF outUL=0,
		COLORREF outLR=0,
		BOOL threeD=FALSE);

	void SetWordWrap(BOOL b = TRUE) {
		m_WordWrap = TRUE;
	}
	void SetTextColor(COLORREF crTextColor = 0xffffffff);
	COLORREF GetTextColor();

	void SetBkColor(COLORREF crBkColor = 0xffffffff);
	COLORREF GetBkColor();
	
	void SetTransparent(CBitmap * bmp, const CRect & srcrect,
		const COLORREF trans);

	void SetShadowDark(COLORREF clr = 0) {
		m_ClrShadowDark = clr;
	}

	void SetShadowLight(COLORREF clr = 0xffffffff) {
		m_ClrShadowLight = clr;
	}

	void SetBlinkTextColors(COLORREF crBlinkTextColor1, COLORREF crBlinkTextColor2);
	void StartTextBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);

	void SetBlinkBkColors(COLORREF crBlinkBkColor1, COLORREF crBlinkBkColor2);
	void StartBkBlink(BOOL bStart = TRUE, UINT nElapse = ST_FLS_NORMAL);

	void EnableNotify(CWnd* pParent = NULL, UINT nMsg = WM_USER);
	static const short GetVersionI();
	static const char* GetVersionC();

    void setText(CString text, const int justify=DT_LEFT);
    void getText(CString &);
	void changeFont(LPLOGFONT lplf);
	void changeFont(CFont * f);
	void initFont();
	void border(BOOL flag);
	int GetItemHeight() { return m_height; }
	int GetItemWidth() { return m_width; }
	CSize GetSize(CString);
	void SetTicking(BOOL ticking) { m_TickerIt = ticking; }
	void StartTicker();
	// Generated message map functions
protected:
	//{{AFX_MSG(CColorStatic)
	afx_msg void OnPaint();
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	//}}AFX_MSG
//    afx_msg void OnPaint( );
	DECLARE_MESSAGE_MAP()
	void SetItemHeight(int height) { m_height=height; }
	void SetItemWidth();
	
private:
	int m_Justify;
	UINT m_nTimerId;

	COLORREF m_crTextColor;
	COLORREF m_crBlinkTextColors[2];
	BOOL m_bTextBlink;
	int m_nTextBlinkStep;

	COLORREF m_crBkColor;
	COLORREF m_crBlinkBkColors[2];
	BOOL m_bBkBlink;
	int m_nBkBlinkStep;

	COLORREF m_crCornerColor;

	COLORREF m_ClrShadowDark;
	COLORREF m_ClrShadowLight;

	COLORREF m_crInUL;
	COLORREF m_crInLR;
	COLORREF m_crOutUL;
	COLORREF m_crOutLR;
	BOOL m_3d;

	COLORREF m_crTrans;
	BOOL m_DoTrans;

	CBrush m_brBkgnd;
	CBrush m_brBlinkBkgnd[2];
	CBrush m_brFg;
	CBrush m_brush;

	CWnd* m_pParent;
	UINT m_nMsg;

    CString m_text,m_text2;
	BOOL m_border;
	int m_height;
	int m_width;
	CFont m_font;
	CRect m_Rect;

	int first;

	CString tmp;
	CString m_desc ;
	int m_TickerX, m_TickerY, m_TickerXStep, m_TickerTime;
	time_t m_TickerDelay;
	BOOL m_TickerIt,m_NeedTicker, m_Ticking;
	CTime m_TickerStart, m_TimeTmp;
	BOOL m_WordWrap;

	CBitmap m_bmpBg;
	CDC m_dcBg;
	BOOL m_BgSet;
	CBitmap * m_bmpOldBg;

public:
	int m_HCenter;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.



#endif 
