// ConfigFiles.cpp : implementation file
//

#include "stdafx.h"
#include "FolderDialog.h"
#include "Registry.h"
#include "MyLog.h"
#include "MBMessageBox.h"
#include "ConfigFiles.h"
#include "MyString.h"
//#define _WIN32_DCOM
#include <objbase.h>
#include <winbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static unsigned long useGenreUL;

/////////////////////////////////////////////////////////////////////////////
// CConfigFiles property page

IMPLEMENT_DYNCREATE(CConfigFiles, CPropertyPage)

CConfigFiles::CConfigFiles(CWnd *p, PlayerCallbacks * pcb) : CPropertyPage(CConfigFiles::IDD),
    /*m_PlayerDlg(p),*/ m_RunAtStartupUL(0), m_scanNew(FALSE), m_UseGenreUL(0),
	m_AlbumSortAlpha(TRUE), m_AlbumSortDate(FALSE),
	m_playercallbacks(pcb)

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
	DDX_Control(pDX, IDC_RUNATSTARTUP, m_RunAtStartup);
	DDX_Control(pDX, IDC_MP3_EXTENSION, m_Mp3Extensions);
	DDX_Control(pDX, IDC_MP3_ADD_EXT, m_Mp3Extension);
	DDX_Control(pDX, IDC_MDB_LOCATION, m_MdbLocation);
	DDX_Control(pDX, IDC_DIRLIST, m_MP3DirList);
	DDX_Control(pDX, IDC_USEGENRE, m_UseGenre);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConfigFiles, CPropertyPage)
	//{{AFX_MSG_MAP(CConfigFiles)
	ON_BN_CLICKED(IDC_DIRADD, OnDiradd)
	ON_LBN_SELCHANGE(IDC_DIRLIST, OnSelchangeDirlist)
	ON_BN_CLICKED(IDC_DIRREMOVE, OnDirremove)
	ON_BN_CLICKED(IDC_DIRSCAN, OnDirscan)
	ON_BN_CLICKED(IDC_LOCATION_BUTTON, OnLocationButton)
	ON_BN_CLICKED(IDC_MP3_ADD, OnMp3Add)
	ON_BN_CLICKED(IDC_MP3_REMOVE, OnMp3Remove)
	ON_BN_CLICKED(IDC_DIRSCAN_NEW, OnDirscanNew)
	ON_BN_CLICKED(IDC_ALBUMSORT_DATE, OnAlbumsortDate)
	ON_BN_CLICKED(IDC_ALBUMSORT_ALPHA, OnAlbumsortAlpha)
	ON_BN_CLICKED(IDC_USEGENRE, OnUsegenre)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConfigFiles message handlers
void CConfigFiles::init() {

    ReadReg();
}



void CConfigFiles::OnDiradd() 
{

    // open a file
	int s = m_MP3DirList.GetCurSel();
	CString dflt;
	if (s <  0)
		s = 0;
	if (m_MP3DirList.GetCount())
		m_MP3DirList.GetText(s, dflt);

	CFolderDialog dialog(dflt);

	int ret;
	ret = dialog.DoModal();
	if (ret == IDOK) {
        // a file was selected
		CString path = dialog.GetPathName();
        m_MP3DirList.AddString(path);
        EnableDisableButtons(); 
        UpdateData(FALSE);
		SetModified(TRUE);
	}
}


void CConfigFiles::OnSelchangeDirlist() 
{
    CWnd * button = GetDlgItem(IDC_DIRREMOVE);
    button->EnableWindow(TRUE);
	SetModified(TRUE);
}

void CConfigFiles::OnDirremove() 
{
    int cursel = m_MP3DirList.GetCurSel();
    m_MP3DirList.DeleteString(cursel);

    EnableDisableButtons(); 
	SetModified(TRUE);

}

class ThreadParams {
public:

	CString (*scanDirectories)(const CStringList & directories,
						  InitDlg * initDlg, BOOL scanNew);
    CStringList * m_dirs;
    InitDlg * m_InitDialog;
    CString m_result;
	BOOL m_scanNew;
};

static int ScanThread_done = 0;

static UINT
ScanThread(LPVOID pParam) {
    ThreadParams *tp = (ThreadParams *)pParam;

	ScanThread_done = 0;
	tp->m_result = (*tp->scanDirectories)(*(tp->m_dirs), tp->m_InitDialog, tp->m_scanNew);
    tp->m_InitDialog->SendUpdateStatus(4, "", 0,0); // EndDialog;

    ScanThread_done = 1;
    return 0;
}

void
CConfigFiles::ScanThreadStart(ThreadParams &tp) {

    AfxBeginThread(ScanThread, (LPVOID) &tp);

    return;
}

void
CConfigFiles::ScanThreadStop() {
}

void CConfigFiles::OnDirscan() 
{

	m_scanNew = FALSE;
	dirScan();
}
void CConfigFiles::OnDirscanNew() 
{
	m_scanNew = TRUE;
	dirScan();	
}

