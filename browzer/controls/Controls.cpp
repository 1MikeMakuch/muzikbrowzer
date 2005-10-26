
#include "stdafx.h"
#include "Controls.h"
#include "MyString.h"

Control::Control(const int irow, const int icol, const CString & desc, 
	CWnd *cwdctrl, CWnd*cwndlabel) : row(irow),
	col(icol), hpos(-1),
	ctrl(cwdctrl), label(cwndlabel),
	height(0), width(0),
	widthpct(0), heightpct(0),
	labelwidth(0), labelheight(0), m_desc(desc), enabled(1),x(0),y(0)
{
	if (ctrl) {
		ctrl->GetWindowRect(rectctrl);
	} else {
		rectctrl = CRect(0,0,0,0);
	}
	if (label) {
		label->GetWindowRect(rectlabel);
	} else {
		rectlabel = CRect(0,0,0,0);
	}
	width = rectctrl.Width();
	height = rectctrl.Height();
	setRects();
}

void Control::setRects() {
	return;
	rectctrl.BottomRight().x = rectctrl.TopLeft().x + width;
	rectctrl.BottomRight().y  = rectctrl.TopLeft().y + height;
	if (label) {
		rectlabel.BottomRight().x = rectlabel.TopLeft().x + width;
		rectlabel.BottomRight().y  = rectlabel.TopLeft().y + labelheight;
		rectctrl.TopLeft().y = rectlabel.BottomRight().y + 1;
		rectctrl.BottomRight().y = rectctrl.TopLeft().y + height;
	}
	rectall.UnionRect(rectctrl,rectlabel);
}

