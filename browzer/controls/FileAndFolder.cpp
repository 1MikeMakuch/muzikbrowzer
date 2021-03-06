// FileAndFolder.cpp : implementation file
//


#include "stdafx.h"
#include "FileAndFolder.h"
#include "SortedArray.h"
#include "MBGlobals.h"
#include "MyLog.h"
#include "MyString.h"
#include "TestHarness/TestHarness.h"
#include "FileUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSystemImageList::CSystemImageList()
{
  //We need to implement reference counting to 
  //overcome an MFC limitation whereby you cannot
  //have two CImageLists attached to the one underlyinh
  //HIMAGELIST. If this was not done then you would get 
  //an ASSERT in MFC if you had two or more CTreeFileCtrl's
  //in your program at the same time
  if (m_nRefCount == 0)
  {
    //Attach to the system image list
    SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
    HIMAGELIST hSystemImageList = (HIMAGELIST) SHGetFileInfo(_T("C:\\"), 0, &sfi, sizeof(SHFILEINFO),
                                                             SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
    VERIFY(m_ImageList.Attach(hSystemImageList));
	if (sfi.hIcon)
	  DestroyIcon(sfi.hIcon);
  }  

  //Increment the reference count
  m_nRefCount++;
}

CSystemImageList::~CSystemImageList()
{
  //Decrement the reference count
  m_nRefCount--;  

  if (m_nRefCount == 0)
  {
    //Detach from the image list to prevent problems on 95/98 where
    //the system image list is shared across processes
    m_ImageList.Detach();
  }
}

CImageList& CSystemImageList::GetImageList()
{
  return m_ImageList;
}

int CSystemImageList::m_nRefCount=0;

/////////////////////////////////////////////////////////////////////////////
// CFileAndFolder dialog


CFileAndFolder::CFileAndFolder(CWnd* pParent, CString dflt)
	: CDialog(CFileAndFolder::IDD, pParent), m_default(dflt)
{
	//{{AFX_DATA_INIT(CFileAndFolder)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_bShowFiles = TRUE;
	m_Tree.setp(this);

	m_sizeGrip.cx = GetSystemMetrics(SM_CXVSCROLL);
	m_sizeGrip.cy = GetSystemMetrics(SM_CYHSCROLL);
}


void CFileAndFolder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileAndFolder)
	DDX_Control(pDX, IDC_MSG, m_Msg);
	DDX_Control(pDX, IDC_TREE, m_Tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFileAndFolder, CDialog)
	//{{AFX_MSG_MAP(CFileAndFolder)
	ON_MESSAGE(MB_TV_MSG, OnItemexpanding)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_BN_CLICKED(IDC_REFRESH_DRIVES, OnRefreshDrives)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileAndFolder message handlers

