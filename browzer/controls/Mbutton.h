#if !defined(AFX_MBUTTON_H__724CF573_5360_408D_838C_9100D9B9A4D4__INCLUDED_)
#define AFX_MBUTTON_H__724CF573_5360_408D_838C_9100D9B9A4D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Mbutton.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMbutton window

class CMbutton : public CButton
{
// Construction
public:
	CMbutton();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMbutton)
	public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMbutton();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMbutton)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MBUTTON_H__724CF573_5360_408D_838C_9100D9B9A4D4__INCLUDED_)
