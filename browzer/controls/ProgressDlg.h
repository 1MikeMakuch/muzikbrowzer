#ifndef __PROGRESS_DLG_H__
#define __PROGRESS_DLG_H__

// Current places where ProgressDlg is used;
// Scan from menu, from config, Add, scan for new
// garbage collect, modifyid3, verify

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressDlg.h : header file
//
#include "stdafx.h"
#include "Resource.h"
#include "ColorStatic.h"
//#include "PlayerApp.h"
/////////////////////////////////////////////////////////////////////////////
// ProgressDlg dialog
class MyThreadClass;

class ProgressDlg : public CDialog
{
// Construction
public:
	ProgressDlg(CDialog * parent, int cflag=0, int abortflag=0);   // standard constructor
	void SetTitle(const CString &);
	void SetLabel(const CString &);
	void UpdateStatus(const CString &);
	void UpdateStatus2(const CString &);

	void ProgressRange(int beg, int end);
	void ProgressPos(int pos);

	void SetThreadFunc(MyThreadClass * pt) { 
		m_pThread = pt;
	}
	void Abort();

// Dialog Data
	//{{AFX_DATA(ProgressDlg)
	enum { IDD = IDD_PROGRESS_DLG };
	CColorStatic	m_InitStatus2;
	CButton	m_AbortButton;
	CProgressCtrl	m_InitProgress;
	CColorStatic	m_InitStatus;
	CColorStatic	m_InitLabel;
	//}}AFX_DATA

    int m_Abort;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ProgressDlg)
	protected:
	virtual BOOL PreTranslateMessage(MSG*);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void SendUpdateStatus(int flag, const CString &msg="", int i1=0, int i2=0);

	// Generated message map functions
	//{{AFX_MSG(ProgressDlg)
	afx_msg void OnInitAbort();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG

    afx_msg LRESULT OnIDMessage(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()

	int m_abortflag;
	MyThreadClass * m_pThread;

	time_t m_starttime,m_elapsed,m_eta,m_now,m_lastupdate;
	float m_timeper;
	CString m_etaS;
	int m_lower,m_upper;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif 
