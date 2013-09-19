// GetBlackOutDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GetBlackOutDlg.h"
#include "Board.h"
#include <math.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static int distance(CPoint p1, CPoint p2) {
	int xd = p1.x - p2.x;
	int yd = p1.y - p2.y;
	double d = sqrt((xd * xd) + (yd * yd));
	return int(d);
}

static CPoint topright(CRect & rect) {
	CPoint p;
	p.x = rect.right;
	p.y = rect.top;
	return p;
}
static CPoint topleft(CRect & rect) {
	CPoint p(rect.TopLeft());
	return p;
}
static CPoint bottomright(CRect & rect) {
	CPoint p(rect.BottomRight());
	return p;
}
static CPoint bottomleft(CRect & rect) {
	CPoint p;
	p.x = rect.left;
	p.y = rect.bottom;
	return p;
}

/////////////////////////////////////////////////////////////////////////////
// CGetBlackOutDlg dialog

CGetBlackOutDlg::CGetBlackOutDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGetBlackOutDlg::IDD, pParent),
	mMoving(FALSE), mWon(FALSE), mAnimate(FALSE), mWinTimerId(0), 
	mystart(0), mTotMoves(0)
{
	mBoardStatic.SetDlg(this);
	//{{AFX_DATA_INIT(CGetBlackOutDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
//	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CGetBlackOutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGetBlackOutDlg)
	DDX_Control(pDX, IDC_STATIC1, mBoardStatic);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CGetBlackOutDlg, CDialog)
	//{{AFX_MSG_MAP(CGetBlackOutDlg)
	ON_WM_PAINT()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGetBlackOutDlg message handlers

