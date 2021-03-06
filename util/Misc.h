#ifndef _MISC_H_
#define _MISC_H_

#include "stdafx.h"
#include "ControlSK.h"
#include <sys/types.h>
#include <sys/timeb.h>

CString MBFormatError(LONG e);

CPoint crtopright(CRect & rect);
CPoint crtopleft(CRect & rect);
CPoint crbottomright(CRect & rect);
CPoint crbottomleft(CRect & rect);
void CRectMove(CRect & rect, const int x, const int y);
void CRectWidth(CRect & rect, int newwidth);
void CRectHeight(CRect & rect, int newheight);
CString CRect2String(const CRect & rect);
CString RGB2CString(const COLORREF c);
class CObjectInt : public CObject
{
	public:
		CObjectInt(int v = 0) { m_int = v; }
		int m_int;
};
class BitmapToCRect
{
public:
	BitmapToCRect(HBITMAP bm, CRect & rect, LayOutStyle los, 
		DWORD width, DWORD height, CString desc="", HDC hdc=NULL)
		
		: m_hBitmap(bm),m_desc(desc),
		m_rect(rect), m_loStyle(los), m_width(width), m_height(height),
		m_dc(NULL), m_hMask(NULL)
	{
		if (hdc) {
			m_dc = new CDC();
			m_dc->Attach(hdc);
		}

	};
	~BitmapToCRect() {
		// don't delete m_hBitmap nor m_hMask here!
		if (m_dc) {
			m_dc->Detach();
			delete m_dc;
		}
	};

	HBITMAP m_hBitmap;
	HBITMAP m_hMask;
	CRect   m_rect;
	LayOutStyle m_loStyle;
	DWORD m_width;
	DWORD m_height;
	CDC * m_dc;
	CString m_desc;

};

#define MBCCFONTFMT "%08X %08X %08X %08X %08X %02X %02X %02X %02X %02X %02X %02X %02X "
#define MBCCFONTFACEPOS 69
#define MBdeffont "FFFFFFF4 00000000 00000000 00000000 000002BC 00 00 00 00 00 00 00 00 Arial"
#define MBdeffontfacename "Arial"
#define MBFontErrorMsg(_MBFONTSPEC_) "Error in SkinDefinition setting:" + CS(_MBFONTSPEC_) + "\r\n\r\nGo to Settings/Display, make a change and click\r\nOK or Apply to create a new SkinDefintion file."

class MBUtil {
public:
	static BOOL RgbTriple(const char * val, unsigned long & rgb);
	static BOOL ConfigFontValidate(const char * font);
	static CString CrToRGB(COLORREF rgb);
	static void BmpToDC(CDC* pDC, BitmapToCRect * bmcr, 
		BOOL doTrans, COLORREF bkcolor,int offset);

	static void BmpToDC(CDC* pDC, HBITMAP bmp, 
	 const int dx, const int dy, const int dwidth, const int dheight,
	 const int swidth, const int sheight, const LayOutStyle los,
	 const BOOL doTrans, const COLORREF transcolor, const int offset);
	
	static void CutAndTileBmp(CDC* pDC, BitmapToCRect * bmcr, 
		int offset,UINT BITBLTTYPE );

//	static void CutAndTileBmp(CDC* pDC, HDC hdcSrc, HBITMAP bmp,
//		CRect & destrect, LayOutStyle los, int srcwidth, int srcheight,
//		int offset,UINT BITBLTTYPE );
	
	static HBITMAP CreateBitmapMask(HBITMAP hSourceBitmap,
				DWORD dwidth, DWORD dheight,
				DWORD swidth, DWORD sheight,
				LayOutStyle los,COLORREF bkcolor);

	static void SecsToHMS(const int seconds, int & hours, int & mins, 
		int & secs,	char * buf);
	static BOOL system(CWnd * cwnd, const CString & command, UINT msg=0);
	static void FontStr2LogFont(const char * fontstr, LPLOGFONT lplf);
	static CString LogFont2FontStr(const LPLOGFONT lplf);
	static void MemDump(const char * p, const UINT length);

};


struct MBAutoTimerCtrS {
	int counter;
	LARGE_INTEGER duration;
};
typedef MBAutoTimerCtrS MBAutoTimerCtr;

class MBAutoTimer {
public:
	MBAutoTimer(const CString & id);
	~MBAutoTimer();
protected:
	CString m_id;
	LARGE_INTEGER m_beg;
	LARGE_INTEGER m_dur;
	friend class MBAutoTimerI;
};

class MBAutoTimerI {
public:
	MBAutoTimerI();
	~MBAutoTimerI();
	void add(const MBAutoTimer * timer);
private:
	CMapStringToPtr m_MBAutoTimerHash;	
};


#define STC(_STC_ARG_) ScreenToClient(_STC_ARG_);
#define CTS(_STC_ARG_) ClientToScreen(_STC_ARG_);
#endif
