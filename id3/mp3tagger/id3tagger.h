// id3tagger.h : main header file for the ID3TAGGER application
//

#if !defined(AFX_ID3TAGGER_H__D74622E3_A5AD_44B3_B6F3_93F2D1511107__INCLUDED_)
#define AFX_ID3TAGGER_H__D74622E3_A5AD_44B3_B6F3_93F2D1511107__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CId3taggerApp:
// See id3tagger.cpp for the implementation of this class
//

class CId3taggerApp : public CWinApp
{
public:
	CId3taggerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CId3taggerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CId3taggerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ID3TAGGER_H__D74622E3_A5AD_44B3_B6F3_93F2D1511107__INCLUDED_)
