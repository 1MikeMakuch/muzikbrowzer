#if !defined(AFX_ID3DISPLAY_H__896E6809_DA10_11D5_8680_002078049F22__INCLUDED_)
#define AFX_ID3DISPLAY_H__896E6809_DA10_11D5_8680_002078049F22__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ID3Display.h : header file
//
#include "id3/tag.h"
#include "Resource.h"
#include "MusicDb.h"
/////////////////////////////////////////////////////////////////////////////
// ID3Display dialog

class ID3Display : public CDialog
{
// Construction
public:
	ID3Display(Song song, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(ID3Display)
	enum { IDD = IDD_ID3_DISPLAY };
	CEdit	m_ID3Display;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(ID3Display)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(ID3Display)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
//    ID3_Tag * mID3_Tag;
	Song m_Song;
	CString displayTag(Song song);
	CString displayOgg(const CString & file);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ID3DISPLAY_H__896E6809_DA10_11D5_8680_002078049F22__INCLUDED_)
