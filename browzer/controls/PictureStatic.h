#if !defined(AFX_PICTURESTATIC_H__6A27855D_00DD_4FE0_89C1_7D7CB7F8BBF7__INCLUDED_)
#define AFX_PICTURESTATIC_H__6A27855D_00DD_4FE0_89C1_7D7CB7F8BBF7__INCLUDED_

#include "Picture.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PictureStatic.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// PictureStatic window

class PictureStatic : public CStatic
{
// Construction
public:
	PictureStatic(bool square=true);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(PictureStatic)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~PictureStatic();
	void load(BYTE * data, size_t size);
	void load(LPTSTR type, LPTSTR resource);
	void load(CString filename);
	void unload();
	void blank();
	void SetBgColor(COLORREF clr) {
		m_BgColor = clr;
	}

	// Generated message map functions
protected:
	CPicture * m_Picture;
	BYTE * m_bytes;
	BOOL m_ploaded;
	COLORREF m_BgColor;
	//{{AFX_MSG(PictureStatic)
	afx_msg void OnPaint();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTURESTATIC_H__6A27855D_00DD_4FE0_89C1_7D7CB7F8BBF7__INCLUDED_)
