#if !defined(AFX_CONFIGIRMAN_H__449E928D_3B06_11D6_8695_002078049F22__INCLUDED_)
#define AFX_CONFIGIRMAN_H__449E928D_3B06_11D6_8695_002078049F22__INCLUDED_
#include "Resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigIrman.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigIrman dialog

class CConfigIrman : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigIrman)

// Construction
public:
	CConfigIrman(CWnd *p=NULL);
	~CConfigIrman();
//    CPlayerDlg * playerdlg() { return m_PlayerDlg; }
    BOOL EnableDisableDialog();

// Dialog Data
	//{{AFX_DATA(CConfigIrman)
	enum { IDD = IDD_CONFIG_IRMAN };
	CString	m_IRComPortStatus;
	CString	m_IRDeleteDesc;
	CString	m_IRDeleteStatus;
	CString	m_IRDownDesc;
	CString	m_IRDownStatus;
	CString	m_IRExitDesc;
	CString	m_IRExitStatus;
	CString	m_IRLeftDesc;
	CString	m_IRLeftStatus;
	CString	m_IRMenuDesc;
	CString	m_IRMenuStatus;
	CString	m_IRNextDesc;
	CString	m_IRNextStatus;
	CString	m_IRPageDownDesc;
	CString	m_IRPageDownStatus;
	CString	m_IRPageUpDesc;
	CString	m_IRPageUpStatus;
	CString	m_IRPauseDesc;
	CString	m_IRPauseStatus;
	CString	m_IRPlayDesc;
	CString	m_IRPlayStatus;
	CString	m_IRPreviousDesc;
	CString	m_IRPreviousStatus;
	CString	m_IRRightDesc;
	CString	m_IRRightStatus;
	CString	m_IRSelectDesc;
	CString	m_IRSelectStatus;
	CString	m_IRStopDesc;
	CString	m_IRStopStatus;
	CString	m_IRUpDesc;
	CString	m_IRUpStatus;
	CString	m_IRVolDownDesc;
	CString	m_IRVolUpDesc;
	CString	m_IRVolUpStatus;
	CString	m_IRVolDownStatus;
	int		m_delay;
	CString	m_IRReverseDesc;
	CString	m_IRReverseStatus;
	CString	m_IRFastForwardDesc;
	CString	m_IRFastForwardStatus;
	CString	m_IRAlphaDownDesc;
	CString	m_IRAlphaDownStatus;
	CString	m_IRAlphaUpDesc;
	CString	m_IRAlphaUpStatus;
	CString	m_IRRandomDesc;
	CString	m_IRRandomStatus;
	CString	m_IRSaveDesc;
	CString	m_IRSaveStatus;
	CString	m_IRShuffleDesc;
	CString	m_IRShuffleStatus;
	CString	m_IRLoadDesc;
	CString	m_IRLoadStatus;
	CString	m_IRClearDesc;
	CString	m_IRClearStatus;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigIrman)
	public:
	virtual BOOL OnApply();
	virtual void OnCancel();
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigIrman)
	afx_msg void OnIrrecord();
	afx_msg void OnIrtest();
	afx_msg void OnIrcom1();
	afx_msg void OnIrinitialize();
	virtual BOOL OnInitDialog();
	afx_msg void OnIrcom2();
	afx_msg void OnIrcom3();
	afx_msg void OnIrcom4();
	afx_msg void OnIrcom5();
	afx_msg void OnIrcom6();
	afx_msg void OnIrcom7();
	afx_msg void OnIrcom8();
	afx_msg void OnKillfocusIrdelay();
	afx_msg void OnIrnone();
	afx_msg void OnIrhelpbutton();
	afx_msg void OnSetfocusIrdelay();
	afx_msg void OnSelchangeIrtype();
	afx_msg void OnChangeIrdelay();
	//}}AFX_MSG
	afx_msg void OnRecordButton(UINT nID );
	afx_msg LRESULT OnSerialMsg (WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

	int HandleIRMessage(int key);

private:
	BOOL m_irrecording;
	BOOL m_irtesting;
	int m_lastkey;
	int m_keycount;
	BOOL m_NeedInit;
	CString m_ComPort;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGIRMAN_H__449E928D_3B06_11D6_8695_002078049F22__INCLUDED_)
