#if !defined(AFX_FILEFOLDERDIALOG_H__A87A6F25_3446_11D3_AA89_00A02475A9BC__INCLUDED_)
#define AFX_FILEFOLDERDIALOG_H__A87A6F25_3446_11D3_AA89_00A02475A9BC__INCLUDED_

class CFileFolderDialog : public CFileDialog
{
DECLARE_DYNAMIC(CFileFolderDialog)

	public:
		CFileFolderDialog(BOOL bOpenFileDialog, // FileOpen(TRUE) or FileSave(FALSE)
			LPCTSTR lpszDefExt = NULL,
			LPCTSTR lpszFileName = NULL,
			DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			LPCTSTR lpszFilter = NULL,
			CWnd* pParentWnd = NULL);
		virtual CString GetPathName() const;
		virtual CString GetFileName() const;
		virtual CString GetFileExt() const;
		virtual CString GetFileTitle() const;
	protected:
		// CString m_strPathName;	// selected full path name

	protected:
		//{{AFX_MSG(CFileFolderDialog)
		virtual BOOL OnInitDialog();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}

#endif // !defined(AFX_FILEFOLDERDIALOG_H__A87A6F25_3446_11D3_AA89_00A02475A9BC__INCLUDED_)

//////////////////////////////////////////////////////////////////////

