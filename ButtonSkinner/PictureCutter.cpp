
//

#include "stdafx.h"
#include <stdlib.h>
#include "PictureCutter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSliderSK

BEGIN_MESSAGE_MAP(CPictureCutter, CStatic)
    //{{AFX_MSG_MAP(CStatic)
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEMOVE() 
//	ON_WM_NCHITTEST()
//	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_WM_KEYDOWN( )
	ON_WM_KEYUP()
	ON_WM_SYSCHAR()
	ON_WM_SYSKEYDOWN( )
END_MESSAGE_MAP()

CPictureCutter::CPictureCutter(CWnd * parent) : m_Parent(parent)
{
	m_Shifted = FALSE;
}
CPictureCutter::~CPictureCutter() {

};
void CPictureCutter::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ) {
    if (nChar == VK_SHIFT) {
		m_Shifted = TRUE;
	}
}
void CPictureCutter::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags ){
    if (nChar == VK_SHIFT) {
		m_Shifted = FALSE;
	}
}

void CPictureCutter::OnSysChar( UINT nChar, UINT nRepCnt, UINT nFlags ) {
}

BOOL CPictureCutter::PreTranslateMessage(MSG* pMsg)
{	// disable ESC & ENTER from killing the dialog
	int x=0;
    if (pMsg->message == WM_KEYDOWN) {
        if (pMsg->wParam == VK_RETURN) {
			x++;
            return TRUE;
		} else if (pMsg->wParam == VK_ESCAPE) {
			x++;
			return TRUE;
        } else if (pMsg->wParam == VK_DELETE) {
			x++;
             return TRUE;
        } else if (pMsg->wParam == VK_LEFT) {
			x++;
		} else if (pMsg->wParam == VK_RIGHT) {
			x++;
		} else if (pMsg->wParam == VK_DOWN) {
			x++;
		} else if (pMsg->wParam == VK_UP) {
			x++;
		}
		if (x) {
			int flag=0;
			m_Parent->PostMessage(MB_BITMAP_CUTTER_MSG, (
				WPARAM)m_Shifted, (LPARAM)pMsg->wParam);
			return TRUE;
		}
    }
    return CStatic::PreTranslateMessage(pMsg);
}
void CPictureCutter::OnSysKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags ){
}

