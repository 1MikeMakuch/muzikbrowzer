// DlgProxy.h : header file
//

#if !defined(AFX_DLGPROXY_H__B5DFFA75_E8A3_4A84_971D_C72FB28FCB5C__INCLUDED_)
#define AFX_DLGPROXY_H__B5DFFA75_E8A3_4A84_971D_C72FB28FCB5C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CId3taggerDlg;

/////////////////////////////////////////////////////////////////////////////
// CId3taggerDlgAutoProxy command target

class CId3taggerDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CId3taggerDlgAutoProxy)

	CId3taggerDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CId3taggerDlg* m_pDialog;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CId3taggerDlgAutoProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CId3taggerDlgAutoProxy();

	// Generated message map functions
	//{{AFX_MSG(CId3taggerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CId3taggerDlgAutoProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CId3taggerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROXY_H__B5DFFA75_E8A3_4A84_971D_C72FB28FCB5C__INCLUDED_)
