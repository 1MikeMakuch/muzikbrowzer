#if !defined(AFX_CONFIGFILES_H__449E928C_3B06_11D6_8695_002078049F22__INCLUDED_)
#define AFX_CONFIGFILES_H__449E928C_3B06_11D6_8695_002078049F22__INCLUDED_
#include "Resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigFiles.h : header file
//

#include "InitDlg.h"
#include "PlayerCallbacks.h"
class ThreadParams;

/////////////////////////////////////////////////////////////////////////////
// CConfigFiles dialog

class CConfigFiles : public CPropertyPage
{
	DECLARE_DYNCREATE(CConfigFiles)

// Construction
public:
	CConfigFiles(CWnd * p = NULL,PlayerCallbacks * pcb =NULL);
	~CConfigFiles();

// Dialog Data
	//{{AFX_DATA(CConfigFiles)
	enum { IDD = IDD_CONFIG_FILES };
	CButton		m_RunAtStartup;
	CStatic		m_MdbLocation;
	CListBox	m_MP3DirList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CConfigFiles)
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
	//{{AFX_MSG(CConfigFiles)
	afx_msg void OnDiradd();
	afx_msg void OnSelchangeDirlist();
	afx_msg void OnDirremove();
	afx_msg void OnDirscan();
	afx_msg void OnLocationButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnDirscanNew();
	afx_msg void OnAlbumsortDate();
	afx_msg void OnAlbumsortAlpha();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
//    CPlayerDlg * m_PlayerDlg;
	PlayerCallbacks * m_playercallbacks;
    void EnableDisableButtons();
    CString m_path;

    CStringList m_origMp3Extensions;
    CString m_origMdbLocation;
    CStringList m_origMP3DirList;
	CStringList m_slMP3DirList;
    unsigned long m_RunAtStartupUL;
	BOOL m_AlbumSortAlpha;
	BOOL m_AlbumSortDate;
	BOOL m_LocDirModified;
	BOOL m_InitialAlbumSortAlpha;
	BOOL m_ResetNeeded;

    int m_ScanThread_continue;

    void ScanThreadStart(ThreadParams &);
    void ScanThreadStop();
    void StoreReg();
	void StoreReg2();
    void ReadReg();
    void init();
    void setRunAtStartup();
	void dirScan(CStringList &);
public:
	BOOL m_scanNew;
	BOOL m_bAdd;
    void setDefaults();
	BOOL AlbumSortAlpha();
	CString mbdir() { return m_path; };
	void AddMusic();
	void Scan(BOOL fornew);
	BOOL resetNeeded() { return m_ResetNeeded; }


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGFILES_H__449E928C_3B06_11D6_8695_002078049F22__INCLUDED_)
