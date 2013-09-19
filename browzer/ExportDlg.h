#if !defined(AFX_EXPORTDLG_H__4A5C3B0F_977D_4091_8A47_6992969A6982__INCLUDED_)
#define AFX_EXPORTDLG_H__4A5C3B0F_977D_4091_8A47_6992969A6982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ExportDlg.h : header file
//
#include "resource.h"
/////////////////////////////////////////////////////////////////////////////
// ExportDlg dialog

class ExportDlg : public CDialog
{
// Construction
public:
	ExportDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ExportDlg)
	enum { IDD = IDD_EXPORT_DLG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ExportDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

public:
	BOOL m_Genre;
	BOOL m_Artist;
	BOOL m_Album;
	BOOL m_Song;
	BOOL m_Track;
	BOOL m_Length;
	BOOL m_Year;
	BOOL m_Commas;
//	BOOL m_Text;
	BOOL m_Html;
	BOOL m_Mp3File;
	CString m_Folder;
	CString m_File;
	CString m_HtmlTemplate;
	CString m_ExtraKeys;
	BOOL m_Again;
	BOOL m_Doit;

// Implementation
protected:
	void EnableDisable();
	// Generated message map functions
	//{{AFX_MSG(ExportDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	afx_msg void OnChange();
	afx_msg void OnExportOuputBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EXPORTDLG_H__4A5C3B0F_977D_4091_8A47_6992969A6982__INCLUDED_)
