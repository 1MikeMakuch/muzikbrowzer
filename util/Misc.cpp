
#include "StdAfx.h"
#include "Misc.h"
#include <stack>
using namespace std;
#include "ConfigFileLexer.h"
#include "TestHarness.h"
#include "MyLog.h"
#include "FileUtils.h"
#include "resource.h"
#include "Process.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString MBFormatErrorOld(LONG e) {
	char msgBuf[ 512 ];
	FormatMessage( FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, e, 0, msgBuf, sizeof( msgBuf ) / sizeof( char ), NULL );
	CString msg(msgBuf);
	return msg;
}
CString MBFormatError(LONG err)
{
	LPTSTR s;
	if(::FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER 
		| FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,err,0,(LPTSTR)&s,0,NULL) == 0)
	{ /* failed */
		// See if it is a known error code
		CString fmt;                          
		fmt.LoadString(IDS_UNKNOWN_ERROR);    
		CString t;
		t.Format(fmt, err, err);
		return t;
	} /* failed */ 	else { /* success */
		LPTSTR p = _tcschr(s, _T('\r'));
		if(p != NULL)
		{ /* lose CRLF */
			*p = _T('\0');
		} /* lose CRLF */

		CString Error = s; // copy to a CString
		::LocalFree(s);
		return Error;
	} /* success */
} // cvError



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
CString CRect2String(const CRect & rect) {
	CString m;
	AutoBuf buf(1000);
	sprintf(buf.p,"L:%4d T:%4d W:%4d H:%4d",
		rect.left,rect.top,rect.Width(),rect.Height());
	m = buf.p;
	return m;
}
CString RGB2CString(const COLORREF c) {
	CString cs = numToString(GetRValue(c));
	cs += "," + numToString(GetGValue(c));
	cs += "," + numToString(GetBValue(c));
	return cs;
}
void CRectMove(CRect & rect, const int x, const int y) {
	int w,h;
	w = rect.Width();
	h = rect.Height();
	rect.top = y;
	rect.bottom = y + h;
	rect.left = x;
	rect.right = x + w;
}
void CRectWidth(CRect & rect, int newwidth) {
	rect.right = rect.left + newwidth;
}
void CRectHeight(CRect & rect, int newheight) {
	rect.bottom = rect.top + newheight;
}

const char * val10 = "0	 !\"$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";
const char * val11 = "100 , 100 , 100";

TEST(stacktest,stacktest)
{
	stack<int> istack;
	istack.push(0);
	istack.push(1);
	istack.push(2);

	int i = istack.top();
}

TEST(ConfigFileParser2, ParseTest2)
{
	stack<KVPair> kvstack;
	char * file = "..\\testdata\\ConfigFileParser.testdat";
	ConfigFileParser(file, &kvstack);
	AutoBuf buf(1000);

	logger.ods("ConfigFileParserTest2");

	int i;
	AutoBuf keyb(1000);
	AutoBuf valb(1000);
	AutoBuf lbuf(1000);
	int result;
	
	CHECK(kvstack.empty() != TRUE);
	if (kvstack.empty())
		return;

	const char * k = kvstack.top().key();
	const char * v = kvstack.top().val();

	CHECK(strcmp(k,"key11") == 0);
	result = strcmp(v,val11);
	CHECK(0 == result);

	kvstack.pop();

	const char * k2 = kvstack.top().key();
	const char * v2 = kvstack.top().val();
	CHECK(strcmp(k2,"key10") == 0);
	result = strcmp(v2,val10) == 0;
	CHECK(0 == result);
	kvstack.pop();
	for (i = 9 ; i > 0 ; i--) {
		CHECK(kvstack.empty() == FALSE);
		sprintf(keyb.p,"key%d",i);
		sprintf(valb.p,"val%d",i);
		const char * k = kvstack.top().key();
		const char * v = kvstack.top().val();
		sprintf(lbuf.p,"[%s]=[%s]",k,v);
		logger.ods(lbuf.p);
		result = strcmp(keyb.p,k);
		CHECK(0 == result);

		result = strcmp(valb.p,v);
		CHECK(0 == result);

		//free(k);free(v);
		kvstack.pop();

	}
	CHECK(kvstack.empty() == TRUE);
}
static unsigned int italic;
static unsigned int underline;
static unsigned int strikeout;
static unsigned int charset;
static unsigned int outprecision;
static unsigned int clipprecision;
static unsigned int quality;
static unsigned int pitchandfamily;
void
MBUtil::FontStr2LogFont(const char * fontstr, LPLOGFONT lplf) {
    sscanf(fontstr, MBCCFONTFMT, 
		&lplf->lfHeight, 
		&lplf->lfWidth, 
		&lplf->lfEscapement, 
		&lplf->lfOrientation,
        &lplf->lfWeight, 
		&italic, 
		&underline, 
		&strikeout, 
		&charset,
        &outprecision, 
		&clipprecision, 
		&quality, 
		&pitchandfamily);

    lplf->lfItalic = italic; 
	lplf->lfUnderline = underline;
    lplf->lfStrikeOut = strikeout; 
	lplf->lfCharSet = charset;
    lplf->lfOutPrecision = outprecision;
	lplf->lfClipPrecision = clipprecision;
    lplf->lfQuality = quality;
	lplf->lfPitchAndFamily = pitchandfamily;

    strcpy(lplf->lfFaceName, fontstr+MBCCFONTFACEPOS);
}
CString MBUtil::LogFont2FontStr(const LPLOGFONT lplf) {
    AutoBuf buf(1000);
    sprintf(buf.p, MBCCFONTFMT,
        lplf->lfHeight, lplf->lfWidth, lplf->lfEscapement, lplf->lfOrientation,
        lplf->lfWeight, lplf->lfItalic, lplf->lfUnderline, lplf->lfStrikeOut,
        lplf->lfCharSet, lplf->lfOutPrecision, lplf->lfClipPrecision,
        lplf->lfQuality, lplf->lfPitchAndFamily);
    char * p = buf.p + strlen(buf.p);
    sprintf(p, "%s", lplf->lfFaceName);
	return CString(buf.p);
}


