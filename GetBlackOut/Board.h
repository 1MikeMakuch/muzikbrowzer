#ifndef _BOARD_H_
#define _BOARD_H_
#include "stdafx.h"
class Piece
{
public:
	Piece();
	~Piece();
public:
	void height(int x) { m_height = x; };
	int height() { return m_height; };
	void width(int x) { m_width = x; };
	int width() { return m_width; };
	void xpos(int x) { m_xpos = x; };
	void xmov(int x) { m_xpos += x; };
	int xpos() { return m_xpos; };
	void ypos(int y) { m_ypos = y; };
	void ymov(int y) { m_ypos += y; };
	int ypos() { return m_ypos; };
	void mov(int x, int y) { xmov(x);ymov(y);};

	CRect mPieceRect;

private:
	int m_height;
	int m_width;
	int m_xpos;
	int m_ypos;

};

class Board
{
public:
	Board();
	~Board();
public:

	int pper() { return 40; };
	int pborder() { return 5; };
	int pwidth() { return ((4 * (pper() + pborder())) + (pborder()*2)); };
	int pheight() { return ((5 * (pper() + pborder())) + (pborder())*2);};
//	int pwidth() { return 600; };
//	int pheight() { return 600; };

	int numpieces() { return 10; };
	int xmax() { return 3; };
	int xsize() { return xmax()+1; };
	int ymax() { return 4; };
	int ysize() { return ymax()+1; };
	BOOL win() {
		if (piece(1)->xpos() == 1 && piece(1)->ypos() == 3) {
			mWon = TRUE;
			return TRUE;
		} else {
			return FALSE;
		}
	}

	BOOL canmove(int piece, int x, int y);

	Piece * piece(int x);
	CRect mBoardRect;
	void move(int piece, int x, int y);
	void init();
	BOOL mWon;

private:
	Piece * mPieces[10];
	
	

};

#endif

