// GetBlackOutDlg.h : header file
//

#if !defined(AFX_GETBLACKOUTDLG_H__D86B94CA_56E4_4248_B3AC_E7667BFE05AA__INCLUDED_)
#define AFX_GETBLACKOUTDLG_H__D86B94CA_56E4_4248_B3AC_E7667BFE05AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Board.h"
#include "GColorStatic.h"
#include "..\browzer\resource.h"


/////////////////////////////////////////////////////////////////////////////
// CGetBlackOutDlg dialog

class CGetBlackOutDlg : public CDialog
{
// Construction
public:
	CGetBlackOutDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CGetBlackOutDlg)
	enum { IDD = IDD_GETBLACKOUT_DIALOG };
	CButton	m_Win;
	GColorStatic	mBoardStatic;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGetBlackOutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnMouseDown( UINT nflags, CPoint point);
	void OnMouseUp( UINT nflags, CPoint point);
	void OnMouseMove( UINT nflags, CPoint point);
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CGetBlackOutDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	void resize();
	void drawall(CDC * cdc);
	Board mBoard;
	BOOL mMoving;
	CPoint mMouseStartPoint;
	int mPieceMoving;
	void winner();
	BOOL mWon;
	BOOL mAnimate;
	int mWinTimerId;
	int mystart;
	int mTotMoves;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETBLACKOUTDLG_H__D86B94CA_56E4_4248_B3AC_E7667BFE05AA__INCLUDED_)
