// MBConfig.cpp : implementation file
//

#include "stdafx.h"
#include "MBConfig.h"
//#include "ConfigColors.h"
//#include "ConfigFonts.h"
#include "ConfigDisplay.h"
#include "ConfigIrman.h"
#include "ConfigFiles.h"
#include "ConfigPassword.h"
#include "Registry.h"
#include "MyString.h"
#include "MyLog.h"

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
    
{m_Password=NULL;}

MBConfig::MBConfig(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{m_Password=NULL;}

CString
MBConfig::mbdir() {
	return m_Files->mbdir();
}

MBConfig::~MBConfig()
{
    if (m_Files) delete m_Files;
    if (m_Irman) delete m_Irman;
	if (m_Display) delete m_Display;
//    if (m_Fonts) delete m_Fonts;
//    if (m_Colors) delete m_Colors;
// leave this in, still need it to call trialMode()
#ifdef MB_USING_TRIAL_MODE 
	if (m_Password) delete m_Password;
#endif
}

void
MBConfig::init() {
    m_Irman = new CConfigIrman(m_CWnd);
//    m_Colors = new CConfigColors(m_CWnd);
    m_Files = new CConfigFiles(m_CWnd, m_playercallbacks);
//    m_Fonts = new CConfigFonts(m_CWnd);
	m_Display = new CConfigDisplay(m_CWnd, m_playercallbacks);
#ifdef MB_USING_TRIAL_MODE
	m_Password = new CConfigPassword(m_CWnd,this);
#endif

    AddPage(m_Files);
	AddPage(m_Display);
    AddPage(m_Irman);
#ifdef MB_USING_TRIAL_MODE
	AddPage(m_Password);
#endif

}

BEGIN_MESSAGE_MAP(MBConfig, CPropertySheet)
	//{{AFX_MSG_MAP(MBConfig)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_COMMAND(ID_MENU_HELP, HelpInfo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// MBConfig message handlers
void
MBConfig::ReadWindowPos() {

    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
    int x1,y1;


    x1 = reg.Read("SettingsDlgX", 0);
    y1 = reg.Read("SettingsDlgY", 0);
    
	if (x1 && y1) {
		CRect dialog;
		GetWindowRect(dialog);
		MoveWindow(x1,y1,dialog.Width(),dialog.Height(),TRUE);
	}
}
void
MBConfig::SaveWindowPos() {
	AutoLog al("SaveWindowPos");
    CRect dialog;
    GetWindowRect(dialog);

    RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );

    reg.Write("SettingsDlgX", dialog.TopLeft().x);
    reg.Write("SettingsDlgY", dialog.TopLeft().y);


}

BOOL MBConfig::OnInitDialog() 
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	ReadWindowPos();
	
//	pApplyButton = GetDlgItem (ID_APPLY_NOW);
//	ASSERT (pApplyButton);
//	pApplyButton->ShowWindow (SW_HIDE);
//	pApplyButton->EnableWindow(TRUE);
	
	return bResult;
}

BOOL
MBConfig::resetNeeded() {
	return m_Files->resetNeeded();
}
LPLOGFONT
MBConfig::getTitlesFont() {
    return m_Display->getTitlesFont();
}
LPLOGFONT
MBConfig::getStatusFont() {
    return m_Display->getStatusFont();
}
LPLOGFONT
MBConfig::getColHdrFont() {
    return m_Display->getColHdrFont();
}
//LPLOGFONT
//MBConfig::getCurPlayFont() {
//    return m_Display->getCurPlayFont();
//}
COLORREF
MBConfig::getColorBkPanel() {
    return m_Display->getBkPanel();
}
COLORREF
MBConfig::getColorBkNormal() {
    return m_Display->getBkNormal();
}
COLORREF
MBConfig::getColorBkHigh() {
    return m_Display->getBkHigh();
}
COLORREF
MBConfig::getColorBkSel() {
    return m_Display->getBkSel();
}
COLORREF
MBConfig::getColorTxPanel() {
    return m_Display->getTxPanel();
}
COLORREF
MBConfig::getColorTxNormal() {
    return m_Display->getTxNormal();
}
COLORREF
MBConfig::getColorTxHigh() {
    return m_Display->getTxHigh();
}
COLORREF
MBConfig::getColorTxSel() {
    return m_Display->getTxSel();
}
COLORREF
MBConfig::getColorTxColHdr() {
    return m_Display->getTxColHdr();
}
COLORREF
MBConfig::getColorBkColHdr() {
    return m_Display->getBkColHdr();
}
//COLORREF
//MBConfig::getColorTxCurPlay() {
//    return m_Display->getTxCurPlay();
//}
//COLORREF
//MBConfig::getColorBkCurPlay() {
//    return m_Display->getBkCurPlay();
//}
//int
//MBConfig::getPanelWidth() {
//    return m_Display->getPanelWidth();
//}

int
MBConfig::getDlgBorderWidth() {
	return m_Display->getBorderWidth();
}
int
MBConfig::getDlgBorderHorz() {
	return m_Display->getBorderHorz();
}
void
MBConfig::ReadReg(RegistryKey & key) {
	m_Display->ReadReg(key);
}

