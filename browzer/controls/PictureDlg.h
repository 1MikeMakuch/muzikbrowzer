#if !defined(AFX_PICTUREDLG_H__171123E7_AE9C_4DEC_A8D0_545B620D0466__INCLUDED_)
#define AFX_PICTUREDLG_H__171123E7_AE9C_4DEC_A8D0_545B620D0466__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Resource.h"
#include "Picture.h"
#include "PictureStatic.h"
// PictureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPictureDlg dialog

class CPictureDlg : public CDialog
{
// Construction
public:
	CPictureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CPictureDlg)
	enum { IDD = IDD_PICTUREDLG };
	PictureStatic	m_PictureCtl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPictureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL LoadFromBuffer(BYTE* pBuff, int nSize, const CRect & rect);
	void resizeControls();
protected:
	CPicture m_Picture;
	long m_width;
	long m_height;
	CPoint m_UpperRight;

	// Generated message map functions
	//{{AFX_MSG(CPictureDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PICTUREDLG_H__171123E7_AE9C_4DEC_A8D0_545B620D0466__INCLUDED_)
