#ifndef _MISC_H_
#define _MISC_H_

#include "stdafx.h"

CString MBFormatError(LONG e);

CPoint crtopright(CRect & rect);
CPoint crtopleft(CRect & rect);
CPoint crbottomright(CRect & rect);
CPoint crbottomleft(CRect & rect);

class CObjectInt : public CObject
{
	public:
		CObjectInt(int v = 0) { m_int = v; }
		int m_int;
};

class MBUtil {
public:
	static BOOL RgbTriple(const char * val, unsigned long & rgb);
	static BOOL ConfigFontValidate(const char * font);
	static CString CrToRGB(COLORREF rgb);
}	;

#endif
