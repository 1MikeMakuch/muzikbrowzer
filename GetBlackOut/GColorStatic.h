#ifndef _GCOLORSTATIC_H
#define _GCOLORSTATIC_H




#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CGetBlackOutDlg ;

// ColorStaticST.h : header file
//

class GColorStatic : public CStatic
{
// Construction
public:
	GColorStatic();
	void SetDlg(CGetBlackOutDlg * d) {
		mdialog = d;
	}

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GColorStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~GColorStatic();

	void border(BOOL flag);

	void draw(CDC * cdc, int x, int y, int cx, int cy, COLORREF clr);
	void setrect(CRect rect) { mRect = rect;};
	void won(int x) { mWon=x;};

	// Generated message map functions
protected:
	//{{AFX_MSG(GColorStatic)
	
	//}}AFX_MSG
    afx_msg void OnPaint( );
	afx_msg void OnLButtonDblClk( UINT, CPoint );
	afx_msg void OnLButtonDown( UINT, CPoint );
	afx_msg void OnLButtonUp( UINT, CPoint );
	afx_msg void OnMouseMove( UINT, CPoint );
	DECLARE_MESSAGE_MAP()

private:

	CWnd* m_pParent;
	UINT m_nMsg;
	CRect mRect;
	BOOL mWon;

	BOOL m_border;
	int m_height;
	int m_width;
	CGetBlackOutDlg *mdialog;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif 
