
// PlayerApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PlayerApp.h"
#include "PlayerDlg.h"
#include "controls/TransparentDialogDlg.h"
#include "TestHarness/TestHarness.h"
#include "MBMessageBox.h"
#include "Registry.h"
#include "muzikbrowzerVersion.h"

#define LOGOWNER
#include "MyLog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Globals


/////////////////////////////////////////////////////////////////////////////
// CMfcPlayerApp

BEGIN_MESSAGE_MAP(CPlayerApp, CWinApp)
	//{{AFX_MSG_MAP(CPlayerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerApp construction

CPlayerApp::CPlayerApp()
{
	m_rebuild = FALSE;
	CString cl = ::GetCommandLine();
	if (String::contains(cl,"rebuild")) {
		m_rebuild = TRUE;
		if (String::contains(cl,"dir=")) {
			m_dir = String::extract(cl,"dir=","");
		}
	}

}



/////////////////////////////////////////////////////////////////////////////
// The one and only CPlayerApp object

CPlayerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPlayerApp initialization

BOOL CPlayerApp::InitInstance()
{
//	if (!AfxSocketInit())
//	{
//		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
//		return FALSE;
//	}
	// For WMP Control
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	RegistryKey reg(HKEY_LOCAL_MACHINE, RegKey);
	CString lfile = reg.ReadCString(RegDbLocation,"C:\\Program Files\\Muzikbrowzer");
	int log = reg.Read("Logging",1);
	if (m_rebuild) {
		if (m_dir.GetLength())
			lfile = m_dir;
		lfile += "\\Muzikbrowzer.RebuildDb.log";
	} else {
		lfile += "\\Muzikbrowzer.log";
	}
	logger.open(lfile);
	logger.loglevel(log);
	logger.log(CS("Muzikbrowzer version: ") + CS(MUZIKBROWZER_VERSION));
	CString cl = ::GetCommandLine();
	logger.log(cl);

#ifdef _DEBUG
	if (!m_rebuild && !String::contains(cl,"killTests"))
		RunTestHarness();
	//return FALSE;
#endif



#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	m_PlayerDlg = new CPlayerDlg(this);
	m_pMainWnd = m_PlayerDlg;
	if (m_rebuild)
		m_PlayerDlg->RebuildOnly(m_dir);

	
	int nResponse = m_PlayerDlg->DoModal();
	delete m_PlayerDlg;
	logger.log("shutdown");

//	delete splashDlg;
	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CPlayerApp::RunTestHarness() {


	TestResult tr;
	TestRegistry::runAllTests(tr);

	if (tr.failures() > 0) {
		MBMessageBox("Muzikbrowzer Test Results", tr.results());
	} else {
		logger.log("Test Results");
		logger.log(tr.results());
	}
	logger.log("Tests Complete");

}
