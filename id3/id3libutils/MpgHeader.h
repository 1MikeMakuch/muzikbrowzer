////////////////////////////////////////////////////////////////////////////////
//CMpgHeader.h
/* CMpgHeader Copyright(C) 2001 John George K., xeonfusion@users.sourceforge.net
based on the CFrameHeader class Copyright (C) Gustav "Grim Reaper" Munkby  http://home.swipnet.se/grd/  email: grd@swipnet.se
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MPGHEADER_H__7BB08D7E_A31F_440E_8610_797424D4E9F7__INCLUDED_)
#define AFX_MPGHEADER_H__7BB08D7E_A31F_440E_8610_797424D4E9F7__INCLUDED_


//#define BOOL bool
//#define TRUE true
//#define FALSE false

#include "structMpgHeader.h"

class CMpgHeader  
{
public:
	BOOL getFilePath(const char* file);
	CMpgHeader();
	virtual ~CMpgHeader();
	unsigned long getHeaderAsBits(const char* header);
	int getNumberOfFrames();
	int iVbrframes;
	BOOL bIsVBitRate;
	char m_vbrheaderchars[13];
	BOOL testVBRHeader(char vbrheader [13]);
	int getFrameLength();
	structMpgHeader m_structmpgheader;
	structMpgHeader getHeaderInfoStruct();
	long headerpos;
	const char* m_filepath;
	int getLengthInSeconds();
	CString emphasisstring;
	CString getEmphasis();
	BOOL getOriginal();
	BOOL getCopyright();
	BOOL getProtection();
	long getSize();
	CString modestring;
	float getVersion();
	int getLayer();
	int getBitrate();
	int getFrequency();
	CString getMode();
	unsigned long headerbits;
	BOOL getValidHeader(const char* file);
	
		
	int getFrameSyncBits()  { return ((headerbits>>21) & 2047); };
    int getVersionBits()	{ return ((headerbits>>19) & 3);  };
    int getLayerBits()		{ return ((headerbits>>17) & 3);  };
    int getProtectionBits()	{ return ((headerbits>>16) & 1);  };
    int getBitrateBits()	{ return ((headerbits>>12) & 15); };
    int getFrequencyBits()	{ return ((headerbits>>10) & 3);  };
    int getPaddingBits()    { return ((headerbits>> 9) & 1);  };
    int getPrivateBits()    { return ((headerbits>> 8) & 1);  };
    int getModeBits()		{ return ((headerbits>> 6) & 3);  };
    int getModeExtBits()	{ return ((headerbits>> 4) & 3);  };
    int getCopyrightBits()  { return ((headerbits>> 3) & 1);  };
    int getOriginalBits()   { return ((headerbits>> 2) & 1);  };
    int getEmphasisBits()	{ return ((headerbits    ) & 3);  };

	BOOL testValidHeader(const char* header);

};

#endif // !defined(AFX_MPGHEADER_H__7BB08D7E_A31F_440E_8610_797424D4E9F7__INCLUDED_)
