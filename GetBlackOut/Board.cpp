
#include "Board.h"

Piece::Piece()
{
	
}

Piece::~Piece() {}

Board::~Board() {
	int i;
	for(i = 0 ; i < numpieces(); i++) {
		delete mPieces[i];
	}
}
Piece * Board::piece(int x) 
{ 
	if (x < 0 || x > 9) {
		return NULL;
	} else {
		return mPieces[x];
	}
}

#define START_FINAL_POS
#undef START_FINAL_POS

void Board::init() {

	mPieces[0]->height(2);
	mPieces[0]->width(1);
	mPieces[0]->xpos(0);
	mPieces[0]->ypos(0);

	mPieces[1]->height(2);
	mPieces[1]->width(2);
#ifdef START_FINAL_POS
	mPieces[1]->xpos(1);
	mPieces[1]->ypos(2);
#else
	mPieces[1]->xpos(1);
	mPieces[1]->ypos(0);
#endif
	mPieces[2]->height(2);
	mPieces[2]->width(1);
	mPieces[2]->xpos(3);
	mPieces[2]->ypos(0);

	mPieces[3]->height(2);
	mPieces[3]->width(1);
	mPieces[3]->xpos(0);
	mPieces[3]->ypos(2);

	mPieces[4]->height(1);
	mPieces[4]->width(2);
#ifdef START_FINAL_POS
	mPieces[4]->xpos(1);
	mPieces[4]->ypos(0);
#else
	mPieces[4]->xpos(1);
	mPieces[4]->ypos(2);
#endif
	mPieces[5]->height(2);
	mPieces[5]->width(1);
	mPieces[5]->xpos(3);
	mPieces[5]->ypos(2);

	mPieces[6]->height(1);
	mPieces[6]->width(1);
#ifdef START_FINAL_POS
	mPieces[6]->xpos(1);
	mPieces[6]->ypos(1);
#else
	mPieces[6]->xpos(1);
	mPieces[6]->ypos(3);
#endif
	mPieces[7]->height(1);
	mPieces[7]->width(1);
#ifdef START_FINAL_POS
	mPieces[7]->xpos(2);
	mPieces[7]->ypos(1);
#else
	mPieces[7]->xpos(2);
	mPieces[7]->ypos(3);
#endif
	mPieces[8]->height(1);
	mPieces[8]->width(1);
	mPieces[8]->xpos(0);
	mPieces[8]->ypos(4);

	mPieces[9]->height(1);
	mPieces[9]->width(1);
	mPieces[9]->xpos(3);
	mPieces[9]->ypos(4);



}
Board::Board() : mWon(FALSE) {
	mPieces[0] = new Piece;
	mPieces[1] = new Piece;
	mPieces[2] = new Piece;
	mPieces[3] = new Piece;
	mPieces[4] = new Piece;
	mPieces[5] = new Piece;
	mPieces[6] = new Piece;
	mPieces[8] = new Piece;
	mPieces[7] = new Piece;
	mPieces[9] = new Piece;
	init();

}
	
BOOL Board::canmove(int ipiece, int x, int y)
{
	if (x == 0 && y == 0) { return FALSE; }
	CRect candrect = piece(ipiece)->mPieceRect;
	candrect.OffsetRect(x,y);
	CRect intersection;
	// 1st check if moved piece is still on the board
	if (intersection.IntersectRect(mBoardRect, candrect)) {
		if (intersection != candrect) {
			return FALSE;
		}
	} else {
		return FALSE;
	}

	// now see if it's moving to a free space

	int i;
	for(i = 0 ; i < numpieces(); i++) {
		if (i != ipiece) {
			CRect prect = piece(i)->mPieceRect;
			BOOL r = intersection.IntersectRect(candrect, prect);
			if (r != 0) {
				return FALSE;
			}
		}
	}
	return TRUE;
}
void Board::move(int ipiece, int x, int y)
{
	if (x > 0) x = 1;
	if (x < 0) x = -1;
	if (y > 0) y = 1;
	if (y < 0) y = -1;

	if (canmove(ipiece, (x * pper()) + pborder(), (y*pper())+pborder())
		|| mWon)
		piece(ipiece)->mov(x,y);

}
