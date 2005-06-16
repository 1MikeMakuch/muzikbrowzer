// BitmapCutter.h : main header file for the BITMAPCUTTER application
//

#if !defined(AFX_BITMAPCUTTER_H__0F575A87_7CBD_4BA6_977A_AE32B3604EE0__INCLUDED_)
#define AFX_BITMAPCUTTER_H__0F575A87_7CBD_4BA6_977A_AE32B3604EE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CBitmapCutterApp:
// See BitmapCutter.cpp for the implementation of this class
//

class CBitmapCutterApp : public CWinApp
{
public:
	CBitmapCutterApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBitmapCutterApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CBitmapCutterApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BITMAPCUTTER_H__0F575A87_7CBD_4BA6_977A_AE32B3604EE0__INCLUDED_)
