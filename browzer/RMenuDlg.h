#if !defined(AFX_RMenuDlg_H__7340F5BB_C8A5_11D5_8680_002078049F22__INCLUDED_)
#define AFX_RMenuDlg_H__7340F5BB_C8A5_11D5_8680_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RMenuDlg.h : header file
//
#include "Resource.h"
#include "MusicDb.h"
#include "ResizableGrip.h"
#include "CDialogSK.h"
#include "Registry.h"
#include "MBConfig.h"
#include "PlayerCallbacks.h"
#include "ColorStatic.h"
#include "ButtonST.h"

class VirtualControl;

/////////////////////////////////////////////////////////////////////////////
// RMenuDlg dialog
class VirtualControl;

class RMenuDlg : public CDialogSK //, public CResizableGrip
{
// Construction
public:
	RMenuDlg(CWnd* pParent, PlayerCallbacks * pcb);
	~RMenuDlg();

// Dialog Data
	//{{AFX_DATA(RMenuDlg)
	enum { IDD = IDD_MENU_DIALOG };
	CButtonST	m_Exit;
	CButtonST	m_Shuffle;
	CButtonST	m_Random;
	CButtonST	m_Clear;
	CButtonST	m_Save;
	CButtonST	m_Load;
	CButtonST	m_Pause;
	CButtonST	m_Play;
	CButtonST	m_Stop;
	CButtonST	m_Help;
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(RMenuDlg)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG*);
	//}}AFX_VIRTUAL
public:
    int nextbutton() { 
        m_currentbutton++;
        if (m_currentbutton > m_maxbutton) {
            m_currentbutton = m_minbutton;
        }
        return m_currentbutton;
    }
    int prevbutton() {
        m_currentbutton--;
        if (m_currentbutton < m_minbutton) {
            m_currentbutton = m_maxbutton;
        }
        return m_currentbutton;
    }
	VirtualControl * m_Control;
    int currentbutton() { return m_currentbutton; }
	void OnControlClick();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(RMenuDlg)
//	afx_msg HBRUSH OnCtlColor( CDC* pDC, CWnd* pWnd, UINT nCtlColor );
	afx_msg void OnMenuClearplaylist();
	afx_msg void OnMenuExit();
	afx_msg void OnMenuLoadplaylist();
	afx_msg void OnMenuPause();
	afx_msg void OnMenuPlay();
	afx_msg void OnMenuShuffleplaylist();
	afx_msg void OnMenuStop();
	afx_msg void OnMenuHelp();
	afx_msg void OnMenuRandomizePlaylist();
	afx_msg void OnMenuSavePlaylist();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnExit();
private:
	int m_minbutton;
    int m_maxbutton;
    int m_currentbutton;

private:
//    MusicLib *m_mdb;
//    CPlayerDlg *m_pd;
	BOOL m_Resizing;
	void resetControls();
    void init();

	CRect m_AdjustRect,m_AdjustInt;
	BOOL m_AdjustLibrary, m_LibraryDragging;
	RegistryKey m_reg;

	PlayerCallbacks *m_parentcallbacks;	
	PlayerCallbacks m_callbacks;	
	COLORREF m_crTransMain,m_crTransPanel;
	int m_SysBarHeight ;
	CFont m_bfont;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RMenuDlg_H__7340F5BB_C8A5_11D5_8680_002078049F22__INCLUDED_)
