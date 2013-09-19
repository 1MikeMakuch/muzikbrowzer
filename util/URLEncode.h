/*****************************************************************************
Module :     URLEncode.H
Notices:     Written 2002 by ChandraSekar Vuppalapati
Description: H URL Encoder
*****************************************************************************/
#ifndef __CURLENCODE_H_
#define __CURLENCODE_H_

#include "stdafx.h"

class CURLEncode
{
private:
	static CString csUnsafeString;
	CString decToHex(char num, int radix);
	char hexToCh(char val[]);
	bool isUnsafe(char compareChar);
	CString convert(char val);
	CString unconvert(char val[]);

public:
	CURLEncode() { };
	virtual ~CURLEncode() { };
	CString URLEncode(const CString & vData);
	CString URLDecode(const CString & vData);
};

#endif //__CURLENCODE_H_