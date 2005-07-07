
#include "StdAfx.h"


#include "Misc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString MBFormatError(LONG e) {
	char msgBuf[ 512 ];
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, e, 0, msgBuf, sizeof( msgBuf ) / sizeof( char ), NULL );
	CString msg(msgBuf);
	return msg;
}


CPoint crtopright(CRect & rect) {
	CPoint p;
	p.x = rect.right;
	p.y = rect.top;
	return p;
}
CPoint crtopleft(CRect & rect) {
	CPoint p(rect.TopLeft());
	return p;
}
CPoint crbottomright(CRect & rect) {
	CPoint p(rect.BottomRight());
	return p;
}
CPoint crbottomleft(CRect & rect) {
	CPoint p;
	p.x = rect.left;
	p.y = rect.bottom;
	return p;
}