// Validate font spec from config file
BOOL
MBUtil::ConfigFontValidate(const char * val)
{
	return ConfigFontParser(val);
}
TEST(ConfigFontParser, parsetest)
{

	char * font1 = "0";
	BOOL r = MBUtil::ConfigFontValidate(font1);
	CHECK(r == FALSE);
	char * font2 = "FFFFFEC0 00000000 00000000 00000000 000002BC 00 00 00 00 00 00 00 00 Arial";
	r = MBUtil::ConfigFontValidate(font2);
	CHECK(r == TRUE);

}
CString
MBUtil::CrToRGB(COLORREF rgb) {
	CString srgb  = numToString(GetRValue(rgb)) + ","
		+ numToString(GetGValue(rgb)) + ","
		+ numToString(GetBValue(rgb));
	return srgb;
}

// get the RGB value from a string like "100,100,100"
BOOL
MBUtil::RgbTriple(const char * vali, unsigned long & rgb) 
{
	AutoBuf val(strlen(vali)+1);
	strcpy(val.p, vali);
	char * pr,*pg,*pb;
	pr = pg = pb = NULL;

	pr = strtok(val.p,", ");
	if (NULL == pr)	return FALSE;
	pg = strtok(NULL,", ");
	if (NULL == pg)	return FALSE;
	pb = strtok(NULL,", ");
	if (NULL == pb)	return FALSE;

	int r,g,b;
	r = g = b = 0;
	r = atoi(pr); g = atoi(pg) ; b = atoi(pb);

	rgb = RGB(r,g,b);
	return TRUE;
}

TEST(MBUtilRgbTriple, test1)
{
	char *buf1 = "10,20,30";

	unsigned long rgb,rgb2;
	BOOL r = MBUtil::RgbTriple(buf1, rgb);
	CHECK(TRUE == r);
	CHECK(1971210 == rgb);

	char *buf2= "";
	r = MBUtil::RgbTriple(buf2, rgb);
	CHECK(FALSE == r);

	char *buf3 = ",";
	r = MBUtil::RgbTriple(buf3, rgb);
	CHECK(FALSE == r);

	char *buf4 = "0,0,";
	r = MBUtil::RgbTriple(buf4, rgb);
	CHECK(FALSE == r);

	char * buf5 = " 1 , 1 , 1 ";
	r = MBUtil::RgbTriple(buf5, rgb);

	char * buf6 = "1,1,1";
	r = MBUtil::RgbTriple(buf6, rgb2);
	CHECK(rgb == rgb2);

	char * buf7 = " 1 1 1 ";
	r = MBUtil::RgbTriple(buf7, rgb2);
	CHECK(rgb == rgb2);

	char * buf8 = " 1  1  1  1 ";
	r = MBUtil::RgbTriple(buf8, rgb2);
	CHECK(rgb == rgb2);

}

