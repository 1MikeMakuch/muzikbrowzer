// Mbutton.cpp : implementation file
//

#include "stdafx.h"
#include "Mbutton.h"
#include "PlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMbutton

CMbutton::CMbutton()
{
}

CMbutton::~CMbutton()
{
}


BEGIN_MESSAGE_MAP(CMbutton, CButton)
	//{{AFX_MSG_MAP(CMbutton)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMbutton message handlers

void CMbutton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    CBrush br;
    CString sBtnText;
	GetWindowText(sBtnText);

    CDC * cdc = CDC::FromHandle(lpDrawItemStruct->hDC);

    br.CreateSolidBrush(thePlayer->config().getColorBkPanel());

    cdc->SetTextColor(thePlayer->config().getColorTxPanel());
    cdc->SetBkColor(thePlayer->config().getColorBkPanel());
    cdc->FillRect(&lpDrawItemStruct->rcItem, &br);
    cdc->DrawText(sBtnText, &lpDrawItemStruct->rcItem, 
        DT_SINGLELINE | DT_TOP | DT_LEFT);
    br.DeleteObject();
}

