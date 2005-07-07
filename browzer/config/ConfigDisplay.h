#if !defined(AFX_CONFIGDISPLAY_H__449E928A_3B06_11D6_8695_002078049F22__INCLUDED_)
#define AFX_CONFIGDISPLAY_H__449E928A_3B06_11D6_8695_002078049F22__INCLUDED_

#include "Resource.h"
#include "FontCombo.h"
#include "PlayerCallbacks.h"
#include "controls/colorbtn.h"
#include "controls/ColorStatic.h"
#include <afxdlgs.h>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigDisplay.h : header file
//
//class CPlayerDlg;
/////////////////////////////////////////////////////////////////////////////
// CConfigDisplay dialog
class RegistryKey;

class CConfigDisplay : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigDisplay)

// Construction
public:
	CConfigDisplay(CWnd*p=NULL, PlayerCallbacks * m_playercallbacks=NULL);
	~CConfigDisplay();

// Dialog Data
	//{{AFX_DATA(CConfigDisplay)
	enum { IDD = IDD_CONFIG_DISPLAY };
	CComboBox	m_BorderVert;
	CComboBox	m_BorderHorz;
	CSpinButtonCtrl	m_GenreSpin;
//	CSpinButtonCtrl		m_PlaylistSpin;
//	CEdit				m_PlaylistHeight;
	CComboBox		m_PanelWidth;
	CComboBox		m_SkinList;
	CComboBox		m_BorderWidth;
	CButton         m_BoldColHdr;
	CButton         m_BoldPanel;
	CButton         m_BoldTitles;
//	CButton         m_ThemeChoose;
//	CButton         m_ThemeCreate;
//	CButton         m_ThemeDelete;
	CColorBtn       m_BkColHdr;
	CColorBtn       m_BkCtrls;
	CColorBtn       m_BkHigh;
	CColorBtn       m_BkNormal;
	CColorBtn       m_BkPanel;
	CColorBtn       m_BkSel;
	CColorBtn       m_Border;
	CColorBtn       m_TxColHdr;
	CColorBtn       m_TxCtrls;
	CColorBtn       m_TxHigh;
	CColorBtn       m_TxNormal;
	CColorBtn       m_TxPanel;
	CColorBtn       m_TxSel;
	CColorStatic    m_SampleColHdr;
	CColorStatic    m_SampleHigh;
	CColorStatic    m_SamplePanel;
	CColorStatic    m_SampleSel;
	CColorStatic    m_SampleTitles;
	CComboBox       m_SizeColHdr;
	CComboBox       m_SizePanel;
	CComboBox       m_SizeTitles;
//	CComboBox       m_ThemeList;
	CFontCombo      m_FontColHdr;
	CFontCombo      m_FontPanel;
	CFontCombo      m_FontTitles;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigDisplay)
	public:
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnApply();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CConfigDisplay)
	virtual BOOL OnInitDialog();
	afx_msg void OnSetDefault();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeFont();
//	afx_msg void OnSaveTheme();
//	afx_msg void OnUseTheme();
//	afx_msg void OnThemeChoose();
	afx_msg void OnSkinChoose();
//	afx_msg void OnThemeDelete();
//	afx_msg void OnUpdateThemeName();
//	afx_msg void OnSelchangeThemeList();
//	afx_msg void OnDblclkThemeList();
//	afx_msg void OnEditchangeThemeList();
//	afx_msg void OnEditupdateThemeList();
//	afx_msg void OnThemeCreate();
	afx_msg void OnSkinDelete();
	afx_msg void OnEditchangeSkinList();
	afx_msg void OnSelendokSkinList();
	afx_msg void OnColorButton();
	//}}AFX_MSG
	afx_msg void OnCButtonMessage(WPARAM w, LPARAM l)  ;
	afx_msg void onbold();
	afx_msg void OnUpdateWidth();
	DECLARE_MESSAGE_MAP()
private:
    PlayerCallbacks * m_playercallbacks;
    COLORREF m_vBkPanel;
    COLORREF m_vBkNormal;
    COLORREF m_vBkHigh;
    COLORREF m_vBkSel;
    COLORREF m_vTxPanel;
    COLORREF m_vTxNormal;
    COLORREF m_vTxHigh;
    COLORREF m_vTxSel;
	COLORREF m_vTxColHdr;
	COLORREF m_vBkColHdr;
	COLORREF m_vBorder;
	COLORREF m_vBkCtrls;
	COLORREF m_vTxCtrls;

    void ReadReg(RegistryKey & key);
    void StoreReg(RegistryKey & key);
    void setDefaults();
    void v2m();
    void m2v();
	void modified(BOOL b);
  
	LPLOGFONT m_lplfTitles;
	LPLOGFONT m_lplfPanel;
	LPLOGFONT m_lplfColHdr;
    LOGFONT m_lfTitles;
	LOGFONT m_lfPanel;
	LOGFONT m_lfColHdr;
    LOGFONT m_samplelfTitles;
	LOGFONT m_samplelfPanel;
	LOGFONT m_samplelfColHdr;
//	LOGFONT m_samplelfSel;
//	LOGFONT m_samplelfHigh;
    int m_SizeSelTitles;
	int m_SizeSelPanel;
	int m_SizeSelColHdr;
    int m_FontSelTitles;
	int m_FontSelPanel;
	int m_FontSelColHdr;
	CFont m_FontSampleTitles;
	CFont m_FontSamplePanel;
	CFont m_FontSampleColHdr;
	CFont m_FontSampleSel;
	CFont m_FontSampleHigh;

//	CString m_sThemeName;
	CString m_sSkinName;
	int m_vBorderWidth;
	int m_vPanelWidth;
//	int m_vPlaylistHeightPct;
	int m_vGenreWidthPct;
	int m_vBorderHorz;
	int m_vBorderVert;

    void init();
	void initFontSels();
    void showSample();
    void copy2lf(LOGFONT &, LOGFONT &, LOGFONT &);
//	CString m_ThemeDir;
	CString m_SkinDir;
//	void readThemes();
	void readSkins();
	
public:
    LPLOGFONT getTitlesFont() { return m_lplfTitles; }
	LPLOGFONT getPanelFont() { return m_lplfPanel; }
	LPLOGFONT getColHdrFont() { return m_lplfColHdr; }

public:
    COLORREF getBkPanel() { return m_vBkPanel; }
    COLORREF getBkNormal() { return m_vBkNormal; }
    COLORREF getBkHigh() { return m_vBkHigh; }
    COLORREF getBkSel() { return m_vBkSel; }
    COLORREF getTxPanel() { return m_vTxPanel; }
    COLORREF getTxNormal() { return m_vTxNormal; }
    COLORREF getTxHigh() { return m_vTxHigh; }
    COLORREF getTxSel() { return m_vTxSel; }
	COLORREF getBkColHdr() { return m_vBkColHdr; }
	COLORREF getTxColHdr() { return m_vTxColHdr; }
	COLORREF getBorder() { return m_vBorder; }
	COLORREF getTxCtrls() { return m_vTxCtrls; }
	COLORREF getBkCtrls() { return m_vBkCtrls; }

	int getBorderWidth() { return m_vBorderWidth; }
	int getPanelWidth() { return m_vPanelWidth; }
	int getBorderHorz() { return m_vBorderHorz; }
	int getBorderVert() { return m_vBorderVert; }
//	double getPlaylistHeightPct();
	double getGenreWidthPct();
	const CString getSkin(const CString key);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGDISPLAYS_H__449E928A_3B06_11D6_8695_002078049F22__INCLUDED_)

