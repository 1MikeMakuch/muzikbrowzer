#if !defined(AFX_ABOUT_H__26779112_4DD5_11D6_869A_002078049F22__INCLUDED_)
#define AFX_ABOUT_H__26779112_4DD5_11D6_869A_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// About.h : header file
//
#include "Resource.h"
#include "HyperLink.h"
/////////////////////////////////////////////////////////////////////////////
// About dialog

class About : public CDialog
{
// Construction
public:
	About(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(About)
	enum { IDD = IDD_ABOUTBOX };
	CEdit	m_ProdVersion;
//	CStatic	m_ProdVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(About)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:

	// Generated message map functions
	//{{AFX_MSG(About)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	CHyperLink	m_hyperlink;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABOUT_H__26779112_4DD5_11D6_869A_002078049F22__INCLUDED_)
