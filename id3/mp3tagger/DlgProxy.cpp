// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "id3tagger.h"
#include "DlgProxy.h"
#include "id3taggerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CId3taggerDlgAutoProxy

IMPLEMENT_DYNCREATE(CId3taggerDlgAutoProxy, CCmdTarget)

CId3taggerDlgAutoProxy::CId3taggerDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT (AfxGetApp()->m_pMainWnd != NULL);
	ASSERT_VALID (AfxGetApp()->m_pMainWnd);
	ASSERT_KINDOF(CId3taggerDlg, AfxGetApp()->m_pMainWnd);
	m_pDialog = (CId3taggerDlg*) AfxGetApp()->m_pMainWnd;
	m_pDialog->m_pAutoProxy = this;
}

CId3taggerDlgAutoProxy::~CId3taggerDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CId3taggerDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CId3taggerDlgAutoProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CId3taggerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CId3taggerDlgAutoProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CId3taggerDlgAutoProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IId3tagger to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {914924E8-D499-47FB-B8FA-A0E766A566D6}
static const IID IID_IId3tagger =
{ 0x914924e8, 0xd499, 0x47fb, { 0xb8, 0xfa, 0xa0, 0xe7, 0x66, 0xa5, 0x66, 0xd6 } };

BEGIN_INTERFACE_MAP(CId3taggerDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CId3taggerDlgAutoProxy, IID_IId3tagger, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {1280B282-F699-4E77-8875-CD5D915427F8}
IMPLEMENT_OLECREATE2(CId3taggerDlgAutoProxy, "Id3tagger.Application", 0x1280b282, 0xf699, 0x4e77, 0x88, 0x75, 0xcd, 0x5d, 0x91, 0x54, 0x27, 0xf8)

/////////////////////////////////////////////////////////////////////////////
// CId3taggerDlgAutoProxy message handlers
