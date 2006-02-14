#if !defined(AFX_FILEANDFOLDER_H__7176A2C6_5200_4076_A620_30212ECEEB2F__INCLUDED_)
#define AFX_FILEANDFOLDER_H__7176A2C6_5200_4076_A620_30212ECEEB2F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// fileandfolder.h : header file
//

#include "Resource.h"
#include "TreeCtrlEx.h"
//#include "ResizableGrip.h"
#define WS_EX_LAYOUT_RTL        0x00400000

/////////////////////////////////////////////////////////////////////////////
// CFileAndFolder dialog

//Class which encapsulates access to the System image list which contains
//all the icons used by the shell to represent the file system
class CSystemImageList
{
public:
//Constructors / Destructors
  CSystemImageList();
  ~CSystemImageList();

//Methods
  CImageList& GetImageList();

protected:
  CImageList m_ImageList;
  static int m_nRefCount;
};

class CFileAndFolder : public CDialog
	
{
// Construction
public:
	CFileAndFolder(CWnd* pParent, CString dflt = "");   // standard constructor
	void SetShowFiles(BOOL bFiles) ;
	void GetPaths(CStringList &);
	void setMsg(const CString &);
	void setTitle(const CString &);

	// Dialog Data
	//{{AFX_DATA(CFileAndFolder)
	enum { IDD = IDD_FILEANDFOLDER };
	CStatic	m_Msg;
	CTreeCtrlEx	m_Tree;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileAndFolder)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnSizing(UINT fwSide, LPRECT pRect);
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL             m_bShowFiles;
	CStringList		m_sSelections;

	// Generated message map functions
	//{{AFX_MSG(CFileAndFolder)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRefreshDrives();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	void OnViewRefresh();
	void DisplayDrives(HTREEITEM hParent, BOOL bUseSetRedraw);
	HTREEITEM InsertFileItem(const CString& sFile, 
		const CString& sPath, HTREEITEM hParent);
	int GetIconIndex(HTREEITEM hItem);
	int GetIconIndex(const CString& sFilename);
	int GetSelIconIndex(const CString& sFilename);
	int GetSelIconIndex(HTREEITEM hItem);
	BOOL HasGotSubEntries(const CString& sDirectory);
	void UpOneLevel();
	void OnUpdateUpOneLevel(CCmdUI* pCmdUI);
	BOOL IsFile(HTREEITEM hItem);
	BOOL IsFolder(HTREEITEM hItem);
	BOOL IsDrive(HTREEITEM hItem);
	BOOL IsFile(const CString& sPath);
	BOOL IsFolder(const CString& sPath);
	BOOL IsDrive(const CString& sPath);
	void DisplayPath(const CString& sPath, HTREEITEM hParent, 
		BOOL bUseSetRedraw=TRUE);
	CString ItemToPath(HTREEITEM hItem);
	static int CompareByFilenameNoCase(CString& element1, CString& element2);
	CString GetFullPath(HTREEITEM htree);
	void resizeControls();
	void ShowDefault();
	void ReadCachedDrives();
	void WriteCachedDrives();
	void OnViewCachedDrives();

// all below from CResizableGrip
private:
	SIZE m_sizeGrip;		// holds grip size
	CScrollBar m_wndGrip;
	static BOOL IsRTL(HWND hwnd)
	{
		DWORD dwExStyle = ::GetWindowLong(hwnd, GWL_EXSTYLE);
		return (dwExStyle & WS_EX_LAYOUT_RTL);
	};

	static LRESULT CALLBACK 
		GripWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	CString m_msg;
	CString m_title;
	CString m_default;

protected:
	BOOL InitGrip();
	void UpdateGripPos();
	void ShowSizeGrip(BOOL bShow = TRUE);	// show or hide the size grip
	CSystemImageList m_SystemImageList;
	CStringList m_CachedDrives;



};
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEANDFOLDER_H__7176A2C6_5200_4076_A620_30212ECEEB2F__INCLUDED_)