BOOL CFileAndFolder::OnInitDialog() 
{
//	m_Tree.SubclassDlgItem(IDC_TREE, this);
	CDialog::OnInitDialog();

	m_Tree.SetImageList(&m_SystemImageList.GetImageList(), TVSIL_NORMAL);	
	ReadCachedDrives();
	OnViewCachedDrives();
	ShowDefault();
	InitGrip();
	ShowSizeGrip(TRUE);
	m_Msg.SetWindowText(m_msg);
	SetWindowText(m_title);
	m_sSelections.RemoveAll();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void CFileAndFolder::ReadCachedDrives() {
	CFile file;
    CFileException fileException;
    if (!file.Open( (LPCTSTR)"MuzikbrowzerCachedDrives", 
			CFile::modeRead,
			&fileException )) {
		return;
	}

	CArchive ar( &file, CArchive::load);
	m_CachedDrives.RemoveAll();
	m_CachedDrives.Serialize(ar);
	ar.Close();
	file.Close();
}
void CFileAndFolder::WriteCachedDrives() {
	CFile file;
    CFileException fileException;
    if (!file.Open( (LPCTSTR)"MuzikbrowzerCachedDrives", 
			CFile::modeCreate | CFile::modeWrite,
			&fileException )) {
		return;
	}

	CArchive ar( &file, CArchive::store);
	m_CachedDrives.Serialize(ar);
	ar.Close();
	file.Close();
}

void
CFileAndFolder::ShowDefault() {
	if (m_default.GetLength() == 0) {
		return;
	}
	CString drive = m_default.Left(3);
	HTREEITEM htree = m_Tree.GetNextItem(TVI_ROOT, TVGN_ROOT);
	FindDefault(htree);
	return;

	while (htree) {
		CString idrive = m_Tree.getItemText(htree);
		if (idrive.CompareNoCase(drive) == 0) {
			DisplayPath(drive, htree);
			m_Tree.Expand(htree,TVE_EXPAND);
			return;
		}
		htree = m_Tree.GetNextItem(htree, TVGN_NEXT);
	}
}
void
CFileAndFolder::FindDefault(HTREEITEM htree, const CString path) {

	
	while (htree) {
		CString partialPath = m_Tree.getItemText(htree);
		logger.ods("Find:"+partialPath);
		CString pathSoFar ;
		if (path.GetLength()) {
			if (!String::endsWith(path,"\\")) {
				pathSoFar = path + "\\" + partialPath;
			} else {
				pathSoFar = path + partialPath;
			}
		} else {
			pathSoFar = partialPath;
		}

		if (FileUtil::IsParentPath(m_default,pathSoFar)) {
			DisplayPath(partialPath, htree);
			m_Tree.Expand(htree,TVE_EXPAND);
			if (m_default.CompareNoCase(pathSoFar) == 0) {
				return;
			}
			htree = m_Tree.GetChildItem(htree);

			FindDefault(htree, pathSoFar);
		}
		htree = m_Tree.GetNextItem(htree, TVGN_NEXT);
	}
}
#ifdef asdf
TEST(CFileAndFolder,test)
{
	CDialog d;
	CString dflt("N:\\musictests\\AeroClips");
	CFileAndFolder dialog(&d, dflt);
	dialog.DoModal();
	exit(0);
}
#endif
CString CFileAndFolder::GetFullPath(HTREEITEM htree) {
	CString path,tmp;
	path = m_Tree.getItemText(htree);
	HTREEITEM hParent = m_Tree.GetParentItem(htree);
	while (hParent) {
		tmp = m_Tree.getItemText(hParent);
		if (tmp.GetAt(tmp.GetLength()-1) == _T('\\')) {
			path = tmp + path;
		} else {
			path =  tmp + "\\" + path;
		}
		hParent = m_Tree.GetParentItem(hParent);
	}
	return path;
}
void CFileAndFolder::OnOK() 
{
	UINT count = m_Tree.GetSelectedCount();
	if (count) {
		HTREEITEM htree = m_Tree.GetFirstSelectedItem();
		m_sSelections.AddTail(GetFullPath(htree));
		while ((htree = m_Tree.GetNextSelectedItem(htree)) != NULL) {
			m_sSelections.AddTail(GetFullPath(htree));
		}
	}
	
	CDialog::OnOK();
}
void CFileAndFolder::GetPaths(CStringList & list) {
	POSITION pos;
	list.RemoveAll();
	for(pos = m_sSelections.GetHeadPosition() ; pos != NULL ;) {
		list.AddTail(m_sSelections.GetNext(pos));
	}
}

void CFileAndFolder::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

void CFileAndFolder::OnViewRefresh() 
{
//	HTREEITEM hSelItem = GetSelectedItem();
//	CString sItem  = ItemToPath(hSelItem);
//	BOOL bExpanded = (GetChildItem(hSelItem) != NULL); 

    DisplayDrives(TVI_ROOT, FALSE);
//	if (sItem.GetLength())
//		hSelItem = SetSelectedPath(sItem, bExpanded);
}
void CFileAndFolder::OnViewCachedDrives() {
	CWaitCursor c;
	if (m_CachedDrives.GetCount() == 0) {
		DisplayDrives(TVI_ROOT, FALSE);
		return;
	}
    POSITION pos = m_CachedDrives.GetHeadPosition();
    for (pos = m_CachedDrives.GetHeadPosition(); pos != NULL; ) {
        CString string = m_CachedDrives.GetAt(pos);
		CString drive,desc;
		drive = string.Left(3);
		desc = string.Mid(4);
		InsertFileItem(desc,drive,TVI_ROOT);

        m_CachedDrives.GetNext(pos);
    }
}
void CFileAndFolder::DisplayDrives(HTREEITEM hParent, BOOL bUseSetRedraw)
{
  CWaitCursor c;

  //Remove any items currently in the tree
   m_Tree.DeleteAllItems();

  //Enumerate the drive letters and add them to the tree control
  DWORD dwDrives = GetLogicalDrives();
  DWORD dwMask = 1;
  m_CachedDrives.RemoveAll();
  for (int i=0; i<32; i++)
  {
    if (dwDrives & dwMask)
    {
      CString sDrive,desc;
      sDrive.Format(_T("%c:\\"), i + _T('A'));
	  char volname[100];
	  char fsname[100];
	  DWORD sn,maxcl,flags;
	  memset(volname,0,100);

	  BOOL x = GetVolumeInformation(
		  sDrive,volname,99,&sn,&maxcl,&flags,fsname,99);
	
	  if (volname) {
		  desc = CString("(") + sDrive + ") " + volname;
	  } else 
		  desc = sDrive;

      InsertFileItem(desc, sDrive, hParent);
	  m_CachedDrives.AddTail(sDrive + "," + desc);
    }
    dwMask <<= 1;
  }
  WriteCachedDrives();
}
HTREEITEM CFileAndFolder::InsertFileItem(const CString& sFile, 
								const CString& sPath, HTREEITEM hParent)
{
  //Retreive the icon indexes for the specified file/folder
  int nIconIndex = GetIconIndex(sPath);
  int nSelIconIndex = GetSelIconIndex(sPath);
  if (nIconIndex == -1 || nSelIconIndex == -1)
  {
    TRACE(_T("Failed in call to SHGetFileInfo for %s, GetLastError:%d\n"), sPath, ::GetLastError());
    return NULL;
  }

  //Add the actual item
	CString sTemp(sFile);
	TV_INSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(TV_INSERTSTRUCT));

	tvis.hParent = hParent;
	tvis.hInsertAfter = TVI_LAST;
	tvis.item.mask = TVIF_CHILDREN | TVIF_IMAGE | TVIF_SELECTEDIMAGE 
		| TVIF_TEXT;
	tvis.item.pszText = sTemp.GetBuffer(sTemp.GetLength());
	tvis.item.cchTextMax = sTemp.GetLength();
	tvis.item.iImage = nIconIndex;
	tvis.item.iSelectedImage = nSelIconIndex;
	tvis.item.cChildren = HasGotSubEntries(sPath);
  HTREEITEM hItem = m_Tree.InsertItem(&tvis);
  sTemp.ReleaseBuffer();

  return hItem;
}
int CFileAndFolder::GetIconIndex(HTREEITEM hItem)
{
  TV_ITEM tvi;
  ZeroMemory(&tvi, sizeof(TV_ITEM));
  tvi.mask = TVIF_IMAGE;
  tvi.hItem = hItem;
  if (m_Tree.GetItem(&tvi))
    return tvi.iImage;
  else
    return -1;
}
int CFileAndFolder::GetIconIndex(const CString& sFilename)
{
	//Retreive the icon index for a specified file/folder
	SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	DWORD r = SHGetFileInfo(sFilename, 0, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_SMALLICON);
	if (sfi.hIcon) 
		DestroyIcon(sfi.hIcon);
	if (0 == r)
		return -1;
	return sfi.iIcon;
}

