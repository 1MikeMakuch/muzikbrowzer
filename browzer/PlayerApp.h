// PlayerApp.h : main header file for the MFCPLAYER application
//

#if !defined(AFX_PLAYERAPP_H__FCF45C44_7793_11D2_9317_0020AFF7E192__INCLUDED_)
#define AFX_PLAYERAPP_H__FCF45C44_7793_11D2_9317_0020AFF7E192__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

//#include "StdAfx.h"
#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CPlayerApp:
// See PlayerApp.cpp for the implementation of this class
//
class CPlayerDlg;

class CPlayerApp : public CWinApp
{
public:
	CPlayerApp();
	void RunTestHarness();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPlayerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	private:
		CPlayerDlg * m_PlayerDlg;
		BOOL m_rebuild;
		CString m_dir;

	public:
// Implementation

	//{{AFX_MSG(CPlayerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()



};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PLAYERAPP_H__FCF45C44_7793_11D2_9317_0020AFF7E192__INCLUDED_)