void MBUtil::BmpToDC(CDC* pDC, HBITMAP bmp, 
	 const int dx, const int dy, const int dwidth, const int dheight,
	 const int swidth, const int sheight, const LayOutStyle los,
	 const BOOL doTrans, const COLORREF transcolor, const int offset)
{
//	FileUtil::BmpLog(bmp,"bmp");
	CRect rect(dx,dy,dx+dwidth,dy+dheight);
	BitmapToCRect bmcr(bmp, rect, los, swidth, sheight, "xxx");
	MBUtil::BmpToDC(pDC, &bmcr, doTrans, transcolor, offset);
}

void MBUtil::BmpToDC(CDC* pDC, BitmapToCRect * bmcr, 
						   BOOL doTrans, COLORREF bkcolor, int offset)
{
    if (!bmcr)
        return ;
	
    HBITMAP    pbmpOldBmp = NULL;
	HBITMAP hMask = NULL;
	UINT BITBLTTYPE = SRCCOPY;

	// offset of 0 is used for painting the main dialog background + the
	// resize border (2). offset of 2 is used for remainder of 
	// background panels so they line up properly. If using this func
	// for other use just leave offset at 0.
	int x,y,srcwidth,srcheight,destwidth,destheight;
	x = bmcr->m_rect.left+offset;
	y =  bmcr->m_rect.top+offset;
	
	// dst dimensions
	destwidth = bmcr->m_rect.Width();
	destheight = bmcr->m_rect.Height();
	
	// src dimensions
	srcwidth = bmcr->m_width;
	srcheight = bmcr->m_height;

	if (doTrans) { // do the transparent stuff
		hMask = MBUtil::CreateBitmapMask(bmcr->m_hBitmap, destwidth, destheight,
			srcwidth, srcheight, bmcr->m_loStyle,bkcolor);

//		if (1 == x)
//			FileUtil::BmpLog(hMask,"mask");
		BitmapToCRect bmcrTrans((HBITMAP)hMask, bmcr->m_rect, bmcr->m_loStyle, 
			srcwidth,srcheight);
		
		MBUtil::CutAndTileBmp(pDC,&bmcrTrans,offset,SRCAND);
		::DeleteObject(hMask);
		BITBLTTYPE = SRCPAINT;
	}
	MBUtil::CutAndTileBmp(pDC, bmcr, offset, BITBLTTYPE);
	

	
    return ;
}
//void MBUtil::CutAndTileBmp(CDC* pDC, HDC hdcSrc, HBITMAP bmp,
//		CRect & destrect, LayOutStyle los, 
//		int srcwidth, int srcheight,
//		int offset,UINT BITBLTTYPE ) {
//
//	BitmapToCRect bmcr(bmp, destrect, los, 
//		srcwidth,srcheight, CS("MBUtil::BmpToDC"), hdcSrc);
//	MBUtil::CutAndTileBmp(pDC, &bmcr, offset, BITBLTTYPE);
//}
static void tileloop(const int xoff,  const int yoff,
					 const int destwidth, const int destheight,
					 const int left, const int right,
					 const int top  , const int bottom,
					 const int width, const int height,
					 const int x_src, const int y_src,
					 CDC * dc, CDC * pDC, const DWORD rop,
					 int PaintEdges)
{
	int lwidth = width;
	int lheight = height;
	int trimWidth = width * PaintEdges;    // PaintEdges == 1 or 0
	int trimHeight = height * PaintEdges;
	int x,y;
	for (x = left; x < right ; x += width) {
		for (y = top; y < bottom; y += height) {
			if (x > right - width)
				lwidth =  ((xoff + destwidth) - trimWidth) - x;
			if (y > bottom - height)
				lheight = ((yoff + destheight) - trimHeight) - y;
			pDC->BitBlt(x,y, lwidth, lheight,
				dc, x_src, y_src, rop);
			lwidth = width; lheight = height;
		}
	}
}

#define TileLoop \
		tileloop(x,y,destwidth,destheight,\
			left,right,top,bottom,width,height,\
			x_src,y_src,dc,pDC,BITBLTTYPE, 0);