int
MBConfig::getDlgBorderVert() {
	return m_Display->getBorderVert();
}

const CString
MBConfig::getSkin(const CString key) {
	return m_Display->getSkin(key);
}
void MBConfig::getSkins(CStringList & l) {
	m_Display->getSkins(l);
}
BOOL MBConfig::ChooseSkin(CString s) {
	return m_Display->OnSkinChoose(s);
}
BOOL MBConfig::verifySkin() {
	return m_Display->verifySkin();
}
BOOL MBConfig::verifySkin(const CString skin, const BOOL stopOnError) {
	return m_Display->verifySkin(skin,stopOnError);
}
BOOL MBConfig::findGoodSkin() {
	return m_Display->findGoodSkin();
}
const CString MBConfig::getCurrentSkin() {
	return m_Display->getCurrentSkin();
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

void MBConfig::getRegistry(const CString & key, CString & value, const CString & dflt) {
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	AutoBuf buf(1000);
	reg.Read((LPCTSTR)key, buf.p, 999, dflt);
	value = buf.p;
}

void MBConfig::setRegistry(const CString & key, const CString & value) {
	RegistryKey reg( HKEY_LOCAL_MACHINE, RegKey );
	reg.Write((LPCTSTR)key, (LPCTSTR)value);
}
int MBConfig::trialMode() { 
	if (m_Password) {
		return m_Password->trialMode(); 
	} else return FALSE;
}
int MBConfig::trialLeft() {
	if (m_Password)
		return m_Password->trialLeft();
	else return 1;
}

BOOL MBConfig::HelpInfo(const int idx) {
    TCHAR szPath[_MAX_PATH],
      szFname[_MAX_FNAME],
      szDir[_MAX_DIR],
      szDrive[_MAX_DRIVE];

	if (0 == GetModuleFileName(NULL, szPath, sizeof(szPath))) {
		DWORD dwError = GetLastError();
		CString msg;
		msg << "HelpInfo: Error " << dwError;
		return FALSE;
	}

	CString helppage,anchor ;

    _splitpath(szPath, szDrive, 
               szDir, szFname, NULL);
    _makepath(szPath, szDrive, szDir, 
                      MUZIKBROWZER, "chm");

	helppage = "ms-its:" + CString(szPath);
	if (0 == idx) {
		anchor = "::/Html/Configuration.htm#General";
	} else if (1 == idx) {
		anchor = "::/Html/Configuration.htm#Display";
	} else if (2 == idx) {
		anchor = "::/Html/Configuration.htm#RemoteControl";
	} else if (3 == idx) {
		anchor = "::/Html/Configuration.htm#License";
	}
	helppage += anchor;

	HINSTANCE h = ShellExecute(this->m_hWnd, "open",  "hh.exe", helppage, 
		szDir, SW_SHOW);
	if ((int)h <= 32) {
		CString msg;
		msg << "ShellExecute on help failed. ShellExecute returned "
			<< (int)h << " on " << szPath << " " << szDir;
	}

    return(TRUE);
}

BOOL MBConfig::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	UINT nID = LOWORD(wParam);
	if(nID == IDHELP) {
		int i = GetActiveIndex();
		HelpInfo(i);
	} else {
		if (IDOK == nID)
			SaveWindowPos();
		return CPropertySheet::OnCommand(wParam, lParam);
	}
	return TRUE;
}

void
MBConfig::GetDirs(CStringArray & dirs,CStringArray & excludes, const CString & dir) {
	m_Files->GetDirs(dirs,excludes,dir);
}
void
MBConfig::AddFolders(const CStringArray & dirs) {
	m_Files->AddFolders(dirs);
}
void
MBConfig::DelFolders(const CStringArray & dirs) {
	m_Files->DelFolders(dirs);
}
BOOL
MBConfig::hideGenre() {
	return m_Files->hideGenre();
}
CString
MBConfig::license() {
	if (m_Password)
		return m_Password->license();
	return "";
}
void
MBConfig::resetTrial() {
	if (m_Password)
		m_Password->resetTrial();
}
void
MBConfig::initSkins() {
	m_Display->init();
}
void 
MBConfig::AddDeletedFiles(const CStringArray & files) {
	m_Files->AddDeletedFiles(files);
}
void
MBConfig::logSettings() {
	MyHash settings ;
	getSettings(settings);
	POSITION pos;
	CString key,val;
	logger.log("Settings:");
	for(pos = settings.GetSortedHead(); pos != NULL; ) {
		settings.GetNextAssoc(pos,key,val);
		logger.log(" "+key,val);
	}

}
void
MBConfig::getSettings(MyHash & settings) {
	if (m_Files) m_Files->getSettings(settings);
	if (m_Display) m_Display->getSettings(settings);
	if (m_Irman) m_Irman->getSettings(settings);
	if (m_Password) m_Password->getSettings(settings);
}
int
MBConfig::logging() {
	return m_Files->logging();
}

