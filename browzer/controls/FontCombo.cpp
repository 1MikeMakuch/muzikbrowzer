// FontBox.cpp : implementation file
//

#include "Stdafx.h"
#include "resource.h"
#include "FontCombo.h"
#include "MBGlobals.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Constant and should not be changed unless 
// image IDB_GLYPH is changed
#define GLYPH_WIDTH 15 

//////////////////////////////////////////////////////////////////////////
// © Paramax Technology Limited                                         // 
// ----------------------------                                         //
//                                                                      //
// The author accepts no liablility for injury or loss of profits       // 
// if this software is used. You willingness to use this software       //
// indicates you accept total liability                                 //
//                                                                      // 
//////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CFontCombo

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CACFontCombo::CFontCombo
//
// DESCRIPTION:	Constructor
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
CFontCombo::CFontCombo() : m_first_init(true)
{
	// Load up glyphs
	m_img.Create(IDB_GLYPHS,15,1,RGB(255,0,255));
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::~CFontCombo
//
// DESCRIPTION:	Constructor
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
CFontCombo::~CFontCombo()
{
   	POSITION pos = m_mapFonts.GetStartPosition();
	
	while (pos)
	{
		CString strKey;
		CFontObj* pFontObj;
		m_mapFonts.GetNextAssoc(pos,strKey,pFontObj);
		delete pFontObj;
	}
}

BEGIN_MESSAGE_MAP(CFontCombo, CComboBox)
	//{{AFX_MSG_MAP(CFontCombo)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_CONTROL_REFLECT(CBN_KILLFOCUS, OnKillfocus)
	ON_CONTROL_REFLECT(CBN_SETFOCUS, OnSetfocus)
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseUp)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontCombo message handlers
// Overridables for onwerdraw combos
void CFontCombo::DeleteItem(LPDELETEITEMSTRUCT) 
{
}
// Overridables for onwerdraw combos
void CFontCombo::MeasureItem(LPMEASUREITEMSTRUCT) 
{
}

// Initialize fonts for combo
int CFontCombo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	Initialize();

	return 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::DrawItem
//
// DESCRIPTION:	Owner draw to render bitmap and font name
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	ASSERT(lpDIS->CtlType == ODT_COMBOBOX); // We've gotta be a combo

	// Lets make a CDC for ease of use
	CDC *pDC = CDC::FromHandle(lpDIS->hDC);
	
	ASSERT(pDC); // Attached failed
	
	CRect rc(lpDIS->rcItem);
	
	// Draw focus rectangle
	if (lpDIS->itemState & ODS_FOCUS)
		pDC->DrawFocusRect(rc);
	
	// Save off context attributes
	int nIndexDC = pDC->SaveDC();

	CBrush brushFill;
	
	// Draw selection state
	if (lpDIS->itemState & ODS_SELECTED)
	{
		brushFill.CreateSolidBrush(::GetSysColor(COLOR_HIGHLIGHT));
		pDC->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else
		brushFill.CreateSolidBrush(pDC->GetBkColor());

	pDC->SetBkMode(TRANSPARENT);
	pDC->FillRect(rc, &brushFill);

	CString strCurFont,strNextFont;
	GetLBText(lpDIS->itemID,strCurFont);

	CFontObj* pFontObj;
	m_mapFonts.Lookup(strCurFont,pFontObj);

	ASSERT(pFontObj != NULL);
	DWORD dwData = pFontObj->GetFlags();
	
	// Render Bitmaps
	if (dwData & TRUETYPE_FONT)
		m_img.Draw(pDC,1, CPoint(rc.left,rc.top),ILD_TRANSPARENT);

	if (dwData & PRINTER_FONT)
		m_img.Draw(pDC,0, CPoint(rc.left,rc.top),ILD_TRANSPARENT);
	
	int nX = rc.left; // Save for lines

	rc.left += GLYPH_WIDTH + 2; // Text Position
	pDC->TextOut(rc.left,rc.top,strCurFont);

	// GetItemData - return font in use
	if (GetItemData(lpDIS->itemID))
	{
		GetLBText(lpDIS->itemID+1,strNextFont);
		CFontObj* pFontObjNext;
		m_mapFonts.Lookup(strNextFont,pFontObjNext);

		if (!GetItemData(lpDIS->itemID+1))
		{
			// Draw font MRU separator ==============
			//                         
			TEXTMETRIC tm;
			pDC->GetTextMetrics(&tm);

			pDC->MoveTo(nX,rc.top+tm.tmHeight);
			pDC->LineTo(rc.right,rc.top+tm.tmHeight);

			pDC->MoveTo(nX,rc.top+tm.tmHeight+2);
			pDC->LineTo(rc.right,rc.top+tm.tmHeight+2);
	
		}
	}

	// Restore State of context
	pDC->RestoreDC(nIndexDC);
}


