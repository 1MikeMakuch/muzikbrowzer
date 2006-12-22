#if !defined(AFX_MBMessageBoxImpl_H__3BB3C3F6_2B0A_11D6_8691_002078049F22__INCLUDED_)
#define AFX_MBMessageBoxImpl_H__3BB3C3F6_2B0A_11D6_8691_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MBMessageBoxImpl.h : header file
//
#include "Resource.h"
#include "MBGlobals.h"
#include "ColorStatic.h"

#define WS_EX_LAYOUT_RTL        0x00400000

class VirtualControl;
/////////////////////////////////////////////////////////////////////////////
// MBMessageBoxImpl dialog

class MBMessageBoxImpl : public CDialog
{
// Construction
public:
	MBMessageBoxImpl(CString & title, CString & info, BOOL log = TRUE,
		BOOL enableCancel=FALSE, CWnd* pParent = NULL);   // standard constructor
	~MBMessageBoxImpl();
// Dialog Data
	//{{AFX_DATA(MBMessageBoxImpl)
	enum { IDD = IDD_MESSAGE_DIALOG };
	CEdit	m_MessageBox2;
	CEdit	m_MessageBox;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MBMessageBoxImpl)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL
	BOOL InitGrip();
	void UpdateGripPos();
	void ShowSizeGrip(BOOL bShow = TRUE);	// show or hide the size grip
	void CalcMsgRect(CRect &);


// Implementation

public:
	VirtualControl * m_Control;
	void SetTabStop(const unsigned int s);

	// Generated message map functions
	//{{AFX_MSG(MBMessageBoxImpl)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnSetfocusMessageBox();
	afx_msg void OnSetfocusOk();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnExit();
private:
	void resizeControls();
    CString m_info;
    CString m_title;
	BOOL m_log;
	CFont mfont;
	BOOL m_EnableCancel;
	CEdit * m_MessageBoxPtr;
	int m_MinWidth;
	int m_MinHeight;
	SIZE m_sizeGrip;		// holds grip size
	CScrollBar m_wndGrip;
	CWnd * m_Parent;
	unsigned int m_tabStop;

	static LRESULT CALLBACK 
		GripWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static BOOL IsRTL(HWND hwnd)
	{
		DWORD dwExStyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
		return (dwExStyle & WS_EX_LAYOUT_RTL);
	};

};

// returns 1 for OK and 0 for Cancel
int MBMessageBox(CString title, CString msg, BOOL log = TRUE, 
				 BOOL enableCancel=FALSE, const unsigned int tabstops=0);

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.





#endif // !defined(AFX_MBMessageBoxImpl_H__3BB3C3F6_2B0A_11D6_8691_002078049F22__INCLUDED_)
