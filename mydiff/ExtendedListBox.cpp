// ExtendedListBox.cpp : implementation file
//

#include "stdafx.h"
//#include "listtest.h"
#include "ExtendedListBox.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtendedListBox

CExtendedListBox::CExtendedListBox() : m_highlights(FALSE), m_companion(NULL)
{
	m_font.CreateStockObject(ANSI_VAR_FONT);
}

CExtendedListBox::~CExtendedListBox()
{
}


// default implementation
BOOL CExtendedListBox::IsItemBold(UINT nIndex) const
{
	if (nIndex>=(DWORD)GetCount())
		return TRUE;	// whatever

	DWORD uData=GetItemData(nIndex);

	return (uData&1);
}
BOOL CExtendedListBox::IsItemDiff(UINT nIndex) const
{
	if (nIndex>=(DWORD)GetCount())
		return TRUE;	// whatever

	DWORD uData=GetItemData(nIndex);

	return (uData&2);
}


BEGIN_MESSAGE_MAP(CExtendedListBox, CListBox)
	//{{AFX_MSG_MAP(CExtendedListBox)
	//}}AFX_MSG_MAP
	ON_WM_VSCROLL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtendedListBox message handlers

void CExtendedListBox::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	ASSERT(GetStyle() & LBS_OWNERDRAWFIXED);
	ModifyStyle(0, LBS_HASSTRINGS | LBS_WANTKEYBOARDINPUT, 0);
	
	CListBox::PreSubclassWindow();
}

void CExtendedListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	ASSERT((GetStyle() & (LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_WANTKEYBOARDINPUT)) ==
		(LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | LBS_WANTKEYBOARDINPUT));

	CDC* pDC = CDC::FromHandle (lpDrawItemStruct->hDC);

	if (((LONG)(lpDrawItemStruct->itemID) >= 0) &&
		(lpDrawItemStruct->itemAction & (ODA_DRAWENTIRE | ODA_SELECT)))
	{
		BOOL fBold = IsItemBold(lpDrawItemStruct->itemID);
		if (fBold) {
			Sleep(0);
		}

		COLORREF newTextColor = fBold ?
			RGB(0,0,0) : RGB(0,0,0);  // light gray
		COLORREF newBkColor = fBold ?
			RGB(240,240,240) : RGB(255,255,255);


		COLORREF oldTextColor = pDC->SetTextColor (newTextColor);

//		COLORREF newBkColor = GetSysColor (COLOR_WINDOW);
		COLORREF oldBkColor = pDC->SetBkColor (newBkColor);

		if (newTextColor == newBkColor)
			newTextColor = RGB(0xC0, 0xC0, 0xC0);   // dark gray

		if (m_highlights && !fBold && ((lpDrawItemStruct->itemState & ODS_SELECTED) != 0))
		{
			pDC->SetTextColor (GetSysColor (COLOR_HIGHLIGHTTEXT));
			pDC->SetBkColor (GetSysColor (COLOR_HIGHLIGHT));
		}

		CString strText;
		GetText (lpDrawItemStruct->itemID, strText);
		strText = String::replace(strText, '\t', "    ");

		const RECT &rc=lpDrawItemStruct->rcItem;
		
		CFont *oldfont = pDC->SelectObject(&m_font);
		pDC->ExtTextOut(rc.left + 2,
				  rc.top + 2,// + max(0, (cyItem - m_cyText) / 2),
				  ETO_OPAQUE, &rc,
				  strText, strText.GetLength (), NULL);
		pDC->SelectObject(oldfont);

		pDC->SetTextColor (oldTextColor);
		pDC->SetBkColor (oldBkColor);
	}

	if ((lpDrawItemStruct->itemAction & ODA_FOCUS) != 0)
		pDC->DrawFocusRect(&lpDrawItemStruct->rcItem);
}

void CExtendedListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	// TODO: Add your code to determine the size of specified item
	UNREFERENCED_PARAMETER(lpMeasureItemStruct);
}

int CExtendedListBox::insertString(int idx, LPCTSTR lpszItem, int bold,
								   int diff) {
	int r = InsertString(idx, lpszItem);
	DWORD flag = 0;
	if (bold)
		flag = 0x1;
	if (diff)
		flag |= 0x2;

	SetItemData(r, flag);

	return r;
}
void CExtendedListBox::OnVScroll(UINT nSBCode, UINT nPos,
								 CScrollBar* pScrollBar) 
{
	CListBox::OnVScroll(nSBCode, nPos, pScrollBar);

	if (m_companion) {
		int newpos = GetScrollPos(SB_VERT);
		m_companion->SetScrollPos(SB_VERT, newpos, TRUE );
		int idx = GetTopIndex();
		m_companion->SetTopIndex(idx);
	}
}
