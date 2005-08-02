// TransparentDialogDlg.cpp : implementation file
//MFC Transparent CDialog windows.

/* Copyright (C) 2002 by Nic Wilson  http://www.nicsoft.com.au
Written by Nic Wilson nicw@bigpond.net.au
All rights reserved

This is free software.
This code may be used in compiled form in any way you desire. This  
file may be redistributed unmodified by any means PROVIDING it is   
not sold for profit without the authors written consent, and   
providing that this notice and the authors name and all copyright   
notices remains intact. If the source code in this file is used in   
any  commercial application then a statement along the lines of   
Portions Copyright © 2002 Nic Wilson MUST be included in   
the startup banner, "About" dialog & printed documentation.   

No warrantee of any kind, expressed or implied, is included with this
software; use at your own risk, responsibility for damages (if any) to
anyone resulting from the use of this software rests entirely with the
user.

Release Date and Version:
Version: 1.0 December 2002



USAGE:

You should be able to use this demo as a skeleton application for your 
own project.

A standard CDialog MFC EXE application was created with the wizard and
apart from the standard  supplied code, the following functions were overridden.


	void OnClose() 
	Used to clean up some resources on exit.
	
	void OnPaint() 
	Used to paint the bimap into the region.

	void OnSize(UINT nType, int cx, int cy)
	Used to create the region and position and resize the dialog.

	void OnLButtonDown(UINT nFlags, CPoint point)
	To allow the dialog to be dragged by clicking anywhere.	


A button was created to allow the user to close the dialog.
A timer was  created to demonstrate auto closing after a period of time.

The image is a normal bmp file that is loaded by the app and ued to paint
the dialog.  TRANSPARENTCOLOR is defined in the dialog class header as bright
purple (RGB(255, 0, 255), but this could be changed to any colour you like.

The dialog window is auto sized to the size of the loaded bitmap then a region
created based on the transparent colour.  The dialog is then positioned in the 
centre of the current screen.   This was done as an example for those who want
to use this application as a splash windows.

The button serves no real purpose and was made a close button simply to demonstrate
using a button.

*/


#include "stdafx.h"
//x#include "TransparentDialog.h"
#include "TransparentDialogDlg.h"
#include "util/Misc.h"
#include "FileUtils.h"
#include "MBGlobals.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTransparentDialogDlg dialog

CTransparentDialogDlg::CTransparentDialogDlg(CWnd* pParent /*=NULL*/,
											 COLORREF xclr)
	: CDialog(CTransparentDialogDlg::IDD, pParent), m_Xclr(xclr)
{
	//{{AFX_DATA_INIT(CTransparentDialogDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
//x	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hBitmap = NULL;
	Create(IDD);
//	m_start = CTime::GetCurrentTime();
}

void CTransparentDialogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTransparentDialogDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTransparentDialogDlg, CDialog)
	//{{AFX_MSG_MAP(CTransparentDialogDlg)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
CTransparentDialogDlg::~CTransparentDialogDlg() {
	if (m_hBitmap != NULL)
		DeleteObject(m_hBitmap);	//not really need but what the heck.
	m_hBitmap = NULL;
}
/////////////////////////////////////////////////////////////////////////////
// CTransparentDialogDlg message handlers

BOOL CTransparentDialogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
//x	SetIcon(m_hIcon, TRUE);			// Set big icon
//x	SetIcon(m_hIcon, FALSE);		// Set small icon
//x	SetTimer(1, 10000, NULL);
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTransparentDialogDlg::OnExit() 
{
	if (m_hBitmap != NULL)
		DeleteObject(m_hBitmap);	//not really need but what the heck.
	m_hBitmap = NULL;
}

void CTransparentDialogDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	if (m_hBitmap != NULL)
		DeleteObject(m_hBitmap);	//not really need but what the heck.
	m_hBitmap = NULL;
	CDialog::OnClose();
}