int CFileAndFolder::GetSelIconIndex(const CString& sFilename)
{
	//Retreive the icon index for a specified file/folder
	SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	DWORD r = SHGetFileInfo(sFilename, 0, &sfi, sizeof(SHFILEINFO), SHGFI_ICON | SHGFI_OPENICON | SHGFI_SMALLICON);
	if (sfi.hIcon) 
		DestroyIcon(sfi.hIcon);
	if (0 == r)
		return -1;
	return sfi.iIcon;
}

int CFileAndFolder::GetSelIconIndex(HTREEITEM hItem)
{
  TV_ITEM tvi;
  ZeroMemory(&tvi, sizeof(TV_ITEM));
  tvi.mask = TVIF_SELECTEDIMAGE;
  tvi.hItem = hItem;
  if (m_Tree.GetItem(&tvi))
    return tvi.iSelectedImage;
  else
    return -1;
}
BOOL CFileAndFolder::HasGotSubEntries(const CString& sDirectory)
{
  ASSERT(sDirectory.GetLength());
  CFileFind find;
  CString sFile;
  if (sDirectory.GetAt(sDirectory.GetLength()-1) == _T('\\'))
    sFile = sDirectory + _T("*.*");
  else
    sFile = sDirectory + _T("\\*.*");
  BOOL bFind = find.FindFile(sFile);  
  while (bFind)
  {
    bFind = find.FindNextFile();
    if (find.IsDirectory() && !find.IsDots())
      return TRUE;
    else if (!find.IsDirectory() && !find.IsHidden() && m_bShowFiles)
      return TRUE;
  }

  return FALSE;
}
void CFileAndFolder::SetShowFiles(BOOL bFiles) 
{ 
  m_bShowFiles = bFiles; 
  if (IsWindow(GetSafeHwnd()))
    OnViewRefresh();
}


