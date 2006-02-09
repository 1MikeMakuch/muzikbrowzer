#if !defined(AFX_GetTextFieldDlg_H__7340F5B8_C8A5_11D5_8680_002078049F22__INCLUDED_)
#define AFX_GetTextFieldDlg_H__7340F5B8_C8A5_11D5_8680_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetTextFieldDlg.h : header file
//
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// GetTextFieldDlg dialog

class GetTextFieldDlg : public CDialog
{
// Construction
public:
	GetTextFieldDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(GetTextFieldDlg)
	enum { IDD = IDD_GETTEXTFIELD };
	CStatic m_cCaption;
	CString m_sTextField;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GetTextFieldDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	int Get(const CString &title, CString & text);
	int Confirm(const CString &title, CString text="");
protected:

	// Generated message map functions
	//{{AFX_MSG(GetTextFieldDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_Title,m_sCaption;
	BOOL m_GettingText,m_Confirming;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GetTextFieldDlg_H__7340F5B8_C8A5_11D5_8680_002078049F22__INCLUDED_)
