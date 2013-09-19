
#ifndef __PICTURE_CUTTER_H__
#define __PICTURE_CUTTER_H__


#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "MBGlobals.h"

class CPictureCutter: public CStatic
{
public:
	CPictureCutter(CWnd * parent=NULL);
	~CPictureCutter();

protected:
	CWnd * m_Parent;
	BOOL m_Shifted ;
    //{{AFX_MSG(CMySliderSK)
	//}}AFX_MSG
	afx_msg void OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg void OnSysChar( UINT nChar, UINT nRepCnt, UINT nFlags );
	afx_msg BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSysKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags );



private:

	DECLARE_MESSAGE_MAP()
};

#endif