#if !defined(AFX_MENUDIALOG_H__1F74495F_B143_11D5_8678_002078049F22__INCLUDED_)
#define AFX_MENUDIALOG_H__1F74495F_B143_11D5_8678_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuDialog.h : header file
//
//#include "PlayerApp.h"
//#include "StdAfx.h"
//#include "VirtualControl.h"
//#include "resource.h"
class VirtualControl;
#include "PlayerDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuDialog dialog

class CMenuDialog : public CDialog
{
// Construction
public:
	CMenuDialog(CWnd* pParent = NULL);   // standard constructor
	~CMenuDialog();
// Dialog Data
	//{{AFX_DATA(CMenuDialog)
	enum { IDD = IDD_MENU_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuDialog)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
    int nextbutton() { 
        m_currentbutton++;
        if (m_currentbutton > m_maxbutton) {
            m_currentbutton = 0;
        }
        return m_currentbutton;
    }
    int prevbutton() {
        m_currentbutton--;
        if (m_currentbutton < 0) {
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
	//{{AFX_MSG(CMenuDialog)
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
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnExit();
private:
    int m_maxbutton;
    int m_currentbutton;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUDIALOG_H__1F74495F_B143_11D5_8678_002078049F22__INCLUDED_)
