#if !defined(AFX_SAVEPLAYLISTDLG_H__7340F5B8_C8A5_11D5_8680_002078049F22__INCLUDED_)
#define AFX_SAVEPLAYLISTDLG_H__7340F5B8_C8A5_11D5_8680_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SavePlaylistDlg.h : header file
//
#include "PlayerDlg.h"
/////////////////////////////////////////////////////////////////////////////
// SavePlaylistDlg dialog

class SavePlaylistDlg : public CDialog
{
// Construction
public:
	SavePlaylistDlg(CPlayerDlg *, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(SavePlaylistDlg)
	enum { IDD = IDD_SAVEPLAYLISTDIALOG };
	CString	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SavePlaylistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(SavePlaylistDlg)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    CPlayerDlg *m_pd;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SAVEPLAYLISTDLG_H__7340F5B8_C8A5_11D5_8680_002078049F22__INCLUDED_)
