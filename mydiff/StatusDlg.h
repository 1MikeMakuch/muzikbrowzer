#if !defined(AFX_STATUSDLG_H__F3002B22_D12E_4B18_9BA4_77F2AA2161E4__INCLUDED_)
#define AFX_STATUSDLG_H__F3002B22_D12E_4B18_9BA4_77F2AA2161E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StatusDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// StatusDlg dialog

class StatusDlg : public CDialog
{
// Construction
public:
	StatusDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(StatusDlg)
	enum { IDD = IDD_DIALOG1 };
	CEdit	m_Status;
	//}}AFX_DATA
	void showStatus(const CString & msg);
	void kill();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(StatusDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	CString m_msg;

	// Generated message map functions
	//{{AFX_MSG(StatusDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATUSDLG_H__F3002B22_D12E_4B18_9BA4_77F2AA2161E4__INCLUDED_)
