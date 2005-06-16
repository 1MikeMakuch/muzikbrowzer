// TransparentDialogDlg.h : header file
//

#if !defined(AFX_TRANSPARENTDIALOGDLG_H__CEDC202E_1C21_4C69_9C81_9DB0F64BCD1E__INCLUDED_)
#define AFX_TRANSPARENTDIALOGDLG_H__CEDC202E_1C21_4C69_9C81_9DB0F64BCD1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CTransparentDialogDlg dialog

class CTransparentDialogDlg : public CDialog
{
#define TRANSPARENTCOLOR RGB(255, 0, 0);
private:

	BITMAP  m_Bitmap;		// Struct to hold info about the bitmap
	HBITMAP m_hBitmap;		// Handle of the bitmap

// Construction
public:
	CTransparentDialogDlg(CWnd* pParent = NULL);	// standard constructor
	~CTransparentDialogDlg();
	CTime m_start;
// Dialog Data
	//{{AFX_DATA(CTransparentDialogDlg)
	enum { IDD = IDD_SPLASH_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTransparentDialogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CTransparentDialogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnExit();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TRANSPARENTDIALOGDLG_H__CEDC202E_1C21_4C69_9C81_9DB0F64BCD1E__INCLUDED_)