void CFileAndFolder::UpOneLevel()
{
  HTREEITEM hItem = m_Tree.GetSelectedItem();
  if (hItem)
  {
    HTREEITEM hParent = m_Tree.GetParentItem(hItem);
    if (hParent)
      m_Tree.Select(hParent, TVGN_CARET);
  }
}

void CFileAndFolder::OnUpdateUpOneLevel(CCmdUI* pCmdUI)
{
  HTREEITEM hItem = m_Tree.GetSelectedItem();
  if (hItem)
    pCmdUI->Enable(m_Tree.GetParentItem(hItem) != NULL);
  else
    pCmdUI->Enable(FALSE);
}

BOOL CFileAndFolder::IsFile(HTREEITEM hItem)
{
  return IsFile(ItemToPath(hItem));
}

BOOL CFileAndFolder::IsFolder(HTREEITEM hItem)
{
  return IsFolder(ItemToPath(hItem));
}

BOOL CFileAndFolder::IsDrive(HTREEITEM hItem)
{
  return IsDrive(ItemToPath(hItem));
}

BOOL CFileAndFolder::IsFile(const CString& sPath)
{
  return ((GetFileAttributes(sPath) & FILE_ATTRIBUTE_DIRECTORY) == 0);
}

BOOL CFileAndFolder::IsFolder(const CString& sPath)
{
  return ((GetFileAttributes(sPath) & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

BOOL CFileAndFolder::IsDrive(const CString& sPath)
{
  return (sPath.GetLength() == 3 && sPath.GetAt(1) == _T(':') && sPath.GetAt(2) == _T('\\'));
}


void CFileAndFolder::DisplayPath(const CString& sPath, 
								 HTREEITEM hParent, BOOL bUseSetRedraw)
{
  CWaitCursor c;

  //Speed up the job by turning off redraw
//  if (bUseSetRedraw)
//    SetRedraw(FALSE);

  //Remove all the items currently under hParent
  HTREEITEM hChild = m_Tree.GetChildItem(hParent);
  while (hChild)
  {
    m_Tree.DeleteItem(hChild);
    hChild = m_Tree.GetChildItem(hParent);
  }

  //Find all the directories and files underneath sPath
  CSortedArray<CString, CString&> DirectoryPaths;
  CSortedArray<CString, CString&> FilePaths;
  CFileFind find;
  CString sFile;
  if (sPath.GetAt(sPath.GetLength()-1) == _T('\\'))
    sFile = sPath + _T("*.*");
  else
    sFile = sPath + _T("\\*.*");
  
  BOOL bFind = find.FindFile(sFile);  
  while (bFind)
  {
    bFind = find.FindNextFile();
    if (find.IsDirectory())
    {
      if (!find.IsDots())
      {
        CString sPath = find.GetFilePath();
        DirectoryPaths.Add(sPath);
      }
    }
    else 
    {
      if (!find.IsHidden() && m_bShowFiles)
      {
        CString sPath = find.GetFilePath();
        FilePaths.Add(sPath);
      }  
    }
  }

  //Now sort the 2 arrays prior to added to the tree control
  DirectoryPaths.SetCompareFunction(CompareByFilenameNoCase);
  FilePaths.SetCompareFunction(CompareByFilenameNoCase);
  DirectoryPaths.Sort();
  FilePaths.Sort();

  //Now add all the directories to the tree control
  for (int i=0; i<DirectoryPaths.GetSize(); i++)
  {
    CString& sPath = DirectoryPaths.ElementAt(i);
    TCHAR path_buffer[_MAX_PATH];
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];
    _tsplitpath(sPath, NULL, NULL, fname, ext);
    _tmakepath(path_buffer, NULL, NULL, fname, ext);
    InsertFileItem(path_buffer, sPath, hParent);
  }

  //And the files to the tree control (if required)
  for (i=0; i<FilePaths.GetSize(); i++)
  {
    CString& sPath = FilePaths.ElementAt(i);
    TCHAR path_buffer[_MAX_PATH];
    TCHAR fname[_MAX_FNAME];
    TCHAR ext[_MAX_EXT];
    _tsplitpath(sPath, NULL, NULL, fname, ext);
    _tmakepath(path_buffer, NULL, NULL, fname, ext);
    InsertFileItem(path_buffer, sPath, hParent);
  }

  //Turn back on the redraw flag
//  if (bUseSetRedraw)
//    SetRedraw(TRUE);
}

CString CFileAndFolder::ItemToPath(HTREEITEM hItem)
{
  CString sPath;
  
  //Create the full string of the tree item
  HTREEITEM hParent = hItem;
  while (hParent)
  {
    CString sItem = m_Tree.getItemText(hParent);
    int nLength = sItem.GetLength();
	if (nLength == 0) {
		return sPath;
	}

    ASSERT(nLength);
    hParent = m_Tree.GetParentItem(hParent);

    if (sItem.GetAt(nLength-1) == _T('\\'))
      sPath = sItem + sPath;
    else
    {
      if (sPath.GetLength())
        sPath = sItem + _T('\\') + sPath;
      else
        sPath = sItem;
    }
  }

  //Add the root folder if there is one
//  if (m_sRootFolder.GetLength())
//    sPath = m_sRootFolder + _T('\\') + sPath;

  return sPath;
}
int CFileAndFolder::CompareByFilenameNoCase(CString& element1, CString& element2) 
{
  return element1.CompareNoCase(element2);
}
void CFileAndFolder::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
  if (pNMTreeView->action == TVE_EXPAND)
  {
    //Add the new items to the tree if it does not have any child items
    //already
    if (!m_Tree.GetChildItem(pNMTreeView->itemNew.hItem))
    {
      CString sPath = ItemToPath(pNMTreeView->itemNew.hItem);
	  if (sPath.GetLength())
		DisplayPath(sPath, pNMTreeView->itemNew.hItem);
    }
  }

  *pResult = 0;
}

void CFileAndFolder::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	resizeControls();	
}