void CConfigFiles::dirScan() {

    CStringList dirs;
    CString dir;
    int n = m_MP3DirList.GetCount();
	if (n < 1) {
		MBMessageBox("Scan", "You must enter at least one directory");
		return;
	}
    int i;
    for(i = 0 ; i < n ; ++i) {
        m_MP3DirList.GetText(i, dir);
        dirs.AddTail(dir);
    }

	InitDlg *m_InitDialog = new InitDlg(0,1);

    ThreadParams tp;
    //tp.m_mlib = &m_PlayerDlg->m_mlib;
	tp.scanDirectories = m_playercallbacks->scanDirectories;
    tp.m_dirs = &dirs;
    tp.m_InitDialog = m_InitDialog;
	tp.m_scanNew = m_scanNew;
    ScanThreadStart(tp);

	m_InitDialog->DoModal();

    CString scaninfo = tp.m_result;

	(*m_playercallbacks->initDb)();

    CString libcounts = (*m_playercallbacks->getLibraryCounts)();

	(*m_playercallbacks->statusset)(libcounts);

	(*m_playercallbacks->UpdateWindow)();

    delete m_InitDialog;

    MBMessageBox("Scan Results", scaninfo);

}

void CConfigFiles::OnLocationButton() 
{
    // open a file
	CString dflt;
	m_MdbLocation.GetWindowText(dflt);
	CFolderDialog dialog(dflt);

	int ret;
	ret = dialog.DoModal();
	if (ret == IDOK) {
        // a file was selected
		CString path = dialog.GetPathName();
        m_MdbLocation.SetWindowText(path);
        EnableDisableButtons(); 
        UpdateData(FALSE);

		(*m_playercallbacks->setDbLocation)(path);
        m_path = path;
		SetModified(TRUE);
	}	
}


void CConfigFiles::ReadReg() {
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );

    CString Location;
    m_origMP3DirList.RemoveAll();
    m_origMp3Extensions.RemoveAll();

    AutoBuf buf(1000);
    unsigned long numdirs,numExtensions;

    reg.Read(RegDbLocation, buf.p, 999, "");
    Location = buf.p;

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

    numdirs = reg.Read(RegNumDirs, 0);

    unsigned long i;
    for (i = 0 ; i < numdirs ; ++i) {
        sprintf(buf.p, "%s%02d", RegDirKey, i);
        TCHAR dir[1000];
        reg.Read(buf.p, dir, 999, "");
        m_origMP3DirList.AddTail(dir);
    }

    numExtensions = reg.Read(RegNumMp3Extensions, 0);
    if (numExtensions == 0) {
        m_origMp3Extensions.AddTail("mp3");
    } else {
        for (i = 0 ; i < numExtensions ; ++i) {
            sprintf(buf.p, "%s%02d", RegMp3ExtensionsKey, i);
            TCHAR extension[100];
            reg.Read(buf.p, extension, 9, "");
            m_origMp3Extensions.AddTail(extension);
        }
    }
    m_RunAtStartupUL = reg.Read(RegRunAtStartup, 0);
	m_UseGenreUL = reg.Read(RegUseGenre, 0);
	useGenreUL = m_UseGenreUL;
	unsigned long x = reg.Read(RegAlbumSort, 0);
	if (x == 0) {
		m_AlbumSortAlpha = 0;
		m_AlbumSortDate = 1;

	} else {
		m_AlbumSortAlpha = 1;
		m_AlbumSortDate = 0;
	}

}


void CConfigFiles::StoreReg() {
    AutoBuf buf(1000);
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );

    CString Location;
    m_MdbLocation.GetWindowText(Location);
    m_origMdbLocation = Location;

    const TCHAR * location = (LPCTSTR) Location;

    unsigned long num = m_MP3DirList.GetCount();
    reg.Write(RegDbLocation, location);
    reg.Write(RegNumDirs, num);

    m_origMP3DirList.RemoveAll();
    unsigned long i;
    for (i = 0 ; i < num ; ++i) {
        sprintf(buf.p, "%s%02d", RegDirKey, i);
        CString Dir;
        m_MP3DirList.GetText(i, Dir);
        const TCHAR * dir = (LPCTSTR)Dir;
        m_origMP3DirList.AddTail(dir);

        reg.Write(buf.p, dir);
    }

    m_origMp3Extensions.RemoveAll();
    num = m_Mp3Extensions.GetCount();
    reg.Write(RegNumMp3Extensions, num);
    for (i = 0 ; i < num ; ++i) {
        sprintf(buf.p, "%s%02d", RegMp3ExtensionsKey, i);
        CString extension;
        m_Mp3Extensions.GetText(i, extension);
        const TCHAR * exten = (LPCTSTR)extension;
        reg.Write(buf.p, exten);
        m_origMp3Extensions.AddTail(exten);
    }
    if (m_RunAtStartup.GetCheck() == 0) {
        m_RunAtStartupUL = 0;
        reg.Write(RegRunAtStartup, (unsigned long) 0);
    } else {
        m_RunAtStartupUL = 1;
        reg.Write(RegRunAtStartup, (unsigned long) 1);
    }
    if (m_UseGenre.GetCheck() == 0) {
        m_UseGenreUL = 0;
        reg.Write(RegUseGenre, (unsigned long) 0);
    } else {
        m_UseGenreUL = 1;
        reg.Write(RegUseGenre, (unsigned long) 1);
    }
    setRunAtStartup();
	reg.Write(RegAlbumSort, (unsigned long)m_AlbumSortAlpha);