// As it says...EnumerateFonts on the system
BOOL CFontCombo::EnumerateFonts()
{
	HDC hDC;
	
	// Get screen fonts
	hDC = ::GetWindowDC(NULL);
	
	LOGFONT lf;
	
	ZeroMemory(&lf,sizeof(lf));
	lf.lfCharSet = ANSI_CHARSET;

	if (!EnumFontFamiliesEx(
			hDC,	// handle to device context
			&lf,	// pointer to logical font information
			(FONTENUMPROC)EnumFamScreenCallBackEx,	// pointer to callback function
			(LPARAM) this,	// application-supplied data
			(DWORD) 0))
		return FALSE;

	::ReleaseDC(NULL,hDC);	

    return TRUE;

	// Now get printer fonts
	CPrintDialog dlg(FALSE);
	if (AfxGetApp()->GetPrinterDeviceDefaults(&dlg.m_pd))
	{
		// GetPrinterDC returns a HDC so attach it
		hDC= dlg.CreatePrinterDC();
		ASSERT(hDC != NULL);

		ZeroMemory(&lf,sizeof(lf));
		lf.lfCharSet = ANSI_CHARSET;

		if (!EnumFontFamiliesEx(
				hDC,	// handle to device context
				&lf,	// pointer to logical font information
				(FONTENUMPROC)EnumFamPrinterCallBackEx,	// pointer to callback function
				(LPARAM) this,	// application-supplied data
				(DWORD) 0))
			return FALSE;

	}
	
	return TRUE; // All's ok
}


////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::OnDestroy
//
// DESCRIPTION:	Destroys font objects
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::OnDestroy() 
{
#ifdef asdf
	POSITION pos = m_mapFonts.GetStartPosition();
	
	while (pos)
	{
		CString strKey;
		CFontObj* pFontObj;
		m_mapFonts.GetNextAssoc(pos,strKey,pFontObj);
		delete pFontObj;
	}
#endif

	CComboBox::OnDestroy();
}


