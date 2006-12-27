// ConfigFiles.cpp : implementation file
//

#include "stdafx.h"
#include "FolderDialog.h"
#include "Registry.h"
#include "MyLog.h"
#include "MBMessageBox.h"
#include "ConfigFiles.h"
#include "MyString.h"
#include "FileAndFolder.h"
#include "FileUtils.h"

//#define _WIN32_DCOM
#include "GetTextField.h"
#include <objbase.h>
#include <winbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CConfigFiles property page

IMPLEMENT_DYNCREATE(CConfigFiles, CPropertyPage)

CConfigFiles::CConfigFiles(CWnd *p, PlayerCallbacks * pcb) : CPropertyPage(CConfigFiles::IDD),
    /*m_PlayerDlg(p),*/ m_RunAtStartupUL(0),
	m_AlbumSortAlpha(TRUE), m_AlbumSortDate(FALSE),
	m_playercallbacks(pcb), m_ResetNeeded(FALSE),
	m_OrigRunAtStartup(FALSE),m_HideGenre(FALSE)

{
	//{{AFX_DATA_INIT(CConfigFiles)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    setDefaults();
    init();
}
void CConfigFiles::setDefaults() {

}

CConfigFiles::~CConfigFiles()
{
}

void CConfigFiles::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConfigFiles)

	DDX_Control(pDX, IDC_RUNATSTARTUP,	m_RunAtStartup);
	DDX_Control(pDX, IDC_MDB_LOCATION,	m_MdbLocation);
	DDX_Control(pDX, IDC_DIRLIST,		m_MP3DirList);
	DDX_Control(pDX, IDC_DIRLIST2,		m_ExcludeList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigFiles, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigFiles)
	ON_BN_CLICKED(IDC_DIRADD, OnDiradd)
	ON_BN_CLICKED(IDC_DIRADD2, OnDiradd2)
	ON_LBN_SELCHANGE(IDC_DIRLIST, OnSelchangeDirlist)
	ON_LBN_SELCHANGE(IDC_DIRLIST2, OnSelchangeDirlist2)
	ON_BN_CLICKED(IDC_DIRREMOVE, OnDirremove)
	ON_BN_CLICKED(IDC_DIRREMOVE2, OnDirremove2)
	ON_BN_CLICKED(IDC_LOCATION_BUTTON, OnLocationButton)
	ON_BN_CLICKED(IDC_ALBUMSORT_DATE, OnAlbumsortDate)
	ON_BN_CLICKED(IDC_ALBUMSORT_ALPHA, OnAlbumsortAlpha)
	ON_BN_CLICKED(IDC_RUNATSTARTUP, OnRunatstartup)
	ON_BN_CLICKED(IDC_HIDE_GENRE, OnHideGenre)
	ON_BN_CLICKED(IDC_DIRCLEAR, OnDirclear)
	ON_BN_CLICKED(IDC_EXCLUDECLEAR, OnExcludeclear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigFiles message handlers
void CConfigFiles::init() {

    ReadReg();
}






void CConfigFiles::OnSelchangeDirlist() 
{
    CWnd * button = GetDlgItem(IDC_DIRREMOVE);
    button->EnableWindow(TRUE);
	SetModified(TRUE);
}
void CConfigFiles::OnSelchangeDirlist2() 
{
    CWnd * button = GetDlgItem(IDC_DIRREMOVE2);
    button->EnableWindow(TRUE);
	SetModified(TRUE);
}

void CConfigFiles::OnDirremove() 
{
    int cursel = m_MP3DirList.GetCurSel();
    m_MP3DirList.DeleteString(cursel);
	m_MP3DirList.SetCurSel(
		cursel < m_MP3DirList.GetCount() ? cursel : cursel -1);

    EnableDisableButtons(); 
	SetModified(TRUE);

	m_slMP3DirList.RemoveAll();
	CString tmp;
	for(cursel = 0; cursel < m_MP3DirList.GetCount(); cursel++) {
		m_MP3DirList.GetText(cursel,tmp);
		m_slMP3DirList.AddTail(tmp);
	}

}
void CConfigFiles::OnDirremove2() 
{
    int cursel = m_ExcludeList.GetCurSel();
    m_ExcludeList.DeleteString(cursel);
	m_ExcludeList.SetCurSel(
		cursel < m_ExcludeList.GetCount() ? cursel : cursel -1);

    EnableDisableButtons(); 
	SetModified(TRUE);

	m_slExcludeList.RemoveAll();
	CString tmp;
	for(cursel = 0; cursel < m_ExcludeList.GetCount(); cursel++) {
		m_ExcludeList.GetText(cursel,tmp);
		m_slExcludeList.AddTail(tmp);
	}
}

void
CConfigFiles::GetDirs(CStringList & dirs, CStringArray & excludes,const CString & dir) {
	POSITION pos;
	CString tmp;
	CString save_m_path;
	dirs.RemoveAll();
	excludes.RemoveAll();

	if (dir.GetLength()) {
		dirs.RemoveAll();
		save_m_path = m_path;
		m_path = dir;
		ReadFolders();
	}
	for (pos = m_slMP3DirList.GetHeadPosition(); pos != NULL; ) {
        tmp = m_slMP3DirList.GetAt(pos);
		dirs.AddTail(tmp);
		m_slMP3DirList.GetNext(pos);
    }
	for (pos = m_slExcludeList.GetHeadPosition(); pos != NULL; ) {
        tmp = m_slExcludeList.GetAt(pos);
		excludes.Add(tmp);
		m_slExcludeList.GetNext(pos);
    }
	if (dir.GetLength()) {
		m_path = save_m_path;
		ReadFolders();
	}
	String::Sort(dirs);
	String::Sort(excludes);
}



void CConfigFiles::OnDiradd() 
{

    // open a file
	int s = m_MP3DirList.GetCurSel();
	CString dflt;
	if (s <  0)
		s = 0;
	if (s = m_MP3DirList.GetCount()) {
		m_MP3DirList.GetText(s-1, dflt);
		dflt = FileUtil::ParentDir(String::upDir(dflt));
	}

	CFileAndFolder dialog(this, dflt);
	dialog.SetShowFiles(FALSE);
	dialog.setTitle("Add folders");
	dialog.setMsg("Select folder(s) to be searched.");

	int ret;
	ret = dialog.DoModal();
	if (ret == IDOK) {
        // a file was selected
		CStringList list;
		dialog.GetPaths(list);
		POSITION pos;
		for(pos = list.GetHeadPosition(); pos != NULL;) {
			CString path = list.GetNext(pos);
			if (path.GetLength())
//				m_MP3DirList.AddString(path);
				m_slMP3DirList.AddTail(path);
		}
		FileUtil::SortEliminateDupsAndSubDirs(m_slMP3DirList);
		list2box(m_slMP3DirList,m_MP3DirList);

        EnableDisableButtons(); 
        UpdateData(FALSE);
		SetModified(TRUE);
	}
}
void CConfigFiles::OnDiradd2() 
{

    // open a file
	int s = m_ExcludeList.GetCurSel();
	CString dflt;
	if (s <  0)
		s = 0;
	if (s = m_ExcludeList.GetCount()) {
		m_ExcludeList.GetText(s-1, dflt);
		dflt = FileUtil::ParentDir(String::upDir(dflt));
	}

	CFileAndFolder dialog(this, dflt);
	dialog.SetShowFiles(FALSE);
	dialog.setTitle("Add folders");
	dialog.setMsg("Select files/folders to be excluded.");

	int ret;
	ret = dialog.DoModal();
	if (ret == IDOK) {
        // a file was selected
		CStringList list;
		dialog.GetPaths(list);
		POSITION pos;
		for(pos = list.GetHeadPosition(); pos != NULL;) {
			CString path = list.GetNext(pos);
			if (path.GetLength())
//				m_MP3DirList.AddString(path);
				m_slExcludeList.AddTail(path);
		}
		FileUtil::SortEliminateDupsAndSubDirs(m_slExcludeList);
		list2box(m_slExcludeList,m_ExcludeList);

        EnableDisableButtons(); 
        UpdateData(FALSE);
		SetModified(TRUE);
	}
}
void CConfigFiles::list2box(const CStringList & list, CListBox & box) {
	if (::IsWindow(box.m_hWnd)) {
		box.ResetContent();
		POSITION pos;
		for(pos = list.GetHeadPosition(); pos != NULL; list.GetNext(pos)) {
			box.AddString(list.GetAt(pos));
		}
	}
}
void CConfigFiles::OnLocationButton() 
{
    // open a file
	CString dflt;
	m_MdbLocation.GetWindowText(dflt);
	CFileAndFolder dialog(this, FileUtil::ParentDir(dflt));
	dialog.SetShowFiles(FALSE);
	dialog.setTitle("Set location of Muzikbrowzer files");
	dialog.setMsg("... where Muzikbrowzer's files, playlists, skins reside");

	int ret;
	ret = dialog.DoModal();
	if (ret == IDOK) {
        // a file was selected
		CStringList paths ;
		dialog.GetPaths(paths );
        POSITION pos;
		for(pos = paths.GetHeadPosition(); pos != NULL;) {
			CString path = paths.GetNext(pos);
			if (path.GetLength()) {
				m_MdbLocation.SetWindowText(path);
				EnableDisableButtons(); 
				UpdateData(FALSE);

				(*m_playercallbacks->setDbLocation)(path);
				m_path = path;
				SetModified(TRUE);
				m_LocDirModified = TRUE;
//				return;
			}
		}
		ReadFolders();
		UpdateData(FALSE);
	}	
}


void CConfigFiles::ReadReg() {
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );

    CString Location;
    AutoBuf buf(1000);
    reg.Read(RegDbLocation, buf.p, 999, "");
    Location = buf.p;

	m_HideGenre = (reg.Read("HideGenre",0) && 1);
	m_InitialHideGenre = m_HideGenre;

    if (Location.GetLength()) {
        m_origMdbLocation = Location;
		m_path = Location;
    } else {
        TCHAR szBuff[_MAX_PATH];
        ::GetModuleFileName(NULL, szBuff, _MAX_PATH);
        int n = strlen(szBuff);
        int i;
        for (i = n-1 ; i > 0 ; --i) {
            if (szBuff[i] == '\\') {
                szBuff[i] = 0;
                i = 0;
            }
        }
        m_origMdbLocation = szBuff;
        reg.Write(RegDbLocation, szBuff);

		(*m_playercallbacks->setDbLocation)(m_origMdbLocation);
        m_path = m_origMdbLocation;
    }

	ReadFolders();

    m_RunAtStartupUL = reg.Read(RegRunAtStartup, 0);
	unsigned long x = reg.Read(RegAlbumSort, 0);
	if (x == 0) {
		m_AlbumSortAlpha = 0;
		m_AlbumSortDate = 1;

	} else {
		m_AlbumSortAlpha = 1;
		m_AlbumSortDate = 0;
	}



}
void CConfigFiles::ReadFolders() {
	m_origMP3DirList.RemoveAll();
	m_slMP3DirList.RemoveAll();
	m_origExcludeList.RemoveAll();
	m_slExcludeList.RemoveAll();
	if (::IsWindow(m_MP3DirList.m_hWnd))
		m_MP3DirList.ResetContent();
	if (::IsWindow(m_ExcludeList.m_hWnd))
		m_ExcludeList.ResetContent();

	RegistryKey dreg(m_path + "\\Muzikbrowzer.folders");
	dreg.ReadFile();
	
	unsigned long numdirs;
    numdirs = dreg.Read(RegNumDirs, 0);
    AutoBuf buf(1000);
    unsigned long i;
    for (i = 0 ; i < numdirs ; ++i) {
        sprintf(buf.p, "%s%02d", RegDirKey, i);
        CString dir;
        //dreg.Read(buf.p, dir, 999, "");
		dir = dreg.ReadCString(buf.p,"");
		if (dir.GetLength()) {
			m_origMP3DirList.AddTail(dir);
			m_slMP3DirList.AddTail(dir);
			if (::IsWindow(m_MP3DirList.m_hWnd))
				m_MP3DirList.AddString(dir);
		}
    }
    numdirs = dreg.Read(RegNumExcludes, 0);
    for (i = 0 ; i < numdirs ; ++i) {
        sprintf(buf.p, "%s%02d", RegExcludePath, i);
        CString dir;
        //dreg.Read(buf.p, dir, 999, "");
		dir = dreg.ReadCString(buf.p,"");
		if (dir.GetLength()) {
			m_origExcludeList.AddTail(dir);
			m_slExcludeList.AddTail(dir);
			if (::IsWindow(m_ExcludeList.m_hWnd))
				m_ExcludeList.AddString(dir);
		}
    }

	// For upgrade to 2.0.2; if no dirs on disk check in Registry
	// and put 'em on disk then remove from Registry
	if (0 == numdirs) {
		RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
		numdirs = reg.Read(RegNumDirs,0);
		reg.DeleteValue(RegNumDirs);
		if (numdirs) {
			unsigned long i;
			for (i = 0 ; i < numdirs ; ++i) {
				sprintf(buf.p, "%s_%02d", RegDirKey, i); // '_' intentional
				CString dir;
				dir = reg.ReadCString(buf.p,"");

				if (dir.GetLength()) {
					m_origMP3DirList.AddTail(dir);
					m_slMP3DirList.AddTail(dir);
					if (::IsWindow(m_MP3DirList.m_hWnd))
						m_MP3DirList.AddString(dir);
				}
			}
			WriteFolders();
		}
		// Just in case there are some there clean 'em up.
		for (i = 0 ; i < 100 ; ++i) {
			sprintf(buf.p, "%s_%02d", RegDirKey, i); // '_' intentional
			reg.DeleteValue(buf.p);
		}
	}
	FileUtil::SortEliminateDupsAndSubDirs(m_slMP3DirList);
	list2box(m_slMP3DirList,m_MP3DirList);
	FileUtil::SortEliminateDupsAndSubDirs(m_slExcludeList);
	list2box(m_slExcludeList,m_ExcludeList);
}

