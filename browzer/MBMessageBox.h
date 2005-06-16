#if !defined(AFX_MBMessageBoxImpl_H__3BB3C3F6_2B0A_11D6_8691_002078049F22__INCLUDED_)
#define AFX_MBMessageBoxImpl_H__3BB3C3F6_2B0A_11D6_8691_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MBMessageBoxImpl.h : header file
//
#include "Resource.h"
#include "MBGlobals.h"
#include "ColorStatic.h"

class VirtualControl;
/////////////////////////////////////////////////////////////////////////////
// MBMessageBoxImpl dialog

class MBMessageBoxImpl : public CDialog
{
// Construction
public:
	MBMessageBoxImpl(CString & title, CString & info, BOOL log = TRUE,
		BOOL enableCancel=FALSE, CWnd* pParent = NULL);   // standard constructor
	~MBMessageBoxImpl();
// Dialog Data
	//{{AFX_DATA(MBMessageBoxImpl)
	enum { IDD = IDD_MESSAGE_DIALOG };
	CEdit	m_MessageBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MBMessageBoxImpl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation

public:
	VirtualControl * m_Control;

	// Generated message map functions
	//{{AFX_MSG(MBMessageBoxImpl)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSetfocusMessageBox();
	afx_msg void OnSetfocusOk();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnExit();
private:
    CString m_info;
    CString m_title;
	BOOL m_log;
	CFont mfont;
	BOOL m_EnableCancel;

};

int MBMessageBox(CString title, CString msg, BOOL log = TRUE, 
				 BOOL enableCancel=FALSE);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.





#endif // !defined(AFX_MBMessageBoxImpl_H__3BB3C3F6_2B0A_11D6_8691_002078049F22__INCLUDED_)
