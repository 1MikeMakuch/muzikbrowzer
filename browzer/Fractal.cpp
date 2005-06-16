// FractalDlg.cpp : implementation file
//

#include "stdafx.h"
//#include "MfcPlayer.h"
#include "Fractal.h"
#include "MBGlobals.h"
#include "MyString.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FractalDlg dialog



FractalDlg::FractalDlg(CRect rect, CWnd* pParent /*=NULL*/)
	: CDialog(FractalDlg::IDD, pParent), m_rect(rect), m_BrushWhite(RGB(255,255,255))
{
	//{{AFX_DATA_INIT(FractalDlg)

	//}}AFX_DATA_INIT

	MyUtil::seed();
	abort = 0;
	usestarts=0;
    

}
FractalDlg::~FractalDlg() {
}


void FractalDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FractalDlg)
	DDX_Control(pDX, IDC_GO, m_Go);
	DDX_Control(pDX, IDC_CANCEL, m_Cancel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FractalDlg, CDialog)
	//{{AFX_MSG_MAP(FractalDlg)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	ON_BN_CLICKED(IDC_GO, OnGo)
	//}}AFX_MSG_MAP

	ON_WM_PAINT()
	ON_WM_TIMER()
//	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()	

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FractalDlg message handlers


BOOL FractalDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int maxx = GetSystemMetrics(SM_CXMAXIMIZED);
	int maxy = GetSystemMetrics(SM_CYMAXIMIZED);

	int x,y;

	width = maxx / 2;
	height = maxy / 2;
	x = width / 2;
	y = height / 2;

	MoveWindow(x,y,width,height,TRUE);
	GetClientRect(crect);
	CRect buttonrect;
	m_Go.GetWindowRect(buttonrect);

	width = crect.Width();
	height = crect.Height();
	height -= buttonrect.Height() + 6;

	x = (width / 2) - (buttonrect.Width() +5);
	y = height +5;
	m_Go.MoveWindow(x,y,buttonrect.Width(),buttonrect.Height(),TRUE);
	x += buttonrect.Width() + 10;
	m_Cancel.MoveWindow(x,y,buttonrect.Width(),buttonrect.Height(),TRUE);

	

	white = RGB(255,255,255);
	black = RGB(0,0,0);
	green = RGB(0,255,0);
	done=0;

	init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
void FractalDlg::init() {
	int x,y;
	for(x = 0 ; x < width ; x++) {
		for(y = 0 ; y < height ; y++) {
			screen[x][y] = 0;
		}
	}
	x = width/2;
	y = height/2;
	screen[x][y] = 1;
}
void
FractalDlg::OnPaint() {
	CPaintDC dc(this); 
	CDC *cdc = GetDC();
	CRect crect;
	GetClientRect(crect);
	crect.bottom;// -= 20;
	CBrush br(black);
	cdc->FillRect(crect, &br);
	int x,y;
	for(x = 0 ; x < width ; x++) {
		for(y = 0 ; y < height ; y++) {
			if (screen[x][y]) {
				cdc->SetPixel(x,y,white);
			}
		}
	}
	ReleaseDC(cdc);
	m_Go.RedrawWindow();
	m_Cancel.RedrawWindow();
	
}
BOOL FractalDlg::OnEraseBkgnd(CDC* pDC) {
	CRect crect;
	GetClientRect(crect);
	crect.bottom;// -= 20;
	CBrush br(black);
	pDC->FillRect(crect, &br);
	int x,y;
	for(x = 0 ; x < width ; x++) {
		for(y = 0 ; y < height ; y++) {
			if (screen[x][y]) {
				pDC->SetPixel(x,y,white);
			}
		}
	}
	m_Go.RedrawWindow();
	m_Cancel.RedrawWindow();
//	CRect drect(crect);
//	drect.bottom += 18;
//	drect.top = drect.bottom - 18;
//	pDC->DrawText( "Click on screen to change origin",drect,DT_LEFT);
	return TRUE;
}
int
FractalDlg::frac(const int x, const int y) {
	if (x <= 0 || y <= 0) return 0;

	if (
		screen[x+1][y]		||
		screen[x  ][y+1]	||
		screen[x+1][y+1]	||
		screen[x-1][y]		||
		screen[x  ][y-1]		||
		screen[x-1][y-1]
		) {
		return 1;
	}
	return 0;
}

