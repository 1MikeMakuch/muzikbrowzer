
// PlayerApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PlayerApp.h"
#include "PlayerDlg.h"
#include "controls/TransparentDialogDlg.h"
#include "TestHarness/TestHarness.h"
#include "MBMessageBox.h"

//#include "InitDlg.h"
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
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance

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

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

	RunTestHarness();


#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
    //COLORREF clrCtlBk = RGB(192, 192, 192);
    //COLORREF clrCtlBk = RGB(0,0,0);
    //COLORREF clrCtlText = RGB(0, 0, 0);
    //SetDialogBkColor(clrCtlBk , clrCtlText  );

    // Check registration here
    // If in eval mode just continue, but throw up a reminder dialog

//	InitDlg *InitDialog = new InitDlg(1,0);
	CTransparentDialogDlg * splashDlg = new CTransparentDialogDlg();
    CString msg = MUZIKBROWZER;
    msg += " initializing";
//    splashDlg->SetLabel(msg);
	splashDlg->ShowWindow(SW_SHOWNORMAL /* CWinApp::m_nCmdShow */);
	splashDlg->UpdateWindow();
//	Sleep(10000);

    m_PlayerDlg = new CPlayerDlg(this, splashDlg);
	m_pMainWnd = m_PlayerDlg;
	int nResponse = m_PlayerDlg->DoModal();
	
	delete splashDlg;
    delete m_PlayerDlg;

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CPlayerApp::RunTestHarness() {

	TestResult tr;
	TestRegistry::runAllTests(tr);
#ifdef _DEBUG
	if (tr.failures() > 0) {
		MBMessageBox("muzikbrowzer Test Results", tr.results());
	} else {
		logger.log("Test Results");
		logger.log(tr.results());
	}
#else
	logger.log("Test Results");
	logger.log(tr.results());
#endif
}