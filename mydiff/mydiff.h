// mydiff.h : main header file for the MYDIFF application
//

#if !defined(AFX_MYDIFF_H__81BC0C1E_B361_4A04_8E4A_0DF7B07A001D__INCLUDED_)
#define AFX_MYDIFF_H__81BC0C1E_B361_4A04_8E4A_0DF7B07A001D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CMydiffApp:
// See mydiff.cpp for the implementation of this class
//

class CMydiffApp : public CWinApp
{
public:
	CMydiffApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMydiffApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CMydiffApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDIFF_H__81BC0C1E_B361_4A04_8E4A_0DF7B07A001D__INCLUDED_)