void CConfigFiles::AddFolders(const CStringList & dirs) {
	POSITION pos;
	for(pos = dirs.GetHeadPosition(); pos != NULL;) {
		m_slMP3DirList.AddTail(dirs.GetAt(pos));
		dirs.GetNext(pos);
	}
	FileUtil::SortEliminateDupsAndSubDirs(m_slMP3DirList);
	WriteFolders();
}
void CConfigFiles::AddDeletedFiles(const CStringList & dirs) {
	POSITION pos;
	for(pos = dirs.GetHeadPosition(); pos != NULL;) {
		m_slExcludeList.AddTail(dirs.GetAt(pos));
		dirs.GetNext(pos);
	}
	FileUtil::SortEliminateDupsAndSubDirs(m_slExcludeList);
	WriteFolders();
}
// This is for when/if the user begins to Add but then cancels it
void CConfigFiles::DelFolders(const CStringList & dirs) {
	CStringList list;
	String::copyCStringList(list, m_slMP3DirList);
	m_slMP3DirList.RemoveAll();

	POSITION pos;
	for(pos = list.GetHeadPosition(); pos != NULL;) {
		CString tmp = list.GetAt(pos);
		if (!String::CStringListContains(dirs,tmp))
			m_slMP3DirList.AddTail(tmp);
		list.GetNext(pos);
	}
	WriteFolders();
}
void CConfigFiles::WriteFolders() {
    AutoBuf buf(1000);
    unsigned long num = m_slMP3DirList.GetCount();
//    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	RegistryKey reg(m_path + "\\Muzikbrowzer.folders");
    reg.Write(RegNumDirs, num);

	num = m_slExcludeList.GetCount();
	reg.Write(RegNumExcludes, num);

    m_origMP3DirList.RemoveAll();
	m_origExcludeList.RemoveAll();

    unsigned long i=0;
	POSITION pos;
	CString dir;
	for (pos = m_slMP3DirList.GetHeadPosition(); pos != NULL; ) {
        dir = m_slMP3DirList.GetAt(pos);
		m_origMP3DirList.AddTail(dir);
		m_slMP3DirList.GetNext(pos);
		sprintf(buf.p, "%s%02d", RegDirKey, i);
		reg.Write(buf.p, dir);
		i++;
    }
	i=0;
	for (pos = m_slExcludeList.GetHeadPosition(); pos != NULL; ) {
        dir = m_slExcludeList.GetAt(pos);
		m_origExcludeList.AddTail(dir);
		m_slExcludeList.GetNext(pos);
		sprintf(buf.p, "%s%02d", RegExcludePath, i);
		reg.Write(buf.p, dir);
		i++;
    }
	reg.WriteFile();
}