void CFileAndFolder::resizeControls() {
	if (!IsWindow(m_hWnd)) return;

	CRect crect,trect,okrect,cancelrect,msgrect,refresh;
	GetClientRect(crect);
	if (!IsWindow(m_Tree.m_hWnd)) return;
	m_Tree.GetClientRect(trect);
	CWnd * wok = GetDlgItem(IDOK);
	CWnd * cok = GetDlgItem(IDCANCEL);

	m_Msg.GetClientRect(msgrect);
	CWnd * wnd = GetDlgItem(IDC_REFRESH_DRIVES);
	wnd->GetClientRect(refresh);

	wok->GetClientRect(okrect);
	int okh = okrect.Height();
	int okw = okrect.Width();



	trect = crect;
	trect.top = trect.top + (msgrect.Height() + 10 + refresh.Height() + 10);
	trect.bottom -= (okh + 10);
	m_Tree.MoveWindow(trect,TRUE);

	int okx = ((crect.Width() - (okw * 3)) / 2);

	okrect.left = okx;
	okrect.right = okrect.left + okw;
	okrect.top = trect.bottom  + 5;
	okrect.bottom = okrect.top + okh;
	wok->MoveWindow(okrect,TRUE);

	cancelrect = okrect;
	cancelrect.left = okrect.left + (okrect.Width() * 2);
	cancelrect.right = cancelrect.left + okrect.Width();
	cok->MoveWindow(cancelrect,TRUE);
}


#define MBCFAF_MIN_WIDTH  300
#define MBCFAF_MIN_HEIGHT  300

