


//////////////////////////////////////////////////////////////////////
// FileFolderDialog.cpp : Implementation File
//

#include "stdafx.h"
#include "FileFolderDialog.h"
#include "dlgs.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileFolderDialog
/* dummy file name for end dialog */
#define DUMMY_FILE "__dummyfile__"

IMPLEMENT_DYNAMIC(CFileFolderDialog, CFileDialog)

CFileFolderDialog::CFileFolderDialog(BOOL bOpenFileDialog,
									 LPCTSTR lpszDefExt,
									 LPCTSTR lpszFileName,
									 DWORD dwFlags,
									 LPCTSTR lpszFilter,
									 CWnd* pParentWnd) :
	CFileDialog(bOpenFileDialog,
		lpszDefExt, lpszFileName, dwFlags, lpszFilter, pParentWnd)
{
}
BEGIN_MESSAGE_MAP(CFileFolderDialog, CFileDialog)
//{{AFX_MSG_MAP(CFileFolderDialog)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

WNDPROC oldWindowProc; /* original window function to save*/
/* hooked window funcction */
static LRESULT CALLBACK SubclassWindowProc(HWND hwnd, UINT uMsg,
										   WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_COMMAND){
		if(LOWORD(wParam) == IDOK){
			/* if OK button is pushed and text box is empty, enter dummy filename for end dialog */
			char filename[1000];
			if(GetDlgItemText(hwnd, edt1, filename, sizeof(filename)) == 0){
				SetDlgItemText(hwnd, edt1, DUMMY_FILE);
			}
		}
	}
	return CallWindowProc(oldWindowProc, hwnd, uMsg, wParam, lParam);
}

BOOL CFileFolderDialog::OnInitDialog() 
{
	CFileDialog::OnInitDialog();
	oldWindowProc = (WNDPROC)::SetWindowLong(GetParent()->m_hWnd, GWL_WNDPROC, (DWORD)SubclassWindowProc);
	return TRUE;
}

CString CFileFolderDialog::GetPathName() const{
	CString pathname(m_szFileName);
	if(CString(m_szFileTitle) == DUMMY_FILE){
		int slashpos = pathname.ReverseFind('\\');
		if(slashpos != -1){ pathname = pathname.Left(slashpos); }
	}
	return pathname;
}
CString CFileFolderDialog::GetFileName() const{
	CString pathname = GetPathName();
	int slashpos = pathname.ReverseFind('\\');
	return pathname.Mid(slashpos + 1);
}
CString CFileFolderDialog::GetFileExt() const{
	CString filename = GetFileName();
	int dotpos = filename.ReverseFind('.');
	return (dotpos != -1) ? filename.Mid(dotpos+1) : "";
}
CString CFileFolderDialog::GetFileTitle() const{
	CString filename = GetFileName();
	int dotpos = filename.ReverseFind('.');
	return filename.Left(dotpos);
}