#define TileLoopNoEdges \
		tileloop(x,y,destwidth,destheight,\
			left,right,top,bottom,width,height,\
			x_src,y_src,dc,pDC,BITBLTTYPE, 1);

void MBUtil::CutAndTileBmp(CDC* pDC, BitmapToCRect * bmcr, 
						   int offset, UINT		BITBLTTYPE )
{
    CDC * dc = NULL;
	HBITMAP pbmpOldBmp = NULL;
	if (NULL != bmcr->m_dc) {
		dc = bmcr->m_dc;
	} else {
		dc = new CDC();
		dc->CreateCompatibleDC(NULL);
		pbmpOldBmp = (HBITMAP)::SelectObject(dc->m_hDC, bmcr->m_hBitmap);
	}

	int x,y,srcwidth,srcheight,destwidth,destheight;
	x = bmcr->m_rect.left+offset;
	y =  bmcr->m_rect.top+offset;
	
	// dst dimensions
	destwidth = bmcr->m_rect.Width();
	destheight = bmcr->m_rect.Height();
	
	// src dimensions
	srcwidth = bmcr->m_width;
	srcheight = bmcr->m_height;

	int x_src,y_src,x_dst,y_dst;
	int width,height;
	int thirdwidth = bmcr->m_width / 3;
	int thirdheight = bmcr->m_height / 3;
	if (thirdwidth > destwidth/3)
		thirdwidth = destwidth / 3;
	if (thirdheight > destheight/3)
		thirdheight = destheight / 3;

	x_src = y_src = x_dst = y_dst = 0;
	x_dst = x;
	y_dst = y;
	int left,right,top,bottom;

    if ( bmcr->m_loStyle == LO_FIXED )
    {
		pDC->BitBlt(bmcr->m_rect.left+offset, bmcr->m_rect.top+offset, 
				destwidth, destheight, dc, 0, 0, BITBLTTYPE);
    } else if (bmcr->m_loStyle == LO_TILED0) {
		left = x;
		right = x + destwidth;
		top = y;
		bottom = y + destheight;
		width = srcwidth;
		height = srcheight;
		TileLoop;
		
		left = x + width;
		right = x + destwidth;
		top = y + height;
		bottom = y + destheight;
		width = srcwidth;
		height = srcheight;
		TileLoop;
    } else if (bmcr->m_loStyle == LO_TILED) {
		width = thirdwidth;
		height = thirdheight;

//goto bottomedge;
		// middle
		x_src = thirdwidth;
		y_src = thirdheight;
		left = width + x;
		right = (destwidth - width) + x;
		top = height + y;
		bottom = (destheight - height) + y;
		TileLoopNoEdges;

		// top edge
		x_src = thirdwidth;
		y_src = 0;
		top = y;;
		bottom = top + height;
		TileLoopNoEdges;

		// right edge
		x_src = bmcr->m_width - thirdwidth;
		y_src = thirdheight;
		top = height + y;
		bottom = (destheight - height) + y;
		left = (destwidth - width) + x;
		right = destwidth + x;
		TileLoopNoEdges;
//bottomedge:
		// bottom edge
		x_src = width;
		y_src = bmcr->m_height - thirdheight;
		top = (destheight - height) + y;
		bottom = destheight + y;
		left = width + x;
		right = (destwidth - width) + x;
		TileLoopNoEdges;
//goto bottomcorners;
		// left edge
		x_src = 0;
		y_src = thirdheight;
		top = height + y;
		bottom = (destheight - height) + y;
		left = x;
		right = width + x;
		TileLoopNoEdges;


		// upper left
		x_src = y_src = x_dst = y_dst = 0;
		x_dst = x;
		y_dst = y;
		width = width = thirdwidth;
		height = height = thirdheight;
		pDC->BitBlt(x_dst,y_dst, width, height, dc, x_src, y_src, BITBLTTYPE);

		// upper right
		x_src = bmcr->m_width - thirdwidth;
		x_dst = (destwidth - thirdwidth)+x;
		pDC->BitBlt(x_dst,y_dst, width, height, dc, x_src, y_src, BITBLTTYPE);
//bottomcorners:
		// lower left
		x_src = 0;
		y_src = bmcr->m_height - thirdheight;
		x_dst = x;
		y_dst = (destheight - thirdheight)+y;
		pDC->BitBlt(x_dst,y_dst, width, height, dc, x_src, y_src, BITBLTTYPE);

		// lower right
		x_src = bmcr->m_width - thirdwidth;
		x_dst = (destwidth - thirdwidth)+x;
		y_dst = (destheight - thirdheight)+y;
		pDC->BitBlt(x_dst,y_dst, width, height, dc, x_src, y_src, BITBLTTYPE);
	}
    else if (bmcr->m_loStyle == LO_CENTER)
    {
        int ixOrg = x + (destwidth - bmcr->m_width) / 2;
        int iyOrg = y + (destheight - bmcr->m_height) / 2;
        pDC->BitBlt(ixOrg+offset, iyOrg+offset, destwidth, destheight, dc, 0, 0, BITBLTTYPE);
    }
    else if ( bmcr->m_loStyle == LO_STRETCHED)
    {
        pDC->StretchBlt(x, y, destwidth, destheight, 
			dc, 0, 0, srcwidth, srcheight, BITBLTTYPE);
    }
//quitit:
	if (NULL == bmcr->m_dc) {
		::SelectObject(dc->m_hDC, pbmpOldBmp);
		dc->DeleteDC();
		delete dc;
	}
}


