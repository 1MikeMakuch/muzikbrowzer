// mbpw.h : main header file for the MBPW application
//

#if !defined(AFX_MBPW_H__8FAAB19B_3990_41B4_BCE4_604A298B20C6__INCLUDED_)
#define AFX_MBPW_H__8FAAB19B_3990_41B4_BCE4_604A298B20C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMbpwApp:
// See mbpw.cpp for the implementation of this class
//

class CMbpwApp : public CWinApp
{
public:
	CMbpwApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMbpwApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMbpwApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MBPW_H__8FAAB19B_3990_41B4_BCE4_604A298B20C6__INCLUDED_)
