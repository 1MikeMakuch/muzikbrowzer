#if !defined(AFX_TIPWND_H__D9FED000_F603_11D1_A8D9_0060970C2D45__INCLUDED_)
#define AFX_TIPWND_H__D9FED000_F603_11D1_A8D9_0060970C2D45__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// TipWnd.h : header file
//

//////////////////////////////////////////////////////////////////////////
// © Paramax Technology Limited                                         // 
// ----------------------------                                         //
//                                                                      //
// The author accepts no liablility for injury or loss of profits       // 
// if this software is used. You willingness to use this software       //
// indicates you accept total liability                                 //
//                                                                      // 
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CTipWnd window

class CTipWnd : public CWnd
{
// Construction
public:
	CTipWnd();

// Attributes
public:
protected:
	CFont		m_font;
	CString		m_strFont;
	
// Operations
public:
	BOOL Create(CWnd* pParent);
	void ShowTips(CPoint pt, const CString& str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTipWnd)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTipWnd();

	// Generated message map functions
protected:
	//{{AFX_MSG(CTipWnd)
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TIPWND_H__D9FED000_F603_11D1_A8D9_0060970C2D45__INCLUDED_)