void CConfigFiles::StoreReg() {
    AutoBuf buf(1000);

    CString Location;
    m_MdbLocation.GetWindowText(Location);
    m_origMdbLocation = Location;

    const TCHAR * location = (LPCTSTR) Location;

	WriteFolders();
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
    reg.Write(RegDbLocation, location);
	m_origMdbLocation = location;

	m_HideGenre = ((CButton*)GetDlgItem(IDC_HIDE_GENRE))->GetCheck();
	reg.Write("HideGenre",m_HideGenre);
	m_InitialHideGenre = m_HideGenre;

    if (m_RunAtStartup.GetCheck() == 0) {
        m_RunAtStartupUL = 0;
        reg.Write(RegRunAtStartup, (unsigned long) 0);
    } else {
        m_RunAtStartupUL = 1;
        reg.Write(RegRunAtStartup, (unsigned long) 1);
    }
	m_OrigRunAtStartup = (BOOL)m_RunAtStartupUL;

    setRunAtStartup();
	reg.Write(RegAlbumSort, (unsigned long)m_AlbumSortAlpha);
	m_InitialAlbumSortAlpha = m_AlbumSortAlpha;


}
void
CConfigFiles::setRunAtStartup() {
    // use this one CSIDL_STARTUP

	IMalloc *m;
	SHGetMalloc(&m);
	if (!m) return;

    LPITEMIDLIST itemList;
    char path[MAX_PATH];
    if (SHGetSpecialFolderLocation(NULL, CSIDL_STARTUP, &itemList) !=
        NOERROR) {
        MBMessageBox("", "Cannot get special folder list");
        return;
    }
    if (!SHGetPathFromIDList(itemList,path)) {
		m->Free(itemList);
        MBMessageBox("","Cannot get special folder path");
        return;
    }
    CString shortCutFile = path;
    shortCutFile += "\\";
    shortCutFile += MUZIKBROWZER;
    shortCutFile += ".lnk";

    if (m_RunAtStartupUL) {
        char shortCutTarget[MAX_PATH+1];
        DWORD r = GetModuleFileName(NULL, shortCutTarget, MAX_PATH);
        if (r == 0) {
			m->Free(itemList);
            MBMessageBox("","GetModuleFileName failed!");
            return;
        }
        CString workingDir = shortCutTarget;
        CString mbdotexe = MUZIKBROWZER;
        mbdotexe += ".exe";
        workingDir = workingDir.Left(workingDir.GetLength()
            - (mbdotexe.GetLength() + 1));


        HRESULT hres;
        IShellLink *psl;

        // Create an IShellLink object and get a pointer to the IShellLink 
        // interface (returned from CoCreateInstance).
        CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

        hres = CoCreateInstance (CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER,
        IID_IShellLink, (void **)&psl);
        if (SUCCEEDED (hres)) {
            IPersistFile *ppf;

            // Query IShellLink for the IPersistFile interface for 
            // saving the shortcut in persistent storage.
            hres = psl->QueryInterface (IID_IPersistFile, (void **)&ppf);
            if (SUCCEEDED (hres)) { 
                WORD wsz [MAX_PATH]; // buffer for Unicode string

                // Set the path to the shortcut target.
                hres = psl->SetPath (shortCutTarget);
                if (! SUCCEEDED (hres))
                    MBMessageBox ("","SetPath failed!");

                // Set the description of the shortcut.
                hres = psl->SetDescription (MUZIKBROWZER);
                if (! SUCCEEDED (hres))
                    MBMessageBox ("","SetDescription failed!");

                hres = psl->SetWorkingDirectory(workingDir);
                if (! SUCCEEDED (hres))
                    MBMessageBox ("","SetWorkingDirectory failed!");

                // Ensure that the string consists of ANSI characters.
                MultiByteToWideChar (CP_ACP, 0, shortCutFile, -1, wsz, MAX_PATH);

                // Save the shortcut via the IPersistFile::Save member function.
                hres = ppf->Save (wsz, TRUE);
                if (! SUCCEEDED (hres))
                    MBMessageBox ("","Save failed!");

                // Release the pointer to IPersistFile.
                ppf->Release ();
            }
            // Release the pointer to IShellLink.
            psl->Release ();
            CoUninitialize();
        }
    } else {

//where
//pszShortcutFile is the file that the shortcut will point to. 
//pszLink is the shortcut you are creating with a LNK extension. 
//pszDesc is the description of the file. This is the string "Shortcut to
//filename", where filename is the name of the shortcut target. 
//You have to call CoInitialize before calling this.
//Check out the Article I mentioned earlier for more details. Its
//available on MSDN. 

        if (!DeleteFile (shortCutFile)) {
            CString msg = "Cannot delete shortcut";
            msg += "\r\n";
            msg += shortCutFile;
            //MBMessageBox ("",msg);
        }
    }
	m->Free(itemList);
}


