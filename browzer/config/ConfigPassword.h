#if !defined(AFX_CONFIGPASSWORD_H__449E928A_3B06_11D6_8695_002078049F22__INCLUDED_)
#define AFX_CONFIGPASSWORD_H__449E928A_3B06_11D6_8695_002078049F22__INCLUDED_

#include "stdafx.h"
#include "Resource.h"
#include "MyString.h"
#include "HyperLink.h"

#include <afxdlgs.h>
#include <afxcmn.h>

#define MB_TRIAL_DAYS 61

//#ifdef _DEBUG
//	#define MB_USING_TRIAL_MODE
//#else
	#undef MB_USING_TRIAL_MODE
//#endif

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigPassword.h : header file
//
//class CPlayerDlg;
class MBConfig;
/////////////////////////////////////////////////////////////////////////////
// CConfigPassword dialog

class CConfigPassword : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigPassword)

// Construction
public:
	CConfigPassword(CWnd*p=NULL,MBConfig *c=NULL);
	~CConfigPassword();

// Dialog Data
	//{{AFX_DATA(CConfigPassword)
	enum { IDD = IDD_CONFIG_PASSWORD };
	CButton	m_ValidatePw;
	CButton m_PurchaseButton;
//	CButton	m_RequestPw;
	CString	m_HostId;
	CString	m_Email;
	CString	m_Firstname;
	CString	m_Lastname;
	CString	m_Password;
	CString	m_Notice;
	CString	m_TrialExpiration;
	CString	m_TrialLabel;
	//}}AFX_DATA

// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigPassword)
	public:
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	static CString s2h(const CString & s);
	static CString h2s(const CString & h);
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigPassword)
	virtual BOOL OnInitDialog();
	afx_msg void OnSendinfo();
	afx_msg void OnValidatePw();
	afx_msg void OnValidate();
	afx_msg void OnPurchaseButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CWnd * m_PlayerDlg;
	int m_TrialMode;
	CTime m_ExpDate;
	MBConfig * m_config;

    void ReadReg();
    void StoreReg();
	void validate();
	CString createPassword();
	CString munge(const CString & msg);
	void EnableDisable();

public:
	int trialLeft();
	CString genHost();
	CString license() { return m_Password; }
	void resetTrial();
	void getSettings(MyHash & settings);
	static CString uuidEncode(const CString & u);
	static CString uuidDecode(const CString & u);
#ifdef MB_USING_TRIAL_MODE
	int trialMode() { return m_TrialMode; }
#else
	int trialMode() { return FALSE; }
#endif

public:
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGPASSWORD_H__449E928A_3B06_11D6_8695_002078049F22__INCLUDED_)
