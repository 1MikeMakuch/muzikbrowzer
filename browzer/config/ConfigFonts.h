#if !defined(AFX_CONFIGFONTS_H__449E928B_3B06_11D6_8695_002078049F22__INCLUDED_)
#define AFX_CONFIGFONTS_H__449E928B_3B06_11D6_8695_002078049F22__INCLUDED_

#include "Resource.h"
#include "FontCombo.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigFonts.h : header file
//
//class CPlayerDlg;
/////////////////////////////////////////////////////////////////////////////
// CConfigFonts dialog

class CConfigFonts : public CPropertyPage
{
    DECLARE_DYNCREATE(CConfigFonts)
// Construction
public:
	CConfigFonts(CWnd *p=NULL);   // standard constructor
	~CConfigFonts();

// Dialog Data
	//{{AFX_DATA(CConfigFonts)
	enum { IDD = IDD_CONFIG_FONTS };
	CStatic		m_SamplePanel;
	CComboBox	m_SizePanel;
	CFontCombo	m_FontPanel;
	CButton		m_BoldPanel;

	CStatic		m_SampleTitles;
	CComboBox	m_SizeTitles;
	CFontCombo	m_FontTitles;
	CButton		m_BoldTitles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigFonts)
	public:
	virtual void OnOK();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigFonts)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CWnd * m_PlayerDlg;
  
	LPLOGFONT m_lplfTitles;
	LPLOGFONT m_lplfPanel;
    LOGFONT m_lfTitles;
	LOGFONT m_lfPanel;
    LOGFONT m_samplelfTitles;
	LOGFONT m_samplelfPanel;
    int m_SizeSelTitles;
	int m_SizeSelPanel;
    int m_FontSelTitles;
	int m_FontSelPanel;
	CFont m_FontSampleTitles;
	CFont m_FontSamplePanel;

    void init();
    void ReadReg();
    void StoreReg();
    void showSample();
    void copy2lf(LOGFONT &, LOGFONT &);
    void setDefault();
public:
    LPLOGFONT getTitlesFont() { return m_lplfTitles; }
	LPLOGFONT getPanelFont() { return m_lplfPanel; }
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGFONTS_H__449E928B_3B06_11D6_8695_002078049F22__INCLUDED_)