BOOL CConfigFiles::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	POSITION pos;
    m_MP3DirList.ResetContent();
	m_ExcludeList.ResetContent();
    for (pos = m_origMP3DirList.GetHeadPosition(); pos != NULL; ) {
        CString extension = m_origMP3DirList.GetAt(pos);
        m_MP3DirList.AddString(extension);
	    m_origMP3DirList.GetNext(pos);
    }
    for (pos = m_origExcludeList.GetHeadPosition(); pos != NULL; ) {
        CString extension = m_origExcludeList.GetAt(pos);
        m_ExcludeList.AddString(extension);
	    m_origExcludeList.GetNext(pos);
    }

    m_MdbLocation.SetWindowText(m_origMdbLocation);


    if (m_RunAtStartupUL) {
        m_RunAtStartup.SetCheck(1);
		m_OrigRunAtStartup = TRUE;
    } else {
        m_RunAtStartup.SetCheck(0);
		m_OrigRunAtStartup = FALSE;
    }

	if (m_AlbumSortAlpha) {
		CheckRadioButton(IDC_ALBUMSORT_DATE,IDC_ALBUMSORT_ALPHA,
			IDC_ALBUMSORT_ALPHA);
		m_InitialAlbumSortAlpha = TRUE;
	} else {
		CheckRadioButton(IDC_ALBUMSORT_DATE,IDC_ALBUMSORT_ALPHA,
			IDC_ALBUMSORT_DATE);
		m_InitialAlbumSortAlpha = FALSE;
	}

	((CButton*)GetDlgItem(IDC_HIDE_GENRE))->SetCheck(m_HideGenre);

    UpdateWindow();

    UpdateData(FALSE);

    EnableDisableButtons(); 
	m_LocDirModified = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigFiles::EnableDisableButtons() {
}