void CTransparentDialogDlg::OnPaint() 
{
	// device context for painting
	CPaintDC dc(this); 
	//Create a memory DC
	HDC hMemDC = ::CreateCompatibleDC(NULL);
	//Select the bitmap in the memory dc.
	HBITMAP oldbitmap = (HBITMAP)SelectObject(hMemDC, m_hBitmap);
	//Copy the memory dc into the screen dc
	::BitBlt(dc.m_hDC, 0, 0, m_Bitmap.bmWidth, m_Bitmap.bmHeight, hMemDC, 0, 0, SRCCOPY);
	//Delete the memory DC and the bitmap
	
	SelectObject(hMemDC, oldbitmap);

	::DeleteDC(hMemDC);
	CDialog::OnPaint();
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.

void CTransparentDialogDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	// Load the image
//	m_hBitmap = (HBITMAP)LoadImage(GetModuleHandle(NULL), "Image.bmp", 
//		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	if (m_hBitmap != NULL)
		DeleteObject(m_hBitmap);	//not really need but what the heck.
	m_hBitmap = NULL;

#pragma hack 
	// this is temp just for dev pusposes. Final splash
	//should be compiled in.
	CString splash = "splash.bmp";
	if (FileUtil::IsReadable(splash)) {
		m_hBitmap = (HBITMAP) LoadImage (NULL,
		splash, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR
		|LR_LOADFROMFILE);
	} else {
		m_hBitmap = (HBITMAP) LoadImage (AfxGetInstanceHandle(), 
			MAKEINTRESOURCE(IDB_SPLASH3), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR);
	}

	if (m_hBitmap == NULL)
	{
		CString msg = MBFormatError(GetLastError());
		MessageBox(msg);
		return;
	}
	//Get information about the bitmap..
	GetObject(m_hBitmap, sizeof(m_Bitmap), &m_Bitmap);	// Get info about the bitmap 
	// Put the bitmap into a memory device context
	CPaintDC dc(this);
	//get a memory dc object
	CDC dcMem;
	//create a compatible dc
	dcMem.CreateCompatibleDC(&dc);	// Select the bitmap into the in-memory DC
	//Select the bitmap into the dc
	CBitmap* pOldBitmap = dcMem.SelectObject(CBitmap::FromHandle(m_hBitmap));
	//Create a couple of region objects.
	CRgn crRgn, crRgnTmp;
	//create an empty region
	crRgn.CreateRectRgn(0, 0, 0, 0);
	//Create a region from a bitmap with transparency colour of Purple
	COLORREF crTransparent = m_Xclr;	
	int iX = 0;
	for (int iY = 0; iY < m_Bitmap.bmHeight; iY++)
	{
		do
		{
			//skip over transparent pixels at start of lines.
			while (iX <= m_Bitmap.bmWidth && dcMem.GetPixel(iX, iY) == crTransparent)
				iX++;
			//remember this pixel
			int iLeftX = iX;
			//now find first non transparent pixel
			while (iX <= m_Bitmap.bmWidth && dcMem.GetPixel(iX, iY) != crTransparent)
				++iX;
			//create a temp region on this info
			crRgnTmp.CreateRectRgn(iLeftX, iY, iX, iY+1);
			//combine into main region.
			crRgn.CombineRgn(&crRgn, &crRgnTmp, RGN_OR);
			//delete the temp region for next pass (otherwise you'll get an ASSERT)
			crRgnTmp.DeleteObject();
		}while(iX < m_Bitmap.bmWidth);
		iX = 0;
	}
	dcMem.SelectObject(pOldBitmap);	// Put the original bitmap back (prevents memory leaks)
	dcMem.DeleteDC();
	//Centre it on current desktop
	SetWindowRgn(crRgn, TRUE);
	iX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (m_Bitmap.bmWidth / 2);
	iY = (GetSystemMetrics(SM_CYSCREEN) / 2) - (m_Bitmap.bmHeight / 2);
	SetWindowPos(NULL, /*&wndTopMost,*/ iX, iY, m_Bitmap.bmWidth, m_Bitmap.bmHeight,
		NULL); 

	// Free resources.

	crRgn.DeleteObject();
}
