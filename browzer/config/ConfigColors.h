#if !defined(AFX_CONFIGCOLORS_H__449E928A_3B06_11D6_8695_002078049F22__INCLUDED_)
#define AFX_CONFIGCOLORS_H__449E928A_3B06_11D6_8695_002078049F22__INCLUDED_

#include "Resource.h"
#include "controls/colorbtn.h"
#include <afxdlgs.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigColors.h : header file
//
//class CPlayerDlg;
/////////////////////////////////////////////////////////////////////////////
// CConfigColors dialog

class CConfigColors : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigColors)

// Construction
public:
	CConfigColors(CWnd*p=NULL);
	~CConfigColors();

// Dialog Data
	//{{AFX_DATA(CConfigColors)
	enum { IDD = IDD_CONFIG_COLORS };
	CColorBtn	m_TxPanel;
	CColorBtn	m_BkPanel;
	CColorBtn	m_TxSel;
	CColorBtn	m_TxNormal;
	CColorBtn	m_TxHigh;
	CColorBtn	m_BkSel;
	CColorBtn	m_BkNormal;
	CColorBtn	m_BkHigh;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigColors)
	public:
	virtual void OnOK();
	virtual void OnCancel();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigColors)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetDefault();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CWnd * m_PlayerDlg;
    COLORREF m_vBkPanel;
    COLORREF m_vBkNormal;
    COLORREF m_vBkHigh;
    COLORREF m_vBkSel;
    COLORREF m_vTxPanel;
    COLORREF m_vTxNormal;
    COLORREF m_vTxHigh;
    COLORREF m_vTxSel;

    void ReadReg();
    void StoreReg();
    void setDefaults();
    void v2m();
    void m2v();
public:
    COLORREF getBkPanel() { return m_vBkPanel; }
    COLORREF getBkNormal() { return m_vBkNormal; }
    COLORREF getBkHigh() { return m_vBkHigh; }
    COLORREF getBkSel() { return m_vBkSel; }
    COLORREF getTxPanel() { return m_vTxPanel; }
    COLORREF getTxNormal() { return m_vTxNormal; }
    COLORREF getTxHigh() { return m_vTxHigh; }
    COLORREF getTxSel() { return m_vTxSel; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGCOLORS_H__449E928A_3B06_11D6_8695_002078049F22__INCLUDED_)
