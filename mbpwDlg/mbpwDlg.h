// mbpwDlg.h : header file
//

#if !defined(AFX_MBPWDLG_H__66DEB496_1E33_4675_B280_3F6215966CA6__INCLUDED_)
#define AFX_MBPWDLG_H__66DEB496_1E33_4675_B280_3F6215966CA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// CMbpwDlg dialog

class CMbpwDlg : public CDialog
{
// Construction
public:
	CMbpwDlg(CWnd* pParent = NULL);	// standard constructor
private:
	void parseInput(CString & host, CString & fname, CString & lname,
		CString & email);
public:
// Dialog Data
	//{{AFX_DATA(CMbpwDlg)
	enum { IDD = IDD_MBPW_DIALOG };
	CEdit	m_Output;
	CEdit	m_Input;
	CButton	m_Generate;
	CButton	m_Email;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMbpwDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMbpwDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnMbpwQuit();
	afx_msg void OnMbpwGenButton();
	afx_msg void OnMbpwEmail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MBPWDLG_H__66DEB496_1E33_4675_B280_3F6215966CA6__INCLUDED_)
