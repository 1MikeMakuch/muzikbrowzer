#ifndef _MP3HEADER_H_
#define _MP3HEADER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


struct structMp3Header  
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

class Mp3Header  
{
public:
	BOOL mb_getFilePath(const char* file);
	Mp3Header();
	virtual ~Mp3Header();
	unsigned long getHeaderAsBits(const char* header);
	int getNumberOfFrames();
	int iVbrframes;
	BOOL bIsVBitRate;
	char m_vbrheaderchars[13];
	BOOL testVBRHeader(char vbrheader [13]);
	int getFrameLength();
	structMp3Header m_structMp3Header;
	structMp3Header getHeaderInfoStruct();
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
#endif