void
FractalDlg::move(int & x, int & y) {
	int dx = MyUtil::random(2);
	int dy = MyUtil::random(2);
//	if (dx == 0)
//		x--;
//	else 
//		x++;
//	if (dy == 0)
//		y--;
//	else 
//		y++;
	int xy = MyUtil::random(2);
	if (xy == 0) {
	if (dx == 0)
		x--;
	else 
		x++;
	} else {
	if (dy == 0)
		y--;
	else 
		y++;
	}
	if (x < 1) x = 1;
	if (y < 1) y = 1;
	if (x >= width) x = width-1;
	if (y >= height) y = height -1;
}
class ThreadParams {
public:
	FractalDlg * dlg;
};
static UINT
FracThread(LPVOID pParam) {
    ThreadParams *tp = (ThreadParams *)pParam;
	tp->dlg->doit();
    return 0;
}
void FractalDlg::OnGo() {
	init();
	ThreadParams tp;
	tp.dlg = (FractalDlg*)this;
	AfxBeginThread(FracThread, (LPVOID) &tp);
}

void FractalDlg::OnCancel() {
	abort = 1;
	CDialog::OnCancel();
}
void
FractalDlg::doit() {
	m_Go.EnableWindow(FALSE);
	m_Cancel.RedrawWindow();
	int x = 0;
	int y = 0;
	time_t start = time(NULL);
	CDC * cdc = GetDC();
	OnEraseBkgnd(cdc);
	done = 0;
	int lastx;
	int lasty;
	while (!abort && !done) {
		int xc ;
		int yc ;
		if (usestarts) { // use clicked point for start
			x = startx;
			y = starty;
		} else { // use random start
			xc = MyUtil::random(width);
			yc = MyUtil::random(height);
			int side = MyUtil::random(4);
			if (side == 0){
				x = 1;
				y = yc;
			} else if (side == 1){
				x = xc;
				y = 1;
			} else if (side == 2){
				x = width-6;
				y = yc;
			} else {
				x = xc;
				y = height-6;
			}
		}
		// x,y is now the starting point
//		if (usestarts && lastx != x && lasty != y)
//			setOrigin(cdc,lastx,lasty,black);
		lastx = x;
		lasty = y;
//		if (usestarts)
//			setOrigin(cdc,x,y,white);
		while (!frac(x,y)) {
			if (abort)
				return;
			move(x,y);
		}
		screen[x][y] = 1;
		cdc->SetPixel(x,y,white);
		if (x < 3 || (width -x) < 3 ||
			y < 3 || (height -y) < 3) {
			done=1;
		}
	}
//	usestarts=0;
	ReleaseDC(cdc);
	m_Go.EnableWindow(TRUE);
	RedrawWindow();
	m_Go.RedrawWindow();
	m_Cancel.RedrawWindow();
}
void FractalDlg::setOrigin(CDC * cdc, int x,int y,COLORREF clr) {
//	if ((width - x) < 5) x = width-5;
//	if ((height -y) < 5) y = height -5;
	cdc->SetPixel(x,y,clr);
	cdc->SetPixel(x+1,y,clr);
	cdc->SetPixel(x,y+1,clr);
	cdc->SetPixel(x+1,y+1,clr);

	cdc->SetPixel(x+2,y,clr);
	cdc->SetPixel(x+2,y+1,clr);
	cdc->SetPixel(x+2,y+2,clr);
	cdc->SetPixel(x+1,y+2,clr);
	cdc->SetPixel(x,y+2,clr);

}

void FractalDlg::OnMouseMove(UINT nFlags, CPoint point) {
	int x = point.x;
	int y = point.y;
	int dx;
	int dy;
	CRect r;
	GetClientRect(r);
	r.bottom -= 20;
	if (r.PtInRect(point)) {
		dx = abs((width/2) - x);
		dy = abs((height/2) - y);

		if (dx > dy) {
			if (x < (width/2)) {
				x = 1;
			} else {
				x = width -6;
			}
		} else {
			if (y < (height/2)) {
				y = 1;
			} else {
				y = height -10;
			}
		}

		startx = x;
		starty = y;
		//startx = point.x;
		//starty = point.y;
		usestarts=1;
	}
}


void FractalDlg::OnLButtonDown(UINT nFlags, CPoint point) {
	int x = point.x;
	int y = point.y;
	int dx;
	int dy;
	CRect r;
	GetClientRect(r);
	r.bottom -= 20;
	if (r.PtInRect(point)) {
		dx = abs((width/2) - x);
		dy = abs((height/2) - y);

		if (dx > dy) {
			if (x < (width/2)) {
				x = 1;
			} else {
				x = width -1;
			}
		} else {
			if (y < (height/2)) {
				y = 1;
			} else {
				y = height -1;
			}
		}

		startx = x;
		starty = y;
//		startx = point.x;
//		starty = point.y;
		usestarts=1;
	}
}

