// PictureStatic.cpp : implementation file
//

#include "stdafx.h"
#include "PictureStatic.h"
#include "MyString.h"
#include "PlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// PictureStatic

PictureStatic::PictureStatic(bool square) : m_bytes(NULL), m_ploaded(FALSE)

{
	m_Picture = new CPicture(this, square);
}

PictureStatic::~PictureStatic()
{
	delete m_Picture;
}


BEGIN_MESSAGE_MAP(PictureStatic, CStatic)
	//{{AFX_MSG_MAP(PictureStatic)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// PictureStatic message handlers

void PictureStatic::OnPaint() 
{
//	OutputDebugString("CPictureStatic::OnPaint\r\n");

	CPaintDC paintdc(this);
    CDC * cdc = GetDC();
    if (cdc == NULL) {
        return;
    }
    CRect rect;
    GetClientRect(rect);

	if (m_ploaded)
		m_Picture->Draw(cdc);

    CRgn Region;
    Region.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
    cdc->SelectClipRgn(&Region);

    Region.DeleteObject();
    cdc->SelectClipRgn(NULL);
    ReleaseDC(cdc);
	
	// Do not call CStatic::OnPaint() for painting messages
}

void
PictureStatic::load(BYTE * data, size_t size) {

//	m_bytes = data;
	m_Picture->LoadFromBuffer(data, size);
//	delete [] data;
	m_ploaded = TRUE;
	RedrawWindow();
}
void
PictureStatic::load(LPTSTR type, LPTSTR resource) {
	m_Picture->Load(type, resource);
	m_ploaded = TRUE;
	RedrawWindow();
}
void
PictureStatic::unload() {
	m_Picture->UnLoad();
//	if (m_bytes) {
//		delete [] m_bytes;
//		m_bytes = NULL;
//	}
	m_ploaded = FALSE;	
	blank();
}
void
PictureStatic::blank() {
	CPaintDC paintdc(this);
    CDC * cdc = GetDC();
    if (cdc == NULL) {
        return;
    }
    CRect rect;
    GetClientRect(rect);

	COLORREF crbg = thePlayer->config().getColorBkNormal();
	CBrush brush(crbg);
	cdc->FillRect(rect, &brush);
 
    ReleaseDC(cdc);
}