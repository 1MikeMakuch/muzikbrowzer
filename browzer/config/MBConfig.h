#if !defined(AFX_MBCONFIG_H__449E928E_3B06_11D6_8695_002078049F22__INCLUDED_)
#define AFX_MBCONFIG_H__449E928E_3B06_11D6_8695_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "StdAfx.h"
#include "PlayerCallbacks.h"
#include "Registry.h"
#include "MyString.h"

// MBConfig.h : header file
//

//class CPlayerDlg;
class CConfigFiles;
class CConfigIrman;
//class CConfigFonts;
//class CConfigColors;
class CConfigDisplay;
class CConfigPassword;

/////////////////////////////////////////////////////////////////////////////
// MBConfig



class MBConfig : public CPropertySheet
{
	DECLARE_DYNAMIC(MBConfig)

// Construction
public:
	MBConfig(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	MBConfig(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
    
	void createit(CWnd *p, PlayerCallbacks * pcb) {
		m_CWnd = (CWnd*)p;
        m_playercallbacks = pcb;
        init();
    }

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MBConfig)
public:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~MBConfig();

	// Generated message map functions
protected:
	//{{AFX_MSG(MBConfig)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	void SaveWindowPos();
	void ReadWindowPos();
private:
	PlayerCallbacks * m_playercallbacks;
//    CPlayerDlg * m_PlayerDlg;
	CWnd * m_CWnd;
    CConfigFiles * m_Files;
    CConfigIrman * m_Irman;
//    CConfigFonts * m_Fonts;
//    CConfigColors * m_Colors;
	CConfigDisplay * m_Display;
	CConfigPassword * m_Password;
    COLORREF m_BKColor;
    COLORREF m_TColor;

    void init();
	BOOL HelpInfo(const int i);

public:
    CConfigFiles * files() { return m_Files; }
    CConfigIrman * irman() { return m_Irman; }
//    CConfigFonts * fonts() { return m_Fonts; }
//    CConfigColors * colors() { return m_Colors;}
	CConfigDisplay * display() { return m_Display; };
	CConfigPassword * password() { return m_Password; }

    LPLOGFONT getTitlesFont();
    LPLOGFONT getStatusFont();
	LPLOGFONT getColHdrFont();
//	LPLOGFONT getCurPlayFont();

    COLORREF getColorBkPanel();
    COLORREF getColorBkNormal();
    COLORREF getColorBkHigh();
    COLORREF getColorBkSel();
    COLORREF getColorTxPanel();
    COLORREF getColorTxNormal();
    COLORREF getColorTxHigh();
    COLORREF getColorTxSel();
	COLORREF getColorTxColHdr();
	COLORREF getColorBkColHdr();
	COLORREF getColorBkPanel2();
//	COLORREF getColorBkCurPlay();
//	COLORREF getColorTxCurPlay();
	int getDlgBorderWidth();
//	double getPlaylistHeightPct();
//	double getGenreWidthPct();
	int getDlgBorderHorz() ;
	int getDlgBorderVert() ;
	BOOL hideGenre();
		
	BOOL AlbumSortAlpha();
	void getRegistry(const CString & key, int & value, const int dflt);
	void setRegistry(const CString & key, const int value);
	void getRegistry(const CString & key, CString & value, const CString &dflt="");
	void setRegistry(const CString & key, const CString & value);
	int trialMode();
	int trialLeft();
	CString mbdir();
	const CString getSkin(const CString key);
	const CString getCurrentSkin();
	void getSkins(CStringList &);
	BOOL ChooseSkin(CString);
	int getPanelWidth() ;
	void ReadReg(RegistryKey &);
	BOOL verifySkin();
	BOOL verifySkin(const CString skin, const BOOL stopOnError=TRUE);
	BOOL findGoodSkin();
	void GetDirs(CStringArray & dirs,CStringArray & excludes,const CString & dir="");
	void AddFolders(const CStringArray & dirs);
	void AddDeletedFiles(const CStringArray & files);
	void DelFolders(const CStringArray & dirs);
	CString license();
	void resetTrial();

	BOOL resetNeeded();
	void initSkins();
	void logSettings();
	void getSettings(MyHash & settings);
	int logging();
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MBCONFIG_H__449E928E_3B06_11D6_8695_002078049F22__INCLUDED_)
