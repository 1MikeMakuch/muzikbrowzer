// MBConfig.cpp : implementation file
//

#include "stdafx.h"
#include "MBConfig.h"
#include "ConfigColors.h"
#include "ConfigFonts.h"
#include "ConfigIrman.h"
#include "ConfigFiles.h"
//#include "ConfigPassword.h"
#include "irman_registry.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

class CPlayerDlg;

/////////////////////////////////////////////////////////////////////////////
// MBConfig

IMPLEMENT_DYNAMIC(MBConfig, CPropertySheet)

MBConfig::MBConfig(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
    
{}

MBConfig::MBConfig(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{}


MBConfig::~MBConfig()
{
    if (m_Files) delete m_Files;
    if (m_Irman) delete m_Irman;
    if (m_Fonts) delete m_Fonts;
    if (m_Colors) delete m_Colors;
//	if (m_Password) delete m_Password;
}

void
MBConfig::init() {
    m_Irman = new CConfigIrman(m_CWnd);
    m_Colors = new CConfigColors(m_CWnd);
    m_Files = new CConfigFiles(m_PlayerDlg);
    m_Fonts = new CConfigFonts(m_CWnd);
//	m_Password = new CConfigPassword(m_CWnd);

    AddPage(m_Files);
    AddPage(m_Irman);
    AddPage(m_Fonts);
    AddPage(m_Colors);
//	AddPage(m_Password);
}

BEGIN_MESSAGE_MAP(MBConfig, CPropertySheet)
	//{{AFX_MSG_MAP(MBConfig)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_COMMAND(ID_MENU_HELP, HelpInfo)
	//}}AFX_MSG_MAP

	ON_WM_HELPINFO()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MBConfig message handlers
LPLOGFONT
MBConfig::getTitlesFont() {
    return m_Fonts->getTitlesFont();
}
LPLOGFONT
MBConfig::getPanelFont() {
    return m_Fonts->getPanelFont();
}
COLORREF
MBConfig::getColorBkPanel() {
    return m_Colors->getBkPanel();
}
COLORREF
MBConfig::getColorBkNormal() {
    return m_Colors->getBkNormal();
}
COLORREF
MBConfig::getColorBkHigh() {
    return m_Colors->getBkHigh();
}
COLORREF
MBConfig::getColorBkSel() {
    return m_Colors->getBkSel();
}
COLORREF
MBConfig::getColorTxPanel() {
    return m_Colors->getTxPanel();
}
COLORREF
MBConfig::getColorTxNormal() {
    return m_Colors->getTxNormal();
}
COLORREF
MBConfig::getColorTxHigh() {
    return m_Colors->getTxHigh();
}
COLORREF
MBConfig::getColorTxSel() {
    return m_Colors->getTxSel();
}
BOOL
MBConfig::UseGenre() {
	return m_Files->UseGenre();
}
BOOL
MBConfig::AlbumSortAlpha() {
	return m_Files->AlbumSortAlpha();
}
void MBConfig::getRegistry(const CString & key, int & value, int dflt) {
    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
    value = reg.Read((LPCTSTR)key, dflt);
}

void MBConfig::setRegistry(const CString & key, const int value) {
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	reg.Write((LPCTSTR)key, value);
}

void MBConfig::getRegistry(const CString & key, CString & value) {
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	AutoBuf buf(1000);
	reg.Read((LPCTSTR)key, buf.p, 999, "");
}

void MBConfig::setRegistry(const CString & key, const CString & value) {
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	reg.Write((LPCTSTR)key, (LPCTSTR)value);
}
//int MBConfig::trialMode() { return m_Password->trialMode(); }

BOOL MBConfig::HelpInfo() {
    TCHAR szPath[_MAX_PATH],
      szFname[_MAX_FNAME],
      szDir[_MAX_DIR],
      szDrive[_MAX_DRIVE];

	if (0 == GetModuleFileName(NULL, szPath, sizeof(szPath))) {
		DWORD dwError = GetLastError();
		CString msg;
		msg << "HelpInfo: Error " << dwError;
//		logger.log(msg);
		return FALSE;
	}

    _splitpath(szPath, szDrive, 
               szDir, szFname, NULL);
    _makepath(szPath, szDrive, szDir, 
                      MUZIKBROWZER, "chm");

//	HtmlHelp(GetDesktopWindow()->m_hWnd, 
//                 szPath,HH_DISPLAY_TOPIC, NULL);

//	HINSTANCE h = ShellExecute(this->m_hWnd, "open", "KeyHH.exe", szPath,
	HINSTANCE h = ShellExecute(this->m_hWnd, "open",  szPath, NULL,
		szDir, SW_SHOW);
	if ((int)h <= 32) {
		CString msg;
		msg << "ShellExecute on help failed. ShellExecute returned "
			<< (int)h << " on " << szPath << " " << szDir;
//		logger.log(msg);
	}

    return(TRUE);
}
BOOL MBConfig::OnHelpInfo(HELPINFO* pHelpInfo) {
	return HelpInfo();

}
BOOL MBConfig::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT nID = LOWORD(wParam);
	if(nID == IDHELP) {
		HelpInfo();
	} else {
		return CPropertySheet::OnCommand(wParam, lParam);
	}
	return TRUE;
}
