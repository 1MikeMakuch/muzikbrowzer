// structMpgHeader.h: interface for the structMpgHeader class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRUCTMPGHEADER_H__E0CA17CA_54DA_4E44_8ED5_7D28467DE300__INCLUDED_)
#define AFX_STRUCTMPGHEADER_H__E0CA17CA_54DA_4E44_8ED5_7D28467DE300__INCLUDED_

#include "stdafx.h"

struct structMpgHeader  
{
	long lSize;
	long lHeaderPosition;
	int ilength;
	float fVersion;
	int iLayer;
	int iBitrate;
	int iFrames;
	int iFrequency;
	CString szMode;
	BOOL bCRC;
	BOOL bCopyright;
	BOOL bOriginal;
	CString szEmphasis;
	BOOL bVBRHeader;
	BOOL bNoHeader;
};


#endif // !defined(AFX_STRUCTMPGHEADER_H__E0CA17CA_54DA_4E44_8ED5_7D28467DE300__INCLUDED_)
