#include "StdAfx.h"
#include <afxtempl.h>
#include "FontTipWnd.h"

#ifndef __FONT_COMBO__
#define __FONT_COMBO__

#define TRUETYPE_FONT		0x0001
#define PRINTER_FONT		0x0002
#define DEVICE_FONT			0x0004

//////////////////////////////////////////////////////////////////////////
// © Paramax Technology Limited                                         // 
// ----------------------------                                         //
//                                                                      //
// The author accepts no liablility for injury or loss of profits       // 
// if this software is used. You willingness to use this software       //
// indicates you accept total liability                                 //
//                                                                      // 
//////////////////////////////////////////////////////////////////////////

class CFontObj : public CObject
{
protected:	
	BOOL	m_bInUse;	// Fonts in use 
	DWORD	m_nFlags;	// Font flags
public:
	CFontObj(DWORD nFlags)
	{	
		m_nFlags = nFlags;
		m_bInUse = FALSE;
	}

	CFontObj(CFontObj* pFontObj)
	{	
		m_nFlags = pFontObj->GetFlags();
		m_bInUse = pFontObj->GetFontInUse();
	}

	int GetFlags() const { return m_nFlags; }
	BOOL GetFontInUse() { return m_bInUse; }
	void SetFontInUse(BOOL bInUse) { m_bInUse = bInUse; }
};

/////////////////////////////////////////////////////////////////////////////
// CFontCombo window
class CFontCombo : public CComboBox
{

// Construction
public:
	CFontCombo();
	void Initialize();

protected:
	BOOL EnumerateFonts();
	void AddFont(CString strName, DWORD dwFlags);
	void SetCurrentFont();
	void SetFontInUse(const CString& strFont);
public:
	static BOOL CALLBACK AFX_EXPORT EnumFamScreenCallBackEx(
	ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
	LPVOID pThis);

	static BOOL CALLBACK AFX_EXPORT EnumFamPrinterCallBackEx(
	ENUMLOGFONTEX* pelf, NEWTEXTMETRICEX* /*lpntm*/, int FontType, 
	LPVOID pThis);

// Attributes
public:
private:
	bool m_first_init;

protected:
	CTipWnd			m_wndTip;
	CString			m_strFontSave;
	CString			m_strDefault;
	CImageList		m_img;

	CTypedPtrMap<CMapStringToPtr, CString, CFontObj*> m_mapFonts;
	CTypedPtrMap<CMapStringToPtr, CString, CFontObj*> m_mruFonts;

	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFontCombo)
	public:
	virtual void DeleteItem(LPDELETEITEMSTRUCT lpDeleteItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFontCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CFontCombo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnKillfocus();
	afx_msg void OnSetfocus();
	afx_msg void OnCloseUp();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#endif