BOOL CGetBlackOutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	resize();
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
void CGetBlackOutDlg::resize() {

	CRect cdialog;
	GetClientRect(cdialog);

	CRect wdialog;
	GetWindowRect(&wdialog);
	CPoint topleft = wdialog.TopLeft();
	CPoint br(topleft.x + mBoard.pwidth()+mBoard.pborder(),
		topleft.y + mBoard.pheight()+ 30 
		+ (mBoard.pper() * 2) + mBoard.pborder());
	wdialog.BottomRight() = br;
	MoveWindow(wdialog, TRUE);

	CWnd *p;
//	p = GetDlgItem(IDD_GETBLACKOUT_DIALOG);
	p = GetDlgItem(IDC_STATIC1);
	CRect staticrect;
	p->GetWindowRect(staticrect);

	CPoint ctl(0,0);
	CPoint cbr(mBoard.pwidth(), mBoard.pheight());
	staticrect.TopLeft() = ctl;
	staticrect.BottomRight() = cbr;
	p->MoveWindow(staticrect, TRUE);
	mBoard.mBoardRect = staticrect;
	CRect slot( 
		(mBoard.pborder() * 2) + mBoard.pper(),
		(mBoard.pheight()-3),
		(mBoard.pborder() * 3) + (mBoard.pper()*3),
		(mBoard.pheight())
		);

	mBoardStatic.setrect(slot);

	CRect btr;

	UpdateWindow();
}
COLORREF bkclr = RGB(255,255,255);
void CGetBlackOutDlg::drawall(CDC * cdc) {
	COLORREF clr ;
	int x,y,cx,cy,i;
	for(i = 0 ; i < mBoard.numpieces(); i++) {
		x = mBoard.pborder() + 
			(mBoard.piece(i)->xpos() * (mBoard.pper() + mBoard.pborder()));
		y = mBoard.pborder() + 
			(mBoard.piece(i)->ypos() * (mBoard.pper() + mBoard.pborder()));
		cx = (mBoard.piece(i)->width() * mBoard.pper());
		if (mBoard.piece(i)->width() == 2)
			cx += mBoard.pborder();

		cy = (mBoard.piece(i)->height() * mBoard.pper());
		if (mBoard.piece(i)->height() == 2)
			cy += mBoard.pborder();
			clr = RGB(255,0,0);
		if (i == 1) {
			clr = RGB(0,0,0);
		}

		if (mAnimate && i == 1) {
			if (mystart == 0) mystart = y;
			mystart += 1;

//			mBoardStatic.draw(x,mystart-1,cx,cy,bkclr);
			mBoardStatic.draw(cdc, x,mystart,cx,cy,clr);
			if (mystart >= mBoard.pheight()+3) {
				KillTimer(mWinTimerId);
				mAnimate = FALSE;
				mWon = TRUE;
				mWinTimerId =0;
				mBoardStatic.won(1);
				MessageBeep(MB_ICONEXCLAMATION);
				char buf[100];
				sprintf(buf, "You did it in %d moves.", mTotMoves);
				MessageBox(buf, "Congratulations", MB_OK);
			}
		} else if (mWon && i == 1) {
			mBoardStatic.draw(cdc, x,mBoard.pheight(),cx,cy,clr);
		} else {
			mBoardStatic.draw(cdc, x,y,cx,cy,clr);
			mBoard.piece(i)->mPieceRect = CRect(x,y,x+cx,y+cy);
		}

	}
	if (mAnimate) {
		static slept = 0;
		if (slept == 0) {
			slept = 1;
//			Sleep(1000);
		}
	}
				
}
void CGetBlackOutDlg::winner() {
	if (mWinTimerId != 0) return;
	if (mBoard.win()) {
		static set = 0;
		if (set == 0) {
			mWinTimerId = SetTimer(1, 100, NULL);
//			set = 1;
		}
	}
	return;
}
void CGetBlackOutDlg::OnTimer(UINT nIDEvent) 
{
	mAnimate = TRUE;
	RedrawWindow();
	CDialog::OnTimer(nIDEvent);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGetBlackOutDlg::OnPaint() 
{
	CDialog::OnPaint();
	CDC * dc = GetDC();
	drawall(dc);
	ReleaseDC(dc);
}

void CGetBlackOutDlg::OnMouseDown( UINT nflags, CPoint point)
{
	if (mWon) return;
	int x,y,cx,cy,i,piece;
	x=y=cx=cy=piece=0;
	for(i = 0 ; i < mBoard.numpieces(); i++) {
		x = mBoard.pborder() + 
			(mBoard.piece(i)->xpos() * (mBoard.pper() + mBoard.pborder()));
		y = mBoard.pborder() + 
			(mBoard.piece(i)->ypos() * (mBoard.pper() + mBoard.pborder()));
		cx = (mBoard.piece(i)->width() * mBoard.pper());
		if (mBoard.piece(i)->width() == 2)
			cx += mBoard.pborder();

		cy = (mBoard.piece(i)->height() * mBoard.pper());
		if (mBoard.piece(i)->height() == 2)
			cy += mBoard.pborder();
		CRect piecerect(x,y,x+cx,y+cy);
		if (piecerect.PtInRect(point)) {
			piece=i;
			i=mBoard.numpieces();
			mMoving = TRUE;
			mMouseStartPoint = point;
			mPieceMoving = piece;
			break;
		}
	}
	int moves = 0;
	if (mMoving) {
		int x,y,sx,sy,sx2,sy2;
		x=y=sx=sy=sx2=sy2=0;
		x = -mBoard.pper(); y = 0;
		if (mBoard.canmove(mPieceMoving,x,y)) {
			moves++;
			sx=x;sy=y;
		}
		x = mBoard.pper(); y = 0;
		if (mBoard.canmove(mPieceMoving,x,y)) {
			moves++;
			if (moves < 2) {
				sx=x;sy=y;
			} else {
				sx2=x;sy2=y;
			}
		}
		x = 0; y = mBoard.pper();
		if (mBoard.canmove(mPieceMoving,x,y)) {
			moves++;
			if (moves < 2) {
				sx=x;sy=y;
			} else {
				sx2=x;sy2=y;
			}
		}
		x = 0; y = -mBoard.pper();
		if (mBoard.canmove(mPieceMoving,x,y)) {
			moves++;
			if (moves < 2) {
				sx=x;sy=y;
			} else {
				sx2=x;sy2=y;
			}
		}

		if (moves == 1) {
			CPoint delta(point);
			delta += CPoint(sx,sy);
			OnMouseMove(0, delta);
			OnMouseUp(0,CPoint());
		} else if (moves == 2) {
			CRect piecerect(mBoard.piece(mPieceMoving)->mPieceRect);
			int deltax, deltay;deltax=deltay=0;
			if (abs(sx) == mBoard.pper() 
					&& abs(sx2) == mBoard.pper()) {
				CRect lhalf(piecerect);
				CRect rhalf(piecerect);
				lhalf.BottomRight().x -= lhalf.Width() / 2;
				rhalf.TopLeft().x = lhalf.BottomRight().x;
				if (lhalf.PtInRect(point)) {
					deltax = sx2;
				} else {
					deltax = sx;
				}
				CPoint delta(point);
				delta += CPoint(deltax,deltay);
				OnMouseMove(0, delta);
				OnMouseUp(0,CPoint());
			} else if (abs(sy) == mBoard.pper() 
					&& abs(sy2) == mBoard.pper()) {
				CRect thalf(piecerect);
				CRect bhalf(piecerect);
				thalf.BottomRight().y -= thalf.Height() / 2;
				bhalf.TopLeft().y = thalf.BottomRight().y;
				if (thalf.PtInRect(point)) {
					deltay = sy;
				} else {
					deltay = sy2;
				}
				CPoint delta(point);
				delta += CPoint(deltax,deltay);
				OnMouseMove(0, delta);
				OnMouseUp(0,CPoint());
			} else if (
					sx == mBoard.pper()
					&& sy2 == mBoard.pper()
					||
					sx2 == mBoard.pper()
					&& sy == mBoard.pper()
					) {
				if (distance(topright(piecerect),point)
					< distance(point, bottomleft(piecerect))) {
					deltay = sy2;
				} else {
					deltax = sx;
				}
				CPoint delta(point);
				delta += CPoint(deltax,deltay);
				OnMouseMove(0, delta);
				OnMouseUp(0,CPoint());		
			} else if (
				sx == -mBoard.pper()
				&& sy2 == mBoard.pper()
				||
				sx2 == -mBoard.pper()
				&& sy == mBoard.pper()
				) {
				if (distance(topleft(piecerect),point)
					< distance(point, bottomright(piecerect))) {
					deltay = sy2;
				} else {
					deltax = sx;
				}
				CPoint delta(point);
				delta += CPoint(deltax,deltay);
				OnMouseMove(0, delta);
				OnMouseUp(0,CPoint());		
			} else if (
				sx == mBoard.pper()
				&& sy2 == -mBoard.pper()
				||
				sx2 == mBoard.pper()
				&& sy == -mBoard.pper()
				) {
				if (distance(topleft(piecerect),point)
					< distance(point, bottomright(piecerect))) {
					deltax = sx;
				} else {
					deltay = sy2;
				}
				CPoint delta(point);
				delta += CPoint(deltax,deltay);
				OnMouseMove(0, delta);
				OnMouseUp(0,CPoint());	
			} else if (
				sx == -mBoard.pper()
				&& sy2 == -mBoard.pper()
				||
				sx2 == -mBoard.pper()
				&& sy == -mBoard.pper()
				) {
				if (distance(topright(piecerect),point)
					< distance(point, bottomleft(piecerect))) {
					deltax = sx;
				} else {
					deltay = sy2;
				}
				CPoint delta(point);
				delta += CPoint(deltax,deltay);
				OnMouseMove(0, delta);
				OnMouseUp(0,CPoint());	
			}			
		}
	}


}
void CGetBlackOutDlg::OnMouseUp( UINT nflags, CPoint point)
{
	mMoving = FALSE;
}
void CGetBlackOutDlg::OnMouseMove( UINT nflags, CPoint point)
{
	if (mMoving && mPieceMoving > -1 && mPieceMoving < mBoard.numpieces()) {
		CPoint delta = point - mMouseStartPoint;
		if (mBoard.canmove(mPieceMoving, delta.x, delta.y)) {
			mTotMoves++;
			mBoard.move(mPieceMoving, delta.x, delta.y);
			winner();
			mMouseStartPoint = point;
			RedrawWindow();
			mMoving = FALSE;
		} else {
			mMoving = FALSE;
		}
	}
}





