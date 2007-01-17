#if !defined(AFX_CONFIGFILES_H__449E928C_3B06_11D6_8695_002078049F22__INCLUDED_)
#define AFX_CONFIGFILES_H__449E928C_3B06_11D6_8695_002078049F22__INCLUDED_
#include "Resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigFiles.h : header file
//

#include "PlayerCallbacks.h"
#include "MyString.h"
#include "HListBox.h"

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
	CEdit		m_ReplayGainMultiplierCtrl;
	CButton		m_DisplayCommentsButton;
	CButton		m_ReplayGainButton;
	CButton		m_LoggingButton;
	CButton		m_RunAtStartup;
	CStatic		m_MdbLocation;
	CHListBox	m_MP3DirList;
	CHListBox	m_ExcludeList;
	CButton		m_Mp3DirRemove;
	CButton		m_Mp3DirClear;
	CButton		m_ExRemove;
	CButton		m_ExClear;
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
	afx_msg void OnDiradd2();
	afx_msg void OnSelchangeDirlist();
	afx_msg void OnSelchangeDirlist2();
	afx_msg void OnDirremove();
	afx_msg void OnDirremove2();
	afx_msg void OnLocationButton();
	virtual BOOL OnInitDialog();
	afx_msg void OnAlbumsortDate();
	afx_msg void OnAlbumsortAlpha();
	afx_msg void OnRunatstartup();
	afx_msg void OnHideGenre();
	afx_msg void OnDirclear();
	afx_msg void OnExcludeclear();
	afx_msg void OnDebugLog();
	afx_msg void OnReplaygain();
	afx_msg void OnDisplayComments();
	afx_msg void OnChangeReplaygainMultiplier();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
//    CPlayerDlg * m_PlayerDlg;
	PlayerCallbacks * m_playercallbacks;
    CString m_path;

    CStringArray m_origMp3Extensions;
    CString m_origMdbLocation;
    CStringArray m_origMP3DirList;
	CStringArray m_slMP3DirList;
    CStringArray m_origExcludeList;
	CStringArray m_CSAExcludeList;

    unsigned long m_RunAtStartupUL;
	BOOL m_OrigRunAtStartup;
	BOOL m_AlbumSortAlpha;
	BOOL m_AlbumSortDate;
	BOOL m_LocDirModified;
	BOOL m_InitialAlbumSortAlpha;
	BOOL m_ResetNeeded;
	BOOL m_HideGenre;
	BOOL m_InitialHideGenre;
	BOOL m_ReplayGain;
	BOOL m_InitialReplayGain;
	int  m_Logging;
	int  m_InitialLogging;
	BOOL m_DisplayComments;
	BOOL m_InitialDisplayComments;
	CString m_ReplayGainMultiplier;
	CString m_InitialReplayGainMultiplier;

    void StoreReg();
	void WriteFolders();
    void ReadReg();
	void ReadFolders();
    void init();
    void setRunAtStartup();
	void EnableDisable();
	
public:

    void setDefaults();
	BOOL AlbumSortAlpha();
	CString mbdir() { return m_path; };
	BOOL resetNeeded() { return m_ResetNeeded; }
	void GetDirs(CStringArray & dirs, CStringArray & excludes,const CString & dir);
	void AddFolders(const CStringArray & dirs);
	void DelFolders(const CStringArray & dirs);
	BOOL hideGenre() { return m_HideGenre; }
	void list2box(const CStringArray & list,CHListBox & box);
	void AddDeletedFiles(const CStringArray & files);
	void getSettings(MyHash & settings);
	int logging() { return m_Logging; }
	BOOL ReplayGain() { return m_ReplayGain; }
	BOOL DisplayComments() { return m_DisplayComments;}
	double ReplayGainMultiplier();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGFILES_H__449E928C_3B06_11D6_8695_002078049F22__INCLUDED_)