HBITMAP MBUtil::CreateBitmapMask(HBITMAP hSourceBitmap, 
				DWORD destwidth, DWORD destheight,
				DWORD srcwidth, DWORD srcheight,
				LayOutStyle los,
				COLORREF bkcolor)
{
	HBITMAP		hMask		= NULL;
	CDC			dcSrc;
	CDC			dcDest;
	CDC			dcTmp;
	HBITMAP		hbmSrcT		= NULL;
	HBITMAP		hbmDestT	= NULL;
	HBITMAP		hbmNewSrc	= NULL;
	HBITMAP		hbmTmp		= NULL;
	COLORREF	crSaveBk;
	COLORREF	crSaveDestText;

	CDC dc;
	dc.CreateCompatibleDC(NULL);

	hMask = ::CreateCompatibleBitmap(dc.m_hDC, srcwidth,srcheight);
	if (hMask == NULL)	return NULL;

	dcSrc.CreateCompatibleDC(NULL);
	dcDest.CreateCompatibleDC(NULL);

	hbmSrcT = (HBITMAP)dcSrc.SelectObject(hSourceBitmap);
	hbmDestT = (HBITMAP)dcDest.SelectObject(hMask);

	crSaveBk = dcSrc.SetBkColor(bkcolor);

	dcDest.BitBlt(0,0,srcwidth,srcheight,&dcSrc,0,0,SRCCOPY);

	crSaveDestText = dcSrc.SetTextColor(RGB(255, 255, 255));
	dcSrc.SetBkColor(RGB(0, 0, 0));

	dcSrc.BitBlt(0,0,srcwidth,srcheight,&dcDest,0,0,SRCAND);

	dcDest.SetTextColor(crSaveDestText);

	dcSrc.SetBkColor(crSaveBk);
	dcSrc.SelectObject(hbmSrcT);
	dcDest.SelectObject(hbmDestT);

	return hMask;
} // End of CreateBitmapMask

void
MBUtil::SecsToHMS(const int duration, int & hours, int & mins, int & secs,
				  char * buf)
{
	hours = duration / 3600;
	secs = duration % 3600;
	mins = secs / 60;
	secs = secs % 60;
	sprintf(buf,"%02d:%02d:%02d",hours,mins,secs);
}
BOOL MBUtil::system(CWnd * cwnd, const CString & command, UINT msg2post) {
#ifdef _DEBUG
//	logger.ods("MBUtil::system begin");

	CString scriptname = "c:\\tmp\\MBUtilsystem_script.sh";
	CFile script;
	CFileException fileException;
	if ( !script.Open( scriptname,
        CFile::modeWrite | CFile::modeCreate | CFile::shareDenyNone,
        &fileException ))
	{
        CString msg = "Unable to write script ";
		msg += scriptname;
			msg += "\r\n";
		msg += MBFormatError(fileException.m_lOsError);
//		logger.ods("MBUtil::system error");
//		logger.ods(msg);
        return FALSE;
	}
	

	CString tmp ;

	tmp += command;
	logger.logd(tmp);

    script.Write((LPCTSTR)tmp, tmp.GetLength());
	script.Write("\n", 1);
    script.Flush();
	script.Close();

	CString pcommand = "c:\\cygwin\\bin\\bash.exe --login ";
	pcommand += "c:\\tmp\\MBUtilsystem_script.sh";

     Process * p = new Process(pcommand, cwnd);
     if(!p->run())
	 { /* failed */
		DWORD err = ::GetLastError();
		CString errstr = MBFormatError(err);
		logger.logd(errstr);
//		logger.ods("MBUtil::system error");
		return FALSE;
	 }
//	 logger.ods("MBUtil::system done");

#endif
	return TRUE;
}
void
MBUtil::MemDump(const char *p, const UINT length) {
	AutoBuf buf(4 * length);
	char * bp = buf.p;
	for(int i = 0 ; i < length; i++) {
		sprintf(bp,"%02x ",p[i]);
		bp += 3;
	}
	logger.logd(buf.p);
}
#ifdef AintUsinIt_DEBUG
static MBAutoTimerI MBAUTOTIMER;

