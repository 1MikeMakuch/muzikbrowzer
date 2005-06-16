// DialogSK.cpp : implementation file
//

#include "stdafx.h"
#include "ControlSK.h"

/////////////////////////////////////////////////////////////////////////////
// CControlSK dialog


//  these variables should have been defined in some standard header but is not
#define WS_EX_LAYERED 0x00080000 
#define LWA_COLORKEY 1 // Use color as the transparency color.
#define LWA_ALPHA    2 // Use bAlpha to determine the opacity of the layer

static lpfnSetLayeredWindowAttributes g_pSetLayeredWindowAttributes;
//  ===========================================================================
//  Function pointer for lyering API in User32.dll
//  ===========================================================================

CControlSK::CControlSK(CWnd* pParent /*=NULL*/)
{
    //{{AFX_DATA_INIT(CBkDialogST)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    Init();
}

CControlSK::CControlSK(UINT uResourceID, CWnd* pParent)

{
    Init();
}


CControlSK::CControlSK(LPCTSTR pszResourceID, CWnd* pParent)
{
    Init();
}

CControlSK::~CControlSK()
{
    FreeResources();
}


BOOL
CControlSK::SetTransparent (HWND m_hWnd, BYTE bAlpha)
{
    if (g_pSetLayeredWindowAttributes == NULL)
        return FALSE;

    if (bAlpha < 255)
    {
        //  set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        
        //  call it with 255 as alpha - opacity
        g_pSetLayeredWindowAttributes(m_hWnd, 0, bAlpha, LWA_ALPHA);
    }
    else
    {
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

        // Ask the window and its children to repaint
        ::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
    }

    return TRUE;
}


BOOL
CControlSK::SetTransparentColor (HWND m_hWnd, COLORREF col, BOOL bTrans)
{
    if (g_pSetLayeredWindowAttributes == NULL)
        return FALSE;

    if (bTrans)
    {
        //  set layered style for the dialog
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
        //  call it with 0 alpha for the given color
        g_pSetLayeredWindowAttributes(m_hWnd, col, 0, LWA_COLORKEY);
    }
    else
    {
        SetWindowLong(m_hWnd, GWL_EXSTYLE, GetWindowLong(m_hWnd, GWL_EXSTYLE) & ~WS_EX_LAYERED);

        // Ask the window and its children to repaint
        ::RedrawWindow(m_hWnd, NULL, NULL, RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
    }

    return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CControlSK message handlers

//void CControlSK::OnLButtonDown(UINT nFlags, CPoint point) 
//{
//    if (m_bEasyMove)
//        PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));	
//
//    CDialog::OnLButtonDown(nFlags, point);
//}

void CControlSK::Init()
{
    m_hBitmap = NULL;
    m_loStyle = LO_DEFAULT;
    //  get the function from the user32.dll 
    HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
    g_pSetLayeredWindowAttributes = (lpfnSetLayeredWindowAttributes)
                        GetProcAddress(hUser32, "SetLayeredWindowAttributes");
}


void CControlSK::FreeResources()
{
    if (m_hBitmap)
        ::DeleteObject (m_hBitmap);
    m_hBitmap = NULL;
}

DWORD CControlSK::SetBitmap(int nBitmap)
{
    HBITMAP    hBitmap       = NULL;
    HINSTANCE  hInstResource = NULL;
    
    // Find correct resource handle
    hInstResource = AfxFindResourceHandle(MAKEINTRESOURCE(nBitmap), RT_BITMAP);
    
    // Load bitmap In
    hBitmap = (HBITMAP)::LoadImage(hInstResource, MAKEINTRESOURCE(nBitmap),
                                   IMAGE_BITMAP, 0, 0, 0);
    
    return SetBitmap(hBitmap);
}

DWORD CControlSK::SetBitmap(HBITMAP hBitmap)
{
    int nRetValue;
    BITMAP  csBitmapSize;
    
    // Free any loaded resource
    FreeResources();
    
    if (hBitmap)
    {
        m_hBitmap = hBitmap;
        
        // Get bitmap size
        nRetValue = ::GetObject(hBitmap, sizeof(csBitmapSize), &csBitmapSize);
        if (nRetValue == 0)
        {
            FreeResources();
            return 0;
        }
        m_dwWidth = (DWORD)csBitmapSize.bmWidth;
        m_dwHeight = (DWORD)csBitmapSize.bmHeight;
    }
    
//    if (IsWindow(this->GetSafeHwnd()))
//        Invalidate();
    
    return 1;
    
}


DWORD CControlSK::SetBitmap(LPCTSTR lpszFileName)
{
    HBITMAP    hBitmap       = NULL;
    hBitmap = (HBITMAP)::LoadImage(0, lpszFileName, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    return SetBitmap(hBitmap);
}

BOOL CControlSK::OnEraseBkgnd(CRect & rect, CDC* pDC) 
{
//	CString msg = "CControlSK::OnEraseBkgnd\r\n";OutputDebugString(msg);
    BOOL bRetValue = TRUE;;//= CDialog::OnEraseBkgnd(pDC);
    if (!m_hBitmap)
        return bRetValue;

//    CRect   rect;
//    GetClientRect(rect);
//	pDC->FillSolidRect(rect, RGB(255,255,255));
//	CRect rect2(0,0,500,500);
//	pDC->FillSolidRect(rect2, RGB(255,255,255));
//	return TRUE;
    
    CDC dc;
    dc.CreateCompatibleDC(pDC);

    HBITMAP    pbmpOldBmp = NULL;

    pbmpOldBmp = (HBITMAP)::SelectObject(dc.m_hDC, m_hBitmap);
    
    if ( m_loStyle == LO_DEFAULT || m_loStyle == LO_RESIZE )
    {
        pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
    }
    else if (m_loStyle == LO_TILE)
    {
        int ixOrg, iyOrg;

        for (iyOrg = 0; iyOrg < rect.Height(); iyOrg += m_dwHeight)
        {
            for (ixOrg = 0; ixOrg < rect.Width(); ixOrg += m_dwWidth)
            {
                pDC->BitBlt (ixOrg, iyOrg, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
            }
        }
    }
    else if (m_loStyle == LO_CENTER)
    {
        int ixOrg = (rect.Width() - m_dwWidth) / 2;
        int iyOrg = (rect.Height() - m_dwHeight) / 2;
        
        pDC->BitBlt(ixOrg, iyOrg, rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);
    }
    else if ( m_loStyle == LO_STRETCH)
    {
        pDC->StretchBlt(0, 0, rect.Width(), rect.Height(), &dc, 0, 0, m_dwWidth, m_dwHeight, SRCCOPY);
    }
    
    ::SelectObject(dc.m_hDC, pbmpOldBmp);

    return bRetValue;
}

//void
//CControlSK::OnSize(UINT nType, int cx, int cy) 
//{
//	CDialog::OnSize(nType, cx, cy);
//	
//	if (m_hBitmap != NULL)
//	{
//		Invalidate();
//	}	
//}

void
CControlSK::SetStyle(CWnd * hWnd, LayOutStyle style)
{
    m_loStyle = style;
    if(m_loStyle == LO_RESIZE && m_hBitmap)
    {
        hWnd->SetWindowPos(0, 0, 0, m_dwWidth, m_dwHeight, SWP_NOMOVE | SWP_NOREPOSITION );
    }
}