////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::SetFontInUse
//
// DESCRIPTION:	Flags a font that is in use, similar to MS Words font MRU list
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::SetFontInUse(const CString& strFont)
{
	CFontObj *pFontObj;
	CString strFontDesc;
	
	// Find the desired font
	if (m_mapFonts.Lookup(strFont,pFontObj))
	{
		// Check the font is not already in use
		if (!pFontObj->GetFontInUse())
		{
			// Set the font as the top most and selected font
			pFontObj->SetFontInUse(TRUE);
			InsertString(0,strFont);
			SetItemData(0,DWORD(1));

			// Only hold six MRU'ed fonts
			// so drop the 7th.
			if (GetItemData(6))
			{
				GetLBText(6,strFontDesc);
				if (!strFontDesc.IsEmpty())
				{
					if (m_mapFonts.Lookup(strFontDesc,pFontObj))
					{
						pFontObj->SetFontInUse(FALSE);
						DeleteString(6);
					}
				}
			}
		}
		else
		{
			// If font is aleady MRU'ed position it as the first font
			int nSel = FindString(-1,strFont);
	
			if (nSel != CB_ERR)
			{
				// Remove from original position
				DeleteString(nSel);
				// Restore to begining of list
			
				InsertString(0,strFont);
				SetItemData(0,TRUE); // Mark as selected
				SetCurSel(0);	// Bring as current selection
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::OnKillfocus
//
// DESCRIPTION:	Set up the current font
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::OnKillfocus() 
{
	SetCurrentFont();
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::OnSetfocus
//
// DESCRIPTION:	Save current font
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::OnSetfocus() 
{
	// Save off original font
	GetWindowText(m_strFontSave);	
	GetParent()->SendMessage(MB_CONFIG_CBUTTON, 0, 0);  
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::SetCurrentFont
//
// DESCRIPTION:	Set up the current font
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::SetCurrentFont()
{
	CString strSelFont;

	// Change font in edit box is a known font
	// otherwise reject and restore orginal
	int nSel = GetCurSel();

	if (nSel == CB_ERR)
	{
		GetWindowText(strSelFont);
		nSel = FindStringExact(-1,strSelFont);
		
		if (nSel == CB_ERR)
		{
			SetWindowText(m_strFontSave);
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::OnCloseUp
//
// DESCRIPTION:	Hide tip window and set font name
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::OnCloseUp()
{
	int nSel;	

	CString strFont;
	// Set Face Name
	SetCurrentFont();
	nSel = GetCurSel();
	if (nSel != CB_ERR)
		GetLBText(nSel,strFont);

	SetFontInUse(strFont);
	m_wndTip.ShowWindow(SW_HIDE);
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::Initialize
//
// DESCRIPTION:	Initials combo box
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::Initialize()
{
	// Yep tip window is created here
	//    static bool first = true;
	//    if (first == true) {
	//        first = false;
	//	    m_wndTip.Create(this);
	//    }
	if (m_first_init == true) {
		m_first_init = false;
		m_wndTip.Create(this);
	}

	// Set default font name
	CString strDefault = "";
	
	CFontObj* pFontObj;
	CString strKey,strComp;
	EnumerateFonts();

	POSITION pos = m_mapFonts.GetStartPosition();
	
	while (pos)
	{
		m_mapFonts.GetNextAssoc(pos,strKey,pFontObj);

		int nMax = GetCount();
		BOOL bInsert = FALSE;
		for (int nIdx=0;nIdx < nMax;nIdx++)
		{			
			GetLBText(nIdx,strComp);
			
			if (strComp.Collate(strKey) == 1)
			{
				bInsert = TRUE;
				InsertString(nIdx,strKey);
				break;
			}
		}		

		if (!bInsert)
			AddString(strKey);
	}

	// We set the timer because its the only way we know when a selection
	// has changed - use for tip window
	SetTimer(MB_FONTCOMBO_TIMER_ID, 500, NULL);
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::AddFont
//
// DESCRIPTION:	Adds a font to the internal array
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::AddFont(CString strName, DWORD dwFlags)
{
	CFontObj* pFontObj;	

	// Check fonts not aleady in the array
	if (!m_mapFonts.Lookup(strName,pFontObj))
		m_mapFonts.SetAt(strName,new CFontObj(dwFlags));
}

////////////////////////////////////////////////////////////////////////////////
//
// FUNCTION:	CFontCombo::OnTimer
//
// DESCRIPTION:	Positions tip window against selected font
//
// INPUTS:		
//
// RETURNS:     
//
// NOTES:       
//
// MODIFICATIONS:
//
// Name			Date      Version	Comments
// N T ALMOND   25/09/98  1.0		Origin
//
////////////////////////////////////////////////////////////////////////////////
void CFontCombo::OnTimer(UINT nIDEvent) 
{
	
	// Is combo open
	if (GetDroppedState( ))
	{
		int nSel = GetCurSel();
		
		// Selected
		if (nSel != -1)
		{
			CString str;
			GetLBText(nSel,str);
			CRect rc;
			GetDroppedControlRect(rc);

			int nHeight = GetItemHeight(0) * ((nSel - GetTopIndex()) + 1);
			
			CPoint pt(rc.right + 5,rc.top + nHeight);

			// Show tip in correct position
			m_wndTip.ShowTips(pt,str);
		}
	}
	
	
	CComboBox::OnTimer(nIDEvent);
}

BOOL CALLBACK AFX_EXPORT CFontCombo::EnumFamScreenCallBackEx(ENUMLOGFONTEX* pelf, 
	NEWTEXTMETRICEX* /*lpntm*/, int FontType, LPVOID pThis)

{
	// don't put in non-printer raster fonts
	if (FontType & RASTER_FONTTYPE)
		return 1;
	
	DWORD dwData;
	
	dwData = (FontType & TRUETYPE_FONTTYPE) ? TRUETYPE_FONT : 0;
	((CFontCombo*)pThis)->AddFont(pelf->elfLogFont.lfFaceName, dwData);
	
	return 1; // Call me back
}

BOOL CALLBACK AFX_EXPORT CFontCombo::EnumFamPrinterCallBackEx(ENUMLOGFONTEX* pelf, 
	NEWTEXTMETRICEX* /* lpntm */, int FontType, LPVOID pThis)
{
	
	if (!(FontType & DEVICE_FONTTYPE))
		return 1;

	if ((FontType & TRUETYPE_FONTTYPE))
		return 1;

	DWORD dwData = PRINTER_FONT;
	
	((CFontCombo*)pThis)->AddFont(pelf->elfLogFont.lfFaceName, dwData);
	
	return 1; // Call me back
}