void CConfigFiles::OnAlbumsortDate() 
{
	m_AlbumSortAlpha = FALSE;
	m_AlbumSortDate = TRUE;
	CheckRadioButton(IDC_ALBUMSORT_DATE,IDC_ALBUMSORT_ALPHA,IDC_ALBUMSORT_DATE);
	UpdateData(FALSE);
	SetModified(TRUE);
}

void CConfigFiles::OnAlbumsortAlpha() 
{
	m_AlbumSortAlpha = TRUE;
	m_AlbumSortDate = FALSE;
	CheckRadioButton(IDC_ALBUMSORT_DATE,IDC_ALBUMSORT_ALPHA,IDC_ALBUMSORT_ALPHA);
	UpdateData(FALSE);
	SetModified(TRUE);
	
}
BOOL CConfigFiles::AlbumSortAlpha()
{
	return m_AlbumSortAlpha;
}


BOOL CConfigFiles::OnApply() 
{
	StoreReg();
	SetModified(FALSE);
	return CPropertyPage::OnApply();
}

void CConfigFiles::OnOK() 
{
	CPropertyPage::OnOK();

    StoreReg();
	SetModified(FALSE);
	if (m_LocDirModified) {
		(*m_playercallbacks->initDb)();
	}
	if (m_AlbumSortAlpha != m_InitialAlbumSortAlpha) {
		m_ResetNeeded = TRUE;
		//MBMessageBox("Notice","Changes to the \"Album sort\" parameter will take effect\r\nthe next time you start Muzikbrowzer. Alternatively hit \"F5\".",FALSE);
	}
}
void CConfigFiles::OnCancel() 
{
//	m_MP3DirList.ResetContent();
//	m_slMP3DirList.RemoveAll();

//	POSITION pos;
//	for (pos = m_origMP3DirList.GetHeadPosition(); pos != NULL; ) {
//		CString dir = m_origMP3DirList.GetAt(pos);
//		m_MP3DirList.AddString(dir);
//		m_slMP3DirList.AddTail(dir);
//		m_origMP3DirList.GetNext(pos);
//	}
	m_MdbLocation.SetWindowText(m_origMdbLocation);
	m_path = m_origMdbLocation;
	ReadFolders();

	if (m_InitialAlbumSortAlpha) {
		CheckRadioButton(IDC_ALBUMSORT_DATE,IDC_ALBUMSORT_ALPHA,
			IDC_ALBUMSORT_ALPHA);
		m_AlbumSortDate = FALSE;
		m_AlbumSortAlpha = TRUE;
	} else {
		CheckRadioButton(IDC_ALBUMSORT_DATE,IDC_ALBUMSORT_ALPHA,
			IDC_ALBUMSORT_DATE);
		m_AlbumSortDate = TRUE;
		m_AlbumSortAlpha = FALSE;
	}


    if (m_OrigRunAtStartup) {
        m_RunAtStartup.SetCheck(1);
    } else {
        m_RunAtStartup.SetCheck(0);
    }

	((CButton*)GetDlgItem(IDC_HIDE_GENRE))->SetCheck(m_InitialHideGenre);
	m_HideGenre = m_InitialHideGenre;

	StoreReg();
//    UpdateData(FALSE);
	CPropertyPage::OnCancel();
}

void CConfigFiles::OnRunatstartup() 
{
	UpdateData(FALSE);
	SetModified(TRUE);
	
}

void CConfigFiles::OnHideGenre() 
{
	UpdateData(FALSE);
	SetModified(TRUE);
	
}

void CConfigFiles::OnDirclear() 
{
	m_MP3DirList.ResetContent();
	m_slMP3DirList.RemoveAll();
	UpdateData(FALSE);
	SetModified(TRUE);
}

void CConfigFiles::OnExcludeclear() 
{
	m_ExcludeList.ResetContent();
	m_slExcludeList.RemoveAll();
	UpdateData(FALSE);
	SetModified(TRUE);
}
