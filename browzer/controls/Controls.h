#ifndef __CONTROLS_H__
#define __CONTROLS_H__

class Control {
public:
	Control(const int irow, const int icol, const CString & desc, 
		CWnd *cwdctrl=NULL, CWnd*cwndlabel=NULL);
	void setRects() ;
	void dump(CString txt);
	CString m_desc;
	int row;
	int col;
	int hpos; // -1=left just, 0=center, 1=right just, other=do nothing
	CWnd * ctrl;
	CWnd * label;
	int width;
	int height;
	double widthpct;
	double heightpct;
	int labelwidth;
	int labelheight;
	int enabled;
	CRect rectctrl;
	CRect rectlabel;
	CRect rectall;
	int x,y;
};
class Controls {
public:
	Controls();
	void init(CWnd * w);
	~Controls();
	void FreeEm();
	CMapWordToPtr m_controls;
	void add(const int row, const int col, const CString & desc, 
			int id, int label=0);
	void remove(int id);
	Control * getObj(int id);
	Control * getObj(int row, int col);
	// x, y must be dialog client coordinates
	void move(Control * cp, const int x, const int y, const int row=-1,
			const int col=-1);
	BOOL CanMove(Control * cp, const int cx, const int cy);
	void findPos(const CPoint & moveby, Control * cp, 
			int & cx, int & cy,int,int);
	void MoveEmOffScreen();
	void MoveToBottom(Control *cp);
	void makeClientCoords(int & x, int & y);
	CRect dialogrect;
	CRect dialogrectreduced;
	int numRows;
	void movem();
private:
	CWnd * m_cwnd;
	int m_RowMaxY[50][50];
	int m_RowTop[50];
	int m_NumControls;
	HDWP m_HDWPHandle;


};

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


#endif