void Control:: dump(CString txt) {
	CString msg;
	AutoBuf buf(1000);
	sprintf(buf.p,"%s %15s r=%d c=%d w=%04d h=%04d\r\n",
		txt, m_desc, row, col, width, height);
//	OutputDebugString(buf.p);
}
Controls::Controls() : m_cwnd(NULL), numRows(0), m_NumControls(0) 
{
}
void Controls::init(CWnd * w) {
	m_cwnd = w;
	memset(m_RowMaxY, 0, sizeof(m_RowMaxY));
	memset(m_RowTop, 0, sizeof(m_RowTop));
	//m_HDWPHandle = BeginDeferWindowPos(m_NumControls);
}
Controls::~Controls() {
	FreeEm();
}
void Controls::FreeEm() {
	POSITION pos;
	WORD dlgid;
	Control *cp;
	for (pos = m_controls.GetStartPosition() ; pos != NULL ; ) {
		m_controls.GetNextAssoc(pos, dlgid, (void*&)cp);
		if (cp) {
			delete cp;
		}
	}
}
void Controls::movem() {
	//EndDeferWindowPos(m_HDWPHandle);
}
void Controls::add(const int row, const int col, const CString & desc, 
		int id, int label) {
	CWnd * cw = m_cwnd->GetDlgItem(id);
	CWnd * cl = NULL;
	Control * cp;
	if (label) {
		cl = m_cwnd->GetDlgItem(label);
	}
	cp = new Control(row, col, desc, cw, cl); 
	m_controls.SetAt((WORD)id, cp);
}
void Controls::remove(int id) {
	CWnd * cw = m_cwnd->GetDlgItem(id);
//		Control * cp;
	if (cw) {
		cw->DestroyWindow();
	}
//		m_controls.RemoveKey((WORD)id);
//		delete cw;
};
Control * Controls::getObj(int id) {
	void * p;
	m_controls.Lookup(id, p);
	return (Control*)p;
}
Control * Controls::getObj(int row, int col) {
	Control * cp;
	WORD dlgid;
	POSITION pos;
	int rowMaxHeight = 0;
	int found = 0;
	for (pos = m_controls.GetStartPosition();
			pos != NULL && found == 0;) {
		cp = NULL;
		m_controls.GetNextAssoc(pos, dlgid, (void*&)cp);
		if (cp->row == row && cp->col == col) {
			found = 1;
		}
	}
	if (found) {
		return cp;
	} else {
		return NULL;
	}
		
}
// x, y must be dialog client coordinates
void Controls::move(Control * cp, const int x, const int y,
					const int row, const int col) {
	int cx = x;
	int cy = y;
	cp->x = x;
	cp->y = y;
	if (cp->label) {
		cp->label->MoveWindow(cx,cy,cp->width,cp->labelheight,TRUE);
		//::DeferWindowPos(m_HDWPHandle, cp->label->m_hWnd, NULL,
		//	cx,cy,cp->width,cp->labelheight,FALSE);
		cy += cp->labelheight;
	}

	cp->ctrl->MoveWindow(cx,cy,cp->width,cp->height, TRUE);

//	::DeferWindowPos(m_HDWPHandle, cp->ctrl->m_hWnd, NULL,
//			cx,cy,cp->width,cp->height,FALSE);


}
BOOL Controls::CanMove(Control * cp, const int cx, const int cy) {
	// cx,cy in screen coords

	CRect candrect(cp->rectall);

	// move candrect to x,y
	int dx,dy;
	dx = cx - candrect.TopLeft().x;
	dy = cy - candrect.TopLeft().y;

	candrect.OffsetRect(CPoint(dx,dy));

	// make sure it's on the dialog
	CRect intersection;
	BOOL i = intersection.IntersectRect(candrect, dialogrectreduced);
	if (i == 0 || intersection != candrect) {
		return FALSE;
	}

	// if previous row has been set don't go above it
	if (cp->row) {
		if (candrect.TopLeft().y <= m_RowMaxY[cp->row - 1][cp->col]) {
			return FALSE;
		}
	}

	// now see if it collides with any other controls

	WORD dlgid;
	POSITION pos;
	int found = 0;
	Control * cpi;
	for (pos = m_controls.GetStartPosition(); pos != NULL;) {
		m_controls.GetNextAssoc(pos, dlgid, (void*&)cpi);
		if (cp != cpi) {
			i = intersection.IntersectRect(candrect, cpi->rectall);
			if (i != 0) {
				return FALSE;
			}
		}
	}
	return TRUE;
}
void Controls::findPos(const CPoint & moveby, Control * cp, 
		int & cx, int & cy, int borderhorz, int bordervert) {

	int x, y, lastx, lasty;
	BOOL can;
	if (cp->col == 0) { // walk up 
		MoveToBottom(cp);
		x = cp->rectall.TopLeft().x;
		y = cp->rectall.TopLeft().y;
		// screen coords for CanMove

		// 1st move up
		lasty = y;
		while (moveby.y && (can = CanMove(cp,x,y)) == TRUE) {
			lasty = y;
			y += moveby.y;
		}
		cy = y = lasty;		

		// for each horizontal move try moving up
		lastx = x;
		while (moveby.x && (can = CanMove(cp,x,y)) == TRUE) {
			lastx = x;
			x += moveby.x;

			lasty = y;
			while (moveby.y && (can = CanMove(cp,x,y)) == TRUE) {
				lasty = y;
				y += moveby.y;
			}
			cy = y = lasty;

		}
		cy += bordervert;
		if (cp->m_desc == "genre") {
			cy += bordervert * 2;
		}
	} else { // start at m_RowTop
		y = m_RowTop[cp->row];
		if (moveby.x < 0) {
			x = dialogrectreduced.TopLeft().x;
		} else if (moveby.x > 0) {
			x = dialogrectreduced.BottomRight().x - cp->width;
		} else if (moveby.x == 0) {
			x = (dialogrectreduced.Width() / 2) - (cp->width / 2);
			x += dialogrectreduced.TopLeft().x;
		}
		lastx = x;
		while (moveby.x && ((can = CanMove(cp,x,y)) == FALSE)
				&& -1 < x && x < dialogrectreduced.BottomRight().x) {
			lastx = x;
			x += -moveby.x;
		}
		cy = y;
	}
	// center
	if (cp->hpos == 0) {
		x = (dialogrectreduced.Width() / 2) - (cp->width / 2);
		x += dialogrectreduced.TopLeft().x;
		lastx = x;
	}
	cx = x = lastx ;

	// return dialog client coords in prep for move
	makeClientCoords(cx,cy);
	if (cp->hpos < 0) {
		cx += borderhorz;
	} else if (cp->hpos > 0) {
		cx -= borderhorz;
	}

}
void Controls::MoveEmOffScreen() {
	WORD dlgid;
	POSITION pos;
	int found = 0;
	Control * cpi;
	CRect rect;
	int x,y;
	x = dialogrect.Width() / 2;
	y = dialogrect.Height() + 1;
	for (pos = m_controls.GetStartPosition(); pos != NULL;) {
		m_controls.GetNextAssoc(pos, dlgid, (void*&)cpi);
		// x,y in dialog client coords
		move(cpi, x,y);
	}
}
void Controls::MoveToBottom(Control *cp) {
	int x,y;
	// xxx: issue here y should be incremented by
	// different between widths of dialogrect and reduced
	x = (dialogrect.Width() / 2) - (cp->width / 2);
	y = (dialogrectreduced.Height() - cp->rectall.Height() - 3);
	// x,y in dialog client coords
	move(cp, x,y);
}
void Controls::makeClientCoords(int & x, int & y) {
	CRect s(x,y,x+1,y+1);
	m_cwnd->ScreenToClient(s);
	x = s.TopLeft().x;
	y = s.TopLeft().y;
}
