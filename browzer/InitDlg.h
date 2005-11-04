#if !defined(AFX_INITDLG_H__2B2759A0_A315_11D5_8676_002078049F22__INCLUDED_)
#define AFX_INITDLG_H__2B2759A0_A315_11D5_8676_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InitDlg.h : header file
//
#include "stdafx.h"
#include "Resource.h"
//#include "PlayerApp.h"
/////////////////////////////////////////////////////////////////////////////
// InitDlg dialog

class InitDlg : public CDialog
{
// Construction
public:
	InitDlg(int cflag, int abortflag, CWnd* pParent = NULL);   // standard constructor
	void SetLabel(CString &);
	void UpdateStatus(CString &);
    void SendUpdateStatus(int flag, CString, int i1, int i2);
    // flag 0 == label, 1 == status, 2 == range, 3 == pos
	void ProgressRange(int beg, int end);
	void ProgressPos(int pos);

// Dialog Data
	//{{AFX_DATA(InitDlg)
	enum { IDD = IDD_INIT_DIALOG };
	CButton	m_AbortButton;
	CProgressCtrl	m_InitProgress;
	CString	m_InitStatus;
	CString	m_InitLabel;
	//}}AFX_DATA

    int m_Abort;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(InitDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(InitDlg)
	afx_msg void OnInitAbort();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

    afx_msg LRESULT OnIDMessage(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()

	int m_abortflag;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INITDLG_H__2B2759A0_A315_11D5_8676_002078049F22__INCLUDED_)