//	if (useGenreUL != m_UseGenreUL) {
//		MBMessageBox("Notice", "muzikbrowzer must be restarted.\r\nClick OK then restart.", FALSE);
//		m_PlayerDlg->OnCancel();
//		exit(0);
//	}
    //m_PlayerDlg->init();

}
void
CConfigFiles::setRunAtStartup() {
    // use this one CSIDL_STARTUP

    LPITEMIDLIST itemList;
    char path[MAX_PATH];
    if (SHGetSpecialFolderLocation(NULL, CSIDL_STARTUP, &itemList) !=
        NOERROR) {
        MBMessageBox("", "Cannot get special folder list");
        return;
    }
    if (!SHGetPathFromIDList(itemList,path)) {
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
}

BOOL CConfigFiles::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
    CWnd * button = GetDlgItem(IDC_DIRREMOVE);
    button->EnableWindow(FALSE);

	m_Mp3Extensions.ResetContent();
    m_MP3DirList.ResetContent();

    POSITION pos;
    for (pos = m_origMp3Extensions.GetHeadPosition(); pos != NULL; ) {
        CString extension = m_origMp3Extensions.GetAt(pos);
        m_Mp3Extensions.AddString(extension);
	    m_origMp3Extensions.GetNext(pos);
    }
    for (pos = m_origMP3DirList.GetHeadPosition(); pos != NULL; ) {
        CString extension = m_origMP3DirList.GetAt(pos);
        m_MP3DirList.AddString(extension);
	    m_origMP3DirList.GetNext(pos);
    }
    m_MdbLocation.SetWindowText(m_origMdbLocation);


    if (m_RunAtStartupUL) {
        m_RunAtStartup.SetCheck(1);
    } else {
        m_RunAtStartup.SetCheck(0);
    }
	if (m_UseGenreUL) {
		m_UseGenre.SetCheck(1);
	} else {
		m_UseGenre.SetCheck(0);
	}

	if (m_AlbumSortAlpha) {
		CheckRadioButton(IDC_ALBUMSORT_DATE,IDC_ALBUMSORT_ALPHA,
			IDC_ALBUMSORT_ALPHA);
	} else {
		CheckRadioButton(IDC_ALBUMSORT_DATE,IDC_ALBUMSORT_ALPHA,
			IDC_ALBUMSORT_DATE);
	}

    UpdateWindow();

    UpdateData(FALSE);

    EnableDisableButtons(); 

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConfigFiles::EnableDisableButtons() {
    CString location;
    m_MdbLocation.GetWindowText(location);
    CWnd * scan = GetDlgItem(IDC_DIRSCAN);
    if (m_MP3DirList.GetCount() < 1 || location.GetLength() < 1) {
        scan->EnableWindow(FALSE);
    } else {
        scan->EnableWindow(TRUE);
    }

}

void CConfigFiles::OnMp3Add() 
{
	CString extension;
    m_Mp3Extension.GetWindowText(extension);
    m_Mp3Extensions.AddString(extension);
    UpdateData(FALSE);
	SetModified(TRUE);
//    StoreReg();
	
}

void CConfigFiles::OnMp3Remove() 
{
    m_Mp3Extensions.DeleteString(m_Mp3Extensions.GetCurSel());
    UpdateData(FALSE);
	SetModified(TRUE);
//    StoreReg();
}

BOOL CConfigFiles::UseGenre() {
	return (m_UseGenreUL == 1);
	return TRUE;
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
	return CPropertyPage::OnApply();
}

void CConfigFiles::OnOK() 
{
	CPropertyPage::OnOK();
    StoreReg();
	SetModified(FALSE);

}
void CConfigFiles::OnCancel() 
{
	m_Mp3Extensions.ResetContent();
    m_MP3DirList.ResetContent();

    POSITION pos;
    for (pos = m_origMp3Extensions.GetHeadPosition(); pos != NULL; ) {
        CString extension = m_origMp3Extensions.GetAt(pos);
        m_Mp3Extensions.AddString(extension);
	    m_origMp3Extensions.GetNext(pos);
    }
    for (pos = m_origMP3DirList.GetHeadPosition(); pos != NULL; ) {
        CString extension = m_origMP3DirList.GetAt(pos);
        m_MP3DirList.AddString(extension);
	    m_origMP3DirList.GetNext(pos);
    }
    m_MdbLocation.SetWindowText(m_origMdbLocation);
    StoreReg();
//    UpdateData(FALSE);
	CPropertyPage::OnCancel();
}

void CConfigFiles::OnUsegenre() 
{
	SetModified(TRUE);
	
}
