#if !defined(AFX_CHECKRESULT_H__A921B19C_0488_4435_98FA_FAACD659D80D__INCLUDED_)
#define AFX_CHECKRESULT_H__A921B19C_0488_4435_98FA_FAACD659D80D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CheckResult.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCheckResult dialog

class CCheckResult : public CDialog
{
// Construction
public:
	CCheckResult(CString buttonLabel, CString & data, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCheckResult)
	enum { IDD = IDD_CHECKDLG };
	CButton	m_Cancel;
	CButton	m_OkButton;
	CEdit	m_Check;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCheckResult)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CString m_buttonlabel;
	CString m_data;

	// Generated message map functions
	//{{AFX_MSG(CCheckResult)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHECKRESULT_H__A921B19C_0488_4435_98FA_FAACD659D80D__INCLUDED_)