void CFileAndFolder::OnSizing(UINT fwSide, LPRECT pRect) {

	int left,right,top,bottom;
	left = pRect->left;
	right = pRect->right;
	top = pRect->top;
	bottom = pRect->bottom;

	if ((fwSide == 7 || fwSide == 1 || fwSide == 4)
		&& (left > right - MBCFAF_MIN_WIDTH))
			left = right - MBCFAF_MIN_WIDTH;

	if ((fwSide == 4 || fwSide == 3 || fwSide == 5)
		&& (top > bottom - MBCFAF_MIN_HEIGHT))
			top = bottom - MBCFAF_MIN_HEIGHT;

	if ((fwSide == 5 || fwSide == 2 || fwSide == 8)
		&& (right < left + MBCFAF_MIN_WIDTH))
		right = left + MBCFAF_MIN_WIDTH;
	
	if ((fwSide == 7 || fwSide == 6 || fwSide == 8)
		&& (bottom < top + MBCFAF_MIN_HEIGHT))
		bottom = top + MBCFAF_MIN_HEIGHT;

	pRect->left = left;
	pRect->right = right;
	pRect->top = top;
	pRect->bottom = bottom;
	CDialog::OnSizing(fwSide, pRect);
	UpdateGripPos();
}


// from CResizableGrip;

void CFileAndFolder::UpdateGripPos()
{
	// size-grip goes bottom right in the client area
	// (any right-to-left adjustment should go here)

	RECT rect;
	GetClientRect(&rect);

	rect.left = rect.right - m_sizeGrip.cx;
	rect.top = rect.bottom - m_sizeGrip.cy;

	// must stay below other children
	m_wndGrip.SetWindowPos(&CWnd::wndBottom, rect.left, rect.top, 0, 0,
		SWP_NOSIZE | SWP_NOACTIVATE | SWP_NOREPOSITION);

	// maximized windows cannot be resized
	if (IsZoomed())
		m_wndGrip.EnableWindow(FALSE);
	else
		m_wndGrip.EnableWindow(TRUE);
}

void CFileAndFolder::ShowSizeGrip(BOOL bShow)
{
	m_wndGrip.ShowWindow(bShow ? SW_SHOW : SW_HIDE);
}

#define RSZ_GRIP_OBJ	_T("ResizableGrip")

BOOL CFileAndFolder::InitGrip()
{
	CRect rect(0 , 0, m_sizeGrip.cx, m_sizeGrip.cy);

	BOOL bRet = m_wndGrip.Create(WS_CHILD | WS_CLIPSIBLINGS | SBS_SIZEGRIP,
		rect, this, 0);

	if (bRet)
	{
		// set a triangular window region
		CRgn rgnGrip, rgn;
		rgn.CreateRectRgn(0,0,1,1);
		rgnGrip.CreateRectRgnIndirect(&rect);
	
		for (int y=0; y<m_sizeGrip.cy; y++)
		{
			rgn.SetRectRgn(0, y, m_sizeGrip.cx-y, y+1);
			rgnGrip.CombineRgn(&rgnGrip, &rgn, RGN_DIFF);
		}
		m_wndGrip.SetWindowRgn((HRGN)rgnGrip.Detach(), FALSE);

		// subclass control
		::SetProp(m_wndGrip, RSZ_GRIP_OBJ,
			(HANDLE)::GetWindowLong(m_wndGrip, GWL_WNDPROC));
		::SetWindowLong(m_wndGrip, GWL_WNDPROC, (LONG)GripWindowProc);

		// update pos
		UpdateGripPos();
		ShowSizeGrip();
	}

	return bRet;
}

LRESULT CFileAndFolder::GripWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	WNDPROC oldWndProc = (WNDPROC)::GetProp(hwnd, RSZ_GRIP_OBJ);

	switch (msg)
	{
	case WM_NCHITTEST:

		// choose proper cursor shape
		if (IsRTL(hwnd))
			return HTBOTTOMLEFT;
		else
			return HTBOTTOMRIGHT;

	case WM_DESTROY:
		
		// unsubclass
		::RemoveProp(hwnd, RSZ_GRIP_OBJ);
		::SetWindowLong(hwnd, GWL_WNDPROC, (LONG)oldWndProc);

		break;
	}

	return ::CallWindowProc(oldWndProc, hwnd, msg, wParam, lParam);
}

void CFileAndFolder::setMsg(const CString & msg) {
	m_msg = msg;
}
void CFileAndFolder::setTitle(const CString & msg) {
	m_title = msg;
}

void CFileAndFolder::OnRefreshDrives() 
{
	OnViewRefresh();
	
}
