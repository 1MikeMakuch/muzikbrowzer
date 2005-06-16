#if !defined(AFX_FractalDlg_H__2B2759A0_A315_11D5_8676_002078049F22__INCLUDED_)
#define AFX_FractalDlg_H__2B2759A0_A315_11D5_8676_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FractalDlg.h : header file
//
#include "stdafx.h"
#include "PlayerApp.h"
/////////////////////////////////////////////////////////////////////////////
// FractalDlg dialog

class FractalDlg : public CDialog
{
// Construction
public:
	FractalDlg(CRect rect, CWnd* pParent = NULL);   // standard constructor
	~FractalDlg();

// Dialog Data
	//{{AFX_DATA(FractalDlg)
	enum { IDD = IDD_FRACTAL_DLG };
	CButton		m_Go;
	CButton		m_Cancel;
	//}}AFX_DATA

    int m_Abort;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(FractalDlg)
	protected:
	afx_msg void OnCancel();
	afx_msg void OnGo();
	virtual void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(FractalDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	CBrush m_BrushWhite;
	COLORREF white;
	COLORREF black;
	COLORREF green;
	char screen[1000][1000];
	CRect crect;
	int width;
	int height;
	int timerid;
	int frac(const int,const int);
	void init();
	void setOrigin(CDC *, int,int,COLORREF);
	void move(int & x, int & y);
	int abort;
	int usestarts;
	int startx;
	int starty;
	int done;
	CRect m_rect;
public:
	void doit();

	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FractalDlg_H__2B2759A0_A315_11D5_8676_002078049F22__INCLUDED_)
