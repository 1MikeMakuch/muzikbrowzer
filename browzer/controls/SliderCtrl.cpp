/////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "SliderCtrl.h"
#include "windows.h"
#include "PlayerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SliderCtrl

SliderCtrl::SliderCtrl()
{
	m_dcBk.m_hDC = NULL;
	ThumbColor = NULL;
	ThumbColorSelected = NULL;
}

SliderCtrl::~SliderCtrl()
{	
	DeleteObject(m_dcBk.SelectObject(&m_bmpBkOld));
	DeleteDC(m_dcBk);			
}


BEGIN_MESSAGE_MAP(SliderCtrl, CSliderCtrl)
	//{{AFX_MSG_MAP(SliderCtrl)
		ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
		ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SliderCtrl message handlers
//-------------------------------------------------------------------
//
void SliderCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
//
// Description	:	Sent by the slider control to notify the parent window 
//					about drawing operations. This notification is sent in 
//					the form of a WM_NOTIFY message.
// Parameters	:	pNMHDR - a pointer to a NM_CUSTOMDRAW structure.
//					pResult - value depends on the current drawing state.
{
	LPNMCUSTOMDRAW lpcd = (LPNMCUSTOMDRAW)pNMHDR;
	CDC *pDC = CDC::FromHandle(lpcd->hdc);
	DWORD dwStyle = this->GetStyle();
	switch(lpcd->dwDrawStage) {
		case CDDS_PREPAINT:
			*pResult = CDRF_NOTIFYITEMDRAW;
			break;
		case CDDS_ITEMPREPAINT: {
//			switch(lpcd->dwItemSpec) {
//				case TBCD_TICS:
//					*pResult = CDRF_SKIPDEFAULT;
//					break;
//				case TBCD_THUMB: {
//					CRect rc;
//					GetThumbRect(&rc);
//					CBrush br(thePlayer->config().getColorBkPanel());
//					pDC->FillRect(rc, &br);
//					CBrush br2(thePlayer->config().getColorTxPanel());
//					pDC->FrameRect(rc, &br2);
//					*pResult = CDRF_SKIPDEFAULT;
//				} break;
//				case TBCD_CHANNEL: {
//					CRect rc;
//					GetChannelRect(&rc);
//					CBrush br(thePlayer->config().getColorTxPanel());
//					pDC->FrameRect(rc, &br);
//					*pResult = CDRF_SKIPDEFAULT;
//				} break;
//				default: {
//					int x = lpcd->dwItemSpec;
//				}
//			}
			*pResult = CDRF_NOTIFYPOSTPAINT | 
				CDRF_SKIPDEFAULT;
		} break;
		case CDDS_ITEMPOSTPAINT: {
			*pResult = CDRF_SKIPDEFAULT;
			switch(lpcd->dwItemSpec) {
				case TBCD_TICS:
					*pResult = CDRF_SKIPDEFAULT;
					break;
				case TBCD_THUMB: {
		//			if (lpcd->uItemState & CDIS_SELECTED)
		//				*pResult = CDRF_NOTIFYPOSTPAINT;
		//			else
		//				*pResult = CDRF_NOTIFYPOSTPAINT;
					CRect rc;
					GetThumbRect(&rc);
					CBrush br(thePlayer->config().getColorBkPanel());
					pDC->FillRect(rc, &br);
					CBrush br2(thePlayer->config().getColorTxPanel());
					pDC->FrameRect(rc, &br2);
					*pResult = CDRF_SKIPDEFAULT;
				} break;
				case TBCD_CHANNEL: {
					CRect rc;
					GetChannelRect(&rc);
					CBrush br(thePlayer->config().getColorTxPanel());
					pDC->FrameRect(rc, &br);
					*pResult = CDRF_SKIPDEFAULT;
					
					GetClientRect(rc);
					CBrush br2(thePlayer->config().getColorBkPanel());
					pDC->FrameRect(rc, &br2);
				} break;
				default: {
					int x = lpcd->dwItemSpec;
				}
			}
			*pResult = //CDRF_NOTIFYPOSTPAINT | 
				CDRF_SKIPDEFAULT;
		} break;
		default: {
			int x = lpcd->dwDrawStage;
		}
	}
}

BOOL SliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}


void SliderCtrl::SetThumbColors(COLORREF face, COLORREF highlight)
{
	ThumbColor = face;
	ThumbColorSelected = highlight;
}
