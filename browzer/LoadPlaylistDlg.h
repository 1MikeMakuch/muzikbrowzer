#if !defined(AFX_LOADPLAYLISTDLG_H__7340F5BB_C8A5_11D5_8680_002078049F22__INCLUDED_)
#define AFX_LOADPLAYLISTDLG_H__7340F5BB_C8A5_11D5_8680_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LoadPlaylistDlg.h : header file
//
#include "PlayerDlg.h"
#include "MusicDb.h"
#include "ExtendedListBox.h"

/////////////////////////////////////////////////////////////////////////////
// LoadPlaylistDlg dialog
class VirtualControl;

class LoadPlaylistDlg : public CDialog
{
// Construction
public:
	LoadPlaylistDlg(CPlayerDlg *p, MusicLib *m, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LoadPlaylistDlg)
	enum { IDD = IDD_LOADPLAYLISTDLG };
	CButton	m_Delete;
	CButton	m_Load;
	CStatic	m_GAAS;
	//}}AFX_DATA
	CExtendedListBox	m_PlaylistSongs;
	CExtendedListBox	m_PlaylistNames;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LoadPlaylistDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
public:
    VirtualControl * m_Control;
    void OnControlSelChange();
    void OnControlClick();
    void OnDelete();
    void OnPageUp();
    void OnPageDown();
    void OnMenuButton();
    void OnExit();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LoadPlaylistDlg)
	afx_msg void OnDblclkPlaylistNames();
	afx_msg void OnSelchangePlaylistNames();
	afx_msg void OnSetfocusPlaylistNames();
	virtual BOOL OnInitDialog();
	afx_msg void OnSetfocusPlaylistSongs();
	afx_msg void OnCancel();
	afx_msg void OnPlaylistOk();
	afx_msg void OnDeletePlaylist();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusPlaylistSongs();
	afx_msg void OnSelchangePlaylistSongs();
	afx_msg void OnPlaylistSave();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
    MusicLib *m_mdb;
    CPlayerDlg *m_pd;

    void init();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOADPLAYLISTDLG_H__7340F5BB_C8A5_11D5_8680_002078049F22__INCLUDED_)