MBAutoTimer::MBAutoTimer(const CString & id):m_id(id) {
//	_ftime(&m_beg);
	QueryPerformanceCounter(&m_beg);
}
MBAutoTimer::~MBAutoTimer() {
	//_ftime(&m_end);
	QueryPerformanceCounter(&m_dur);

	m_dur.QuadPart -= m_beg.QuadPart;
//	m_dur.QuadPart *= 1000 ; 

	MBAUTOTIMER.add(this);
}
MBAutoTimerI::MBAutoTimerI(){}
MBAutoTimerI::~MBAutoTimerI()
{
	AutoBuf buf(500);
	CString id;
	MBAutoTimerCtr * p;
	LARGE_INTEGER liFreq;
	POSITION pos;

	QueryPerformanceFrequency(&liFreq);

	CStringList idlist;

	for (pos = MBAUTOTIMER.m_MBAutoTimerHash.GetStartPosition(); 
			pos != NULL;) {
		MBAUTOTIMER.m_MBAutoTimerHash.GetNextAssoc(pos,id,(void *&)p);
		String::insertSort(idlist,id);
	}
	sprintf(buf.p,"MBAutoTimer:%-30s %-6s %-12s %-12s", 
		"Method","calls","ticks","ticks/call");
	logger.ods(buf.p);
	for (pos = idlist.GetHeadPosition(); pos != NULL;) {
		CString id = idlist.GetAt(pos);
		if (MBAUTOTIMER.m_MBAutoTimerHash.Lookup(id,(void *&)p)) {
			if (p) {
//				duration.QuadPart = 
//					(10000 * p->duration.QuadPart) / liFreq.QuadPart;
				ULONG tpc = 
					p->duration.QuadPart / p->counter;
				ULONG d = p->duration.QuadPart;

				sprintf(buf.p,"MBAutoTimer:%-30s %-6d %-12d %-12d", 
					id,
					p->counter,
					d,
					tpc);
				logger.ods(buf.p);
				delete p;
			}
		}
		idlist.GetNext(pos);
	}
}
void
MBAutoTimerI::add(const MBAutoTimer * timer)
{
	MBAutoTimerCtr * p;

	if (m_MBAutoTimerHash.Lookup(timer->m_id, (void *&)p)) {
		p->counter++;
		p->duration.QuadPart += timer->m_dur.QuadPart;
	} else {
		p = new MBAutoTimerCtr ;
		p->counter = 1;
		p->duration.QuadPart = timer->m_dur.QuadPart;
		m_MBAutoTimerHash.SetAt(timer->m_id,p);
	}
}

#ifdef asdf
TEST(MBAutoTimer, test)
{
	AutoBuf buf(10);
	int sleep = 50;
	for (int i = 0 ; i < 11 ; ++i) {
		sprintf(buf.p,"%s%03d","test ",i);
		MBAutoTimer t(buf.p);
		Sleep(i * sleep);
	}
	for (i = 0 ; i < 11 ; ++i) {
		sprintf(buf.p,"%s%03d","test ",i);
		MBAutoTimer t(buf.p);
		Sleep(i * sleep);
	}
}
#endif
#else // if _DEBUG
MBAutoTimer::MBAutoTimer(const CString & id):m_id(id){}
MBAutoTimer::~MBAutoTimer() {}
MBAutoTimerI::MBAutoTimerI(){}
MBAutoTimerI::~MBAutoTimerI(){}
void MBAutoTimerI::add(const MBAutoTimer * timer){}

#endif