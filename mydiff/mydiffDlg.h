// mydiffDlg.h : header file
//

#if !defined(AFX_MYDIFFDLG_H__E2F5F3A2_D5D8_4168_A2E4_DCD886F228D3__INCLUDED_)
#define AFX_MYDIFFDLG_H__E2F5F3A2_D5D8_4168_A2E4_DCD886F228D3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "ExtendedListBox.h"

#include "SourceLine.h"
#include "StatusDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMydiffDlg dialog

class CMydiffDlg : public CDialog
{
// Construction
public:
	CMydiffDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CMydiffDlg)
	enum { IDD = IDD_MYDIFF_DIALOG };
	CEdit	m_EditDiffPos;
	CEdit	m_EditCWD;
	CButton	m_ButtonRediff;
	CButton	m_ButtonFilePrev;
	CButton	m_ButtonDiffPrev;
	CButton	m_ButtonFileNext;
	CButton	m_ButtonDiffNext;
	CExtendedListBox	m_Data2;
	CExtendedListBox	m_Data1;
	CExtendedListBox	m_Files;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMydiffDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL
// Implementation
protected:
	HICON m_hIcon;
	BOOL m_Running;
	int m_currentFile;


//	CStringList m_File1;
//	CStringList m_File2;
	SrcLineList m_File1;
	SrcLineList m_File2;
	CStringList m_CurrFile;
	CStringList m_Diff;
	CString m_currentfile;
	POSITION m_currentdiff;
	POSITION m_diffHead;
	POSITION m_diffTail;
	CObList m_DiffList;
	int m_totalNumDiffs;

	CStringList m_ComOutput;
	UINT m_msg2post;

	CString m_cwd;
	StatusDlg * m_StatusDlg;


	void mysystem(const CString & command, UINT msg);

	void dodiff();
	void incrementFile();
	void decrementFile();
	CString readDFile(const CString & file);
	void filebuf2csl(CStringList & list, const char * buf, 
		long length);
	void processDiff();
	void showDiff(SrcLineList & src, CExtendedListBox & box);
	void edGetCom(const CString & line, int & lineno1, int & lineno2, char & op);
	int pageHeight();
	CString verify();
	void showDiffPos();
	void resizeControls();
	void getcwd();
	void controlsEnable(BOOL);
	void showStatus(const CString & msg);
	void killStatus();

	afx_msg LRESULT OnFinished(WPARAM, LPARAM);
	afx_msg LRESULT OnLine(WPARAM, LPARAM);
	afx_msg LRESULT OnCstatDone(WPARAM, LPARAM);
	afx_msg LRESULT OnDiffDone(WPARAM, LPARAM);
	afx_msg LRESULT OnGetFileDone(WPARAM, LPARAM);
	afx_msg LRESULT OnVerifyDone(WPARAM, LPARAM);

	// Generated message map functions
	//{{AFX_MSG(CMydiffDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonNextDiff();
	afx_msg void OnButtonNextFile();
	afx_msg void OnButtonPrevDiff();
	afx_msg void OnButtonPrevFile();
	afx_msg void OnButtonRediff();
	afx_msg void OnDblclkListFiles();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIFFDLG_H__E2F5F3A2_D5D8_4168_A2E4_DCD886F228D3__INCLUDED_)

